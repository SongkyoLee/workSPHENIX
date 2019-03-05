#!/bin/csh

set initfile = 0
set endfile = 1000


root -l -b -q draw_qg_emfrac_jet4_Al_uninst.C\(\"Al_uninst\",\"20GeV\",$initfile,$endfile,3,0\)
root -l -b -q draw_qg_emfrac_jet4_Al_uninst.C\(\"Al_uninst\",\"30GeV\",$initfile,$endfile,3,0\)
root -l -b -q draw_qg_emfrac_jet4_Al_uninst.C\(\"Al_uninst\",\"40GeV\",$initfile,$endfile,3,0\)
root -l -b -q draw_qg_emfrac_jet4_Al_uninst.C\(\"Al_uninst\",\"50GeV\",$initfile,$endfile,3,0\)

### 50GeV
#root -l -b -q draw_qg_emfrac_jet4_Al_uninst.C\(\"Al_uninst\",\"50GeV\",$initfile,$endfile,1,0\)
#root -l -b -q draw_qg_emfrac_jet4_Al_uninst.C\(\"Al_uninst\",\"50GeV\",$initfile,$endfile,3,0\)
#root -l -b -q draw_qg_emfrac_jet4_Al_uninst.C\(\"Al_uninst\",\"50GeV\",$initfile,$endfile,4,0\)
#root -l -b -q draw_qg_emfrac_jet4_Al_uninst.C\(\"Al_uninst\",\"50GeV\",$initfile,$endfile,4,1\)
#root -l -b -q draw_qg_emfrac_jet4_Al_uninst.C\(\"Al_uninst\",\"50GeV\",$initfile,$endfile,5,0\)

echo " *** DONE *** "
