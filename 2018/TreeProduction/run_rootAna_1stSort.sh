#!/bin/csh

set initfile = 0
set endfile = 400
#set endfile = 1000
#set gammaendfile = 1000
#set jetendfile = 1000

#void rootAna_1stSort(string label = "GammaJet20GeV_Al",int initfile=0,int endfile = 1000, bool doetopcut=true, bool dodphicut=true)

#foreach sample (GammaJet QCDJet)
foreach sample (QCDJet)
  #foreach note (R02 R04)
  foreach note (R02)
    foreach ihcal (SS310 Alframe Al)
    foreach energy (20GeV 30GeV 40GeV 50GeV)
#      foreach energy (20GeV)
        echo "***** Run" $sample$energy$note\_$ihcal
#       root -l -b -q rootAna_1stSort.C\(\"$sample$energy\_$ihcal\",$initfile,$endfile,false,false\)
        root -l -b -q rootAna_1stSort.C\(\"$sample$energy$note\_$ihcal\",$initfile,$endfile,true,true\)
      end
    end
  end
end

echo "***** All Done" 
 
