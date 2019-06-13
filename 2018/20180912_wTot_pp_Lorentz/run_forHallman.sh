#!/bin/csh

set initfile = 0
set endfile = 400

#void draw_forHallman(bool isCalib=true,
#                              string sampleType = "QCDJet",
#                              string jetE = "20GeV",
#                              string note = "R04",
#                              bool doetopcut=true,
#                              bool dodphicut=true)

###########################################################################################################
###########################################################################################################
###########################################################################################################

foreach note (R02 R04)
  foreach energy (20GeV 30GeV 40GeV 50GeV)
    root -l -b -q draw_forHallman.C\(false,\"QCDJet\",\"$energy\",\"$note\",true,true\)
    root -l -b -q draw_forHallman.C\(true,\"QCDJet\",\"$energy\",\"$note\",true,true\)
  end  
end  

echo "***** All Done"  

