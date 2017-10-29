
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

void rootAna_blackhole(string inType = "BH_SS310_PiPl_25GeV_v2",int initfile=0,int endfile =47)
{
  bool do_E3x3 = true;
  bool do_E5x5 = true;
  bool do_clE = true;
  
  float true_p = 25;
  
  //// Suppress error messages from bad files
  gErrorIgnoreLevel = kFatal; 

  int bad_files = 0; 

  TH2D* h2d = new TH2D("h2d","h2d",60,0,30,60,0,30);
  TH1D* h1d = new TH1D("h1d","h1d",60,0,30);
  
  //// loop over files 
  for(int j=initfile; j<=endfile; j++){
    
    TString fname = Form("./out_%s/DVP_QCD35_10k_G4sPHENIX_%s_%d.root",inType.c_str(),inType.c_str(),j);
    if (j%100==0) cout << fname << endl; 

    TFile* fin = new TFile(fname);
    if(fin->GetSize()<10000){fin->Close(); bad_files++; continue;}
    if(fin->IsZombie()) {fin->Close(); bad_files++; continue;}
    //cout << "fin: "<<fin<<endl;
 
    TTree* track_tree = (TTree*)fin->Get("track_tree");
    track_tree->AddFriend("bhhit_tree",fin);
    //track_tree->Draw("ohcal_E3x3:cemc_E3x3+ihcal_E3x3>>h2d","","colz");
    //track_tree->Draw("bhhit_tree.edepTot>>h1d","","");
  
    int track_evt;  
    int track_n;  
    track_tree->SetBranchAddress("event",&track_evt); 
    track_tree->SetBranchAddress("track_n",&track_n); 
    float cemc_E3x3[999], ihcal_E3x3[999], ohcal_E3x3[999];
    track_tree->SetBranchAddress("cemc_E3x3",&cemc_E3x3); 
    track_tree->SetBranchAddress("ihcal_E3x3",&ihcal_E3x3); 
    track_tree->SetBranchAddress("ohcal_E3x3",&ohcal_E3x3); 
    
    int bhhit_evt;  
    float bhhit_edepTot;  
    track_tree->SetBranchAddress("bhhit_tree.event",&bhhit_evt); 
    track_tree->SetBranchAddress("bhhit_tree.bhhit_edepTot",&bhhit_edepTot); 
    
    int nsim = track_tree->GetEntries();
    for(int isim=0; isim<nsim; isim++){
      track_tree->GetEvent(isim);
      for(int itrk=0; itrk<track_n; itrk++){
        if (cemc_E3x3[itrk]>0 && ihcal_E3x3[itrk] >0 && ohcal_E3x3[itrk] > 0 &&
        cemc_E3x3[itrk]+ihcal_E3x3[itrk]<1 && ohcal_E3x3[itrk] < 2) {
          h2d->Fill(cemc_E3x3[itrk]+ihcal_E3x3[itrk],ohcal_E3x3[itrk]);
          h1d->Fill(bhhit_edepTot);
        }
      }
    }
  }
  cout << "Number of bad files = " << bad_files << endl; 

  //TCanvas* c1 = new TCanvas("c1","c1",500,500);
  //cout << h2d->GetEntries() << endl;
  //h2d->Draw("colz");
  //c1->SaveAs("out_scale/blackhole_2D.pdf");
  TCanvas* c2 = new TCanvas("c2","c2",500,500);
  h1d->GetXaxis()->SetTitle("Black hole hits #Sigma E_{dep}");
  h1d->GetXaxis()->CenterTitle(1);
  h1d->Draw();
  c2->SaveAs("out_scale/blackhole_1D.pdf");

  return; 
}
