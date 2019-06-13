
########## 1) Make 2nd-sorted rootfiles

./run_rootAna_2ndSort_gammajet.sh
./run_rootAna_2ndSort_qcdjet.sh

########## 2) Get scales from gammajet samples

->method01 only for "before". Actually used methods are "method03,04, and 05".

./run_method01_asymm_gammajet.sh
./run_method02_minuit_over_recojet.sh
./run_method03_minuit_over_recogamma.sh
./run_method04_hybrid.sh
./run_method05_asymm_manual.sh
( ->log files will be saved in logs/***) 

########## 3) Apply scales to dijet samples

./run_final_qcdjet.sh
( ->log files will be saved in logs/***) 

########## 4) Read logs and Draw scales vs E 

./getScale.sh
root -l 'draw_SFvsE.C("SS310")'
root -l 'draw_SFvsE.C("Alframe")'

########## 5) Read logs and Draw JES and JER vs E 

./getJESJER.sh
root -l 'draw_JESJERvsE.C("final_qcd")'

########## 5) Read wTot and repeat 3)-5) (not yet!!!)

root -l get_wTot.C
*modify: run_final_qcdjet_wTot.sh 

