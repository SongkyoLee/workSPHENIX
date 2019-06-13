#!/bin/csh

set initfile = 0
set endfile = 1000

foreach ihcal (SS310)
  foreach energy (20GeV 30GeV 40GeV 50GeV)
    root -l -b -q method04_hybrid.C\(\"GammaJet\",\"$ihcal\",\"$energy\",$initfile,$endfile,true,true\) >&logs/log_method04_$initfile\_$endfile\_$energy\_$ihcal&
  end
end
### Alframe: identical to method03!!!
foreach ihcal (Alframe)
  foreach energy (20GeV 30GeV 40GeV 50GeV)
    root -l -b -q method04_hybrid_alframe.C\(\"GammaJet\",\"$ihcal\",\"$energy\",$initfile,$endfile,true,true\) >&logs/log_method04_$initfile\_$endfile\_$energy\_$ihcal&
  end
end

#root -l -b -q method04_hybrid.C\(\"GammaJet\",\"SS310\",\"20GeV\",$initfile,$endfile,true,true\) >&logs/log_method04_20GeV_SS310&
#root -l -b -q method04_hybrid.C\(\"GammaJet\",\"SS310\",\"30GeV\",$initfile,$endfile,true,true\) >&logs/log_method04_30GeV_SS310&
#root -l -b -q method04_hybrid.C\(\"GammaJet\",\"SS310\",\"40GeV\",$initfile,$endfile,true,true\) >&logs/log_method04_40GeV_SS310&
#root -l -b -q method04_hybrid.C\(\"GammaJet\",\"SS310\",\"50GeV\",$initfile,$endfile,true,true\) >&logs/log_method04_50GeV_SS310&

echo "***** All Done" 
 
