
#include <iostream>
#include <sstream>
#include <string> 
#include "TROOT.h"
#include "TChain.h"
#include "TMath.h"
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TNtuple.h"
#include "TRandom3.h"
#include "TVector.h"
#include "TRotation.h"
#include "TF1.h"
#include "TFile.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TSpectrum.h"
#include "TVector3.h"

#include <string.h>
#include "TChain.h"
#include "TTree.h"
#include "TKey.h"
#include "Riostream.h"

#include "TError.h"

#include <vector>

void rootAna_mergefiles(string inType = "SS310_PiPl_25GeV",
                  int initfile=0,
                  int endfile =200)
{
  //// Suppress error messages from bad files
  gErrorIgnoreLevel = kFatal; 
  int bad_files = 0; 
    
  int event=1; //start from event 1!! 
  int track_n;  
  float track_p[999];  
  float cemc_E3x3[999], ihcal_E3x3[999], ohcal_E3x3[999];
  float cemc_E5x5[999], ihcal_E5x5[999], ohcal_E5x5[999];
  float cemc_clE[999], ihcal_clE[999], ohcal_clE[999];
  
  int old_event; 
  int old_track_n;  
  float old_track_p[999];  
  float old_cemc_E3x3[999], old_ihcal_E3x3[999], old_ohcal_E3x3[999];
  float old_cemc_E5x5[999], old_ihcal_E5x5[999], old_ohcal_E5x5[999];
  float old_cemc_clE[999], old_ihcal_clE[999], old_ohcal_clE[999];
    
  //// write new tree 
  string foutname = Form("./mergedRootFiles/merged_%dto%d_ParticleGun_G4sPHENIX_%s.root",initfile,endfile,inType.c_str());
  //string foutname = "test.root";
  cout << "foutname" << foutname << endl;
  TFile* outFile = new TFile( foutname.c_str(), "RECREATE");
  TTree* outTree = new TTree("track_tree","all reco track");
  outTree->Branch("event",&event,"event/I"); 
  outTree->Branch("track_n",&track_n,"track_n/I"); 
  outTree->Branch("track_p",&track_p,"track_p[track_n]/F"); 
  outTree->Branch("cemc_E3x3",&cemc_E3x3,"cemc_E3x3[track_n]/F"); 
  outTree->Branch("ihcal_E3x3",&ihcal_E3x3,"ihcal_E3x3[track_n]/F"); 
  outTree->Branch("ohcal_E3x3",&ohcal_E3x3,"ohcal_E3x3[track_n]/F"); 
  outTree->Branch("cemc_E5x5",&cemc_E5x5,"cemc_E5x5[track_n]/F"); 
  outTree->Branch("ihcal_E5x5",&ihcal_E5x5,"ihcal_E5x5[track_n]/F"); 
  outTree->Branch("ohcal_E5x5",&ohcal_E5x5,"ohcal_E5x5[track_n]/F"); 
  outTree->Branch("cemc_clE",&cemc_clE,"cemc_clE[track_n]/F"); 
  outTree->Branch("ihcal_clE",&ihcal_clE,"ihcal_clE[track_n]/F"); 
  outTree->Branch("ohcal_clE",&ohcal_clE,"ohcal_clE[track_n]/F"); 
  
  //loop over files
  for(int j=initfile; j<=endfile; j++){
    
    TString fname = Form("./out_%s/ParticleGun_G4sPHENIX_%s_%d.root",inType.c_str(),inType.c_str(),j);
    if (j%100==0) cout << fname << endl; 

    //// skip bad files
    TFile* fin = new TFile(fname,"READ"); 
    if(fin->GetSize()<10000){fin->Close(); bad_files++; continue;}
    if(fin->IsZombie()) {fin->Close(); bad_files++; continue;}
    
    //// read original tree 
    TTree* inTree = (TTree*)fin->Get("track_tree");
    inTree->SetName("inTree");
    inTree->SetBranchAddress("event",&old_event); 
    inTree->SetBranchAddress("track_n",&old_track_n); 
    inTree->SetBranchAddress("track_p",&old_track_p); 
    inTree->SetBranchAddress("cemc_E3x3",&old_cemc_E3x3); 
    inTree->SetBranchAddress("ihcal_E3x3",&old_ihcal_E3x3); 
    inTree->SetBranchAddress("ohcal_E3x3",&old_ohcal_E3x3); 
    inTree->SetBranchAddress("cemc_E5x5",&old_cemc_E5x5); 
    inTree->SetBranchAddress("ihcal_E5x5",&old_ihcal_E5x5); 
    inTree->SetBranchAddress("ohcal_E5x5",&old_ohcal_E5x5); 
    inTree->SetBranchAddress("cemc_clE",&old_cemc_clE); 
    inTree->SetBranchAddress("ihcal_clE",&old_ihcal_clE); 
    inTree->SetBranchAddress("ohcal_clE",&old_ohcal_clE); 

    // loop over event (nsim)
    int nsim = inTree->GetEntries();
    //cout << j <<"th nsim :" <<nsim<<endl;
    for(int isim=0; isim<nsim; isim++){
      inTree->GetEvent(isim);
      if (event%1000==0) { cout << event<<"th events" << endl; }
      track_n = old_track_n;
      for(int itrk=0; itrk<track_n; itrk++){
        //cout << "itrk: " << itrk<<endl;    
        track_p[itrk] = old_track_p[itrk];
        cemc_E3x3[itrk] = old_cemc_E3x3[itrk];
        ihcal_E3x3[itrk] = old_ihcal_E3x3[itrk];
        ohcal_E3x3[itrk] = old_ohcal_E3x3[itrk];
        cemc_E5x5[itrk] = old_cemc_E5x5[itrk];
        ihcal_E5x5[itrk] = old_ihcal_E5x5[itrk];
        ohcal_E5x5[itrk] = old_ohcal_E5x5[itrk];
        cemc_clE[itrk] = old_cemc_clE[itrk];
        ihcal_clE[itrk] = old_ihcal_clE[itrk];
        ohcal_clE[itrk] = old_ohcal_clE[itrk];
      } //end of ntrk loop
      event++;
      outTree->Fill();
    }  //end of nsim loop
  } //end of file loop
    
  outFile->cd();
  outTree->Write();
  outFile->Close();

  return; 
}
