
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

//#define ITER_START 0
//#define NUM_ITER 199
#define ITER_START 9
#define NUM_ITER 9


void readInside()
{

  // Suppress error messages from bad files
  gErrorIgnoreLevel = kFatal; 

  TChain event("event");
  TChain tracks("tracks");
  TChain prim_jets("prim_jets");
  TChain trk_jets("trk_jets");
  TChain twr_jets("twr_jets");
  TChain matched_jets("matched_jets");
  
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
 
      event.AddFile(fname);
      tracks.AddFile(fname);
      prim_jets.AddFile(fname);
      trk_jets.AddFile(fname);
      twr_jets.AddFile(fname);
      matched_jets.AddFile(fname);
      
  }

  cout << "Number of bad files = " << bad_files << endl; 
  cout << "Total simulated events = " << n_evts << endl; 
  
  // Histograms
  
  //TFile f("Constituents.root","RECREATE");

  TH1D *hjet_p_eta = new TH1D("hjet_p_eta", "Jet Pseudorapidity (primary)", 100,1.0,5.0); 
  TH1D *hjet_trk_eta = new TH1D("hjet_trk_eta", "Jet Pseudorapidity (tracks)", 100,1.0,5.0); 
  TH1D *hjet_twr_eta = new TH1D("hjet_twr_eta", "Jet Pseudorapidity (tower)", 100,1.0,5.0); 
  TH1D *hjet_p_ptot = new TH1D("hjet_p_ptot", "Jet Momentum (primary)", 100,0.0,200.0); 
  TH1D *hjet_trk_ptot = new TH1D("hjet_trk_ptot", "Jet Momentum (tracks)", 100,0.0,200.0); 
  TH1D *hjet_twr_ptot = new TH1D("hjet_twr_ptot", "Jet Momentum (tower)", 100,0.0,200.0); 

  //float nbin=36, binmin=-0.5, binmax=35.5;
  float nbin=41, binmin=-0.5, binmax=40.5;
  TH1D *hjet_p_nconstit = new TH1D("hjet_p_nconstit" , "Number of primary constituents", nbin, binmin, binmax);
  TH1D *hjet_p_nconstit_quark = new TH1D("hjet_p_nconstit_quark" , "Number of primary constituents", nbin, binmin, binmax);
  TH1D *hjet_p_nconstit_gluon = new TH1D("hjet_p_nconstit_gluon" , "Number of primary constituents", nbin, binmin, binmax);
  TH1D *hjet_p_nconstit_else = new TH1D("hjet_p_nconstit_else" , "Number of primary constituents", nbin, binmin, binmax);
  hjet_p_nconstit->Sumw2();
  hjet_p_nconstit_quark->Sumw2();
  hjet_p_nconstit_gluon->Sumw2();
  hjet_p_nconstit_else->Sumw2();
  
  TH1D *hjet_trk_nconstit = new TH1D("hjet_trk_nconstit" , "Number of track constituents", nbin, binmin, binmax);
  TH1D *hjet_twr_nconstit = new TH1D("hjet_twr_nconstit" , "Number of tower constituents", nbin, binmin, binmax);
  TH1D *hjet_npg = new TH1D("hjet_npg" , "Number of jet progenitors", 6, binmin, 5.5);
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
  
  ///////////////////////////////////////////////////
  //// tracks
  int event_tracks;
  tracks.SetBranchAddress("event",&event_tracks); 
  int gflavor;
  tracks.SetBranchAddress("gflavor",&gflavor); 
  float gpx, gpy, gpz;
  tracks.SetBranchAddress("gpx",&gpx); 
  tracks.SetBranchAddress("gpy",&gpy); 
  tracks.SetBranchAddress("gpz",&gpz); 
  float gvx, gvy, gvz;
  tracks.SetBranchAddress("gvx",&gvx); 
  tracks.SetBranchAddress("gvy",&gvy); 
  tracks.SetBranchAddress("gvz",&gvz); 
  float isprim;
  tracks.SetBranchAddress("isprim",&isprim); 
  float charge;
  tracks.SetBranchAddress("charge",&charge); 
  float px, py, pz;
  tracks.SetBranchAddress("px",&px); 
  tracks.SetBranchAddress("py",&py); 
  tracks.SetBranchAddress("pz",&pz); 
  
  ///////////////////////////////////////////////////
  //// prim_jets
  int event_prim_jets;
  prim_jets.SetBranchAddress("event",&event_prim_jets); 
  int prim_jet_constit;
  prim_jets.SetBranchAddress("jet_constit",&prim_jet_constit); 
  float prim_jet_pT, prim_jet_eta, prim_jet_phi;
  prim_jets.SetBranchAddress("jet_pT",&prim_jet_pT); 
  prim_jets.SetBranchAddress("jet_eta",&prim_jet_eta); 
  prim_jets.SetBranchAddress("jet_phi",&prim_jet_phi); 
  
  std::vector<int> *pg_prim_id = 0; 
  prim_jets.SetBranchAddress("pg_prim_id",&pg_prim_id); 
  std::vector<double> *pg_prim_fract = 0; 
  prim_jets.SetBranchAddress("pg_prim_fract",&pg_prim_fract); 
  std::vector<int> *pg_prim_status = 0; 
  prim_jets.SetBranchAddress("pg_prim_status",&pg_prim_status); 
  
  ///////////////////////////////////////////////////
  //// trk_jets
  int event_trk_jets;
  trk_jets.SetBranchAddress("event",&event_trk_jets); 
  int trk_jet_constit;
  trk_jets.SetBranchAddress("jet_constit",&trk_jet_constit); 
  float trk_jet_pT, trk_jet_eta, trk_jet_phi;
  trk_jets.SetBranchAddress("jet_pT",&trk_jet_pT); 
  trk_jets.SetBranchAddress("jet_eta",&trk_jet_eta); 
  trk_jets.SetBranchAddress("jet_phi",&trk_jet_phi); 
  
  float trk_z_lead;
  trk_jets.SetBranchAddress("z_lead",&trk_z_lead); 
  int trk_charge_lead;
  trk_jets.SetBranchAddress("charge_lead",&trk_charge_lead); 
  
  std::vector<int> *pg_trk_id = 0; 
  trk_jets.SetBranchAddress("pg_trk_id",&pg_trk_id); 
  std::vector<double> *pg_trk_fract = 0; 
  trk_jets.SetBranchAddress("pg_trk_fract",&pg_trk_fract); 
  std::vector<int> *pg_trk_status = 0; 
  trk_jets.SetBranchAddress("pg_trk_status",&pg_trk_status); 
  ///////////////////////////////////////////////////
  //// twr_jets
  int event_twr_jets;
  twr_jets.SetBranchAddress("event",&event_twr_jets); 
  int twr_jet_constit;
  twr_jets.SetBranchAddress("jet_constit",&twr_jet_constit); 
  float twr_jet_pT, twr_jet_eta, twr_jet_phi;
  twr_jets.SetBranchAddress("jet_pT",&twr_jet_pT); 
  twr_jets.SetBranchAddress("jet_eta",&twr_jet_eta); 
  twr_jets.SetBranchAddress("jet_phi",&twr_jet_phi); 
  
  float twr_z_lead;
  twr_jets.SetBranchAddress("z_lead",&twr_z_lead); 
  int twr_charge_lead;
  twr_jets.SetBranchAddress("charge_lead",&twr_charge_lead); 
  
  std::vector<int> *pg_twr_id = 0; 
  twr_jets.SetBranchAddress("pg_twr_id",&pg_twr_id); 
  std::vector<double> *pg_twr_fract = 0; 
  twr_jets.SetBranchAddress("pg_twr_fract",&pg_twr_fract); 
  std::vector<int> *pg_twr_status = 0; 
  twr_jets.SetBranchAddress("pg_twr_status",&pg_twr_status); 
  ///////////////////////////////////////////////////
  //// matched_jets
  int event_matched_jets;
  float jet_p_ptot, jet_trk_ptot, jet_twr_ptot; 
  matched_jets.SetBranchAddress("event",&event_matched_jets); 
  matched_jets.SetBranchAddress("jet_p_ptot",&jet_p_ptot); 
  matched_jets.SetBranchAddress("jet_trk_ptot",&jet_trk_ptot); 
  matched_jets.SetBranchAddress("jet_twr_ptot",&jet_twr_ptot); 

  float jet_p_pT, jet_trk_pT, jet_twr_pT; 
  matched_jets.SetBranchAddress("jet_p_pT",&jet_p_pT); 
  matched_jets.SetBranchAddress("jet_trk_pT",&jet_trk_pT); 
  matched_jets.SetBranchAddress("jet_twr_pT",&jet_twr_pT); 

  float jet_p_eta, jet_trk_eta, jet_twr_eta; 
  matched_jets.SetBranchAddress("jet_p_eta",&jet_p_eta); 
  matched_jets.SetBranchAddress("jet_trk_eta",&jet_trk_eta); 
  matched_jets.SetBranchAddress("jet_twr_eta",&jet_twr_eta); 

  float jet_p_phi, jet_trk_phi, jet_twr_phi; 
  matched_jets.SetBranchAddress("jet_p_phi",&jet_p_phi); 
  matched_jets.SetBranchAddress("jet_trk_phi",&jet_trk_phi); 
  matched_jets.SetBranchAddress("jet_twr_phi",&jet_twr_phi); 

  int jet_p_nconstit, jet_trk_nconstit, jet_twr_nconstit; 
  matched_jets.SetBranchAddress("jet_p_nconstit",&jet_p_nconstit); 
  matched_jets.SetBranchAddress("jet_trk_nconstit",&jet_trk_nconstit); 
  matched_jets.SetBranchAddress("jet_twr_nconstit",&jet_twr_nconstit); 

  // These variables are for the track jet

  std::vector<int> *pg_id = 0; 
  matched_jets.SetBranchAddress("pg_twr_id",&pg_id); 
  std::vector<double> *pg_fract = 0; 
  matched_jets.SetBranchAddress("pg_twr_fract",&pg_fract); 
  std::vector<int> *pg_status = 0; 
  matched_jets.SetBranchAddress("pg_twr_status",&pg_status); 

  float z_lead; 
  matched_jets.SetBranchAddress("trk_z_lead",&z_lead); 

  int charge_lead; 
  matched_jets.SetBranchAddress("trk_charge_lead",&charge_lead); 

  float jet_trk_dR, jet_twr_dR; 
  matched_jets.SetBranchAddress("jet_trk_dR",&jet_trk_dR); 
  matched_jets.SetBranchAddress("jet_twr_dR",&jet_twr_dR); 

  float jet_comb_ptot; 
  matched_jets.SetBranchAddress("jet_comb_ptot",&jet_comb_ptot); 
  float jet_comb_pT; 
  matched_jets.SetBranchAddress("jet_comb_pT",&jet_comb_pT); 
  float jet_comb_eta; 
  matched_jets.SetBranchAddress("jet_comb_eta",&jet_comb_eta); 
  float jet_comb_phi; 
  matched_jets.SetBranchAddress("jet_comb_phi",&jet_comb_phi); 
  float jet_comb_neutral_fract; 
  matched_jets.SetBranchAddress("jet_comb_neutral_fract",&jet_comb_neutral_fract); 

  float jet_comb_bh_ptot; 
  matched_jets.SetBranchAddress("jet_comb_bh_ptot",&jet_comb_bh_ptot); 

  // Jet hadron properties
  
  std::vector<double> *jet_had_Z = 0; 
  std::vector<double> *jet_had_Ztrue = 0; 
  matched_jets.SetBranchAddress("jet_had_Z",&jet_had_Z); 
  matched_jets.SetBranchAddress("jet_had_Ztrue",&jet_had_Ztrue); 

  // Tower Jet constituents

  std::vector<double> *cemc_constituent_E = 0; 
  std::vector<double> *cemc_constituent_dR = 0; 
  std::vector<double> *ihcal_constituent_E = 0; 
  std::vector<double> *ihcal_constituent_dR = 0; 
  std::vector<double> *ohcal_constituent_E = 0; 
  std::vector<double> *ohcal_constituent_dR = 0; 

  matched_jets.SetBranchAddress("cemc_constituent_E",&cemc_constituent_E); 
  matched_jets.SetBranchAddress("cemc_constituent_dR",&cemc_constituent_dR); 
  matched_jets.SetBranchAddress("ihcal_constituent_E",&ihcal_constituent_E); 
  matched_jets.SetBranchAddress("ihcal_constituent_dR",&ihcal_constituent_dR); 
  matched_jets.SetBranchAddress("ohcal_constituent_E",&ohcal_constituent_E); 
  matched_jets.SetBranchAddress("ohcal_constituent_dR",&ohcal_constituent_dR); 

  ///////////////////////////////////////////////////
  unsigned long long ntrktr = tracks.GetEntries();
  cout << "Examining " << ntrktr << " tracks entries ..." << endl; 
  
  TCanvas* c_gflavor = new TCanvas("c_gflavor","c_gflavor",600,600);
  gPad->SetRightMargin(2.5);
  gStyle->SetOptStat(1);
  TH1D* h1D_gflavor = new TH1D("h1D_gflavor","h1D_gflavor",8001,-4000,4000);
