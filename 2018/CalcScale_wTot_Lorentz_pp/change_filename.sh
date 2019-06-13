#!/bin/csh

set initfile = 0
set endfile = 1000

foreach sample (GammaJet QCDJet)
  foreach ihcal (SS310 Alframe)
    foreach energy (20GeV 30GeV 40GeV 50GeV)
      #ls -al sortedRootFiles/sorted_0to1000_$sample\_G4sPHENIX_jet4_$energy\_$ihcal\_doetopcut0_dodphicut0.root
      #ls -al sortedRootFiles/sorted_0to1000_$sample\_G4sPHENIX_jet4_$energy\_$ihcal\_doetopcut1_dodphicut1.root
      mv sortedRootFiles/sorted_0to1000_$sample\_G4sPHENIX_jet4_$energy\_$ihcal\_doetopcut0_dodphicut0.root sortedRootFiles/sorted_0to1000_$sample\_G4sPHENIX_jet4_$energy\_$ihcal\_R04_doetopcut0_dodphicut0.root
      mv sortedRootFiles/sorted_0to1000_$sample\_G4sPHENIX_jet4_$energy\_$ihcal\_doetopcut1_dodphicut1.root sortedRootFiles/sorted_0to1000_$sample\_G4sPHENIX_jet4_$energy\_$ihcal\_R04_doetopcut1_dodphicut1.root
    end
  end
end

echo "***** All Done" 
 
