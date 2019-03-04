#!/bin/csh
#void draw_roofit_totalE(string outDir = "outMergedPdfs",
#                           string inType = "SS310_25GeV", 
#                           bool do_cemcScale=false,
#                           bool do_ihcalScale=false,
#                           bool do_totalScale=false,
#                           bool do_cemcMipCut=false,
#                           bool do_ihcalMipCut=false,
#                           bool do_totMipCut=false,
#                           bool do_doubleGaus = true)

foreach outdir (outTotalE)
  echo "***Run for" $outdir
  foreach intype (SS310_25GeV Al_25GeV SS310_10GeV Al_10GeV)
#  foreach intype (SS310_25GeV SS310_10GeV)
    echo $intype
    ### 1) NOScale
    #root -l -b -q 'draw_roofit_totalE.C("'$outdir'","'$intype'",false,false,false,false,false,false,true)'
    ### 2) CEMCScale + IHCALScale
    #root -l -b -q 'draw_roofit_totalE.C("'$outdir'","'$intype'",true,true,false,false,false,false,true)'
    ### 3) CEMCScale + IHCALScale + OHCALScale
    root -l -b -q 'draw_roofit_totalE.C("'$outdir'","'$intype'",true,true,true,false,false,false,true)'
  end
end

