#!/bin/csh

#void draw_JESJERvsE_Syst01(string ihcalType="SS310", string note="R04"){

foreach ihcal (SS310 Al Alframe)
  foreach note (R02 R04)
      root -l -b -q  comp_JESJERvsE_syst.C\(\"$ihcal\",\"$note\"\)
  end
end

echo "***** All Done" 
 
