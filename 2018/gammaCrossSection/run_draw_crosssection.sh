#!/bin/csh

set initfile = 0
set endfile = 300
#set endfile = 1000

#void draw_crosssection(string sampleType = "GammaJet",
#                              string ihcalType = "SS310",
#                              string jetE = "allGeV",
#                              string version = "ver7",
#                              int initfile=0,
#                              int endfile =300,
#                              bool doetopcut=true,
#                              bool dodphicut=true)

set energy = "allGeV"

foreach sample (GammaJet)
  foreach ihcal (SS310)
    foreach vtx (vtx10gaus)
      echo "***** Run" $sample\_$energy\_$vtx\_$ihcal
      root -l -b -q draw_crosssection.C\(\"$sample\",\"$ihcal\",\"$energy\",\"ver1\",$initfile,$endfile,true,true\)
      root -l -b -q draw_crosssection.C\(\"$sample\",\"$ihcal\",\"$energy\",\"ver2\",$initfile,$endfile,true,true\)
      root -l -b -q draw_crosssection.C\(\"$sample\",\"$ihcal\",\"$energy\",\"ver3\",$initfile,$endfile,true,true\)
      root -l -b -q draw_crosssection.C\(\"$sample\",\"$ihcal\",\"$energy\",\"ver4\",$initfile,$endfile,true,true\)
      root -l -b -q draw_crosssection.C\(\"$sample\",\"$ihcal\",\"$energy\",\"ver5\",$initfile,$endfile,true,true\)
      root -l -b -q draw_crosssection.C\(\"$sample\",\"$ihcal\",\"$energy\",\"ver6\",$initfile,$endfile,true,true\)
      root -l -b -q draw_crosssection.C\(\"$sample\",\"$ihcal\",\"$energy\",\"ver7\",$initfile,$endfile,true,true\)
    end
  end
end

echo "***** All Done"  
