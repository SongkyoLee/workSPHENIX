#!/bin/csh
root -l -b -q 'draw_ohcalScale.C("SS310_PiPl_25GeV",true)'
root -l -b -q 'draw_ohcalScale.C("SS310_PiMi_25GeV",true)'
root -l -b -q 'draw_ohcalScale.C("Al_PiPl_25GeV",true)'
root -l -b -q 'draw_ohcalScale.C("Al_PiMi_25GeV",true)'
root -l -b -q 'draw_ohcalScale.C("SS310_PiPl_25GeV",false)'
root -l -b -q 'draw_ohcalScale.C("SS310_PiMi_25GeV",false)'
root -l -b -q 'draw_ohcalScale.C("Al_PiPl_25GeV",false)'
root -l -b -q 'draw_ohcalScale.C("Al_PiMi_25GeV",false)'

root -l -b -q 'draw_ohcalScale_PlMiCombined.C("SS310_25GeV",true)'
root -l -b -q 'draw_ohcalScale_PlMiCombined.C("Al_25GeV",true)'
root -l -b -q 'draw_ohcalScale_PlMiCombined.C("SS310_25GeV",false)'
root -l -b -q 'draw_ohcalScale_PlMiCombined.C("Al_25GeV",false)'
