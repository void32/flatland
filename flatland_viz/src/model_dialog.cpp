/*
 *  ______                   __  __              __
 * /\  _  \           __    /\ \/\ \            /\ \__
 * \ \ \L\ \  __  __ /\_\   \_\ \ \ \____    ___\ \ ,_\   ____
 *  \ \  __ \/\ \/\ \\/\ \  /'_` \ \ '__`\  / __`\ \ \/  /',__\
 *   \ \ \/\ \ \ \_/ |\ \ \/\ \L\ \ \ \L\ \/\ \L\ \ \ \_/\__, `\
 *    \ \_\ \_\ \___/  \ \_\ \___,_\ \_,__/\ \____/\ \__\/\____/
 *     \/_/\/_/\/__/    \/_/\/__,_ /\/___/  \/___/  \/__/\/___/
 * @copyright Copyright 2017 Avidbots Corp.
 * @name   model_dialog.cpp
 * @brief  Main window and toolbars for flatland_viz
 * @author Joseph Duchesne
 * @author Mike Brousseau
 *
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2017, Avidbots Corp.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the Avidbots Corp. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#include <QColorDialog>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QVBoxLayout>

#include <flatland_server/timekeeper.h>
#include <ros/ros.h>
#include <boost/filesystem.hpp>
#include <iostream>
#include "flatland_viz/model_dialog.h"

// Initialize static variables
QColor ModelDialog::saved_color_;

QString ModelDialog::SelectFile() {
  QString fileName =
      QFileDialog::getOpenFileName(this, tr("Open model file"), "", "");
  if (fileName.isEmpty())
    return fileName;
  else {
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
      QMessageBox::information(this, tr("Unable to open file"),
                               file.errorString());
      return fileName;
    }
    file.close();

    return fileName;
  }
}

ModelDialog::ModelDialog(QWidget *parent) : QWidget(parent) {
  ROS_ERROR_STREAM("ModelDialog::ModelDialog");

  path_to_model_file = SelectFile();
  QVBoxLayout *v_layout = new QVBoxLayout;

  // we are injecting horizontal layouts into the master vertical layout
  QHBoxLayout *h1_layout = new QHBoxLayout;
  QHBoxLayout *h2_layout = new QHBoxLayout;
  QHBoxLayout *h3_layout = new QHBoxLayout;
  QHBoxLayout *h4_layout = new QHBoxLayout;

  QLabel *colorLabel = new QLabel;
  int frameStyle = QFrame::Sunken | QFrame::Panel;
  colorLabel->setFrameStyle(frameStyle);
  color_button = new QPushButton("");
  QPushButton *okButton = new QPushButton("ok");
  QPushButton *cancelButton = new QPushButton("cancel");
  connect(color_button, &QAbstractButton::clicked, this,
          &ModelDialog::SetColor);

  // path label and path
  QLabel *path = new QLabel;
  path->setText(path_to_model_file);
  h1_layout->addWidget(new QLabel("path:"));
  h1_layout->addWidget(path);
  v_layout->addLayout(h1_layout);

  // name label and name LineEdit
  QLineEdit *name_edit = new QLineEdit;
  h2_layout->addWidget(new QLabel("name:"));
  h2_layout->addWidget(name_edit);

  // set the default name to the filename parsed using boost
  std::string bsfn =
      boost::filesystem::basename(path_to_model_file.toStdString());
  QString fn = QString::fromStdString(bsfn);
  name_edit->setText(fn);

  v_layout->addLayout(h2_layout);

  // color label and button
  h3_layout->addWidget(new QLabel("color"));
  h3_layout->addWidget(color_button);
  v_layout->addLayout(h3_layout);

  // ok button
  h4_layout->addWidget(okButton);
  connect(okButton, &QAbstractButton::clicked, this,
          &ModelDialog::OkButtonClicked);

  // first time use color
  // after that, use saved_color_
  static bool first_time = true;
  QColor color;
  if (first_time) {
    ROS_ERROR_STREAM("firstTime");
    first_time = false;
    color = Qt::blue;
  } else {
    ROS_ERROR_STREAM("anotherTime");
    color = saved_color_;
  }

  ModelDialog::SetButtonColor(&color, color_button);

  // cancel button
  h4_layout->addWidget(cancelButton);
  connect(cancelButton, &QAbstractButton::clicked, this,
          &ModelDialog::CancelButtonClicked);

  v_layout->addLayout(h4_layout);

  setLayout(v_layout);

  // delete the Dialog if the user clicks on the x to close window
  this->setAttribute(Qt::WA_DeleteOnClose, true);
}

void ModelDialog::CancelButtonClicked() {
  ROS_ERROR_STREAM("Cancel clicked");
  this->close();
}

void ModelDialog::OkButtonClicked() {
  ROS_ERROR_STREAM("Ok clicked");
  ROS_ERROR_STREAM("connect to ROS model service");
}

void ModelDialog::SetColor() {
  const QColorDialog::ColorDialogOptions options =
      (QColorDialog::ShowAlphaChannel);

  const QColor color =
      QColorDialog::getColor(Qt::green, this, "Select Color", options);
  if (color.isValid()) {
    color_button->setText(color.name());
  }

  ModelDialog::SetButtonColor(&color, color_button);
  saved_color_ = color;
}

void ModelDialog::SetButtonColor(const QColor *c, QPushButton *b) {
  b->setText(c->name());
  b->setPalette(QPalette(*c));
  b->setAutoFillBackground(true);
  QPalette pal = b->palette();
  pal.setColor(QPalette::Button, *c);
  // c->setAutoFillBackground(true);
  QString qs = "background-color:" + c->name();
  color_button->setStyleSheet(qs);
}

void spawnModelClient() {
  boost::filesystem::path this_file_dir;
  boost::filesystem::path world_yaml;
  boost::filesystem::path robot_yaml;
  Timekeeper timekeeper;
  ros::NodeHandle nh;
  ros::ServiceClient client;
  flatland_msgs::SpawnModel srv;
  std::thread simulation_thread;
  bool stop_thread;
  World *w;

  // std::string world_yaml =
  //    this_file_dir / fs::path("load_world_tests/simple_test_A/world.yaml");
}
// // Register sigint shutdown handler
// signal(SIGINT, SigintHandler);

/**
 * @name        SigintHandler
 * @brief       Interrupt handler - sends shutdown signal to simulation_manager
 * @param[in]   sig: signal itself
 */
