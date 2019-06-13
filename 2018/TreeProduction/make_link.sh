#!/bin/csh

foreach ihcal (SS310 Al Alframe) 
  foreach note (R02 R04)
    #foreach energy (20GeV 30GeV 40GeV 50GeV)
    foreach energy (40GeV 50GeV)
      #ls -al out_QCDJet$energy$note\_$ihcal 
      #ls /sphenix/user/kyolee/outRootFiles/out_20180717_$ihcal\_$energy\_$note out_QCDJet$energy$note\_$ihcal | wc
      rmf out_QCDJet$energy$note\_$ihcal
      ln -s /sphenix/user/kyolee/outRootFiles/out_20180717_$ihcal\_$energy\_$note out_QCDJet$energy$note\_$ihcal

    end  
  end  
end  

echo "***** All Done"  

