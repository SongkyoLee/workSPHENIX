#!/bin/csh

set initfile = 0
set endfile = 310
#set endfile = 1000

#void test_rootAna_2ndSort_gammajet(string sampleType = "GammaJet",
#                              string ihcalType = "SS310",
#                              string jetE = "50GeV",
#                              string vtxType = "vtx10flat",
#                              int initfile=0,
#                              int endfile =1000,
#                              bool doetopcut=true,
#                              bool dodphicut=true)
set energy = "allGeV"

foreach sample (GammaJet)
#  foreach ihcal (SS310 Alframe)
  foreach ihcal (SS310)
  #foreach ihcal (Alframe)
    #foreach vtx (vtx0 vtx10flat vtx10gaus)
    foreach vtx (vtx10gaus)
      echo "***** Run" $sample\_$energy\_$vtx\_$ihcal
      root -l -b -q rootAna_2ndSort_gammajet_ver1.C\(\"$sample\",\"$ihcal\",\"$energy\",\"$vtx\",$initfile,$endfile,true,true\)
      root -l -b -q rootAna_2ndSort_gammajet_ver2.C\(\"$sample\",\"$ihcal\",\"$energy\",\"$vtx\",$initfile,$endfile,true,true\)
      root -l -b -q rootAna_2ndSort_gammajet_ver3.C\(\"$sample\",\"$ihcal\",\"$energy\",\"$vtx\",$initfile,$endfile,true,true\)
      root -l -b -q rootAna_2ndSort_gammajet_ver4.C\(\"$sample\",\"$ihcal\",\"$energy\",\"$vtx\",$initfile,$endfile,true,true\)
      root -l -b -q rootAna_2ndSort_gammajet_ver5.C\(\"$sample\",\"$ihcal\",\"$energy\",\"$vtx\",$initfile,$endfile,true,true\)
      root -l -b -q rootAna_2ndSort_gammajet_ver6.C\(\"$sample\",\"$ihcal\",\"$energy\",\"$vtx\",$initfile,$endfile,true,true\)
      root -l -b -q rootAna_2ndSort_gammajet_ver7.C\(\"$sample\",\"$ihcal\",\"$energy\",\"$vtx\",$initfile,$endfile,true,true\)
    end
  end
end

echo "***** All Done"  
