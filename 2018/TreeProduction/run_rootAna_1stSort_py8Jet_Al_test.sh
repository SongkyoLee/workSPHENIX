#!/bin/csh

set starting = 0
#set step = 1000
set step = 100
#set iteration = 20
set iteration = 1

foreach sample (py8Jet)
  foreach note (R04)
    foreach ihcal (Al)
      foreach energy (allGeV)
        set i = 0
        while ( $i<$iteration )
        #foreach initfile(0 1000 2000 3000 4000 5000 6000 7000 8000)
          set initfile = `expr $starting + $i \* $step`
          set endfile = `expr $initfile + $step`
          echo "***** Run" $sample$energy$note\_$ihcal "from" $initfile "to" $endfile
          #root -l -b -q rootAna_1stSort.C\(\"$sample$energy\_$ihcal\",$initfile,$endfile,false,false\)
          root -l -b -q rootAna_1stSort.C\(\"$sample$energy$note\_$ihcal\",$initfile,$endfile,true,true\)
          @ i++
        end
      end
    end
  end
end

echo "***** All Done" 
 
