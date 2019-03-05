#!/bin/csh

set initfile = 0
set endfile = 1000
#set endfile = 600

## MyTree_SS310_NotScaled: 600
## MyTree_Al_NotScaled: 636
## 100MeV_Al_NotScaled_v3: 420
## OnlyHcalScaled: 583

foreach prefix (MyTree_SS310_NotScaled)
  echo "***** Run" $prefix
  root -l -b -q rootAna_emfrac.C\(\"$prefix\",$initfile,$endfile\)
end
  
foreach prefix (MyTree_Al_NotScaled) # pid is float in this file..
  echo "***** Run" $prefix
  root -l -b -q rootAna_emfrac_floatpid.C\(\"$prefix\",$initfile,$endfile\)
end

