#!/bin/csh 

#https://unix.stackexchange.com/questions/207171/get-value-after-specific-word
#backtick `
#ls -l ./logs/*.out | egrep -c '^-'
#ls -l ./logs/ | egrep -c '*.out'

###########################################################################3

set initfile = 0
set endfile = 200
#set endfile = 21000
#set endfile = `ls -l ./logs/ | egrep -c '*.out'`
#echo "total out files = $endfile"

if (-d txt_jes) then
  echo "N.B. txt_jes dir already exits! It will be removed and newly created!"
  rm -r txt_jes
endif
mkdir txt_jes

#foreach ihcal (SS310 Al Alframe)
foreach ihcal (Al)
  foreach note (48fmR02 48fmR04)
    foreach method (before recogamma truthjet)
      foreach energy (20GeV 30GeV 40GeV 50GeV)
        echo "Run log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note"
        awk '{for (I=1;I<=NF;I++) if ($I == "jes_mean") {print $(I+2)};}' logs/log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note >>txt_jes/jes_mean_$method\_$ihcal\_$note.out 
        awk '{for (I=1;I<=NF;I++) if ($I == "jes_sig/mean") {print $(I+2)};}' logs/log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note >>txt_jes/jes_sigmean_$method\_$ihcal\_$note.out 
        #awk '{for (I=1;I<=NF;I++) if ($I == "jer_sig") {print $(I+2)};}' logs/log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note >>txt_jes/jer_sig_$method\_$ihcal\_$note.out 
      end
    end
  end
end

echo "energy 20,30,40,50 itertaion in a log"
echo "*** All done"

