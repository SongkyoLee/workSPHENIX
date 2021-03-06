#!/bin/csh

#https://unix.stackexchange.com/questions/207171/get-value-after-specific-word
#backtick `
#ls -l ./logs/*.out | egrep -c '^-'
#ls -l ./logs/ | egrep -c '*.out'

###########################################################################3

set initfile = 0
#set endfile = 9000
set endfile = 9000
set qcdjetendfile = 1000
#set endfile = `ls -l ./logs/ | egrep -c '*.out'`
#echo "total out files = $endfile"

set ii = $initfile
set jj = $initfile
set step = 300

if (-d txt_scale) then
  echo "N.B. txt_scale dir already exits! It will be removed and newly created!"
  rm -r txt_scale
endif
mkdir txt_scale

set counter = 0

while ($ii < $endfile)
  @ jj = $ii + $step
  echo "*** Run init = " $ii
  echo "end = " $jj

  ##### (before method01 method02 method03 method04 method5))
  foreach ihcal (SS310 Alframe)
    foreach method (before method01 method02 method03 method04 method05)
      #foreach energy (20GeV 30GeV 40GeV 50GeV)
      foreach energy (20GeV)
        echo "Run log_final_qcd_$method\_$ii\_$jj\_$energy\_$ihcal"
        #awk '{for (I=1;I<=NF;I++) if ($I == "cemcScale") {print $(I+2)};}' logs/log_final_qcd_$method\_$ii\_$jj\_$energy\_$ihcal >>txt_scale/cemcScale_$method\_$ihcal\_$counter.out 
        #awk '{for (I=1;I<=NF;I++) if ($I == "ihcalScale") {print $(I+2)};}' logs/log_final_qcd_$method\_$ii\_$jj\_$energy\_$ihcal >>txt_scale/ihcalScale_$method\_$ihcal\_$counter.out 
        #awk '{for (I=1;I<=NF;I++) if ($I == "ohcalScale") {print $(I+2)};}' logs/log_final_qcd_$method\_$ii\_$jj\_$energy\_$ihcal >>txt_scale/ohcalScale_$method\_$ihcal\_$counter.out 
        awk '{for (I=1;I<=NF;I++) if ($I == "cemcScale") {print $(I+2)};}' logs/log_final_qcd_$method\_$ii\_$jj\_$energy\_$ihcal >>txt_scale/cemcScale_$method\_$ihcal.out 
        awk '{for (I=1;I<=NF;I++) if ($I == "ihcalScale") {print $(I+2)};}' logs/log_final_qcd_$method\_$ii\_$jj\_$energy\_$ihcal >>txt_scale/ihcalScale_$method\_$ihcal.out 
        awk '{for (I=1;I<=NF;I++) if ($I == "ohcalScale") {print $(I+2)};}' logs/log_final_qcd_$method\_$ii\_$jj\_$energy\_$ihcal >>txt_scale/ohcalScale_$method\_$ihcal.out 
      end
    end
  end
  @ ii = $ii + $step
  @ counter++;
end

echo "$endfile/$step sets iteration in a log"
echo "***** All Done" 
 

