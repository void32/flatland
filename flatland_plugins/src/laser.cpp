/*
 *  ______                   __  __              __
 * /\  _  \           __    /\ \/\ \            /\ \__
 * \ \ \L\ \  __  __ /\_\   \_\ \ \ \____    ___\ \ ,_\   ____
 *  \ \  __ \/\ \/\ \\/\ \  /'_` \ \ '__`\  / __`\ \ \/  /',__\
 *   \ \ \/\ \ \ \_/ |\ \ \/\ \L\ \ \ \L\ \/\ \L\ \ \ \_/\__, `\
 *    \ \_\ \_\ \___/  \ \_\ \___,_\ \_,__/\ \____/\ \__\/\____/
 *     \/_/\/_/\/__/    \/_/\/__,_ /\/___/  \/___/  \/__/\/___/
 * @copyright Copyright 2017 Avidbots Corp.
 * @name	  laser.cpp
 * @brief   Laser plugin
 * @author  Chunshang Li
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

#include <flatland_plugins/laser.h>
#include <flatland_server/model_body.h>
#include <flatland_server/model_plugin.h>
#include <pluginlib/class_list_macros.h>

using namespace flatland_server;

namespace flatland_plugins {

void Laser::OnInitialize(const YAML::Node &config) {
  ROS_INFO_NAMED("LaserPlugin", "Laser Initialized");
  ModelBody *b = model_->GetBody("base");
  // b->physics_body_->ApplyForce(b2Vec2(50, 0),
  //                              b->physics_body_->GetWorldCenter(), true);
b->physics_body_->SetLinearVelocity(b2Vec2(5, 0));

// b->physics_body_->SetFixedRotation(true)
printf("*********%d\n", b->physics_body_->IsFixedRotation());

model_->GetBody("base")->physics_body_->Dump();
model_->GetBody("left_wheel")->physics_body_->Dump();
model_->GetBody("right_wheel")->physics_body_->Dump();
model_->joints_[0]->physics_joint_->Dump();
model_->joints_[1]->physics_joint_->Dump();
}

void Laser::BeforePhysicsStep(double timestep) {
  ROS_WARN_THROTTLE_NAMED(1, "LaserPlugin", "BeforePhysicsUpdate");

model_->GetBody("base")->physics_body_->SetAngularVelocity(3.1416);

  model_->DebugVisualize();
}
};

PLUGINLIB_EXPORT_CLASS(flatland_plugins::Laser, flatland_server::ModelPlugin)