#!/bin/csh

set initfile = 0
set endfile = 300
#set endfile = 1000

#void rootAna_2ndSort_gammajet(string sampleType = "GammaJet",
#                              string ihcalType = "Alframe",
#                              string jetInE = "20GeV",
#                              string jetOutE = "20GeV",
#                              string vtxType = "vtx10gaus",
#                              int initfile=0,
#                              int endfile =1000,
#                              bool doetopcut=false,
#                              bool dodphicut=false)

set energy = "allGeV"

foreach sample (GammaJet)
#  foreach ihcal (SS310 Alframe)
  foreach ihcal (SS310)
  #foreach ihcal (Alframe)
    #foreach vtx (vtx0 vtx10flat vtx10gaus)
    foreach vtx (vtx10gaus)
      echo "***** Run" $sample\_$energy\_$vtx\_$ihcal
      ### ver1) reco_gamma_e > 20, |reco_gamma_eta| < 0.45 (higher emfrac, pid==22)
      root -l -b -q rootAna_2ndSort_gammajet_ver1.C\(\"$sample\",\"$ihcal\",\"$energy\",\"$energy\",\"$vtx\",$initfile,$endfile,true,true\)
      ### ver2) E_HCAL / E_EMCAL < 0.1 
      root -l -b -q rootAna_2ndSort_gammajet_ver2.C\(\"$sample\",\"$ihcal\",\"$energy\",\"$energy\",\"$vtx\",$initfile,$endfile,true,true\)
      ### ver3) reco_n ==1
      root -l -b -q rootAna_2ndSort_gammajet_ver3.C\(\"$sample\",\"$ihcal\",\"$energy\",\"$energy\",\"$vtx\",$initfile,$endfile,true,true\)
      ### ver4) dPhi(gamma-jet) > pi*1/2 (default!) 
      root -l -b -q rootAna_2ndSort_gammajet_ver4.C\(\"$sample\",\"$ihcal\",\"$energy\",\"$energy\",\"$vtx\",$initfile,$endfile,true,true\)
      ### ver5) dPhi(gamma-jet) > pi*7/8 
      root -l -b -q rootAna_2ndSort_gammajet_ver5.C\(\"$sample\",\"$ihcal\",\"$energy\",\"$energy\",\"$vtx\",$initfile,$endfile,true,true\)
      ### ver6) dPhi(gamma-jet) > pi*31/32 
      root -l -b -q rootAna_2ndSort_gammajet_ver6.C\(\"$sample\",\"$ihcal\",\"$energy\",\"$energy\",\"$vtx\",$initfile,$endfile,true,true\)
      ### ver7) (same as ver4), but cons_n ==1 
      root -l -b -q rootAna_2ndSort_gammajet_ver7.C\(\"$sample\",\"$ihcal\",\"$energy\",\"$energy\",\"$vtx\",$initfile,$endfile,true,true\)
      ### ver8) (same as ver4), but no reco_gamma_e cut
      root -l -b -q rootAna_2ndSort_gammajet_ver8.C\(\"$sample\",\"$ihcal\",\"$energy\",\"$energy\",\"$vtx\",$initfile,$endfile,true,true\)
    end
    end
  end
end

echo "***** All Done"  
