#!/bin/csh

set initfile = 0
set endfile = 1000

#void final_qcdjet(string sampleType = "QCDJet",
#                              string ihcalType = "SS310",
#                              string jetE = "50GeV",
#                              int initfile=0,
#                              int endfile =1000,
#                              float total_sf=1.00,
#                              float cemc_sf=1.00,
#                              float ihcal_sf = 1.00,
#                              float ohcal_sf=1.00,
#                              bool doetopcut=true,
#                              bool dodphicut=true)

###########################################################################################################
###########################################################################################################
###########################################################################################################

##### (before method01 method02 method03 method04 method5))
foreach ihcal (SS310 Alframe) 
  foreach method (before method01 method02 method03 method04 method05)
  #foreach method (before method03 method04 method05)
    foreach energy (20GeV 30GeV 40GeV 50GeV)
      set thiscemc = `awk '{for (I=1;I<=NF;I++) if ($I == "cemcScale") {print $(I+2)};}' logs/log_$method\_$initfile\_$endfile\_$energy\_$ihcal`; #echo $thiscemc
      set thisihcal = `awk '{for (I=1;I<=NF;I++) if ($I == "ihcalScale") {print $(I+2)};}' logs/log_$method\_$initfile\_$endfile\_$energy\_$ihcal`; #echo $thisihcal
      set thisohcal = `awk '{for (I=1;I<=NF;I++) if ($I == "ohcalScale") {print $(I+2)};}' logs/log_$method\_$initfile\_$endfile\_$energy\_$ihcal`; #echo $thisohcal
      root -l -b -q final_qcdjet.C\(\"QCDJet\",\"$ihcal\",\"$energy\",$initfile,$endfile,1.00,$thiscemc,$thisihcal,$thisohcal,true,true\) >&logs/log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal&
    end  
  end  
end  

echo "***** All Done"  

########## before
#root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"20GeV\",$initfile,$endfile,1.00,1.00,1.00,1.00,true,true\)
#foreach energy (20GeV 30GeV 40GeV 50GeV)
#  root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"$energy\",$initfile,$endfile,1.00,1.00,1.00,1.00,true,true\)
#end

