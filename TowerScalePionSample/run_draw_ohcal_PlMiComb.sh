#!/bin/csh
###void draw_roofit_ohcal_PlMiComb(string outDir = "outScaleTrackP", string inType = "SS310_25GeV", bool do_doubleGaus = true, bool ohcalScaled =false)

foreach outdir (outScaleTruthP outScaleTrackP)
  echo $outdir
  foreach intype (SS310_25GeV Al_25GeV SS310_10GeV Al_10GeV)
  echo $intype
  ###2Gauss
  ## ohcalScaled=0
  root -l -b -q 'draw_roofit_ohcal_PlMiComb.C("'$outdir'","'$intype'",true,false)'
  ## ohcalScaled=1
  #root -l -b -q 'draw_roofit_ohcal_PlMiComb.C("'$outdir'","'$intype'",true,true)'
  ###1Gauss
  ## ohcalScaled=0
  root -l -b -q 'draw_roofit_ohcal_PlMiComb.C("'$outdir'","'$intype'",false,false)'
  ## ohcalScaled=1
  #root -l -b -q 'draw_roofit_ohcal_PlMiComb.C("'$outdir'","'$intype'",false,true)'
  end
end

