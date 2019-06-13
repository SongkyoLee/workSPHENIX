#!/bin/csh

######################################################
###### NOT USED anymore!!! (replaced with method05!!!!)
######################################################

#void method01_asymm_gammajet(string sampleType = "GammaJet",
#                              string ihcalType = "SS310",
#                              string jetE = "50GeV",
#                              int initfile=0,
#                              int endfile =1000,
#                              float c_a=1.00,
#                              float c_b1=1.00,
#                              float c_b2 = 1.00,
#                              float c_c=1.00,
#                              bool doetopcut=true,
#                              bool dodphicut=true)

set initfile = 0
#set endfile = 1000
set endfile = 9000

set ii = $initfile
set jj = $initfile
set step = 300

########## before
while ($ii < $endfile)
  @ jj = $ii + $step
  echo "*** Run init = " $ii
  echo "end = " $jj
  foreach ihcal (SS310 Alframe)
    #foreach energy (20GeV 30GeV 40GeV 50GeV)
    foreach energy (20GeV)
      root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"$ihcal\",\"$energy\",$ii,$jj,1.00,1.00,1.00,1.00,true,true\) >&logs/log_before_$ii\_$jj\_$energy\_$ihcal&
      sleep 3s
    end
  end
  @ ii = $ii + $step
end

set ii = $initfile
set jj = $initfile
set step = 300

######################################################

########## empty method01
while ($ii < $endfile)
  @ jj = $ii + $step
  echo "*** Run init = " $ii
  echo "end = " $jj
  foreach ihcal (SS310 Alframe)
    #foreach energy (20GeV 30GeV 40GeV 50GeV)
    foreach energy (20GeV)
      root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"$ihcal\",\"$energy\",$ii,$jj,1.00,1.00,1.00,1.00,true,true\) >&logs/log_method01_$ii\_$jj\_$energy\_$ihcal&
      sleep 3s
    end
  end
  @ ii = $ii + $step
end

########## before
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"SS310\",\"20GeV\",$initfile,$endfile,1.00,1.00,1.00,1.00,true,true\)
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"SS310\",\"30GeV\",$initfile,$endfile,1.00,1.00,1.00,1.00,true,true\)
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"SS310\",\"40GeV\",$initfile,$endfile,1.00,1.00,1.00,1.00,true,true\)
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"SS310\",\"50GeV\",$initfile,$endfile,1.00,1.00,1.00,1.00,true,true\)
### c_c
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"SS310\",\"20GeV\",$initfile,$endfile,1.00,1.00,1.00,2.25,true,true\)
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"SS310\",\"30GeV\",$initfile,$endfile,1.00,1.00,1.00,1.90,true,true\)
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"SS310\",\"40GeV\",$initfile,$endfile,1.00,1.00,1.00,1.72,true,true\)
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"SS310\",\"50GeV\",$initfile,$endfile,1.00,1.00,1.00,1.63,true,true\)
### c_b, c_c
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"SS310\",\"20GeV\",$initfile,$endfile,1.00,1.06,1.00,2.25,true,true\)
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"SS310\",\"30GeV\",$initfile,$endfile,1.00,1.00,1.14,1.90,true,true\)
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"SS310\",\"40GeV\",$initfile,$endfile,1.00,1.00,1.34,1.72,true,true\)
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"SS310\",\"50GeV\",$initfile,$endfile,1.00,1.00,1.36,1.63,true,true\)
### a_a, c_b, c_c
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"SS310\",\"20GeV\",$initfile,$endfile,1.65,1.06,1.00,2.25,true,true\)
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"SS310\",\"30GeV\",$initfile,$endfile,1.37,1.00,1.14,1.90,true,true\)
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"SS310\",\"40GeV\",$initfile,$endfile,1.19,1.00,1.34,1.72,true,true\)
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"SS310\",\"50GeV\",$initfile,$endfile,1.16,1.00,1.36,1.63,true,true\)

########## before
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"Alframe\",\"20GeV\",$initfile,$endfile,1.00,1.00,1.00,1.00,true,true\)
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"Alframe\",\"30GeV\",$initfile,$endfile,1.00,1.00,1.00,1.00,true,true\)
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"Alframe\",\"40GeV\",$initfile,$endfile,1.00,1.00,1.00,1.00,true,true\)
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"Alframe\",\"50GeV\",$initfile,$endfile,1.00,1.00,1.00,1.00,true,true\)
### c_b
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"Alframe\",\"20GeV\",$initfile,$endfile,1.00,1.12,1.00,1.00,true,true\)
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"Alframe\",\"30GeV\",$initfile,$endfile,1.00,1.00,1.01,1.00,true,true\)
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"Alframe\",\"40GeV\",$initfile,$endfile,1.00,1.00,1.19,1.00,true,true\)
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"Alframe\",\"50GeV\",$initfile,$endfile,1.00,1.00,1.29,1.00,true,true\)
### c_a, c_b
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"Alframe\",\"20GeV\",$initfile,$endfile,2.54,1.12,1.00,1.00,true,true\)
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"Alframe\",\"30GeV\",$initfile,$endfile,2.09,1.00,1.01,1.00,true,true\)
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"Alframe\",\"40GeV\",$initfile,$endfile,1.73,1.00,1.19,1.00,true,true\)
#root -l -b -q method01_asymm_gammajet.C\(\"GammaJet\",\"Alframe\",\"50GeV\",$initfile,$endfile,1.54,1.00,1.29,1.00,true,true\)

echo "***** All Done" 
 
