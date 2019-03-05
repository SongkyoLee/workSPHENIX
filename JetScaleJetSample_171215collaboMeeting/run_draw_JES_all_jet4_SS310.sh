#!/bin/csh

set fitminOrig=0.3
set fitmaxOrig=1.3
########## iter1
#set fitmin20=0.4
#set fitmax20=2.0
#set fitmin50=0.4
#set fitmax50=2.0
########## iter2 (+-2sigma)
#mean 1.21148e+00, sigma 1.89041e-01
#set fitmin20=0.83
#set fitmax20=1.59
#mean 1.09442e+00, sigma 1.43021e-01
#set fitmin50=0.80
#set fitmax50=1.38
########## iter3 (+-2sigma)
#mean 1.20763e+00 1.77826e-01
#set fitmin20=0.85
#set fitmax20=1.56
#mean 1.09013e+00 sigma 1.31255e-01
#set fitmin50=0.83
#set fitmax50=1.35
########## iter4 (+-2sigma)
#mean 1.20599e+00 sigma 1.75274e-01
set fitmin20=0.86
set fitmax20=1.56
#mean 1.08896e+00 sigma 1.29530e-01
set fitmin50=0.83
set fitmax50=1.35


### 20GeV
root -l -b -q draw_JES_all_jet4.C\(\"SS310\",\"20GeV\",$fitminOrig,$fitmaxOrig,1.00,1.00,1.00\)
###root -l -b -q draw_JES_all_jet4.C\(\"SS310\",\"20GeV\",$fitminOrig,$fitmaxOrig,1.00,2.74,1.17\)
root -l -b -q draw_JES_all_jet4.C\(\"SS310\",\"20GeV\",$fitmin20,$fitmax20,1.25,3.42,1.46\)
### 50GeV
root -l -b -q draw_JES_all_jet4.C\(\"SS310\",\"50GeV\",$fitminOrig,$fitmaxOrig,1.00,1.00,1.00\)
root -l -b -q draw_JES_all_jet4.C\(\"SS310\",\"50GeV\",$fitmin50,$fitmax50,1.00,3.56,1.28\)

### 20GeV
root -l -b -q draw_JES_all_jet4.C\(\"Al\",\"20GeV\",$fitminOrig,$fitmaxOrig,1.00,1.00,1.00\)
###root -l -b -q draw_JES_all_jet4.C\(\"Al\",\"20GeV\",$fitminOrig,$fitmaxOrig,1.00,6.69,1.04\)
root -l -b -q draw_JES_all_jet4.C\(\"Al\",\"20GeV\",$fitmin20,$fitmax20,1.25,8.36,1.30\)
### 50GeV
root -l -b -q draw_JES_all_jet4.C\(\"Al\",\"50GeV\",$fitminOrig,$fitmaxOrig,1.00,1.00,1.00\)
root -l -b -q draw_JES_all_jet4.C\(\"Al\",\"50GeV\",$fitmin50,$fitmax50,1.00,9.12,1.27\)

echo "All Done ***"
