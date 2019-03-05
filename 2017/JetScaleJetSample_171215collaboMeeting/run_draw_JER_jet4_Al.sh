#!/bin/csh

########## 20Orig
#set fitmin20Orig=-1.0
#set fitmax20Orig=0.2
## -3.62405e-01 2.01506e-01 
#set fitmin20Orig=-0.77
#set fitmax20Orig=0.041
## -3.36137e-01 2.19767e-01
#set fitmin20Orig=-0.78
#set fitmax20Orig=0.10
## -3.44713e-01 2.05705e-01
#set fitmin20Orig=-0.76
#set fitmax20Orig=0.067
## -3.38190e-01 2.13463e-01
#set fitmin20Orig=-0.77
#set fitmax20Orig=0.089
## -3.42032e-01 2.10700e-01
#set fitmin20Orig=-0.77
#set fitmax20Orig=0.080
## -3.42032e-01 2.10700e-01 //done

########## 20
#set fitmin20=-0.4
#set fitmax20=0.6
## -1.29196e-01 1.53754e-01 
#set fitmin20=-0.44
#set fitmax20=0.18
## -1.31007e-01 1.70454e-01
#set fitmin20=-0.47
#set fitmax20=0.21
## -1.38124e-01 1.69555e-01
#set fitmin20=-0.48
#set fitmax20=0.20
## -1.36662e-01 1.72768e-01 
#set fitmin20=-0.48
#set fitmax20=0.21
## -1.38124e-01 1.69555e-01
set fitmin20=-0.48
set fitmax20=0.20
## -1.36662e-01 1.72768e-01 //done



########## 50Orig
#set fitmin50Orig=-1.0
#set fitmax50Orig=0.2
## -3.19273e-01 1.40797e-01 
#set fitmin50Orig=-0.60
#set fitmax50Orig=0.038
## -3.02065e-01 1.51908e-01
#set fitmin50Orig=-0.61
#set fitmax50Orig=0.0018
## -3.00390e-01 1.60300e-01
#set fitmin50Orig=-0.62
#set fitmax50Orig=0.02
## -3.02530e-01 1.56483e-01
set fitmin50Orig=-0.62
set fitmax50Orig=0.010
## -3.02530e-01 1.56483e-01 //done

########## 50
#set fitmin50=-0.4
#set fitmax50=0.6
## 1.92450e-02 1.29201e-01
#set fitmin50=-0.24
#set fitmax50=0.28
## 2.52627e-02 1.22727e-01
set fitmin50=-0.22
set fitmax50=0.27
## 2.63895e-02 1.21132e-01 //done



### 20GeV
root -l -b -q draw_JER_jet4.C\(\"Al\",\"20GeV\",$fitmin20Orig,$fitmax20Orig,1.00,1.00,1.00\)
#root -l -b -q draw_JER_jet4.C\(\"Al\",\"20GeV\",$fitmin20,$fitmax20,1.00,5.40,0.84\)
### 50GeV
#root -l -b -q draw_JER_jet4.C\(\"Al\",\"50GeV\",$fitmin50Orig,$fitmax50Orig,1.00,1.00,1.00\)
#root -l -b -q draw_JER_jet4.C\(\"Al\",\"50GeV\",$fitmin50,$fitmax50,1.00,7.75,1.08\)

echo "All Done ***"
