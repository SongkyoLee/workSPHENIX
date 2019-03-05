#!/bin/csh

set initfile = 0
set endfile = 200
#set endfile = 1000

#set gammaendfile = 1000
#set jetendfile = 1000

#void rootAna_1stSort(string label = "HIJet20GeV48fm_Al",int initfile=0,int endfile = 200, bool doetopcut=false, bool dodphicut=false)

foreach sample (HIJet)
  #foreach vtxtype (04fm 48fm 04fmR02 48fmR02)
#  foreach vtx (48fm 48fmR02)
  foreach vtx (48fmR02)
    foreach ihcal (Al)
      foreach energy (20GeV 30GeV 40GeV 50GeV)
        echo "***** Run" $sample$energy$vtx\_$ihcal
        root -l -b -q rootAna_1stSort.C\(\"$sample$energy$vtx\_$ihcal\",$initfile,$endfile,false,false\)
        #root -l -b -q rootAna_1stSort.C\(\"$sample$energy$vst\_$ihcal\",$initfile,$endfile,true,true\)
      end
    end
  end
end

echo "***** All Done" 
 
