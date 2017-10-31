#!/bin/csh
###void draw_roofit_ohcal_PlMiCombined(string inType = "SS310_25GeV", bool do_doubleGaus = true, bool ohcalScaled =false)

### 25GeV
#root -l -b -q 'draw_roofit_ohcal.C("SS310_PiPl_25GeV",true)'
#root -l -b -q 'draw_roofit_ohcal.C("SS310_PiMi_25GeV",true)'
#root -l -b -q 'draw_roofit_ohcal.C("Al_PiPl_25GeV",true)'
#root -l -b -q 'draw_roofit_ohcal.C("Al_PiMi_25GeV",true)'
#root -l -b -q 'draw_roofit_ohcal.C("SS310_PiPl_25GeV",false)'
#root -l -b -q 'draw_roofit_ohcal.C("SS310_PiMi_25GeV",false)'
#root -l -b -q 'draw_roofit_ohcal.C("Al_PiPl_25GeV",false)'
#root -l -b -q 'draw_roofit_ohcal.C("Al_PiMi_25GeV",false)'

root -l -b -q 'draw_roofit_ohcal_PlMiCombined.C("SS310_25GeV",true,false)'
root -l -b -q 'draw_roofit_ohcal_PlMiCombined.C("Al_25GeV",true,false)'
root -l -b -q 'draw_roofit_ohcal_PlMiCombined.C("SS310_25GeV",true,true)'
root -l -b -q 'draw_roofit_ohcal_PlMiCombined.C("Al_25GeV",true,true)'
#1G
#root -l -b -q 'draw_roofit_ohcal_PlMiCombined.C("SS310_25GeV",false,false)'
#root -l -b -q 'draw_roofit_ohcal_PlMiCombined.C("Al_25GeV",false,false)'
#root -l -b -q 'draw_roofit_ohcal_PlMiCombined.C("SS310_25GeV",false,true)'
#root -l -b -q 'draw_roofit_ohcal_PlMiCombined.C("Al_25GeV",false,true)'

### 10GeV
#root -l -b -q 'draw_roofit_ohcal.C("SS310_PiPl_10GeV",true)'
#root -l -b -q 'draw_roofit_ohcal.C("SS310_PiMi_10GeV",true)'
#root -l -b -q 'draw_roofit_ohcal.C("Al_PiPl_10GeV",true)'
#root -l -b -q 'draw_roofit_ohcal.C("Al_PiMi_10GeV",true)'
#root -l -b -q 'draw_roofit_ohcal.C("SS310_PiPl_10GeV",false)'
#root -l -b -q 'draw_roofit_ohcal.C("SS310_PiMi_10GeV",false)'
#root -l -b -q 'draw_roofit_ohcal.C("Al_PiPl_10GeV",false)'
#root -l -b -q 'draw_roofit_ohcal.C("Al_PiMi_10GeV",false)'

root -l -b -q 'draw_roofit_ohcal_PlMiCombined.C("SS310_10GeV",true,false)'
root -l -b -q 'draw_roofit_ohcal_PlMiCombined.C("Al_10GeV",true,false)'
root -l -b -q 'draw_roofit_ohcal_PlMiCombined.C("SS310_10GeV",true,true)'
root -l -b -q 'draw_roofit_ohcal_PlMiCombined.C("Al_10GeV",true,true)'
#1G
#root -l -b -q 'draw_roofit_ohcal_PlMiCombined.C("SS310_10GeV",false,false)'
#root -l -b -q 'draw_roofit_ohcal_PlMiCombined.C("Al_10GeV",false,false)'
#root -l -b -q 'draw_roofit_ohcal_PlMiCombined.C("SS310_10GeV",false,true)'
#root -l -b -q 'draw_roofit_ohcal_PlMiCombined.C("Al_10GeV",false,true)'
