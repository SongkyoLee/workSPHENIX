#!/bin/csh

set initfile = 0
#set endfile = 12000
set endfile = 24000

#void final_qcdjet_py8jet(string sampleType = "py8Jet",
#                              string ihcalType = "SS310",
#                              string jetE = "50GeV",
#                              string note = "R04",
#                              int initfile=0,
#                              int endfile =400,
#                              float total_sf=1.00,
#                              float cemc_sf=1.00,
#                              float ihcal_sf = 1.00,
#                              float ohcal_sf=1.00,
#                              bool doetopcut=true,
#                              bool dodphicut=true)

###########################################################################################################
###########################################################################################################
###########################################################################################################

foreach ihcal (Al Alframe) 
#foreach ihcal (Al) 
  #foreach note (R02 R04)
  foreach note (R04)
    foreach method (before truthjet)
      foreach energy (20GeV 30GeV 40GeV 50GeV)
        set thiscemc = `awk '{for (I=1;I<=NF;I++) if ($I == "cemcScale") {print $(I+2)};}' logs_py8jet/log_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note`; #echo $thiscemc
        set thisihcal = `awk '{for (I=1;I<=NF;I++) if ($I == "ihcalScale") {print $(I+2)};}' logs_py8jet/log_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note`; #echo $thisihcal
        set thisohcal = `awk '{for (I=1;I<=NF;I++) if ($I == "ohcalScale") {print $(I+2)};}' logs_py8jet/log_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note`; #echo $thisohcal
        root -l -b -q final_qcdjet_py8jet.C\(\"py8Jet\",\"$ihcal\",\"$energy\",\"$note\",$initfile,$endfile,1.00,$thiscemc,$thisihcal,$thisohcal,true,true\) >&logs_py8jet/log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note&
        sleep 1s
      end  
    end  
  end  
end  

echo "***** All Done"  

