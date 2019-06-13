
########## 1) Make 2nd-sorted rootfiles

###./run_rootAna_2ndSort_gammajet.sh (NU)
###./run_rootAna_2ndSort_qcdjet.sh (Nu)
./run_rootAna_2ndSort_qcdjet_ptbinning.sh (Gen Slices should be in pT)

########## 2) Get scales using minuit

./run_make_log_before.sh <- to make log files of "before scaling"
###./run_minuit_recogamma.sh <- old method: not used now!!
./run_minuit_truthjet.sh <- new method (cemcScale, ihcalScale, and ohcalScale)
###./run_minuit_truthjet_2par.sh <- new method (ihcalScale and ohcalScale) 
( --->log files will be saved in logs/***) 

########## 3) Apply scales to dijet samples

./run_final_qcdjet.sh
( ->log files will be saved in logs/***) 
( ->pdf and root files will be saved in outFinalXXX/***) 

########## 4-1) Read logs/*** and Draw scales vs E 

./getScale.sh
( ->txt files will be saved in txt_scale/***) 
 root -l -b -q 'draw_SFvsE.C("SS310","R02")';
 root -l -b -q 'draw_SFvsE.C("SS310","R04")';
 root -l -b -q 'draw_SFvsE.C("Al","R02")';
 root -l -b -q 'draw_SFvsE.C("Al","R04")';
 root -l -b -q 'draw_SFvsE.C("Alframe","R02")';
 root -l -b -q 'draw_SFvsE.C("Alframe","R04")';
( ->pdf files will be saved in outvsE/***) 

########## 4-2) Read logs/*** and Draw JES and JER vs E 

./getJESJER.sh
( ->txt files will be saved in txt_jet/***) 
 root -l -b -q 'draw_JESJERvsE.C("SS310","R02")';
 root -l -b -q 'draw_JESJERvsE.C("SS310","R04")';
 root -l -b -q 'draw_JESJERvsE.C("Al","R02")';
 root -l -b -q 'draw_JESJERvsE.C("Al","R04")';
 root -l -b -q 'draw_JESJERvsE.C("Alframe","R02")';
 root -l -b -q 'draw_JESJERvsE.C("Alframe","R04")';
( ->pdf files will be saved in outvsE/***) 

### overlay SS310,Al,Alframe 
 root -l -b -q 'draw_JESJERvsE_overlay.C("R02")';
 root -l -b -q 'draw_JESJERvsE_overlay.C("R04")';

########## 4-3) Read outFinalXXX/***.root and Draw forHallman 

N.B.! final_qcd should be run for "before" and "truthjet" only, because (1,1,1) are stored as "before", and (X,X,X) are stored as "truthjet".
e.g. "truthjet_2par", etc. will be stored as "truthjet" as well, and ruin the draw_forHallman.C

./run_forHallman.sh
( ->pdf files will be saved in outHallman/***) 

########## 5) Read txt_scale and txt_jet and make log files with total_sf

./run_get_total_sf.sh
( --->log files will be saved in logs_wTot/***) 

########## 6) Apply scales to dijet samples

./run_final_qcdjet_wTot.sh
( ->log files will be saved in logs_wTot/***) 
( ->pdf and root files will be saved in outFinalXXX/***) 

########## 7-1) Read logs_wTot/*** and Draw scales vs E 

./getScale_wTot.sh
( ->txt files will be saved in txt_scale_wTot/***) 
 root -l -b -q 'draw_SFvsE_wTot.C("SS310","R02")';
 root -l -b -q 'draw_SFvsE_wTot.C("SS310","R04")';
 root -l -b -q 'draw_SFvsE_wTot.C("Al","R02")';
 root -l -b -q 'draw_SFvsE_wTot.C("Al","R04")';
 root -l -b -q 'draw_SFvsE_wTot.C("Alframe","R02")';
 root -l -b -q 'draw_SFvsE_wTot.C("Alframe","R04")';
( ->pdf files will be saved in outvsE_wTot/***) 

########## 7-2) Read logs_wTot/*** and Draw JES and JER vs E 

./getJESJER_wTot.sh
( ->txt files will be saved in txt_jet_wTot/***) 
 root -l -b -q 'draw_JESJERvsE_wTot.C("SS310","R02")';
 root -l -b -q 'draw_JESJERvsE_wTot.C("SS310","R04")';
 root -l -b -q 'draw_JESJERvsE_wTot.C("Al","R02")';
 root -l -b -q 'draw_JESJERvsE_wTot.C("Al","R04")';
 root -l -b -q 'draw_JESJERvsE_wTot.C("Alframe","R02")';
 root -l -b -q 'draw_JESJERvsE_wTot.C("Alframe","R04")';
( ->pdf files will be saved in outvsE_wTot/***) 

### overlay SS310,Al,Alframe 
 root -l -b -q 'draw_JESJERvsE_overlay_wTot.C("R02")';
 root -l -b -q 'draw_JESJERvsE_overlay_wTot.C("R04")';

########## 8) Compare wTot vs. woTot

 root -l -b -q 'comp_JESJERvsE_wTot_woTot.C("SS310","R02")';
 root -l -b -q 'comp_JESJERvsE_wTot_woTot.C("SS310","R04")';
 root -l -b -q 'comp_JESJERvsE_wTot_woTot.C("Al","R02")';
 root -l -b -q 'comp_JESJERvsE_wTot_woTot.C("Al","R04")';
 root -l -b -q 'comp_JESJERvsE_wTot_woTot.C("Alframe","R02")';
 root -l -b -q 'comp_JESJERvsE_wTot_woTot.C("Alframe","R04")';


 ########################################
 ############ systematics ###############
 ########################################

########## 1) Apply scales to dijet samples

(from 01-08 ) 
./run_final_qcdjet_syst01.sh
( ->log files will be saved in logs_syst01/***) 
( ->pdf and root files will be saved in outFinalXXXSyst01/***) 

########## 2-1) Read logs/*** and Draw scales vs E 

./getScaleSyst01.sh
( ->txt files will be saved in txt_scale/***) 
 root -l -b -q 'draw_SFvsE_Syst01.C("SS310","R02")';
 root -l -b -q 'draw_SFvsE_Syst01.C("SS310","R04")';
 root -l -b -q 'draw_SFvsE_Syst01.C("Al","R02")';
 root -l -b -q 'draw_SFvsE_Syst01.C("Al","R04")';
 root -l -b -q 'draw_SFvsE_Syst01.C("Alframe","R02")';
 root -l -b -q 'draw_SFvsE_Syst01.C("Alframe","R04")';
( ->pdf files will be saved in outvsE_Syst01/***) 

########## 2-2) Read logs/*** and Draw JES and JER vs E 

./getJESJERSyst01.sh
( ->txt files will be saved in txt_jet/***) 
 root -l -b -q 'draw_JESJERvsE_syst01.C("SS310","R02")';
 root -l -b -q 'draw_JESJERvsE_syst01.C("SS310","R04")';
 root -l -b -q 'draw_JESJERvsE_syst01.C("Al","R02")';
 root -l -b -q 'draw_JESJERvsE_syst01.C("Al","R04")';
 root -l -b -q 'draw_JESJERvsE_syst01.C("Alframe","R02")';
 root -l -b -q 'draw_JESJERvsE_syst01.C("Alframe","R04")';
( ->pdf files will be saved in outvsE_syst01/***) 

### overlay SS310,Al,Alframe 
 root -l -b -q 'draw_JESJERvsE_overlay.C("R02")';
 root -l -b -q 'draw_JESJERvsE_overlay.C("R04")';


