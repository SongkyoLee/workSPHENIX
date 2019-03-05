#!/bin/csh

#set initfile = 0
#set endfile = 100
#set endfile = 400
#set endfile = 4000
#set initfile = 4000
#set endfile = 8000
#set initfile = 8000
#set endfile = 12000
#set initfile = 15000
#set endfile = 16000
#set initfile = 16000
#set endfile = 18000
#set initfile = 18000
#set endfile = 20000
#set gammaendfile = 1000
#set jetendfile = 1000

#void rootAna_1stSort(string label = "GammaJet20GeV_Al",int initfile=0,int endfile = 1000, bool doetopcut=true, bool dodphicut=true)

foreach sample (py8Jet)
  foreach note (R04)
    #foreach ihcal (Alframe Al)
    #foreach ihcal (Al)
    foreach ihcal (Alframe)
      foreach energy (allGeV)
        #foreach initfile(12000 14000 16000 18000 20000 22000)
        foreach initfile(20000 22000)
          set endfile = `expr $initfile + 2000`
          echo "***** Run" $sample$energy$note\_$ihcal "from" $initfile "to" $endfile
          #root -l -b -q rootAna_1stSort.C\(\"$sample$energy\_$ihcal\",$initfile,$endfile,false,false\)
          root -l -b -q rootAna_1stSort.C\(\"$sample$energy$note\_$ihcal\",$initfile,$endfile,true,true\)
        end
      end
    end
  end
end

echo "***** All Done" 
 
