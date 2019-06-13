#!/bin/csh

set initfile = 0
set endfile = 400

#void rootAna_2ndSort_qcdjet(string sampleType = "QCDJet",
#                              string ihcalType = "Al",
#                              string jetInE = "20GeV",
#                              string jetOutE = "20GeV",
#                              string note = "R02",
#                              int initfile=0,
#                              int endfile =400,
#                              bool doetopcut=true,
#                              bool dodphicut=true)

foreach sample (QCDJet)
  foreach ihcal (SS310 Al Alframe)
  #foreach ihcal (Alframe)
    foreach note (R02 R04)
    #foreach note (R04)
      foreach energy (20GeV 30GeV 40GeV 50GeV)
        echo "***** Run" $sample$energy\_$ihcal\_$note
        root -l -b -q rootAna_2ndSort_qcdjet.C\(\"$sample\",\"$ihcal\",\"$energy\",\"$energy\",\"$note\",$initfile,$endfile,true,true\)
      end
    end
  end
end

echo "***** All Done"  
