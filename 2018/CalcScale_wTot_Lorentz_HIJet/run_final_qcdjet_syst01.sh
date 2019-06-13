#!/bin/csh

set initfile = 0
set endfile = 200
#set logendfile = 400

#void final_qcdjet_Syst01(string sampleType = "HIJet",
#                              string ihcalType = "Al",
#                              string jetE = "20GeV",
#                              string note = "48fmR04",
#                              int initfile=0,
#                              int endfile =1000,
#                              float total_sf=1.00,
#                              float cemc_sf=1.00,
#                              float ihcal_sf = 1.00,
#                              float ohcal_sf=1.00,
#                              bool doetopcut=false,
#                              bool dodphicut=false)

###########################################################################################################
###########################################################################################################
###########################################################################################################

#foreach ihcal (SS310 Al Alframe) 
foreach ihcal (Al) 
  foreach note (04fmR02 04fmR04 48fmR02 48fmR04)
    #foreach method (before truthjet truthjet_2par)
    foreach method (before truthjet_hi)
      foreach energy (20GeV 30GeV 40GeV 50GeV)
        set thiscemc = `awk '{for (I=1;I<=NF;I++) if ($I == "cemcScale") {print $(I+2)};}' logs/log_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note`; #echo $thiscemc
        set thisihcal = `awk '{for (I=1;I<=NF;I++) if ($I == "ihcalScale") {print $(I+2)};}' logs/log_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note`; #echo $thisihcal
        set thisohcal = `awk '{for (I=1;I<=NF;I++) if ($I == "ohcalScale") {print $(I+2)};}' logs/log_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note`; #echo $thisohcal
        root -l -b -q final_qcdjet_Syst01.C\(\"HIJet\",\"$ihcal\",\"$energy\",\"$note\",$initfile,$endfile,1.00,$thiscemc,$thisihcal,$thisohcal,false,false\) >&logs_syst01/log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note&
        sleep 1s
      end  
    end  
  end  
end  

echo "***** All Done"  

