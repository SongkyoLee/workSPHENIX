
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
// SS310_FRAME_RSCALE_OUTPUT (Not yet finished??)
//
//////////////

void drawMultiplicity(TString ihcalType = "SS310_OUTPUT")
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
  float ntrkbin=21, trkbinmin=-0.5, trkbinmax=20.5;
  float ntwrbin=141, twrbinmin=-0.5, twrbinmax=140.5;
  
  //// Check correlations
  TH2D* h2d_prim_trk = new TH2D("h2d_prim_trk",";# of constituents in primary jets;# of constituents in track jets",nprimbin,primbinmin, primbinmax, ntrkbin, trkbinmin, trkbinmax);
  TH2D* h2d_prim_trk_quark = new TH2D("h2d_prim_trk_quark",";# of constituents in primary jets;# of constituents in track jets",nprimbin,primbinmin, primbinmax, ntrkbin, trkbinmin, trkbinmax);
  TH2D* h2d_prim_trk_gluon = new TH2D("h2d_prim_trk_gluon",";# of constituents in primary jets;# of constituents in track jets",nprimbin,primbinmin, primbinmax, ntrkbin, trkbinmin, trkbinmax);
  
  TH2D* h2d_prim_twr = new TH2D("h2d_prim_twr",";# of constituents in primary jets;# of constituents in tower jets",nprimbin,primbinmin, primbinmax, ntwrbin, twrbinmin, twrbinmax);
  TH2D* h2d_prim_twr_quark = new TH2D("h2d_prim_twr_quark",";# of constituents in primary jets;# of constituents in tower jets",nprimbin,primbinmin, primbinmax, ntwrbin, twrbinmin, twrbinmax);
  TH2D* h2d_prim_twr_gluon = new TH2D("h2d_prim_twr_gluon",";# of constituents in primary jets;# of constituents in tower jets",nprimbin,primbinmin, primbinmax, ntwrbin, twrbinmin, twrbinmax);
 
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
/*
    cout << ""<<endl;
    cout << "Prim"<<endl;
    for (int ipg=0; ipg< jet_prim_npg; ipg++){
      cout << "* ipg = " << ipg << endl;
      cout << "fract = " << pg_prim_fract->at(ipg) << endl;
    }
    cout << ""<<endl;
    cout << "Track"<<endl;
    for (int ipg=0; ipg< jet_trk_npg; ipg++){
      cout << "* ipg = " << ipg << endl;
      cout << "fract = " << pg_trk_fract->at(ipg) << endl;
    }
    cout << ""<<endl;
    cout << "Tower"<<endl;
    for (int ipg=0; ipg< jet_twr_npg; ipg++){
      cout << "* ipg = " << ipg << endl;
      cout << "fract = " << pg_twr_fract->at(ipg) << endl;
    }
*/
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
    //// 1-1) total jets
    hjet_p_nconstit->Fill(jet_p_nconstit); 
    hjet_trk_nconstit->Fill(jet_trk_nconstit); 
    hjet_twr_nconstit->Fill(jet_twr_nconstit); 
    //// 1-2)quark & gluon jets
    //h2d_prim_trk_quark->Fill(jet_p_nconstit,jet_trk_nconstit);
    //h2d_prim_twr_quark->Fill(jet_p_nconstit,jet_twr_nconstit);
    //h2d_prim_trk_gluon->Fill(jet_p_nconstit,jet_trk_nconstit);
    //h2d_prim_twr_gluon->Fill(jet_p_nconstit,jet_twr_nconstit);
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
    for (int ic=0; ic<prim_constituent_pT->size(); ic++){
      pTsum_prim += prim_constituent_pT->at(ic);   
      pTsqsum_prim += pow(prim_constituent_pT->at(ic),2);
    }
    pTD_prim = sqrt(pTsqsum_prim)/pTsum_prim;  
    hjet_p_pTD->Fill(pTD_prim); 
    if(is_prim_quark) { hjet_p_pTD_quark->Fill(pTD_prim);} 
    else if(is_prim_gluon) { hjet_p_pTD_gluon->Fill(pTD_prim);} 
    else { hjet_p_pTD_else->Fill(pTD_prim);} 

    //// trk jets
    for (int ic=0; ic<trk_constituent_pT->size(); ic++){
      pTsum_trk += trk_constituent_pT->at(ic);   
      pTsqsum_trk += pow(trk_constituent_pT->at(ic),2);
    }
    pTD_trk = sqrt(pTsqsum_trk)/pTsum_trk;  
    hjet_trk_pTD->Fill(pTD_trk); 
    if(is_trk_quark) { hjet_trk_pTD_quark->Fill(pTD_trk);} 
    else if(is_trk_gluon) { hjet_trk_pTD_gluon->Fill(pTD_trk);} 
    else { hjet_trk_pTD_else->Fill(pTD_trk);} 

    //// tower jets (N.B. Add cemc, ihcal, and ohcal all together!!)
    for (int ic=0; ic<cemc_constituent_pT->size(); ic++){
      pTsum_twr += cemc_constituent_pT->at(ic);   
      pTsqsum_twr += pow(cemc_constituent_pT->at(ic),2);
    }
    for (int ic=0; ic<ihcal_constituent_pT->size(); ic++){
      pTsum_twr += ihcal_constituent_pT->at(ic);   
      pTsqsum_twr += pow(ihcal_constituent_pT->at(ic),2);
    }
    for (int ic=0; ic<ohcal_constituent_pT->size(); ic++){
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
  TFile* fout = new TFile(Form("outMultiplicity/%s_multivar.root",ihcalType.Data()),"RECREATE");
  fout->cd();
  h2d_prim_trk->Write(); 
  h2d_prim_twr->Write(); 
  hjet_p_nconstit->Write(); 
  hjet_p_nconstit_quark->Write(); 
  hjet_p_nconstit_gluon->Write(); 
  hjet_trk_nconstit->Write(); 
  hjet_trk_nconstit_quark->Write(); 
  hjet_trk_nconstit_gluon->Write(); 
  hjet_twr_nconstit->Write(); 
  hjet_twr_nconstit_quark->Write(); 
  hjet_twr_nconstit_gluon->Write(); 
  hjet_p_pTD->Write(); 
  hjet_p_pTD_quark->Write(); 
  hjet_p_pTD_gluon->Write(); 
  hjet_trk_pTD->Write(); 
  hjet_trk_pTD_quark->Write(); 
  hjet_trk_pTD_gluon->Write(); 
  hjet_twr_pTD->Write(); 
  hjet_twr_pTD_quark->Write(); 
  hjet_twr_pTD_gluon->Write(); 
  fout->Close();

  return;
  TLegend* leg = new TLegend(0.63, 0.58, 0.93, 0.78);
  TLegend* leg2 = new TLegend(0.63, 0.68, 0.93, 0.78);
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(32);
  latex->SetTextSize(0.040);

  TPaletteAxis* pal = new TPaletteAxis();
        
  TCanvas* c_cor = new TCanvas("c_cor","c_cor",800,400);
  c_cor->Divide(2,1);
  c_cor->cd(1);
  gPad->SetRightMargin(3.0);
  h2d_prim_trk->GetXaxis()->CenterTitle(1);
  h2d_prim_trk->GetYaxis()->CenterTitle(1);
  h2d_prim_trk->Draw("colz");
  c_cor->Update(); // mandatory to find palette!!
  pal = (TPaletteAxis*)h2d_prim_trk->GetListOfFunctions()->FindObject("palette");
  pal->SetX2NDC(0.92);
  h2d_prim_trk->Draw("colz");
  
  c_cor->cd(2);
  gPad->SetRightMargin(3.0);
  h2d_prim_twr->GetXaxis()->CenterTitle(1);
  h2d_prim_twr->GetYaxis()->CenterTitle(1);
  h2d_prim_twr->Draw("colz");
  c_cor->Update(); // mandatory to find palette!!
  pal = (TPaletteAxis*)h2d_prim_twr->GetListOfFunctions()->FindObject("palette");
  pal->SetX2NDC(0.92);
  h2d_prim_twr->Draw("colz");
  c_cor->Modified();
  c_cor->Update();
  c_cor->SaveAs(Form("outMultiplicity/%s_cor.pdf",ihcalType.Data()));
 /* 
  TCanvas* c_cor_quark = new TCanvas("c_cor_quark","c_cor_quark",800,400);
  c_cor_quark->Divide(2,1);
  c_cor_quark->cd(1);
  gPad->SetRightMargin(3.0);
  h2d_prim_trk_quark->GetXaxis()->CenterTitle(1);
  h2d_prim_trk_quark->GetYaxis()->CenterTitle(1);
  h2d_prim_trk_quark->Draw("colz");
  c_cor_quark->Update(); // mandatory to find palette!!
  pal = (TPaletteAxis*)h2d_prim_trk_quark->GetListOfFunctions()->FindObject("palette");
  pal->SetX2NDC(0.92);
  h2d_prim_trk_quark->Draw("colz");
  c_cor_quark->cd(2);
  gPad->SetRightMargin(3.0);
  h2d_prim_twr_quark->GetXaxis()->CenterTitle(1);
  h2d_prim_twr_quark->GetYaxis()->CenterTitle(1);
  h2d_prim_twr_quark->Draw("colz");
  c_cor_quark->Update(); // mandatory to find palette!!
  pal = (TPaletteAxis*)h2d_prim_twr_quark->GetListOfFunctions()->FindObject("palette");
  pal->SetX2NDC(0.92);
  h2d_prim_twr_quark->Draw("colz");
  c_cor_quark->Modified();
  c_cor_quark->Update();
  c_cor_quark->SaveAs(Form("outMultiplicity/%s_cor_quark.pdf",ihcalType.Data()));
  
  TCanvas* c_cor_gluon = new TCanvas("c_cor_gluon","c_cor_gluon",800,400);
  c_cor_gluon->Divide(2,1);
  c_cor_gluon->cd(1);
  gPad->SetRightMargin(3.0);
  h2d_prim_trk_gluon->GetXaxis()->CenterTitle(1);
  h2d_prim_trk_gluon->GetYaxis()->CenterTitle(1);
  h2d_prim_trk_gluon->Draw("colz");
  c_cor_gluon->Update(); // mandatory to find palette!!
  pal = (TPaletteAxis*)h2d_prim_trk_gluon->GetListOfFunctions()->FindObject("palette");
  pal->SetX2NDC(0.92);
  h2d_prim_trk_gluon->Draw("colz");
  c_cor_gluon->cd(2);
  gPad->SetRightMargin(3.0);
  h2d_prim_twr_gluon->GetXaxis()->CenterTitle(1);
  h2d_prim_twr_gluon->GetYaxis()->CenterTitle(1);
  h2d_prim_twr_gluon->Draw("colz");
  c_cor_gluon->Update(); // mandatory to find palette!!
  pal = (TPaletteAxis*)h2d_prim_twr_gluon->GetListOfFunctions()->FindObject("palette");
  pal->SetX2NDC(0.92);
  h2d_prim_twr_gluon->Draw("colz");
  c_cor_gluon->Modified();
  c_cor_gluon->Update();
  c_cor_gluon->SaveAs(Form("outMultiplicity/%s_cor_gluon.pdf",ihcalType.Data()));
*/ 
  ////////////////////////////////////////////////////////
  
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
  
  leg->AddEntry(hjet_p_nconstit,"Total","l");
  leg->AddEntry(hjet_p_nconstit_quark,"Light quark","l");
  leg->AddEntry(hjet_p_nconstit_gluon,"Gluon","l");
  leg->AddEntry(hjet_p_nconstit_else,"Else","l");
  leg2->AddEntry(hjet_p_nconstit_quark,"Light quark","l");
  leg2->AddEntry(hjet_p_nconstit_gluon,"Gluon","l");
  
  TCanvas* c_prim_multi = new TCanvas("c_prim_multi","c_prim_multi",800,400);
  c_prim_multi->Divide(2,1);
  c_prim_multi->cd(1);
  hjet_p_nconstit->GetXaxis()->SetTitle("# of constituents (Primary particles)");
  hjet_p_nconstit->GetXaxis()->CenterTitle(1);
  hjet_p_nconstit->Draw("hist");
  hjet_p_nconstit_quark->Draw("hist same");
  hjet_p_nconstit_gluon->Draw("hist same");
  hjet_p_nconstit_else->Draw("hist same");
  leg->Draw();
  latex->DrawLatex(0.91,0.88,ihcalType.Data());
  latex->DrawLatex(0.91,0.82,"Primary jet");

  c_prim_multi->cd(2); 
  TH1D* norm_hjet_p_nconstit = (TH1D*) hjet_p_nconstit->Clone();
  TH1D* norm_hjet_p_nconstit_quark = (TH1D*) hjet_p_nconstit_quark->Clone();
  TH1D* norm_hjet_p_nconstit_gluon = (TH1D*) hjet_p_nconstit_gluon->Clone();
  TH1D* norm_hjet_p_nconstit_else = (TH1D*) hjet_p_nconstit_else->Clone();
  
  norm_hjet_p_nconstit->Scale(1./norm_hjet_p_nconstit->Integral());
  norm_hjet_p_nconstit_quark->Scale(1./norm_hjet_p_nconstit_quark->Integral());
  norm_hjet_p_nconstit_gluon->Scale(1./norm_hjet_p_nconstit_gluon->Integral());
  norm_hjet_p_nconstit_else->Scale(1./norm_hjet_p_nconstit_else->Integral());

  norm_hjet_p_nconstit_quark->GetXaxis()->SetTitle("# of constituents (Primary particles)");
  norm_hjet_p_nconstit_quark->GetXaxis()->CenterTitle(1);
  norm_hjet_p_nconstit_quark->Draw("hist");
  norm_hjet_p_nconstit_gluon->Draw("hist same");
  leg2->Draw();
  latex->DrawLatex(0.91,0.88,ihcalType.Data());
  latex->DrawLatex(0.91,0.82,"Primary jet");
  c_prim_multi->Modified();
  c_prim_multi->Update();
  c_prim_multi->SaveAs(Form("outMultiplicity/%s_prim_multi.pdf",ihcalType.Data()));
  
  ////////////////////////////////////////////////////////
  
  hjet_trk_nconstit->SetLineColor(kGray+3);
  hjet_trk_nconstit->SetMarkerColor(kGray+3);
  hjet_trk_nconstit->SetMarkerSize(0);
  hjet_trk_nconstit->SetLineStyle(7);
  hjet_trk_nconstit_quark->SetLineColor(kPink+3);
  hjet_trk_nconstit_quark->SetMarkerColor(kPink+3);
  hjet_trk_nconstit_quark->SetMarkerSize(0);
  hjet_trk_nconstit_quark->SetFillColor(kPink+2);
  hjet_trk_nconstit_quark->SetFillStyle(3445);
  hjet_trk_nconstit_gluon->SetLineColor(kAzure+4);
  hjet_trk_nconstit_gluon->SetMarkerColor(kAzure+4);
  hjet_trk_nconstit_gluon->SetMarkerSize(0);
  hjet_trk_nconstit_gluon->SetFillColor(kAzure+5);
  hjet_trk_nconstit_gluon->SetFillStyle(3454);
  hjet_trk_nconstit_else->SetLineColor(kSpring+3);
  hjet_trk_nconstit_else->SetMarkerColor(kSpring+3);
  hjet_trk_nconstit_else->SetMarkerSize(0);
  hjet_trk_nconstit_else->SetFillColor(kSpring+9);
  hjet_trk_nconstit_else->SetFillStyle(3003);
  
  TCanvas* c_trk_multi = new TCanvas("c_trk_multi","c_trk_multi",800,400);
  c_trk_multi->Divide(2,1);
  c_trk_multi->cd(1);
  hjet_trk_nconstit->GetXaxis()->SetTitle("# of constituents (reco tracks)");
  hjet_trk_nconstit->GetXaxis()->CenterTitle(1);
  hjet_trk_nconstit->Draw("hist");
  hjet_trk_nconstit_quark->Draw("hist same");
  hjet_trk_nconstit_gluon->Draw("hist same");
  hjet_trk_nconstit_else->Draw("hist same");
  latex->DrawLatex(0.91,0.88,ihcalType.Data());
  latex->DrawLatex(0.91,0.82,"Track jet");
  leg->Draw();
  
  c_trk_multi->cd(2); 
  TH1D* norm_hjet_trk_nconstit = (TH1D*) hjet_trk_nconstit->Clone();
  TH1D* norm_hjet_trk_nconstit_quark = (TH1D*) hjet_trk_nconstit_quark->Clone();
  TH1D* norm_hjet_trk_nconstit_gluon = (TH1D*) hjet_trk_nconstit_gluon->Clone();
  TH1D* norm_hjet_trk_nconstit_else = (TH1D*) hjet_trk_nconstit_else->Clone();
  
  norm_hjet_trk_nconstit->Scale(1./norm_hjet_trk_nconstit->Integral());
  norm_hjet_trk_nconstit_quark->Scale(1./norm_hjet_trk_nconstit_quark->Integral());
  norm_hjet_trk_nconstit_gluon->Scale(1./norm_hjet_trk_nconstit_gluon->Integral());
  norm_hjet_trk_nconstit_else->Scale(1./norm_hjet_trk_nconstit_else->Integral());

  norm_hjet_trk_nconstit_quark->GetXaxis()->SetTitle("# of constituents (reco tracks)");
  norm_hjet_trk_nconstit_quark->GetXaxis()->CenterTitle(1);
  norm_hjet_trk_nconstit_quark->Draw("hist");
  norm_hjet_trk_nconstit_gluon->Draw("hist same");
  leg2->Draw();
  latex->DrawLatex(0.91,0.88,ihcalType.Data());
  latex->DrawLatex(0.91,0.82,"Track jet");
  c_trk_multi->Modified();
  c_trk_multi->Update();
  c_trk_multi->SaveAs(Form("outMultiplicity/%s_trk_multi.pdf",ihcalType.Data()));
  
  ////////////////////////////////////////////////////////
  
  hjet_twr_nconstit->SetLineColor(kGray+3);
  hjet_twr_nconstit->SetMarkerColor(kGray+3);
  hjet_twr_nconstit->SetMarkerSize(0);
  hjet_twr_nconstit->SetLineStyle(7);
  hjet_twr_nconstit_quark->SetLineColor(kPink+3);
  hjet_twr_nconstit_quark->SetMarkerColor(kPink+3);
  hjet_twr_nconstit_quark->SetMarkerSize(0);
  hjet_twr_nconstit_quark->SetFillColor(kPink+2);
  hjet_twr_nconstit_quark->SetFillStyle(3445);
  hjet_twr_nconstit_gluon->SetLineColor(kAzure+4);
  hjet_twr_nconstit_gluon->SetMarkerColor(kAzure+4);
  hjet_twr_nconstit_gluon->SetMarkerSize(0);
  hjet_twr_nconstit_gluon->SetFillColor(kAzure+5);
  hjet_twr_nconstit_gluon->SetFillStyle(3454);
  hjet_twr_nconstit_else->SetLineColor(kSpring+3);
  hjet_twr_nconstit_else->SetMarkerColor(kSpring+3);
  hjet_twr_nconstit_else->SetMarkerSize(0);
  hjet_twr_nconstit_else->SetFillColor(kSpring+9);
  hjet_twr_nconstit_else->SetFillStyle(3003);
  
  TCanvas* c_twr_multi = new TCanvas("c_twr_multi","c_twr_multi",800,400);
  c_twr_multi->Divide(2,1);
  c_twr_multi->cd(1);
  hjet_twr_nconstit->GetXaxis()->SetTitle("# of constituents (towers)");
  hjet_twr_nconstit->GetXaxis()->CenterTitle(1);
  hjet_twr_nconstit->Draw("hist");
  hjet_twr_nconstit_quark->Draw("hist same");
  hjet_twr_nconstit_gluon->Draw("hist same");
  hjet_twr_nconstit_else->Draw("hist same");
  leg->Draw();
  latex->DrawLatex(0.91,0.88,ihcalType.Data());
  latex->DrawLatex(0.91,0.82,"Tower jet");

  c_twr_multi->cd(2); 
  TH1D* norm_hjet_twr_nconstit = (TH1D*) hjet_twr_nconstit->Clone();
  TH1D* norm_hjet_twr_nconstit_quark = (TH1D*) hjet_twr_nconstit_quark->Clone();
  TH1D* norm_hjet_twr_nconstit_gluon = (TH1D*) hjet_twr_nconstit_gluon->Clone();
  TH1D* norm_hjet_twr_nconstit_else = (TH1D*) hjet_twr_nconstit_else->Clone();
  
  norm_hjet_twr_nconstit->Scale(1./norm_hjet_twr_nconstit->Integral());
  norm_hjet_twr_nconstit_quark->Scale(1./norm_hjet_twr_nconstit_quark->Integral());
  norm_hjet_twr_nconstit_gluon->Scale(1./norm_hjet_twr_nconstit_gluon->Integral());
  norm_hjet_twr_nconstit_else->Scale(1./norm_hjet_twr_nconstit_else->Integral());

  norm_hjet_twr_nconstit_quark->GetXaxis()->SetTitle("# of constituents (towres)");
  norm_hjet_twr_nconstit_quark->GetXaxis()->CenterTitle(1);
  norm_hjet_twr_nconstit_quark->Draw("hist");
  norm_hjet_twr_nconstit_gluon->Draw("hist same");
  leg2->Draw();
  latex->DrawLatex(0.91,0.88,ihcalType.Data());
  latex->DrawLatex(0.91,0.82,"Tower jet");
  
  c_twr_multi->Modified();
  c_twr_multi->Update();
  c_twr_multi->SaveAs(Form("outMultiplicity/%s_twr_multi.pdf",ihcalType.Data()));
 
  return; 
}
