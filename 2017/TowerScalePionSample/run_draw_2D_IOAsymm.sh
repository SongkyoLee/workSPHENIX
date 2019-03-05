#!/bin/csh
#void draw_2D_IOAsymm(string outDir = "out2D_IOAsymm",
#                           string inType = "SS310_25GeV", 
#                           bool do_cemcScale=false,
#                           bool do_ihcalScale=false,
#                           bool do_ohcalScale=false,
#                           bool do_cemcMipCut=false,
#                           bool do_ihcalMipCut=false,
#                           bool do_totMipCut=false,
#                           bool do_profile=true)


foreach outdir (out2D_IOAsymm)
  echo "***Run for" $outdir
  foreach intype (SS310_25GeV Al_25GeV SS310_10GeV Al_10GeV)
#  foreach intype (SS310_25GeV SS310_10GeV)
    echo $intype
    ### NoScale
    root -l -b -q 'draw_2D_IOAsymm.C("'$outdir'","'$intype'",false,false,false,false,false,false,true)'
    ### IHCALscale
    #root -l -b -q 'draw_2D_IOAsymm.C("'$outdir'","'$intype'",false,true,false,false,false,false,true)'
  end
end
