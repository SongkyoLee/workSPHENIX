#!/bin/csh

#set initfile = 0
#set endfile = 100

foreach sample (py8Jet)
  foreach note (R04)
    foreach ihcal (Al Alframe)
    #foreach ihcal (Al)
    #foreach ihcal (Alframe)
      foreach energy (allGeV)
        foreach initfile (0 1000 2000 3000)
          set endfile = `expr $initfile + 1000`
          echo "***** Run" $sample$energy$note\_$ihcal "from" $initfile "to" $endfile
          ##root -l -b -q rootAna_1stSort_py8jet_lorentz.C\(\"$sample$energy$note\_$ihcal\",$initfile,$endfile,1.00,1.00,1.00,1.00,true,true\)
          #root -l -b -q rootAna_1stSort_py8jet_lorentz.C\(\"$sample$energy$note\_$ihcal\",$initfile,$endfile,1.00,0.50,0.50,0.50,true,true\)
      end
    end
  end
end

echo "***** All Done" 
 