//  TH2D* h2D_01 = new TH2D("h2D_01",";#eta;#phi",50,-1.2,1.2,50,0,6.28);
  TH2D* h2D_01 = new TH2D("h2D_01",";#eta;#phi",200,-1.2,1.2,200,0,6.4);

  double gptot=-9999;
  double gpt=-9999;
  double geta=-9999;
  double gphi=-9999;
  
  double ptot=-9999;
  double pt=-9999;
  double eta=-9999;
  double phi=-9999;
  
  for(unsigned long long itrktr=0; itrktr<ntrktr; itrktr++){

//    cout << "xia" << endl; 
    tracks.GetEvent(itrktr);
    if (event_tracks==1){
      gptot = sqrt(pow(gpx,2)+pow(gpy,2)+pow(gpz,2));
      gpt = sqrt(pow(gpx,2)+pow(gpy,2));
      geta = getEta(gpt,gpz); 
      gphi = XYtoPhi_02PI(gpx,gpy);
      
      ptot = sqrt(pow(px,2)+pow(py,2)+pow(pz,2));
      pt = sqrt(pow(px,2)+pow(py,2));
      eta = getEta(pt,pz); 
      phi = XYtoPhi_02PI(px,py);

      h1D_gflavor->Fill(gflavor);
//      h2D_01->Fill(geta,gphi);     
      //if (gflavor<-1)
      //h2D_01->Fill(geta,gphi,gptot);     
      //h2D_01->Fill(geta,gphi,gflavor);     
      //h2D_01->Fill(geta,gphi,gvz);     
      //h2D_01->Fill(geta,gphi,isprim);     
      //h2D_01->Fill(geta,gphi,charge);     
      
      // reco track
      h2D_01->Fill(eta,phi,ptot);     
    } 
  }
  //h1D_gflavor->Draw();
  //h2D_01->Draw("colz");
  //h2D_01->Draw("");
  //h2D_01->Draw("lego2");
  
  ///////////////////////////////////////////////////
  unsigned long long npJtr = prim_jets.GetEntries();
  cout << "Examining " << npJtr << " prim_jets entries ..." << endl; 
  
  TCanvas* c_prim_id = new TCanvas("c_prim_id","c_prim_id",600,600);
  gPad->SetRightMargin(2.5);
  //gStyle->SetOptStat(1);
  TH1D* h1D_prim_id = new TH1D("h1D_prim_id","h1D_prim_id",8001,-4000,4000);
  TH1D *hjet_prim_nconstit = new TH1D("hjet_prim_nconstit" , "Number of primary jets constituents", nbin, binmin, binmax);
  //TH2D* h2D_01 = new TH2D("h2D_01",";#eta;#phi",50,-1.2,1.2,50,0,6.28);
  
  int prim_jets_npg = 9999;
  for(unsigned long long ipJtr=0; ipJtr<npJtr; ipJtr++){
    prim_jets.GetEvent(ipJtr); 
    if (event_prim_jets==1){
      cout << "prim_jet_constit!! = " << prim_jet_constit << endl; 
      cout << "prim_jet_pT!! = " << prim_jet_pT << endl; 
      cout << "prim_jet_eta!! = " << prim_jet_eta << endl; 
      cout << "prim_jet_phi!! = " << prim_jet_phi << endl; 
      //cout << "*** ipJtr = " << ipJtr << endl;
      prim_jets_npg = pg_prim_id->size();
      //cout << " pg size: "<< prim_jets_npg << endl;
      for (int ipg=0; ipg<prim_jets_npg; ipg++) {
        //cout << "   pg_prim_id:" << pg_prim_id->at(ipg) << endl;
        //cout << "   pg_prim_fract:" << pg_prim_fract->at(ipg) << endl;
        //cout << "   pg_prim_status:" << pg_prim_status->at(ipg) << endl;
        //cout << "" << endl;
      }
    }
  }
  
  ///////////////////////////////////////////////////
  unsigned long long ntrkJtr = trk_jets.GetEntries();
  cout << "Examining " << ntrkJtr << " trk_jets entries ..." << endl; 
  
  TCanvas* c_trk_id = new TCanvas("c_trk_id","c_trk_id",600,600);
  gPad->SetRightMargin(2.5);
  //gStyle->SetOptStat(1);
  TH1D* h1D_trk_id = new TH1D("h1D_trk_id","h1D_trk_id",8001,-4000,4000);
  TH1D *hjet_trk_nconstit = new TH1D("hjet_trk_nconstit" , "Number of track jets constituents", nbin, binmin, binmax);
  //TH2D* h2D_01 = new TH2D("h2D_01",";#eta;#phi",50,-1.2,1.2,50,0,6.28);
  
  int trk_jets_npg = 9999;
  for(unsigned long long itrkJtr=0; itrkJtr<ntrkJtr; itrkJtr++){
    trk_jets.GetEvent(itrkJtr); 
    if (event_trk_jets==1){
      cout << "trk_jet_constit!! = " << trk_jet_constit << endl; 
      cout << "trk_jet_pT!! = " << trk_jet_pT << endl; 
      cout << "trk_jet_eta!! = " << trk_jet_eta << endl; 
      cout << "trk_jet_phi!! = " << trk_jet_phi << endl; 
      cout << "trk_z_lead!! = " << trk_z_lead << endl; 
      cout << "trk_charge_lead!! = " << trk_charge_lead << endl; 
      cout << "*** ipJtr = " << ipJtr << endl;
      trk_jets_npg = pg_trk_id->size();
      cout << " pg size: "<< trk_jets_npg << endl;
      for (int ipg=0; ipg<trk_jets_npg; ipg++) {
        cout << "   pg_trk_id:" << pg_trk_id->at(ipg) << endl;
        cout << "   pg_trk_fract:" << pg_trk_fract->at(ipg) << endl;
        cout << "   pg_trk_status:" << pg_trk_status->at(ipg) << endl;
        cout << "" << endl;
      }
    }
  }
  
  ///////////////////////////////////////////////////
  unsigned long long ntwrJtr = twr_jets.GetEntries();
  cout << "Examining " << ntwrJtr << " twr_jets entries ..." << endl; 
  
  TCanvas* c_twr_id = new TCanvas("c_twr_id","c_twr_id",600,600);
  gPad->SetRightMargin(2.5);
  //gStyle->SetOptStat(1);
  TH1D* h1D_twr_id = new TH1D("h1D_twr_id","h1D_twr_id",8001,-4000,4000);
  TH1D *hjet_twr_nconstit = new TH1D("hjet_twr_nconstit" , "Number of track jets constituents", nbin, binmin, binmax);
  //TH2D* h2D_01 = new TH2D("h2D_01",";#eta;#phi",50,-1.2,1.2,50,0,6.28);
  
  int twr_jets_npg = 9999;
  for(unsigned long long itwrJtr=0; itwrJtr<ntwrJtr; itwrJtr++){
    twr_jets.GetEvent(itwrJtr); 
    if (event_twr_jets==1){
      cout << "twr_jet_constit!! = " << twr_jet_constit << endl; 
      cout << "twr_jet_pT!! = " << twr_jet_pT << endl; 
      cout << "twr_jet_eta!! = " << twr_jet_eta << endl; 
      cout << "twr_jet_phi!! = " << twr_jet_phi << endl; 
      cout << "twr_z_lead!! = " << twr_z_lead << endl; 
      cout << "twr_charge_lead!! = " << twr_charge_lead << endl; 
      cout << "*** ipJtr = " << ipJtr << endl;
      twr_jets_npg = pg_twr_id->size();
      cout << " pg size: "<< twr_jets_npg << endl;
      for (int ipg=0; ipg<twr_jets_npg; ipg++) {
        cout << "   pg_twr_id:" << pg_twr_id->at(ipg) << endl;
        cout << "   pg_twr_fract:" << pg_twr_fract->at(ipg) << endl;
        cout << "   pg_twr_status:" << pg_twr_status->at(ipg) << endl;
        cout << "" << endl;
      }
    }
  }
  
  
  return;
  
  ///////////////////////////////////////////////////
  
  unsigned long long nsim = matched_jets.GetEntries();
  int n_good_matched_jets = 0; 

  cout << "Examining " << nsim << " matched_jets entries ..." << endl; 
 
  for(unsigned long long isim=0; isim<nsim; isim++){

    matched_jets.GetEvent(isim);
 
    cout << "isim = " << isim << endl;
    cout << " * event_matched_jets = " << event_matched_jets << endl;
    // cuts for jet sources

    if( fabs(jet_trk_eta)>0.6 ) continue; 
    if( jet_p_ptot<50.0) continue; 

    n_good_matched_jets++; 

    int jet_npg = pg_id->size(); 
    hjet_p_nconstit->Fill(jet_p_nconstit); 
    
    bool is_quark = false; 
    bool is_gluon = false;
    
    hjet_npg->Fill(jet_npg);
    if(jet_npg>0){
      hjet_pg1->Fill(pg_id->at(0)); 
      hjet_pg1_fract->Fill(pg_fract->at(0));
      if(pg_fract->at(0)>0.5){
        if( (pg_id->at(0)==1) || (pg_id->at(0)==2) ) 
	  is_quark = true; 
	else if (pg_id->at(0)==21) 
	  is_gluon = true; 
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
    
    if(is_quark){
      hjet_p_nconstit_quark->Fill(jet_p_nconstit); 
    }
    if(is_gluon){
      hjet_p_nconstit_gluon->Fill(jet_p_nconstit); 
    }
    if(!is_quark && !is_gluon){
      hjet_p_nconstit_else->Fill(jet_p_nconstit); 
    }
   
    
/*
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

    // Properties of hadrons in matched_jets

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
*/
  }
    
    cout << "Total:" << hjet_p_nconstit->GetEntries() << endl;
    cout << "Quark:" << hjet_p_nconstit_quark->GetEntries() << endl;
    cout << "Gluon:" << hjet_p_nconstit_gluon->GetEntries() << endl;
    cout << "Else:" << hjet_p_nconstit_else->GetEntries() << endl;
    
    hjet_p_nconstit->SetLineColor(kGray+3);
    hjet_p_nconstit->SetMarkerColor(kGray+3);
    hjet_p_nconstit->SetMarkerSize(0);
    hjet_p_nconstit->SetLineStyle(7);
    hjet_p_nconstit_quark->SetLineColor(kPink+3);
    hjet_p_nconstit_quark->SetMarkerColor(kPink+3);
    hjet_p_nconstit_quark->SetMarkerSize(0);
    hjet_p_nconstit_quark->SetFillColor(kPink+2);
    hjet_p_nconstit_quark->SetFillStyle(3445);
    hjet_p_nconstit_gluon->SetLineColor(kAzure+4);
    hjet_p_nconstit_gluon->SetMarkerColor(kAzure+4);
    hjet_p_nconstit_gluon->SetMarkerSize(0);
    hjet_p_nconstit_gluon->SetFillColor(kAzure+5);
    hjet_p_nconstit_gluon->SetFillStyle(3454);
    hjet_p_nconstit_else->SetLineColor(kSpring+3);
    hjet_p_nconstit_else->SetMarkerColor(kSpring+3);
    hjet_p_nconstit_else->SetMarkerSize(0);
    hjet_p_nconstit_else->SetFillColor(kSpring+9);
    hjet_p_nconstit_else->SetFillStyle(3003);
    
//    TCanvas* c_multi_01 = new TCanvas("c_multi_01","c_multi_01",600,600);
    c_multi_01->cd();
    /*
    hjet_p_nconstit->Draw("hist e");
    hjet_p_nconstit_quark->Draw("hist e same");
    hjet_p_nconstit_gluon->Draw("hist e same");
    hjet_p_nconstit_else->Draw("hist e same");
    */
    hjet_p_nconstit->GetXaxis()->SetTitle("# of constituents");
    hjet_p_nconstit->GetXaxis()->CenterTitle();
    hjet_p_nconstit->Draw("hist");
    hjet_p_nconstit_quark->Draw("hist same");
    hjet_p_nconstit_gluon->Draw("hist same");
    hjet_p_nconstit_else->Draw("hist same");

//    TCanvas* c_multi_02 = new TCanvas("c_multi_02","c_multi_02",600,600);
    c_multi_02->cd();
    
    TH1D* norm_hjet_p_nconstit = (TH1D*) hjet_p_nconstit->Clone();
    TH1D* norm_hjet_p_nconstit_quark = (TH1D*) hjet_p_nconstit_quark->Clone();
    TH1D* norm_hjet_p_nconstit_gluon = (TH1D*) hjet_p_nconstit_gluon->Clone();
    TH1D* norm_hjet_p_nconstit_else = (TH1D*) hjet_p_nconstit_else->Clone();
    
    norm_hjet_p_nconstit->Scale(1./norm_hjet_p_nconstit->Integral());
    norm_hjet_p_nconstit_quark->Scale(1./norm_hjet_p_nconstit_quark->Integral());
    norm_hjet_p_nconstit_gluon->Scale(1./norm_hjet_p_nconstit_gluon->Integral());
    norm_hjet_p_nconstit_else->Scale(1./norm_hjet_p_nconstit_else->Integral());

    /*
    norm_hjet_p_nconstit_quark->Draw("hist e");
    norm_hjet_p_nconstit_gluon->Draw("hist e same");
    */
    norm_hjet_p_nconstit_quark->GetXaxis()->SetTitle("# of constituents");
    norm_hjet_p_nconstit_quark->GetXaxis()->CenterTitle();
    norm_hjet_p_nconstit_quark->Draw("hist");
    norm_hjet_p_nconstit_gluon->Draw("hist same");
    
    //return;


/*
  // Normalize

  if(n_good_matched_jets>0){
    cemc_dNdR->Scale(1.0/(float)n_good_matched_jets); 
    cemc_dEdR->Scale(1.0/(float)n_good_matched_jets); 
    cemc_1rdEdR->Scale(1.0/(float)n_good_matched_jets);
    cemc_1rdEdR->SetBinContent(1,0.0);  
    ihcal_dNdR->Scale(1.0/(float)n_good_matched_jets); 
    ihcal_dEdR->Scale(1.0/(float)n_good_matched_jets); 
    ihcal_1rdEdR->Scale(1.0/(float)n_good_matched_jets); 
    ihcal_1rdEdR->SetBinContent(1,0.0); 
    ohcal_dNdR->Scale(1.0/(float)n_good_matched_jets); 
    ohcal_dEdR->Scale(1.0/(float)n_good_matched_jets); 
    ohcal_1rdEdR->Scale(1.0/(float)n_good_matched_jets); 
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
*/  

}

double XYtoPhi(double x, double y)
{
  // -Pi to +Pi
  Double_t phi = atan2(y,x);
  if(phi<-TMath::Pi()) phi += TMath::TwoPi();
  if(phi>=TMath::Pi()) phi -= TMath::TwoPi();
  return phi;
}

double XYtoPhi_02PI(double x, double y)
{
  // 0 to 2pi
  Double_t phi = atan2(y,x);
  if(phi<0.0) phi += TMath::TwoPi();
  if(phi>=TMath::TwoPi()) phi -= TMath::TwoPi();
  return phi;
}

double getEta(double pt, double pz)
{
  float theta = XYtoPhi(pz,pt);
  float eta = -log(tan(theta/2.0));
  return eta;
}