// void SigintHandler(int sig) {
//   ROS_WARN_NAMED("Node", "*** Shutting down... ***");

//   if (window != nullptr) {
//     delete window;
//     window = nullptr;
//   }
//   ROS_INFO_STREAM_NAMED("Node", "Beginning ros shutdown");
//   ros::shutdown();
// }

/*
world_yaml =
      this_file_dir / fs::path("load_world_tests/simple_test_A/world.yaml");

  robot_yaml = this_file_dir /
               fs::path("load_world_tests/simple_test_A/turtlebot.model.yaml");

  w = World::MakeWorld(world_yaml.string());

  srv.request.name = "service_manager_test_robot";
  srv.request.ns = "robot123";
  srv.request.yaml_path = robot_yaml.string();
  srv.request.pose.x = 1;
  srv.request.pose.y = 2;
  srv.request.pose.theta = 3;

  client = nh.serviceClient<flatland_msgs::SpawnModel>("spawn_model");

  // Threading is required since client.call blocks executing until return
  StartSimulationThread();

  ASSERT_TRUE(client.call(srv));

  StopSimulationThread();

  ASSERT_TRUE(srv.response.success);
  ASSERT_STREQ("", srv.response.message.c_str());

  ASSERT_EQ(5, w->models_.size());
  EXPECT_STREQ("service_manager_test_robot", w->models_[4]->name_.c_str());
  EXPECT_STREQ("robot123", w->models_[4]->namespace_.c_str());
  EXPECT_FLOAT_EQ(1, w->models_[4]->bodies_[0]->physics_body_->GetPosition().x);
  EXPECT_FLOAT_EQ(2, w->models_[4]->bodies_[0]->physics_body_->GetPosition().y);
  EXPECT_FLOAT_EQ(3, w->models_[4]->bodies_[0]->physics_body_->GetAngle());
  EXPECT_EQ(5, w->models_[4]->bodies_.size());

  delete w;
*/
