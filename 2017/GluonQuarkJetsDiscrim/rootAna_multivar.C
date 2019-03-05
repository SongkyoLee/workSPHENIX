
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

////////////// ihcalType
//
// SS310_OUTPUT
// AL_OUTPUT
// AL_NOIHCAL_OUTPUT
// SS310_FRAME_OUTPUT
//
// SS310_RSCALE_OUTPUT
// AL_RSCALE_OUTPUT
// AL_NOIHCAL_RSCALE_OUTPUT
// SS310_FRAME_RSCALE3_OUTPUT
// SS310_NOIHCAL_RSCALE2_OUTPUT 
//
//////////////

void rootAna_multivar(TString ihcalType = "SS310_RSCALE_OUTPUT")
{

  //// Suppress error messages from bad files
  gErrorIgnoreLevel = kFatal; 

  TChain jets("matched_jets");
  
  int n_evts = 0; 
  int bad_files = 0; 

  for(int j=ITER_START; j<=NUM_ITER; j++){
    
    //TString fname = "./jet_analysis_";
    //fname += j; 
    //fname += ".root";
    TString fname = Form("/sphenix/user/lajoie/sPHENIX/jet_simulations/%s/jet_analysis_%d.root",ihcalType.Data(),j);
    //cout << fname << endl; 

    //// skip bad files
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
  
  ////////////////////////////////////////////////////////
  //// Histograms
  
  //gStyle->SetOptStat(1); 
  float nprimbin=41, primbinmin=-0.5, primbinmax=40.5;
  float ntrkbin=19, trkbinmin=-0.5, trkbinmax=18.5;
  float ntwrbin=131, twrbinmin=-0.5, twrbinmax=130.5;
  
  ////////////////////////////////////////////////////////
  //// Check correlations
  TH2D* h2d_prim_trk = new TH2D("h2d_prim_trk",";# of constituents in primary jets;# of constituents in track jets",nprimbin,primbinmin, primbinmax, ntrkbin, trkbinmin, trkbinmax);
  TH2D* h2d_prim_twr = new TH2D("h2d_prim_twr",";# of constituents in primary jets;# of constituents in tower jets",nprimbin,primbinmin, primbinmax, ntwrbin, twrbinmin, twrbinmax);
 
  ////////////////////////////////////////////////////////
  //// nconstitut - primary jets 
  TH1D *hjet_p_nconstit = new TH1D("hjet_p_nconstit" , "Number of primary constituents", nprimbin, primbinmin, primbinmax);
  TH1D *hjet_p_nconstit_quark = new TH1D("hjet_p_nconstit_quark" , "Number of primary constituents", nprimbin, primbinmin, primbinmax);
  TH1D *hjet_p_nconstit_gluon = new TH1D("hjet_p_nconstit_gluon" , "Number of primary constituents", nprimbin, primbinmin, primbinmax);
  TH1D *hjet_p_nconstit_else = new TH1D("hjet_p_nconstit_else" , "Number of primary constituents", nprimbin, primbinmin, primbinmax);
  hjet_p_nconstit->Sumw2();
  hjet_p_nconstit_quark->Sumw2();
  hjet_p_nconstit_gluon->Sumw2();
  hjet_p_nconstit_else->Sumw2();
  
  //// nconstitut - track jets 
  TH1D *hjet_trk_nconstit = new TH1D("hjet_trk_nconstit" , "Number of track constituents", ntrkbin, trkbinmin, trkbinmax);
  TH1D *hjet_trk_nconstit_quark = new TH1D("hjet_trk_nconstit_quark" , "Number of tracks constituents", ntrkbin, trkbinmin, trkbinmax);
  TH1D *hjet_trk_nconstit_gluon = new TH1D("hjet_trk_nconstit_gluon" , "Number of tracks constituents", ntrkbin, trkbinmin, trkbinmax);
  TH1D *hjet_trk_nconstit_else = new TH1D("hjet_trk_nconstit_else" , "Number of tracks constituents", ntrkbin, trkbinmin, trkbinmax);
  hjet_trk_nconstit->Sumw2();
  hjet_trk_nconstit_quark->Sumw2();
  hjet_trk_nconstit_gluon->Sumw2();
  hjet_trk_nconstit_else->Sumw2();
  
  //// nconstitut - tower jets 
  TH1D *hjet_twr_nconstit = new TH1D("hjet_twr_nconstit" , "Number of tower constituents", ntwrbin, twrbinmin, twrbinmax);
  TH1D *hjet_twr_nconstit_quark = new TH1D("hjet_twr_nconstit_quark" , "Number of tower constituents", ntwrbin, twrbinmin, twrbinmax);
  TH1D *hjet_twr_nconstit_gluon = new TH1D("hjet_twr_nconstit_gluon" , "Number of tower constituents", ntwrbin, twrbinmin, twrbinmax);
  TH1D *hjet_twr_nconstit_else = new TH1D("hjet_twr_nconstit_else" , "Number of tower constituents", ntwrbin, twrbinmin, twrbinmax);
  hjet_twr_nconstit->Sumw2();
  hjet_twr_nconstit_quark->Sumw2();
  hjet_twr_nconstit_gluon->Sumw2();
  hjet_twr_nconstit_else->Sumw2();
  
  ////////////////////////////////////////////////////////
  float nprim2bin=100, prim2binmin=0., prim2binmax=1.0;
  float ntrk2bin=100, trk2binmin=0., trk2binmax=1.0;
  float ntwr2bin=100, twr2binmin=0., twr2binmax=1.0;
  
  //// pTD - primary jets 
  TH1D *hjet_p_pTD = new TH1D("hjet_p_pTD" , "pTD of primary jets", nprim2bin, prim2binmin, prim2binmax);
  TH1D *hjet_p_pTD_quark = new TH1D("hjet_p_pTD_quark" , "pTD of primary jets", nprim2bin, prim2binmin, prim2binmax);
  TH1D *hjet_p_pTD_gluon = new TH1D("hjet_p_pTD_gluon" , "pTD of primary jets", nprim2bin, prim2binmin, prim2binmax);
  TH1D *hjet_p_pTD_else = new TH1D("hjet_p_pTD_else" , "pTD of primary jets", nprim2bin, prim2binmin, prim2binmax);
  hjet_p_pTD->Sumw2();
  hjet_p_pTD_quark->Sumw2();
  hjet_p_pTD_gluon->Sumw2();
  hjet_p_pTD_else->Sumw2();
  
  //// pTD - track jets 
  TH1D *hjet_trk_pTD = new TH1D("hjet_trk_pTD" , "pTD of track jets", ntrk2bin, trk2binmin, trk2binmax);
  TH1D *hjet_trk_pTD_quark = new TH1D("hjet_trk_pTD_quark" , "pTD of track jets", ntrk2bin, trk2binmin, trk2binmax);
  TH1D *hjet_trk_pTD_gluon = new TH1D("hjet_trk_pTD_gluon" , "pTD of track jets", ntrk2bin, trk2binmin, trk2binmax);
  TH1D *hjet_trk_pTD_else = new TH1D("hjet_trk_pTD_else" , "pTD of track jets", ntrk2bin, trk2binmin, trk2binmax);
  hjet_trk_pTD->Sumw2();
  hjet_trk_pTD_quark->Sumw2();
  hjet_trk_pTD_gluon->Sumw2();
  hjet_trk_pTD_else->Sumw2();

  //// pTD - tower jets 
  TH1D *hjet_twr_pTD = new TH1D("hjet_twr_pTD" , "pTD of tower jets", ntwr2bin, twr2binmin, twr2binmax);
  TH1D *hjet_twr_pTD_quark = new TH1D("hjet_twr_pTD_quark" , "pTD of tower jets", ntwr2bin, twr2binmin, twr2binmax);
  TH1D *hjet_twr_pTD_gluon = new TH1D("hjet_twr_pTD_gluon" , "pTD of tower jets", ntwr2bin, twr2binmin, twr2binmax);
  TH1D *hjet_twr_pTD_else = new TH1D("hjet_twr_pTD_else" , "pTD of tower jets", ntwr2bin, twr2binmin, twr2binmax);
  hjet_twr_pTD->Sumw2();
  hjet_twr_pTD_quark->Sumw2();
  hjet_twr_pTD_gluon->Sumw2();
  hjet_twr_pTD_else->Sumw2();

  ////////////////////////////////////////////////////////
  
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

  //// trace back progenitors
  std::vector<int> *pg_prim_id = 0; 
  jets.SetBranchAddress("pg_prim_id",&pg_prim_id); 
  std::vector<double> *pg_prim_fract = 0; 
  jets.SetBranchAddress("pg_prim_fract",&pg_prim_fract); 
  std::vector<int> *pg_prim_status = 0; 
  jets.SetBranchAddress("pg_prim_status",&pg_prim_status); 
  
  std::vector<int> *pg_trk_id = 0; 
  jets.SetBranchAddress("pg_trk_id",&pg_trk_id); 
  std::vector<double> *pg_trk_fract = 0; 
  jets.SetBranchAddress("pg_trk_fract",&pg_trk_fract); 
  std::vector<int> *pg_trk_status = 0; 
  jets.SetBranchAddress("pg_trk_status",&pg_trk_status); 

  std::vector<int> *pg_twr_id = 0; 
  jets.SetBranchAddress("pg_twr_id",&pg_twr_id); 
  std::vector<double> *pg_twr_fract = 0; 
  jets.SetBranchAddress("pg_twr_fract",&pg_twr_fract); 
  std::vector<int> *pg_twr_status = 0; 
  jets.SetBranchAddress("pg_twr_status",&pg_twr_status); 

  //// pT of constituents (for pT_D calculation)
  std::vector<double> *prim_constituent_pT = 0;
  std::vector<double> *trk_constituent_pT = 0;
  std::vector<double> *cemc_constituent_pT = 0;
  std::vector<double> *ihcal_constituent_pT = 0;
  std::vector<double> *ohcal_constituent_pT = 0;
  jets.SetBranchAddress("prim_constituent_pT",&prim_constituent_pT);
  jets.SetBranchAddress("trk_constituent_pT",&trk_constituent_pT);
  jets.SetBranchAddress("cemc_constituent_pT",&cemc_constituent_pT);
  jets.SetBranchAddress("ihcal_constituent_pT",&ihcal_constituent_pT);
  jets.SetBranchAddress("ohcal_constituent_pT",&ohcal_constituent_pT);
  
  ////////////////////////////////////////////////////////
 
  //// we only check matched jets!! 
  unsigned long long nsim = jets.GetEntries();
  int n_good_jets = 0; 

  cout << "Examining " << nsim << " jets entries ..." << endl; 
 
  for(unsigned long long isim=0; isim<nsim; isim++){

    jets.GetEvent(isim);
 
    //// cuts for jet sources
    if( fabs(jet_trk_eta)>0.6 ) continue; 
    if( jet_p_ptot<50.0) continue; 
    n_good_jets++; 

    int jet_prim_npg = pg_prim_id->size(); 
    int jet_trk_npg = pg_trk_id->size(); 
    int jet_twr_npg = pg_twr_id->size(); 
//    if (jet_twr_npg > jet_prim_npg){
//      cout << "***** jet_prim_npg: " << jet_prim_npg<<endl;
//      cout << "***** jet_trk_npg: " << jet_trk_npg<<endl;
//      cout << "***** jet_twr_npg: " << jet_twr_npg<<endl;
//      cout << "" << endl;
//    }
    bool is_prim_quark = false; 
    bool is_prim_gluon = false;
    bool is_trk_quark = false; 
    bool is_trk_gluon = false;
    bool is_twr_quark = false; 
    bool is_twr_gluon = false;
    
    //// Progenitors are already ordered in terms of fraction?!
    if(jet_prim_npg>0){
      if(pg_prim_fract->at(0)>0.5){
        if( (pg_prim_id->at(0)==1) || (pg_prim_id->at(0)==2) ) 
	  is_prim_quark = true; 
	else if (pg_prim_id->at(0)==21) 
	  is_prim_gluon = true; 
      }
    }
    if(jet_trk_npg>0){
      if(pg_trk_fract->at(0)>0.5){
        if( (pg_trk_id->at(0)==1) || (pg_trk_id->at(0)==2) ) 
	  is_trk_quark = true; 
	else if (pg_trk_id->at(0)==21) 
	  is_trk_gluon = true; 
      }
    }
    if(jet_twr_npg>0){
      if(pg_twr_fract->at(0)>0.5){
        if( (pg_twr_id->at(0)==1) || (pg_twr_id->at(0)==2) ) 
	  is_twr_quark = true; 
	else if (pg_twr_id->at(0)==21) 
	  is_twr_gluon = true; 
      }
    }
    
    ////////////////////////////////////////////////////////
    ////////// Check correlataion
    h2d_prim_trk->Fill(jet_p_nconstit,jet_trk_nconstit);
    h2d_prim_twr->Fill(jet_p_nconstit,jet_twr_nconstit);
    
    ////////////////////////////////////////////////////////
    ////////// 1) constituent
    hjet_p_nconstit->Fill(jet_p_nconstit); 
    hjet_trk_nconstit->Fill(jet_trk_nconstit); 
    hjet_twr_nconstit->Fill(jet_twr_nconstit); 
    
    if(is_prim_quark){ hjet_p_nconstit_quark->Fill(jet_p_nconstit);}
    else if(is_prim_gluon){ hjet_p_nconstit_gluon->Fill(jet_p_nconstit);}
    else { hjet_p_nconstit_else->Fill(jet_p_nconstit); } 
    
    if(is_trk_quark){ hjet_trk_nconstit_quark->Fill(jet_trk_nconstit);}
    else if(is_trk_gluon){ hjet_trk_nconstit_gluon->Fill(jet_trk_nconstit);}
    else { hjet_trk_nconstit_else->Fill(jet_trk_nconstit); } 
    
    if(is_twr_quark){ hjet_twr_nconstit_quark->Fill(jet_twr_nconstit);}
    else if(is_twr_gluon){ hjet_twr_nconstit_gluon->Fill(jet_twr_nconstit);}
    else { hjet_twr_nconstit_else->Fill(jet_twr_nconstit); } 
   
    ////////////////////////////////////////////////////////
    //// 2) pT_D calculation
    double pTsum_prim = 0;
    double pTsum_trk = 0;
    double pTsum_twr = 0;
    double pTsqsum_prim = 0;
    double pTsqsum_trk = 0;
    double pTsqsum_twr = 0;
    double pTD_prim = 0;
    double pTD_trk = 0;
    double pTD_twr = 0;
    //// prim jets
    for (unsigned int ic=0; ic<prim_constituent_pT->size(); ic++){
      pTsum_prim += prim_constituent_pT->at(ic);   
      pTsqsum_prim += pow(prim_constituent_pT->at(ic),2);
    }
    pTD_prim = sqrt(pTsqsum_prim)/pTsum_prim;  
    hjet_p_pTD->Fill(pTD_prim); 
    if(is_prim_quark) { hjet_p_pTD_quark->Fill(pTD_prim);} 
    else if(is_prim_gluon) { hjet_p_pTD_gluon->Fill(pTD_prim);} 
    else { hjet_p_pTD_else->Fill(pTD_prim);} 

    //// trk jets
    for (unsigned int ic=0; ic<trk_constituent_pT->size(); ic++){
      pTsum_trk += trk_constituent_pT->at(ic);   
      pTsqsum_trk += pow(trk_constituent_pT->at(ic),2);
    }
    pTD_trk = sqrt(pTsqsum_trk)/pTsum_trk;  
    hjet_trk_pTD->Fill(pTD_trk); 
    if(is_trk_quark) { hjet_trk_pTD_quark->Fill(pTD_trk);} 
    else if(is_trk_gluon) { hjet_trk_pTD_gluon->Fill(pTD_trk);} 
    else { hjet_trk_pTD_else->Fill(pTD_trk);} 

    //// tower jets (N.B. Add cemc, ihcal, and ohcal all together!!)
    for (unsigned int ic=0; ic<cemc_constituent_pT->size(); ic++){
      pTsum_twr += cemc_constituent_pT->at(ic);   
      pTsqsum_twr += pow(cemc_constituent_pT->at(ic),2);
    }
    for (unsigned int ic=0; ic<ihcal_constituent_pT->size(); ic++){
      pTsum_twr += ihcal_constituent_pT->at(ic);   
      pTsqsum_twr += pow(ihcal_constituent_pT->at(ic),2);
    }
    for (unsigned int ic=0; ic<ohcal_constituent_pT->size(); ic++){
      pTsum_twr += ohcal_constituent_pT->at(ic);   
      pTsqsum_twr += pow(ohcal_constituent_pT->at(ic),2);
    }
    pTD_twr = sqrt(pTsqsum_twr)/pTsum_twr;  
    hjet_twr_pTD->Fill(pTD_twr); 
    if(is_twr_quark) { hjet_twr_pTD_quark->Fill(pTD_twr);} 
    else if(is_twr_gluon) { hjet_twr_pTD_gluon->Fill(pTD_twr);} 
    else { hjet_twr_pTD_else->Fill(pTD_twr);} 

  } //end of jet entry loop
  
  cout << "Total:" << hjet_p_nconstit->GetEntries() << endl;
  cout << "Quark:" << hjet_p_nconstit_quark->GetEntries() << endl;
  cout << "Gluon:" << hjet_p_nconstit_gluon->GetEntries() << endl;
  cout << "Else:" << hjet_p_nconstit_else->GetEntries() << endl;
   
  ////////////////////////////////////////////////////////
  TFile* fout = new TFile(Form("outMultivar/%s_multivar.root",ihcalType.Data()),"RECREATE");
  fout->cd();
  h2d_prim_trk->Write(); 
  h2d_prim_twr->Write(); 
  hjet_p_nconstit->Write(); 
  hjet_p_nconstit_quark->Write(); 
  hjet_p_nconstit_gluon->Write(); 
  hjet_p_nconstit_else->Write(); 
  hjet_trk_nconstit->Write(); 
  hjet_trk_nconstit_quark->Write(); 
  hjet_trk_nconstit_gluon->Write(); 
  hjet_trk_nconstit_else->Write(); 
  hjet_twr_nconstit->Write(); 
  hjet_twr_nconstit_quark->Write(); 
  hjet_twr_nconstit_gluon->Write(); 
  hjet_twr_nconstit_else->Write(); 
  hjet_p_pTD->Write(); 
  hjet_p_pTD_quark->Write(); 
  hjet_p_pTD_gluon->Write(); 
  hjet_p_pTD_else->Write(); 
  hjet_trk_pTD->Write(); 
  hjet_trk_pTD_quark->Write(); 
  hjet_trk_pTD_gluon->Write(); 
  hjet_trk_pTD_else->Write(); 
  hjet_twr_pTD->Write(); 
  hjet_twr_pTD_quark->Write(); 
  hjet_twr_pTD_gluon->Write(); 
  hjet_twr_pTD_else->Write(); 
  fout->Close();

  return; 
}
