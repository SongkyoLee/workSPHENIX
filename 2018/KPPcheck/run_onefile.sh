#!/bin/csh

set initfile = 0
set endfile = 1000

foreach sample (QCDJet ObjectiveKPP ThresholdKPP)
#foreach sample (ThresholdKPP)
#foreach sample (QCDJet) 
  foreach energy (20GeV 30GeV 40GeV 50GeV)
  #foreach energy (20GeV)
    #foreach res(R04)
    foreach res(R02)
      root -l -b -q jes_onefile.C\(\"$sample\",\"Al\",\"$energy\",\"$res\",$initfile,$endfile,true,true\) >&logs/log_onefile_$initfile\_$endfile\_$energy\_$sample\_$res&
    end  
  end  
end  

echo "***** All Done"  

