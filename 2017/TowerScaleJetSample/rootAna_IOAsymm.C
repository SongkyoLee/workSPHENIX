
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

// MyTree_Al_NotScaled
// MyTree_Al_Scaled_v3 

void rootAna_emfrac(TString ihcalType = "MyTree_Al_NotScaled",int initfile=0,int endfile =1)
{

  //// Suppress error messages from bad files
  gErrorIgnoreLevel = kFatal; 

  int bad_files = 0; 
 
  ////////////////////////////////////////////////////////
  TH1D* h1D_truthjet4_e = new TH1D("truthjet4_e",";Truth Jet E;",100,0,100);
  TH1D* h1D_truthjet4_p = new TH1D("truthjet4_p",";Truth Jet p;",100,0,100);
  TH1D* h1D_truthjet4_pt = new TH1D("truthjet4_pt",";Truth Jet p_{T};",100,0,100);
  TH1D* h1D_truthjet4_eta = new TH1D("truthjet4_eta",";Truth Jet #eta;",100,-1.5,1.5);
  TH1D* h1D_truthjet4_phi = new TH1D("truthjet4_phi",";Truth Jet #phi;",100,-3.5,3.5);
  TH1D* h1D_truthjet4_cons_n = new TH1D("truthjet4_cons_n",";Truth Jet # of constituents;",100,0,100);
  TH1D* h1D_truthjet4_cons_e = new TH1D("truthjet4_cons_e",";Truth Jet constituents E;",80,0,80);
  TH1D* h1D_truthjet4_cons_p = new TH1D("truthjet4_cons_p",";Truth Jet constituents p;",80,0,80);
  TH1D* h1D_truthjet4_cons_pt = new TH1D("truthjet4_cons_pt",";Truth Jet constituents p_{T};",80,0,80);
  TH1D* h1D_truthjet4_cons_eta = new TH1D("truthjet4_cons_eta",";Truth Jet constituents #eta;",100,-1.5,1.5);
  TH1D* h1D_truthjet4_cons_phi = new TH1D("truthjet4_cons_phi",";Truth Jet constituents #phi;",100,-3.5,3.5);
  TH1D* h1D_truthjet4_cons_dR = new TH1D("truthjet4_cons_dR",";Truth Jet constituents dR;",100,0.,1.0);
  TH1D* h1D_truthjet4_cons_pid = new TH1D("truthjet4_cons_pid",";Truth Jet constituents pid;",5000,-2500,2500);
  ////////////////////////////////////////////////////////
  TH1D* h1D_towerjet4_e = new TH1D("towerjet4_e",";Tower Jet E;",100,0,100);
  TH1D* h1D_towerjet4_p = new TH1D("towerjet4_p",";Tower Jet p;",100,0,100);
  TH1D* h1D_towerjet4_pt = new TH1D("towerjet4_pt",";Tower Jet p_{T};",100,0,100);
  TH1D* h1D_towerjet4_eta = new TH1D("towerjet4_eta",";Tower Jet #eta;",100,-1.5,1.5);
  TH1D* h1D_towerjet4_phi = new TH1D("towerjet4_phi",";Tower Jet #phi;",100,-3.5,3.5);
  TH1D* h1D_towerjet4_cemc_n = new TH1D("towerjet4_cemc_n",";Tower Jet # of cemc;",300,0,300);
//  TH1D* h1D_towerjet4_cemc_e = new TH1D("towerjet4_cemc_e",";Tower Jet cemc E;",100,0,50);
  TH1D* h1D_towerjet4_cemc_e = new TH1D("towerjet4_cemc_e",";Tower Jet cemc E;",100,0,1);
  TH1D* h1D_towerjet4_cemc_p = new TH1D("towerjet4_cemc_p",";Tower Jet cemc p;",100,0,50);
  TH1D* h1D_towerjet4_cemc_pt = new TH1D("towerjet4_cemc_pt",";Tower Jet cemc p_{T};",100,0,50);
  TH1D* h1D_towerjet4_cemc_eta = new TH1D("towerjet4_cemc_eta",";Tower Jet cemc #eta;",100,-1.5,1.5);
  TH1D* h1D_towerjet4_cemc_phi = new TH1D("towerjet4_cemc_phi",";Tower Jet cemc #phi;",100,-3.5,3.5);
  TH1D* h1D_towerjet4_cemc_dR = new TH1D("towerjet4_cemc_dR",";Tower Jet cemc dR;",100,0.,1.0);
  ////////////////////////////////////////////////////////
  TH1D* h1D_towerjet4_ihcal_n = new TH1D("towerjet4_ihcal_n",";Tower Jet # of ihcal;",100,0,100);
  //TH1D* h1D_towerjet4_ihcal_e = new TH1D("towerjet4_ihcal_e",";Tower Jet ihcal E;",100,0,20);
  TH1D* h1D_towerjet4_ihcal_e = new TH1D("towerjet4_ihcal_e",";Tower Jet ihcal E;",100,0,1);
  TH1D* h1D_towerjet4_ihcal_p = new TH1D("towerjet4_ihcal_p",";Tower Jet ihcal p;",100,0,20);
  TH1D* h1D_towerjet4_ihcal_pt = new TH1D("towerjet4_ihcal_pt",";Tower Jet ihcal p_{T};",100,0,20);
  TH1D* h1D_towerjet4_ihcal_eta = new TH1D("towerjet4_ihcal_eta",";Tower Jet ihcal #eta;",100,-1.5,1.5);
  TH1D* h1D_towerjet4_ihcal_phi = new TH1D("towerjet4_ihcal_phi",";Tower Jet ihcal #phi;",100,-3.5,3.5);
  TH1D* h1D_towerjet4_ihcal_dR = new TH1D("towerjet4_ihcal_dR",";Tower Jet ihcal dR;",100,0.,1.0);
  ////////////////////////////////////////////////////////
  TH1D* h1D_towerjet4_ohcal_n = new TH1D("towerjet4_ohcal_n",";Tower Jet # of ohcal;",100,0,100);
  //TH1D* h1D_towerjet4_ohcal_e = new TH1D("towerjet4_ohcal_e",";Tower Jet ohcal E;",100,0,50);
  TH1D* h1D_towerjet4_ohcal_e = new TH1D("towerjet4_ohcal_e",";Tower Jet ohcal E;",100,0,1);
  TH1D* h1D_towerjet4_ohcal_p = new TH1D("towerjet4_ohcal_p",";Tower Jet ohcal p;",100,0,50);
  TH1D* h1D_towerjet4_ohcal_pt = new TH1D("towerjet4_ohcal_pt",";Tower Jet ohcal p_{T};",100,0,50);
  TH1D* h1D_towerjet4_ohcal_eta = new TH1D("towerjet4_ohcal_eta",";Tower Jet ohcal #eta;",100,-1.5,1.5);
  TH1D* h1D_towerjet4_ohcal_phi = new TH1D("towerjet4_ohcal_phi",";Tower Jet ohcal #phi;",100,-3.5,3.5);
  TH1D* h1D_towerjet4_ohcal_dR = new TH1D("towerjet4_ohcal_dR",";Tower Jet ohcal dR;",100,0.,1.0);
  ////////////////////////////////////////////////////////
  //TH2D* h2D_JES_emfrac = new TH2D("h2D_JES_emfrac",";Truth Jet EM fraction;JES (=E_{Tower Jet}/E_{Truth Jet})",100, 0., 1., 100, 0, 1.5);
  //TH2D* h2D_JES_calfrac = new TH2D("h2D_JES_calfrac",";(E_{HCAL}-E_{EMCAL})/(E_{HCAL}+E_{EMCAL});JES (=E_{Tower Jet}/E_{Truth Jet})",100, -1., 1., 100, 0, 1.5);
  TH2D* h2D_JES_emfrac = new TH2D("h2D_JES_emfrac",";Truth Jet EM fraction;JES (=E_{Tower Jet}/E_{Truth Jet})",100, 0., 1., 100, 0, 2.0);
  TH2D* h2D_JES_calfrac = new TH2D("h2D_JES_calfrac",";(E_{HCAL}-E_{EMCAL})/(E_{HCAL}+E_{EMCAL});JES (=E_{Tower Jet}/E_{Truth Jet})",100, -1., 1., 100, 0, 2.0);
  ////////////////////////////////////////////////////////

  //// loop over files 
  for(int j=initfile; j<endfile; j++){
    
    TString fname = Form("./out_%s/DVP_QCD35_10k_G4sPHENIX_%s_%d.root",ihcalType.Data(),ihcalType.Data(),j);
    if (j%100==0) cout << fname << endl; 

    //// skip bad files
    TFile* fin = new TFile(fname); 
    if(fin->GetSize()<10000){fin->Close(); bad_files++; continue;}
    if(fin->IsZombie()) {fin->Close(); bad_files++; continue;}
    
    TTree* maintree = (TTree*)fin->Get("truthjet4_tree");
    TTree* friendtree = (TTree*)fin->Get("towerjet4_tree");
    //cout << "truthjet4_tree entries:" << maintree->GetEntries() << endl; 
    //cout << "towerjet4_tree entries:" << friendtree->GetEntries() << endl; 
    //TTree* treeF = new TTree();
    //treeF->AddFriend("truthjet4_tree",fin);
    //treeF->AddFriend("towerjet4_tree",fin);
    //treeF->Print(); 
    //maintree->AddFriend("towerjet4_tree",fin);

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
//    float truthjet4_cons_pid[999]; // KYO: to be modified!!!!!
    int truthjet4_cons_pid[999];
    maintree->SetBranchAddress("truthjet4_cons_n",&truthjet4_cons_n); 
    maintree->SetBranchAddress("truthjet4_cons_e",&truthjet4_cons_e); 
    maintree->SetBranchAddress("truthjet4_cons_p",&truthjet4_cons_p); 
    maintree->SetBranchAddress("truthjet4_cons_pt",&truthjet4_cons_pt); 
    maintree->SetBranchAddress("truthjet4_cons_eta",&truthjet4_cons_eta); 
    maintree->SetBranchAddress("truthjet4_cons_phi",&truthjet4_cons_phi); 
    maintree->SetBranchAddress("truthjet4_cons_dR",&truthjet4_cons_dR); 
    maintree->SetBranchAddress("truthjet4_cons_pid",&truthjet4_cons_pid); 
    
    int towerevent; 
    float towerjet4_e, towerjet4_p;
    float towerjet4_pt, towerjet4_eta, towerjet4_phi;
    //maintree->SetBranchAddress("towerjet4_tree.event",&towerevent); 
    friendtree->SetBranchAddress("event",&towerevent); 
    friendtree->SetBranchAddress("towerjet4_e",&towerjet4_e); 
    friendtree->SetBranchAddress("towerjet4_p",&towerjet4_p); 
    friendtree->SetBranchAddress("towerjet4_pt",&towerjet4_pt); 
    friendtree->SetBranchAddress("towerjet4_eta",&towerjet4_eta); 
    friendtree->SetBranchAddress("towerjet4_phi",&towerjet4_phi); 
    
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
    //// 1) Fill the basic truth jet info
    for (int itrue=0; itrue<maintree->GetEntries(); itrue++){
      maintree->GetEntry(itrue);
		  h1D_truthjet4_e->Fill(truthjet4_e);
		  h1D_truthjet4_p->Fill(truthjet4_p);
		  h1D_truthjet4_pt->Fill(truthjet4_pt);
		  h1D_truthjet4_eta->Fill(truthjet4_eta);
		  h1D_truthjet4_phi->Fill(truthjet4_phi);
		  h1D_truthjet4_cons_n->Fill(truthjet4_cons_n);
      for (int icons =0; icons < truthjet4_cons_n; icons ++) {
        h1D_truthjet4_cons_e->Fill(truthjet4_cons_e[icons]);
        h1D_truthjet4_cons_p->Fill(truthjet4_cons_p[icons]);
        h1D_truthjet4_cons_pt->Fill(truthjet4_cons_pt[icons]);
        h1D_truthjet4_cons_eta->Fill(truthjet4_cons_eta[icons]);
        h1D_truthjet4_cons_phi->Fill(truthjet4_cons_phi[icons]);
        h1D_truthjet4_cons_dR->Fill(truthjet4_cons_dR[icons]);
        h1D_truthjet4_cons_pid->Fill(truthjet4_cons_pid[icons]);
		  } 
    }
    ////////////////////////////////////////////////////////
    //// 2) Fill the basic tower jet info
    for (int itwr=0; itwr<friendtree->GetEntries(); itwr++){
      friendtree->GetEntry(itwr);
		  h1D_towerjet4_e->Fill(towerjet4_e);
		  h1D_towerjet4_p->Fill(towerjet4_p);
		  h1D_towerjet4_pt->Fill(towerjet4_pt);
		  h1D_towerjet4_eta->Fill(towerjet4_eta);
		  h1D_towerjet4_phi->Fill(towerjet4_phi);
		  h1D_towerjet4_cemc_n->Fill(towerjet4_cemc_n);
      for (int icemc =0; icemc < towerjet4_cemc_n; icemc ++) {
        h1D_towerjet4_cemc_e->Fill(towerjet4_cemc_e[icemc]);
        h1D_towerjet4_cemc_p->Fill(towerjet4_cemc_p[icemc]);
        h1D_towerjet4_cemc_pt->Fill(towerjet4_cemc_pt[icemc]);
        h1D_towerjet4_cemc_eta->Fill(towerjet4_cemc_eta[icemc]);
        h1D_towerjet4_cemc_phi->Fill(towerjet4_cemc_phi[icemc]);
        h1D_towerjet4_cemc_dR->Fill(towerjet4_cemc_dR[icemc]);
		  } 
		  h1D_towerjet4_ihcal_n->Fill(towerjet4_ihcal_n);
      for (int iihcal =0; iihcal < towerjet4_ihcal_n; iihcal ++) {
        h1D_towerjet4_ihcal_e->Fill(towerjet4_ihcal_e[iihcal]);
        h1D_towerjet4_ihcal_p->Fill(towerjet4_ihcal_p[iihcal]);
        h1D_towerjet4_ihcal_pt->Fill(towerjet4_ihcal_pt[iihcal]);
        h1D_towerjet4_ihcal_eta->Fill(towerjet4_ihcal_eta[iihcal]);
        h1D_towerjet4_ihcal_phi->Fill(towerjet4_ihcal_phi[iihcal]);
        h1D_towerjet4_ihcal_dR->Fill(towerjet4_ihcal_dR[iihcal]);
		  } 
		  h1D_towerjet4_ohcal_n->Fill(towerjet4_ohcal_n);
      for (int iohcal =0; iohcal < towerjet4_ohcal_n; iohcal ++) {
        h1D_towerjet4_ohcal_e->Fill(towerjet4_ohcal_e[iohcal]);
        h1D_towerjet4_ohcal_p->Fill(towerjet4_ohcal_p[iohcal]);
        h1D_towerjet4_ohcal_pt->Fill(towerjet4_ohcal_pt[iohcal]);
        h1D_towerjet4_ohcal_eta->Fill(towerjet4_ohcal_eta[iohcal]);
        h1D_towerjet4_ohcal_phi->Fill(towerjet4_ohcal_phi[iohcal]);
        h1D_towerjet4_ohcal_dR->Fill(towerjet4_ohcal_dR[iohcal]);
		  } 
    }
    ////////////////////////////////////////////////////////
    //// 3) Fill need-more-calculation info 
    
    //// loop over truth jet entries
    //for (int itrue=0; itrue<maintree->GetEntries(); itrue++){
    for (int itrue=0; itrue<6; itrue++){

      maintree->GetEntry(itrue);
      
      if (truthjet4_e < 50) continue;
      if (fabs(truthjet4_eta) > 0.6) continue;
       
      //// matching tower jets (smallest dR) to get JES
      float jes_ratio = 999; //finally needed
      float match_dR = 999;
      float tmp_dR = 999;
      int match_idx = 999;
      for (int itwr=0; itwr<friendtree->GetEntries(); itwr++){
        friendtree->GetEntry(itwr);
        if (truthevent!=towerevent) { continue; } // only in the same event
        tmp_dR = sqrt(pow(truthjet4_eta-towerjet4_eta,2)+ pow(truthjet4_phi-towerjet4_phi,2));
        if (tmp_dR < match_dR){ 
          match_dR = tmp_dR;
          match_idx = itwr;
        }
      }
      //cout << "match_dR = " << match_dR << endl; 
      if (match_dR > 0.4) continue; 
      //if (match_dR > 0.3) continue; 
      
      //// BEST MATCHED tower jet!! 
      friendtree->GetEntry(match_idx);

      jes_ratio = towerjet4_e/truthjet4_e;
      //cout << "jes_ratio = " << jes_ratio << endl; 
      
      //// loop over tower constituents to get cal fraction 
      float cal_frac = 999; //finally needed
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
      cal_frac = (ihcal_sum+ohcal_sum-cemc_sum)/(ihcal_sum+ohcal_sum+cemc_sum);
      //cout << "cal_frac = " << cal_frac << endl;
      
      //// loop over truth constituents to get em fraction 
      float em_frac = 999; //finally needed 
      float em_den = 0;
      float em_num = 0;
      for (int icons =0; icons < truthjet4_cons_n; icons ++) {
        //// Denominator: All energy
        em_den += truthjet4_cons_e[icons]; 
        //// Numerator: EM only, using truth ptl inf ( e+-, gamma, pi0, eta) 
        if( (fabs(truthjet4_cons_pid[icons])==11) || (truthjet4_cons_pid[icons]==22) || 
        (truthjet4_cons_pid[icons]==111) || (truthjet4_cons_pid[icons]==221) ){
           em_num += truthjet4_cons_e[icons]; 
        } 
      } 
      em_frac = em_num/em_den;
      //cout << "em_frac = " << em_frac << endl;
      
      h2D_JES_calfrac->Fill(cal_frac,jes_ratio); 
      h2D_JES_emfrac->Fill(em_frac,jes_ratio); 
    
    } // end of truth jet loop
  
    fin->Close(); 
  } // end of file loop

  //TCanvas* c1 = new TCanvas("c1","",600,600);
  //h2D_JES_emfrac->Draw();

  h2D_JES_calfrac->SetOption("colz");
  h2D_JES_emfrac->SetOption("colz");

  TFile* fout = new TFile(Form("outEMfrac/emfrac_%s.root",ihcalType.Data()),"RECREATE");
  fout->cd();
  h1D_truthjet4_e->Write();
  h1D_truthjet4_p->Write();
  h1D_truthjet4_pt->Write();
  h1D_truthjet4_eta->Write();
  h1D_truthjet4_phi->Write();
  h1D_truthjet4_cons_n->Write();
  h1D_truthjet4_cons_e->Write();
  h1D_truthjet4_cons_p->Write();
  h1D_truthjet4_cons_pt->Write();
  h1D_truthjet4_cons_eta->Write();
  h1D_truthjet4_cons_phi->Write();
  h1D_truthjet4_cons_dR->Write();
  h1D_truthjet4_cons_pid->Write();
  h1D_towerjet4_e->Write();
  h1D_towerjet4_p->Write();
  h1D_towerjet4_pt->Write();
  h1D_towerjet4_eta->Write();
  h1D_towerjet4_phi->Write();
  h1D_towerjet4_cemc_n->Write();
  h1D_towerjet4_cemc_e->Write();
  h1D_towerjet4_cemc_p->Write();
  h1D_towerjet4_cemc_pt->Write();
  h1D_towerjet4_cemc_eta->Write();
  h1D_towerjet4_cemc_phi->Write();
  h1D_towerjet4_cemc_dR->Write();
  h1D_towerjet4_ihcal_n->Write();
  h1D_towerjet4_ihcal_e->Write();
  h1D_towerjet4_ihcal_p->Write();
  h1D_towerjet4_ihcal_pt->Write();
  h1D_towerjet4_ihcal_eta->Write();
  h1D_towerjet4_ihcal_phi->Write();
  h1D_towerjet4_ihcal_dR->Write();
  h1D_towerjet4_ohcal_n->Write();
  h1D_towerjet4_ohcal_e->Write();
  h1D_towerjet4_ohcal_p->Write();
  h1D_towerjet4_ohcal_pt->Write();
  h1D_towerjet4_ohcal_eta->Write();
  h1D_towerjet4_ohcal_phi->Write();
  h1D_towerjet4_ohcal_dR->Write();
  h2D_JES_calfrac->Write();
  h2D_JES_emfrac->Write();
  fout->Close();

  return; 
}
