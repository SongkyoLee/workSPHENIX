#!/bin/csh

set initfile = 0
set endfile = 1000

######################################################
###### NOT USED anymore!!!
######################################################

foreach ihcal (SS310)
  foreach energy (20GeV 30GeV 40GeV 50GeV)
    root -l -b -q method02_minuit_over_recojet.C\(\"GammaJet\",\"$ihcal\",\"$energy\",$initfile,$endfile,true,true\) >&logs/log_method02_$initfile\_$endfile\_$energy\_$ihcal&
  end
end
foreach ihcal (Alframe)
  foreach energy (20GeV 30GeV 40GeV 50GeV)
    root -l -b -q method02_minuit_over_recojet_alframe.C\(\"GammaJet\",\"$ihcal\",\"$energy\",$initfile,$endfile,true,true\) >&logs/log_method02_$initfile\_$endfile\_$energy\_$ihcal&
  end
end

#root -l -b -q method02_minuit_over_recojet.C\(\"GammaJet\",\"SS310\",\"20GeV\",$initfile,$endfile,true,true\)
#root -l -b -q method02_minuit_over_recojet.C\(\"GammaJet\",\"SS310\",\"30GeV\",$initfile,$endfile,true,true\)
#root -l -b -q method02_minuit_over_recojet.C\(\"GammaJet\",\"SS310\",\"40GeV\",$initfile,$endfile,true,true\)
#root -l -b -q method02_minuit_over_recojet.C\(\"GammaJet\",\"SS310\",\"50GeV\",$initfile,$endfile,true,true\)
#root -l -b -q method02_minuit_over_recojet_alframe.C\(\"GammaJet\",\"Alframe\",\"20GeV\",$initfile,$endfile,true,true\)
#root -l -b -q method02_minuit_over_recojet_alframe.C\(\"GammaJet\",\"Alframe\",\"30GeV\",$initfile,$endfile,true,true\)
#root -l -b -q method02_minuit_over_recojet_alframe.C\(\"GammaJet\",\"Alframe\",\"40GeV\",$initfile,$endfile,true,true\)
#root -l -b -q method02_minuit_over_recojet_alframe.C\(\"GammaJet\",\"Alframe\",\"50GeV\",$initfile,$endfile,true,true\)

echo "***** All Done" 
 
