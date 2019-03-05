#!/bin/csh

########## 20Orig
#set fitmin20Orig=-1.0
#set fitmax20Orig=0.2
## -3.23992e-01 1.84479e-01
#set fitmin20Orig=-0.69
#set fitmax20Orig=0.045
## -3.00048e-01 1.98080e-01
#set fitmin20Orig=-0.70
#set fitmax20Orig=0.096
## -3.06605e-01 1.86587e-01
#set fitmin20Orig=-0.68
#set fitmax20Orig=0.067
## -3.01231e-01 1.92198e-01
#set fitmin20Orig=-0.69
#set fitmax20Orig=0.083
## -3.05139e-01 1.89608e-01
#set fitmin20Orig=-0.68
#set fitmax20Orig=0.074
## -3.03552e-01 1.87771e-01
set fitmin20Orig=-0.68
set fitmax20Orig=0.072
## -3.03552e-01 1.87771e-01 //done

########## 20
#set fitmin20=-0.4
#set fitmax20=0.6
## -1.37476e-01 1.42052e-01
#set fitmin20=-0.42
#set fitmax20=0.15
## -1.36496e-01 1.58236e-01
#set fitmin20=-0.45
#set fitmax20=0.18
## -1.42890e-01 1.57522e-01 
set fitmin20=-0.46
set fitmax20=0.17
## -1.42890e-01 1.57522e-01  //done

########## 50Orig
#set fitmin50Orig=-1.0
#set fitmax50Orig=0.2
## -2.88516e-01 1.33642e-01
#set fitmin50Orig=-0.56
#set fitmax50Orig=-0.021
## -2.68944e-01 1.50545e-01
#set fitmin50Orig=-0.57
#set fitmax50Orig=-0.032
## -2.66793e-01 1.53292e-01
#set fitmin50Orig=-0.57
#set fitmax50Orig=-0.040
## -2.66793e-01 1.53292e-01
#set fitmin50Orig=-0.57
#set fitmax50Orig=0.039
## -2.74537e-01 1.41136e-01 
#set fitmin50Orig=-0.56
#set fitmax50Orig=0.0077
## -2.71334e-01  1.47051e-01 
#set fitmin50Orig=-0.57
#set fitmax50Orig=0.023
## -2.72949e-01 1.44306e-01
set fitmin50Orig=-0.56
set fitmax50Orig=0.016
## -2.72949e-01 1.44306e-01 //done


########## 50
#set fitmin50=-0.4
#set fitmax50=0.6
## -3.32392e-02 1.18832e-01
#set fitmin50=-0.27
#set fitmax50=0.21
## -2.71519e-02 1.15073e-01
#set fitmin50=-0.26
#set fitmax50=0.20
## -2.53657e-02 1.13832e-01
set fitmin50=-0.25
set fitmax50=0.20
## -2.53657e-02 1.13832e-01 // done

 

### 20GeV
#root -l -b -q draw_JER_jet4.C\(\"SS310\",\"20GeV\",$fitmin20Orig,$fitmax20Orig,1.00,1.00,1.00\)
###root -l -b -q draw_JER_jet4.C\(\"SS310\",\"20GeV\",$fitmin20,$fitmax20,1.00,2.42,1.46\)
#root -l -b -q draw_JER_jet4.C\(\"SS310\",\"20GeV\",$fitmin20,$fitmax20,1.00,2.15,0.92\)
### 50GeV
root -l -b -q draw_JER_jet4.C\(\"SS310\",\"50GeV\",$fitmin50Orig,$fitmax50Orig,1.00,1.00,1.00\)
#root -l -b -q draw_JER_jet4.C\(\"SS310\",\"50GeV\",$fitmin50,$fitmax50,1.00,2.84,1.02\)

echo "All Done ***"
