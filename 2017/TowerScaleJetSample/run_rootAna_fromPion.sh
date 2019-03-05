#!/bin/csh

set initfile = 0
#set endfile = 1000
set endfile = 300
#set endfile = 50

## MyTree_SS310_NotScaled: 600
## MyTree_Al_NotScaled: 636
## 100MeV_Al_NotScaled_v3: 420
## OnlyHcalScaled: 583

#foreach prefix (Jet_SS25Scale Jet_Al25Scale Jet_SS10Scale Jet_Al10Scale Jet_SS25ScaleNoCEMC Jet_Al25ScaleNoCEMC)
foreach prefix (Jet_SS25Scale Jet_Al25Scale Jet_SS25ScaleNoCEMC Jet_Al25ScaleNoCEMC)
  echo "***** Run" $prefix
  root -l -b -q rootAna_emfrac_fromPion.C\(\"$prefix\",$initfile,$endfile\)
end

