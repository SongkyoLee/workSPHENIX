#!/bin/csh

set initfile = 0
set endfile = 1000

foreach ihcal (SS310 Alframe)
  foreach energy (20GeV 30GeV 40GeV 50GeV)
    root -l -b -q method05_asymm_manual.C\(\"GammaJet\",\"$ihcal\",\"$energy\",$initfile,$endfile,true,true\) >&logs/log_method05_$initfile\_$endfile\_$energy\_$ihcal&
  end
end

#root -l -b -q method05_asymm_manual.C\(\"GammaJet\",\"SS310\",\"20GeV\",$initfile,$endfile,true,true\) >&logs/log_method05_20GeV_SS310&
#root -l -b -q method05_asymm_manual.C\(\"GammaJet\",\"SS310\",\"30GeV\",$initfile,$endfile,true,true\) >&logs/log_method05_30GeV_SS310&
#root -l -b -q method05_asymm_manual.C\(\"GammaJet\",\"SS310\",\"40GeV\",$initfile,$endfile,true,true\) >&logs/log_method05_40GeV_SS310&
#root -l -b -q method05_asymm_manual.C\(\"GammaJet\",\"SS310\",\"50GeV\",$initfile,$endfile,true,true\) >&logs/log_method05_50GeV_SS310&
#root -l -b -q method05_asymm_manual.C\(\"GammaJet\",\"Alframe\",\"20GeV\",$initfile,$endfile,true,true\) >&logs/log_method05_20GeV_SS310&
#root -l -b -q method05_asymm_manual.C\(\"GammaJet\",\"Alframe\",\"30GeV\",$initfile,$endfile,true,true\) >&logs/log_method05_30GeV_SS310&
#root -l -b -q method05_asymm_manual.C\(\"GammaJet\",\"Alframe\",\"40GeV\",$initfile,$endfile,true,true\) >&logs/log_method05_40GeV_SS310&
#root -l -b -q method05_asymm_manual.C\(\"GammaJet\",\"Alframe\",\"50GeV\",$initfile,$endfile,true,true\) >&logs/log_method05_50GeV_SS310&

echo "***** All Done" 
 
