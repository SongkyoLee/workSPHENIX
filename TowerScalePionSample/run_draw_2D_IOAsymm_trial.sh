#!/bin/csh
#void draw_2D_IOAsymm_trial(string outDir = "out2D_IOAsymm_Trial",
#                           string inType = "SS310_25GeV", 
#                           bool do_cemcScale=false,
#                           bool do_ihcalScale=false,
#                           bool do_ohcalScale=false,
#                           bool do_cemcMipCut=false,
#                           bool do_ihcalMipCut=false,
#                           bool do_totMipCut=false,
#                           bool do_profile=true,
#                           float ihcal_sf = 1.)

set ihcalsf = 1.3

#foreach outdir (out2D_IOAsymm_Trial)
#  echo "***Run for" $outdir
  #foreach intype (SS310_25GeV SS310_10GeV)
#    echo $intype
    ### IHCALScale
    ###root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,'$ihcalsf')'
    #root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,1.00)'
    #root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,1.30)'
    #root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,1.60)'
    #root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,1.70)'
    #root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,1.75)'
    #root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,1.80)'
    #root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,1.85)'
    #root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,1.90)'
    #root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,2.20)'
#    root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,2.50)'
#  end
#end

foreach outdir (out2D_IOAsymm_Trial)
  echo "***Run for" $outdir
  foreach intype (Al_25GeV Al_10GeV)
    echo $intype
    ### IHCALScale
    ###root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,'$ihcalsf')'
    #root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,1.00)'
    #root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,2.00)'
    #root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,3.00)'
    #root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,3.50)'
    #root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,4.00)'
    #root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,4.25)'
    #root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,4.50)'
    #root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,4.75)'
    #root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,5.00)'
    #root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,5.25)'
    #root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,5.50)'
    #root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,5.75)'
    #root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,6.00)'
    #root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,6.50)'
    #root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,7.00)'
    root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,4.36)'
#    root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,4.37)'
#    root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,4.38)'
#    root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,4.39)'
#    root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,5.73)'
#    root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,5.74)'
#    root -l -b -q 'draw_2D_IOAsymm_trial.C("'$outdir'","'$intype'",false,true,false,false,false,false,true,5.75)'
  end
end

