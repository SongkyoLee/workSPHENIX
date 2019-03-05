#!/bin/csh

set fitminOrig=-1.0
set fitmaxOrig=0.2
########## iter1
#set fitmin20Orig=-1.0
#set fitmax20Orig=0.2
## 20: mean -3.23654e-01 sigma 1.91788e-01
#set fitmin50Orig=-1.0
#set fitmax50Orig=0.2
## 50: mean -2.88359e-01 sigma 1.32617e-01
########## iter2
#set fitmin20Orig=-0.71
#set fitmax20Orig=0.06
## 20: mean -3.01643e-01 sigma 1.98588e-01
#set fitmin50Orig=-0.56
#set fitmax50Orig=-0.023
## 50: mean -2.68405e-01 sigma 1.49670e-01
########## iter3
#set fitmin20Orig=-0.70
#set fitmax20Orig=0.09
## 20: mean -3.04052e-01 sigma 1.89909e-01
#set fitmin50Orig=-0.57
#set fitmax50Orig=-0.031
## 50: mean -2.65548e-01 sigma 1.53354e-01
########## iter4
#set fitmin20Orig=-0.68
#set fitmax20Orig=0.076
## 20: mean -3.02625e-01 sigma 1.88295e-01
set fitmin50Orig=-0.57
set fitmax50Orig=-0.04
## 50: mean -2.65548e-01 sigma 1.53354e-01 //done
########## iter5
set fitmin20Orig=-0.68
set fitmax20Orig=0.074
## 20: mean -3.02625e-01 sigma 1.88295e-01 //done

########## iter1
#set fitmin20=-0.4
#set fitmax20=0.6
## 20: mean 1.69451e-01 sigma 1.20918e-01
#set fitmin50=-0.4
#set fitmax50=0.6
## 50: mean 8.37416e-02 sigma 1.16100e-01
########## iter2
#set fitmin20=-0.07
#set fitmax20=0.41
## 20: mean 1.77314e-01 sigma 1.20854e-01
#set fitmin50=-0.15
#set fitmax50=0.32
## 50: mean 8.70929e-02 sigma 1.09768e-01
########## iter3
#set fitmin20=-0.065
#set fitmax20=0.42
## 20: mean 1.77687e-01 sigma 1.18057e-01
set fitmin50=-0.13
set fitmax50=0.31
## 50: mean 8.69673e-02 sigma 1.09568e-01 //done
########## iter4
set fitmin20=-0.058
set fitmax20=0.41
## 20: mean 1.77687e-01 sigma 1.18057e-01 //done


### 20GeV
root -l -b -q draw_JER_all_jet4.C\(\"SS310\",\"20GeV\",$fitmin20Orig,$fitmax20Orig,1.00,1.00,1.00\)
root -l -b -q draw_JER_all_jet4.C\(\"SS310\",\"20GeV\",$fitmin20Orig,$fitmax20Orig,1.00,2.74,1.17\)
root -l -b -q draw_JER_all_jet4.C\(\"SS310\",\"20GeV\",$fitmin20,$fitmax20,1.25,3.42,1.46\)
### 50GeV
root -l -b -q draw_JER_all_jet4.C\(\"SS310\",\"50GeV\",$fitmin50Orig,$fitmax50Orig,1.00,1.00,1.00\)
root -l -b -q draw_JER_all_jet4.C\(\"SS310\",\"50GeV\",$fitmin50,$fitmax50,1.00,3.56,1.28\)

echo "All Done ***"
