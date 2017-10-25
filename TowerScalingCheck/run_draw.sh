#!/bin/csh

foreach prefix (MyTree_Al_NotScaled)
#foreach prefix (100MeV_Al_NotScaled_v3)
#foreach prefix (OnlyHcalScaled)
#foreach prefix (100MeV_Al_NotScaled_v3 OnlyHcalScaled)
  echo "***** Run" $prefix
  root -l -b -q draw_emfrac.C\(\"$prefix\"\)
end

