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

using namespace std;

double DeltaPhi(double phi1, double phi2);

void rootAna_2ndSort_gammajet(string sampleType = "GammaJet",
                              string ihcalType = "Al",
                              string jetInE = "20GeV",
                              string jetOutE = "20GeV",
                              string note = "R04",
                              int initfile=0,
                              int endfile =1000,
                              bool doetopcut=false,
                              bool dodphicut=false)
{
  //// N.B.: DO NOT SCALE in this code!!!

  //////////////////////////////////////////////////////////////////////////// 
  //// write new tree
  string foutname = Form("./2ndSortedRootFiles/2ndSorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_%s_doetopcut%d_dodphicut%d.root",initfile,endfile,sampleType.c_str(),jetOutE.c_str(),ihcalType.c_str(),note.c_str(),(int)doetopcut,(int)dodphicut);
  TFile* fout = new TFile( foutname.c_str(), "RECREATE");
  TTree* outTree = new TTree("out_tree","out_tree");
  
  int evt=1;
  //// Set the one with larger em_frac as gamma, and the other as a jet
  float true_gamma_e, true_gamma_p, true_gamma_pt, true_gamma_eta, true_gamma_phi;
  float true_gamma_lcons_z, true_gamma_emfrac;
  int true_gamma_cons_n, true_gamma_lcons_pid;

  float true_jet_e, true_jet_p, true_jet_pt, true_jet_eta, true_jet_phi;
  float true_jet_lcons_z, true_jet_emfrac;
  int true_jet_cons_n, true_jet_lcons_pid;
  
  float reco_gamma_e, reco_gamma_p, reco_gamma_pt, reco_gamma_eta, reco_gamma_phi;
  float reco_gamma_clcemc_hadEsum, reco_gamma_clcemc_emEsum, reco_gamma_clcemc_totEsum;
  float reco_gamma_clihcal_totEsum, reco_gamma_clohcal_totEsum;
  float reco_gamma_cemcEsum, reco_gamma_ihcalEsum, reco_gamma_ohcalEsum;

  float reco_jet_e, reco_jet_p, reco_jet_pt, reco_jet_eta, reco_jet_phi;
  float reco_jet_clcemc_hadEsum, reco_jet_clcemc_emEsum, reco_jet_clcemc_totEsum;
  float reco_jet_clihcal_totEsum, reco_jet_clohcal_totEsum;
  float reco_jet_cemcEsum, reco_jet_ihcalEsum, reco_jet_ohcalEsum;
  
	float true_dPhi;
	float reco_dPhi;

	outTree->Branch("evt",&evt,"evt/I");
  outTree->Branch("true_gamma_e",&true_gamma_e,"true_gamma_e/F");
  outTree->Branch("true_gamma_p",&true_gamma_p,"true_gamma_p/F");
  outTree->Branch("true_gamma_pt",&true_gamma_pt,"true_gamma_pt/F");
  outTree->Branch("true_gamma_eta",&true_gamma_eta,"true_gamma_eta/F");
  outTree->Branch("true_gamma_phi",&true_gamma_phi,"true_gamma_phi/F");
  outTree->Branch("true_gamma_lcons_z",&true_gamma_lcons_z,"true_gamma_lcons_z/F");
  outTree->Branch("true_gamma_emfrac",&true_gamma_emfrac,"true_gamma_emfrac/F");
  outTree->Branch("true_gamma_cons_n",&true_gamma_cons_n,"true_gamma_cons_n/I");
  outTree->Branch("true_gamma_lcons_pid",&true_gamma_lcons_pid,"true_gamma_lcons_pid/I");
 
  outTree->Branch("true_jet_e",&true_jet_e,"true_jet_e/F");
  outTree->Branch("true_jet_p",&true_jet_p,"true_jet_p/F");
  outTree->Branch("true_jet_pt",&true_jet_pt,"true_jet_pt/F");
  outTree->Branch("true_jet_eta",&true_jet_eta,"true_jet_eta/F");
  outTree->Branch("true_jet_phi",&true_jet_phi,"true_jet_phi/F");
  outTree->Branch("true_jet_lcons_z",&true_jet_lcons_z,"true_jet_lcons_z/F");
  outTree->Branch("true_jet_emfrac",&true_jet_emfrac,"true_jet_emfrac/F");
  outTree->Branch("true_jet_cons_n",&true_jet_cons_n,"true_jet_cons_n/I");
  outTree->Branch("true_jet_lcons_pid",&true_jet_lcons_pid,"true_jet_lcons_pid/I"); 
  
  outTree->Branch("reco_gamma_e",&reco_gamma_e,"reco_gamma_e/F");
  outTree->Branch("reco_gamma_p",&reco_gamma_p,"reco_gamma_p/F");  
  outTree->Branch("reco_gamma_pt",&reco_gamma_pt,"reco_gamma_pt/F");  
  outTree->Branch("reco_gamma_eta",&reco_gamma_eta,"reco_gamma_eta/F");  
  outTree->Branch("reco_gamma_phi",&reco_gamma_phi,"reco_gamma_phi/F");  
  outTree->Branch("reco_gamma_clcemc_hadEsum",&reco_gamma_clcemc_hadEsum,"reco_gamma_clcemc_hadEsum/F"); //cluster 
  outTree->Branch("reco_gamma_clcemc_emEsum",&reco_gamma_clcemc_emEsum,"reco_gamma_clcemc_emEsum/F");  
  outTree->Branch("reco_gamma_clcemc_totEsum",&reco_gamma_clcemc_totEsum,"reco_gamma_clcemc_totEsum/F");  
  outTree->Branch("reco_gamma_clihcal_totEsum",&reco_gamma_clihcal_totEsum,"reco_gamma_clihcal_totEsum/F");  
  outTree->Branch("reco_gamma_clohcal_totEsum",&reco_gamma_clohcal_totEsum,"reco_gamma_clohcal_totEsum/F");  
  outTree->Branch("reco_gamma_cemcEsum",&reco_gamma_cemcEsum,"reco_gamma_cemcEsum/F");  //tower
  outTree->Branch("reco_gamma_ihcalEsum",&reco_gamma_ihcalEsum,"reco_gamma_ihcalEsum/F");  
  outTree->Branch("reco_gamma_ohcalEsum",&reco_gamma_ohcalEsum,"reco_gamma_ohcalEsum/F");  

	outTree->Branch("reco_jet_e",&reco_jet_e,"reco_jet_e/F");
  outTree->Branch("reco_jet_p",&reco_jet_p,"reco_jet_p/F");
  outTree->Branch("reco_jet_pt",&reco_jet_pt,"reco_jet_pt/F");
  outTree->Branch("reco_jet_eta",&reco_jet_eta,"reco_jet_eta/F");
  outTree->Branch("reco_jet_phi",&reco_jet_phi,"reco_jet_phi/F");
  outTree->Branch("reco_jet_clcemc_hadEsum",&reco_jet_clcemc_hadEsum,"reco_jet_clcemc_hadEsum/F"); //cluster
  outTree->Branch("reco_jet_clcemc_emEsum",&reco_jet_clcemc_emEsum,"reco_jet_clcemc_emEsum/F"); 
  outTree->Branch("reco_jet_clcemc_totEsum",&reco_jet_clcemc_totEsum,"reco_jet_clcemc_totEsum/F");
  outTree->Branch("reco_jet_clihcal_totEsum",&reco_jet_clihcal_totEsum,"reco_jet_clihcal_totEsum/F");  
  outTree->Branch("reco_jet_clohcal_totEsum",&reco_jet_clohcal_totEsum,"reco_jet_clohcal_totEsum/F");  
  outTree->Branch("reco_jet_cemcEsum",&reco_jet_cemcEsum,"reco_jet_cemcEsum/F"); //tower-NOTSCALED
  outTree->Branch("reco_jet_ihcalEsum",&reco_jet_ihcalEsum,"reco_jet_ihcalEsum/F");//tower-scaled
  outTree->Branch("reco_jet_ohcalEsum",&reco_jet_ohcalEsum,"reco_jet_ohcalEsum/F");//tower-scaled

	outTree->Branch("true_dPhi",&true_dPhi,"true_dPhi/F");
  outTree->Branch("reco_dPhi",&reco_dPhi,"reco_dPhi/F");

  ////////////////////////////////////// 
  //// read-in files
  ////////////////////////////////////// 
  string fname;
  fname = Form("./sortedRootFiles/sorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_%s_doetopcut%d_dodphicut%d.root",initfile,endfile,sampleType.c_str(),jetInE.c_str(),ihcalType.c_str(),note.c_str(),(int)doetopcut,(int)dodphicut);

  TFile* fin = new TFile( fname.c_str(), "READ");
  if(!fin->IsOpen()) { cout <<"WARNING!! file not found"<<endl; return;};
  TTree* true_tree = (TTree*)fin->Get("true_tree");
  TTree* reco_tree = (TTree*)fin->Get("reco_tree");
  true_tree->AddFriend("reco_tree",fin);

  if (true_tree->GetEntries() != reco_tree->GetEntries()) {
    cout <<"WARNING!! Entries of true_tree and reco_tree do not match!"<<endl; return;
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
  int reco_cemc_n[999], reco_ihcal_n[999], reco_ohcal_n[999];
  float reco_cemcEsum[999], reco_ihcalEsum[999], reco_ohcalEsum[999];
  int reco_clcemc_n[999], reco_clcemc_had_n[999], reco_clcemc_em_n[999];
  float reco_clcemc_totEsum[999], reco_clcemc_hadEsum[999], reco_clcemc_emEsum[999];
  int reco_clihcal_n[999], reco_clohcal_n[999];
  float reco_clihcal_totEsum[999], reco_clohcal_totEsum[999];
  true_tree->SetBranchAddress("reco_tree.event",&reco_event); 
  true_tree->SetBranchAddress("reco_n",&reco_n); 
  true_tree->SetBranchAddress("reco_e",&reco_e); 
  true_tree->SetBranchAddress("reco_p",&reco_p); 
  true_tree->SetBranchAddress("reco_pt",&reco_pt); 
  true_tree->SetBranchAddress("reco_eta",&reco_eta); 
  true_tree->SetBranchAddress("reco_phi",&reco_phi); 
  true_tree->SetBranchAddress("reco_cemc_n",&reco_cemc_n); 
  true_tree->SetBranchAddress("reco_ihcal_n",&reco_ihcal_n); 
  true_tree->SetBranchAddress("reco_ohcal_n",&reco_ohcal_n); 
  true_tree->SetBranchAddress("reco_cemcEsum",&reco_cemcEsum); 
  true_tree->SetBranchAddress("reco_ihcalEsum",&reco_ihcalEsum); 
  true_tree->SetBranchAddress("reco_ohcalEsum",&reco_ohcalEsum); 
  true_tree->SetBranchAddress("reco_clcemc_n",&reco_clcemc_n); 
  true_tree->SetBranchAddress("reco_clcemc_had_n",&reco_clcemc_had_n); 
  true_tree->SetBranchAddress("reco_clcemc_em_n",&reco_clcemc_em_n); 
  true_tree->SetBranchAddress("reco_clcemc_totEsum",&reco_clcemc_totEsum); 
  true_tree->SetBranchAddress("reco_clcemc_hadEsum",&reco_clcemc_hadEsum); 
  true_tree->SetBranchAddress("reco_clcemc_emEsum",&reco_clcemc_emEsum); 
  true_tree->SetBranchAddress("reco_clihcal_n",&reco_clihcal_n); 
  true_tree->SetBranchAddress("reco_clohcal_n",&reco_clohcal_n); 
  true_tree->SetBranchAddress("reco_clihcal_totEsum",&reco_clihcal_totEsum); 
  true_tree->SetBranchAddress("reco_clohcal_totEsum",&reco_clohcal_totEsum); 
  
  ////////////////////////////////////// 
  for (int ievt=0; ievt<true_tree->GetEntries(); ievt++){ 
    if (ievt%1000==0) cout << "Processing Event :" <<ievt << endl; 
    true_tree->GetEvent(ievt); 
    
    ////Option 1) select events only containing 2 truth objects 
    ////if (true_n!=2) continue; 
    ////int idx_1 =0; int idx_2 =1; 
    
    ////Option 2) select two truth objects with highest E
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

    //if ((true_emfrac[idx_1] > true_emfrac[idx_2]) && true_lcons_pid[idx_1] ==22 && true_cons_n[idx_1]==1 ){
		if ((true_emfrac[idx_1] >= true_emfrac[idx_2]) && true_lcons_pid[idx_1] ==22){
      true_gamma_e=true_e[idx_1];
      true_gamma_p=true_p[idx_1];
      true_gamma_pt=true_pt[idx_1];
      true_gamma_eta=true_eta[idx_1];
      true_gamma_phi=true_phi[idx_1];
      true_gamma_lcons_z=true_lcons_z[idx_1];
      true_gamma_emfrac=true_emfrac[idx_1];
      true_gamma_cons_n=true_cons_n[idx_1];
      true_gamma_lcons_pid=true_lcons_pid[idx_1];
			true_jet_e=true_e[idx_2];
      true_jet_p=true_p[idx_2];
      true_jet_pt=true_pt[idx_2];
      true_jet_eta=true_eta[idx_2];
      true_jet_phi=true_phi[idx_2];
      true_jet_lcons_z=true_lcons_z[idx_2];
      true_jet_emfrac=true_emfrac[idx_2];
      true_jet_cons_n=true_cons_n[idx_2];
      true_jet_lcons_pid=true_lcons_pid[idx_2];
    //} else if ((true_emfrac[idx_1] < true_emfrac[idx_2]) && true_lcons_pid[idx_2] ==22 && true_cons_n[idx_2]==1 ){
		} else if ((true_emfrac[idx_1] <= true_emfrac[idx_2]) && true_lcons_pid[idx_2] ==22 ){
      true_gamma_e=true_e[idx_2];
      true_gamma_p=true_p[idx_2];
      true_gamma_pt=true_pt[idx_2];
      true_gamma_eta=true_eta[idx_2];
      true_gamma_phi=true_phi[idx_2];
      true_gamma_lcons_z=true_lcons_z[idx_2];
      true_gamma_emfrac=true_emfrac[idx_2];
      true_gamma_cons_n=true_cons_n[idx_2];
      true_gamma_lcons_pid=true_lcons_pid[idx_2];
      true_jet_e=true_e[idx_1];
      true_jet_p=true_p[idx_1];
      true_jet_pt=true_pt[idx_1];
      true_jet_eta=true_eta[idx_1];
      true_jet_phi=true_phi[idx_1];
      true_jet_lcons_z=true_lcons_z[idx_1];
      true_jet_emfrac=true_emfrac[idx_1];
      true_jet_cons_n=true_cons_n[idx_1];
      true_jet_lcons_pid=true_lcons_pid[idx_1];
    } else {
      //cout << "Gamma not found" << endl;
      continue;
    } 
     
    ////////// cut on truth //////////
    //if (true_gamma_e < 50 || true_jet_e < 50) continue;
    //if (true_gamma_e < 50) continue; //only to gamma
    //if (fabs(true_gamma_eta) > 0.45 || fabs(true_jet_eta) > 0.45) continue;
    
    ///////////////////////// 
    //// reco matching
    ///////////////////////// 
    float tmp_gamma_dPhi = 999;
    float tmp_gamma_dR = 999;
    float match_gamma_dR = 999;
    float tmp_jet_dPhi = 999;
    float tmp_jet_dR = 999;
    float match_jet_dR = 999;
    int tmp_gamma_idx = 999;
    
    if (reco_n!=2) continue; //select events only containing 2 reco objects 
   
    ////////// 1) match gamma
    for (int ireco=0; ireco<reco_n; ireco++){
      tmp_gamma_dPhi = fabs( DeltaPhi(true_gamma_phi,reco_phi[ireco]) );
      tmp_gamma_dR = sqrt(pow(true_gamma_eta-reco_eta[ireco],2)+pow(tmp_gamma_dPhi,2));
      if (tmp_gamma_dR < match_gamma_dR){
        match_gamma_dR = tmp_gamma_dR;
        reco_gamma_e = reco_e[ireco];
        reco_gamma_p = reco_p[ireco];
        reco_gamma_pt = reco_pt[ireco];
        reco_gamma_eta = reco_eta[ireco];
        reco_gamma_phi = reco_phi[ireco];
        //// cluster
        reco_gamma_clcemc_hadEsum = reco_clcemc_hadEsum[ireco]; 
        reco_gamma_clcemc_emEsum = reco_clcemc_emEsum[ireco]; 
        reco_gamma_clcemc_totEsum = reco_clcemc_totEsum[ireco]; 
        reco_gamma_clihcal_totEsum = reco_clihcal_totEsum[ireco]; 
        reco_gamma_clohcal_totEsum = reco_clohcal_totEsum[ireco]; 
        //// tower
        reco_gamma_cemcEsum = reco_cemcEsum[ireco];
        reco_gamma_ihcalEsum = reco_ihcalEsum[ireco];
        reco_gamma_ohcalEsum = reco_ohcalEsum[ireco];
        
        tmp_gamma_idx = ireco;
      }
    } //EOL over reco_n
    
    ////////// 2) match jet + scaling
    for (int ireco=0; ireco<reco_n; ireco++){
      if (ireco == tmp_gamma_idx) continue; // reject the one reco-ed as gamma
      tmp_jet_dPhi = fabs( DeltaPhi(true_jet_phi,reco_phi[ireco]) );
      tmp_jet_dR = sqrt(pow(true_jet_eta-reco_eta[ireco],2)+pow(tmp_jet_dPhi,2));
      if (tmp_jet_dR < match_jet_dR){
        match_jet_dR = tmp_jet_dR;
        reco_jet_e = reco_e[ireco];
        reco_jet_p = reco_p[ireco];
        reco_jet_pt = reco_pt[ireco];
        reco_jet_eta = reco_eta[ireco];
        reco_jet_phi = reco_phi[ireco];
        //// cluster
        reco_jet_clcemc_hadEsum = reco_clcemc_hadEsum[ireco];
        reco_jet_clcemc_emEsum = reco_clcemc_emEsum[ireco]; 
        reco_jet_clcemc_totEsum = reco_clcemc_totEsum[ireco]; 
        reco_jet_clihcal_totEsum = reco_clihcal_totEsum[ireco]; 
        reco_jet_clohcal_totEsum = reco_clohcal_totEsum[ireco]; 
        //// tower (cemcEsum remained the same!!)
        //reco_jet_cemcEsum = reco_jet_clcemc_hadEsum+reco_jet_clcemc_emEsum; //scaled sum
        reco_jet_cemcEsum = reco_cemcEsum[ireco];
        reco_jet_ihcalEsum = reco_ihcalEsum[ireco];
        reco_jet_ohcalEsum = reco_ohcalEsum[ireco];
      }
    } //EOL over reco_n 
   
    /// double-check dPhi
    true_dPhi = fabs( DeltaPhi(true_gamma_phi, true_jet_phi) );
		reco_dPhi = fabs( DeltaPhi(reco_gamma_phi, reco_jet_phi) );
    
    ////////// cut on reco //////////
    if (fabs(reco_gamma_eta) > 0.45) continue;
    if (fabs(reco_jet_eta) > 0.45) continue;
    //// *** dR(reco-true) matching  
    //if (match_gamma_dR > 0.4 || match_jet_dR > 0.4) continue; 
    //// *** dR(gamma-jet) in different hemisphere
    if (reco_dPhi < TMath::Pi()*(1/2.)) continue;
		//if (reco_dPhi < TMath::Pi()*(7/8.)) continue;
    //// HCAL/CEMC cut for highly efficient photon
    if ((reco_gamma_ihcalEsum+reco_gamma_ohcalEsum)/reco_gamma_cemcEsum  >0.1) continue;
    //// *** reco jet e range (cut on reco)
		if (jetOutE.compare("allGeV")==0){
			if (!(reco_gamma_e > 20)) continue; //only to reco gamma
		}
    else if (jetOutE.compare("20GeV")==0){
      if (!(reco_gamma_e > 20 && reco_gamma_e <30)) continue; //only to reco gamma
    }
    else if (jetOutE.compare("30GeV")==0){
      if (!(reco_gamma_e > 30 && reco_gamma_e <40)) continue; //only to reco gamma
    }
    else if (jetOutE.compare("40GeV")==0){
      if (!(reco_gamma_e > 40 && reco_gamma_e <50)) continue; //only to reco gamma
    }
    else if (jetOutE.compare("50GeV")==0){
      if (!(reco_gamma_e > 50 && reco_gamma_e <60)) continue; //only to reco gamma
    }
    else { cout << "KYO: select among 20-50GeV" << endl; return;}

    outTree->Fill();
    evt++;
  
  } // EOL over evt
  
  ////////////////////////////////////// 
  fout->cd();
  outTree->Write();
  fout->Close(); 

  
  return; 
 
}

double DeltaPhi(double phi1, double phi2){
  Double_t dphi;
  dphi = phi1 - phi2;
  if(dphi<-TMath::Pi()) dphi += TMath::TwoPi();
  if(dphi>=TMath::Pi()) dphi -= TMath::TwoPi();
  return dphi;
}
