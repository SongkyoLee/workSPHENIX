#!/bin/csh

set initfile = 0
set endfile = 1000
#set endfile = 1

foreach prefix (SS310 Al Al_uninst)
  foreach energy (20GeV 30GeV 40GeV 50GeV)
  echo "***** Run" $prefix
#  root -l -b -q rootAna_sorting_jet4.C\(\"$prefix\",$initfile,$endfile\)
  root -l -b -q rootAna_sorting_jet4.C\(\"gammajet_$energy\_$prefix\",$initfile,$endfile\)
  end
end
  
