#!/usr/bin/perl

use File::Copy;
my $countb = 0;
my $countb = 100;
my $ld = 25;


fcopy(10,0,$ld); # copies ee10.png               to  bb01.png ... bb26.png
fcopy(10,1,13);  # copies ee10.png ... ee23.png  to  bb27.png ... bb40.png
fcopy(23,0,$ld); # copies ee23.png               to  bb41.png ... bb66.png
fcopy(30,1,8);   # copies ee30.png ... ee37.png  to  bb42.png ... bb49.png
fcopy(37,0,$ld); # copies ee37.png               to  bb50.png ... bb75.png

system("convert -delay 5 -loop 0 bb*.png text.gif");


my $sfn = "aa$starta.png";

sub fcopy {
	my $starta = $_[0];
	my $incr = $_[1];
	my $num = $_[2];
	print $starta."\n";
	print $incr  ."\n";
	print $num   ."\n";
	$counta = $starta;
	for(my $i=0; $i<$num; $i++) {
		my $sfn = "ee$counta" . ".png";
		my $dfn = "bb$countb" . ".png";
		print("Copying $sfn to $dfn \n");
		copy($sfn,$dfn) or die "Copy failed: $!";
		$counta = $counta +$incr;
		$countb = $countb +1;
	}
}
