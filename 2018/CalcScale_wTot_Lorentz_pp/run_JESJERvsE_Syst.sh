#!/bin/csh

#void draw_JESJERvsE_Syst01(string ihcalType="SS310", string note="R04"){

foreach ihcal (SS310 Al Alframe)
  foreach note (R02 R04)
      root -l -b -q  draw_JESJERvsE_Syst01.C\(\"$ihcal\",\"$note\"\)
      root -l -b -q  draw_JESJERvsE_Syst02.C\(\"$ihcal\",\"$note\"\)
      root -l -b -q  draw_JESJERvsE_Syst03.C\(\"$ihcal\",\"$note\"\)
      root -l -b -q  draw_JESJERvsE_Syst04.C\(\"$ihcal\",\"$note\"\)
      root -l -b -q  draw_JESJERvsE_Syst05.C\(\"$ihcal\",\"$note\"\)
      root -l -b -q  draw_JESJERvsE_Syst06.C\(\"$ihcal\",\"$note\"\)
      root -l -b -q  draw_JESJERvsE_Syst07.C\(\"$ihcal\",\"$note\"\)
      root -l -b -q  draw_JESJERvsE_Syst08.C\(\"$ihcal\",\"$note\"\)
  end
end

echo "***** All Done" 
 
