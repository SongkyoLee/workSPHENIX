#!/bin/csh

set initfile = 0
#set endfile = 400
set endfile = 200

#foreach ihcal (SS310 Al)
foreach ihcal (Al)
  foreach note (04fmR02 04fmR04 48fmR02 48fmR04)
    foreach energy (20GeV 30GeV 40GeV 50GeV)
      root -l -b -q minuit_truthjet_hi.C\(\"HIJet\",\"$ihcal\",\"$energy\",\"$note\",$initfile,$endfile,false,false\) >&logs/log_truthjet_hi_$initfile\_$endfile\_$energy\_$ihcal\_$note&
    end
  end
end

#foreach ihcal (Alframe)
#  foreach note (R02 R04)
#    foreach energy (20GeV 30GeV 40GeV 50GeV)
#      root -l -b -q minuit_truthjet_hi_alframe.C\(\"HIJet\",\"$ihcal\",\"$energy\",\"$note\",$initfile,$endfile,false,false\) >&logs/log_truthjet_$initfile\_$endfile\_$energy\_$ihcal\_$note&
#    end
#  end
#end

echo "***** All Done" 
 
