#!/bin/csh

set initfile = 0
set qcdjetendfile = 1000

foreach ihcal (SS310 Alframe) 
  foreach energy (20GeV)
    set thiscemc = `awk '{for (I=1;I<=NF;I++) if ($I == "cemc_mean-sig") {print $(I+2)};}' logs/log_stat_SF_$ihcal`; #echo $thiscemc
    set thisihcal = `awk '{for (I=1;I<=NF;I++) if ($I == "ihcal_mean-sig") {print $(I+2)};}' logs/log_stat_SF_$ihcal`; #echo $thisihcal
    set thisohcal = `awk '{for (I=1;I<=NF;I++) if ($I == "ohcal_mean-sig") {print $(I+2)};}' logs/log_stat_SF_$ihcal`; #echo $thisohcal
    root -l -b -q final_qcdjet.C\(\"QCDJet\",\"$ihcal\",\"$energy\",$initfile,$qcdjetendfile,1.00,$thiscemc,$thisihcal,$thisohcal,true,true\) >&logs/log_final_qcd_conserv_minimum_$energy\_$ihcal&
    sleep 3s
    set thiscemc = `awk '{for (I=1;I<=NF;I++) if ($I == "cemc_mean+sig") {print $(I+2)};}' logs/log_stat_SF_$ihcal`; #echo $thiscemc
    set thisihcal = `awk '{for (I=1;I<=NF;I++) if ($I == "ihcal_mean+sig") {print $(I+2)};}' logs/log_stat_SF_$ihcal`; #echo $thisihcal
    set thisohcal = `awk '{for (I=1;I<=NF;I++) if ($I == "ohcal_mean+sig") {print $(I+2)};}' logs/log_stat_SF_$ihcal`; #echo $thisohcal
    root -l -b -q final_qcdjet.C\(\"QCDJet\",\"$ihcal\",\"$energy\",$initfile,$qcdjetendfile,1.00,$thiscemc,$thisihcal,$thisohcal,true,true\) >&logs/log_final_qcd_conserv_maximum_$energy\_$ihcal&
    sleep 3s
  end  
end  

echo "***** All Done" 
 
