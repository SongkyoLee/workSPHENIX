#!/bin/csh

set initfile = 0
#set endfile = 1000
set endfile = 9000

set ii = $initfile
set jj = $initfile
set step = 300

while ($ii < $endfile)
  @ jj = $ii + $step
  echo "*** Run init = " $ii
  echo "end = " $jj
  foreach ihcal (SS310)
    #foreach energy (20GeV 30GeV 40GeV 50GeV)
    foreach energy (20GeV)
      root -l -b -q method02_minuit_over_recojet.C\(\"GammaJet\",\"$ihcal\",\"$energy\",$ii,$jj,true,true\) >&logs/log_method02_$ii\_$jj\_$energy\_$ihcal&
      sleep 3s
    end
  end
  foreach ihcal (Alframe)
    #foreach energy (20GeV 30GeV 40GeV 50GeV)
    foreach energy (20GeV)
      root -l -b -q method02_minuit_over_recojet_alframe.C\(\"GammaJet\",\"$ihcal\",\"$energy\",$ii,$jj,true,true\) >&logs/log_method02_$ii\_$jj\_$energy\_$ihcal&
      sleep 3s
    end
  end
  @ ii = $ii + $step
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
 
