#!/bin/csh
### 25GeV
root -l -b -q 'draw_roofit_ohcalScale.C("SS310_PiPl_25GeV",true)'
root -l -b -q 'draw_roofit_ohcalScale.C("SS310_PiMi_25GeV",true)'
root -l -b -q 'draw_roofit_ohcalScale.C("Al_PiPl_25GeV",true)'
root -l -b -q 'draw_roofit_ohcalScale.C("Al_PiMi_25GeV",true)'
root -l -b -q 'draw_roofit_ohcalScale.C("SS310_PiPl_25GeV",false)'
root -l -b -q 'draw_roofit_ohcalScale.C("SS310_PiMi_25GeV",false)'
root -l -b -q 'draw_roofit_ohcalScale.C("Al_PiPl_25GeV",false)'
root -l -b -q 'draw_roofit_ohcalScale.C("Al_PiMi_25GeV",false)'

root -l -b -q 'draw_roofit_ohcalScale_PlMiCombined.C("SS310_25GeV",true)'
root -l -b -q 'draw_roofit_ohcalScale_PlMiCombined.C("Al_25GeV",true)'
root -l -b -q 'draw_roofit_ohcalScale_PlMiCombined.C("SS310_25GeV",false)'
root -l -b -q 'draw_roofit_ohcalScale_PlMiCombined.C("Al_25GeV",false)'

### 10GeV
root -l -b -q 'draw_roofit_ohcalScale.C("SS310_PiPl_10GeV",true)'
root -l -b -q 'draw_roofit_ohcalScale.C("SS310_PiMi_10GeV",true)'
root -l -b -q 'draw_roofit_ohcalScale.C("Al_PiPl_10GeV",true)'
root -l -b -q 'draw_roofit_ohcalScale.C("Al_PiMi_10GeV",true)'
root -l -b -q 'draw_roofit_ohcalScale.C("SS310_PiPl_10GeV",false)'
root -l -b -q 'draw_roofit_ohcalScale.C("SS310_PiMi_10GeV",false)'
root -l -b -q 'draw_roofit_ohcalScale.C("Al_PiPl_10GeV",false)'
root -l -b -q 'draw_roofit_ohcalScale.C("Al_PiMi_10GeV",false)'

root -l -b -q 'draw_roofit_ohcalScale_PlMiCombined.C("SS310_10GeV",true)'
root -l -b -q 'draw_roofit_ohcalScale_PlMiCombined.C("Al_10GeV",true)'
root -l -b -q 'draw_roofit_ohcalScale_PlMiCombined.C("SS310_10GeV",false)'
root -l -b -q 'draw_roofit_ohcalScale_PlMiCombined.C("Al_10GeV",false)'

