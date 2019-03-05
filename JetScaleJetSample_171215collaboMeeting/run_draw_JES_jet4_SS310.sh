#!/bin/csh

########## 20Orig
set fitmin20Orig=0.3
set fitmax20Orig=1.3
##
set fitmin20Orig=0.52
set fitmax20Orig=1.01
##
set fitmin20Orig=0.52
set fitmax20Orig=1.00
## 7.62868e-01 1.19915e-01 // done

########## 20
set fitmin20=0.4
set fitmax20=1.6
##
set fitmin20=0.60
set fitmax20=1.14
##
set fitmin20=0.61
set fitmax20=1.13
##
set fitmin20=0.60
set fitmax20=1.13
## 8.68162e-01 1.31760e-01 //done

########## 50Orig
#set fitmin50Orig=0.3
#set fitmax50Orig=1.3
## 7.81357e-01 9.56909e-02
set fitmin50Orig=0.59
set fitmax50Orig=0.97
## 7.83131e-01 9.46780e-02 //done

########## 50
#set fitmin50=0.4
#set fitmax50=1.6
## 9.67739e-01 1.22616e-01
#set fitmin50=0.72
#set fitmax50=1.21
## 9.69066e-01  1.14230e-01
#set fitmin50=0.74
#set fitmax50=1.20
## 9.69310e-01 1.12817e-01
set fitmin50=0.74
set fitmax50=1.19
## 9.69310e-01 1.12817e-01 //done

### 20GeV
#root -l -b -q draw_JES_jet4.C\(\"SS310\",\"20GeV\",$fitmin20Orig,$fitmax20Orig,1.00,1.00,1.00\)
root -l -b -q draw_JES_jet4.C\(\"SS310\",\"20GeV\",$fitmin20,$fitmax20,1.00,2.15,0.92\)
### 50GeV
#root -l -b -q draw_JES_jet4.C\(\"SS310\",\"50GeV\",$fitmin50Orig,$fitmax50Orig,1.00,1.00,1.00\)
#root -l -b -q draw_JES_jet4.C\(\"SS310\",\"50GeV\",$fitmin50,$fitmax50,1.00,2.84,1.02\)

echo "All Done ***"
