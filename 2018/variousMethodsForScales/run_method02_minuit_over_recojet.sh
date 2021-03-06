#!/bin/csh

set initfile = 0
set endfile = 1000

#void method02_minuit_over_recojet(string sampleType = "GammaJet",
#                              string ihcalType = "SS310",
#                              string jetE = "50GeV",
#                              int initfile=0,
#                              int endfile =1000,
#                              bool doetopcut=true,
#                              bool dodphicut=true)

root -l -b -q method02_minuit_over_recojet.C\(\"GammaJet\",\"SS310\",\"20GeV\",$initfile,$endfile,true,true\)
root -l -b -q method02_minuit_over_recojet.C\(\"GammaJet\",\"SS310\",\"30GeV\",$initfile,$endfile,true,true\)
root -l -b -q method02_minuit_over_recojet.C\(\"GammaJet\",\"SS310\",\"40GeV\",$initfile,$endfile,true,true\)
root -l -b -q method02_minuit_over_recojet.C\(\"GammaJet\",\"SS310\",\"50GeV\",$initfile,$endfile,true,true\)

root -l -b -q method02_minuit_over_recojet_alframe.C\(\"GammaJet\",\"Alframe\",\"20GeV\",$initfile,$endfile,true,true\)
root -l -b -q method02_minuit_over_recojet_alframe.C\(\"GammaJet\",\"Alframe\",\"30GeV\",$initfile,$endfile,true,true\)
root -l -b -q method02_minuit_over_recojet_alframe.C\(\"GammaJet\",\"Alframe\",\"40GeV\",$initfile,$endfile,true,true\)
root -l -b -q method02_minuit_over_recojet_alframe.C\(\"GammaJet\",\"Alframe\",\"50GeV\",$initfile,$endfile,true,true\)

echo "***** All Done" 
 
