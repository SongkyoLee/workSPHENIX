#!/bin/csh

set initfile = 0
#set endfile = 1000
set endfile = 400

foreach sample (QCDJet)
  foreach note (R04)
    foreach ihcal (SS310 Alframe Al)
      #foreach energy (20GeV 30GeV 40GeV 50GeV)
      foreach energy (20GeV)
        echo "***** Run" $sample$energy$note\_$ihcal
        root -l -b -q rootAna_2ndSort_qcdjet.C\(\"$sample\",\"$ihcal\",\"$energy\",\"$energy\",\"$note\",$initfile,$endfile,true,true\)
      end
    end
  end
end

echo "***** All Done"  
