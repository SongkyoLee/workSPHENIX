#!/bin/csh

#https://unix.stackexchange.com/questions/207171/get-value-after-specific-word
#backtick `
#ls -l ./logs/*.out | egrep -c '^-'
#ls -l ./logs/ | egrep -c '*.out'

###########################################################################3

set initfile = 0
set qcdjetendfile = 1000

if (-d txt_conserv) then
  echo "N.B. txt_conserv dir already exits! It will be removed and newly created!"
  rm -r txt_conserv
endif
mkdir txt_conserv

set counter = 0

foreach ihcal (SS310 Alframe)
  foreach energy (20GeV)
    echo "Run log_final_qcd_conserv_$energy\_$ihcal"
    awk '{for (I=1;I<=NF;I++) if ($I == "jes_mean") {print $(I+2)};}' logs/log_final_qcd_conserv_minimum_$energy\_$ihcal >>txt_conserv/jes_mean_minimum_$ihcal.out 
    awk '{for (I=1;I<=NF;I++) if ($I == "jes_sig") {print $(I+2)};}' logs/log_final_qcd_conserv_minimum_$energy\_$ihcal >>txt_conserv/jes_sig_minimum_$ihcal.out 
    awk '{for (I=1;I<=NF;I++) if ($I == "jer_sig") {print $(I+2)};}' logs/log_final_qcd_conserv_minimum_$energy\_$ihcal >>txt_conserv/jer_sig_minimum_$ihcal.out 
    awk '{for (I=1;I<=NF;I++) if ($I == "jes_mean") {print $(I+2)};}' logs/log_final_qcd_conserv_maximum_$energy\_$ihcal >>txt_conserv/jes_mean_maximum_$ihcal.out 
    awk '{for (I=1;I<=NF;I++) if ($I == "jes_sig") {print $(I+2)};}' logs/log_final_qcd_conserv_maximum_$energy\_$ihcal >>txt_conserv/jes_sig_maximum_$ihcal.out 
    awk '{for (I=1;I<=NF;I++) if ($I == "jer_sig") {print $(I+2)};}' logs/log_final_qcd_conserv_maximum_$energy\_$ihcal >>txt_conserv/jer_sig_maximum_$ihcal.out 
  end
end

echo "minimum/maximum iteration in a log"
echo "***** All Done" 
 

