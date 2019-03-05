#!/bin/csh

set initfile = 0
set endfile = 300


######### SS310 
### 20GeV
#root -l -b -q draw_qg_emfrac_jet4.C\(\"SS310\",\"20GeV\",$initfile,$endfile,1.00,1.00,1.00\)
#root -l -b -q draw_qg_emfrac_jet4.C\(\"SS310\",\"20GeV\",$initfile,$endfile,1.00,2.34,1.00\)
#root -l -b -q draw_qg_emfrac_jet4.C\(\"SS310\",\"20GeV\",$initfile,$endfile,1.00,2.15,0.92\)
### 40GeV
#root -l -b -q draw_qg_emfrac_jet4.C\(\"SS310\",\"40GeV\",$initfile,$endfile,1.00,1.00,1.00\)
root -l -b -q draw_qg_emfrac_jet4.C\(\"SS310\",\"40GeV\",$initfile,$endfile,1.00,2.59,1.04\)
### 50GeV
#root -l -b -q draw_qg_emfrac_jet4.C\(\"SS310\",\"50GeV\",$initfile,$endfile,1.00,1.00,1.00\)
#root -l -b -q draw_qg_emfrac_jet4.C\(\"SS310\",\"50GeV\",$initfile,$endfile,1.00,2.84,1.02\)

######### Al
### 20GeV
#root -l -b -q draw_qg_emfrac_jet4.C\(\"Al\",\"20GeV\",$initfile,$endfile,1.00,1.00,1.00\)
#root -l -b -q draw_qg_emfrac_jet4.C\(\"Al\",\"20GeV\",$initfile,$endfile,1.00,6.43,1.00\)
#root -l -b -q draw_qg_emfrac_jet4.C\(\"Al\",\"20GeV\",$initfile,$endfile,1.00,5.40,0.84\)
### 50GeV
#root -l -b -q draw_qg_emfrac_jet4.C\(\"Al\",\"50GeV\",$initfile,$endfile,1.00,1.00,1.00\)
#root -l -b -q draw_qg_emfrac_jet4.C\(\"Al\",\"50GeV\",$initfile,$endfile,1.00,7.75,1.08\)

echo " *** DONE *** "
