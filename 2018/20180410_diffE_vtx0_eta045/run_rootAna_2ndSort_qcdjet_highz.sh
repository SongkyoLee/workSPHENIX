#!/bin/csh

set initfile = 0
#set endfile = 1000
set endfile = 250

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
    foreach energy (40GeV)
      echo "***** Run" $sample$energy\_$ihcal
      #root -l -b -q rootAna_2ndSort_qcdjet_highz.C\(\"$sample\",\"$ihcal\",\"allGeV\",\"$energy\",\"highz\",$initfile,$endfile,true,true\)
      root -l -b -q rootAna_2ndSort_qcdjet_highz.C\(\"$sample\",\"$ihcal\",\"allGeV\",\"$energy\",\"noptcut_highz\",$initfile,$endfile,true,true\)
      root -l -b -q rootAna_2ndSort_qcdjet_highz.C\(\"$sample\",\"$ihcal\",\"allGeV\",\"$energy\",\"noptcut_allz\",$initfile,$endfile,true,true\)
    end
  end
end


echo "***** All Done"  
