#!/bin/csh

set initfile = 0
set endfile = 400

foreach ihcal (SS310 Al Alframe)
  foreach note (R02 R04)
    foreach energy (20GeV 30GeV 40GeV 50GeV)
      #foreach method (before truthjet truthjet_2par)
      foreach method (before truthjet)
        root -l -b -q get_total_sf.C\(\"$ihcal\",\"$energy\",\"$note\",\"$method\",$initfile,$endfile\)
      end
    end
  end
end

echo "***** All Done" 
 
