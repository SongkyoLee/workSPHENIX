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
#include "TLorentzVector.h"

#include <string.h>
#include "TChain.h"
#include "TTree.h"
#include "TKey.h"
#include "Riostream.h"

#include "TError.h"

#include <vector>
 
////// variables for new true_tree 
int this_true_event=1; //start from event 1!! 
float vtx_x, vtx_y, vtx_z;
int true_n;
float true_e[999], true_p[999], true_pt[999], true_eta[999], true_phi[999], true_m[999];
//// only store leading cons info. (one per jet)
int true_cons_n[999];
float true_lcons_z[999];
int true_lcons_pid[999];
float true_emfrac[999];
//// only store leading pg info. (one per jet)
int true_pg_n[999];
int true_lpg_id[999];
float true_lpg_fract[999];
int true_lpg_status[999];

////// variables for new reco_tree 
int this_reco_event=1; //start from event 1!! 
int reco_n;
float reco_e[999], reco_p[999], reco_pt[999], reco_eta[999], reco_phi[999], reco_m[999];
int reco_cemc_n[999], reco_ihcal_n[999], reco_ohcal_n[999];
float reco_cemcEsum[999], reco_ihcalEsum[999], reco_ohcalEsum[999];
int reco_clcemc_n[999];
int reco_clcemc_had_n[999], reco_clcemc_em_n[999];
float reco_clcemc_hadEsum[999], reco_clcemc_emEsum[999], reco_clcemc_totEsum[999];
int reco_clihcal_n[999];
float reco_clihcal_totEsum[999];
int reco_clohcal_n[999];
float reco_clohcal_totEsum[999];
float orig_reco_e[999], orig_reco_p[999], orig_reco_pt[999], orig_reco_eta[999], orig_reco_phi[999], orig_reco_m[999];

void FillEmptyTrueEvents();
void FillEmptyRecoEvents();

