#!/bin/csh

#void draw_JESJERvsE_Syst01(string ihcalType="Al", string note="04fmR04"){

foreach ihcal (Al)
  foreach note1st (04fm 48fm)
    foreach note2nd (R02 R04)
      root -l -b -q  comp_JER_pp_HI_syst.C\(\"$ihcal\",\"$note1st\",\"$note2nd\"\)
    end
  end
end

echo "***** All Done" 
 
