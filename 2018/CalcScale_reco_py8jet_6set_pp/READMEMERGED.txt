
########## 1) Make 2nd-sorted rootfiles

root -l -b -q 'rootAna_2ndSort_qcdjet_ptbinning_merged.C("QCDJet","Al","noGeV","R04",0,400,true,true)'

########## 2) Get scales using minuit

./run_make_log_before_merged.sh <- to make log files of "before scaling"
./run_minuit_truthjet_merged.sh <- new method (cemcScale, ihcalScale, and ohcalScale)
( --->log files will be saved in logs_merged/***) 

########## 3) Apply scales to dijet samples

*** for scale
./run_final_qcdjet_merged.sh
( ->log files will be saved in logs_merged/***) 
( ->pdf and root files will be saved in outFinalXXXMerged/***) 

*** for JES and JER
./run_final_qcdjet_RecoToTrue_merged.sh
( ->log files will be saved in logs_RecoToTrue_merged/***) 
( ->pdf and root files will be saved in outFinalXXXRecoToTrueMerged/***) 

########## 4-1) Read logs/*** and Draw scales vs E 

./getScaleMerged.sh
( ->txt files will be saved in txt_scale_merged/***) 
 root -l -b -q 'draw_SFvsE_merged.C("Al","R04")';
( ->pdf files will be saved in outvsEMerged/***) 

########## 4-2) Read logs/*** and Draw JES and JER vs E 

./getJESJERMerged.sh
( ->txt files will be saved in txt_jet_merged/***) 
 root -l -b -q 'draw_JESJERvsE_merged.C("Al","R04")';
( ->pdf files will be saved in outvsEMerged/***) 

./getJESJERRecoToTrueMerged.sh
( ->txt files will be saved in txt_jet_RecoToTrue_merged/***) 
 root -l -b -q 'draw_JESJERvsE_RecoToTrue_merged.C("Al","R04")';
( ->pdf files will be saved in outvsERecoToTrueMerged/***) 

