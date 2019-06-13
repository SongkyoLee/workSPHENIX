#!/bin/csh

set initfile = 0
set endfile = 200

#set logendfile = 1000
set logendfile = 400

#void final_qcdjet(string sampleType = "HIJet",
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
  foreach note1st (04fm 48fm)
    foreach note2nd (R02 R04)
      #foreach method (before truthjet truthjet_2par)
      foreach method (before truthjet)
      foreach energy (20GeV 30GeV 40GeV 50GeV)
        set thiscemc = `awk '{for (I=1;I<=NF;I++) if ($I == "cemcScale") {print $(I+2)};}' logs_wTot/log_$method\_$initfile\_$logendfile\_$energy\_$ihcal\_$note2nd`; #echo $thiscemc
        set thisihcal = `awk '{for (I=1;I<=NF;I++) if ($I == "ihcalScale") {print $(I+2)};}' logs_wTot/log_$method\_$initfile\_$logendfile\_$energy\_$ihcal\_$note2nd`; #echo $thisihcal
        set thisohcal = `awk '{for (I=1;I<=NF;I++) if ($I == "ohcalScale") {print $(I+2)};}' logs_wTot/log_$method\_$initfile\_$logendfile\_$energy\_$ihcal\_$note2nd`; #echo $thisohcal
        set thistotal = `awk '{for (I=1;I<=NF;I++) if ($I == "totalScale") {print $(I+2)};}' logs_wTot/log_$method\_$initfile\_$logendfile\_$energy\_$ihcal\_$note2nd`; #echo $thistotal
        root -l -b -q final_qcdjet.C\(\"HIJet\",\"$ihcal\",\"$energy\",\"$note1st$note2nd\",$initfile,$endfile,$thistotal,$thiscemc,$thisihcal,$thisohcal,false,false\) >&logs_wTot/log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note1st$note2nd&
        sleep 1s
      end  
    end  
  end  
end  

echo "***** All Done"  

