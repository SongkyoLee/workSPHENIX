#!/bin/csh

set initfile = 0
set endfile = 400

foreach ihcal (SS310 Al)
  foreach note (R02 R04)
    foreach energy (20GeV 30GeV 40GeV 50GeV)
      root -l -b -q minuit_truthjet_wTot.C\(\"QCDJet\",\"$ihcal\",\"$energy\",\"$note\",$initfile,$endfile,true,true\) >&logs/log_truthjet_wTot_$initfile\_$endfile\_$energy\_$ihcal\_$note&
    end
  end
end

foreach ihcal (Alframe)
  foreach note (R02 R04)
    foreach energy (20GeV 30GeV 40GeV 50GeV)
      root -l -b -q minuit_truthjet_wTot_alframe.C\(\"QCDJet\",\"$ihcal\",\"$energy\",\"$note\",$initfile,$endfile,true,true\) >&logs/log_truthjet_wTot_$initfile\_$endfile\_$energy\_$ihcal\_$note&
    end
  end
end

echo "***** All Done" 
 
