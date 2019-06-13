#!/bin/csh

#./getScale_multi.sh
#./getJESJER_multi.sh

foreach ihcal (SS310 Alframe) 
  root -l -b -q draw_stat_SF_multi.C\(\"$ihcal\"\) >&logs/log_stat_SF_$ihcal&
  root -l -b -q draw_stat_JESJER_multi.C\(\"$ihcal\"\) >&logs/log_stat_JESJER_$ihcal&
end  

echo "***** All Done" 
 
########## before
#root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"20GeV\",$initfile,$endfile,1.00,1.00,1.00,1.00,true,true\)
#foreach energy (20GeV 30GeV 40GeV 50GeV)
#  root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"$energy\",$initfile,$endfile,1.00,1.00,1.00,1.00,true,true\)
#end

