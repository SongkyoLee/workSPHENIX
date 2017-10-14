
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

// number of files...
#define ITER_START 0
//#define NUM_FILE 1000
#define NUM_FILE 100

// MyTree_Al_NotScaled
// MyTree_Al_Scaled_v3 

void rootAna_emfrac(TString ihcalType = "MyTree_Al_NotScaled")
{

  //// Suppress error messages from bad files
  gErrorIgnoreLevel = kFatal; 

  int bad_files = 0; 
  
  TH2D* h2D_JES_emfrac = new TH2D("h2D_JES_emfrac",";Truth Jet EM fraction;JES (Jet E_{reco}/E_{truth})",100, 0., 1., 100, 0, 1.5);

  for(int j=ITER_START; j<NUM_FILE; j++){
    
    TString fname = Form("./out_%s/DVP_QCD35_10k_G4sPHENIX_%s_%d.root",ihcalType.Data(),ihcalType.Data(),j);
    cout << fname << endl; 

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
    float truthjet4_cons_pid[999]; // KYO: to be modified!!!!!
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
 
    ////////////////////////////////////////////////////////
    //// loop over truth jet entries
    for (int itrue=0; itrue<maintree->GetEntries(); itrue++){
      maintree->GetEntry(itrue);
      //cout << "******** itrue = " << itrue << endl; 
      //cout << "truthevent :" << truthevent << endl; 
      
      //// find matched tower jets and get ratios reco/truth energy
      float match_dR = 999;
      float tmp_dR = 999;
      float matchjet4_e = 999;
      float ratio_e = 999; 
    for (int itwr=0; itwr<friendtree->GetEntries(); itwr++){
        friendtree->GetEntry(itwr);
        if (truthevent!=towerevent) { continue; }
        tmp_dR = sqrt(pow(truthjet4_eta-towerjet4_eta,2)+ pow(truthjet4_phi-towerjet4_phi,2));
        if (tmp_dR < match_dR){ 
          match_dR = tmp_dR;
          matchjet4_e = towerjet4_e; 
        }
      }
      //// BEST MATCH 
      ratio_e = matchjet4_e/truthjet4_e;
      //cout << "match_dR = " << match_dR << endl; 
      //cout << "truthjet4_e = " << truthjet4_e << endl; 
      //cout << "matchjet4_e = " << matchjet4_e << endl; 
      //cout << "ratio_e = " << ratio_e << endl; 
      if (match_dR > 0.4) continue; 
      //// check constituents and get EM fraction 
      float em_den = 0;
      float em_num = 0;
      float em_frac = 999; 
      for (int icons =0; icons < truthjet4_cons_n; icons ++) {
        //// Denominator: All energy
        em_den += truthjet4_cons_e[icons]; 
        //cout << "  * truthjet4_cons_pid[icons]: "<<truthjet4_cons_pid[icons]<<endl;
        //// Numerator: EM only, using truth particle info: e+-, gamma, pi0, eta: 
        if( (fabs(truthjet4_cons_pid[icons])==11) || (truthjet4_cons_pid[icons]==22) || 
        (truthjet4_cons_pid[icons]==111) || (truthjet4_cons_pid[icons]==221) ){
           em_num += truthjet4_cons_e[icons]; 
        } 
      } 
      em_frac = em_num/em_den;
      //cout << "em_den : " << em_den << endl;
      //cout << "em_num : " << em_num << endl;
      //cout << "em_frac : " << em_frac << endl;
      
      h2D_JES_emfrac->Fill(em_frac,ratio_e); 
    
    } // end of truth jet loop
  
    fin->Close(); 
  } // end of file loop

  //TCanvas* c1 = new TCanvas("c1","",600,600);
  //h2D_JES_emfrac->Draw();

  TFile* fout = new TFile(Form("out_emfrac/emfrac_%s.root",ihcalType.Data()),"RECREATE");
  fout->cd();
  h2D_JES_emfrac->Write();
  fout->Close();

  return; 
}
