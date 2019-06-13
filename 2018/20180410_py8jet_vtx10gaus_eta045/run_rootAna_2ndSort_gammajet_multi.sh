#!/bin/csh

set initfile = 0
#set endfile = 310
#set endfile = 1000
set endfile = 9000

set ii = $initfile
set jj = $initfile
set step = 300

#void test_rootAna_2ndSort_gammajet(string sampleType = "GammaJet",
#                              string ihcalType = "SS310",
#                              string jetInE = "50GeV",
#                              string jetOutE = "50GeV",
#                              string vtxType = "vtx10flat",
#                              int initfile=0,
#                              int endfile =1000,
#                              bool doetopcut=true,
#                              bool dodphicut=true)

set ein = "allGeV"
set eout = "20GeV"

while ($ii < $endfile)
  @ jj = $ii + $step
  echo "*** Run init = " $ii
  echo "end = " $jj
  #root -l -b -q rootAna_2ndSort_gammajet.C\(\"GammaJet\",\"SS310\",\"$ein\",\"$eout\",\"vtx10gaus\",$ii,$jj,true,true\)
  #root -l -b -q rootAna_2ndSort_gammajet.C\(\"GammaJet\",\"Alframe\",\"$ein\",\"$eout\",\"vtx10gaus\",$ii,$jj,true,true\)
  root -l -b -q rootAna_2ndSort_gammajet.C\(\"GammaJet\",\"Al\",\"$ein\",\"$eout\",\"vtx10gaus\",$ii,$jj,true,true\)
  @ ii = $ii + $step
end

echo "***** All Done"  
