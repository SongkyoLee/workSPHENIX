#!/bin/csh

set initfile = 0
set endfile = 400
#set endfile = 1000
#set gammaendfile = 1000
#set jetendfile = 1000

#void rootAna_1stSort(string label = "GammaJet20GeV_Al",int initfile=0,int endfile = 1000, bool doetopcut=true, bool dodphicut=true)

#foreach sample (GammaJet QCDJet)
foreach sample (QCDJet)
  foreach note (R02 R04)
    foreach ihcal (SS310 Alframe Al)
      #foreach energy (20GeV 30GeV 40GeV 50GeV)
      foreach energy (30GeV 40GeV 50GeV)
      #foreach energy (20GeV)
        foreach method (before truthjet)
          echo "***** Run" $sample$energy$note\_$ihcal "("$method")"
          set thiscemc = `awk '{for (I=1;I<=NF;I++) if ($I == "cemcScale") {print $(I+2)};}' logs/log_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note`; #echo $thiscemc
          set thisihcal = `awk '{for (I=1;I<=NF;I++) if ($I == "ihcalScale") {print $(I+2)};}' logs/log_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note`; #echo $thisihcal
          set thisohcal = `awk '{for (I=1;I<=NF;I++) if ($I == "ohcalScale") {print $(I+2)};}' logs/log_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note`; #echo $thisohcal
          #root -l -b -q rootAna_1stSort_lorentz.C\(\"$sample$energy$note\_$ihcal\",$initfile,$endfile,1.00,$thiscemc,$thisihcal,$thisohcal,false,false\)
          root -l -b -q rootAna_1stSort_lorentz.C\(\"$sample$energy$note\_$ihcal\",$initfile,$endfile,1.00,$thiscemc,$thisihcal,$thisohcal,true,true\)
        end
      end
    end
  end
end

echo "***** All Done" 
 
