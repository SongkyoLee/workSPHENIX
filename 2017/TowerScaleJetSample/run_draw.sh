#!/bin/csh

#foreach prefix (MyTree_SS310_NotScaled)
#foreach prefix (MyTree_Al_NotScaled) # pid is float in this file..
#foreach prefix (100MeV_Al_NotScaled_v3)
#foreach prefix (OnlyHcalScaled) #100MeV applied

foreach prefix (MyTree_SS310_NotScaled MyTree_Al_NotScaled)
  echo "***** Run" $prefix
  root -l -b -q draw_emfrac.C\(\"$prefix\"\)
end

