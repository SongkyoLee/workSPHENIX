#!/bin/csh

set initfile=0
#set endfile=500
set endfile=200
#set endfile=50

set cemccut=0.5
set ss_ihcalcut=0.3
set al_ihcalcut=0.15
### ohcalcut: only used to fill nomip histos!!! (applied to all step)
### skip if cemcmip && ihcalmip && 80% of true_p is stored in black holes
#set ohcalcut_25gev=5.0 # only used to fill nomip histos!!! (applied to all step)
#set ohcalcut_10gev=2.0 # only used to fill nomip histos!!! (applied to all step)
set ohcalcut_25gev=2.0 # only used to fill nomip histos!!! (applied to all step)
set ohcalcut_10gev=1.2 # only used to fill nomip histos!!! (applied to all step)
set nocut=9999.0

###########################################################
########### STEP 1) NO scale + cemc mip cut + ihcal mip cut
###########################################################

### 25 GeV
root -l -b -q 'rootAna_scale.C("SS310_PiPl_25GeV",'$initfile,''$endfile',false,false,false,'$cemccut','$ss_ihcalcut','$ohcalcut_25gev')'
root -l -b -q 'rootAna_scale.C("SS310_PiMi_25GeV",'$initfile,''$endfile',false,false,false,'$cemccut','$ss_ihcalcut','$ohcalcut_25gev')'
root -l -b -q 'rootAna_scale.C("Al_PiPl_25GeV",'$initfile,''$endfile',false,false,false,'$cemccut','$al_ihcalcut','$ohcalcut_25gev')'
root -l -b -q 'rootAna_scale.C("Al_PiMi_25GeV",'$initfile,''$endfile',false,false,false,'$cemccut','$al_ihcalcut','$ohcalcut_25gev')'
### 10 GeV
root -l -b -q 'rootAna_scale.C("SS310_PiPl_10GeV",'$initfile,''$endfile',false,false,false,'$cemccut','$ss_ihcalcut','$ohcalcut_10gev')'
root -l -b -q 'rootAna_scale.C("SS310_PiMi_10GeV",'$initfile,''$endfile',false,false,false,'$cemccut','$ss_ihcalcut','$ohcalcut_10gev')'
root -l -b -q 'rootAna_scale.C("Al_PiPl_10GeV",'$initfile,''$endfile',false,false,false,'$cemccut','$al_ihcalcut','$ohcalcut_10gev')'
root -l -b -q 'rootAna_scale.C("Al_PiMi_10GeV",'$initfile,''$endfile',false,false,false,'$cemccut','$al_ihcalcut','$ohcalcut_10gev')'

###########################################################
########### STEP 2) ohcal scale + cemc mip cut
###########################################################

### 25 GeV
root -l -b -q 'rootAna_scale.C("SS310_PiPl_25GeV",'$initfile,''$endfile',false,false,true,'$cemccut','$nocut','$ohcalcut_25gev')'
root -l -b -q 'rootAna_scale.C("SS310_PiMi_25GeV",'$initfile,''$endfile',false,false,true,'$cemccut','$nocut','$ohcalcut_25gev')'
root -l -b -q 'rootAna_scale.C("Al_PiPl_25GeV",'$initfile,''$endfile',false,false,true,'$cemccut','$nocut','$ohcalcut_25gev')'
root -l -b -q 'rootAna_scale.C("Al_PiMi_25GeV",'$initfile,''$endfile',false,false,true,'$cemccut','$nocut','$ohcalcut_25gev')'
### 10 GeV
root -l -b -q 'rootAna_scale.C("SS310_PiPl_10GeV",'$initfile,''$endfile',false,false,true,'$cemccut','$nocut','$ohcalcut_10gev')'
root -l -b -q 'rootAna_scale.C("SS310_PiMi_10GeV",'$initfile,''$endfile',false,false,true,'$cemccut','$nocut','$ohcalcut_10gev')'
root -l -b -q 'rootAna_scale.C("Al_PiPl_10GeV",'$initfile,''$endfile',false,false,true,'$cemccut','$nocut','$ohcalcut_10gev')'
root -l -b -q 'rootAna_scale.C("Al_PiMi_10GeV",'$initfile,''$endfile',false,false,true,'$cemccut','$nocut','$ohcalcut_10gev')'

###########################################################
########### STEP 3) ihcal scale + ohcal scale + NO mip cut
###########################################################

