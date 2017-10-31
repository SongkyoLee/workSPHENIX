#!/bin/csh

set initfile=0
#set endfile=500
set endfile=200 ##default
#set endfile=30

set cemccut=0.5
set ss_ihcalcut=0.3
set al_ihcalcut=0.15
#set totmipcut_25gev=2.0 # only used to fill nomip histos!!!
#set totmipcut_10gev=1.2 # only used to fill nomip histos!!!
set totmipcut_25gev=4.5 # only used to fill nomip histos!!!
set totmipcut_10gev=1.5 # only used to fill nomip histos!!!
set nocut=9999.0

###########################################################
########### STEP 1) NO scale + cemc mip cut + ihcal mip cut
###########################################################

### 25 GeV
#root -l -b -q 'rootAna_scale.C("SS310_PiPl_25GeV",'$initfile,''$endfile',false,false,false,'$cemccut','$ss_ihcalcut','$totmipcut_25gev')'
#root -l -b -q 'rootAna_scale.C("SS310_PiMi_25GeV",'$initfile,''$endfile',false,false,false,'$cemccut','$ss_ihcalcut','$totmipcut_25gev')'
#root -l -b -q 'rootAna_scale.C("Al_PiPl_25GeV",'$initfile,''$endfile',false,false,false,'$cemccut','$al_ihcalcut','$totmipcut_25gev')'
#root -l -b -q 'rootAna_scale.C("Al_PiMi_25GeV",'$initfile,''$endfile',false,false,false,'$cemccut','$al_ihcalcut','$totmipcut_25gev')'
### 10 GeV
#root -l -b -q 'rootAna_scale.C("SS310_PiPl_10GeV",'$initfile,''$endfile',false,false,false,'$cemccut','$ss_ihcalcut','$totmipcut_10gev')'
#root -l -b -q 'rootAna_scale.C("SS310_PiMi_10GeV",'$initfile,''$endfile',false,false,false,'$cemccut','$ss_ihcalcut','$totmipcut_10gev')'
#root -l -b -q 'rootAna_scale.C("Al_PiPl_10GeV",'$initfile,''$endfile',false,false,false,'$cemccut','$al_ihcalcut','$totmipcut_10gev')'
#root -l -b -q 'rootAna_scale.C("Al_PiMi_10GeV",'$initfile,''$endfile',false,false,false,'$cemccut','$al_ihcalcut','$totmipcut_10gev')'

###########################################################
########### STEP 2) ohcal scale + cemc mip cut
###########################################################

### 25 GeV
root -l -b -q 'rootAna_scale.C("SS310_PiPl_25GeV",'$initfile,''$endfile',false,false,true,'$cemccut','$ss_ihcalcut','$totmipcut_25gev')'
root -l -b -q 'rootAna_scale.C("SS310_PiMi_25GeV",'$initfile,''$endfile',false,false,true,'$cemccut','$ss_ihcalcut','$totmipcut_25gev')'
root -l -b -q 'rootAna_scale.C("Al_PiPl_25GeV",'$initfile,''$endfile',false,false,true,'$cemccut','$al_ihcalcut','$totmipcut_25gev')'
root -l -b -q 'rootAna_scale.C("Al_PiMi_25GeV",'$initfile,''$endfile',false,false,true,'$cemccut','$al_ihcalcut','$totmipcut_25gev')'
### 10 GeV
root -l -b -q 'rootAna_scale.C("SS310_PiPl_10GeV",'$initfile,''$endfile',false,false,true,'$cemccut','$ss_ihcalcut','$totmipcut_10gev')'
root -l -b -q 'rootAna_scale.C("SS310_PiMi_10GeV",'$initfile,''$endfile',false,false,true,'$cemccut','$ss_ihcalcut','$totmipcut_10gev')'
root -l -b -q 'rootAna_scale.C("Al_PiPl_10GeV",'$initfile,''$endfile',false,false,true,'$cemccut','$al_ihcalcut','$totmipcut_10gev')'
root -l -b -q 'rootAna_scale.C("Al_PiMi_10GeV",'$initfile,''$endfile',false,false,true,'$cemccut','$al_ihcalcut','$totmipcut_10gev')'

###########################################################
########### STEP 3) ihcal scale + ohcal scale + NO mip cut
###########################################################