void rootAna_1stSort_lorentz(string label = "QCDJet20GeVR04_SS310",
                            int initfile=0,
                            int endfile = 5, 
                            float total_sf = 1.00,
                            float cemc_sf = 1.00,
                            float ihcal_sf = 1.00,
                            float ohcal_sf =1.00,
                            bool doetopcut=true, 
                            bool dodphicut=true)
{

  bool doscale=false;
  if (total_sf!=1 || cemc_sf !=1 || ihcal_sf !=1 || ohcal_sf!=1) {doscale=true;};

  //// etop cut
  TFile* fetop = new TFile("out1DHist/emi_etop.root","READ");
  TGraphErrors* g_sigma = (TGraphErrors*)fetop->Get("g_sigma");
  cout << "g_sigma = " << g_sigma << endl;
  const int np = g_sigma->GetN();
  cout << "np = " << np << endl;
  double px[17];
  double sig[17];
  double ex[17];
  double xlow[17];
  double xhigh[17];
  
  for (int ip=0; ip < np; ip++){
    g_sigma->GetPoint(ip, px[ip], sig[ip]); 
    xlow[ip] = px[ip]-g_sigma->GetErrorX(ip);
    xhigh[ip] = px[ip]+g_sigma->GetErrorX(ip);
    if (ip ==np-1) {xhigh[ip]= 531;}
    //cout << ip << "th px = "<< px[ip] << ", sig = " << sig[ip] << endl;
    cout << ip << "th xlow = "<< xlow[ip] << ", xhigh = " << xhigh[ip] << endl;
    cout << 1-3*sig[ip] << " < E/p < "<< 1+3*sig[ip] << endl;
  }


  //// Suppress error messages from bad files
  gErrorIgnoreLevel = kFatal; 

  int bad_files = 0; 

  string sampleType;
  string ihcalType;
  string jetE;
  string note;
 
  if (label.compare("py8GammaJet_vtx10gaus_SS310")==0){ sampleType = "GammaJet"; ihcalType = "SS310"; jetE="allGeV"; note = "vtx10gaus"; }
  else if (label.compare("py8GammaJet_vtx10gaus_Alframe")==0){ sampleType = "GammaJet"; ihcalType = "Alframe"; jetE="allGeV"; note = "vtx10gaus"; }
  else if (label.compare("py8GammaJet_vtx10gaus_Al")==0){ sampleType = "GammaJet"; ihcalType = "Al"; jetE="allGeV"; note = "vtx10gaus"; }
  else if (label.compare("py8Jet_vtx10gaus_SS310")==0){ sampleType = "QCDJet"; ihcalType = "SS310"; jetE="allGeV"; note = "vtx10gaus"; }
  else if (label.compare("py8Jet_vtx10gaus_Alframe")==0){ sampleType = "QCDJet"; ihcalType = "Alframe"; jetE="allGeV"; note = "vtx10gaus"; }
  else if (label.compare("py8Jet_vtx10gaus_Al")==0){ sampleType = "QCDJet"; ihcalType = "Al"; jetE="allGeV"; note = "vtx10gaus"; }
  else if (label.compare("py8jet_highz_SS310")==0){ sampleType = "QCDJet"; ihcalType = "SS310"; jetE="allGeV"; note = "highz"; }
  else if (label.compare("py8jet_highz_Alframe")==0){ sampleType = "QCDJet"; ihcalType = "Alframe"; jetE="allGeV"; note = "highz"; }
  else if (label.compare("py8jet_noptcut_highz_SS310")==0){ sampleType = "QCDJet"; ihcalType = "SS310"; jetE="allGeV"; note = "noptcut_highz"; }
  else if (label.compare("py8jet_noptcut_highz_Alframe")==0){ sampleType = "QCDJet"; ihcalType = "Alframe"; jetE="allGeV"; note = "noptcut_highz"; }
  else if (label.compare("py8jet_noptcut_allz_SS310")==0){ sampleType = "QCDJet"; ihcalType = "SS310"; jetE="allGeV"; note = "noptcut_allz"; }
  else if (label.compare("py8jet_noptcut_allz_Alframe")==0){ sampleType = "QCDJet"; ihcalType = "Alframe"; jetE="allGeV"; note = "noptcut_allz"; }
  else if (label.compare("GammaJet20GeV_SS310")==0){ sampleType = "GammaJet"; ihcalType = "SS310"; jetE="20GeV"; note = "vtx0"; }
  else if (label.compare("GammaJet30GeV_SS310")==0){ sampleType = "GammaJet"; ihcalType = "SS310"; jetE="30GeV"; note = "vtx0"; }
  else if (label.compare("GammaJet40GeV_SS310")==0){ sampleType = "GammaJet"; ihcalType = "SS310"; jetE="40GeV"; note = "vtx0"; }
  else if (label.compare("GammaJet50GeV_SS310")==0){ sampleType = "GammaJet"; ihcalType = "SS310"; jetE="50GeV"; note = "vtx0"; }
  else if (label.compare("GammaJet20GeV_Al")==0){ sampleType = "GammaJet"; ihcalType = "Al"; jetE="20GeV"; note = "vtx0"; }
  else if (label.compare("GammaJet30GeV_Al")==0){ sampleType = "GammaJet"; ihcalType = "Al"; jetE="30GeV"; note = "vtx0"; }
  else if (label.compare("GammaJet40GeV_Al")==0){ sampleType = "GammaJet"; ihcalType = "Al"; jetE="40GeV"; note = "vtx0"; }
  else if (label.compare("GammaJet50GeV_Al")==0){ sampleType = "GammaJet"; ihcalType = "Al"; jetE="50GeV"; note = "vtx0"; }
  else if (label.compare("GammaJet20GeV_Alframe")==0){ sampleType = "GammaJet"; ihcalType = "Alframe"; jetE="20GeV"; note = "vtx0"; }
  else if (label.compare("GammaJet30GeV_Alframe")==0){ sampleType = "GammaJet"; ihcalType = "Alframe"; jetE="30GeV"; note = "vtx0"; }
  else if (label.compare("GammaJet40GeV_Alframe")==0){ sampleType = "GammaJet"; ihcalType = "Alframe"; jetE="40GeV"; note = "vtx0"; }
  else if (label.compare("GammaJet50GeV_Alframe")==0){ sampleType = "GammaJet"; ihcalType = "Alframe"; jetE="50GeV"; note = "vtx0"; }
  else if (label.compare("QCDJet20GeVR02_SS310")==0){ sampleType = "QCDJet"; ihcalType = "SS310"; jetE="20GeV"; note = "R02"; }
  else if (label.compare("QCDJet30GeVR02_SS310")==0){ sampleType = "QCDJet"; ihcalType = "SS310"; jetE="30GeV"; note = "R02"; }
  else if (label.compare("QCDJet40GeVR02_SS310")==0){ sampleType = "QCDJet"; ihcalType = "SS310"; jetE="40GeV"; note = "R02"; }
  else if (label.compare("QCDJet50GeVR02_SS310")==0){ sampleType = "QCDJet"; ihcalType = "SS310"; jetE="50GeV"; note = "R02"; }
  else if (label.compare("QCDJet20GeVR02_Al")==0){ sampleType = "QCDJet"; ihcalType = "Al"; jetE="20GeV"; note = "R02"; }
  else if (label.compare("QCDJet30GeVR02_Al")==0){ sampleType = "QCDJet"; ihcalType = "Al"; jetE="30GeV"; note = "R02"; }
  else if (label.compare("QCDJet40GeVR02_Al")==0){ sampleType = "QCDJet"; ihcalType = "Al"; jetE="40GeV"; note = "R02"; }
  else if (label.compare("QCDJet50GeVR02_Al")==0){ sampleType = "QCDJet"; ihcalType = "Al"; jetE="50GeV"; note = "R02"; }
  else if (label.compare("QCDJet20GeVR02_Alframe")==0){ sampleType = "QCDJet"; ihcalType = "Alframe"; jetE="20GeV"; note = "R02"; }
  else if (label.compare("QCDJet30GeVR02_Alframe")==0){ sampleType = "QCDJet"; ihcalType = "Alframe"; jetE="30GeV"; note = "R02"; }
  else if (label.compare("QCDJet40GeVR02_Alframe")==0){ sampleType = "QCDJet"; ihcalType = "Alframe"; jetE="40GeV"; note = "R02"; }
  else if (label.compare("QCDJet50GeVR02_Alframe")==0){ sampleType = "QCDJet"; ihcalType = "Alframe"; jetE="50GeV"; note = "R02"; }
  else if (label.compare("QCDJet20GeVR04_SS310")==0){ sampleType = "QCDJet"; ihcalType = "SS310"; jetE="20GeV"; note = "R04"; }
  else if (label.compare("QCDJet30GeVR04_SS310")==0){ sampleType = "QCDJet"; ihcalType = "SS310"; jetE="30GeV"; note = "R04"; }
  else if (label.compare("QCDJet40GeVR04_SS310")==0){ sampleType = "QCDJet"; ihcalType = "SS310"; jetE="40GeV"; note = "R04"; }
  else if (label.compare("QCDJet50GeVR04_SS310")==0){ sampleType = "QCDJet"; ihcalType = "SS310"; jetE="50GeV"; note = "R04"; }
  else if (label.compare("QCDJet20GeVR04_Al")==0){ sampleType = "QCDJet"; ihcalType = "Al"; jetE="20GeV"; note = "R04"; }
  else if (label.compare("QCDJet30GeVR04_Al")==0){ sampleType = "QCDJet"; ihcalType = "Al"; jetE="30GeV"; note = "R04"; }
  else if (label.compare("QCDJet40GeVR04_Al")==0){ sampleType = "QCDJet"; ihcalType = "Al"; jetE="40GeV"; note = "R04"; }
  else if (label.compare("QCDJet50GeVR04_Al")==0){ sampleType = "QCDJet"; ihcalType = "Al"; jetE="50GeV"; note = "R04"; }
  else if (label.compare("QCDJet20GeVR04_Alframe")==0){ sampleType = "QCDJet"; ihcalType = "Alframe"; jetE="20GeV"; note = "R04"; }
  else if (label.compare("QCDJet30GeVR04_Alframe")==0){ sampleType = "QCDJet"; ihcalType = "Alframe"; jetE="30GeV"; note = "R04"; }
  else if (label.compare("QCDJet40GeVR04_Alframe")==0){ sampleType = "QCDJet"; ihcalType = "Alframe"; jetE="40GeV"; note = "R04"; }
  else if (label.compare("QCDJet50GeVR04_Alframe")==0){ sampleType = "QCDJet"; ihcalType = "Alframe"; jetE="50GeV"; note = "R04"; }
  else { cout << "KYO WARNING: select proper ihcal type"<<endl; return; }

  //// write new tree 
  //string foutname = Form("./sortedRootFiles/sorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_%s_doetopcut%d_dodphicut%d.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),note.c_str(),(int)doetopcut,(int)dodphicut);
  string foutname = Form("./sortedRootFilesLorentz/sorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_%s_doetopcut%d_dodphicut%d_doscale%d.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),note.c_str(),(int)doetopcut,(int)dodphicut,(int)doscale);
  TFile* outFile = new TFile( foutname.c_str(), "RECREATE");
  TTree* outTrueTree = new TTree("true_tree","true_tree");
  TTree* outRecoTree = new TTree("reco_tree","reco_tree");
  
  outTrueTree->Branch("event",&this_true_event,"event/I");
  outTrueTree->Branch("vtx_x",&vtx_x,"vtx_x/F");
  outTrueTree->Branch("vtx_y",&vtx_y,"vtx_y/F");
  outTrueTree->Branch("vtx_z",&vtx_z,"vtx_z/F");
  outTrueTree->Branch("true_n",&true_n,"true_n/I");
  outTrueTree->Branch("true_e",&true_e,"true_e[true_n]/F");
  outTrueTree->Branch("true_p",&true_p,"true_p[true_n]/F");
  outTrueTree->Branch("true_pt",&true_pt,"true_pt[true_n]/F");
  outTrueTree->Branch("true_eta",&true_eta,"true_eta[true_n]/F");
  outTrueTree->Branch("true_phi",&true_phi,"true_phi[true_n]/F");
  outTrueTree->Branch("true_m",&true_m,"true_m[true_n]/F");
  outTrueTree->Branch("true_cons_n",&true_cons_n,"true_cons_n[true_n]/I");
  outTrueTree->Branch("true_lcons_z",&true_lcons_z,"true_lcons_z[true_n]/F");
  outTrueTree->Branch("true_lcons_pid",&true_lcons_pid,"true_lcons_pid[true_n]/I");
  outTrueTree->Branch("true_emfrac",&true_emfrac,"true_emfrac[true_n]/F");
  outTrueTree->Branch("true_pg_n",&true_pg_n,"true_pg_n[true_n]/I");
  outTrueTree->Branch("true_lpg_id",&true_lpg_id,"true_lpg_id[true_n]/I");
  outTrueTree->Branch("true_lpg_fract",&true_lpg_fract,"true_lpg_fract[true_n]/F");
  outTrueTree->Branch("true_lpg_status",&true_lpg_status,"true_lpg_status[true_n]/I");

  outRecoTree->Branch("event",&this_reco_event,"event/I");
  outRecoTree->Branch("reco_n",&reco_n,"reco_n/I");
  outRecoTree->Branch("reco_e",&reco_e,"reco_e[reco_n]/F");
  outRecoTree->Branch("reco_p",&reco_p,"reco_p[reco_n]/F");
  outRecoTree->Branch("reco_pt",&reco_pt,"reco_pt[reco_n]/F");
  outRecoTree->Branch("reco_eta",&reco_eta,"reco_eta[reco_n]/F");
  outRecoTree->Branch("reco_phi",&reco_phi,"reco_phi[reco_n]/F");
  outRecoTree->Branch("reco_m",&reco_m,"reco_m[reco_n]/F");
  outRecoTree->Branch("reco_cemc_n",&reco_cemc_n,"reco_cemc_n[reco_n]/I");
  outRecoTree->Branch("reco_ihcal_n",&reco_ihcal_n,"reco_ihcal_n[reco_n]/I");
  outRecoTree->Branch("reco_ohcal_n",&reco_ohcal_n,"reco_ohcal_n[reco_n]/I");
  outRecoTree->Branch("reco_cemcEsum",&reco_cemcEsum,"reco_cemcEsum[reco_n]/F");
  outRecoTree->Branch("reco_ihcalEsum",&reco_ihcalEsum,"reco_ihcalEsum[reco_n]/F");
  outRecoTree->Branch("reco_ohcalEsum",&reco_ohcalEsum,"reco_ohcalEsum[reco_n]/F");
  outRecoTree->Branch("reco_clcemc_n",&reco_clcemc_n,"reco_clcemc_n[reco_n]/I");
  outRecoTree->Branch("reco_clcemc_had_n",&reco_clcemc_had_n,"reco_clcemc_had_n[reco_n]/I");
  outRecoTree->Branch("reco_clcemc_em_n",&reco_clcemc_em_n,"reco_clcemc_em_n[reco_n]/I");
  outRecoTree->Branch("reco_clcemc_hadEsum",&reco_clcemc_hadEsum,"reco_clcemc_hadEsum[reco_n]/F");
  outRecoTree->Branch("reco_clcemc_emEsum",&reco_clcemc_emEsum,"reco_clcemc_emEsum[reco_n]/F");
  outRecoTree->Branch("reco_clcemc_totEsum",&reco_clcemc_totEsum,"reco_clcemc_totEsum[reco_n]/F");
  outRecoTree->Branch("reco_clihcal_n",&reco_clihcal_n,"reco_clihcal_n[reco_n]/I");
  outRecoTree->Branch("reco_clihcal_totEsum",&reco_clihcal_totEsum,"reco_clihcal_totEsum[reco_n]/F");
  outRecoTree->Branch("reco_clohcal_n",&reco_clohcal_n,"reco_clohcal_n[reco_n]/I");
  outRecoTree->Branch("reco_clohcal_totEsum",&reco_clohcal_totEsum,"reco_clohcal_totEsum[reco_n]/F");
  
  outRecoTree->Branch("orig_reco_e",&orig_reco_e,"orig_reco_e[reco_n]/F");
  outRecoTree->Branch("orig_reco_p",&orig_reco_p,"orig_reco_p[reco_n]/F");
  outRecoTree->Branch("orig_reco_pt",&orig_reco_pt,"orig_reco_pt[reco_n]/F");
  outRecoTree->Branch("orig_reco_eta",&orig_reco_eta,"orig_reco_eta[reco_n]/F");
  outRecoTree->Branch("orig_reco_phi",&orig_reco_phi,"orig_reco_phi[reco_n]/F");
  outRecoTree->Branch("orig_reco_m",&orig_reco_m,"orig_reco_m[reco_n]/F");

  TH1D* h_track_p_had = new TH1D("h_track_p_had","",500,0,50);
  TH1D* h_etop_had = new TH1D("h_had_etop","",500,-0,5); 
  TH1D* h_etop = new TH1D("h_etop","",500,-0,5); 
  const int nhist = 17;
  float xmin[nhist] = {0,1,2,3,4,5,6,7,8,9,10,12,14,16,18,20,22};
  float xmax[nhist] = {1,2,3,4,5,6,7,8,9,10,12,14,16,18,20,22,24};
  TH1D* h_etop_diff[nhist];
  for( int ih=0; ih<nhist;ih++){
    h_etop_diff[ih]= new TH1D(Form("h_etop_diff_%dto%d",(int)xmin[ih],(int)xmax[ih]),"",500,-0,5);
  }
  TH1D* h_deta = new TH1D("h_deta","",500,-0.5,0.5); 
  TH1D* h_dphi = new TH1D("h_dphi","",500,-0.5,0.5); 
  TH1D* h_dr = new TH1D("h_dr","",500,0,1); 
  TH1D* h_prob = new TH1D("h_prob","",500,0,1.01); 
  TH1D* h_prob_had = new TH1D("h_prob_had","",500,0,1.01); 
  TH1D* h_chi2 = new TH1D("h_chi2","",500,0,20); 
  TH1D* h_chi2_had = new TH1D("h_chi2_had","",500,0,20); 

  //// For QCDjet constituents 
  TH1D* h_pt_tot = new TH1D("h_pt_tot","",100,0,50); 
  TH1D* h_pt_pipm = new TH1D("h_pt_pipm","",100,0,50); 
  TH1D* h_pt_pi0 = new TH1D("h_pt_pi0","",100,0,50); 
  TH1D* h_pt_eta0 = new TH1D("h_pt_eta0","",100,0,50); 
  TH1D* h_pt_epm = new TH1D("h_pt_epm","",100,0,50); 
  TH1D* h_pt_gamma = new TH1D("h_pt_gamma","",100,0,50); 
  TH1D* h_pt_proton = new TH1D("h_pt_proton","",100,0,50); 
  TH1D* h_pt_neutron = new TH1D("h_pt_neutron","",100,0,50); 
  TH1D* h_pt_kpm = new TH1D("h_pt_kpm","",100,0,50); 
  TH1D* h_pt_k0 = new TH1D("h_pt_k0","",100,0,50); 
  TH1D* h_pt_other = new TH1D("h_pt_other","",100,0,50); 
  
  ///////////////////////////////////////////////////////////////////////////
  int true_count=0;
  int reco_count=0;
  
  //// loop over files 
  for(int j=initfile; j<endfile; j++){
      
    string fname;
    if (sampleType.compare("GammaJet")==0) {
      if (note.compare("vtx0")==0) {
        if (jetE.compare("20GeV")==0) { fname = Form("./out_%s/QCD15GeV_gamma+jet_R04jet20GeVeta0p6_G4sPHENIX_File_%d.root",label.c_str(),j); }
        else if (jetE.compare("30GeV")==0) { fname = Form("./out_%s/QCD25GeV_gamma+jet_R04jet30GeVeta0p6_G4sPHENIX_File_%d.root",label.c_str(),j); }
        else if (jetE.compare("40GeV")==0) { fname = Form("./out_%s/QCD30GeV_gamma+jet_R04jet40GeVeta0p6_G4sPHENIX_File_%d.root",label.c_str(),j); }
        else if (jetE.compare("50GeV")==0) { fname = Form("./out_%s/QCD35GeV_gamma+jet_R04jet50GeVeta0p6_G4sPHENIX_File_%d.root",label.c_str(),j); }
        else { cout << "WARNING! wrong file selection" << endl; return;  }
      } else if (note.compare("vtx0gaus")==0) { fname = Form("./out_%s/QCD15GeV_gammajet5GeVeta0p6_G4sPHENIX_File_%d.root",label.c_str(),j); }
      else { cout << "KYO WARNING: note string unknown"<<endl; return; }
    } else if (sampleType.compare("QCDJet")==0) {
      if (note.compare("R02")==0 || note.compare("R04")==0) {
        if (jetE.compare("20GeV")==0) { fname = Form("./out_%s/QCD10GeV_R04jet20GeVeta0p45_G4sPHENIX_%d.root",label.c_str(),j); }
        else if (jetE.compare("30GeV")==0) { fname = Form("./out_%s/QCD15GeV_R04jet30GeVeta0p45_G4sPHENIX_%d.root",label.c_str(),j); }
        else if (jetE.compare("40GeV")==0) { fname = Form("./out_%s/QCD25GeV_R04jet40GeVeta0p45_G4sPHENIX_%d.root",label.c_str(),j); }
        else if (jetE.compare("50GeV")==0) { fname = Form("./out_%s/QCD35GeV_R04jet50GeVeta0p45_G4sPHENIX_%d.root",label.c_str(),j); }
        else { cout << "WARNING! wrong file selection" << endl; return;  }
      }
      else if (note.compare("vtx0")==0) { fname = Form("./out_%s/QCD15GeV_jet5GeVeta0p6_G4sPHENIX_File_%d.root",label.c_str(),j);}
      else if (note.compare("highz")==0 || note.compare("noptcut_highz")==0 || note.compare("noptcut_allz")==0) { fname = Form("./out_%s/QCD30GeV_jet40GeVeta0p45_G4sPHENIX_File_%d.root",label.c_str(),j);}
      else if (note.compare("vtx0gaus")==0) { fname = Form("./out_%s/QCD15GeV_jet15GeVeta0p6_G4sPHENIX_File_%d.root",label.c_str(),j); }
      else { cout << "KYO WARNING: note string unknown"<<endl; return; }
    }
    else { cout << "KYO WARNING: sampleType string unknown"<<endl; return; }

    if (j%50==0) cout << fname << endl; 
    //// skip bad files
    TFile* fin = new TFile(fname.c_str()); 
    //if (j%50==0) cout <<fin<<endl;
    if(fin->GetSize()<10000){fin->Close(); bad_files++; continue;}
    if(fin->IsZombie()) {fin->Close(); bad_files++; continue;}
    
    TTree* truthjet4_tree = (TTree*)fin->Get("truthjet4_tree");
    TTree* towerjet4_tree = (TTree*)fin->Get("towerjet4_tree");

    if (bad_files >0) cout << "Number of bad files = " << bad_files << endl; 
    
    ////////////////////////////////////////////////////////
    int truthevent=-999; 
    float truth_vtx_x=-999, truth_vtx_y=-999, truth_vtx_z=-999;
    float truthjet4_e=-999, truthjet4_p=-999;
    float truthjet4_pt=-999, truthjet4_eta=-999, truthjet4_phi=-999;
    truthjet4_tree->SetBranchAddress("event",&truthevent); 
    truthjet4_tree->SetBranchAddress("vtx_x",&truth_vtx_x); 
    truthjet4_tree->SetBranchAddress("vtx_y",&truth_vtx_y); 
    truthjet4_tree->SetBranchAddress("vtx_z",&truth_vtx_z); 
    truthjet4_tree->SetBranchAddress("truthjet4_e",&truthjet4_e); 
    truthjet4_tree->SetBranchAddress("truthjet4_p",&truthjet4_p); 
    truthjet4_tree->SetBranchAddress("truthjet4_pt",&truthjet4_pt); 
    truthjet4_tree->SetBranchAddress("truthjet4_eta",&truthjet4_eta); 
    truthjet4_tree->SetBranchAddress("truthjet4_phi",&truthjet4_phi); 
    int truthjet4_cons_n =-999;
    std::vector<float> *truthjet4_cons_e=0;
    std::vector<float> *truthjet4_cons_p=0;
    std::vector<float> *truthjet4_cons_pt=0;
    std::vector<float> *truthjet4_cons_eta=0;
    std::vector<float> *truthjet4_cons_phi=0;
    std::vector<float> *truthjet4_cons_dR=0;
    std::vector<int> *truthjet4_cons_pid=0;
    int truthjet4_pg_n=-999;
    std::vector<int> *truthjet4_pg_id=0;
    std::vector<float> *truthjet4_pg_fract=0;
    std::vector<int> *truthjet4_pg_status=0;
    truthjet4_tree->SetBranchAddress("truthjet4_cons_n",&truthjet4_cons_n); 
    truthjet4_tree->SetBranchAddress("truthjet4_cons_e",&truthjet4_cons_e); 
    truthjet4_tree->SetBranchAddress("truthjet4_cons_p",&truthjet4_cons_p); 
    truthjet4_tree->SetBranchAddress("truthjet4_cons_pt",&truthjet4_cons_pt); 
    truthjet4_tree->SetBranchAddress("truthjet4_cons_eta",&truthjet4_cons_eta); 
    truthjet4_tree->SetBranchAddress("truthjet4_cons_phi",&truthjet4_cons_phi); 
    truthjet4_tree->SetBranchAddress("truthjet4_cons_dR",&truthjet4_cons_dR); 
    truthjet4_tree->SetBranchAddress("truthjet4_cons_pid",&truthjet4_cons_pid); 
    truthjet4_tree->SetBranchAddress("truthjet4_pg_n",&truthjet4_pg_n); 
    truthjet4_tree->SetBranchAddress("truthjet4_pg_id",&truthjet4_pg_id); 
    truthjet4_tree->SetBranchAddress("truthjet4_pg_fract",&truthjet4_pg_fract); 
    truthjet4_tree->SetBranchAddress("truthjet4_pg_status",&truthjet4_pg_status); 
    
    ////////////////////////////////////////////////////////
    int towerevent=-999; 
    float towerjet4_e=-999, towerjet4_p=-999;
    float towerjet4_pt=-999, towerjet4_eta=-999, towerjet4_phi=-999;
    towerjet4_tree->SetBranchAddress("event",&towerevent); 
    towerjet4_tree->SetBranchAddress("towerjet4_e",&towerjet4_e); 
    towerjet4_tree->SetBranchAddress("towerjet4_p",&towerjet4_p); 
    towerjet4_tree->SetBranchAddress("towerjet4_pt",&towerjet4_pt); 
    towerjet4_tree->SetBranchAddress("towerjet4_eta",&towerjet4_eta); 
    towerjet4_tree->SetBranchAddress("towerjet4_phi",&towerjet4_phi); 
    ////////// 
    int towerjet4_cemc_n=-999;
    std::vector<float> *towerjet4_cemc_e=0;
    std::vector<float> *towerjet4_cemc_p=0;
    std::vector<float> *towerjet4_cemc_pt=0;
    std::vector<float> *towerjet4_cemc_eta=0;
    std::vector<float> *towerjet4_cemc_phi=0;
    std::vector<float> *towerjet4_cemc_dR=0;
    std::vector<int> *towerjet4_cemc_id=0;
    towerjet4_tree->SetBranchAddress("towerjet4_cemc_n",&towerjet4_cemc_n); 
    towerjet4_tree->SetBranchAddress("towerjet4_cemc_e",&towerjet4_cemc_e); 
    towerjet4_tree->SetBranchAddress("towerjet4_cemc_p",&towerjet4_cemc_p); 
    towerjet4_tree->SetBranchAddress("towerjet4_cemc_pt",&towerjet4_cemc_pt); 
    towerjet4_tree->SetBranchAddress("towerjet4_cemc_eta",&towerjet4_cemc_eta); 
    towerjet4_tree->SetBranchAddress("towerjet4_cemc_phi",&towerjet4_cemc_phi); 
    towerjet4_tree->SetBranchAddress("towerjet4_cemc_dR",&towerjet4_cemc_dR); 
    towerjet4_tree->SetBranchAddress("towerjet4_cemc_id",&towerjet4_cemc_id); 
    ////////// 
    int towerjet4_clcemc_n=-999;
    std::vector<float> *towerjet4_clcemc_e=0;
    std::vector<float> *towerjet4_clcemc_p=0;
    std::vector<float> *towerjet4_clcemc_pt=0;
    std::vector<float> *towerjet4_clcemc_eta=0;
    std::vector<float> *towerjet4_clcemc_phi=0;
    std::vector<float> *towerjet4_clcemc_dR=0;
    std::vector<int> *towerjet4_clcemc_id=0;
    std::vector<int> *towerjet4_clcemc_ntwr=0;
    std::vector<float> *towerjet4_clcemc_prob=0;
    std::vector<float> *towerjet4_clcemc_chi2=0;
    std::vector<float> *towerjet4_clcemc_twrEsum=0;
    std::vector<float> *towerjet4_clcemc_twrNfrac=0;
    towerjet4_tree->SetBranchAddress("towerjet4_clcemc_n",&towerjet4_clcemc_n); 
    towerjet4_tree->SetBranchAddress("towerjet4_clcemc_e",&towerjet4_clcemc_e); 
    towerjet4_tree->SetBranchAddress("towerjet4_clcemc_p",&towerjet4_clcemc_p); 
    towerjet4_tree->SetBranchAddress("towerjet4_clcemc_pt",&towerjet4_clcemc_pt); 
    towerjet4_tree->SetBranchAddress("towerjet4_clcemc_eta",&towerjet4_clcemc_eta); 
    towerjet4_tree->SetBranchAddress("towerjet4_clcemc_phi",&towerjet4_clcemc_phi); 
    towerjet4_tree->SetBranchAddress("towerjet4_clcemc_dR",&towerjet4_clcemc_dR); 
    towerjet4_tree->SetBranchAddress("towerjet4_clcemc_id",&towerjet4_clcemc_id); 
    towerjet4_tree->SetBranchAddress("towerjet4_clcemc_ntwr",&towerjet4_clcemc_ntwr); 
    towerjet4_tree->SetBranchAddress("towerjet4_clcemc_prob",&towerjet4_clcemc_prob); 
    towerjet4_tree->SetBranchAddress("towerjet4_clcemc_chi2",&towerjet4_clcemc_chi2); 
    towerjet4_tree->SetBranchAddress("towerjet4_clcemc_twrEsum",&towerjet4_clcemc_twrEsum); 
    towerjet4_tree->SetBranchAddress("towerjet4_clcemc_twrNfrac",&towerjet4_clcemc_twrNfrac); 
    ////////// 
    int towerjet4_clcemcMatched_n=-999;
    std::vector<float> *towerjet4_clcemcMatched_e=0;
    std::vector<float> *towerjet4_clcemcMatched_p=0;
    std::vector<float> *towerjet4_clcemcMatched_pt=0;
    std::vector<float> *towerjet4_clcemcMatched_eta=0;
    std::vector<float> *towerjet4_clcemcMatched_phi=0;
    std::vector<int> *towerjet4_clcemcMatched_id=0;
    std::vector<float> *towerjet4_clcemcMatched_prob=0;
    std::vector<float> *towerjet4_clcemcMatched_chi2=0;
    std::vector<float> *towerjet4_clcemcMatched_track_p=0;
    std::vector<float> *towerjet4_clcemcMatched_track_deta=0;
    std::vector<float> *towerjet4_clcemcMatched_track_dphi=0;
    std::vector<float> *towerjet4_clcemcMatched_track_dr=0;
    towerjet4_tree->SetBranchAddress("towerjet4_clcemcMatched_n",&towerjet4_clcemcMatched_n); 
    towerjet4_tree->SetBranchAddress("towerjet4_clcemcMatched_e",&towerjet4_clcemcMatched_e); 
    towerjet4_tree->SetBranchAddress("towerjet4_clcemcMatched_p",&towerjet4_clcemcMatched_p); 
    towerjet4_tree->SetBranchAddress("towerjet4_clcemcMatched_pt",&towerjet4_clcemcMatched_pt); 
    towerjet4_tree->SetBranchAddress("towerjet4_clcemcMatched_eta",&towerjet4_clcemcMatched_eta); 
    towerjet4_tree->SetBranchAddress("towerjet4_clcemcMatched_phi",&towerjet4_clcemcMatched_phi); 
    towerjet4_tree->SetBranchAddress("towerjet4_clcemcMatched_id",&towerjet4_clcemcMatched_id); 
    towerjet4_tree->SetBranchAddress("towerjet4_clcemcMatched_prob",&towerjet4_clcemcMatched_prob); 
    towerjet4_tree->SetBranchAddress("towerjet4_clcemcMatched_chi2",&towerjet4_clcemcMatched_chi2); 
    towerjet4_tree->SetBranchAddress("towerjet4_clcemcMatched_track_p",&towerjet4_clcemcMatched_track_p); 
    towerjet4_tree->SetBranchAddress("towerjet4_clcemcMatched_track_deta",&towerjet4_clcemcMatched_track_deta); 
    towerjet4_tree->SetBranchAddress("towerjet4_clcemcMatched_track_dphi",&towerjet4_clcemcMatched_track_dphi); 
    towerjet4_tree->SetBranchAddress("towerjet4_clcemcMatched_track_dr",&towerjet4_clcemcMatched_track_dr); 
    ////////// 
    ////////// 
    int towerjet4_ihcal_n=-999;
    std::vector<float> *towerjet4_ihcal_e=0;
    std::vector<float> *towerjet4_ihcal_p=0;
    std::vector<float> *towerjet4_ihcal_pt=0;
    std::vector<float> *towerjet4_ihcal_eta=0;
    std::vector<float> *towerjet4_ihcal_phi=0;
    std::vector<float> *towerjet4_ihcal_dR=0;
    towerjet4_tree->SetBranchAddress("towerjet4_ihcal_n",&towerjet4_ihcal_n); 
    towerjet4_tree->SetBranchAddress("towerjet4_ihcal_e",&towerjet4_ihcal_e); 
    towerjet4_tree->SetBranchAddress("towerjet4_ihcal_p",&towerjet4_ihcal_p); 
    towerjet4_tree->SetBranchAddress("towerjet4_ihcal_pt",&towerjet4_ihcal_pt); 
    towerjet4_tree->SetBranchAddress("towerjet4_ihcal_eta",&towerjet4_ihcal_eta); 
    towerjet4_tree->SetBranchAddress("towerjet4_ihcal_phi",&towerjet4_ihcal_phi); 
    towerjet4_tree->SetBranchAddress("towerjet4_ihcal_dR",&towerjet4_ihcal_dR); 
    ////////// 
    int towerjet4_clihcal_n=-999;
    std::vector<float> *towerjet4_clihcal_e=0;
    std::vector<float> *towerjet4_clihcal_p=0;
    std::vector<float> *towerjet4_clihcal_pt=0;
    std::vector<float> *towerjet4_clihcal_eta=0;
    std::vector<float> *towerjet4_clihcal_phi=0;
    std::vector<float> *towerjet4_clihcal_dR=0;
    std::vector<int> *towerjet4_clihcal_ntwr=0;
    std::vector<int> *towerjet4_clihcal_id=0;
    std::vector<float> *towerjet4_clihcal_twrEsum=0;
    std::vector<float> *towerjet4_clihcal_twrNfrac=0;
    towerjet4_tree->SetBranchAddress("towerjet4_clihcal_n",&towerjet4_clihcal_n); 
    towerjet4_tree->SetBranchAddress("towerjet4_clihcal_e",&towerjet4_clihcal_e); 
    towerjet4_tree->SetBranchAddress("towerjet4_clihcal_p",&towerjet4_clihcal_p); 
    towerjet4_tree->SetBranchAddress("towerjet4_clihcal_pt",&towerjet4_clihcal_pt); 
    towerjet4_tree->SetBranchAddress("towerjet4_clihcal_eta",&towerjet4_clihcal_eta); 
    towerjet4_tree->SetBranchAddress("towerjet4_clihcal_phi",&towerjet4_clihcal_phi); 
    towerjet4_tree->SetBranchAddress("towerjet4_clihcal_dR",&towerjet4_clihcal_dR); 
    towerjet4_tree->SetBranchAddress("towerjet4_clihcal_ntwr",&towerjet4_clihcal_ntwr); 
    towerjet4_tree->SetBranchAddress("towerjet4_clihcal_id",&towerjet4_clihcal_id); 
    towerjet4_tree->SetBranchAddress("towerjet4_clihcal_twrEsum",&towerjet4_clihcal_twrEsum); 
    towerjet4_tree->SetBranchAddress("towerjet4_clihcal_twrNfrac",&towerjet4_clihcal_twrNfrac); 
    ////////// 
    ////////// 
    int towerjet4_ohcal_n=-999;
    std::vector<float> *towerjet4_ohcal_e=0;
    std::vector<float> *towerjet4_ohcal_p=0;
    std::vector<float> *towerjet4_ohcal_pt=0;
    std::vector<float> *towerjet4_ohcal_eta=0;
    std::vector<float> *towerjet4_ohcal_phi=0;
    std::vector<float> *towerjet4_ohcal_dR=0;
    towerjet4_tree->SetBranchAddress("towerjet4_ohcal_n",&towerjet4_ohcal_n); 
    towerjet4_tree->SetBranchAddress("towerjet4_ohcal_e",&towerjet4_ohcal_e); 
    towerjet4_tree->SetBranchAddress("towerjet4_ohcal_p",&towerjet4_ohcal_p); 
    towerjet4_tree->SetBranchAddress("towerjet4_ohcal_pt",&towerjet4_ohcal_pt); 
    towerjet4_tree->SetBranchAddress("towerjet4_ohcal_eta",&towerjet4_ohcal_eta); 
    towerjet4_tree->SetBranchAddress("towerjet4_ohcal_phi",&towerjet4_ohcal_phi); 
    towerjet4_tree->SetBranchAddress("towerjet4_ohcal_dR",&towerjet4_ohcal_dR); 
    ////////// 
    int towerjet4_clohcal_n=-999;
    std::vector<float> *towerjet4_clohcal_e=0;
    std::vector<float> *towerjet4_clohcal_p=0;
    std::vector<float> *towerjet4_clohcal_pt=0;
    std::vector<float> *towerjet4_clohcal_eta=0;
    std::vector<float> *towerjet4_clohcal_phi=0;
    std::vector<float> *towerjet4_clohcal_dR=0;
    std::vector<int> *towerjet4_clohcal_ntwr=0;
    std::vector<int> *towerjet4_clohcal_id=0;
    std::vector<float> *towerjet4_clohcal_twrEsum=0;
    std::vector<float> *towerjet4_clohcal_twrNfrac=0;
    towerjet4_tree->SetBranchAddress("towerjet4_clohcal_n",&towerjet4_clohcal_n); 
    towerjet4_tree->SetBranchAddress("towerjet4_clohcal_e",&towerjet4_clohcal_e); 
    towerjet4_tree->SetBranchAddress("towerjet4_clohcal_p",&towerjet4_clohcal_p); 
    towerjet4_tree->SetBranchAddress("towerjet4_clohcal_pt",&towerjet4_clohcal_pt); 
    towerjet4_tree->SetBranchAddress("towerjet4_clohcal_eta",&towerjet4_clohcal_eta); 
    towerjet4_tree->SetBranchAddress("towerjet4_clohcal_phi",&towerjet4_clohcal_phi); 
    towerjet4_tree->SetBranchAddress("towerjet4_clohcal_dR",&towerjet4_clohcal_dR); 
    towerjet4_tree->SetBranchAddress("towerjet4_clohcal_ntwr",&towerjet4_clohcal_ntwr); 
    towerjet4_tree->SetBranchAddress("towerjet4_clohcal_id",&towerjet4_clohcal_id); 
    towerjet4_tree->SetBranchAddress("towerjet4_clohcal_twrEsum",&towerjet4_clohcal_twrEsum); 
    towerjet4_tree->SetBranchAddress("towerjet4_clohcal_twrNfrac",&towerjet4_clohcal_twrNfrac); 
    ////////////////////////////////////////////////////////
    
    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    int prev_truthevent=j*50+1;
    true_n=0;
    //// loop over truth jet entries
    for (int itrue=0; itrue<truthjet4_tree->GetEntries(); itrue++){

      truthjet4_tree->GetEvent(itrue);
      
      if (prev_truthevent !=j*50+truthevent) { //match the same event number
        //// 1) save theprevious event to tree
        this_true_event=prev_truthevent;
        //cout << "  Fill previous evts! this_true_event = " << this_true_event << endl;
        outTrueTree->Fill();
        //// 2) check the missing events
        while (prev_truthevent+1 !=j*50+truthevent) {
          //// save empty true events
          prev_truthevent = prev_truthevent+1; 
          FillEmptyTrueEvents();
          this_true_event=prev_truthevent;
          //cout << "  Fill! empty evts! this_true_event = " << this_true_event << endl;
          outTrueTree->Fill();
        } 
        //// 3) re-initialize for a new event
        true_n=0;
        prev_truthevent = j*50+truthevent; 
      }
      //cout << "itrue : " << itrue << endl; // # of jets 
      //cout << "prev_truthevent : " << prev_truthevent << endl;  
      //cout << "truthevent : " << truthevent << endl; // read-in event (to be read below)
      //cout << "   Fill prev! this_true_event : " << this_true_event << endl; // write-out event (filled above, truthevent-1)
      //cout << "true_count : " << true_count << endl; // +50 per file 
     
      vtx_x = truth_vtx_x;
      vtx_y = truth_vtx_y;
      vtx_z = truth_vtx_z;
      
      //////////////////////////////////////////////////////////////////////// 
      //// loop over truth constituents
      //////////////////////////////////////////////////////////////////////// 
      //// z = ( p_cons * cos(d_theta)) / p_jet
      float jet_theta = 2*atan(exp(-1*truthjet4_eta));
      float this_theta = -999;
      float this_z = -999;
      float highest_z = -999;
      int highest_pid = -999;
      float em_den = 0;
      float em_num = 0;
      for (int icons =0; icons < truthjet4_cons_n; icons ++) {
        this_theta = 2*atan(exp(-1*truthjet4_cons_eta->at(icons)));
        this_z = truthjet4_cons_p->at(icons)*cos(fabs(jet_theta-this_theta))/truthjet4_p;
        if (this_z > highest_z) {
          highest_z = this_z;
          highest_pid = truthjet4_cons_pid->at(icons);
        }
        //// Denominator: All energy
        em_den += truthjet4_cons_e->at(icons); 
        //// Numerator: EM energy only, using truth ptl info ( e+-, gamma, pi0, eta) 
        if( (fabs(truthjet4_cons_pid->at(icons))==11) || (truthjet4_cons_pid->at(icons)==22) || 
        (truthjet4_cons_pid->at(icons)==111) || (truthjet4_cons_pid->at(icons)==221) ){
           em_num += truthjet4_cons_e->at(icons); 
        } 
        //// Fill Hist
        h_pt_tot->Fill(truthjet4_cons_pt->at(icons));
        if ( fabs(truthjet4_cons_pid->at(icons)) == 211 ) { h_pt_pipm->Fill(truthjet4_cons_pt->at(icons)); } 
        else if ( truthjet4_cons_pid->at(icons) == 111 ) { h_pt_pi0->Fill(truthjet4_cons_pt->at(icons)); } 
        else if ( truthjet4_cons_pid->at(icons) == 221 ) { h_pt_eta0->Fill(truthjet4_cons_pt->at(icons)); } 
        else if ( fabs(truthjet4_cons_pid->at(icons)) == 11 ) { h_pt_epm->Fill(truthjet4_cons_pt->at(icons)); } 
        else if ( truthjet4_cons_pid->at(icons) == 22 ) { h_pt_gamma->Fill(truthjet4_cons_pt->at(icons)); } 
        else if ( fabs(truthjet4_cons_pid->at(icons)) == 2212 ) { h_pt_proton->Fill(truthjet4_cons_pt->at(icons)); } 
        else if ( fabs(truthjet4_cons_pid->at(icons)) == 2112 ) { h_pt_neutron->Fill(truthjet4_cons_pt->at(icons)); } 
        else if ( fabs(truthjet4_cons_pid->at(icons)) == 321 ) { h_pt_kpm->Fill(truthjet4_cons_pt->at(icons)); } 
        else if ( fabs(truthjet4_cons_pid->at(icons)) == 130 ) { h_pt_k0->Fill(truthjet4_cons_pt->at(icons)); } 
        else { h_pt_other->Fill(truthjet4_cons_pt->at(icons)); cout << "other : " <<truthjet4_cons_pid->at(icons) << endl; } 
      } 
     
      //////////////////////////////////////////////////////////////////////// 
      //// loop over truth progenitor
      //////////////////////////////////////////////////////////////////////// 
      float highest_fract = -999;
      int highest_id = -999;
      int highest_status = -999;
      for (int ipg =0; ipg < truthjet4_pg_n; ipg ++) {
        if (truthjet4_pg_fract->at(ipg) > highest_fract) {
          highest_fract = truthjet4_pg_fract->at(ipg);
          highest_id = truthjet4_pg_id->at(ipg);
          highest_status = truthjet4_pg_status->at(ipg);
        }
      } 
    
      //////////////////////////////////////////////////////////////////////// 
      ////// Assign values
      //////////////////////////////////////////////////////////////////////// 
      
      true_e[true_n]=truthjet4_e;
      true_p[true_n]=truthjet4_p;
      true_pt[true_n]=truthjet4_pt;
      true_eta[true_n]=truthjet4_eta;
      true_phi[true_n]=truthjet4_phi;
      true_m[true_n]= sqrt(pow(truthjet4_e,2)-pow(truthjet4_p,2));
      
      true_cons_n[true_n]=truthjet4_cons_n;
      true_pg_n[true_n]=truthjet4_pg_n;
      
      true_lcons_z[true_n] = highest_z;
      true_lcons_pid[true_n] = highest_pid;
      true_emfrac[true_n] = em_num/em_den;

      true_lpg_id[true_n] = highest_id;
      true_lpg_fract[true_n] = highest_fract;
      true_lpg_status[true_n] = highest_status;
      
      true_n++;
    } // EOL over truth jet
    //// 3) save the LAST event to tree;
    this_true_event=true_count+prev_truthevent;
    outTrueTree->Fill();
    
    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    std::vector<TLorentzVector> calib_cemc;
    std::vector<TLorentzVector> calib_ihcal;
    std::vector<TLorentzVector> calib_ohcal;
    std::vector<TLorentzVector> calib_clcemc;
    std::vector<TLorentzVector> calib_clcemc_em;
    std::vector<TLorentzVector> calib_clcemc_had;
    std::vector<TLorentzVector> calib_clihcal;
    std::vector<TLorentzVector> calib_clohcal;
    
    int prev_towerevent=j*50+1;
    reco_n=0;
    //// loop over reco jet entries
    for (int ireco=0; ireco<towerjet4_tree->GetEntries(); ireco++){
      
      towerjet4_tree->GetEvent(ireco);
      
      //cout << ""<<endl;
      //cout << "ireco = " << ireco << endl;
      //cout << "prev_towerevent = " << prev_towerevent << endl;
      //cout << "towerevent = " << towerevent << endl;
      if (prev_towerevent !=j*50+towerevent) { //match the same event number
        //// 1) save theprevious event to tree
        this_reco_event=prev_towerevent;
        //cout << "  Fill previous evts! this_reco_event = " << this_reco_event << endl;
        outRecoTree->Fill();
        //// 2) check the missing events
        while (prev_towerevent+1 !=j*50+towerevent) {
          //// save empty reco events
          prev_towerevent = prev_towerevent+1; 
          FillEmptyRecoEvents();
          this_reco_event=prev_towerevent;
          //cout << "  Fill! empty evts! this_reco_event = " << this_reco_event << endl;
          outRecoTree->Fill();
        } 
        //// 3) re-initialize for a new event
        reco_n=0;
        prev_towerevent = j*50+towerevent; 
      }
/*      
      reco_e[reco_n]=towerjet4_e;
      //cout << "   reco_e["<<reco_n<<"] = " << reco_e[reco_n] << endl;
      reco_p[reco_n]=towerjet4_p;
      reco_pt[reco_n]=towerjet4_pt;
      reco_eta[reco_n]=towerjet4_eta;
      reco_phi[reco_n]=towerjet4_phi;
*/
      //////////////////////////////////////////////////////////////////////// 
      //// loop over tower constituents to get tower energy sum
      //////////////////////////////////////////////////////////////////////// 
      TLorentzVector tmp_calib_towerjet;
      TLorentzVector tmp_calib_clusterjet;
      TLorentzVector tmp_calib_myjet;
      
      TLorentzVector tmp_calib_cemc;
      TLorentzVector tmp_calib_ihcal;
      TLorentzVector tmp_calib_ohcal;
      
      reco_cemc_n[reco_n]=towerjet4_cemc_n;
      reco_ihcal_n[reco_n]=towerjet4_ihcal_n;
      reco_ohcal_n[reco_n]=towerjet4_ohcal_n;

      //if (towerevent==1 && ireco==0) cout << "********** towerjet4_cemc_n = " << towerjet4_cemc_n << endl;
      //if (towerevent==1) cout << "********** towerjet4_cemc_n = " << towerjet4_cemc_n << endl;
      //if (towerevent==1) cout << "********** towerjet4_ihcal_n = " << towerjet4_ihcal_n << endl;
      //if (towerevent==1) cout << "********** towerjet4_ohcal_n = " << towerjet4_ohcal_n << endl;
      
      calib_cemc.clear();
      calib_ihcal.clear();
      calib_ohcal.clear();
      float cemc_sum = 0;
      float ihcal_sum = 0;
      float ohcal_sum = 0;
      for (int icemc =0; icemc < towerjet4_cemc_n; icemc ++) {
        //if (towerevent==1 && ireco==0) cout << icemc <<"th cemc_pt = " << towerjet4_cemc_pt->at(icemc) << endl;
        //if (towerevent==1) cout << icemc <<"th cemc_pt = " << towerjet4_cemc_pt->at(icemc) << endl;
        //cemc_sum += towerjet4_cemc_e->at(icemc); 
        //tmp_calib_cemc.SetPtEtaPhiE(towerjet4_cemc_pt->at(icemc),towerjet4_cemc_eta->at(icemc),towerjet4_cemc_phi->at(icemc),towerjet4_cemc_e->at(icemc));
        tmp_calib_cemc.SetPtEtaPhiM(towerjet4_cemc_pt->at(icemc),towerjet4_cemc_eta->at(icemc),towerjet4_cemc_phi->at(icemc),0);
        //tmp_calib_cemc.SetPtEtaPhiM(total_sf*cemc_sf*towerjet4_cemc_pt->at(icemc),towerjet4_cemc_eta->at(icemc),towerjet4_cemc_phi->at(icemc),0);
        calib_cemc.push_back(tmp_calib_cemc);
      } 
      for (int iihcal =0; iihcal < towerjet4_ihcal_n; iihcal ++) {
        //ihcal_sum += towerjet4_ihcal_e->at(iihcal); 
        //tmp_calib_ihcal.SetPtEtaPhiE(towerjet4_ihcal_pt->at(iihcal),towerjet4_ihcal_eta->at(iihcal),towerjet4_ihcal_phi->at(iihcal),towerjet4_ihcal_e->at(iihcal));
        //tmp_calib_ihcal.SetPtEtaPhiM(towerjet4_ihcal_pt->at(iihcal),towerjet4_ihcal_eta->at(iihcal),towerjet4_ihcal_phi->at(iihcal),0);
        tmp_calib_ihcal.SetPtEtaPhiM(total_sf*ihcal_sf*towerjet4_ihcal_pt->at(iihcal),towerjet4_ihcal_eta->at(iihcal),towerjet4_ihcal_phi->at(iihcal),0);
        calib_ihcal.push_back(tmp_calib_ihcal);
      } 
      for (int iohcal =0; iohcal < towerjet4_ohcal_n; iohcal ++) {
        //ohcal_sum += towerjet4_ohcal_e->at(iohcal); 
        //tmp_calib_ohcal.SetPtEtaPhiE(towerjet4_ohcal_pt->at(iohcal),towerjet4_ohcal_eta->at(iohcal),towerjet4_ohcal_phi->at(iohcal),towerjet4_ohcal_e->at(iohcal));
        //tmp_calib_ohcal.SetPtEtaPhiM(towerjet4_ohcal_pt->at(iohcal),towerjet4_ohcal_eta->at(iohcal),towerjet4_ohcal_phi->at(iohcal),0);
        tmp_calib_ohcal.SetPtEtaPhiM(total_sf*ohcal_sf*towerjet4_ohcal_pt->at(iohcal),towerjet4_ohcal_eta->at(iohcal),towerjet4_ohcal_phi->at(iohcal),0);
        calib_ohcal.push_back(tmp_calib_ohcal);
      } 
      
      //if (towerevent==1 ) cout << "calib_cemc.size() = " << calib_cemc.size() << endl; 
      for (int icemc =0; icemc < calib_cemc.size(); icemc ++) {
        //if (towerevent==1) {
        //  cout << icemc <<"th calib_cemc_pt = " << calib_cemc.at(icemc).Pt() << endl;
        //  cout << icemc <<"th calib_cemc_eta = " << calib_cemc.at(icemc).Eta() << endl;
        //  cout << icemc <<"th calib_cemc_phi = " << calib_cemc.at(icemc).Phi() << endl;
        //  cout << icemc <<"th calib_cemc_e = " << calib_cemc.at(icemc).E() << endl;
        //  cout << icemc <<"th calib_cemc_et = " << calib_cemc.at(icemc).Et() << endl;
        //  cout << icemc <<"th calib_cemc_M = " << calib_cemc.at(icemc).M() << endl;
        //}
        cemc_sum += calib_cemc.at(icemc).E(); 
        tmp_calib_towerjet += calib_cemc.at(icemc);
      }
      //if (towerevent==1 ) cout << "calib_ihcal.size() = " << calib_ihcal.size() << endl; 
      for (int iihcal =0; iihcal < calib_ihcal.size(); iihcal ++) {
        ihcal_sum += calib_ihcal.at(iihcal).E(); 
        tmp_calib_towerjet += calib_ihcal.at(iihcal);
        tmp_calib_myjet += calib_ihcal.at(iihcal);
      }
      //if (towerevent==1 ) cout << "calib_ohcal.size() = " << calib_ohcal.size() << endl; 
      for (int iohcal =0; iohcal < calib_ohcal.size(); iohcal ++) {
        ohcal_sum += calib_ohcal.at(iohcal).E(); 
        tmp_calib_towerjet += calib_ohcal.at(iohcal);
        tmp_calib_myjet += calib_ohcal.at(iohcal);
      }
      
      //////////////////////////////////////////////////////////////////////// 
      //// loop over cluster constituents to sort out EM vs. hadronic clusters 
      //////////////////////////////////////////////////////////////////////// 
      TLorentzVector tmp_calib_clcemc;
      TLorentzVector tmp_calib_clcemc_em;
      TLorentzVector tmp_calib_clcemc_had;
      TLorentzVector tmp_calib_clihcal;
      TLorentzVector tmp_calib_clohcal;
      
      int clcemc_had_n = 0;
      int clcemc_em_n = 0;
      int clcemc_n = 0;
      int clihcal_n = 0;
      int clohcal_n = 0;
      float clcemc_hadsum = 0;
      float clcemc_emsum = 0;
      float clcemc_totsum = 0;
      float clihcal_totsum = 0;
      float clohcal_totsum = 0;
      //cout << "**** towerjet4_clcemc_n = " << towerjet4_clcemc_n << endl;
      //cout << "**** towerjet4_clcemcMatched_n = " << towerjet4_clcemcMatched_n << endl;
      
      ////////////////////////////////////////////////////////////////// 
      ////// STEP 1 : Remove double counts (two tracks being matched the same cluster) and define had_cluster candidates (tmp_*)
      ////////////////////////////////////////////////////////////////// 

      float max_p = 0;
      std::vector<int> tmp_id;
      std::vector<float> tmp_prob;
      std::vector<float> tmp_chi2;
      std::vector<float> tmp_track_p;
      std::vector<float> tmp_track_deta;
      std::vector<float> tmp_track_dphi;
      std::vector<float> tmp_track_dr;
      std::vector<float> tmp_e;
      std::vector<float> tmp_pt;
      std::vector<float> tmp_eta;
      std::vector<float> tmp_phi;
      std::vector<float> tmp_etop;
      
      std::vector<int> clcemc_had_id;
      std::vector<int> clcemc_em_id;
      
      for (int icemcmat =0; icemcmat < towerjet4_clcemcMatched_n; icemcmat ++) {
        bool isInList = false;
        max_p = towerjet4_clcemcMatched_track_p->at(icemcmat);
        //// check if the cluster is already in the list (two tracks being matched the same cluster)
        for (int il =0; il< (int)tmp_id.size(); il++){
          if ( (int)towerjet4_clcemcMatched_id->at(icemcmat) == (int)tmp_id.at(il) ){
            isInList = true;
            //// modify p values to the highest one among associated tracks
            if (towerjet4_clcemcMatched_track_p->at(icemcmat) > tmp_track_p.at(il) ){
              tmp_track_p.at(il) = towerjet4_clcemcMatched_track_p->at(icemcmat);
            }
          }
        } // EOL over lcemcMatched-until-now (tmp_id)
        if (!isInList){
          tmp_id.push_back((int)towerjet4_clcemcMatched_id->at(icemcmat));
          tmp_prob.push_back((float)towerjet4_clcemcMatched_prob->at(icemcmat));
          tmp_chi2.push_back((float)towerjet4_clcemcMatched_chi2->at(icemcmat));
          tmp_track_deta.push_back((float)towerjet4_clcemcMatched_track_deta->at(icemcmat));
          tmp_track_dphi.push_back((float)towerjet4_clcemcMatched_track_dphi->at(icemcmat));
          tmp_track_dr.push_back((float)towerjet4_clcemcMatched_track_dr->at(icemcmat));
          tmp_track_p.push_back((float)towerjet4_clcemcMatched_track_p->at(icemcmat));
          tmp_e.push_back((float)towerjet4_clcemcMatched_e->at(icemcmat));
          tmp_pt.push_back((float)towerjet4_clcemcMatched_pt->at(icemcmat));
          tmp_eta.push_back((float)towerjet4_clcemcMatched_eta->at(icemcmat));
          tmp_phi.push_back((float)towerjet4_clcemcMatched_phi->at(icemcmat));
        }
      } //EOL over clcemc_matched
     
      //// Apply E/p cut to sort out and reject EM energy from hadronic energy, among track-Matched clusters 
      ////////////////////////////////////////////////////////////////// 
      ////// STEP 2 : Apply E/p cut and dEtadPhi cut to reject EM energy from hadronic energy among (tmp_*)
      ////////////////////////////////////////////////////////////////// 
      
      calib_clcemc_had.clear();
     
      if (towerevent==1) cout << "***************tmp_track_p.size() = " <<  tmp_track_p.size() << endl; 
      for (int icc =0; icc < tmp_track_p.size(); icc ++) {
        if (towerevent==1) cout << "     "<<icc<<"th tmp_id = " << tmp_id.at(icc) << endl;
        //// fill hist
        h_etop->Fill((float)tmp_e.at(icc)/tmp_track_p.at(icc)); //all track-Matched
        for( int ih=0; ih<nhist;ih++){
          if (xmin[ih] < tmp_track_p.at(icc) && tmp_track_p.at(icc) < xmax[ih]) { h_etop_diff[ih]->Fill((float)tmp_e.at(icc)/tmp_track_p.at(icc)); }
        }
        h_deta->Fill((float)tmp_track_deta.at(icc)); //all track-Matched
        h_dphi->Fill((float)tmp_track_dphi.at(icc)); //all track-Matched
        h_dr->Fill((float)tmp_track_dr.at(icc)); //all track-Matched
        h_prob->Fill((float)tmp_prob.at(icc)); //all track-Matched
        h_chi2->Fill((float)tmp_chi2.at(icc)); //all track-Matched

        //// apply cuts
        bool isetop = false;
        bool isdphi = false;
        
        if ( doetopcut)  {
          //// if p < 1GeV, all considered hadrnoic energy
          if (tmp_track_p.at(icc)<1) { isetop=true; }
          else {
            for (int ip=0; ip<np; ip++){
              if ( xlow[ip] < tmp_track_p.at(icc) && tmp_track_p.at(icc) < xhigh[ip] && ( ((float)tmp_e.at(icc)/tmp_track_p.at(icc)) < 1-3*sig[ip] || ((float)tmp_e.at(icc)/tmp_track_p.at(icc)) > 1+3*sig[ip] )){ isetop = true; break; } 
            }
          }
        } else { isetop = true; }
        
        if ( dodphicut) {
          // N.B. allGeV = vtx10gaus, 20-50 GeV = vtx0
          if (jetE.compare("allGeV")==0){ 
            if (fabs((float)tmp_track_deta.at(icc))< 0.0385339 && fabs((float)tmp_track_dphi.at(icc)) < 0.0533628) { isdphi = true; }
            else { isdphi = false; } 
          }
          if (jetE.compare("20GeV")==0){ 
            if (fabs((float)tmp_track_deta.at(icc))<0.0265182 && fabs((float)tmp_track_dphi.at(icc)) < 0.0778772) { isdphi = true; }
            else { isdphi = false; } 
          }
          else if (jetE.compare("30GeV")==0){ 
            if (fabs((float)tmp_track_deta.at(icc))<0.0264454 && fabs((float)tmp_track_dphi.at(icc)) < 0.0715491) { isdphi = true; }
            else { isdphi = false; } 
          }
          else if (jetE.compare("40GeV")==0){ 
            if (fabs((float)tmp_track_deta.at(icc))<0.0263104 && fabs((float)tmp_track_dphi.at(icc)) < 0.0664563) { isdphi = true; }
            else { isdphi = false; } 
          }
          else if (jetE.compare("50GeV")==0){ 
            if (fabs((float)tmp_track_deta.at(icc))<0.0268391 && fabs((float)tmp_track_dphi.at(icc)) < 0.0643709) { isdphi = true; }
            else { isdphi = false; } 
          }
        } else { isdphi = true; }
        //cout << " isetop = " << (int)isetop<<endl;
        //cout << " isdphi = " << (int)isdphi<<endl;
        if ( isetop && isdphi ) {
          if (towerevent==1) cout << "               matched tmp_id.at(icc) = " << tmp_id.at(icc) << endl;
          clcemc_had_id.push_back(tmp_id.at(icc));
          h_prob_had->Fill((float)tmp_prob.at(icc)); //hadron energy among track-Matched
          h_chi2_had->Fill((float)tmp_chi2.at(icc)); //hadron energy among track-Matched
          h_track_p_had->Fill((float)tmp_track_p.at(icc));
          h_etop_had->Fill((float)tmp_e.at(icc)/tmp_track_p.at(icc));
        } //these are actual clcemc_had!! (id stored for future reference)
      } // EOL over tmp_track_p 
        
      /////////////////////////////////
      calib_clcemc.clear();
      calib_clcemc_em.clear();
      calib_clcemc_had.clear();
      calib_clihcal.clear();
      calib_clohcal.clear();

      for (int icemc =0; icemc < towerjet4_clcemc_n; icemc ++) {
        //clcemc_totsum += towerjet4_clcemc_e->at(icemc); 
        tmp_calib_clcemc.SetPtEtaPhiM(towerjet4_clcemc_pt->at(icemc),towerjet4_clcemc_eta->at(icemc),towerjet4_clcemc_phi->at(icemc),0);
        calib_clcemc.push_back(tmp_calib_clcemc);
        clcemc_n++;
        //// sort out EM vs. HAD
        if (towerevent==1) cout << "towerjet4_clcemc_id->at(icemc) = " << towerjet4_clcemc_id->at(icemc) << endl;
        bool isHad = false;
        for (int icemchad =0; icemchad < clcemc_had_id.size(); icemchad ++) {
          if ((int)towerjet4_clcemc_id->at(icemc) == (int)clcemc_had_id.at(icemchad) ) {
            isHad = true; break;
          }
        } // EOL over clcemc_had_id (HAD)
        if (towerevent==1) cout << "     isHad = " << (int)isHad <<endl;
        if (isHad){
          clcemc_hadsum += towerjet4_clcemc_e->at(icemc);
          tmp_calib_clcemc_had.SetPtEtaPhiM(towerjet4_clcemc_pt->at(icemc),towerjet4_clcemc_eta->at(icemc),towerjet4_clcemc_phi->at(icemc),0);
          calib_clcemc_had.push_back(tmp_calib_clcemc_had);
          clcemc_had_n++;  
        }else {
          clcemc_emsum += towerjet4_clcemc_e->at(icemc);
          tmp_calib_clcemc_em.SetPtEtaPhiM(towerjet4_clcemc_pt->at(icemc),towerjet4_clcemc_eta->at(icemc),towerjet4_clcemc_phi->at(icemc),0);
          calib_clcemc_em.push_back(tmp_calib_clcemc_em);
          clcemc_em_n++;  
        }
      } // EOL over towerjet4_clcemc_n (ALL)

      for (int iihcal =0; iihcal < towerjet4_clihcal_n; iihcal ++) {
        //clihcal_totsum += towerjet4_clihcal_e->at(iihcal); 
        tmp_calib_clihcal.SetPtEtaPhiM(towerjet4_clihcal_pt->at(iihcal),towerjet4_clihcal_eta->at(iihcal),towerjet4_clihcal_phi->at(iihcal),0);
        calib_clihcal.push_back(tmp_calib_clihcal);
        clihcal_n++; 
      } 
      for (int iohcal =0; iohcal < towerjet4_clohcal_n; iohcal ++) {
        //clohcal_totsum += towerjet4_clohcal_e->at(iohcal); 
        tmp_calib_clohcal.SetPtEtaPhiM(towerjet4_clohcal_pt->at(iohcal),towerjet4_clohcal_eta->at(iohcal),towerjet4_clohcal_phi->at(iohcal),0);
        calib_clohcal.push_back(tmp_calib_clohcal);
        clohcal_n++; 
      } 
     
      for (int icemc =0; icemc < calib_clcemc.size(); icemc ++) {
        clcemc_totsum += calib_clcemc.at(icemc).E();
        tmp_calib_clusterjet += calib_clcemc.at(icemc);
        tmp_calib_myjet += calib_clcemc.at(icemc);
      }
      
      for (int iihcal =0; iihcal < calib_clihcal.size(); iihcal ++) {
        clihcal_totsum += calib_clihcal.at(iihcal).E();
        tmp_calib_clusterjet += calib_clihcal.at(iihcal);
      }
      for (int iohcal =0; iohcal < calib_clohcal.size(); iohcal ++) {
        clohcal_totsum += calib_clohcal.at(iohcal).E();
        tmp_calib_clusterjet += calib_clohcal.at(iohcal);
      }
      if (towerevent==1) cout << "clcemc_n = " << clcemc_n << endl;
      if (towerevent==1) cout << "clcemc_em_n = " <<  clcemc_em_n << endl;
      if (towerevent==1) cout << "clcemc_had_n = " <<  clcemc_had_n << endl;
      if (towerevent==1) cout << "clcemc_totsum = " << clcemc_totsum << endl;
      if (towerevent==1) cout << "clcemc_emsum = " << clcemc_emsum << endl;
      if (towerevent==1) cout << "clcemc_hadsum = " << clcemc_hadsum << endl;
      
      //////////////////////////////////////////////////////////////////////// 
      ////// Assign values
      //////////////////////////////////////////////////////////////////////// 

      reco_e[reco_n]=tmp_calib_myjet.E();
      reco_p[reco_n]=tmp_calib_myjet.P();
      reco_pt[reco_n]=tmp_calib_myjet.Pt();
      reco_eta[reco_n]=tmp_calib_myjet.Eta();
      reco_phi[reco_n]=tmp_calib_myjet.Phi();
      reco_m[reco_n]=tmp_calib_myjet.M();
      //reco_m[reco_n]=sqrt(pow(tmp_calib_myjet.E(),2)-pow(tmp_calib_myjet.P(),2));

      reco_cemcEsum[reco_n]=cemc_sum;
      reco_ihcalEsum[reco_n]=ihcal_sum;
      reco_ohcalEsum[reco_n]=ohcal_sum;
      
      reco_clcemc_n[reco_n]=clcemc_n;
      reco_clcemc_had_n[reco_n]=clcemc_had_n;
      reco_clcemc_em_n[reco_n]=clcemc_em_n;
      //reco_clcemc_em_n[reco_n]=clcemc_n-clcemc_had_n;
      reco_clihcal_n[reco_n]=clihcal_n;
      reco_clohcal_n[reco_n]=clohcal_n;
      reco_clcemc_totEsum[reco_n]=clcemc_totsum;
      reco_clcemc_hadEsum[reco_n]=clcemc_hadsum;
      reco_clcemc_emEsum[reco_n]=clcemc_totsum-clcemc_hadsum;
      reco_clihcal_totEsum[reco_n]=clihcal_totsum;
      reco_clohcal_totEsum[reco_n]=clohcal_totsum;
      
      orig_reco_e[reco_n]=towerjet4_e;
      orig_reco_p[reco_n]=towerjet4_p;
      orig_reco_pt[reco_n]=towerjet4_pt;
      orig_reco_eta[reco_n]=towerjet4_eta;
      orig_reco_phi[reco_n]=towerjet4_phi;
      orig_reco_m[reco_n]=sqrt(pow(towerjet4_e,2)-pow(towerjet4_p,2));
      /*
      if (towerevent==1) {
        cout << "orig_reco_e[reco_n] = " << orig_reco_e[reco_n] << endl;
        cout << "  cemc_sum+ihcal_sum+ohcal_sum = " << cemc_sum+ihcal_sum+ohcal_sum << endl;
        cout << "  clcemc_totsum+clihcal_totsum+clohcal_totsum = " << clcemc_totsum+clihcal_totsum+clohcal_totsum << endl; 
        cout << "  tmp_calib_towerjet.E() = " << tmp_calib_towerjet.E() << endl;
        cout << "  tmp_calib_clusterjet.E() = " << tmp_calib_clusterjet.E() << endl;
        cout << "  tmp_calib_myjet.E() = " << tmp_calib_myjet.E() << endl;
      
        cout << "orig_reco_m[reco_n] = " << orig_reco_m[reco_n] << endl;
        cout << "  my orig_M = " << sqrt(pow(towerjet4_e,2)-pow(towerjet4_p,2)) << endl;
        cout << "reco_m[reco_n] = " << reco_m[reco_n] << endl;
        cout << "  tmp_calib_myjet.M() = " << tmp_calib_myjet.M() << endl; 
        cout << "  my calib M = " << sqrt(pow(tmp_calib_myjet.E(),2)-pow(tmp_calib_myjet.P(),2)) << endl;
        cout << "" << endl;
      }*/

      reco_n++;
    } // EOL over tower jet
    //// 3) save LAST event to tree;
    this_reco_event=prev_towerevent;
    //cout << "  Fill previous evts! this_reco_event = " << this_reco_event << endl;
    outRecoTree->Fill();
    ////
    
    ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////
    //cout << "before closing the file "<< j << endl;
    if (this_true_event%1000==0) {
      cout << "************************ this_true_event =" << this_true_event << ", this_reco_event =" << this_reco_event <<  endl; 
    }
    fin->Close(); 
    //// 4) If last events in the last file is empty, save empty events
    for (int ie = this_true_event+1; ie <= (j+1)*50; ie++){
      this_true_event ++;
      FillEmptyTrueEvents();
      cout << "  Fill last evts! this_true_event = " << this_true_event << endl;
      outTrueTree->Fill();
    }
    for (int ie = this_reco_event+1; ie <= (j+1)*50; ie++){
      this_reco_event ++;
      FillEmptyRecoEvents();
      cout << "  Fill last evts! this_reco_event = " << this_reco_event << endl;
      outRecoTree->Fill();
    }

  } // EOL over files

  //// 4) If last events in the last file is empty, save empty events
  /*
  for (int ie = this_true_event+1; ie <= endfile*50; ie++){
    cout << "ie = " << ie << endl; 
    cout << "this_true_event = " << this_true_event << endl; 
    this_true_event ++;
    FillEmptyTrueEvents();
    outTrueTree->Fill();
  }
  for (int ie = this_reco_event+1; ie <= endfile*50; ie++){
    cout << "ie = " << ie << endl; 
    cout << "this_reco_event = " << this_reco_event << endl; 
    this_reco_event ++;
    FillEmptyRecoEvents();
    outRecoTree->Fill();
  }
  */

  outFile->cd();
  outTrueTree->Write();
  outRecoTree->Write();
  h_track_p_had->Write();
  h_etop_had->Write();
  h_etop->Write();
  for( int ih=0; ih<nhist;ih++){
    h_etop_diff[ih]->Write();
  }
  h_deta->Write();
  h_dphi->Write();
  h_dr->Write();
  h_prob->Write();
  h_prob_had->Write();
  h_chi2->Write();
  h_chi2_had->Write();
  h_pt_tot->Write();
  h_pt_pipm->Write();
  h_pt_pi0->Write();
  h_pt_eta0->Write();
  h_pt_epm->Write();
  h_pt_gamma->Write();
  h_pt_proton->Write();
  h_pt_neutron->Write();
  h_pt_kpm->Write();
  h_pt_k0->Write();
  h_pt_other->Write();
  outFile->Close();
   
  return; 
}

void FillEmptyTrueEvents(){
  true_n=0;
  vtx_x = -999;
  vtx_y = -999;
  vtx_z = -999;
  true_e[true_n]=-999;
  true_p[true_n]=-999;
  true_pt[true_n]=-999;
  true_eta[true_n]=-999;
  true_phi[true_n]=-999;
  true_m[true_n]=-999;
  true_cons_n[true_n]=-999;
  true_pg_n[true_n]=-999;
  true_lcons_z[true_n] =-999;
  true_lcons_pid[true_n] =-999;
  true_emfrac[true_n] =-999;
  true_lpg_id[true_n] =-999;
  true_lpg_fract[true_n] =-999;
  true_lpg_status[true_n] =-999;
}

void FillEmptyRecoEvents(){
  reco_n=0;
  reco_e[reco_n]=-999;
  reco_p[reco_n]=-999;
  reco_pt[reco_n]=-999;
  reco_eta[reco_n]=-999;
  reco_phi[reco_n]=-999;
  reco_m[reco_n]=-999;
  reco_cemc_n[reco_n]=-999;
  reco_ihcal_n[reco_n]=-999;
  reco_ohcal_n[reco_n]=-999;
  reco_cemcEsum[reco_n]=-999;
  reco_ihcalEsum[reco_n]=-999;
  reco_ohcalEsum[reco_n]=-999;
  reco_clcemc_n[reco_n]=-999;
  reco_clcemc_had_n[reco_n]=-999;
  reco_clcemc_em_n[reco_n]=-999;
  reco_clihcal_n[reco_n]=-999;
  reco_clohcal_n[reco_n]=-999;
  reco_clcemc_totEsum[reco_n]=-999;
  reco_clcemc_hadEsum[reco_n]=-999;
  reco_clcemc_emEsum[reco_n]=-999;
  reco_clihcal_totEsum[reco_n]=-999;
  reco_clohcal_totEsum[reco_n]=-999;
  orig_reco_e[reco_n]=-999;
  orig_reco_p[reco_n]=-999;
  orig_reco_pt[reco_n]=-999;
  orig_reco_eta[reco_n]=-999;
  orig_reco_phi[reco_n]=-999;
  orig_reco_m[reco_n]=-999;
}
