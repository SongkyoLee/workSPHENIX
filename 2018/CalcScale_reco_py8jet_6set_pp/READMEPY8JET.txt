
########## 1) Make 2nd-sorted rootfiles

./run_rootAna_2ndSort_py8jet.sh

########## 2) Get scales using minuit

./run_make_log_before_py8jet.sh <- to make log files of "before scaling"
./run_minuit_truthjet_py8jet.sh <- new method (cemcScale, ihcalScale, and ohcalScale)
( --->log files will be saved in logs_py8jet/***) 

########## 3) Apply scales to dijet samples && Read logs/*** and Draw scales vs E 

*** for scale
./run_final_qcdjet_py8jet.sh
( ->log files will be saved in logs_py8jet/***) 
( ->pdf and root files will be saved in outFinalXXXPy8jet/***) 

./getScalePy8jet.sh
( ->txt files will be saved in txt_scale_py8jet/***) 
 root -l -b -q 'draw_SFvsE_py8jet.C("Al","R04")';
 root -l -b -q 'draw_SFvsE_py8jet.C("Alframe","R04")';
( ->pdf files will be saved in outvsEPy8jet/***) 

./getJESJERPy8jet.sh
( ->txt files will be saved in txt_jet_py8jet/***) 
 root -l -b -q 'draw_JESJERvsE_py8jet.C("Al","R04")';
 root -l -b -q 'draw_JESJERvsE_py8jet.C("Alframe","R04")';
( ->pdf files will be saved in outvsEPy8jet/***) 

########## 4) Read logs/*** and Draw JES and JER vs E  

*** for JES and JER (need txt_scale_py8jet as an input!!!!!)
./run_final_qcdjet_RecoToTrue_py8jet.sh
( ->log files will be saved in logs_RecoToTrue_py8jet/***) 
( ->pdf and root files will be saved in outFinalXXXRecoToTruePy8jet/***) 

./getJESJERRecoToTruePy8jet.sh
( ->txt files will be saved in txt_jet_RecoToTrue_py8jet/***) 
 root -l -b -q 'draw_JESJERvsE_RecoToTrue_py8jet.C("Al","R04")';
 root -l -b -q 'draw_JESJERvsE_RecoToTrue_py8jet.C("Alframe","R04")';
( ->pdf files will be saved in outvsERecoToTruePy8jet/***) 


########## 5) After 1stSortedLorentz is done at rcas, make 2ndSortedLorentz!

./run_rootAna_2ndSort_qcdjet_lorentz_ptbinning.sh

