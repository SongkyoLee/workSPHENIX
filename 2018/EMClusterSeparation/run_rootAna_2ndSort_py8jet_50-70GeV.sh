#!/bin/csh

set initfile = 0
set endfile = 1000

#void rootAna_2ndSort_qcdjet_ptbinning_py8jet(string sampleType = "QCDJet",
#                              string ihcalType = "Al",
#                              string jetInE = "20GeV",
#                              string jetOutE = "20GeV",
#                              string note = "R02",
#                              int initfile=0,
#                              int endfile =400,
#                              bool doetopcut=true,
#                              bool dodphicut=true)

foreach sample (py8Jet)
  foreach ihcal (Al Alframe)
  #foreach ihcal (Al)
    #foreach note (R02 R04)
    foreach note (R02)
      foreach energy (50GeV 60GeV 70GeV)
        root -l -b -q rootAna_2ndSort_qcdjet_ptbinning_py8jet.C\(\"$sample\",\"$ihcal\",\"$energy\",\"$energy\",\"$note\",$initfile,$endfile,true,true\)
      end
    end
  end
end

echo "***** All Done"  
