#!/bin/csh
#void draw_2D_EHAsymm_trial(string outDir = "out2D_EHAsymm_Trial",
#                           string inType = "SS310_25GeV", 
#                           bool do_cemcScale=false,
#                           bool do_ihcalScale=false,
#                           bool do_ohcalScale=false,
#                           bool do_cemcMipCut=false,
#                           bool do_ihcalMipCut=false,
#                           bool do_totMipCut=false,
#                           bool do_profile=true,
#                           float cemc_sf = 1.)

set cemcsf = 1.3

#foreach outdir (out2D_EHAsymm_Trial)
#  echo "***Run for" $outdir
#  foreach intype (SS310_10GeV)
#    echo $intype
#    ### CEMCScale + IHCALScale
#    ###root -l -b -q 'draw_2D_EHAsymm_trial.C("'$outdir'","'$intype'",true,true,false,false,false,false,true,'$ihcalsf')'
#    #root -l -b -q 'draw_2D_EHAsymm_trial.C("'$outdir'","'$intype'",true,true,false,false,false,false,true,1.00)'
#    #root -l -b -q 'draw_2D_EHAsymm_trial.C("'$outdir'","'$intype'",true,true,false,false,false,false,true,1.10)'
#    #root -l -b -q 'draw_2D_EHAsymm_trial.C("'$outdir'","'$intype'",true,true,false,false,false,false,true,1.12)'
#    #root -l -b -q 'draw_2D_EHAsymm_trial.C("'$outdir'","'$intype'",true,true,false,false,false,false,true,1.14)'
#    #root -l -b -q 'draw_2D_EHAsymm_trial.C("'$outdir'","'$intype'",true,true,false,false,false,false,true,1.16)'
#    #root -l -b -q 'draw_2D_EHAsymm_trial.C("'$outdir'","'$intype'",true,true,false,false,false,false,true,1.18)'
#    #root -l -b -q 'draw_2D_EHAsymm_trial.C("'$outdir'","'$intype'",true,true,false,false,false,false,true,1.20)'
#    #root -l -b -q 'draw_2D_EHAsymm_trial.C("'$outdir'","'$intype'",true,true,false,false,false,false,true,1.30)'
#    root -l -b -q 'draw_2D_EHAsymm_trial.C("'$outdir'","'$intype'",true,true,false,false,false,false,true,1.15)'
#    root -l -b -q 'draw_2D_EHAsymm_trial.C("'$outdir'","'$intype'",true,true,false,false,false,false,true,1.17)'
#  end
#end

foreach outdir (out2D_EHAsymm_Trial)
  echo "***Run for" $outdir
  foreach intype (Al_10GeV)
    echo $intype
    ### CEMCScale + IHCALScale
    ###root -l -b -q 'draw_2D_EHAsymm_trial.C("'$outdir'","'$intype'",true,true,false,false,false,false,true,'$ihcalsf')'
    #root -l -b -q 'draw_2D_EHAsymm_trial.C("'$outdir'","'$intype'",true,true,false,false,false,false,true,1.00)'
    #root -l -b -q 'draw_2D_EHAsymm_trial.C("'$outdir'","'$intype'",true,true,false,false,false,false,true,1.10)'
    #root -l -b -q 'draw_2D_EHAsymm_trial.C("'$outdir'","'$intype'",true,true,false,false,false,false,true,1.20)'
    #root -l -b -q 'draw_2D_EHAsymm_trial.C("'$outdir'","'$intype'",true,true,false,false,false,false,true,1.30)'
    #root -l -b -q 'draw_2D_EHAsymm_trial.C("'$outdir'","'$intype'",true,true,false,false,false,false,true,1.40)'
    #root -l -b -q 'draw_2D_EHAsymm_trial.C("'$outdir'","'$intype'",true,true,false,false,false,false,true,1.46)'
    #root -l -b -q 'draw_2D_EHAsymm_trial.C("'$outdir'","'$intype'",true,true,false,false,false,false,true,1.48)'
    #root -l -b -q 'draw_2D_EHAsymm_trial.C("'$outdir'","'$intype'",true,true,false,false,false,false,true,1.50)'
    #root -l -b -q 'draw_2D_EHAsymm_trial.C("'$outdir'","'$intype'",true,true,false,false,false,false,true,1.52)'
    #root -l -b -q 'draw_2D_EHAsymm_trial.C("'$outdir'","'$intype'",true,true,false,false,false,false,true,1.60)'
    #root -l -b -q 'draw_2D_EHAsymm_trial.C("'$outdir'","'$intype'",true,true,false,false,false,false,true,1.70)'
    root -l -b -q 'draw_2D_EHAsymm_trial.C("'$outdir'","'$intype'",true,true,false,false,false,false,true,1.47)'
    root -l -b -q 'draw_2D_EHAsymm_trial.C("'$outdir'","'$intype'",true,true,false,false,false,false,true,1.49)'
  end
end

