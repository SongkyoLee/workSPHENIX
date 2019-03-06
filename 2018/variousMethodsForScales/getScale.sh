#!/bin/csh 

#https://unix.stackexchange.com/questions/207171/get-value-after-specific-word
#backtick `
#ls -l ./logs/*.out | egrep -c '^-'
#ls -l ./logs/ | egrep -c '*.out'

#set initfile = 0
#set endfile = 310
#set endfile = `ls -l ./logs/ | egrep -c '*.out'`

#echo "total out files = $endfile"

###########################################################################3

#set outcemc = "cemc_final_qcd"
#set outcemc = "cemc_final_qcd_wTot"
set outcemc = "SCALEcemc_$1" 
echo $outcemc.out
if (-f $outcemc.out) then 
  echo "N.B. $outcemc.out already exits! It will be removed and newly created!"
  rm -f $outcemc.out
endif

#cat log_final_qcd

#set ir = $initfile
#while ($ir < $endfile)
# # echo "$ir th file"
#  awk '{for (I=1;I<=NF;I++) if ($I == "cemcScale") {print $(I+2)};}' log_final_qcd
awk '{for (I=1;I<=NF;I++) if ($I == "cemcScale") {print $(I+2)};}' log_$1 >>$outcemc.out
#  @ ir++
#end

###########################################################################3

set outihcal = "SCALEihcal_$1" 
echo $outihcal.out
if (-f $outihcal.out) then 
  echo "N.B. $outihcal.out already exits! It will be removed and newly created!"
  rm -f $outihcal.out
endif

awk '{for (I=1;I<=NF;I++) if ($I == "ihcalScale") {print $(I+2)};}' log_$1 >>$outihcal.out

###########################################################################3

set outohcal = "SCALEohcal_$1" 
echo $outohcal.out
if (-f $outohcal.out) then 
  echo "N.B. $outohcal.out already exits! It will be removed and newly created!"
  rm -f $outohcal.out
endif

awk '{for (I=1;I<=NF;I++) if ($I == "ohcalScale") {print $(I+2)};}' log_$1 >>$outohcal.out


echo "*** All done"
