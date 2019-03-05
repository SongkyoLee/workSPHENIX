#!/bin/csh

########## 20Orig
set fitmin20Orig=0.3
set fitmax20Orig=1.3
##
set fitmin20Orig=0.49
set fitmax20Orig=1.00
##
set fitmin20Orig=0.49
set fitmax20Orig=0.99
## 7.42785e-01 1.25096e-01 //done

########## 20
set fitmin20=0.4
set fitmax20=1.6
## 
set fitmin20=0.57
set fitmax20=1.17
## 
set fitmin20=0.58
set fitmax20=1.16
## 8.70827e-01 1.45023e-01  //done

########## 50Orig
#set fitmin50Orig=0.3
#set fitmax50Orig=1.3
## 7.63489e-01 1.00164e-01
#set fitmin50Orig=0.56
#set fitmax50Orig=0.96
## 7.66041e-01 9.91696e-02
set fitmin50Orig=0.57
set fitmax50Orig=0.96
## 7.66041e-01 9.91696e-02 //done

########## 50
#set fitmin50=0.4
#set fitmax50=1.6
## 1.02019e+00 1.44433e-01
#set fitmin50=0.73
#set fitmax50=1.31
## 1.01956e+00 1.33892e-01
#set fitmin50=0.75
#set fitmax50=1.29
## 1.01950e+00 1.32139e-01
set fitmin50=0.76
set fitmax50=1.28
## 1.01932e+00 1.30252e-01 //done

### 20GeV
#root -l -b -q draw_JES_jet4.C\(\"Al\",\"20GeV\",$fitmin20Orig,$fitmax20Orig,1.00,1.00,1.00\)
root -l -b -q draw_JES_jet4.C\(\"Al\",\"20GeV\",$fitmin20,$fitmax20,1.00,5.40,0.84\)
### 50GeV
#root -l -b -q draw_JES_jet4.C\(\"Al\",\"50GeV\",$fitmin50Orig,$fitmax50Orig,1.00,1.00,1.00\)
#root -l -b -q draw_JES_jet4.C\(\"Al\",\"50GeV\",$fitmin50,$fitmax50,1.00,7.75,1.08\)

echo "All Done ***"
