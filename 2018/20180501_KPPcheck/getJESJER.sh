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

if (-d txt_jes) then
  echo "N.B. txt_jes dir already exits! It will be removed and newly created!"
  rm -r txt_jes
endif
mkdir txt_jes

foreach sample (QCDJet ObjectiveKPP ThresholdKPP)
#foreach ihcal (SS310 Alframe Al)
  foreach energy (20GeV 30GeV 40GeV 50GeV)
    foreach res(R02 R04)
      echo "Run log_onefile_$initfile\_$endfile\_$energy\_$sample\_$res"
      awk '{for (I=1;I<=NF;I++) if ($I == "jes_mean") {print $(I+2)};}' logs/log_onefile_$initfile\_$endfile\_$energy\_$sample\_$res >>txt_jes/jes_mean_$sample\_$res.out 
      awk '{for (I=1;I<=NF;I++) if ($I == "jes_sig/mean") {print $(I+2)};}' logs/log_onefile_$initfile\_$endfile\_$energy\_$sample\_$res >>txt_jes/jes_sigmean_$sample\_$res.out 
    end
  end
end

echo "energy 20,30,40,50 itertaion in a log"
echo "*** All done"

