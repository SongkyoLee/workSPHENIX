#!/bin/csh

set initfile = 0
set endfile = 400

#void rootAna_2ndSort_qcdjet_lorentz_ptbinning(string sampleType = "QCDJet",
#                              string ihcalType = "SS310",
#                              string jetInE = "20GeV",
#                              string jetOutE = "20GeV",
#                              string note = "R02",
#                              int initfile=0,
#                              int endfile =400,
#                              bool doetopcut=true,
#                              bool dodphicut=true,
#                              bool dodscale=true)

foreach sample (QCDJet)
  foreach ihcal (SS310 Al Alframe)
  #foreach ihcal (SS310)
    foreach note (R02 R04)
    #foreach note (R04)
      ###1) noGeV
      #root -l -b -q rootAna_2ndSort_qcdjet_lorentz_ptbinning.C\(\"$sample\",\"$ihcal\",\"20GeV\",\"noGeV\",\"$note\",$initfile,$endfile,true,true\)
      ###2) allGeV
      #root -l -b -q rootAna_2ndSort_qcdjet_lorentz_ptbinning.C\(\"$sample\",\"$ihcal\",\"20GeV\",\"allGeV\",\"$note\",$initfile,$endfile,true,true\)
      ###3) diff E
      foreach energy (20GeV 30GeV 40GeV 50GeV)
        echo "***** Run" $sample$energy\_$ihcal\_$note
        root -l -b -q rootAna_2ndSort_qcdjet_lorentz_ptbinning.C\(\"$sample\",\"$ihcal\",\"$energy\",\"$energy\",\"$note\",$initfile,$endfile,true,true,false\)
        root -l -b -q rootAna_2ndSort_qcdjet_lorentz_ptbinning.C\(\"$sample\",\"$ihcal\",\"$energy\",\"$energy\",\"$note\",$initfile,$endfile,true,true,true\)
      end
    end
  end
end

echo "***** All Done"  
