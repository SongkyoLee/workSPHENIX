
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

void rootAna_matching_Al_uninst_jet4(string ihcalType = "Al_uninst",
                              string jetE = "50GeV",
                              string outDir = "outJet4Matched",
                              int initfile=0,
                              int endfile =1000,
                              int ver =4)
{
  //// E = c_tot ( c_em * E_cemc + c_h ( c_ih * C_ihcal + C_ohcal) )
  //float cemc_sf =c_tot*c_em;
  //float ihcal_sf =c_tot*c_h*c_ih;
  //float ohcal_sf =c_tot*c_h;
  float cemc_sf =1;
  float ihcal_sf =1;
  float ohcal_sf =1;
  
  TFile* filefit = new TFile(Form("outJet4_2DEHAsymm_Al_uninst/%s_Al_uninst_totE_EHAsymm_scale_%.0f_%.0f_%.0f.root",jetE.c_str(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100),"READ");
  TF1 *f_EHAsymm = (TF1*)filefit->Get("pol4");
  filefit->Close();
  cout << f_EHAsymm<<endl;
  double refval = f_EHAsymm->Eval(1);
  cout << "refval :" << refval << endl;
  
  ////////////////////////////////////// 
  //// gamma histograms
  ////////////////////////////////////// 
  TH1D* h1D_true_gamma_e = new TH1D("h1D_true_gamma_e",";Truth #gamma E (GeV);",100,0,100);      
  TH1D* h1D_true_gamma_p = new TH1D("h1D_true_gamma_p",";Truth #gamma p (GeV);",100,0,100);      
  TH1D* h1D_true_gamma_pt = new TH1D("h1D_true_gamma_pt",";Truth #gamma p_{T} (GeV);",100,0,100);      
  TH1D* h1D_true_gamma_lcons_z = new TH1D("h1D_true_gamma_lcons_z",";Truth #gamma leading z;",101,-0.05,1.05);      
  TH1D* h1D_true_gamma_emfrac = new TH1D("h1D_true_gamma_emfrac",";Truth #gamma EM fraction;",101,-0.05,1.05);      
  ////////////////////////////////////// 
  TH1D* h1D_reco_gamma_e = new TH1D("h1D_reco_gamma_e",";Reco #gamma E (GeV);",100,0,100);
  TH1D* h1D_reco_gamma_p = new TH1D("h1D_reco_gamma_p",";Reco #gamma p (GeV);",100,0,100);
  TH1D* h1D_reco_gamma_pt = new TH1D("h1D_reco_gamma_pt",";Reco #gamma p_{T} (GeV);",100,0,100);      
  TH1D* h1D_reco_gamma_cemcEsum = new TH1D("h1D_reco_gamma_cemcEsum",";Reco #gamma E_{EMCAL} (GeV);",100,0,100);      
  TH1D* h1D_reco_gamma_ihcalEsum = new TH1D("h1D_reco_gamma_ihcalEsum",";Reco #gamma E_{IHCAL} (GeV);",100,0,100);      
  TH1D* h1D_reco_gamma_ohcalEsum = new TH1D("h1D_reco_gamma_ohcalEsum",";Reco #gamma E_{OHCAL} (GeV);",100,0,100);      
  TH1D* h1D_reco_gamma_outerTwo = new TH1D("h1D_reco_gamma_outerTwo",";Reco #gamma E_{IHCAL}+E_{OHCAL} (GeV);",100,0,100);      
  TH1D* h1D_reco_gamma_totE = new TH1D("h1D_reco_gamma_totE",";Reco #gamma E_{EMCAL}+E_{IHCAL}+E_{OHCAL} (GeV);",100,0,100);      
  TH1D* h1D_reco_gamma_outerTwoOverTruth = new TH1D("h1D_reco_gamma_outerTwoOverTruth",";Reco #gamma (E_{IHCAL}+E_{OHCAL}) / E_{Truth};",100,0,2);      
  TH1D* h1D_reco_gamma_outerTwoOverReco = new TH1D("h1D_reco_gamma_outerTwoOverReco",";Reco #gamma (E_{IHCAL}+E_{OHCAL}) / E_{Reco};",100,0,2);      
  TH1D* h1D_reco_gamma_IOAsymm = new TH1D("h1D_reco_gamma_IOAsymm",";Reco #gamma E_{IHCAL}-E_{OHCAL} asymmetry;",101,-1.05,1.05);      
  TH1D* h1D_reco_gamma_EHAsymm = new TH1D("h1D_reco_gamma_EHAsymm",";Reco #gamma E_{EMCAL}-E_{HCAL} asymmetry;",101,-1.05,1.05);      
  ////////////////////////////////////// 
  TH1D* h1D_gamma_JES = new TH1D("h1D_gamma_JES",";#gamma E_{Reco}/E_{Truth};",100,0,2);      
  TH1D* h1D_gamma_dR = new TH1D("h1D_gamma_dR",";#gamma #Delta R(Truth-Reco);",100,0,0.5);
  ////////////////////////////////////// 
  ////////////////////////////////////// 
  TH2D* h2D_gamma_outerTwo_IOAsymm = new TH2D("h2D_gamma_outerTwo_IOAsymm",";Reco #gamma E_{IHCAL}-E_{OHCAL} asymmetry;#gamma E_{IHCAL}+E_{OHCAL} (GeV)",101,-1.05,1.05,100,0,100); 
  TH2D* h2D_gamma_outerTwo_EHAsymm = new TH2D("h2D_gamma_outerTwo_EHAsymm",";Reco #gamma E_{EMCAL}-E_{HCAL} asymmetry;#gamma E_{IHCAL}+E_{OHCAL} (GeV)",101,-1.05,1.05,100,0,100); 
  TH2D* h2D_gamma_outerTwoOverTruth_IOAsymm = new TH2D("h2D_gamma_outerTwoOverTruth_IOAsymm",";Reco #gamma E_{IHCAL}-E_{OHCAL} asymmetry;#gamma (E_{IHCAL}+E_{OHCAL}) / E_{Truth}",101,-1.05,1.05,100,0,2); 
  TH2D* h2D_gamma_outerTwoOverTruth_EHAsymm = new TH2D("h2D_gamma_outerTwoOverTruth_EHAsymm",";Reco #gamma E_{EMCAL}-E_{HCAL} asymmetry;#gamma (E_{IHCAL}+E_{OHCAL}) / E_{Truth}",101,-1.05,1.05,100,0,2); 
  TH2D* h2D_gamma_outerTwoOverReco_IOAsymm = new TH2D("h2D_gamma_outerTwoOverReco_IOAsymm",";Reco #gamma E_{IHCAL}-E_{OHCAL} asymmetry;#gamma (E_{IHCAL}+E_{OHCAL}) / E_{Reco}",101,-1.05,1.05,100,0,2); 
  TH2D* h2D_gamma_outerTwoOverReco_EHAsymm = new TH2D("h2D_gamma_outerTwoOverReco_EHAsymm",";Reco #gamma E_{EMCAL}-E_{HCAL} asymmetry;#gamma (E_{IHCAL}+E_{OHCAL}) / E_{Reco}",101,-1.05,1.05,100,0,2); 
  TH2D* h2D_gamma_totE_IOAsymm = new TH2D("h2D_gamma_totE_IOAsymm",";Reco #gamma E_{IHCAL}-E_{OHCAL} asymmetry;#gamma E_{Reco}",101,-1.05,1.05,100,0,100); 
  TH2D* h2D_gamma_totE_EHAsymm = new TH2D("h2D_gamma_totE_EHAsymm",";Reco #gamma E_{EMCAL}-E_{HCAL} asymmetry;#gamma E_{Reco}",101,-1.05,1.05,100,0,100); 
  TH2D* h2D_gamma_JES_IOAsymm = new TH2D("h2D_gamma_JES_IOAsymm",";Reco #gamma E_{IHCAL}-E_{OHCAL} asymmetry;#gamma E_{Reco}/E_{Truth}",101,-1.05,1.05,100,0,2); 
  TH2D* h2D_gamma_JES_EHAsymm = new TH2D("h2D_gamma_JES_EHAsymm",";Reco #gamma E_{EMCAL}-E_{HCAL} asymmetry;#gamma E_{Reco}/E_{Truth}",101,-1.05,1.05,100,0,2); 
  TH2D* h2D_gamma_JES_emfrac = new TH2D("h2D_gamma_JES_emfrac",";Truth #gamma EM fraction;#gamma E_{Reco}/E_{Truth}",101,-0.05,1.05,100,0,2); 

  ////////////////////////////////////// 
  //// jet histograms
  ////////////////////////////////////// 
  TH1D* h1D_true_jet_e = new TH1D("h1D_true_jet_e",";Truth Jet E (GeV);",100,0,100);      
  TH1D* h1D_true_jet_p = new TH1D("h1D_true_jet_p",";Truth Jet p (GeV);",100,0,100);      
  TH1D* h1D_true_jet_pt = new TH1D("h1D_true_jet_pt",";Truth Jet p_{T} (GeV);",100,0,100);      
  TH1D* h1D_true_jet_lcons_z = new TH1D("h1D_true_jet_lcons_z",";Truth Jet leading z;",101,-0.05,1.05);      
  TH1D* h1D_true_jet_emfrac = new TH1D("h1D_true_jet_emfrac",";Truth Jet EM fraction;",101,-0.05,1.05);      
  ////////////////////////////////////// 
  TH1D* h1D_reco_jet_e = new TH1D("h1D_reco_jet_e",";Reco Jet E (GeV);",100,0,100);
  TH1D* h1D_reco_jet_p = new TH1D("h1D_reco_jet_p",";Reco Jet p (GeV);",100,0,100);
  TH1D* h1D_reco_jet_pt = new TH1D("h1D_reco_jet_pt",";Reco Jet p_{T} (GeV);",100,0,100);      
  TH1D* h1D_reco_jet_cemcEsum = new TH1D("h1D_reco_jet_cemcEsum",";Reco Jet E_{EMCAL} (GeV);",100,0,100);      
  TH1D* h1D_reco_jet_ihcalEsum = new TH1D("h1D_reco_jet_ihcalEsum",";Reco Jet E_{IHCAL} (GeV);",100,0,100);      
  TH1D* h1D_reco_jet_ohcalEsum = new TH1D("h1D_reco_jet_ohcalEsum",";Reco Jet E_{OHCAL} (GeV);",100,0,100);      
  TH1D* h1D_reco_jet_outerTwo = new TH1D("h1D_reco_jet_outerTwo",";Reco Jet E_{IHCAL}+E_{OHCAL} (GeV);",100,0,100);      
  TH1D* h1D_reco_jet_totE = new TH1D("h1D_reco_jet_totE",";Reco Jet E_{EMCAL}+E_{IHCAL}+E_{OHCAL} (GeV);",100,0,100);      
  TH1D* h1D_reco_jet_outerTwoOverTruth = new TH1D("h1D_reco_jet_outerTwoOverTruth",";Reco Jet (E_{IHCAL}+E_{OHCAL}) / E_{Truth};",100,0,2);      
  TH1D* h1D_reco_jet_outerTwoOverReco = new TH1D("h1D_reco_jet_outerTwoOverReco",";Reco Jet (E_{IHCAL}+E_{OHCAL}) / E_{Reco};",100,0,2);      
  TH1D* h1D_reco_jet_IOAsymm = new TH1D("h1D_reco_jet_IOAsymm",";Reco Jet E_{IHCAL}-E_{OHCAL} asymmetry;",101,-1.05,1.05);      
  TH1D* h1D_reco_jet_EHAsymm = new TH1D("h1D_reco_jet_EHAsymm",";Reco Jet E_{EMCAL}-E_{HCAL} asymmetry;",101,-1.05,1.05);      
  ////////////////////////////////////// 
  TH1D* h1D_jet_JES = new TH1D("h1D_jet_JES",";Jet E_{Reco}/E_{Truth};",100,0,2);      
  TH1D* h1D_jet_dR = new TH1D("h1D_jet_dR",";Jet #Delta R(Truth-Reco);",100,0,0.5);
  ////////////////////////////////////// 
  ////////////////////////////////////// 
  TH2D* h2D_jet_outerTwo_IOAsymm = new TH2D("h2D_jet_outerTwo_IOAsymm",";Reco Jet E_{IHCAL}-E_{OHCAL} asymmetry;Jet E_{IHCAL}+E_{OHCAL} (GeV)",101,-1.05,1.05,100,0,100); 
  TH2D* h2D_jet_outerTwo_EHAsymm = new TH2D("h2D_jet_outerTwo_EHAsymm",";Reco Jet E_{EMCAL}-E_{HCAL} asymmetry;Jet E_{IHCAL}+E_{OHCAL} (GeV)",101,-1.05,1.05,100,0,100); 
  TH2D* h2D_jet_outerTwoOverTruth_IOAsymm = new TH2D("h2D_jet_outerTwoOverTruth_IOAsymm",";Reco Jet E_{IHCAL}-E_{OHCAL} asymmetry;Jet (E_{IHCAL}+E_{OHCAL}) / E_{Truth}",101,-1.05,1.05,100,0,2); 
  TH2D* h2D_jet_outerTwoOverTruth_EHAsymm = new TH2D("h2D_jet_outerTwoOverTruth_EHAsymm",";Reco Jet E_{EMCAL}-E_{HCAL} asymmetry;Jet (E_{IHCAL}+E_{OHCAL}) / E_{Truth}",101,-1.05,1.05,100,0,2); 
  TH2D* h2D_jet_outerTwoOverReco_IOAsymm = new TH2D("h2D_jet_outerTwoOverReco_IOAsymm",";Reco Jet E_{IHCAL}-E_{OHCAL} asymmetry;Jet (E_{IHCAL}+E_{OHCAL}) / E_{Reco}",101,-1.05,1.05,100,0,2); 
  TH2D* h2D_jet_outerTwoOverReco_EHAsymm = new TH2D("h2D_jet_outerTwoOverReco_EHAsymm",";Reco Jet E_{EMCAL}-E_{HCAL} asymmetry;Jet (E_{IHCAL}+E_{OHCAL}) / E_{Reco}",101,-1.05,1.05,100,0,2); 
  TH2D* h2D_jet_totE_IOAsymm = new TH2D("h2D_jet_totE_IOAsymm",";Reco Jet E_{IHCAL}-E_{OHCAL} asymmetry;Jet E_{Reco}",101,-1.05,1.05,100,0,100); 
  TH2D* h2D_jet_totE_EHAsymm = new TH2D("h2D_jet_totE_EHAsymm",";Reco Jet E_{EMCAL}-E_{HCAL} asymmetry;Jet E_{Reco}",101,-1.05,1.05,100,0,100); 
  TH2D* h2D_jet_JES_IOAsymm = new TH2D("h2D_jet_JES_IOAsymm",";Reco Jet E_{IHCAL}-E_{OHCAL} asymmetry;Jet E_{Reco}/E_{Truth}",101,-1.05,1.05,100,0,2); 
  TH2D* h2D_jet_JES_EHAsymm = new TH2D("h2D_jet_JES_EHAsymm",";Reco Jet E_{EMCAL}-E_{HCAL} asymmetry;Jet E_{Reco}/E_{Truth}",101,-1.05,1.05,100,0,2); 
  TH2D* h2D_jet_JES_emfrac = new TH2D("h2D_jet_JES_emfrac",";Truth Jet EM fraction;Jet E_{Reco}/E_{Truth}",101,-0.05,1.05,100,0,2); 
  ////////////////////////////////////// 
  //// jet-to-gamma histograms
  ////////////////////////////////////// 
  TH1D* h1D_JtoG_true_dPhi = new TH1D("h1D_JtoG_true_dPhi",";dPhi(True Gamma - True Jet);",100,0,3.15);
  TH1D* h1D_JtoG_reco_dPhi = new TH1D("h1D_JtoG_reco_dPhi",";dPhi(Reco Gamma - Reco Jet);",100,0,3.15);
  TH1D* h1D_JtoG_outerTwo = new TH1D("h1D_JtoG_outerTwo",";(Jet E_{IHCAL}+E_{OHCAL}) / (#gamma E_{Reco});",100,0,2); 
  TH1D* h1D_JtoG_totE = new TH1D("h1D_JtoG_totE",";(Jet E_{Reco}) / (#gamma E_{Reco});",100,0,2); 
  TH2D* h2D_JtoG_outerTwo_IOAsymm = new TH2D("h2D_JtoG_outerTwo_IOAsymm",";Reco Jet E_{IHCAL}-E_{OHCAL} asymmetry;(Jet E_{IHCAL}+E_{OHCAL}) / (#gamma E_{Reco})",101,-1.05,1.05,100,0,2); 
  TH2D* h2D_JtoG_outerTwo_EHAsymm = new TH2D("h2D_JtoG_outerTwo_EHAsymm",";Reco Jet E_{EMCAL}-E_{HCAL} asymmetry;(Jet E_{IHCAL}+E_{OHCAL}) / (#gamma E_{Reco})",101,-1.05,1.05,100,0,2); 
  TH2D* h2D_JtoG_totE_IOAsymm = new TH2D("h2D_JtoG_totE_IOAsymm",";Reco Jet E_{IHCAL}-E_{OHCAL} asymmetry;(Jet E_{Reco}) / (#gamma E_{Reco})",101,-1.05,1.05,100,0,2); 
  TH2D* h2D_JtoG_totE_EHAsymm = new TH2D("h2D_JtoG_totE_EHAsymm",";Reco Jet E_{EMCAL}-E_{HCAL} asymmetry;(Jet E_{Reco}) / (#gamma E_{Reco})",101,-1.05,1.05,100,0,2); 
  TH2D* h2D_JtoG_totE_emfrac = new TH2D("h2D_JtoG_totE_emfrac",";Truth Jet EM fraction;(Jet E_{Reco}) / (#gamma E_{Reco})",101,-0.05,1.05,100,0,2); 

  ////////////////////////////////////// 
  //// read-in files
  ////////////////////////////////////// 
  TFile* fin = new TFile(Form("./sortedRootFiles/sorted_%dto%d_gammajet_G4sPHENIX_jet4_%s_%s.root",initfile,endfile,jetE.c_str(),ihcalType.c_str()),"READ");
  TTree* true_tree = (TTree*)fin->Get("true_tree");
  TTree* reco_tree = (TTree*)fin->Get("reco_tree");
  true_tree->AddFriend("reco_tree",fin);

  if (true_tree->GetEntries() != reco_tree->GetEntries()) {
    cout <<"WARNING!! Entries of true_tree and reco_tree do not match!">>endl; return;
  }
  //// true_tree
  int true_event;
  int true_n;
  float true_e[999], true_p[999], true_pt[999], true_eta[999], true_phi[999];
  int true_cons_n[999];
  float true_lcons_z[999], true_emfrac[999];
  int true_lcons_pid[999];
  true_tree->SetBranchAddress("event",&true_event); 
  true_tree->SetBranchAddress("true_n",&true_n); 
  true_tree->SetBranchAddress("true_e",&true_e); 
  true_tree->SetBranchAddress("true_p",&true_p); 
  true_tree->SetBranchAddress("true_pt",&true_pt); 
  true_tree->SetBranchAddress("true_eta",&true_eta); 
  true_tree->SetBranchAddress("true_phi",&true_phi); 
  true_tree->SetBranchAddress("true_cons_n",&true_cons_n); 
  true_tree->SetBranchAddress("true_lcons_z",&true_lcons_z); 
  true_tree->SetBranchAddress("true_lcons_pid",&true_lcons_pid); 
  true_tree->SetBranchAddress("true_emfrac",&true_emfrac); 
  
  //// reco_tree
  int reco_event;
  int reco_n;
  float reco_e[999], reco_p[999], reco_pt[999], reco_eta[999], reco_phi[999];
  float reco_cemcEsum[999], reco_ihcalEsum[999], reco_ohcalEsum[999];
  true_tree->SetBranchAddress("reco_tree.event",&reco_event); 
  true_tree->SetBranchAddress("reco_n",&reco_n); 
  true_tree->SetBranchAddress("reco_e",&reco_e); 
  true_tree->SetBranchAddress("reco_p",&reco_p); 
  true_tree->SetBranchAddress("reco_pt",&reco_pt); 
  true_tree->SetBranchAddress("reco_eta",&reco_eta); 
  true_tree->SetBranchAddress("reco_phi",&reco_phi); 
  true_tree->SetBranchAddress("reco_cemcEsum",&reco_cemcEsum); 
  true_tree->SetBranchAddress("reco_ihcalEsum",&reco_ihcalEsum); 
  true_tree->SetBranchAddress("reco_ohcalEsum",&reco_ohcalEsum); 
 
  ////////////////////////////////////// 
  int counts=0;
  for (int ievt=0; ievt<true_tree->GetEntries(); ievt++){ 
  //for (int ievt=0; ievt<10; ievt++){ 
    if (ievt%5000==0) cout << "Processing Event :" <<ievt << endl; 
    true_tree->GetEvent(ievt); 
    
    //if (true_n!=2) continue; //select events only containing 2 objects 
    //int idx_1 =0; int idx_2 =1; 
    
    //// select two objects with highest E
    int idx_1 =999;
    int idx_2 =999;
    float max_e_1 = -999;
    float max_e_2 = -999;
    for (int itrue=0; itrue<true_n; itrue++){
      if (max_e_1<true_e[itrue]) {
        max_e_1 = true_e[itrue];
        idx_1 = itrue;
      }
    }
    for (int itrue=0; itrue<true_n; itrue++){
      if (itrue == idx_1) {continue;} //reject the most highest 
      if (max_e_2<true_e[itrue]) {
        max_e_2 = true_e[itrue];
        idx_2 = itrue;
      }
    }

    //// Set the one with larger em_frac as gamma, and the other as a jet
    float true_gamma_e, true_gamma_p, true_gamma_pt, true_gamma_eta, true_gamma_phi;
    float true_gamma_lcons_z, true_gamma_emfrac;
    float true_jet_e, true_jet_p, true_jet_pt, true_jet_eta, true_jet_phi;
    float true_jet_lcons_z, true_jet_emfrac;
    
    float reco_gamma_e, reco_gamma_p, reco_gamma_pt, reco_gamma_eta, reco_gamma_phi;
    float reco_gamma_cemcEsum, reco_gamma_ihcalEsum, reco_gamma_ohcalEsum;
    float reco_gamma_outerTwo;
    float reco_gamma_outerTwoOverTruth, reco_gamma_outerTwoOverReco;
    float reco_gamma_totE;
    float reco_gamma_IOAsymm;
    float reco_gamma_EHAsymm;
    float reco_jet_e, reco_jet_p, reco_jet_pt, reco_jet_eta, reco_jet_phi;
    float reco_jet_cemcEsum, reco_jet_ihcalEsum, reco_jet_ohcalEsum;
    float reco_jet_outerTwo;
    float reco_jet_outerTwoOverTruth, reco_jet_outerTwoOverReco;
    float reco_jet_totE;
    float reco_jet_IOAsymm;
    float reco_jet_EHAsymm;
    //// for Al_uninst
    float reco_jet_totE_tmp;
    float reco_jet_EHAsymm_tmp;
    
    if ((true_emfrac[idx_1] > true_emfrac[idx_2]) && true_lcons_pid[idx_1] ==22 && true_cons_n[idx_1]==1 ){
      true_gamma_e=true_e[idx_1];
      true_gamma_p=true_p[idx_1];
      true_gamma_pt=true_pt[idx_1];
      true_gamma_eta=true_eta[idx_1];
      true_gamma_phi=true_phi[idx_1];
      true_gamma_lcons_z=true_lcons_z[idx_1];
      true_gamma_emfrac=true_emfrac[idx_1];
      true_jet_e=true_e[idx_2];
      true_jet_p=true_p[idx_2];
      true_jet_pt=true_pt[idx_2];
      true_jet_eta=true_eta[idx_2];
      true_jet_phi=true_phi[idx_2];
      true_jet_lcons_z=true_lcons_z[idx_2];
      true_jet_emfrac=true_emfrac[idx_2];
    } else if ((true_emfrac[idx_1] < true_emfrac[idx_2]) && true_lcons_pid[idx_2] ==22 && true_cons_n[idx_2]==1 ){
      true_gamma_e=true_e[idx_2];
      true_gamma_p=true_p[idx_2];
      true_gamma_pt=true_pt[idx_2];
      true_gamma_eta=true_eta[idx_2];
      true_gamma_phi=true_phi[idx_2];
      true_gamma_lcons_z=true_lcons_z[idx_2];
      true_gamma_emfrac=true_emfrac[idx_2];
      true_jet_e=true_e[idx_1];
      true_jet_p=true_p[idx_1];
      true_jet_pt=true_pt[idx_1];
      true_jet_eta=true_eta[idx_1];
      true_jet_phi=true_phi[idx_1];
      true_jet_lcons_z=true_lcons_z[idx_1];
      true_jet_emfrac=true_emfrac[idx_1];
    } else {
      //cout << "Gamma not found" << endl;
      continue;
    } 
     
    ////////// cut on truth //////////
    //if (true_gamma_e < 50 || true_jet_e < 50) continue;
    //if (true_gamma_e < 50) continue; //only to gamma
    //if (fabs(true_gamma_eta) > 0.6 || fabs(true_jet_eta) > 0.6) continue;
    
    ///////////////////////// 
    //// reco matching
    ///////////////////////// 
    float match_gamma_dR = 999;
    float tmp_gamma_dR = 999;
    float match_jet_dR = 999;
    float tmp_jet_dR = 999;
    int tmp_gamma_idx = 999;
    
    if (reco_n!=2) continue; //select events only containing 2 objects 
    
    ////////// 1) match gamma
    for (int ireco=0; ireco<reco_n; ireco++){
      tmp_gamma_dR = sqrt(pow(true_gamma_eta-reco_eta[ireco],2)+pow(true_gamma_phi-reco_phi[ireco],2));
      if (tmp_gamma_dR < match_gamma_dR){
        match_gamma_dR = tmp_gamma_dR;
        reco_gamma_e = reco_e[ireco];
        reco_gamma_p = reco_p[ireco];
        reco_gamma_pt = reco_pt[ireco];
        reco_gamma_eta = reco_eta[ireco];
        reco_gamma_phi = reco_phi[ireco];
        reco_gamma_cemcEsum = reco_cemcEsum[ireco];
        reco_gamma_ihcalEsum = reco_ihcalEsum[ireco];
        reco_gamma_ohcalEsum = reco_ohcalEsum[ireco];
        reco_gamma_outerTwo = reco_ihcalEsum[ireco]+reco_ohcalEsum[ireco];
        reco_gamma_totE = reco_cemcEsum[ireco]+reco_ihcalEsum[ireco]+reco_ohcalEsum[ireco];
        reco_gamma_outerTwoOverTruth = reco_gamma_outerTwo/true_gamma_e;
        //reco_gamma_outerTwoOverReco = reco_gamma_outerTwo/reco_gamma_e;
        reco_gamma_outerTwoOverReco = reco_gamma_outerTwo/reco_gamma_totE;
        reco_gamma_IOAsymm = (reco_gamma_ihcalEsum-reco_gamma_ohcalEsum)/(reco_gamma_ihcalEsum+reco_gamma_ohcalEsum);
        reco_gamma_EHAsymm = (reco_gamma_cemcEsum-reco_gamma_ihcalEsum-reco_gamma_ohcalEsum)/(reco_gamma_cemcEsum+reco_gamma_ihcalEsum+reco_gamma_ohcalEsum);
        tmp_gamma_idx = ireco;
      }
    } //end of reco_n loop 
    
    ////////// 2) match jet + scaling
    for (int ireco=0; ireco<reco_n; ireco++){
      if (ireco == tmp_gamma_idx) continue; // reject one recoed as gamma
      tmp_jet_dR = sqrt(pow(true_jet_eta-reco_eta[ireco],2)+pow(true_jet_phi-reco_phi[ireco],2));
      if (tmp_jet_dR < match_jet_dR){
        match_jet_dR = tmp_jet_dR;
        reco_jet_e = reco_e[ireco];
        reco_jet_p = reco_p[ireco];
        reco_jet_pt = reco_pt[ireco];
        reco_jet_eta = reco_eta[ireco];
        reco_jet_phi = reco_phi[ireco];
        //// for Al_uninst
        reco_jet_totE_tmp = reco_cemcEsum[ireco]+reco_ihcalEsum[ireco]+reco_ohcalEsum[ireco];
        reco_jet_EHAsymm_tmp = (reco_cemcEsum[ireco]-reco_ihcalEsum[ireco]-reco_ohcalEsum[ireco])/(reco_cemcEsum[ireco]+reco_ihcalEsum[ireco]+reco_ohcalEsum[ireco]);
        //// non-constant scaling
        if (ver==1) {
          reco_jet_cemcEsum = reco_cemcEsum[ireco];
          reco_jet_ihcalEsum = reco_ihcalEsum[ireco];
          reco_jet_ohcalEsum = reco_ohcalEsum[ireco]*refval/f_EHAsymm->Eval(reco_jet_EHAsymm_tmp);
        }
        else if (ver==2) { //wrong
          reco_jet_cemcEsum = reco_cemcEsum[ireco];
          reco_jet_ihcalEsum = reco_ihcalEsum[ireco];
          reco_jet_ohcalEsum = ( reco_ohcalEsum[ireco]*refval/f_EHAsymm->Eval(reco_jet_EHAsymm_tmp) ) * ( 2./(1-reco_jet_EHAsymm_tmp) );
        }
        else if (ver==3) {
          reco_jet_cemcEsum = reco_cemcEsum[ireco]*refval/f_EHAsymm->Eval(reco_jet_EHAsymm_tmp);
          reco_jet_ihcalEsum = reco_ihcalEsum[ireco]*refval/f_EHAsymm->Eval(reco_jet_EHAsymm_tmp);
          reco_jet_ohcalEsum = reco_ohcalEsum[ireco]*refval/f_EHAsymm->Eval(reco_jet_EHAsymm_tmp);
        }
        else if (ver==4) {
          reco_jet_cemcEsum = reco_cemcEsum[ireco];
          reco_jet_ihcalEsum = reco_ihcalEsum[ireco];
          reco_jet_ohcalEsum = reco_ohcalEsum[ireco]*
                    ( 1+ (refval-f_EHAsymm->Eval(reco_jet_EHAsymm_tmp))/(reco_ohcalEsum[ireco]/reco_gamma_totE) ); //gamma info..
        }
        else if (ver==5) {
          reco_jet_cemcEsum = reco_cemcEsum[ireco];
          reco_jet_ihcalEsum = reco_ihcalEsum[ireco];
          reco_jet_ohcalEsum = reco_ohcalEsum[ireco]*
                    ( 1+ (refval-f_EHAsymm->Eval(reco_jet_EHAsymm_tmp))/(reco_ohcalEsum[ireco]/reco_jet_totE_tmp) ); //totE instead of gamma 
        }
        /*
        cout << ""<<endl;
        cout << "reco_jet_cemcEsum : "<<reco_jet_cemcEsum<<endl;
        cout << "reco_jet_ihcalEsum : "<<reco_jet_ihcalEsum<<endl;
        cout << "reco_ohcalEsum[ireco] : "<<reco_ohcalEsum[ireco]<<endl;
        cout << "*reco_jet_EHAsymm : "<<reco_jet_EHAsymm<<endl;
        cout << "f_EHAsymm->Eval(reco_jet_EHAsymm) : "<<f_EHAsymm->Eval(reco_jet_EHAsymm)<<endl;
        cout << "scale factor : " << refval/f_EHAsymm->Eval(reco_jet_EHAsymm) <<endl;
        cout << "*reco_jet_ohcalEsum : "<<reco_jet_ohcalEsum<<endl;
        */
        reco_jet_outerTwo = reco_jet_ihcalEsum+reco_jet_ohcalEsum;
        reco_jet_totE = reco_jet_cemcEsum+reco_jet_ihcalEsum+reco_jet_ohcalEsum;
        reco_jet_outerTwoOverTruth = reco_jet_outerTwo/true_jet_e;
        reco_jet_outerTwoOverReco = reco_jet_outerTwo/reco_jet_totE;
        reco_jet_IOAsymm = (reco_jet_ihcalEsum-reco_jet_ohcalEsum)/(reco_jet_ihcalEsum+reco_jet_ohcalEsum);
        reco_jet_EHAsymm = (reco_jet_cemcEsum-reco_jet_ihcalEsum-reco_jet_ohcalEsum)/(reco_jet_cemcEsum+reco_jet_ihcalEsum+reco_jet_ohcalEsum);
      }
    } //end of reco_n loop 
   
    /// double-check dPhi
    float true_dPhi = fabs( DeltaPhi(true_gamma_phi, true_jet_phi) );
    float reco_dPhi = fabs( DeltaPhi(reco_gamma_phi, reco_jet_phi) );
    
    ////////// cut on reco //////////
    //// *** dR(reco-true) matching  
    //if (match_gamma_dR > 0.4 || match_jet_dR > 0.4) continue; 
    //// *** dR(gamma-jet) in different hemisphere
    //if (reco_dPhi < TMath::Pi()/2.) continue;
    //if (reco_dPhi < TMath::Pi()*(3/4.)) continue;
    //if (reco_dPhi < TMath::Pi()*(7/8.)) continue;
    //if (reco_dPhi < TMath::Pi()*(31/32.)) continue;
    //// HCAL/CEMC cut for highly efficient photon
    if (reco_gamma_outerTwo/reco_gamma_cemcEsum  >0.1) continue;
    //// *** reco jet e range (cut on reco)
    if (jetE.compare("20GeV")==0){
      if (!(reco_gamma_e > 20 && reco_gamma_e <30)) continue; //only to reco gamma
    }
    else if (jetE.compare("30GeV")==0){
      if (!(reco_gamma_e > 30 && reco_gamma_e <40)) continue; //only to reco gamma
    }
    else if (jetE.compare("40GeV")==0){
      if (!(reco_gamma_e > 40 && reco_gamma_e <50)) continue; //only to reco gamma
    }
    else if (jetE.compare("50GeV")==0){
      if (!(reco_gamma_e > 50 && reco_gamma_e <60)) continue; //only to reco gamma
    }

    ///////////////////////// 
    //// fill the histograms
    ///////////////////////// 
    h1D_true_gamma_e->Fill(true_gamma_e);
    h1D_true_gamma_p->Fill(true_gamma_p);
    h1D_true_gamma_pt->Fill(true_gamma_pt);
    h1D_true_gamma_lcons_z->Fill(true_gamma_lcons_z);
    h1D_true_gamma_emfrac->Fill(true_gamma_emfrac);
    h1D_reco_gamma_e->Fill(reco_gamma_e);
    h1D_reco_gamma_p->Fill(reco_gamma_p);
    h1D_reco_gamma_pt->Fill(reco_gamma_pt);
    h1D_reco_gamma_cemcEsum->Fill(reco_gamma_cemcEsum);
    h1D_reco_gamma_ihcalEsum->Fill(reco_gamma_ihcalEsum);
    h1D_reco_gamma_ohcalEsum->Fill(reco_gamma_ohcalEsum);
    h1D_reco_gamma_outerTwo->Fill(reco_gamma_outerTwo);
    h1D_reco_gamma_totE->Fill(reco_gamma_totE);
    h1D_reco_gamma_outerTwoOverTruth->Fill(reco_gamma_outerTwoOverTruth);
    h1D_reco_gamma_outerTwoOverReco->Fill(reco_gamma_outerTwoOverReco);
    h1D_reco_gamma_IOAsymm->Fill(reco_gamma_IOAsymm);
    h1D_reco_gamma_EHAsymm->Fill(reco_gamma_EHAsymm);
    //h1D_gamma_JES->Fill(reco_gamma_e/true_gamma_e);
    h1D_gamma_JES->Fill(reco_gamma_totE/true_gamma_e);
    h1D_gamma_dR->Fill(match_gamma_dR);
    ////////////////////////////////////// 
    h2D_gamma_outerTwo_IOAsymm->Fill(reco_gamma_IOAsymm,reco_gamma_outerTwo);
    h2D_gamma_outerTwo_EHAsymm->Fill(reco_gamma_EHAsymm,reco_gamma_outerTwo);
    h2D_gamma_outerTwoOverTruth_IOAsymm->Fill(reco_gamma_IOAsymm,reco_gamma_outerTwo/true_gamma_e);
    h2D_gamma_outerTwoOverTruth_EHAsymm->Fill(reco_gamma_EHAsymm,reco_gamma_outerTwo/true_gamma_e);
    //h2D_gamma_outerTwoOverReco_IOAsymm->Fill(reco_gamma_IOAsymm,reco_gamma_outerTwo/reco_gamma_e);
    //h2D_gamma_outerTwoOverReco_EHAsymm->Fill(reco_gamma_EHAsymm,reco_gamma_outerTwo/reco_gamma_e);
    h2D_gamma_outerTwoOverReco_IOAsymm->Fill(reco_gamma_IOAsymm,reco_gamma_outerTwo/reco_gamma_totE);
    h2D_gamma_outerTwoOverReco_EHAsymm->Fill(reco_gamma_EHAsymm,reco_gamma_outerTwo/reco_gamma_totE);
    h2D_gamma_totE_IOAsymm->Fill(reco_gamma_IOAsymm,reco_gamma_totE);
    h2D_gamma_totE_EHAsymm->Fill(reco_gamma_EHAsymm,reco_gamma_totE);
    //h2D_gamma_JES_IOAsymm->Fill(reco_gamma_IOAsymm,reco_gamma_e/true_gamma_e);
    //h2D_gamma_JES_EHAsymm->Fill(reco_gamma_EHAsymm,reco_gamma_e/true_gamma_e);
    h2D_gamma_JES_IOAsymm->Fill(reco_gamma_IOAsymm,reco_gamma_totE/true_gamma_e);
    h2D_gamma_JES_EHAsymm->Fill(reco_gamma_EHAsymm,reco_gamma_totE/true_gamma_e);
    h2D_gamma_JES_emfrac->Fill(true_gamma_emfrac,reco_gamma_totE/true_gamma_e);
    ////////////////////////////////////// 
    ////////////////////////////////////// 
    h1D_true_jet_e->Fill(true_jet_e);
    h1D_true_jet_p->Fill(true_jet_p);
    h1D_true_jet_pt->Fill(true_jet_pt);
    h1D_true_jet_lcons_z->Fill(true_jet_lcons_z);
    h1D_true_jet_emfrac->Fill(true_jet_emfrac);
    h1D_reco_jet_e->Fill(reco_jet_e);
    h1D_reco_jet_p->Fill(reco_jet_p);
    h1D_reco_jet_pt->Fill(reco_jet_pt);
    h1D_reco_jet_cemcEsum->Fill(reco_jet_cemcEsum);
    h1D_reco_jet_ihcalEsum->Fill(reco_jet_ihcalEsum);
    h1D_reco_jet_ohcalEsum->Fill(reco_jet_ohcalEsum);
    h1D_reco_jet_outerTwo->Fill(reco_jet_outerTwo);
    h1D_reco_jet_totE->Fill(reco_jet_totE);
    h1D_reco_jet_outerTwoOverTruth->Fill(reco_jet_outerTwoOverTruth);
    h1D_reco_jet_outerTwoOverReco->Fill(reco_jet_outerTwoOverReco);
    h1D_reco_jet_IOAsymm->Fill(reco_jet_IOAsymm);
    h1D_reco_jet_EHAsymm->Fill(reco_jet_EHAsymm);
    h1D_jet_JES->Fill(reco_jet_totE/true_jet_e);
    h1D_jet_dR->Fill(match_jet_dR);
    ////////////////////////////////////// 
    h2D_jet_outerTwo_IOAsymm->Fill(reco_jet_IOAsymm,reco_jet_outerTwo);
    h2D_jet_outerTwo_EHAsymm->Fill(reco_jet_EHAsymm,reco_jet_outerTwo);
    h2D_jet_outerTwoOverTruth_IOAsymm->Fill(reco_jet_IOAsymm,reco_jet_outerTwo/true_jet_e);
    h2D_jet_outerTwoOverTruth_EHAsymm->Fill(reco_jet_EHAsymm,reco_jet_outerTwo/true_jet_e);
    h2D_jet_outerTwoOverReco_IOAsymm->Fill(reco_jet_IOAsymm,reco_jet_outerTwo/reco_jet_totE);
    h2D_jet_outerTwoOverReco_EHAsymm->Fill(reco_jet_EHAsymm,reco_jet_outerTwo/reco_jet_totE);
    h2D_jet_totE_IOAsymm->Fill(reco_jet_IOAsymm,reco_jet_totE);
    h2D_jet_totE_EHAsymm->Fill(reco_jet_EHAsymm,reco_jet_totE);
    h2D_jet_JES_IOAsymm->Fill(reco_jet_IOAsymm,reco_jet_totE/true_jet_e);
    h2D_jet_JES_EHAsymm->Fill(reco_jet_EHAsymm,reco_jet_totE/true_jet_e);
    h2D_jet_JES_emfrac->Fill(true_jet_emfrac,reco_jet_totE/true_jet_e);
    ////////////////////////////////////// 
    //// avoid truth info. reco_jet/reco_gamma
    ////////////////////////////////////// 
    h1D_JtoG_true_dPhi->Fill(true_dPhi);
    h1D_JtoG_reco_dPhi->Fill(reco_dPhi);
    h1D_JtoG_outerTwo->Fill(reco_jet_outerTwo/reco_gamma_totE);
    h1D_JtoG_totE->Fill(reco_jet_totE/reco_gamma_totE);
    h2D_JtoG_outerTwo_IOAsymm->Fill(reco_jet_IOAsymm,reco_jet_outerTwo/reco_gamma_totE);
    h2D_JtoG_outerTwo_EHAsymm->Fill(reco_jet_EHAsymm,reco_jet_outerTwo/reco_gamma_totE);
    h2D_JtoG_totE_IOAsymm->Fill(reco_jet_IOAsymm,reco_jet_totE/reco_gamma_totE);
    h2D_JtoG_totE_EHAsymm->Fill(reco_jet_EHAsymm,reco_jet_totE/reco_gamma_totE);
    h2D_JtoG_totE_emfrac->Fill(true_jet_emfrac,reco_jet_totE/reco_gamma_totE);
    ////////////////////////////////////// 
  
    counts++;
  
  } // end of event loop
  cout << "Total true events analyzed : " << counts<<"/"<<true_tree->GetEntries() << endl; 
 
  h2D_gamma_outerTwo_IOAsymm->SetOption("colz");
  h2D_gamma_outerTwo_EHAsymm->SetOption("colz");
  h2D_gamma_outerTwoOverTruth_IOAsymm->SetOption("colz");
  h2D_gamma_outerTwoOverTruth_EHAsymm->SetOption("colz");
  h2D_gamma_outerTwoOverReco_IOAsymm->SetOption("colz");
  h2D_gamma_outerTwoOverReco_EHAsymm->SetOption("colz");
  h2D_gamma_totE_IOAsymm->SetOption("colz");
  h2D_gamma_totE_EHAsymm->SetOption("colz");
  h2D_gamma_JES_IOAsymm->SetOption("colz");
  h2D_gamma_JES_EHAsymm->SetOption("colz");
  h2D_gamma_JES_emfrac->SetOption("colz");
  ////
  h2D_jet_outerTwo_IOAsymm->SetOption("colz");
  h2D_jet_outerTwo_EHAsymm->SetOption("colz");
  h2D_jet_outerTwoOverTruth_IOAsymm->SetOption("colz");
  h2D_jet_outerTwoOverTruth_EHAsymm->SetOption("colz");
  h2D_jet_outerTwoOverReco_IOAsymm->SetOption("colz");
  h2D_jet_outerTwoOverReco_EHAsymm->SetOption("colz");
  h2D_jet_totE_IOAsymm->SetOption("colz");
  h2D_jet_totE_EHAsymm->SetOption("colz");
  h2D_jet_JES_IOAsymm->SetOption("colz");
  h2D_jet_JES_EHAsymm->SetOption("colz");
  h2D_jet_JES_emfrac->SetOption("colz");
  ////  
  h2D_JtoG_outerTwo_IOAsymm->SetOption("colz");
  h2D_JtoG_outerTwo_EHAsymm->SetOption("colz");
  h2D_JtoG_totE_IOAsymm->SetOption("colz");
  h2D_JtoG_totE_EHAsymm->SetOption("colz");
  h2D_JtoG_totE_emfrac->SetOption("colz");

  ////////////////////////////////////// 
//  TFile* fout = new TFile(Form("%s%s/gammajet_matched_%s_%s_scale_%.0f_%.0f_%.0f.root",outDir.c_str(),jetE.c_str(),jetE.c_str(),ihcalType.c_str(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100),"RECREATE");
  TFile* fout = new TFile(Form("%sAlUninst/gammajet_matched_%s_%s_ver_%d.root",outDir.c_str(),jetE.c_str(),ihcalType.c_str(),ver),"RECREATE");
  fout->cd();
  ////
  h1D_true_gamma_e->Write();
  h1D_true_gamma_p->Write();
  h1D_true_gamma_pt->Write();
  h1D_true_gamma_lcons_z->Write();
  h1D_true_gamma_emfrac->Write();
  h1D_reco_gamma_e->Write();
  h1D_reco_gamma_p->Write();
  h1D_reco_gamma_pt->Write();
  h1D_reco_gamma_cemcEsum->Write();
  h1D_reco_gamma_ihcalEsum->Write();
  h1D_reco_gamma_ohcalEsum->Write();
  h1D_reco_gamma_outerTwo->Write();
  h1D_reco_gamma_totE->Write();
  h1D_reco_gamma_outerTwoOverTruth->Write();
  h1D_reco_gamma_outerTwoOverReco->Write();
  h1D_reco_gamma_IOAsymm->Write();
  h1D_reco_gamma_EHAsymm->Write();
  h1D_gamma_JES->Write();
  h1D_gamma_dR->Write();
  h2D_gamma_outerTwo_IOAsymm->Write();
  h2D_gamma_outerTwo_EHAsymm->Write();
  h2D_gamma_outerTwoOverTruth_IOAsymm->Write();
  h2D_gamma_outerTwoOverTruth_EHAsymm->Write();
  h2D_gamma_outerTwoOverReco_IOAsymm->Write();
  h2D_gamma_outerTwoOverReco_EHAsymm->Write();
  h2D_gamma_totE_IOAsymm->Write();
  h2D_gamma_totE_EHAsymm->Write();
  h2D_gamma_JES_IOAsymm->Write();
  h2D_gamma_JES_EHAsymm->Write();
  h2D_gamma_JES_emfrac->Write();
  ////
  h1D_true_jet_e->Write();
  h1D_true_jet_p->Write();
  h1D_true_jet_pt->Write();
  h1D_true_jet_lcons_z->Write();
  h1D_true_jet_emfrac->Write();
  h1D_reco_jet_e->Write();
  h1D_reco_jet_p->Write();
  h1D_reco_jet_pt->Write();
  h1D_reco_jet_cemcEsum->Write();
  h1D_reco_jet_ihcalEsum->Write();
  h1D_reco_jet_ohcalEsum->Write();
  h1D_reco_jet_outerTwo->Write();
  h1D_reco_jet_totE->Write();
  h1D_reco_jet_outerTwoOverTruth->Write();
  h1D_reco_jet_outerTwoOverReco->Write();
  h1D_reco_jet_IOAsymm->Write();
  h1D_reco_jet_EHAsymm->Write();
  h1D_jet_JES->Write();
  h1D_jet_dR->Write();
  h2D_jet_outerTwo_IOAsymm->Write();
  h2D_jet_outerTwo_EHAsymm->Write();
  h2D_jet_outerTwoOverTruth_IOAsymm->Write();
  h2D_jet_outerTwoOverTruth_EHAsymm->Write();
  h2D_jet_outerTwoOverReco_IOAsymm->Write();
  h2D_jet_outerTwoOverReco_EHAsymm->Write();
  h2D_jet_totE_IOAsymm->Write();
  h2D_jet_totE_EHAsymm->Write();
  h2D_jet_JES_IOAsymm->Write();
  h2D_jet_JES_EHAsymm->Write();
  h2D_jet_JES_emfrac->Write();
  ////
  h1D_JtoG_true_dPhi->Write();
  h1D_JtoG_reco_dPhi->Write();
  h1D_JtoG_outerTwo->Write();
  h1D_JtoG_totE->Write();
  h2D_JtoG_outerTwo_IOAsymm->Write();
  h2D_JtoG_outerTwo_EHAsymm->Write();
  h2D_JtoG_totE_IOAsymm->Write();
  h2D_JtoG_totE_EHAsymm->Write();
  h2D_JtoG_totE_emfrac->Write();
  ////////////////////////////////////// 
  fout->Close(); 

  cout << Form("%sAlUninst/gammajet_matched_%s_%s_ver_%d.root",outDir.c_str(),jetE.c_str(),ihcalType.c_str(),ver) << endl;
  
  return; 
 
}

double DeltaPhi(double phi1, double phi2){
  Double_t dphi;
  dphi = phi1 - phi2;
  if(dphi<-TMath::Pi()) dphi += TMath::TwoPi();
  if(dphi>=TMath::Pi()) dphi -= TMath::TwoPi();
  return dphi;
}
