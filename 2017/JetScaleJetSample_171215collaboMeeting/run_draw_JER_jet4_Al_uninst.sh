#!/bin/csh

########## 20Orig
set fitmin20Orig=-1.0
set fitmax20Orig=0.2
## 
set fitmin20Orig=-0.82
set fitmax20Orig=0.034
## 
set fitmin20Orig=-0.83
set fitmax20Orig=0.10
## 
set fitmin20Orig=-0.81
set fitmax20Orig=0.062
## 
set fitmin20Orig=-0.82
set fitmax20Orig=0.087
## 
set fitmin20Orig=-0.82
set fitmax20Orig=0.072
## -3.71590e-01 2.21714e-01 //done

########## 20
set fitmin20=-0.4
set fitmax20=0.6
## 
set fitmin20=-0.49
set fitmax20=0.12
## 
set fitmin20=-0.57
set fitmax20=0.17
## 
set fitmin20=-0.57
set fitmax20=0.15
## 
set fitmin20=-0.59
set fitmax20=0.17
## 
set fitmin20=-0.59
set fitmax20=0.16
## 
set fitmin20=-0.60
set fitmax20=0.16
## -2.15764e-01 1.89848e-01 //done


########## 50Orig
#set fitmin50Orig=-1.0
#set fitmax50Orig=0.2
## -3.91200e-01 1.69138e-01
#set fitmin50Orig=-0.73
#set fitmax50Orig=-0.053
## -3.58079e-01 2.01905e-01
#set fitmin50Orig=-0.76
#set fitmax50Orig=0.045
## -3.74978e-01 1.78251e-01
#set fitmin50Orig=-0.73
#set fitmax50Orig=-0.018
## -3.65133e-01 1.92045e-01
#set fitmin50Orig=-0.75
#set fitmax50Orig=0.019
## -3.72569e-01 1.83449e-01
#set fitmin50Orig=-0.74
#set fitmax50Orig=-0.0057
## -3.68252e-01 1.86735e-01
set fitmin50Orig=-0.74
set fitmax50Orig=0.0052
## -3.68252e-01 1.86735e-01 //done


########## 50
#set fitmin50=-0.4
#set fitmax50=0.6
## -1.18923e-01 1.24913e-01
#set fitmin50=-0.37
#set fitmax50=0.13
## -1.12640e-01 1.36266e-01
#set fitmin50=-0.39
#set fitmax50=0.16
## -1.16418e-01 1.34073e-01
set fitmin50=-0.38
set fitmax50=0.15
## -1.13867e-01 1.34251e-01 //done



### 20GeV
#root -l -b -q draw_JER_jet4_Al_uninst.C\(\"Al_uninst\",\"20GeV\",$fitmin20Orig,$fitmax20Orig,3,0,1\)
root -l -b -q draw_JER_jet4_Al_uninst.C\(\"Al_uninst\",\"20GeV\",$fitmin20,$fitmax20,3,0,0\)
### 50GeV
#root -l -b -q draw_JER_jet4_Al_uninst.C\(\"Al_uninst\",\"50GeV\",$fitmin50Orig,$fitmax50Orig,3,0,1\)
#root -l -b -q draw_JER_jet4_Al_uninst.C\(\"Al_uninst\",\"50GeV\",$fitmin50,$fitmax50,3,0,0\)

echo "All Done ***"
