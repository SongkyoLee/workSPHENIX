#!/bin/csh

###Usage: ./run_final_qcdjet.sh >&log_final_qcd&

set initfile = 0
set endfile = 1000

#void final_qcdjet(string sampleType = "QCDJet",
#                              string ihcalType = "SS310",
#                              string jetE = "50GeV",
#                              int initfile=0,
#                              int endfile =1000,
#                              float total_sf=1.00,
#                              float cemc_sf=1.00,
#                              float ihcal_sf = 1.00,
#                              float ohcal_sf=1.00,
#                              bool doetopcut=true,
#                              bool dodphicut=true)

##########################################################################################################
##########################################################################################################
##########################################################################################################

## before
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"20GeV\",$initfile,$endfile,1.00,1.00,1.00,1.00,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"30GeV\",$initfile,$endfile,1.00,1.00,1.00,1.00,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"40GeV\",$initfile,$endfile,1.00,1.00,1.00,1.00,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"50GeV\",$initfile,$endfile,1.00,1.00,1.00,1.00,true,true\)
## method01: asymm 
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"20GeV\",$initfile,$endfile,1.00,1.75,3.71,1.65,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"30GeV\",$initfile,$endfile,1.00,1.37,2.97,1.56,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"40GeV\",$initfile,$endfile,1.00,1.19,2.74,1.59,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"50GeV\",$initfile,$endfile,1.00,1.16,2.57,1.58,true,true\)
## method02: over_recojet
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"20GeV\",$initfile,$endfile,1.00,2.22,4.44,2.15,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"30GeV\",$initfile,$endfile,1.00,1.60,3.21,2.00,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"40GeV\",$initfile,$endfile,1.00,1.27,3.05,1.83,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"50GeV\",$initfile,$endfile,1.00,1.25,2.71,1.78,true,true\)
## method03: over_recogamma
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"20GeV\",$initfile,$endfile,1.00,1.51,2.31,1.52,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"30GeV\",$initfile,$endfile,1.00,1.31,2.14,1.51,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"40GeV\",$initfile,$endfile,1.00,1.20,2.19,1.51,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"50GeV\",$initfile,$endfile,1.00,1.15,2.19,1.51,true,true\)
## method04: hybrid
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"20GeV\",$initfile,$endfile,1.00,1.43,2.78,1.29,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"30GeV\",$initfile,$endfile,1.00,1.31,2.53,1.31,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"40GeV\",$initfile,$endfile,1.00,1.17,2.52,1.38,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"50GeV\",$initfile,$endfile,1.00,1.13,2.45,1.42,true,true\)
## method05 : asymm_manual
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"20GeV\",$initfile,$endfile,1.00,1.76,3.64,1.68,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"30GeV\",$initfile,$endfile,1.00,1.40,2.97,1.54,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"40GeV\",$initfile,$endfile,1.00,1.21,2.82,1.55,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"50GeV\",$initfile,$endfile,1.00,1.18,2.63,1.52,true,true\)

##########################################################################################################
##########################################################################################################
##########################################################################################################

## before
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"Alframe\",\"20GeV\",$initfile,$endfile,1.00,1.00,1.00,1.00,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"Alframe\",\"30GeV\",$initfile,$endfile,1.00,1.00,1.00,1.00,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"Alframe\",\"40GeV\",$initfile,$endfile,1.00,1.00,1.00,1.00,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"Alframe\",\"50GeV\",$initfile,$endfile,1.00,1.00,1.00,1.00,true,true\)
## method01: asymm 
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"Alframe\",\"20GeV\",$initfile,$endfile,1.00,2.84,1.00,2.54,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"Alframe\",\"30GeV\",$initfile,$endfile,1.00,2.09,1.00,2.11,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"Alframe\",\"40GeV\",$initfile,$endfile,1.00,1.73,1.00,2.06,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"Alframe\",\"50GeV\",$initfile,$endfile,1.00,1.54,1.00,1.99,true,true\)
## method02: over_recojet
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"Alframe\",\"20GeV\",$initfile,$endfile,1.00,3.66,1.00,3.05,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"Alframe\",\"30GeV\",$initfile,$endfile,1.00,2.30,1.00,2.56,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"Alframe\",\"40GeV\",$initfile,$endfile,1.00,1.88,1.00,2.40,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"Alframe\",\"50GeV\",$initfile,$endfile,1.00,1.77,1.00,2.20,true,true\)
## method03: over_recogamma
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"Alframe\",\"20GeV\",$initfile,$endfile,1.00,2.05,1.00,1.76,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"Alframe\",\"30GeV\",$initfile,$endfile,1.00,1.62,1.00,1.78,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"Alframe\",\"40GeV\",$initfile,$endfile,1.00,1.47,1.00,1.79,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"Alframe\",\"50GeV\",$initfile,$endfile,1.00,1.40,1.00,1.78,true,true\)
## method04: hybrid // identical to method3!!!!
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"Alframe\",\"20GeV\",$initfile,$endfile,1.00,2.05,1.00,1.76,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"Alframe\",\"30GeV\",$initfile,$endfile,1.00,1.62,1.00,1.78,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"Alframe\",\"40GeV\",$initfile,$endfile,1.00,1.47,1.00,1.79,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"Alframe\",\"50GeV\",$initfile,$endfile,1.00,1.40,1.00,1.78,true,true\)
## method05 : asymm_manual
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"Alframe\",\"20GeV\",$initfile,$endfile,1.00,2.85,1.00,2.51,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"Alframe\",\"30GeV\",$initfile,$endfile,1.00,2.10,1.00,2.11,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"Alframe\",\"40GeV\",$initfile,$endfile,1.00,1.77,1.00,2.03,true,true\)
root -l -b -q final_qcdjet.C\(\"QCDJet\",\"Alframe\",\"50GeV\",$initfile,$endfile,1.00,1.58,1.00,1.97,true,true\)

echo "***** All Done"  
