#!/bin/csh 

#https://unix.stackexchange.com/questions/207171/get-value-after-specific-word
#backtick `
#ls -l ./logs_syst06/*.out | egrep -c '^-'
#ls -l ./logs_syst06/ | egrep -c '*.out'

###########################################################################3

set initfile = 0
set endfile = 400
#set endfile = `ls -l ./logs_syst06/ | egrep -c '*.out'`
#echo "total out files = $endfile"

if (-d txt_scale_syst06) then
  echo "N.B. txt_scale_syst06 dir already exits! It will be removed and newly created!"
  rm -r txt_scale_syst06
endif
mkdir txt_scale_syst06

foreach ihcal (SS310 Al Alframe)
  foreach note (R02 R04)
    #foreach method (before truthjet truthjet_2par)
    foreach method (before truthjet)
      foreach energy (20GeV 30GeV 40GeV 50GeV)
        echo "Run log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note"
        awk '{for (I=1;I<=NF;I++) if ($I == "cemcScale") {print $(I+2)};}' logs_syst06/log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note >>txt_scale_syst06/cemcScale_$method\_$ihcal\_$note.out 
        awk '{for (I=1;I<=NF;I++) if ($I == "ihcalScale") {print $(I+2)};}' logs_syst06/log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note >>txt_scale_syst06/ihcalScale_$method\_$ihcal\_$note.out 
        awk '{for (I=1;I<=NF;I++) if ($I == "ohcalScale") {print $(I+2)};}' logs_syst06/log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note >>txt_scale_syst06/ohcalScale_$method\_$ihcal\_$note.out 
      end
    end
  end
end

echo "energy 20,30,40,50 itertaion in a log"
echo "*** All done"
