#!/bin/csh

set initfile = 0
set endfile = 1000

#void test_rootAna_2ndSort_gammajet(string sampleType = "GammaJet",
#                              string ihcalType = "SS310",
#                              string jetInE = "50GeV",
#                              string jetOutE = "50GeV",
#                              string vtxType = "vtx10gaus",
#                              int initfile=0,
#                              int endfile =1000,
#                              bool doetopcut=true,
#                              bool dodphicut=true)

foreach sample (GammaJet)
#  foreach ihcal (SS310 Alframe)
  foreach ihcal (Al)
    foreach note (R02 R04)
    #foreach note (R04)
      foreach energy (20GeV 30GeV 40GeV 50GeV)
        echo "***** Run" $sample$energy\_$ihcal\_$note
        root -l -b -q rootAna_2ndSort_gammajet.C\(\"$sample\",\"$ihcal\",\"$energy\",\"$energy\",\"$note\",$initfile,$endfile,true,true\)
      end
    end
  end
end

echo "***** All Done"  
