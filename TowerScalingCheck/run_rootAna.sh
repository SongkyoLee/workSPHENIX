#!/bin/csh

set initfile = 0
set endfile = 1000
#set endfile = 600
## MyTree_Al_NotScaled: 636
##MyTree_Al_Scaled_v3: 585
## 100MeV_Al_NotScaled_v3: 420
## OnlyHcalScaled: 583

foreach prefix (MyTree_Al_NotScaled) ###pid is float in this tree..
#foreach prefix (100MeV_Al_NotScaled_v3)
#foreach prefix (OnlyHcalScaled)
#foreach prefix (100MeV_Al_NotScaled_v3 OnlyHcalScaled)
  echo "***** Run" $prefix
  root -l -b -q rootAna_emfrac.C\(\"$prefix\",$initfile,$endfile\)
end

