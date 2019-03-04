#!/bin/csh

set initfile=0
#set endfile=500
set endfile=200 ##default
#set endfile=30

foreach intype (SS310_PiPl_25GeV SS310_PiMi_25GeV Al_PiPl_25GeV Al_PiMi_25GeV SS310_PiPl_10GeV SS310_PiMi_10GeV Al_PiPl_10GeV Al_PiMi_10GeV)
  echo $intype
  root -l -b -q 'rootAna_mergefiles.C("'$intype'",'$initfile','$endfile')'
end

