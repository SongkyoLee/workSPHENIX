#!/bin/csh
#void draw_2D_JES_emfrac(string ihcalType = "SS310",
#                    string jetE = "50GeV",
#                    string inDir = "outJet4Matched",
#                    string outDir = "outJet4_2DJESemfrac",
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
## 1) nothing
root -l -b -q draw_2D_JES_emfrac.C\(\"SS310\",\"50GeV\",\"outJet4Matched\",\"outJet4_2DJESemfrac\",$initfile,$endfile,1.00,1.00,1.00,1.00\)
## 3) c_TOT + c_H + c_IH
root -l -b -q draw_2D_JES_emfrac.C\(\"SS310\",\"50GeV\",\"outJet4Matched\",\"outJet4_2DJESemfrac\",$initfile,$endfile,1.00,1.00,1.28,2.78\)

##### Al
