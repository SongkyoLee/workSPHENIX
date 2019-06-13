#!/bin/csh

set initfile = 0
set endfile = 1000

set energy = "allGeV"

foreach sample (GammaJet)
  foreach ihcal (SS310)
    foreach energy (20GeV 30GeV 40GeV 50GeV)
      echo "***** Run" $sample\_$energy\_nover\_$ihcal
      root -l -b -q draw_crosssection.C\(\"$sample\",\"$ihcal\",\"$energy\",\"nover\",$initfile,$endfile,true,true\) >& logs/log_crosssection_$initfile\_$endfile\_$energy\_$ihcal&

    end
  end
end

echo "***** All Done"  
