#!/bin/csh 

#https://unix.stackexchange.com/questions/207171/get-value-after-specific-word
#backtick `
#ls -l ./logs/*.out | egrep -c '^-'
#ls -l ./logs/ | egrep -c '*.out'

###########################################################################3

set initfile = 0
set endfile = 1000
#set endfile = `ls -l ./logs/ | egrep -c '*.out'`
#echo "total out files = $endfile"

if (-d txt_scale) then
  echo "N.B. txt_scale dir already exits! It will be removed and newly created!"
  rm -r txt_scale
endif
mkdir txt_scale

#foreach ihcal (SS310 Al Alframe)
foreach ihcal (Al Alframe)
  foreach note (R02 R04)
    foreach method (before recogamma truthjet)
      foreach energy (20GeV 30GeV 40GeV 50GeV)
        echo "Run log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note"
        awk '{for (I=1;I<=NF;I++) if ($I == "cemcScale") {print $(I+2)};}' logs/log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note >>txt_scale/cemcScale_$method\_$ihcal\_$note.out 
        awk '{for (I=1;I<=NF;I++) if ($I == "ihcalScale") {print $(I+2)};}' logs/log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note >>txt_scale/ihcalScale_$method\_$ihcal\_$note.out 
        awk '{for (I=1;I<=NF;I++) if ($I == "ohcalScale") {print $(I+2)};}' logs/log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note >>txt_scale/ohcalScale_$method\_$ihcal\_$note.out 
      end
    end
  end
end

echo "energy 20,30,40,50 itertaion in a log"
echo "*** All done"
