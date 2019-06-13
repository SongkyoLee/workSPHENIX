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

if (-d txt_jet) then
  echo "N.B. txt_jet dir already exits! It will be removed and newly created!"
  rm -r txt_jet
endif
mkdir txt_jet

#foreach ihcal (SS310 Al Alframe)
foreach ihcal (Al)
  foreach note (04fmR02 04fmR04 48fmR02 48fmR04)
    #foreach method (before truthjet truthjet_2par)
    foreach method (before truthjet_hi)
      foreach energy (20GeV 30GeV 40GeV 50GeV)
        echo "Run log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note"
        awk '{for (I=1;I<=NF;I++) if ($I == "jes_mean") {print $(I+2)};}' logs/log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note >>txt_jet/jes_$method\_$ihcal\_$note.out 
        awk '{for (I=1;I<=NF;I++) if ($I == "jes_sig/mean") {print $(I+2)};}' logs/log_final_qcd_$method\_$initfile\_$endfile\_$energy\_$ihcal\_$note >>txt_jet/jer_$method\_$ihcal\_$note.out 
      end
    end
  end
end

echo "energy 20,30,40,50 itertaion in a log"
echo "*** All done"

