#!/bin/csh

set initfile = 0
set endfile = 200

foreach ihcal (Al)
  foreach note (04fmR02 04fmR04 48fmR02 48fmR04)
    foreach energy (20GeV 30GeV 40GeV 50GeV)
      #foreach method (before truthjet truthjet_2par)
      foreach method (before truthjet_hi)
        root -l -b -q get_total_sf.C\(\"$ihcal\",\"$energy\",\"$note\",\"$method\",$initfile,$endfile\)
      end
    end
  end
end

echo "***** All Done" 
 
