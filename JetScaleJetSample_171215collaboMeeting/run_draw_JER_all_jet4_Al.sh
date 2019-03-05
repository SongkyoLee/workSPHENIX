#!/bin/csh

set fitminOrig=-1.0
set fitmaxOrig=0.2
########## iter1
#set fitmin20Orig=-1.0
#set fitmax20Orig=0.2
## 20: -3.62405e-01 2.01506e-01
#set fitmin50Orig=-1.0
#set fitmax50Orig=0.2
## 50: -3.19273e-01 1.40797e-01
########## iter2
#set fitmin20Orig=-0.77
#set fitmax20Orig=0.041
## 20: -3.36137e-01 2.19767e-01
#set fitmin50Orig=-0.60
#set fitmax50Orig=-0.038
## 50: -2.93852e-01 1.66283e-01
########## iter3
#set fitmin20Orig=-0.78
#set fitmax20Orig=0.10
## 20: -3.44713e-01 2.05705e-01
#set fitmin50Orig=-0.63
#set fitmax50Orig=0.039
## 50: -3.05489e-01 1.55267e-01
########## iter4
#set fitmin20Orig=-0.76
#set fitmax20Orig=0.07
## 20:  -3.38190e-01 2.13463e-01
#set fitmin50Orig=-0.62
#set fitmax50Orig=0.005
## 50:n -3.00390e-01 1.60300e-01
########## iter5
#set fitmin20Orig=-0.77
#set fitmax20Orig=0.09
## 20: -3.42032e-01 2.10700e-01
#set fitmin50Orig=-0.62
#set fitmax50Orig=0.02
## 50: -3.02530e-01 1.56483e-01
########## iter6
set fitmin20Orig=-0.77
set fitmax20Orig=0.08
## 20: -3.42032e-01 2.10700e-01 //done
set fitmin50Orig=-0.62
set fitmax50Orig=0.01
## 50: -3.02530e-01 1.56483e-01 //done

########## iter1
#set fitmin20=-0.4
#set fitmax20=0.6
## 20: 1.61823e-01 1.29788e-01
#set fitmin50=-0.4
#set fitmax50=0.6
## 50: 1.03341e-01 1.23752e-01
########## iter2
#set fitmin20=-0.098
#set fitmax20=0.42
## 20: 1.71277e-01 1.27804e-01
#set fitmin50=-0.14
#set fitmax50=0.35
## 50: 1.06901e-01 1.16192e-01
########## iter3
#set fitmin20=-0.084
#set fitmax20=0.43
## 20: 1.71993e-01 1.24574e-01
set fitmin50=-0.13
set fitmax50=0.34
## 50: 1.06901e-01 1.16192e-01 // done
########## iter4
#set fitmin20=-0.077
#set fitmax20=0.42
## 20: 1.72606e-01 1.25864e-01
########## iter5
set fitmin20=-0.079
set fitmax20=0.42
## 20: 1.72606e-01 1.25864e-01 //done
 
### 20GeV
root -l -b -q draw_JER_all_jet4.C\(\"Al\",\"20GeV\",$fitmin20Orig,$fitmax20Orig,1.00,1.00,1.00\)
###root -l -b -q draw_JER_all_jet4.C\(\"Al\",\"20GeV\",$fitmin20Orig,$fitmax20Orig,1.00,6.69,1.04\)
root -l -b -q draw_JER_all_jet4.C\(\"Al\",\"20GeV\",$fitmin20,$fitmax20,1.25,8.36,1.30\)
### 50GeV
root -l -b -q draw_JER_all_jet4.C\(\"Al\",\"50GeV\",$fitmin50Orig,$fitmax50Orig,1.00,1.00,1.00\)
root -l -b -q draw_JER_all_jet4.C\(\"Al\",\"50GeV\",$fitmin50,$fitmax50,1.00,9.12,1.27\)

echo "All Done ***"
