#!/bin/csh 

#https://unix.stackexchange.com/questions/207171/get-value-after-specific-word
#backtick `
#ls -l ./logs/*.out | egrep -c '^-'
#ls -l ./logs/ | egrep -c '*.out'

###########################################################################3

set initfile = 0
set endfile = 200
#set endfile = `ls -l ./logs/ | egrep -c '*.out'`
#echo "total out files = $endfile"

if (-d txt_scale_wTot) then
  echo "N.B. txt_scale_wTot dir already exits! It will be removed and newly created!"
  rm -r txt_scale_wTot
endif
mkdir txt_scale_wTot

#foreach ihcal (SS310 Al Alframe)
foreach ihcal (Al)
  foreach note (04fmR02 04fmR04 48fmR02 48fmR04)
    #foreach method (before truthjet truthjet_2par)
    foreach method (before truthjet)
      foreach energy (20GeV 30GeV 40GeV 50GeV)
        echo "Run log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note"
        awk '{for (I=1;I<=NF;I++) if ($I == "cemcScale") {print $(I+2)};}' logs_wTot/log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note >>txt_scale_wTot/cemcScale_$method\_$ihcal\_$note.out 
        awk '{for (I=1;I<=NF;I++) if ($I == "ihcalScale") {print $(I+2)};}' logs_wTot/log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note >>txt_scale_wTot/ihcalScale_$method\_$ihcal\_$note.out 
        awk '{for (I=1;I<=NF;I++) if ($I == "ohcalScale") {print $(I+2)};}' logs_wTot/log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note >>txt_scale_wTot/ohcalScale_$method\_$ihcal\_$note.out 
        awk '{for (I=1;I<=NF;I++) if ($I == "totalScale") {print $(I+2)};}' logs_wTot/log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note >>txt_scale_wTot/totalScale_$method\_$ihcal\_$note.out 
      end
    end
  end
end

echo "energy 20,30,40,50 itertaion in a log"
echo "*** All done"
