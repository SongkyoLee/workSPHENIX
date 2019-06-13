#!/bin/csh

set initfile = 0
set endfile = 200

#foreach ihcal (SS310 Al Alframe)
foreach ihcal (Al)
  #foreach note (R02 R04)
  foreach note (04fmR02 04fmR04 48fmR02 48fmR04)
    foreach energy (20GeV 30GeV 40GeV 50GeV)
      root -l -b -q make_log_before.C\(\"QCDJet\",\"$ihcal\",\"$energy\",\"$note\",$initfile,$endfile,true,true\)
    end
  end
end

echo "***** All Done" 
 
