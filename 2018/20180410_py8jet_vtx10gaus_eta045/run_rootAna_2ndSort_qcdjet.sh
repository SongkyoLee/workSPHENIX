#!/bin/csh

set initfile = 0
set endfile = 1000

#void test_rootAna_2ndSort_qcdjet(string sampleType = "GammaJet",
#                              string ihcalType = "SS310",
#                              string jetInE = "50GeV",
#                              string jetOutE = "50GeV",
#                              string vtxType = "vtx10gaus",
#                              int initfile=0,
#                              int endfile =1000,
#                              bool doetopcut=true,
#                              bool dodphicut=true)

foreach sample (QCDJet)
  foreach ihcal (SS310 Alframe)
  #foreach ihcal (SS310)
  #foreach ihcal (Alframe)
    foreach energy (20GeV 30GeV 40GeV 50GeV)
#    foreach energy (20GeV)
      echo "***** Run" $sample$energy\_$ihcal
      root -l -b -q rootAna_2ndSort_qcdjet.C\(\"$sample\",\"$ihcal\",\"$energy\",\"$energy\",\"nover\",$initfile,$endfile,true,true\)
    end
  end
end

echo "***** All Done"  
