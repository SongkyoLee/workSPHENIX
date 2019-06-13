#!/bin/csh

set initfile = 0
set endfile = 9000
set qcdjetendfile = 1000

set ii = $initfile
set jj = $initfile
set step = 300

while ($ii < $endfile)
  @ jj = $ii + $step
  echo "*** Run init = " $ii
  echo "end = " $jj

  foreach ihcal (SS310 Alframe) 
    foreach method (before method01 method02 method03 method04 method05)
    #foreach method (before method03 method04 method05)
      #foreach energy (20GeV 30GeV 40GeV 50GeV)
      foreach energy (20GeV)
        set thiscemc = `awk '{for (I=1;I<=NF;I++) if ($I == "cemcScale") {print $(I+2)};}' logs/log_$method\_$ii\_$jj\_$energy\_$ihcal`; #echo $thiscemc
        set thisihcal = `awk '{for (I=1;I<=NF;I++) if ($I == "ihcalScale") {print $(I+2)};}' logs/log_$method\_$ii\_$jj\_$energy\_$ihcal`; #echo $thisihcal
        set thisohcal = `awk '{for (I=1;I<=NF;I++) if ($I == "ohcalScale") {print $(I+2)};}' logs/log_$method\_$ii\_$jj\_$energy\_$ihcal`; #echo $thisohcal
        #root -l -b -q final_qcdjet.C\(\"QCDJet\",\"$ihcal\",\"$energy\",$ii,$jj,1.00,$thiscemc,$thisihcal,$thisohcal,true,true\) >&logs/log_final_qcd_$method\_$ii\_$jj\_$energy\_$ihcal&
        root -l -b -q final_qcdjet.C\(\"QCDJet\",\"$ihcal\",\"$energy\",$initfile,$qcdjetendfile,1.00,$thiscemc,$thisihcal,$thisohcal,true,true\) >&logs/log_final_qcd_$method\_$ii\_$jj\_$energy\_$ihcal&
        sleep 3s
      end  
    end  
  end  

  @ ii = $ii + $step
end

echo "***** All Done" 
 
########## before
#root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"20GeV\",$initfile,$endfile,1.00,1.00,1.00,1.00,true,true\)
#foreach energy (20GeV 30GeV 40GeV 50GeV)
#  root -l -b -q final_qcdjet.C\(\"QCDJet\",\"SS310\",\"$energy\",$initfile,$endfile,1.00,1.00,1.00,1.00,true,true\)
#end

