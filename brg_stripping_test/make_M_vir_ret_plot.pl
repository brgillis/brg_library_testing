#!/usr/bin/perl -w

$cmd = "java -jar ~/Dropbox/gillis-comp-shared/stilts.jar plot2d " .
    "ofmt=png out='M_vir_ret_test_results.png' legend=false grid=false " .
    "ylog=false xpix=400 ypix=400 ylo=-0.8 ";

for($i=0;$i<=8;$i++) {
    $cmd = $cmd . "ifmt".$i."=ascii in".$i."=stripping_test_orbital".$i.".dat " .
	"cmd".$i."='addcol log\\(M/M0\\) log10(m_vir_ret)' ydata".$i."=log\\(M/M0\\) " .
	"cmd".$i."='addcol t/P t/8.77683' xdata".$i."=t/P " .
        "shape".$i."=filled_circle " .
	"size".$i."=0 line".$i."=DotToDot "
}

$cmd = $cmd . "color0=black color1=blue color2=cyan color3=green color4=yellow " .
              "color5=red color6=magenta color7=blue color8=cyan ";

$cmd = $cmd . "name0='c=1.00' name1='c=.99' name2='c=.95' name3='c=.9' " .
    "name4='c=.8' name5='c=.7' name6='c=.6' name7='c=.5' name8='c=.4' ";

system($cmd);

