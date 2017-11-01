#!/bin/csh
###void draw_roofit_outerTwo(string outDir = "outScaleTrackP", string inType = "SS310_25GeV", string mipcutDir="innerTwoMipCut", bool ohcalScale=false, bool do_doubleGaus = true)

#foreach outdir (outScaleTruthP outScaleTrackP)
foreach outdir (outScaleTrackP)
  echo "***Run for" $outdir
  foreach intype (SS310_25GeV Al_25GeV SS310_10GeV Al_10GeV)
#  foreach intype (SS310_25GeV Al_25GeV)
    echo $intype
    foreach cutdir (cemcMipCut)
      root -l -b -q 'draw_roofit_outerTwo.C("'$outdir'","'$intype'","'$cutdir'",false,true)'
#      root -l -b -q 'draw_roofit_outerTwo.C("'$outdir'","'$intype'","'$cutdir'",true,true)'
    end
  end
end

