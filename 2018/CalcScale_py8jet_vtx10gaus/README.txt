
#################### 1) Make 2nd-sorted rootfiles

(***for diffE)
./run_rootAna_2ndSort_gammajet.sh
./run_rootAna_2ndSort_qcdjet.sh

(***for multi)
./run_rootAna_2ndSort_gammajet_multi.sh
./run_rootAna_2ndSort_qcdjet_formulti.sh

#################### 2) Get scales from gammajet samples

->method01 only for "before". Actually used methods are "method03,04, and 05".

(***for diffE)
./run_method01_asymm_gammajet.sh
./run_method02_minuit_over_recojet.sh
./run_method03_minuit_over_recogamma.sh
./run_method04_hybrid.sh
./run_method05_asymm_manual.sh

(***for multi)
./run_method01_asymm_gammajet_multi.sh
./run_method02_minuit_over_recojet_multi.sh
./run_method03_minuit_over_recogamma_multi.sh
./run_method04_hybrid_multi.sh
./run_method05_asymm_manual_multi.sh

( ->log files will be saved in logs/log_method***) 

#################### 3) Apply scales to dijet samples

(***for diffE)
./run_final_qcdjet.sh

(***for multi)
./run_final_qcdjet_multi.sh

( ->log files will be saved in logs/log_final_qcd_***) 

#################### 4) Read logs and Draw scales_vs_E, and JES/JER_vs_E 

(***for diffE)
./getScale.sh
( ->txt files will be saved in txt_scale)
root -l 'draw_SFvsE.C("SS310")'
root -l 'draw_SFvsE.C("Alframe")'
./getJESJER.sh
( ->txt files will be saved in txt_jes)
root -l 'draw_JESJERvsE.C("final_qcd")'

(***for multi)
./run_stat_multi.sh
( ->txt files will be saved in txt_scale and txt_jes)
(Unlike diffE, run by shell script, and  ->log files will be saved in logs/log_stat_***) 

#################### 5) Compare results (diffE and multi) and draw final plot

root -l 'compare_SF.C("SS310")'
root -l 'compare_SF.C("Alframe")'
root -l compare_JESJER.C

#################### 6) Check conservative JES and JER (all SFs shifted 1sigma up, 1sigma down)

./run_final_qcdjet_conserv.sh
( ->log files will be saved in logs/log_final_qcd_conserv_***) 

./getJESJER_conserv.sh
( ->txt files will be saved in txt_conserv)




#################### 6) Read wTot and repeat 3)-5) (not yet!!!)

root -l get_wTot.C
*modify: run_final_qcdjet_wTot.sh 

