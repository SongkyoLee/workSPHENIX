#!/bin/csh

#foreach prefix (MyTree_SS310_NotScaled)
#foreach prefix (MyTree_Al_NotScaled) # pid is float in this file..

root -l -b -q draw_modi_emfrac.C\(\"MyTree_SS310_NotScaled\",0,600,1.07,3.60,1.28\)
root -l -b -q draw_modi_emfrac.C\(\"MyTree_Al_NotScaled\",0,600,1.01,9.29,1.20\)

#foreach prefix (MyTree_SS310_NotScaled MyTree_Al_NotScaled)
#  echo "***** Run" $prefix
#  root -l -b -q draw_emfrac.C\(\"$prefix\"\)
#end

