#!/bin/csh

#foreach prefix (Jet_SS25Scale Jet_Al25Scale Jet_SS10Scale Jet_Al10Scale Jet_SS25ScaleNoCEMC Jet_Al25ScaleNoCEMC)
foreach prefix (Jet_SS25Scale Jet_Al25Scale Jet_SS25ScaleNoCEMC Jet_Al25ScaleNoCEMC)
  echo "***** Run" $prefix
  root -l -b -q draw_emfrac_fromPion.C\(\"$prefix\"\)
end

