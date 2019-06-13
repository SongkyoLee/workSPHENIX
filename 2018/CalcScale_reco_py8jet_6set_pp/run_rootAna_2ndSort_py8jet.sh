#!/bin/csh

set initfile = 0
#set endfile = 4000
#set initfile = 4000
#set endfile = 8000
#set initfile = 8000
#set endfile = 12000
#set initfile = 12000
#set endfile = 16000
#set initfile = 16000
#set endfile = 20000
#set initfile = 20000
set endfile = 24000

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
    foreach note (R04)
      root -l -b -q rootAna_2ndSort_qcdjet_ptbinning_py8jet.C\(\"$sample\",\"$ihcal\",\"allGeV\",\"noGeV\",\"$note\",$initfile,$endfile,true,true\)
    end
  end
end

echo "***** All Done"  
