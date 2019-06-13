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

set ein = "allGeV"
set eout = "20GeV"

foreach sample (QCDJet)
  #foreach ihcal (SS310 Alframe Al)
  foreach ihcal (Al)
    echo "***** Run" $sample$ein\_$ihcal
    root -l -b -q rootAna_2ndSort_qcdjet.C\(\"$sample\",\"$ihcal\",\"$ein\",\"$eout\",\"vtx10gaus\",$initfile,$endfile,true,true\)
  end
end

echo "***** All Done"  
