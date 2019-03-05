#!/bin/csh
##############################################
### Not used!!
### use trial_XXX.C instead of this macro!!!
##############################################

set initfile = 0
set endfile = 1000

### E = c_tot ( c_em * E_cemc + c_h ( c_ih * C_ihcal + C_ohcal) )
set c_tot = 1.00
set c_em = 1.00
set c_h = 1.00
#set c_h = 1.02
#set c_h = 1.27 //for Al_uninst only for check
#set c_h = 1.50
#set c_h = 2.54
#set c_ih = 1.00
#set c_ih = 2.78 #SS310 50GeV
#set c_ih = 2.34 #SS310 20GeV
#set c_ih = 7.18 #Al 50GeV
set c_ih = 6.43 #Al 20GeV

#foreach prefix (SS310 Al Al_uninst)
#foreach prefix (SS310)
foreach prefix (Al)
#foreach prefix (Al_uninst)
  echo "***** Run" $prefix
#  foreach energy (20GeV 30GeV 40GeV 50GeV)
#  foreach energy (50GeV)
  foreach energy (20GeV)
    root -l -b -q draw_2D_EHAsymm.C\(\"$prefix\",\"$energy\",\"outJet4Matched\",\"outJet4_2DEHAsymm\",$initfile,$endfile,$c_tot,$c_em,$c_h,$c_ih\)
  end
end
  
