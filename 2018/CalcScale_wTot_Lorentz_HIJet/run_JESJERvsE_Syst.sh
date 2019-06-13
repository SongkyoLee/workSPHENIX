#!/bin/csh

#void draw_JESJERvsE_Syst01(string ihcalType="Al", string note="04fmR04"){

foreach ihcal (Al)
  foreach note (04fmR02 04fmR04 48fmR02 48fmR04)
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
 
