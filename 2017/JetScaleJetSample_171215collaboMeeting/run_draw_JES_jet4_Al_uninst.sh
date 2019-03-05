#!/bin/csh

########## 20Orig
set fitmin20Orig=0.3
set fitmax20Orig=1.3
## 
set fitmin20Orig=0.45
set fitmax20Orig=0.99
## 7.22914e-01 1.34054e-01 //done 

########## 20
set fitmin20=0.4
set fitmax20=1.6
## 
set fitmin20=0.52
set fitmax20=1.10
## 8.14807e-01 1.43766e-01 // done

########## 50Orig
set fitmin50Orig=0.3
set fitmax50Orig=1.3
##
set fitmin50Orig=0.51
set fitmax50Orig=0.94
## 7.27940e-01 1.07704e-01 //done

########## 50
set fitmin50=0.4
set fitmax50=1.6
##
set fitmin50=0.65
set fitmax50=1.13
##
set fitmin50=0.66
set fitmax50=1.12
##
set fitmin50=0.67
set fitmax50=1.12
## 8.91837e-01 1.13196e-01 //done


### 20GeV
#root -l -b -q draw_JES_jet4_Al_uninst.C\(\"Al_uninst\",\"20GeV\",$fitmin20Orig,$fitmax20Orig,3,0,1\)
#root -l -b -q draw_JES_jet4_Al_uninst.C\(\"Al_uninst\",\"20GeV\",$fitmin20,$fitmax20,3,0,0\)
### 50GeV
#root -l -b -q draw_JES_jet4_Al_uninst.C\(\"Al_uninst\",\"50GeV\",$fitmin50Orig,$fitmax50Orig,3,0,1\)
root -l -b -q draw_JES_jet4_Al_uninst.C\(\"Al_uninst\",\"50GeV\",$fitmin50,$fitmax50,3,0,0\)

echo "All Done ***"
