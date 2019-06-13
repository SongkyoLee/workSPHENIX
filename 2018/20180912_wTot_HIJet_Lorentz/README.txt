########## 1) Make 2nd-sorted rootfiles

###./run_rootAna_2ndSort_qcdjet_HIJet.sh
./run_rootAna_2ndSort_qcdjet_HIJet_ptbinning.sh (Gen Slices should be in pT)

########## 2) Get scales using minuit

./run_make_log_before.sh <- to make log files of "before scaling"
./run_minuit_truthjet_hi.sh <- (ihcalScale and ohcalScale)

########## 3) Apply scales to dijet samples

./run_final_qcdjet.sh
( ->log files will be saved in logs/***)
( ->pdf and root files will be saved in outFinalXXX/***)

########## 4-1) Read logs/*** and Draw scales vs E

./getScale.sh
( ->txt files will be saved in txt_scale/***)
not important... (since it is the same as pp)

########## 4-2) Read logs/*** and Draw JES and JER vs E

./getJESJER.sh
( ->txt files will be saved in txt_jet/***)
root -l -b -q 'draw_JESJERvsE.C("Al","04fmR02")';
root -l -b -q 'draw_JESJERvsE.C("Al","04fmR04")';
root -l -b -q 'draw_JESJERvsE.C("Al","48fmR02")';
root -l -b -q 'draw_JESJERvsE.C("Al","48fmR04")';
( ->pdf files will be saved in outvsE/***)

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
not important... (since it is the same as pp)

########## 7-2) Read logs_wTot/*** and Draw JES and JER vs E

./getJESJER_wTot.sh
( ->txt files will be saved in txt_jet_wTot/***)
root -l -b -q 'draw_JESJERvsE_wTot.C("Al","04fmR02")';
root -l -b -q 'draw_JESJERvsE_wTot.C("Al","04fmR04")';
root -l -b -q 'draw_JESJERvsE_wTot.C("Al","48fmR02")';
root -l -b -q 'draw_JESJERvsE_wTot.C("Al","48fmR04")';
( ->pdf files will be saved in outvsE/***)

########## 8) Compare wTot vs. woTot

root -l -b -q 'comp_JESJERvsE_wTot_woTot.C("Al","04fmR02")';
root -l -b -q 'comp_JESJERvsE_wTot_woTot.C("Al","04fmR04")';
root -l -b -q 'comp_JESJERvsE_wTot_woTot.C("Al","48fmR02")';
root -l -b -q 'comp_JESJERvsE_wTot_woTot.C("Al","48fmR04")';

########## 9) Compare pp vs. HI

root -l -b -q 'comp_JER_pp_HI.C("Al","04fm","R02")';
root -l -b -q 'comp_JER_pp_HI.C("Al","04fm","R04")';
root -l -b -q 'comp_JER_pp_HI.C("Al","48fm","R02")';
root -l -b -q 'comp_JER_pp_HI.C("Al","48fm","R04")';

