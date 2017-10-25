#!/bin/csh
set event=500
#root -l -b -q 'rootAna_ohcalScale.C("SS310_PiPl_25GeV",0,'$event',0.5,0.3)'
#root -l -b -q 'rootAna_ohcalScale.C("SS310_PiMi_25GeV",0,'$event',0.5,0.3)'
#root -l -b -q 'rootAna_ohcalScale.C("Al_PiPl_25GeV",0,'$event',0.5,0.15)'
#root -l -b -q 'rootAna_ohcalScale.C("Al_PiMi_25GeV",0,'$event',0.5,0.15)'

root -l -b -q 'rootAna_ihcalScale.C("SS310_PiPl_25GeV",0,'$event',0.5,531.)'
#root -l -b -q 'rootAna_ihcalScale.C("SS310_PiMi_25GeV",0,'$event',0.5,531.)'
root -l -b -q 'rootAna_ihcalScale.C("Al_PiPl_25GeV",0,'$event',0.5,531.)'
#root -l -b -q 'rootAna_ihcalScale.C("Al_PiMi_25GeV",0,'$event',0.5,531.)'
