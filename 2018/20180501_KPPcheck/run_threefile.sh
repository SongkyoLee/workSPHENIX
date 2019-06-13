#!/bin/csh

set initfile = 0
set endfile = 1000

foreach sample (QCDJet) 
  foreach energy (20GeV 30GeV 40GeV 50GeV)
  #foreach energy (20GeV)
    foreach res(R04)
      root -l -b -q jes_threefile.C\(\"$sample\",\"Al\",\"$energy\",\"$res\",$initfile,$endfile,true,true\)
    end  
  end  
end  

echo "***** All Done"  
      
#root -l -b -q jes_threefile.C\(\"QCDJet\",\"$sample\",\"$energy\",\"$res\",$initfile,$endfile,true,true\) >&logs/log_final_qcd_\_$initfile\_$endfile\_$energy\_$ihcal&

