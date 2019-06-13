#!/bin/csh

set initfile = 0
set endfile = 1000

foreach sample (GammaJet)
  foreach ihcal (SS310 Alframe Al)
    foreach energy (20GeV 30GeV 40GeV 50GeV)
      echo "***** Run" $sample$energy\_$ihcal
      root -l -b -q rootAna_2ndSort_gammajet.C\(\"$sample\",\"$ihcal\",\"$energy\",\"$energy\",\"vtx0\",$initfile,$endfile,true,true\)
    end
  end
end

echo "***** All Done"  
