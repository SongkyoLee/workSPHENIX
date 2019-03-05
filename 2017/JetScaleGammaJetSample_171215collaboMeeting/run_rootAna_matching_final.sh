#!/bin/csh

### E = c_tot ( c_em * E_cemc + c_h ( c_ih * C_ihcal + C_ohcal) )
set initfile = 0
set endfile = 1000

##### SS310
### 20GeV
#root -l -b -q rootAna_matching_jet4.C\(\"SS310\",\"20GeV\",\"outJet4Matched\",$initfile,$endfile,1.25,1.00,1.17,2.34\)
##### Al
### 20GeV
root -l -b -q rootAna_matching_jet4.C\(\"Al\",\"20GeV\",\"outJet4Matched\",$initfile,$endfile,1.25,1.00,1.04,6.43\)

