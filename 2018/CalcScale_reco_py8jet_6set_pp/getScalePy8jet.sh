#!/bin/csh 

#https://unix.stackexchange.com/questions/207171/get-value-after-specific-word
#backtick `
#ls -l ./logs_py8jet/*.out | egrep -c '^-'
#ls -l ./logs_py8jet/ | egrep -c '*.out'

###########################################################################3

set initfile = 0
#set endfile = 12000
set endfile = 24000
#set endfile = `ls -l ./logs_py8jet/ | egrep -c '*.out'`
#echo "total out files = $endfile"

if (-d txt_scale_py8jet) then
  echo "N.B. txt_scale_py8jet dir already exits! It will be removed and newly created!"
  rm -r txt_scale_py8jet
endif
mkdir txt_scale_py8jet

foreach ihcal (Al Alframe)
#foreach ihcal (Alframe)
  #foreach note (R02 R04)
  foreach note (R04)
    foreach method (before truthjet)
      foreach energy (20GeV 30GeV 40GeV 50GeV)
        echo "Run log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note"
        awk '{for (I=1;I<=NF;I++) if ($I == "cemcScale") {print $(I+2)};}' logs_py8jet/log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note >>txt_scale_py8jet/cemcScale_$method\_$ihcal\_$note.out 
        awk '{for (I=1;I<=NF;I++) if ($I == "ihcalScale") {print $(I+2)};}' logs_py8jet/log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note >>txt_scale_py8jet/ihcalScale_$method\_$ihcal\_$note.out 
        awk '{for (I=1;I<=NF;I++) if ($I == "ohcalScale") {print $(I+2)};}' logs_py8jet/log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note >>txt_scale_py8jet/ohcalScale_$method\_$ihcal\_$note.out 
      end
    end
  end
end

echo "energy 20,30,40,50 itertaion in a log"
echo "*** All done"
