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

#set outJES = "JES_final_qcd"
set outJES = "JES_$1" 
echo $outJES.out
if (-f $outJES.out) then 
  echo "N.B. $outJES.out already exits! It will be removed and newly created!"
  rm -f $outJES.out
endif

#cat log_final_qcd

#set ir = $initfile
#while ($ir < $endfile)
# # echo "$ir th file"
#  awk '{for (I=1;I<=NF;I++) if ($I == "*jes_mean") {print $(I+2)};}' log_final_qcd
awk '{for (I=1;I<=NF;I++) if ($I == "*jes_mean") {print $(I+2)};}' log_$1 >>$outJES.out
#  @ ir++
#end

###########################################################################3

set outJER = "JER_$1" 
echo $outJER.out
if (-f $outJER.out) then 
  echo "N.B. $outJER.out already exits! It will be removed and newly created!"
  rm -f $outJER.out
endif

awk '{for (I=1;I<=NF;I++) if ($I == "*jer_sig") {print $(I+2)};}' log_$1 >>$outJER.out

echo "*** All done"
