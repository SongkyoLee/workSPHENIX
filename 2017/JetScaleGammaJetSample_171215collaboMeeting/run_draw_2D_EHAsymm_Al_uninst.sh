#!/bin/csh
#void draw_2D_EHAsymm_Al_uninst(string ihcalType = "Al_uninst",
#                              string jetE = "50GeV",
#                              string outDir = "outJet4Matched",
#                              int initfile=0,
#                              int endfile =1000,
#                              int ver =2)

set initfile = 0
set endfile = 1000

foreach prefix (Al_uninst)
  echo "***** Run" $prefix
  foreach energy (20GeV 30GeV 40GeV 50GeV)
#  foreach energy (50GeV)
#    root -l -b -q draw_2D_EHAsymm_Al_uninst.C\(\"$prefix\",\"$energy\",\"outJet4Matched\",\"outJet4_2DEHAsymm_Al_uninst\",$initfile,$endfile,1\)
    root -l -b -q draw_2D_EHAsymm_Al_uninst.C\(\"$prefix\",\"$energy\",\"outJet4Matched\",\"outJet4_2DEHAsymm_Al_uninst\",$initfile,$endfile,3\)
#    root -l -b -q draw_2D_EHAsymm_Al_uninst.C\(\"$prefix\",\"$energy\",\"outJet4Matched\",\"outJet4_2DEHAsymm_Al_uninst\",$initfile,$endfile,4\)
#    root -l -b -q draw_2D_EHAsymm_Al_uninst.C\(\"$prefix\",\"$energy\",\"outJet4Matched\",\"outJet4_2DEHAsymm_Al_uninst\",$initfile,$endfile,5\)
  end
end
  
