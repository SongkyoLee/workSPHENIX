#!/bin/csh

set initfile = 0
set endfile = 400

#foreach ihcal (SS310 Al Alframe)
foreach ihcal (Al Alframe)
  #foreach note (R02 R04)
  foreach note (R04)
    foreach energy (20GeV 30GeV 40GeV 50GeV)
      root -l -b -q make_log_before_merged.C\(\"QCDJet\",\"$ihcal\",\"$energy\",\"$note\",$initfile,$endfile,true,true\)
    end
  end
end

echo "***** All Done" 
 
