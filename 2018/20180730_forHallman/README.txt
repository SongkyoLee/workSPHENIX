
########## 1) Make 2nd-sorted rootfiles

./run_rootAna_2ndSort_gammajet.sh (not used now)
./run_rootAna_2ndSort_qcdjet.sh

########## 2) Get scales using minuit

./run_make_log_before.sh <- to make log files of "before scaling"
./run_minuit_recogamma.sh <- old method: not used now!!
./run_minuit_truthjet.sh <- new method

( --->log files will be saved in logs/***) 

########## 3) Apply scales to dijet samples

./run_final_qcdjet.sh
( ->log files will be saved in logs/***) 
( ->pdf and root files will be saved in outFinalXXX/***) 

########## 4) Read logs/*** and Draw scales vs E 

./getScale.sh
( ->txt files will be saved in txt_scale/***) 
root -l 'draw_SFvsE.C("SS310","R02")'
root -l 'draw_SFvsE.C("SS310","R04")'
root -l 'draw_SFvsE.C("Al","R02")'
root -l 'draw_SFvsE.C("Al","R04")'
root -l 'draw_SFvsE.C("Alframe","R02")'
root -l 'draw_SFvsE.C("Alframe","R04")'
( ->pdf files will be saved in outvsE/***) 

########## 5) Read logs/*** and Draw JES and JER vs E 

./getJESJER.sh
( ->txt files will be saved in txt_jet/***) 
root -l 'draw_JESJERvsE.C("SS310","R02")'
root -l 'draw_JESJERvsE.C("SS310","R04")'
root -l 'draw_JESJERvsE.C("Al","R02")'
root -l 'draw_JESJERvsE.C("Al","R04")'
root -l 'draw_JESJERvsE.C("Alframe","R02")'
root -l 'draw_JESJERvsE.C("Alframe","R04")'
( ->pdf files will be saved in outvsE/***) 

########## 6) Read outFinalXXX/***.root and Draw forHallman 

./run_forHallman.sh
( ->pdf files will be saved in outHallman/***) 

