#!/bin/csh

set starting = 10000
set step = 1000
set iteration = 10

foreach sample (py8Jet)
  foreach note (R04)
    foreach ihcal (Al)
      foreach energy (allGeV)
        set i = 0
        while ( $i<$iteration )
          set initfile = `expr $starting + $i \* $step`
          set endfile = `expr $initfile + $step`
          echo "***** Run" $sample$energy$note\_$ihcal "from" $initfile "to" $endfile
          root -l -b -q rootAna_1stSort_py8jet_lorentz_AltoAlframe.C\(\"$sample$energy$note\_$ihcal\",$initfile,$endfile,1.00,1.00,1.00,1.00,true,true\)
#          root -l -b -q rootAna_1stSort_py8jet_lorentz_AltoAlframe.C\(\"$sample$energy$note\_$ihcal\",$initfile,$endfile,1.00,0.50,0.50,0.50,true,true\)
          @ i++
      end
    end
  end
end

echo "***** All Done" 
 
