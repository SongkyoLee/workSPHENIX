#!/bin/csh

set initfile = 0
#set endfile = 12000
set endfile = 24000

foreach ihcal (Al)
  #foreach note (R02 R04)
  foreach note (R04)
    foreach energy (20GeV 30GeV 40GeV 50GeV)
      root -l -b -q minuit_truthjet_py8jet.C\(\"py8Jet\",\"$ihcal\",\"$energy\",\"$note\",$initfile,$endfile,true,true\) >&logs_py8jet/log_truthjet_$initfile\_$endfile\_$energy\_$ihcal\_$note&
    end
  end
end

foreach ihcal (Alframe)
  #foreach note (R02 R04)
  foreach note (R04)
    foreach energy (20GeV 30GeV 40GeV 50GeV)
      root -l -b -q minuit_truthjet_alframe_py8jet.C\(\"py8Jet\",\"$ihcal\",\"$energy\",\"$note\",$initfile,$endfile,true,true\) >&logs_py8jet/log_truthjet_$initfile\_$endfile\_$energy\_$ihcal\_$note&
    end
  end
end

echo "***** All Done" 
 
