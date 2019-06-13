#!/bin/csh

#void draw_JESJERvsE_Syst01(string ihcalType="Al", string note="04fmR04"){

foreach ihcal (Al)
  foreach note (04fmR02 04fmR04 48fmR02 48fmR04)
      root -l -b -q  comp_JESJERvsE_syst.C\(\"$ihcal\",\"$note\"\)
  end
end

echo "***** All Done" 
 
