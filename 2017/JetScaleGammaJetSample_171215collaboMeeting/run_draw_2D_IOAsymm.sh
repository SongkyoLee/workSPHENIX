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
set c_ih = 1.00

foreach prefix (SS310 Al)
  echo "***** Run" $prefix
  foreach energy (20GeV 30GeV 40GeV 50GeV)
    root -l -b -q draw_2D_IOAsymm.C\(\"$prefix\",\"$energy\",\"outJet4Matched\",\"outJet4_2DIOAsymm\",$initfile,$endfile,$c_tot,$c_em,$c_h,$c_ih\)
  end
end
  
