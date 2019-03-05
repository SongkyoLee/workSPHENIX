#!/bin/csh
#void draw_2D_JtoG_emfrac(string ihcalType = "SS310",
#                    string jetE = "50GeV",
#                    string inDir = "outJet4Matched",
#                    string outDir = "outJet4_2DJtoGemfrac",
#                    int initfile=0,
#                    int endfile =1000,
#                    float c_tot=1.00,
#                    float c_em=1.00,
#                    float c_h = 1.00,
#                    float c_ih=1.00)

### E = c_tot ( c_em * E_cemc + c_h ( c_ih * C_ihcal + C_ohcal) )
set initfile = 0
set endfile = 1000

##### SS310
##
#root -l -b -q draw_2D_JtoG_emfrac.C\(\"SS310\",\"20GeV\",\"outJet4Matched\",\"outJet4_2DJtoGemfrac\",$initfile,$endfile,1.00,1.00,1.00,1.00\)
#root -l -b -q draw_2D_JtoG_emfrac.C\(\"SS310\",\"20GeV\",\"outJet4Matched\",\"outJet4_2DJtoGemfrac\",$initfile,$endfile,1.25,1.00,1.17,2.34\)
##
root -l -b -q draw_2D_JtoG_emfrac.C\(\"SS310\",\"50GeV\",\"outJet4Matched\",\"outJet4_2DJtoGemfrac\",$initfile,$endfile,1.00,1.00,1.00,1.00\)
root -l -b -q draw_2D_JtoG_emfrac.C\(\"SS310\",\"50GeV\",\"outJet4Matched\",\"outJet4_2DJtoGemfrac\",$initfile,$endfile,1.00,1.00,1.02,2.78\)

##### Al
