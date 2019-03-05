
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

void rootAna_sorting_jet4(string label = "gammajet_20GeV_SS310",int initfile=0,int endfile =1000)
{

  //// Suppress error messages from bad files
  gErrorIgnoreLevel = kFatal; 

  int bad_files = 0; 

  string ihcalType;
  string jetE;
    
  if (label.compare("gammajet_20GeV_SS310")==0){ ihcalType = "SS310"; jetE="20GeV"; }
  else if (label.compare("gammajet_20GeV_Al")==0){ ihcalType = "Al"; jetE="20GeV"; } 
  else if (label.compare("gammajet_20GeV_Al_uninst")==0){ ihcalType = "Al_uninst"; jetE="20GeV";} 
  else if (label.compare("gammajet_30GeV_SS310")==0){ ihcalType = "SS310"; jetE="30GeV"; }
  else if (label.compare("gammajet_30GeV_Al")==0){ ihcalType = "Al"; jetE="30GeV"; } 
  else if (label.compare("gammajet_30GeV_Al_uninst")==0){ ihcalType = "Al_uninst"; jetE="30GeV";} 
  else if (label.compare("gammajet_40GeV_SS310")==0){ ihcalType = "SS310"; jetE="40GeV"; }
  else if (label.compare("gammajet_40GeV_Al")==0){ ihcalType = "Al"; jetE="40GeV"; } 
  else if (label.compare("gammajet_40GeV_Al_uninst")==0){ ihcalType = "Al_uninst"; jetE="40GeV";} 
  else if (label.compare("gammajet_50GeV_SS310")==0){ ihcalType = "SS310"; jetE="50GeV"; }
  else if (label.compare("gammajet_50GeV_Al")==0){ ihcalType = "Al"; jetE="50GeV"; } 
  else if (label.compare("gammajet_50GeV_Al_uninst")==0){ ihcalType = "Al_uninst"; jetE="50GeV";} 
  else { cout << "KYO WARNING: select proper ihcal type"<<endl; return; }

  //// write new tree 
  string foutname = Form("./sortedRootFiles/sorted_%dto%d_gammajet_G4sPHENIX_jet4_%s_%s.root",initfile,endfile,jetE.c_str(),ihcalType.c_str());
  TFile* outFile = new TFile( foutname.c_str(), "RECREATE");
  TTree* outTrueTree = new TTree("true_tree","true_tree");
  TTree* outRecoTree = new TTree("reco_tree","reco_tree");
  
  int true_event=1; //start from event 1!! 
  int true_n;
  float true_e[999], true_p[999], true_pt[999], true_eta[999], true_phi[999];
  // only store leading cons info. (one per jet)
  int true_cons_n[999];
  float true_lcons_z[999];
  int true_lcons_pid[999];
  float true_emfrac[999];
  // only store leading pg info. (one per jet)
  int true_pg_n[999];
  int true_lpg_id[999];
  float true_lpg_fract[999];
  int true_lpg_status[999];
  outTrueTree->Branch("event",&true_event,"event/I");
  outTrueTree->Branch("true_n",&true_n,"true_n/I");
  outTrueTree->Branch("true_e",&true_e,"true_e[true_n]/F");
  outTrueTree->Branch("true_p",&true_p,"true_p[true_n]/F");
  outTrueTree->Branch("true_pt",&true_pt,"true_pt[true_n]/F");
  outTrueTree->Branch("true_eta",&true_eta,"true_eta[true_n]/F");
  outTrueTree->Branch("true_phi",&true_phi,"true_phi[true_n]/F");
  outTrueTree->Branch("true_cons_n",&true_cons_n,"true_cons_n[true_n]/I");
  outTrueTree->Branch("true_lcons_z",&true_lcons_z,"true_lcons_z[true_n]/F");
  outTrueTree->Branch("true_lcons_pid",&true_lcons_pid,"true_lcons_pid[true_n]/I");
  outTrueTree->Branch("true_emfrac",&true_emfrac,"true_emfrac[true_n]/F");
  outTrueTree->Branch("true_pg_n",&true_pg_n,"true_pg_n[true_n]/I");
  outTrueTree->Branch("true_lpg_id",&true_lpg_id,"true_lpg_id[true_n]/I");
  outTrueTree->Branch("true_lpg_fract",&true_lpg_fract,"true_lpg_fract[true_n]/F");
  outTrueTree->Branch("true_lpg_status",&true_lpg_status,"true_lpg_status[true_n]/I");

  int reco_event=1; //start from event 1!! 
  int reco_n;
  float reco_e[999], reco_p[999], reco_pt[999], reco_eta[999], reco_phi[999];
  int reco_cemc_n[999], reco_ihcal_n[999], reco_ohcal_n[999];
  float reco_cemcEsum[999], reco_ihcalEsum[999], reco_ohcalEsum[999];
  outRecoTree->Branch("event",&reco_event,"event/I");
  outRecoTree->Branch("reco_n",&reco_n,"reco_n/I");
  outRecoTree->Branch("reco_e",&reco_e,"reco_e[reco_n]/F");
  outRecoTree->Branch("reco_p",&reco_p,"reco_p[reco_n]/F");
  outRecoTree->Branch("reco_pt",&reco_pt,"reco_pt[reco_n]/F");
  outRecoTree->Branch("reco_eta",&reco_eta,"reco_eta[reco_n]/F");
  outRecoTree->Branch("reco_phi",&reco_phi,"reco_phi[reco_n]/F");
  outRecoTree->Branch("reco_cemc_n",&reco_cemc_n,"reco_cemc_n[reco_n]/I");
  outRecoTree->Branch("reco_ihcal_n",&reco_ihcal_n,"reco_ihcal_n[reco_n]/I");
  outRecoTree->Branch("reco_ohcal_n",&reco_ohcal_n,"reco_ohcal_n[reco_n]/I");
  outRecoTree->Branch("reco_cemcEsum",&reco_cemcEsum,"reco_cemcEsum[reco_n]/F");
  outRecoTree->Branch("reco_ihcalEsum",&reco_ihcalEsum,"reco_ihcalEsum[reco_n]/F");
  outRecoTree->Branch("reco_ohcalEsum",&reco_ohcalEsum,"reco_ohcalEsum[reco_n]/F");
  
  int true_count=0;
  int reco_count=0;
  //// loop over files 
  for(int j=initfile; j<endfile; j++){
   
    string fname;
    if (jetE.compare("20GeV")==0) { fname = Form("./out_%s/QCD15GeV_gamma+jet_R04jet20GeVeta0p6_G4sPHENIX_%s_%d.root",label.c_str(),ihcalType.c_str(),j);}
    else if (jetE.compare("30GeV")==0) { fname = Form("./out_%s/QCD25GeV_gamma+jet_R04jet30GeVeta0p6_G4sPHENIX_%s_%d.root",label.c_str(),ihcalType.c_str(),j);}
    else if (jetE.compare("40GeV")==0) { fname = Form("./out_%s/QCD30GeV_gamma+jet_R04jet40GeVeta0p6_G4sPHENIX_%s_%d.root",label.c_str(),ihcalType.c_str(),j);}
    else if (jetE.compare("50GeV")==0) { fname = Form("./out_%s/QCD35GeV_gamma+jet_R04jet50GeVeta0p6_G4sPHENIX_%s_%d.root",label.c_str(),ihcalType.c_str(),j);}
    //string fname = Form("./out_%s/QCD15_gammajet_R04jet50GeVeta0p6_G4sPHENIX_%s_%d.root",label.c_str(),label.c_str(),j);
    if (j%100==0) cout << fname << endl; 

    //// skip bad files
    TFile* fin = new TFile(fname.c_str()); 
    if (j%100==0) cout <<fin<<endl;
    if(fin->GetSize()<10000){fin->Close(); bad_files++; continue;}
    if(fin->IsZombie()) {fin->Close(); bad_files++; continue;}
    
    TTree* maintree = (TTree*)fin->Get("truthjet4_tree");
    TTree* friendtree = (TTree*)fin->Get("towerjet4_tree");

    if (bad_files >0) cout << "Number of bad files = " << bad_files << endl; 
    
    ////////////////////////////////////////////////////////
    int truthevent; 
    float truthjet4_e, truthjet4_p;
    float truthjet4_pt, truthjet4_eta, truthjet4_phi;
    maintree->SetBranchAddress("event",&truthevent); 
    maintree->SetBranchAddress("truthjet4_e",&truthjet4_e); 
    maintree->SetBranchAddress("truthjet4_p",&truthjet4_p); 
    maintree->SetBranchAddress("truthjet4_pt",&truthjet4_pt); 
    maintree->SetBranchAddress("truthjet4_eta",&truthjet4_eta); 
    maintree->SetBranchAddress("truthjet4_phi",&truthjet4_phi); 
    int truthjet4_cons_n;
    float truthjet4_cons_e[999], truthjet4_cons_p[999];
    float truthjet4_cons_pt[999], truthjet4_cons_eta[999], truthjet4_cons_phi[999];
    float truthjet4_cons_dR[999];
    int truthjet4_cons_pid[999];
    int truthjet4_pg_n;
    int truthjet4_pg_id[999];
    float truthjet4_pg_fract[999];
    int truthjet4_pg_status[999];
    maintree->SetBranchAddress("truthjet4_cons_n",&truthjet4_cons_n); 
    maintree->SetBranchAddress("truthjet4_cons_e",&truthjet4_cons_e); 
    maintree->SetBranchAddress("truthjet4_cons_p",&truthjet4_cons_p); 
    maintree->SetBranchAddress("truthjet4_cons_pt",&truthjet4_cons_pt); 
    maintree->SetBranchAddress("truthjet4_cons_eta",&truthjet4_cons_eta); 
    maintree->SetBranchAddress("truthjet4_cons_phi",&truthjet4_cons_phi); 
    maintree->SetBranchAddress("truthjet4_cons_dR",&truthjet4_cons_dR); 
    maintree->SetBranchAddress("truthjet4_cons_pid",&truthjet4_cons_pid); 
    maintree->SetBranchAddress("truthjet4_pg_n",&truthjet4_pg_n); 
    maintree->SetBranchAddress("truthjet4_pg_id",&truthjet4_pg_id); 
    maintree->SetBranchAddress("truthjet4_pg_fract",&truthjet4_pg_fract); 
    maintree->SetBranchAddress("truthjet4_pg_status",&truthjet4_pg_status); 
    
    ////////////////////////////////////////////////////////
    int towerevent; 
    float towerjet4_e, towerjet4_p;
    float towerjet4_pt, towerjet4_eta, towerjet4_phi;
    friendtree->SetBranchAddress("event",&towerevent); 
    friendtree->SetBranchAddress("towerjet4_e",&towerjet4_e); 
    friendtree->SetBranchAddress("towerjet4_p",&towerjet4_p); 
    friendtree->SetBranchAddress("towerjet4_pt",&towerjet4_pt); 
    friendtree->SetBranchAddress("towerjet4_eta",&towerjet4_eta); 
    friendtree->SetBranchAddress("towerjet4_phi",&towerjet4_phi); 
    ////////// 
    int towerjet4_cemc_n;
    float towerjet4_cemc_scale;
    float towerjet4_cemc_e[999], towerjet4_cemc_p[999];
    float towerjet4_cemc_pt[999], towerjet4_cemc_eta[999], towerjet4_cemc_phi[999];
    float towerjet4_cemc_dR[999];
    friendtree->SetBranchAddress("towerjet4_cemc_n",&towerjet4_cemc_n); 
    friendtree->SetBranchAddress("towerjet4_cemc_scale",&towerjet4_cemc_scale); 
    friendtree->SetBranchAddress("towerjet4_cemc_e",&towerjet4_cemc_e); 
    friendtree->SetBranchAddress("towerjet4_cemc_p",&towerjet4_cemc_p); 
    friendtree->SetBranchAddress("towerjet4_cemc_pt",&towerjet4_cemc_pt); 
    friendtree->SetBranchAddress("towerjet4_cemc_eta",&towerjet4_cemc_eta); 
    friendtree->SetBranchAddress("towerjet4_cemc_phi",&towerjet4_cemc_phi); 
    friendtree->SetBranchAddress("towerjet4_cemc_dR",&towerjet4_cemc_dR); 
    ////////// 
    int towerjet4_ihcal_n;
    float towerjet4_ihcal_scale;
    float towerjet4_ihcal_e[999], towerjet4_ihcal_p[999];
    float towerjet4_ihcal_pt[999], towerjet4_ihcal_eta[999], towerjet4_ihcal_phi[999];
    float towerjet4_ihcal_dR[999];
    friendtree->SetBranchAddress("towerjet4_ihcal_n",&towerjet4_ihcal_n); 
    friendtree->SetBranchAddress("towerjet4_ihcal_scale",&towerjet4_ihcal_scale); 
    friendtree->SetBranchAddress("towerjet4_ihcal_e",&towerjet4_ihcal_e); 
    friendtree->SetBranchAddress("towerjet4_ihcal_p",&towerjet4_ihcal_p); 
    friendtree->SetBranchAddress("towerjet4_ihcal_pt",&towerjet4_ihcal_pt); 
    friendtree->SetBranchAddress("towerjet4_ihcal_eta",&towerjet4_ihcal_eta); 
    friendtree->SetBranchAddress("towerjet4_ihcal_phi",&towerjet4_ihcal_phi); 
    friendtree->SetBranchAddress("towerjet4_ihcal_dR",&towerjet4_ihcal_dR); 
    ////////// 
    int towerjet4_ohcal_n;
    float towerjet4_ohcal_scale;
    float towerjet4_ohcal_e[999], towerjet4_ohcal_p[999];
    float towerjet4_ohcal_pt[999], towerjet4_ohcal_eta[999], towerjet4_ohcal_phi[999];
    float towerjet4_ohcal_dR[999];
    friendtree->SetBranchAddress("towerjet4_ohcal_n",&towerjet4_ohcal_n); 
    friendtree->SetBranchAddress("towerjet4_ohcal_scale",&towerjet4_ohcal_scale); 
    friendtree->SetBranchAddress("towerjet4_ohcal_e",&towerjet4_ohcal_e); 
    friendtree->SetBranchAddress("towerjet4_ohcal_p",&towerjet4_ohcal_p); 
    friendtree->SetBranchAddress("towerjet4_ohcal_pt",&towerjet4_ohcal_pt); 
    friendtree->SetBranchAddress("towerjet4_ohcal_eta",&towerjet4_ohcal_eta); 
    friendtree->SetBranchAddress("towerjet4_ohcal_phi",&towerjet4_ohcal_phi); 
    friendtree->SetBranchAddress("towerjet4_ohcal_dR",&towerjet4_ohcal_dR); 
    
    ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////
    int prev_truthevent=1;
    true_n=0;
    //// loop over truth jet entries
    for (int itrue=0; itrue<maintree->GetEntries(); itrue++){

      maintree->GetEntry(itrue);
       
      if (prev_truthevent !=truthevent) { //match the same event number
        //// KYO!! save previous event to tree;
        //true_event=j*50+prev_truthevent;
        true_event=true_count+prev_truthevent;
        outTrueTree->Fill();
        //cout << " *********** FILL TRUE EVENT: "<< true_event<<" **********"<<endl;
        //// re-initialized for new event
        true_n=0;
        prev_truthevent=truthevent;
      }
      
      true_e[true_n]=truthjet4_e;
      true_p[true_n]=truthjet4_p;
      true_pt[true_n]=truthjet4_pt;
      true_eta[true_n]=truthjet4_eta;
      true_phi[true_n]=truthjet4_phi;
      true_cons_n[true_n]=truthjet4_cons_n;
      true_pg_n[true_n]=truthjet4_pg_n;
      
      //// loop over truth constituents
      //// z = ( p_cons * cos(d_theta)) / p_jet
      float jet_theta = 2*atan(exp(-1*truthjet4_eta));
      float this_theta = -999;
      float this_z = -999;
      float highest_z = -999;
      int highest_pid = -999;
      float em_den = 0;
      float em_num = 0;
      for (int icons =0; icons < truthjet4_cons_n; icons ++) {
        this_theta = 2*atan(exp(-1*truthjet4_cons_eta[icons]));
        this_z = truthjet4_cons_p[icons]*cos(fabs(jet_theta-this_theta))/truthjet4_p;
        if (this_z > highest_z) {
          highest_z = this_z;
          highest_pid = truthjet4_cons_pid[icons];
        }
        //// Denominator: All energy
        em_den += truthjet4_cons_e[icons]; 
        //// Numerator: EM only, using truth ptl inf ( e+-, gamma, pi0, eta) 
        if( (fabs(truthjet4_cons_pid[icons])==11) || (truthjet4_cons_pid[icons]==22) || 
        (truthjet4_cons_pid[icons]==111) || (truthjet4_cons_pid[icons]==221) ){
           em_num += truthjet4_cons_e[icons]; 
        } 
      } 
      true_lcons_z[true_n] = highest_z;
      true_lcons_pid[true_n] = highest_pid;
      true_emfrac[true_n] = em_num/em_den;
     
      //// loop over truth progenitor
      float highest_fract = -999;
      int highest_id = -999;
      int highest_status = -999;
      for (int ipg =0; ipg < truthjet4_pg_n; ipg ++) {
        if (truthjet4_pg_fract[ipg] > highest_fract) {
          highest_fract = truthjet4_pg_fract[ipg];
          highest_id = truthjet4_pg_id[ipg];
          highest_status = truthjet4_pg_status[ipg];
        }
      } 
      true_lpg_id[true_n] = highest_id;
      true_lpg_fract[true_n] = highest_fract;
      true_lpg_status[true_n] = highest_status;
      ////
      
      true_n++;
    } // end of truth jet loop
    
    //// KYO!! save LAST event to tree;
    //true_event=j*50+prev_truthevent;
    true_event=true_count+prev_truthevent;
    outTrueTree->Fill();
    //cout << " *********** FILL TRUE EVENT: "<< true_event<<" **********"<<endl;
    
    ///////////////////////////////////////////////////////////////////////////
    int prev_towerevent=1;
    reco_n=0;
    //// loop over reco jet entries
    for (int ireco=0; ireco<friendtree->GetEntries(); ireco++){

      friendtree->GetEntry(ireco);
       
      if (prev_towerevent !=towerevent) { //match the same event number
        //// KYO!! save previous event to tree;
        //reco_event=j*50+prev_towerevent;
        reco_event=reco_count+prev_towerevent;
        outRecoTree->Fill();
        //cout << " *********** FILL RECO EVENT: "<< reco_event<<" **********"<<endl;
        //// re-initialized for new event
        reco_n=0;
        prev_towerevent=towerevent;
      }
      
      reco_e[reco_n]=towerjet4_e;
      reco_p[reco_n]=towerjet4_p;
      reco_pt[reco_n]=towerjet4_pt;
      reco_eta[reco_n]=towerjet4_eta;
      reco_phi[reco_n]=towerjet4_phi;
      reco_cemc_n[reco_n]=towerjet4_cemc_n;
      reco_ihcal_n[reco_n]=towerjet4_ihcal_n;
      reco_ohcal_n[reco_n]=towerjet4_ohcal_n;
      //// loop over tower constituents to get cal fraction 
      float cemc_sum = 0;
      float ihcal_sum = 0;
      float ohcal_sum = 0;
      for (int icemc =0; icemc < towerjet4_cemc_n; icemc ++) {
        cemc_sum += towerjet4_cemc_e[icemc]; 
      } 
      for (int iihcal =0; iihcal < towerjet4_ihcal_n; iihcal ++) {
        ihcal_sum += towerjet4_ihcal_e[iihcal]; 
      } 
      for (int iohcal =0; iohcal < towerjet4_ohcal_n; iohcal ++) {
        ohcal_sum += towerjet4_ohcal_e[iohcal]; 
      } 
      reco_cemcEsum[reco_n]=cemc_sum;
      reco_ihcalEsum[reco_n]=ihcal_sum;
      reco_ohcalEsum[reco_n]=ohcal_sum;
      
      reco_n++;
    } // end of tower jet loop
    
    //// KYO!! save LAST event to tree;
    //reco_event=j*50+prev_towerevent;
    reco_event=reco_count+prev_towerevent;
    outRecoTree->Fill();
    //cout << " *********** FILL RECO EVENT: "<< reco_event<<" **********"<<endl;
    ////

    //cout << "before closing the file "<< j << endl;
    true_count =true_event;
    reco_count =reco_event;
    if (true_count%5000==0) {
      cout << "true_count =" << true_count << ", reco_count =" << reco_count << endl; 
    }
    fin->Close(); 

  } // end of file loop

  outFile->cd();
  outTrueTree->Write();
  outRecoTree->Write();
  outFile->Close();
   
  return; 
}
