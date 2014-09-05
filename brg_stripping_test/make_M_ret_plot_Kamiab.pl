#!/usr/bin/perl -w

$cmd = "java -jar ~/Dropbox/gillis-comp-shared/stilts.jar plot2d " .
    "ofmt=eps out='M_ret_test_results_Kamiab.eps' legend=false grid=false " .
    "ylog=false xpix=400 ypix=400 ylo=0.3 yhi=1.05 ";

$cmd = $cmd . "ifmt0=ascii in0=stripping_test_orbitalC0.dat " .
	"cmd0='addcol M/M0 m_ret' ydata0=M/M0 " .
	"cmd0='addcol t/P t/8.77683' xdata0=t/P " .
	"shape0=filled_circle " .
	"size0=0 line0=DotToDot ";


$cmd = $cmd . "name0='c=.9'";

system($cmd);

