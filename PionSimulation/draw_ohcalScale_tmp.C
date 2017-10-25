
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

void draw_ohcalScale_tmp(TString inType = "SS310_PiPl_25GeV")
{
  bool do_E3x3 = true;
  bool do_E5x5 = true;
  bool do_clE = true;
  
  float fitmin = 0.4;
  float fitmax = 0.9;


  TFile* fin = new TFile(Form("out_ohcal_scale/ohcal_scale_%s.root",inType.Data()),"READ");
  cout << "fin: " << fin << endl;
  //TH1D* h1D_01 = (TH1D*)fin->Get("afterCut/ohcal_frac_E3x3_cut");
  TH1D* h1D_01 = (TH1D*)fin->Get("afterCut/outerTwo_frac_E3x3_cut");
  cout << "h1D_01: " << h1D_01 << endl;
  h1D_01->Rebin(4);
  h1D_01->GetXaxis()->SetRangeUser(0,1.6);
 
  TF1* gaus01 = new TF1("gaus01","gaus",fitmin,fitmax);
  //gaus01->SetLineColor(kGray+2);
  gaus01->SetLineColor(kRed+1);
  gaus01->SetLineStyle(1);
  
  TCanvas*c1 = new TCanvas("c1","",500,500);
  SetHistStyle(h1D_01,0,0); 
  h1D_01->Draw("pe");
  h1D_01->Fit("gaus01","N");
  gaus01->Draw("same");
  
  /*
  ////////////////////////////////////////////////////////
  //TH1D* h1D_g4particle_pt = new TH1D("g4particle_pt",";g4particle_pt;",50,0,50);
  //TH1D* h1D_g4particle_eta = new TH1D("g4particle_eta",";g4particle_eta;",100,-1.5,1.5);
  ////////////////////////////////////////////////////////
  TH1D* h1D_cemc_E3x3 = new TH1D("cemc_E3x3",";cemc_E3x3;",600,0,30);
  TH1D* h1D_ihcal_E3x3 = new TH1D("ihcal_E3x3",";ihcal_E3x3;",600,0,30);
  TH1D* h1D_ohcal_E3x3 = new TH1D("ohcal_E3x3",";ohcal_E3x3;",600,0,30);
  TH1D* h1D_cemc_E5x5 = new TH1D("cemc_E5x5",";cemc_E5x5;",600,0,30);
  TH1D* h1D_ihcal_E5x5 = new TH1D("ihcal_E5x5",";ihcal_E5x5;",600,0,30);
  TH1D* h1D_ohcal_E5x5 = new TH1D("ohcal_E5x5",";ohcal_E5x5;",600,0,30);
  TH1D* h1D_cemc_clE = new TH1D("cemc_clE",";cemc_clE;",600,0,30);
  TH1D* h1D_ihcal_clE = new TH1D("ihcal_clE",";ihcal_clE;",600,0,30);
  TH1D* h1D_ohcal_clE = new TH1D("ohcal_clE",";ohcal_clE;",600,0,30);
  ////
  TH1D* h1D_cemc_E3x3_cut = new TH1D("cemc_E3x3_cut",";cemc_E3x3;",600,0,30);
  TH1D* h1D_ihcal_E3x3_cut = new TH1D("ihcal_E3x3_cut",";ihcal_E3x3;",600,0,30);
  TH1D* h1D_ohcal_E3x3_cut = new TH1D("ohcal_E3x3_cut",";ohcal_E3x3;",600,0,30);
  TH1D* h1D_ohcal_incident_E3x3_cut = new TH1D("ohcal_incident_E3x3_cut",";Truth E - cemc_E - ihcal_E;",600,20,30);
  TH1D* h1D_ohcal_frac_E3x3_cut = new TH1D("ohcal_frac_E3x3_cut",";ohcal_E / (Truth E - cemc_E - ihcal_E) ;",600,0,2.);
  TH1D* h1D_outerTwo_incident_E3x3_cut = new TH1D("outerTwo_incident_E3x3_cut",";Truth E - cemc_E;",600,20,30);
  TH1D* h1D_outerTwo_frac_E3x3_cut = new TH1D("outerTwo_frac_E3x3_cut",";(ihcal_E + ohcal_E) / (Truth E - cemc_E);",600,0,2.);
  ////
  TH1D* h1D_cemc_E5x5_cut = new TH1D("cemc_E5x5_cut",";cemc_E5x5;",600,0,30);
  TH1D* h1D_ihcal_E5x5_cut = new TH1D("ihcal_E5x5_cut",";ihcal_E5x5;",600,0,30);
  TH1D* h1D_ohcal_E5x5_cut = new TH1D("ohcal_E5x5_cut",";ohcal_E5x5;",600,0,30);
  TH1D* h1D_ohcal_incident_E5x5_cut = new TH1D("ohcal_incident_E5x5_cut",";Truth E - cemc_E - ihcal_E;",600,20,30);
  TH1D* h1D_ohcal_frac_E5x5_cut = new TH1D("ohcal_frac_E5x5_cut",";ohcal_E / (Truth E - cemc_E - ihcal_E) ;",600,0,2.);
  TH1D* h1D_outerTwo_incident_E5x5_cut = new TH1D("outerTwo_incident_E5x5_cut",";Truth E - cemc_E;",600,20,30);
  TH1D* h1D_outerTwo_frac_E5x5_cut = new TH1D("outerTwo_frac_E5x5_cut",";(ihcal_E + ohcal_E) / (Truth E - cemc_E);",600,0,2.);
  ////
  TH1D* h1D_cemc_clE_cut = new TH1D("cemc_clE_cut",";cemc_clE;",600,0,30);
  TH1D* h1D_ihcal_clE_cut = new TH1D("ihcal_clE_cut",";ihcal_clE;",600,0,30);
  TH1D* h1D_ohcal_clE_cut = new TH1D("ohcal_clE_cut",";ohcal_clE;",600,0,30);
  TH1D* h1D_ohcal_incident_clE_cut = new TH1D("ohcal_incident_clE_cut",";Truth E - cemc_E - ihcal_E;",600,20,30);
  TH1D* h1D_ohcal_frac_clE_cut = new TH1D("ohcal_frac_clE_cut",";ohcal_E / (Truth E - cemc_E - ihcal_E) ;",600,0,2.);
  TH1D* h1D_outerTwo_incident_clE_cut = new TH1D("outerTwo_incident_clE_cut",";Truth E - cemc_E;",600,20,30);
  TH1D* h1D_outerTwo_frac_clE_cut = new TH1D("outerTwo_frac_clE_cut",";(ihcal_E + ohcal_E) / (Truth E - cemc_E);",600,0,2.);
  ////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////
  TH2D* h2D_cemc_vs_ihcal_E3x3 = new TH2D("h2D_cemc_vs_ihcal_E3x3",";cemc E3x3;ihcal E3x3",600, 0., 30., 600, 0, 30.);
  TH2D* h2D_cemc_vs_ohcal_E3x3 = new TH2D("h2D_cemc_vs_ohcal_E3x3",";cemc E3x3;ohcal E3x3",600, 0., 30., 600, 0, 30.);
  TH2D* h2D_ihcal_vs_ohcal_E3x3 = new TH2D("h2D_ihcal_vs_ohcal_E3x3",";ihcal E3x3;ohcal E3x3",600, 0., 30., 600, 0, 30.);
  TH2D* h2D_innerTwo_vs_ohcal_E3x3 = new TH2D("h2D_innerTwo_vs_ohcal_E3x3",";cemc E3x3 + ihcal E3x3;ohcal E3x3",600, 0., 30., 600, 0, 30.);
  TH2D* h2D_cemc_vs_outerTwo_E3x3 = new TH2D("h2D_cemc_vs_outerTwo_E3x3",";cemc E3x3 + ihcal E3x3;ohcal E3x3",600, 0., 30., 600, 0, 30.);
  ////
  TH2D* h2D_cemc_vs_ihcal_E5x5 = new TH2D("h2D_cemc_vs_ihcal_E5x5",";cemc E5x5;ihcal E5x5",600, 0., 30., 600, 0, 30.);
  TH2D* h2D_cemc_vs_ohcal_E5x5 = new TH2D("h2D_cemc_vs_ohcal_E5x5",";cemc E5x5;ohcal E5x5",600, 0., 30., 600, 0, 30.);
  TH2D* h2D_ihcal_vs_ohcal_E5x5 = new TH2D("h2D_ihcal_vs_ohcal_E5x5",";ihcal E5x5;ohcal E5x5",600, 0., 30., 600, 0, 30.);
  TH2D* h2D_innerTwo_vs_ohcal_E5x5 = new TH2D("h2D_innerTwo_vs_ohcal_E5x5",";cemc E5x5 + ihcal E5x5;ohcal E5x5",600, 0., 30., 600, 0, 30.);
  TH2D* h2D_cemc_vs_outerTwo_E5x5 = new TH2D("h2D_cemc_vs_outerTwo_E5x5",";cemc E5x5 + ihcal E5x5;ohcal E5x5",600, 0., 30., 600, 0, 30.);
  ////
  TH2D* h2D_cemc_vs_ihcal_clE = new TH2D("h2D_cemc_vs_ihcal_clE",";cemc clE;ihcal clE",600, 0., 30., 600, 0, 30.);
  TH2D* h2D_cemc_vs_ohcal_clE = new TH2D("h2D_cemc_vs_ohcal_clE",";cemc clE;ohcal clE",600, 0., 30., 600, 0, 30.);
  TH2D* h2D_ihcal_vs_ohcal_clE = new TH2D("h2D_ihcal_vs_ohcal_clE",";ihcal clE;ohcal clE",600, 0., 30., 600, 0, 30.);
  TH2D* h2D_innerTwo_vs_ohcal_clE = new TH2D("h2D_innerTwo_vs_ohcal_clE",";cemc clE + ihcal clE;ohcal clE",600, 0., 30., 600, 0, 30.);
  TH2D* h2D_cemc_vs_outerTwo_clE = new TH2D("h2D_cemc_vs_outerTwo_clE",";cemc clE + ihcal clE;ohcal clE",600, 0., 30., 600, 0, 30.);
  ////////////////////////////////////////////////////////
  TH2D* h2D_cemc_vs_ihcal_E3x3_cut = new TH2D("h2D_cemc_vs_ihcal_E3x3_cut",";cemc E3x3;ihcal E3x3",600, 0., 30., 600, 0, 30.);
  TH2D* h2D_cemc_vs_ohcal_E3x3_cut = new TH2D("h2D_cemc_vs_ohcal_E3x3_cut",";cemc E3x3;ohcal E3x3",600, 0., 30., 600, 0, 30.);
  TH2D* h2D_ihcal_vs_ohcal_E3x3_cut = new TH2D("h2D_ihcal_vs_ohcal_E3x3_cut",";ihcal E3x3;ohcal E3x3",600, 0., 30., 600, 0, 30.);
  TH2D* h2D_innerTwo_vs_ohcal_E3x3_cut = new TH2D("h2D_innerTwo_vs_ohcal_E3x3_cut",";cemc E3x3 + ihcal E3x3;ohcal E3x3",600, 0., 30., 600, 0, 30.);
  TH2D* h2D_cemc_vs_outerTwo_E3x3_cut = new TH2D("h2D_cemc_vs_outerTwo_E3x3_cut",";cemc E3x3 + ihcal E3x3;ohcal E3x3",600, 0., 30., 600, 0, 30.);
  ////
  TH2D* h2D_cemc_vs_ihcal_E5x5_cut = new TH2D("h2D_cemc_vs_ihcal_E5x5_cut",";cemc E5x5;ihcal E5x5",600, 0., 30., 600, 0, 30.);
  TH2D* h2D_cemc_vs_ohcal_E5x5_cut = new TH2D("h2D_cemc_vs_ohcal_E5x5_cut",";cemc E5x5;ohcal E5x5",600, 0., 30., 600, 0, 30.);
  TH2D* h2D_ihcal_vs_ohcal_E5x5_cut = new TH2D("h2D_ihcal_vs_ohcal_E5x5_cut",";ihcal E5x5;ohcal E5x5",600, 0., 30., 600, 0, 30.);
  TH2D* h2D_innerTwo_vs_ohcal_E5x5_cut = new TH2D("h2D_innerTwo_vs_ohcal_E5x5_cut",";cemc E5x5 + ihcal E5x5;ohcal E5x5",600, 0., 30., 600, 0, 30.);
  TH2D* h2D_cemc_vs_outerTwo_E5x5_cut = new TH2D("h2D_cemc_vs_outerTwo_E5x5_cut",";cemc E5x5 + ihcal E5x5;ohcal E5x5",600, 0., 30., 600, 0, 30.);
  ////
  TH2D* h2D_cemc_vs_ihcal_clE_cut = new TH2D("h2D_cemc_vs_ihcal_clE_cut",";cemc clE;ihcal clE",600, 0., 30., 600, 0, 30.);
  TH2D* h2D_cemc_vs_ohcal_clE_cut = new TH2D("h2D_cemc_vs_ohcal_clE_cut",";cemc clE;ohcal clE",600, 0., 30., 600, 0, 30.);
  TH2D* h2D_ihcal_vs_ohcal_clE_cut = new TH2D("h2D_ihcal_vs_ohcal_clE_cut",";ihcal clE;ohcal clE",600, 0., 30., 600, 0, 30.);
  TH2D* h2D_innerTwo_vs_ohcal_clE_cut = new TH2D("h2D_innerTwo_vs_ohcal_clE_cut",";cemc clE + ihcal clE;ohcal clE",600, 0., 30., 600, 0, 30.);
  TH2D* h2D_cemc_vs_outerTwo_clE_cut = new TH2D("h2D_cemc_vs_outerTwo_clE_cut",";cemc clE + ihcal clE;ohcal clE",600, 0., 30., 600, 0, 30.);
  ////////////////////////////////////////////////////////
  
  int track_n;  
  track_trees->SetBranchAddress("track_n",&track_n); 
  float cemc_E3x3[999], ihcal_E3x3[999], ohcal_E3x3[999];
  track_trees->SetBranchAddress("cemc_E3x3",&cemc_E3x3); 
  track_trees->SetBranchAddress("ihcal_E3x3",&ihcal_E3x3); 
  track_trees->SetBranchAddress("ohcal_E3x3",&ohcal_E3x3); 
  float cemc_E5x5[999], ihcal_E5x5[999], ohcal_E5x5[999];
  track_trees->SetBranchAddress("cemc_E5x5",&cemc_E5x5); 
  track_trees->SetBranchAddress("ihcal_E5x5",&ihcal_E5x5); 
  track_trees->SetBranchAddress("ohcal_E5x5",&ohcal_E5x5); 
  float cemc_clE[999], ihcal_clE[999], ohcal_clE[999];
  track_trees->SetBranchAddress("cemc_clE",&cemc_clE); 
  track_trees->SetBranchAddress("ihcal_clE",&ihcal_clE); 
  track_trees->SetBranchAddress("ohcal_clE",&ohcal_clE); 

  int nsim = track_trees->GetEntries();
  float ohcal_incident =0;
  float ohcal_frac =0;
  float outerTwo_incident =0;
  float outerTwo_frac =0;

  for(int isim=0; isim<nsim; isim++){
    track_trees->GetEvent(isim);
    for(int itrk=0; itrk<track_n; itrk++){
      if (do_E3x3){    
        h1D_cemc_E3x3->Fill(cemc_E3x3[itrk]); 
        h1D_ihcal_E3x3->Fill(ihcal_E3x3[itrk]); 
        h1D_ohcal_E3x3->Fill(ohcal_E3x3[itrk]); 
        h2D_cemc_vs_ihcal_E3x3->Fill(cemc_E3x3[itrk],ihcal_E3x3[itrk]);
        h2D_cemc_vs_ohcal_E3x3->Fill(cemc_E3x3[itrk],ohcal_E3x3[itrk]);
        h2D_ihcal_vs_ohcal_E3x3->Fill(ihcal_E3x3[itrk],ohcal_E3x3[itrk]);
        h2D_innerTwo_vs_ohcal_E3x3->Fill(cemc_E3x3[itrk]+ihcal_E3x3[itrk], ohcal_E3x3[itrk]);
        h2D_cemc_vs_outerTwo_E3x3->Fill(cemc_E3x3[itrk], ihcal_E3x3[itrk]+ohcal_E3x3[itrk]);
      }
      if (do_E5x5){    
        h1D_cemc_E5x5->Fill(cemc_E5x5[itrk]); 
        h1D_ihcal_E5x5->Fill(ihcal_E5x5[itrk]); 
        h1D_ohcal_E5x5->Fill(ohcal_E5x5[itrk]); 
        h2D_cemc_vs_ihcal_E5x5->Fill(cemc_E5x5[itrk],ihcal_E5x5[itrk]);
        h2D_cemc_vs_ohcal_E5x5->Fill(cemc_E5x5[itrk],ohcal_E5x5[itrk]);
        h2D_ihcal_vs_ohcal_E5x5->Fill(ihcal_E5x5[itrk],ohcal_E5x5[itrk]);
        h2D_innerTwo_vs_ohcal_E5x5->Fill(cemc_E5x5[itrk]+ihcal_E5x5[itrk], ohcal_E5x5[itrk]);
        h2D_cemc_vs_outerTwo_E5x5->Fill(cemc_E5x5[itrk], ihcal_E5x5[itrk]+ohcal_E5x5[itrk]);
      }
      if (do_clE){    
        h1D_cemc_clE->Fill(cemc_clE[itrk]); 
        h1D_ihcal_clE->Fill(ihcal_clE[itrk]); 
        h1D_ohcal_clE->Fill(ohcal_clE[itrk]); 
        h2D_cemc_vs_ihcal_clE->Fill(cemc_clE[itrk],ihcal_clE[itrk]);
        h2D_cemc_vs_ohcal_clE->Fill(cemc_clE[itrk],ohcal_clE[itrk]);
        h2D_ihcal_vs_ohcal_clE->Fill(ihcal_clE[itrk],ohcal_clE[itrk]);
        h2D_innerTwo_vs_ohcal_clE->Fill(cemc_clE[itrk]+ihcal_clE[itrk], ohcal_clE[itrk]);
        h2D_cemc_vs_outerTwo_clE->Fill(cemc_clE[itrk], ihcal_clE[itrk]+ohcal_clE[itrk]);
      }

      if (cemc_E3x3[itrk] < cemc_cut && ihcal_E3x3[itrk]< ihcal_cut){
        if (do_E3x3){    
          h1D_cemc_E3x3_cut->Fill(cemc_E3x3[itrk]); 
          h1D_ihcal_E3x3_cut->Fill(ihcal_E3x3[itrk]); 
          h1D_ohcal_E3x3_cut->Fill(ohcal_E3x3[itrk]); 
          ohcal_incident = true_p - (cemc_E3x3[itrk]+ihcal_E3x3[itrk]);
          ohcal_frac = ohcal_E3x3[itrk]/ohcal_incident;
          outerTwo_incident = true_p - cemc_E3x3[itrk];
          outerTwo_frac = (ihcal_E3x3[itrk]+ohcal_E3x3[itrk])/outerTwo_incident;
          h1D_ohcal_incident_E3x3_cut->Fill(ohcal_incident); 
          h1D_ohcal_frac_E3x3_cut->Fill(ohcal_frac); 
          h1D_outerTwo_incident_E3x3_cut->Fill(outerTwo_incident); 
          h1D_outerTwo_frac_E3x3_cut->Fill(outerTwo_frac); 
          h2D_cemc_vs_ihcal_E3x3_cut->Fill(cemc_E3x3[itrk],ihcal_E3x3[itrk]);
          h2D_cemc_vs_ohcal_E3x3_cut->Fill(cemc_E3x3[itrk],ohcal_E3x3[itrk]);
          h2D_ihcal_vs_ohcal_E3x3_cut->Fill(ihcal_E3x3[itrk],ohcal_E3x3[itrk]);
          h2D_innerTwo_vs_ohcal_E3x3_cut->Fill(cemc_E3x3[itrk]+ihcal_E3x3[itrk], ohcal_E3x3[itrk]);
          h2D_cemc_vs_outerTwo_E3x3_cut->Fill(cemc_E3x3[itrk], ihcal_E3x3[itrk]+ohcal_E3x3[itrk]);
        }
        if (do_E5x5){    
          h1D_cemc_E5x5_cut->Fill(cemc_E5x5[itrk]); 
          h1D_ihcal_E5x5_cut->Fill(ihcal_E5x5[itrk]); 
          h1D_ohcal_E5x5_cut->Fill(ohcal_E5x5[itrk]); 
          ohcal_incident = true_p - (cemc_E5x5[itrk]+ihcal_E5x5[itrk]);
          ohcal_frac = ohcal_E5x5[itrk]/ohcal_incident;
          outerTwo_incident = true_p - cemc_E5x5[itrk];
          outerTwo_frac = (ihcal_E5x5[itrk]+ohcal_E5x5[itrk])/outerTwo_incident;
          h1D_ohcal_incident_E5x5_cut->Fill(ohcal_incident); 
          h1D_ohcal_frac_E5x5_cut->Fill(ohcal_frac); 
          h1D_outerTwo_incident_E5x5_cut->Fill(outerTwo_incident); 
          h1D_outerTwo_frac_E5x5_cut->Fill(outerTwo_frac); 
          h2D_cemc_vs_ihcal_E5x5_cut->Fill(cemc_E5x5[itrk],ihcal_E5x5[itrk]);
          h2D_cemc_vs_ohcal_E5x5_cut->Fill(cemc_E5x5[itrk],ohcal_E5x5[itrk]);
          h2D_ihcal_vs_ohcal_E5x5_cut->Fill(ihcal_E5x5[itrk],ohcal_E5x5[itrk]);
          h2D_innerTwo_vs_ohcal_E5x5_cut->Fill(cemc_E5x5[itrk]+ihcal_E5x5[itrk], ohcal_E5x5[itrk]);
          h2D_cemc_vs_outerTwo_E5x5_cut->Fill(cemc_E5x5[itrk], ihcal_E5x5[itrk]+ohcal_E5x5[itrk]);
        }
        if (do_clE){    
          h1D_cemc_clE_cut->Fill(cemc_clE[itrk]); 
          h1D_ihcal_clE_cut->Fill(ihcal_clE[itrk]); 
          h1D_ohcal_clE_cut->Fill(ohcal_clE[itrk]); 
          ohcal_incident = true_p - (cemc_clE[itrk]+ihcal_clE[itrk]);
          ohcal_frac = ohcal_clE[itrk]/ohcal_incident;
          outerTwo_incident = true_p - cemc_clE[itrk];
          outerTwo_frac = (ihcal_clE[itrk]+ohcal_clE[itrk])/outerTwo_incident;
          h1D_ohcal_incident_clE_cut->Fill(ohcal_incident); 
          h1D_ohcal_frac_clE_cut->Fill(ohcal_frac); 
          h1D_outerTwo_incident_clE_cut->Fill(outerTwo_incident); 
          h1D_outerTwo_frac_clE_cut->Fill(outerTwo_frac); 
          h2D_cemc_vs_ihcal_clE_cut->Fill(cemc_clE[itrk],ihcal_clE[itrk]);
          h2D_cemc_vs_ohcal_clE_cut->Fill(cemc_clE[itrk],ohcal_clE[itrk]);
          h2D_ihcal_vs_ohcal_clE_cut->Fill(ihcal_clE[itrk],ohcal_clE[itrk]);
          h2D_innerTwo_vs_ohcal_clE_cut->Fill(cemc_clE[itrk]+ihcal_clE[itrk], ohcal_clE[itrk]);
          h2D_cemc_vs_outerTwo_clE_cut->Fill(cemc_clE[itrk], ihcal_clE[itrk]+ohcal_clE[itrk]);
        }
      } //end of cut
    } //end of ntrk loop
  }  //end of nsim loop
      
  h2D_cemc_vs_ihcal_E3x3->SetOption("colz");
  h2D_cemc_vs_ohcal_E3x3->SetOption("colz");
  h2D_ihcal_vs_ohcal_E3x3->SetOption("colz");
  h2D_innerTwo_vs_ohcal_E3x3->SetOption("colz");
  h2D_cemc_vs_outerTwo_E3x3->SetOption("colz");
  h2D_cemc_vs_ihcal_E5x5->SetOption("colz");
  h2D_cemc_vs_ohcal_E5x5->SetOption("colz");
  h2D_ihcal_vs_ohcal_E5x5->SetOption("colz");
  h2D_innerTwo_vs_ohcal_E5x5->SetOption("colz");
  h2D_cemc_vs_outerTwo_E5x5->SetOption("colz");
  h2D_cemc_vs_ihcal_clE->SetOption("colz");
  h2D_cemc_vs_ohcal_clE->SetOption("colz");
  h2D_ihcal_vs_ohcal_clE->SetOption("colz");
  h2D_innerTwo_vs_ohcal_clE->SetOption("colz");
  h2D_cemc_vs_outerTwo_clE->SetOption("colz");
  ////
  h2D_cemc_vs_ihcal_E3x3_cut->SetOption("colz");
  h2D_cemc_vs_ohcal_E3x3_cut->SetOption("colz");
  h2D_ihcal_vs_ohcal_E3x3_cut->SetOption("colz");
  h2D_innerTwo_vs_ohcal_E3x3_cut->SetOption("colz");
  h2D_cemc_vs_outerTwo_E3x3_cut->SetOption("colz");
  h2D_cemc_vs_ihcal_E5x5_cut->SetOption("colz");
  h2D_cemc_vs_ohcal_E5x5_cut->SetOption("colz");
  h2D_ihcal_vs_ohcal_E5x5_cut->SetOption("colz");
  h2D_innerTwo_vs_ohcal_E5x5_cut->SetOption("colz");
  h2D_cemc_vs_outerTwo_E5x5_cut->SetOption("colz");
  h2D_cemc_vs_ihcal_clE_cut->SetOption("colz");
  h2D_cemc_vs_ohcal_clE_cut->SetOption("colz");
  h2D_ihcal_vs_ohcal_clE_cut->SetOption("colz");
  h2D_innerTwo_vs_ohcal_clE_cut->SetOption("colz");
  h2D_cemc_vs_outerTwo_clE_cut->SetOption("colz");
  
  //TCanvas* c1 = new TCanvas("c1","",600,600);
  //h2D_JES_ohcal_scale->Draw();

  TFile* fout = new TFile(Form("out_ohcal_scale/ohcal_scale_%s.root",inType.Data()),"RECREATE");
  fout->cd();
  
  TDirectory* beforeCut = fout->mkdir("beforeCut");
  beforeCut->cd();
  if (do_E3x3){    
    h1D_cemc_E3x3->Write();
    h1D_ihcal_E3x3->Write();
    h1D_ohcal_E3x3->Write();
    h2D_cemc_vs_ihcal_E3x3->Write();
    h2D_cemc_vs_ohcal_E3x3->Write();
    h2D_ihcal_vs_ohcal_E3x3->Write();
    h2D_innerTwo_vs_ohcal_E3x3->Write();
    h2D_cemc_vs_outerTwo_E3x3->Write();
  }
  if (do_E5x5){    
    h1D_cemc_E5x5->Write();
    h1D_ihcal_E5x5->Write();
    h1D_ohcal_E5x5->Write();
    h2D_cemc_vs_ihcal_E5x5->Write();
    h2D_cemc_vs_ohcal_E5x5->Write();
    h2D_ihcal_vs_ohcal_E5x5->Write();
    h2D_innerTwo_vs_ohcal_E5x5->Write();
    h2D_cemc_vs_outerTwo_E5x5->Write();
  }
  if (do_clE){    
    h1D_cemc_clE->Write();
    h1D_ihcal_clE->Write();
    h1D_ohcal_clE->Write();
    h2D_cemc_vs_ihcal_clE->Write();
    h2D_cemc_vs_ohcal_clE->Write();
    h2D_ihcal_vs_ohcal_clE->Write();
    h2D_innerTwo_vs_ohcal_clE->Write();
    h2D_cemc_vs_outerTwo_clE->Write();
  }
  
  TDirectory* afterCut = fout->mkdir("afterCut");
  afterCut->cd();
  if (do_E3x3){    
    h1D_cemc_E3x3_cut->Write();
    h1D_ihcal_E3x3_cut->Write();
    h1D_ohcal_E3x3_cut->Write();
    h1D_ohcal_incident_E3x3_cut->Write();
    h1D_ohcal_frac_E3x3_cut->Write();
    h1D_outerTwo_incident_E3x3_cut->Write();
    h1D_outerTwo_frac_E3x3_cut->Write();
    h2D_cemc_vs_ihcal_E3x3_cut->Write();
    h2D_cemc_vs_ohcal_E3x3_cut->Write();
    h2D_ihcal_vs_ohcal_E3x3_cut->Write();
    h2D_innerTwo_vs_ohcal_E3x3_cut->Write();
    h2D_cemc_vs_outerTwo_E3x3_cut->Write();
  }
  if (do_E5x5){    
    h1D_cemc_E5x5_cut->Write();
    h1D_ihcal_E5x5_cut->Write();
    h1D_ohcal_E5x5_cut->Write();
    h1D_ohcal_incident_E5x5_cut->Write();
    h1D_ohcal_frac_E5x5_cut->Write();
    h1D_outerTwo_incident_E5x5_cut->Write();
    h1D_outerTwo_frac_E5x5_cut->Write();
    h2D_cemc_vs_ihcal_E5x5_cut->Write();
    h2D_cemc_vs_ohcal_E5x5_cut->Write();
    h2D_ihcal_vs_ohcal_E5x5_cut->Write();
    h2D_innerTwo_vs_ohcal_E5x5_cut->Write();
    h2D_cemc_vs_outerTwo_E5x5_cut->Write();
  }
  if (do_clE){    
    h1D_cemc_clE_cut->Write();
    h1D_ihcal_clE_cut->Write();
    h1D_ohcal_clE_cut->Write();
    h1D_ohcal_incident_clE_cut->Write();
    h1D_ohcal_frac_clE_cut->Write();
    h1D_outerTwo_incident_clE_cut->Write();
    h1D_outerTwo_frac_clE_cut->Write();
    h2D_cemc_vs_ihcal_clE_cut->Write();
    h2D_cemc_vs_ohcal_clE_cut->Write();
    h2D_ihcal_vs_ohcal_clE_cut->Write();
    h2D_innerTwo_vs_ohcal_clE_cut->Write();
    h2D_cemc_vs_outerTwo_clE_cut->Write();
  } 
  
  fout->Close();
*/  
  return; 
}
