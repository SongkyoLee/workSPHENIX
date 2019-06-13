#!/bin/csh

set initfile = 0
#set endfile = 300
set endfile = 1000

#void rootAna_1stSort(string label = "GammaJet20GeV_Al",int initfile=0,int endfile = 1000, bool doetopcut=true, bool dodphicut=true)

foreach sample (py8Jet)
#  foreach note (R02 R04)
  foreach note (R02)
    foreach ihcal (Al Alframe)
    #foreach ihcal (Al)
    #foreach ihcal (Alframe)
      foreach energy (50GeV 60GeV 70GeV)
          echo "***** Run" $sample$energy$note\_$ihcal "from" $initfile "to" $endfile
          #root -l -b -q rootAna_1stSort.C\(\"$sample$energy\_$ihcal\",$initfile,$endfile,false,false\)
          root -l -b -q rootAna_1stSort.C\(\"$sample$energy$note\_$ihcal\",$initfile,$endfile,true,true\)
      end
    end
  end
end

echo "***** All Done" 
 
