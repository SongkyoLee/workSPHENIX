#!/bin/csh

set initfile = 0
set endfile = 1000

#void method05_asymm_manual(string sampleType = "GammaJet",
#                              string ihcalType = "SS310",
#                              string jetE = "50GeV",
#                              int initfile=0,
#                              int endfile =1000,
#                              float c_a=1.00,
#                              float c_b1=1.00,
#                              float c_b2 = 1.00,
#                              float c_c=1.00,
#                              bool doetopcut=true,
#                              bool dodphicut=true)

root -l -b -q method05_asymm_manual.C\(\"GammaJet\",\"SS310\",\"20GeV\",$initfile,$endfile,true,true\)
root -l -b -q method05_asymm_manual.C\(\"GammaJet\",\"SS310\",\"30GeV\",$initfile,$endfile,true,true\)
root -l -b -q method05_asymm_manual.C\(\"GammaJet\",\"SS310\",\"40GeV\",$initfile,$endfile,true,true\)
root -l -b -q method05_asymm_manual.C\(\"GammaJet\",\"SS310\",\"50GeV\",$initfile,$endfile,true,true\)

root -l -b -q method05_asymm_manual.C\(\"GammaJet\",\"Alframe\",\"20GeV\",$initfile,$endfile,true,true\)
root -l -b -q method05_asymm_manual.C\(\"GammaJet\",\"Alframe\",\"30GeV\",$initfile,$endfile,true,true\)
root -l -b -q method05_asymm_manual.C\(\"GammaJet\",\"Alframe\",\"40GeV\",$initfile,$endfile,true,true\)
root -l -b -q method05_asymm_manual.C\(\"GammaJet\",\"Alframe\",\"50GeV\",$initfile,$endfile,true,true\)

echo "***** All Done" 
 
