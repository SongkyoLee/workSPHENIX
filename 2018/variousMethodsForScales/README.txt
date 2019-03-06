20810402

########## 1) Make 2nd-sorted rootfiles

./run_rootAna_2ndSort_gammajet.sh
./run_rootAna_2ndSort_qcdjet.sh

########## 2) Get scales from gammajet samples

./run_method01_asymm_gammajet.sh
./run_method02_minuit_over_recojet.sh
./run_method03_minuit_over_recogamma.sh
./run_method04_hybrid.sh
./run_method05_asymm_manual.sh

########## 3) Apply scales to dijet samples

* modify: run_final_qcdjet.sh 
./run_final_qcdjet.sh >&log_final_qcd&

########## 4) Read logs and Draw scales vs E 

./getScale.sh final_qcd
root -l 'draw_SFvsE_SS310.C("final_qcd")'
root -l 'draw_SFvsE_Alframe.C("final_qcd")'

########## 5) Read logs and Draw JES and JER vs E 

./getJESJER.sh final_qcd
root -l 'draw_JESJERvsE.C("final_qcd")'

########## 5) Read wTot and repeat 3)-5)

root -l get_wTot.C
*modify: run_final_qcdjet_wTot.sh 

./run_final_qcdjet_wTot.sh >&log_final_qcd_wTot&
./getScale.sh final_qcd_wTot
root -l 'draw_SFvsE_SS310.C("final_qcd_wTot")'
root -l 'draw_SFvsE_Alframe.C("final_qcd_wTot")'
./getJESJER.sh final_qcd_wTot
root -l 'draw_JESJERvsE.C("final_qcd_wTot")'

