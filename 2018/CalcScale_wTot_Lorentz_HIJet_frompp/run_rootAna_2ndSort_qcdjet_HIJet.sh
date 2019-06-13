#!/bin/csh

set initfile = 0
set endfile = 200
#set endfile = 1000

foreach sample (HIJet)
  foreach note (04fmR04 04fmR02 48fmR04 48fmR02)
  #foreach note (04fmR04 04fmR02)
    foreach ihcal (Al)
      foreach energy (20GeV 30GeV 40GeV 50GeV)
        echo "***** Run" $sample$energy$note\_$ihcal
        root -l -b -q rootAna_2ndSort_qcdjet.C\(\"$sample\",\"$ihcal\",\"$energy\",\"$energy\",\"$note\",$initfile,$endfile,false,false\)
      end
    end
  end
end

echo "***** All Done"  
