
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

#define ITER_START 0
#define NUM_ITER 199


void AnalysisMatched()
{


  // Suppress error messages from bad files
  gErrorIgnoreLevel = kFatal; 

  TChain jets("matched_jets");
  
  int n_evts = 0; 
  int bad_files = 0; 

  for(int j=ITER_START; j<=NUM_ITER; j++){
      
      TString fname = "./jet_analysis_";
      fname += j; 
      fname += ".root";

      //cout << fname << endl; 

      // skip bad files
      TFile f(fname); 
      if(f.GetSize()<10000){f.Close(); bad_files++; continue;}
      if(f.IsZombie()) {f.Close(); bad_files++; continue;}
      TTree *evts = (TTree *)f.Get("event"); 
      n_evts += evts->GetEntries(); 
      f.Close(); 
 
      jets.AddFile(fname);
      
  }

  cout << "Number of bad files = " << bad_files << endl; 
  cout << "Total simulated events = " << n_evts << endl; 
  
  // Histograms
  
  TFile f("JETS_plots_matched_jets.root","RECREATE");

  TH1D *hjet_p_eta = new TH1D("hjet_p_eta", "Jet Pseudorapidity (primary)", 100,1.0,5.0); 
  TH1D *hjet_trk_eta = new TH1D("hjet_trk_eta", "Jet Pseudorapidity (tracks)", 100,1.0,5.0); 
  TH1D *hjet_twr_eta = new TH1D("hjet_twr_eta", "Jet Pseudorapidity (tower)", 100,1.0,5.0); 
  TH1D *hjet_p_ptot = new TH1D("hjet_p_ptot", "Jet Momentum (primary)", 100,0.0,200.0); 
  TH1D *hjet_trk_ptot = new TH1D("hjet_trk_ptot", "Jet Momentum (tracks)", 100,0.0,200.0); 
  TH1D *hjet_twr_ptot = new TH1D("hjet_twr_ptot", "Jet Momentum (tower)", 100,0.0,200.0); 

  TH1D *hjet_p_nconstit = new TH1D("hjet_p_nconstit" , "Number of primary constituents", 31, -0.5, 30.5);
  TH1D *hjet_trk_nconstit = new TH1D("hjet_trk_nconstit" , "Number of track constituents", 31, -0.5, 30.5);
  TH1D *hjet_twr_nconstit = new TH1D("hjet_twr_nconstit" , "Number of tower constituents", 31, -0.5, 30.5);
  TH1D *hjet_npg = new TH1D("hjet_npg" , "Number of jet progenitors", 6, -0.5, 5.5);
  TH1D *hcharge_lead = new TH1D("hcharge_lead" , "Charge of leading particle (track jet)", 3, -1.5, 1.5);
  TH1D *hz_lead = new TH1D("hz_lead" , "z of leading charged particle (track jet)", 100, 0.0, 1.0);

  TH1D *hjet_pg1 = new TH1D("hjet_pg1","Jet Dominant Progenitors",49,-24.5,24.5);
  TH1D *hjet_pg1_fract = new TH1D("hjet_pg1_fract","Jet Dominant Progenitors energy fraction",101,-0.01,1.01);
  TH1D *hjet_pg2 = new TH1D("hjet_pg2","Jet Dominant Progenitors",49,-24.5,24.5);
  TH1D *hjet_pg2_fract = new TH1D("hjet_pg2_fract","Jet pg=2 Progenitors energy fraction",101,-0.01,1.01);
  TH1D *hjet_pg3 = new TH1D("hjet_pg3","Jet Dominant Progenitors",49,-24.5,24.5);
  TH1D *hjet_pg3_fract = new TH1D("hjet_pg3_fract","Jet pg=3 Progenitors energy fraction",101,-0.01,1.01);
  TH1D *hjet_pg4 = new TH1D("hjet_pg4","Jet Dominant Progenitors",49,-24.5,24.5);
  TH1D *hjet_pg4_fract = new TH1D("hjet_pg4_fract","Jet pg=4 Progenitors energy fraction",101,-0.01,1.01);

  TH1D *hjet_trk_MatchdR = new TH1D("hjet_trk_MatchdR","Primary jet match delta-R to Track Jet",100,0.0,0.5); 
  TH1D *hjet_twr_MatchdR = new TH1D("hjet_twr_MatchdR","Primary jet match delta-R to Tower Jet",100,0.0,0.5); 

  TH2D *hjet_trk_Eres = new TH2D("hjet_trk_Eres","Track Jet Energy Resolution",100,0.0,300.0,100,-1.0,1.0); 
  TH2D *hjet_twr_Eres = new TH2D("hjet_twr_Eres","Tower Jet Energy Resolution",100,0.0,300.0,100,-1.0,1.0); 
  TH2D *hjet_comb_Eres = new TH2D("hjet_comb_Eres","Combined Jet Energy Resolution",100,0.0,300.0,100,-1.0,1.0); 
  TH2D *hjet_comb_bh_Eres = new TH2D("hjet_comb_bh_Eres","Combined Jet Energy Resolution",100,0.0,300.0,100,-1.0,1.0); 
  TH1D *hjet_comb_neut_fract = new TH1D("hjet_comb_neut_fract","Combined Jet Neutral Energy Fraction",100,0.0,1.0); 

  TH2D *hjet_comb_EtaRes = new TH2D("hjet_comb_EtaRes","Combined Jet Eta Resolution",100,0.0,300.0,100,-0.4,0.4); 
  TH2D *hjet_trk_EtaRes = new TH2D("hjet_trk_EtaRes","Track Jet Eta Resolution",100,0.0,300.0,100,-0.4,0.4); 
  TH2D *hjet_twr_EtaRes = new TH2D("hjet_twr_EtaRes","Tower Jet Eta Resolution",100,0.0,300.0,100,-0.4,0.4); 

  TH2D *hjet_comb_PhiRes = new TH2D("hjet_comb_PhiRes","Combined Jet Phi Resolution",100,0.0,300.0,100,-0.3,0.3); 
  TH2D *hjet_trk_PhiRes = new TH2D("hjet_trk_PhiRes","Track Jet Phi Resolution",100,0.0,300.0,100,-0.3,0.3); 
  TH2D *hjet_twr_PhiRes = new TH2D("hjet_twr_PhiRes","Tower Jet Phi Resolution",100,0.0,300.0,100,-0.3,0.3); 

  TH2D *hjet_twr_Eres_quark = new TH2D("hjet_twr_Eres_quark","Tower Jet Energy Resolution",100,0.0,300.0,100,-1.0,1.0); 
  TH2D *hjet_comb_Eres_quark = new TH2D("hjet_comb_Eres_quark","Combined Jet Energy Resolution",100,0.0,300.0,100,-1.0,1.0); 
  TH2D *hjet_twr_Eres_gluon = new TH2D("hjet_twr_Eres_gluon","Tower Jet Energy Resolution",100,0.0,300.0,100,-1.0,1.0); 
  TH2D *hjet_comb_Eres_gluon = new TH2D("hjet_comb_Eres_gluon","Combined Jet Energy Resolution",100,0.0,300.0,100,-1.0,1.0); 
  TH1D *hjet_comb_neut_fract_quark = new TH1D("hjet_comb_neut_fract_quark","Combined Jet Neutral Energy Fraction",100,0.0,1.0); 
  TH1D *hjet_comb_neut_fract_gluon = new TH1D("hjet_comb_neut_fract_gluon","Combined Jet Neutral Energy Fraction",100,0.0,1.0); 

  TH1D *had_Z_meas = new TH1D("had_Z_meas","Measured charged hadron z",100,0.0,1.0);
  TH2D *had_Z_resolution = new TH2D("had_Z_resolution","z Resolution for charged hadrons",100,0.0,1.0,100,-1.0,1.0); 

  TH1D *cemc_dNdR = new TH1D("cemc_dNdR","dN/dR for towers in jet",100,0.0,0.5); 
  TH1D *ihcal_dNdR = new TH1D("ihcal_dNdR","dN/dR for towers in jet",100,0.0,0.5); 
  TH1D *ohcal_dNdR = new TH1D("ohcal_dNdR","dN/dR for towers in jet",100,0.0,0.5); 

  TH1D *cemc_dEdR = new TH1D("cemc_dEdR","dE/dR for towers in jet",100,0.0,0.5); 
  TH1D *ihcal_dEdR = new TH1D("ihcal_dEdR","dE/dR for towers in jet",100,0.0,0.5); 
  TH1D *ohcal_dEdR = new TH1D("ohcal_dEdR","dE/dR for towers in jet",100,0.0,0.5); 

  TH1D *cemc_1rdEdR = new TH1D("cemc_1rdEdR","1/r dE/dR for towers in jet",100,0.0,0.5); 
  TH1D *ihcal_1rdEdR = new TH1D("ihcal_1rdEdR","1/r dE/dR for towers in jet",100,0.0,0.5); 
  TH1D *ohcal_1rdEdR = new TH1D("ohcal_1rdEdR","1/r dE/dR for towers in jet",100,0.0,0.5); 

  TH1D *hjet_twr_emfract = new TH1D("hjet_twr_emfract","Tower Jet CEMC Energy Fraction",100,0.0,1.0);
  TH2D *hjet_twrE_vs_emfract = new TH2D("hjet_twrE_vs_emfract","Tower Jet Energy vs. CEMC Energy Fraction",100,0.0,1.0,100,0.0,75.0);

  // Jets Plots

  float jet_p_ptot, jet_trk_ptot, jet_twr_ptot; 
  jets.SetBranchAddress("jet_p_ptot",&jet_p_ptot); 
  jets.SetBranchAddress("jet_trk_ptot",&jet_trk_ptot); 
  jets.SetBranchAddress("jet_twr_ptot",&jet_twr_ptot); 

  float jet_p_pT, jet_trk_pT, jet_twr_pT; 
  jets.SetBranchAddress("jet_p_pT",&jet_p_pT); 
  jets.SetBranchAddress("jet_trk_pT",&jet_trk_pT); 
  jets.SetBranchAddress("jet_twr_pT",&jet_twr_pT); 

  float jet_p_eta, jet_trk_eta, jet_twr_eta; 
  jets.SetBranchAddress("jet_p_eta",&jet_p_eta); 
  jets.SetBranchAddress("jet_trk_eta",&jet_trk_eta); 
  jets.SetBranchAddress("jet_twr_eta",&jet_twr_eta); 

  float jet_p_phi, jet_trk_phi, jet_twr_phi; 
  jets.SetBranchAddress("jet_p_phi",&jet_p_phi); 
  jets.SetBranchAddress("jet_trk_phi",&jet_trk_phi); 
  jets.SetBranchAddress("jet_twr_phi",&jet_twr_phi); 

  int jet_p_nconstit, jet_trk_nconstit, jet_twr_nconstit; 
  jets.SetBranchAddress("jet_p_nconstit",&jet_p_nconstit); 
  jets.SetBranchAddress("jet_trk_nconstit",&jet_trk_nconstit); 
  jets.SetBranchAddress("jet_twr_nconstit",&jet_twr_nconstit); 

  // These variables are for the track jet

  std::vector<int> *pg_id = 0; 
  jets.SetBranchAddress("pg_twr_id",&pg_id); 
  std::vector<double> *pg_fract = 0; 
  jets.SetBranchAddress("pg_twr_fract",&pg_fract); 
  std::vector<int> *pg_status = 0; 
  jets.SetBranchAddress("pg_twr_status",&pg_status); 

  float z_lead; 
  jets.SetBranchAddress("trk_z_lead",&z_lead); 

  int charge_lead; 
  jets.SetBranchAddress("trk_charge_lead",&charge_lead); 

  float jet_trk_dR, jet_twr_dR; 
  jets.SetBranchAddress("jet_trk_dR",&jet_trk_dR); 
  jets.SetBranchAddress("jet_twr_dR",&jet_twr_dR); 

  float jet_comb_ptot; 
  jets.SetBranchAddress("jet_comb_ptot",&jet_comb_ptot); 
  float jet_comb_pT; 
  jets.SetBranchAddress("jet_comb_pT",&jet_comb_pT); 
  float jet_comb_eta; 
  jets.SetBranchAddress("jet_comb_eta",&jet_comb_eta); 
  float jet_comb_phi; 
  jets.SetBranchAddress("jet_comb_phi",&jet_comb_phi); 
  float jet_comb_neutral_fract; 
  jets.SetBranchAddress("jet_comb_neutral_fract",&jet_comb_neutral_fract); 

  float jet_comb_bh_ptot; 
  jets.SetBranchAddress("jet_comb_bh_ptot",&jet_comb_bh_ptot); 

  // Jet hadron properties
  
  std::vector<double> *jet_had_Z = 0; 
  std::vector<double> *jet_had_Ztrue = 0; 
  jets.SetBranchAddress("jet_had_Z",&jet_had_Z); 
  jets.SetBranchAddress("jet_had_Ztrue",&jet_had_Ztrue); 

  // Tower Jet constituents

  std::vector<double> *cemc_constituent_E = 0; 
  std::vector<double> *cemc_constituent_dR = 0; 
  std::vector<double> *ihcal_constituent_E = 0; 
  std::vector<double> *ihcal_constituent_dR = 0; 
  std::vector<double> *ohcal_constituent_E = 0; 
  std::vector<double> *ohcal_constituent_dR = 0; 

  jets.SetBranchAddress("cemc_constituent_E",&cemc_constituent_E); 
  jets.SetBranchAddress("cemc_constituent_dR",&cemc_constituent_dR); 
  jets.SetBranchAddress("ihcal_constituent_E",&ihcal_constituent_E); 
  jets.SetBranchAddress("ihcal_constituent_dR",&ihcal_constituent_dR); 
  jets.SetBranchAddress("ohcal_constituent_E",&ohcal_constituent_E); 
  jets.SetBranchAddress("ohcal_constituent_dR",&ohcal_constituent_dR); 

  unsigned long long nsim = jets.GetEntries();
  int n_good_jets = 0; 

  cout << "Examining " << nsim << " jets entries ..." << endl; 
 
  for(unsigned long long isim=0; isim<nsim; isim++){

    jets.GetEvent(isim);
 
    // cuts for jet sources

    if( fabs(jet_trk_eta)>0.6 ) continue; 
    if( jet_p_ptot<50.0) continue; 

    n_good_jets++; 

    // Plots of jet properties 

    int jet_npg = pg_id->size(); 

    hjet_p_eta->Fill(jet_p_eta); 
    hjet_trk_eta->Fill(jet_trk_eta); 
    hjet_twr_eta->Fill(jet_twr_eta); 

    hjet_p_ptot->Fill(jet_p_ptot); 
    hjet_trk_ptot->Fill(jet_trk_ptot); 
    hjet_twr_ptot->Fill(jet_twr_ptot); 
   
    hjet_p_nconstit->Fill(jet_p_nconstit); 
    hjet_trk_nconstit->Fill(jet_trk_nconstit); 
    hjet_twr_nconstit->Fill(jet_twr_nconstit); 

    bool light_quark = false; 
    bool gluon = false;

    hjet_npg->Fill(jet_npg);
    if(jet_npg>0){
      hjet_pg1->Fill(pg_id->at(0)); 
      hjet_pg1_fract->Fill(pg_fract->at(0));
      if(pg_fract->at(0)>0.5){
        if( (pg_id->at(0)==1) || (pg_id->at(0)==2) ) 
	  light_quark = true; 
	else if (pg_id->at(0)==21) 
	  gluon = true; 
      }
    }
    if(jet_npg>=2) {
      hjet_pg2->Fill(pg_id->at(1)); 
      hjet_pg2_fract->Fill(pg_fract->at(1));
    }
    if(jet_npg>=3) {
      hjet_pg3->Fill(pg_id->at(2)); 
      hjet_pg3_fract->Fill(pg_fract->at(2));
    }
    if(jet_npg>=4) {
      hjet_pg4->Fill(pg_id->at(3)); 
      hjet_pg4_fract->Fill(pg_fract->at(3));
    }

    hcharge_lead->Fill(charge_lead); 
    hz_lead->Fill(z_lead); 

    hjet_trk_MatchdR->Fill(jet_trk_dR); 
    hjet_twr_MatchdR->Fill(jet_twr_dR); 
    
    // Energy distributions and sums

    float twr_cemc_E = 0.0; 
    float twr_ihcal_E = 0.0; 
    float twr_ohcal_E = 0.0; 

    for(unsigned int i=0; i<cemc_constituent_E->size(); i++){
      cemc_dNdR->Fill(cemc_constituent_dR->at(i)); 
      cemc_dEdR->Fill(cemc_constituent_dR->at(i),cemc_constituent_E->at(i)); 
      cemc_1rdEdR->Fill(cemc_constituent_dR->at(i),cemc_constituent_E->at(i)/cemc_constituent_dR->at(i)); 
      twr_cemc_E += cemc_constituent_E->at(i); 
    }
    for(unsigned int i=0; i<ihcal_constituent_E->size(); i++){
      ihcal_dNdR->Fill(ihcal_constituent_dR->at(i)); 
      ihcal_dEdR->Fill(ihcal_constituent_dR->at(i),ihcal_constituent_E->at(i)); 
      ihcal_1rdEdR->Fill(ihcal_constituent_dR->at(i),ihcal_constituent_E->at(i)/ihcal_constituent_dR->at(i)); 
      twr_ihcal_E += ihcal_constituent_E->at(i); 
    }
    for(unsigned int i=0; i<ohcal_constituent_E->size(); i++){
      ohcal_dNdR->Fill(ohcal_constituent_dR->at(i)); 
      ohcal_dEdR->Fill(ohcal_constituent_dR->at(i),ohcal_constituent_E->at(i)); 
      ohcal_1rdEdR->Fill(ohcal_constituent_dR->at(i),ohcal_constituent_E->at(i)/ohcal_constituent_dR->at(i)); 
      twr_ohcal_E += ohcal_constituent_E->at(i); 
    }

    float twr_jet_emfract = twr_cemc_E/(twr_cemc_E+twr_ihcal_E+twr_ohcal_E); 
    hjet_twr_emfract->Fill(twr_jet_emfract); 
    hjet_twrE_vs_emfract->Fill(twr_jet_emfract,jet_twr_ptot);

    // Properties of hadrons in jets

    for(unsigned int i=0; i<jet_had_Ztrue->size(); i++){
      had_Z_meas->Fill(jet_had_Z->at(i)); 
      had_Z_resolution->Fill(jet_had_Ztrue->at(i), (jet_had_Z->at(i)-jet_had_Ztrue->at(i))/jet_had_Ztrue->at(i)); 
    }

    // Jet properties and energy resolutions
    
    hjet_trk_Eres->Fill(jet_p_ptot,(jet_trk_ptot-jet_p_ptot)/jet_p_ptot); 
    hjet_twr_Eres->Fill(jet_p_ptot,(jet_twr_ptot-jet_p_ptot)/jet_p_ptot); 
    hjet_comb_Eres->Fill(jet_p_ptot,(jet_comb_ptot-jet_p_ptot)/jet_p_ptot);
    hjet_comb_bh_Eres->Fill(jet_p_ptot,(jet_comb_bh_ptot-jet_p_ptot)/jet_p_ptot);

    hjet_comb_neut_fract->Fill(jet_comb_neutral_fract); 
    
    if(light_quark){
      hjet_twr_Eres_quark->Fill(jet_p_ptot,(jet_twr_ptot-jet_p_ptot)/jet_p_ptot); 
      hjet_comb_Eres_quark->Fill(jet_p_ptot,(jet_comb_ptot-jet_p_ptot)/jet_p_ptot);
      hjet_comb_neut_fract_quark->Fill(jet_comb_neutral_fract); 
    }
    if(gluon){
      hjet_twr_Eres_gluon->Fill(jet_p_ptot,(jet_twr_ptot-jet_p_ptot)/jet_p_ptot); 
      hjet_comb_Eres_gluon->Fill(jet_p_ptot,(jet_comb_ptot-jet_p_ptot)/jet_p_ptot);
      hjet_comb_neut_fract_gluon->Fill(jet_comb_neutral_fract); 
    }

    hjet_comb_EtaRes->Fill(jet_p_ptot,jet_comb_eta-jet_p_eta); 
    hjet_trk_EtaRes->Fill(jet_p_ptot,jet_trk_eta-jet_p_eta); 
    hjet_twr_EtaRes->Fill(jet_p_ptot,jet_twr_eta-jet_p_eta); 

    hjet_comb_PhiRes->Fill(jet_p_ptot,jet_comb_phi-jet_p_phi); 
    hjet_trk_PhiRes->Fill(jet_p_ptot,jet_trk_phi-jet_p_phi); 
    hjet_twr_PhiRes->Fill(jet_p_ptot,jet_twr_phi-jet_p_phi); 

  }

  // Normalize

  if(n_good_jets>0){
    cemc_dNdR->Scale(1.0/(float)n_good_jets); 
    cemc_dEdR->Scale(1.0/(float)n_good_jets); 
    cemc_1rdEdR->Scale(1.0/(float)n_good_jets);
    cemc_1rdEdR->SetBinContent(1,0.0);  
    ihcal_dNdR->Scale(1.0/(float)n_good_jets); 
    ihcal_dEdR->Scale(1.0/(float)n_good_jets); 
    ihcal_1rdEdR->Scale(1.0/(float)n_good_jets); 
    ihcal_1rdEdR->SetBinContent(1,0.0); 
    ohcal_dNdR->Scale(1.0/(float)n_good_jets); 
    ohcal_dEdR->Scale(1.0/(float)n_good_jets); 
    ohcal_1rdEdR->Scale(1.0/(float)n_good_jets); 
    ohcal_1rdEdR->SetBinContent(1,0.0); 
  }

  // make some projections
  TH1D *hjet_twr_Eres_py = hjet_twr_Eres->ProjectionY(); 
  hjet_twr_Eres_py->GetXaxis()->SetTitle("(E_{tower jet} - E_{prim jet})/E_{prim jet}"); 
  TH1D *hjet_comb_Eres_py = hjet_comb_Eres->ProjectionY(); 
  hjet_comb_Eres_py->GetXaxis()->SetTitle("(E_{tower jet} - E_{prim jet})/E_{prim jet}"); 
  TH1D *hjet_comb_bh_Eres_py = hjet_comb_bh_Eres->ProjectionY(); 
  hjet_comb_bh_Eres_py->GetXaxis()->SetTitle("(E_{tower jet} - E_{prim jet})/E_{prim jet}"); 

  TH1D *hjet_twr_Eres_gluon_py = hjet_twr_Eres_gluon->ProjectionY(); 
  hjet_twr_Eres_gluon_py->GetXaxis()->SetTitle("(E_{tower jet} - E_{prim jet})/E_{prim jet}"); 
  TH1D *hjet_comb_Eres_gluon_py = hjet_comb_Eres_gluon->ProjectionY(); 
  hjet_comb_Eres_gluon_py->GetXaxis()->SetTitle("(E_{tower jet} - E_{prim jet})/E_{prim jet}"); 

  TH1D *hjet_twr_Eres_quark_py = hjet_twr_Eres_quark->ProjectionY(); 
  hjet_twr_Eres_quark_py->GetXaxis()->SetTitle("(E_{tower jet} - E_{prim jet})/E_{prim jet}"); 
  TH1D *hjet_comb_Eres_quark_py = hjet_comb_Eres_quark->ProjectionY(); 
  hjet_comb_Eres_quark_py->GetXaxis()->SetTitle("(E_{tower jet} - E_{prim jet})/E_{prim jet}"); 

  TH1D *hjet_twr_EtaRes_py = hjet_twr_EtaRes->ProjectionY();
  hjet_twr_EtaRes_py->GetXaxis()->SetTitle("#eta_{twr jet} - #eta_{prim jet}"); 
  TH1D *hjet_comb_EtaRes_py = hjet_comb_EtaRes->ProjectionY(); 
  hjet_comb_EtaRes_py->GetXaxis()->SetTitle("#eta_{twr jet} - #eta_{prim jet}"); 

  TH1D *hjet_twr_PhiRes_py = hjet_twr_PhiRes->ProjectionY();
  hjet_twr_PhiRes_py->GetXaxis()->SetTitle("#phi_{twr jet} - #phi_{prim jet}"); 
  TH1D *hjet_comb_PhiRes_py = hjet_comb_PhiRes->ProjectionY(); 
  hjet_comb_PhiRes_py->GetXaxis()->SetTitle("#phi_{twr jet} - #phi_{prim jet}"); 

  // Make 'em pretty 

  cemc_dEdR->GetXaxis()->SetTitle("r = #sqrt{#Delta#phi^{2} + #Delta#phi^{2}}"); 
  cemc_dEdR->GetYaxis()->SetTitle("#frac{1}{N} #frac{dE}{dr} (GeV)"); 
  cemc_1rdEdR->GetXaxis()->SetTitle("r = #sqrt{#Delta#phi^{2} + #Delta#phi^{2}}"); 
  cemc_1rdEdR->GetYaxis()->SetTitle("#frac{1}{N} #frac{1}{r} #frac{dE}{dr} (GeV)"); 
  
  ihcal_dEdR->GetXaxis()->SetTitle("r = #sqrt{#Delta#phi^{2} + #Delta#phi^{2}}"); 
  ihcal_dEdR->GetYaxis()->SetTitle("#frac{1}{N} #frac{dE}{dr} (GeV)"); 
  ihcal_1rdEdR->GetXaxis()->SetTitle("r = #sqrt{#Delta#phi^{2} + #Delta#phi^{2}}"); 
  ihcal_1rdEdR->GetYaxis()->SetTitle("#frac{1}{N} #frac{1}{r} #frac{dE}{dr} (GeV)"); 

  ohcal_dEdR->GetXaxis()->SetTitle("r = #sqrt{#Delta#phi^{2} + #Delta#phi^{2}}"); 
  ohcal_dEdR->GetYaxis()->SetTitle("#frac{1}{N} #frac{dE}{dr} (GeV)"); 
  ohcal_1rdEdR->GetXaxis()->SetTitle("r = #sqrt{#Delta#phi^{2} + #Delta#phi^{2}}"); 
  ohcal_1rdEdR->GetYaxis()->SetTitle("#frac{1}{N} #frac{1}{r} #frac{dE}{dr} (GeV)"); 
  
  f.Write();
  f.Close();

}
