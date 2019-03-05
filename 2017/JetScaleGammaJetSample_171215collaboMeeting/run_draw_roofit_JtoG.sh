#!/bin/csh
#void draw_roofit_JtoG(string ihcalType = "SS310",
#                    string jetE = "50GeV",
#                    string inDir = "outJet4Matched",
#                    string outDir = "outJet4JtoG",
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
### 20GeV
root -l -b -q draw_roofit_JtoG.C\(\"SS310\",\"20GeV\",\"outJet4Matched\",\"outJet4JtoG\",$initfile,$endfile,1.00,1.00,1.00,1.00\)
root -l -b -q draw_roofit_JtoG.C\(\"SS310\",\"20GeV\",\"outJet4Matched\",\"outJet4JtoG\",$initfile,$endfile,1.00,1.00,1.17,2.34\)
root -l -b -q draw_roofit_JtoG.C\(\"SS310\",\"20GeV\",\"outJet4Matched\",\"outJet4JtoG\",$initfile,$endfile,1.25,1.00,1.17,2.34\)
### 50GeV
root -l -b -q draw_roofit_JtoG.C\(\"SS310\",\"50GeV\",\"outJet4Matched\",\"outJet4JtoG\",$initfile,$endfile,1.00,1.00,1.00,1.00\)
root -l -b -q draw_roofit_JtoG.C\(\"SS310\",\"50GeV\",\"outJet4Matched\",\"outJet4JtoG\",$initfile,$endfile,1.00,1.00,1.28,2.78\)

##### Al
### 20GeV
root -l -b -q draw_roofit_JtoG.C\(\"Al\",\"20GeV\",\"outJet4Matched\",\"outJet4JtoG\",$initfile,$endfile,1.00,1.00,1.00,1.00\)
root -l -b -q draw_roofit_JtoG.C\(\"Al\",\"20GeV\",\"outJet4Matched\",\"outJet4JtoG\",$initfile,$endfile,1.00,1.00,1.04,6.43\)
root -l -b -q draw_roofit_JtoG.C\(\"Al\",\"20GeV\",\"outJet4Matched\",\"outJet4JtoG\",$initfile,$endfile,1.25,1.00,1.04,6.43\)
### 50GeV
root -l -b -q draw_roofit_JtoG.C\(\"Al\",\"50GeV\",\"outJet4Matched\",\"outJet4JtoG\",$initfile,$endfile,1.00,1.00,1.00,1.00\)
root -l -b -q draw_roofit_JtoG.C\(\"Al\",\"50GeV\",\"outJet4Matched\",\"outJet4JtoG\",$initfile,$endfile,1.00,1.00,1.27,7.18\)

