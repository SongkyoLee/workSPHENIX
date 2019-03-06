#!/bin/csh

set initfile = 0
set endfile = 1000

#void test_rootAna_2ndSort_gammajet(string sampleType = "GammaJet",
#                              string ihcalType = "SS310",
#                              string jetE = "50GeV",
#                              int initfile=0,
#                              int endfile =1000,
#                              bool doetopcut=true,
#                              bool dodphicut=true)

foreach sample (GammaJet)
  foreach ihcal (SS310 Alframe)
  #foreach ihcal (SS310)
  #foreach ihcal (Alframe)
    foreach energy (20GeV 30GeV 40GeV 50GeV)
#    foreach energy (20GeV)
      echo "***** Run" $sample$energy\_$ihcal
      root -l -b -q rootAna_2ndSort_gammajet.C\(\"$sample\",\"$ihcal\",\"$energy\",$initfile,$endfile,true,true\)
    end
  end
end

echo "***** All Done"  
