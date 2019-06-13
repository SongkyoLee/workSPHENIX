#!/bin/csh

set initfile = 0
set endfile = 1000

foreach sample (QCDJet ObjectiveKPP ThresholdKPP)
#foreach sample (ObjectiveKPP ThresholdKPP)
#foreach sample (QCDJet)
  foreach ihcal (Al)
    foreach energy (20GeV 30GeV 40GeV 50GeV)
    #foreach energy (20GeV)
      #foreach radi (R02 R04)
      #foreach radi (R04)
      foreach radi (R02)
        echo "***** Run" $sample$energy\_$ihcal
        root -l -b -q rootAna_2ndSort_qcdjet_kpp_pt.C\(\"$sample\",\"$ihcal\",\"$energy\",\"$energy\",\"$radi\",$initfile,$endfile,true,true\)
      end
    end
  end
end

echo "***** All Done"  
