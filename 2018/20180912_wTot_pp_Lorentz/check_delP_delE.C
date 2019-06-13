#include "TMinuit.h"

void recursiveFit(TH1D* t_h1, TF1* t_tf1, float t_fitmin, float t_fitmax, float* t_mean, float* t_sig, float* t_sigErr);

void check_delP_delE(string sampleType = "QCDJet",
                              string ihcalType = "Alframe",
                              string jetE = "20GeV",
                              string note = "R04",
                              int initfile=0,
                              int endfile =400,
                              float total_sf = 1.00,
                              float cemc_sf = 1.00,
                              float ihcal_sf = 1.00,
                              float ohcal_sf =1.00,
                              bool doetopcut=true,
                              bool dodphicut=true)
{
  cemc_sf = cemc_sf*total_sf;
  ihcal_sf = ihcal_sf*total_sf;
  ohcal_sf = ohcal_sf*total_sf;
 
  //// https://root.cern.ch/doc/master/classTColor.html 
  //gStyle->SetPalette(1);
  //gStyle->SetPalette(kBird); //default
  //gStyle->SetPalette(51); //navy
  //gStyle->SetPalette(52); //black
  gStyle->SetPalette(55); //rainbow color
  //gStyle->SetPalette(kFuchsia); //warm violet
  //gStyle->SetPalette(kLake); //cool violet
  //TColor::InvertPalette();
  
  string fname;
//  if (ihcalType.compare("Alframe") == 0) { 
//    fname = Form("./2ndSortedRootFiles/2ndSorted_%dto%d_%s_G4sPHENIX_jet4_%s_Al_%s_doetopcut%d_dodphicut%d.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),note.c_str(),(int)doetopcut,(int)dodphicut);
//  } else {
    fname = Form("./2ndSortedRootFiles/2ndSorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_%s_doetopcut%d_dodphicut%d.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),note.c_str(),(int)doetopcut,(int)dodphicut);
//  }
  TFile* fin = new TFile( fname.c_str(), "READ");
  if(!fin->IsOpen()) { cout <<"WARNING!! file not found"<<endl; return;};
  TTree* out_tree = (TTree*)fin->Get("out_tree");

  //TH2D* h_emfrac = new TH2D("h_emfrac",";Truth EM fraction;Jet E_{Reco} / Jet E_{Truth}",101,-0.05,1.05,100,0,2);
  //TH2D* h_JES_JPTS = new TH2D("h_JES_JPTS",";p_{T,Jet}^{Reco}/p_{T,Jet}^{Truth};E_{Jet}^{Reco}/E_{Jet}^{Truth}",100,0,2,100,0,2);
  TH2D* h_JER_JPR = new TH2D("h_JER_JPR",";(E_{Jet}^{Reco}-E_{Jet}^{Truth}) / E_{Jet}^{Truth};(p_{Jet}^{Reco}-p_{Jet}^{Truth}) / p_{Jet}^{Truth}",100,-1,1,100,-1,1);
  TH2D* h_JER_JPR_abs = new TH2D("h_JER_JPR_abs",";| E_{Jet}^{Reco}-E_{Jet}^{Truth} | / E_{Jet}^{Truth};| p_{Jet}^{Reco}-p_{Jet}^{Truth} | / p_{Jet}^{Truth}",100,0,1,100,0,1);
  TH2D* h_JER_JPTR = new TH2D("h_JER_JPTR",";(E_{Jet}^{Reco}-E_{Jet}^{Truth}) / E_{Jet}^{Truth};(p_{T,Jet}^{Reco}-p_{T,Jet}^{Truth}) / p_{T,Jet}^{Truth}",100,-1,1,100,-1,1);
  TH2D* h_JER_JPTR_abs = new TH2D("h_JER_JPTR_abs",";| E_{Jet}^{Reco}-E_{Jet}^{Truth} | / E_{Jet}^{Truth};| p_{T,Jet}^{Reco}-p_{T,Jet}^{Truth} | / p_{T,Jet}^{Truth}",100,0,1,100,0,1);
  TH2D* h_JER_JMR = new TH2D("h_JER_JMR",";(E_{Jet}^{Reco}-E_{Jet}^{Truth}) / E_{Jet}^{Truth};(M_{Jet}^{Reco}-M_{Jet}^{Truth}) / M_{Jet}^{Truth}",100,-1,1,100,-1,1);
  TH2D* h_JER_JMR_abs = new TH2D("h_JER_JMR_abs",";| E_{Jet}^{Reco}-E_{Jet}^{Truth} | / E_{Jet}^{Truth};| M_{Jet}^{Reco}-M_{Jet}^{Truth} | / M_{Jet}^{Truth}",100,0,1,100,0,1);
 
  int evt;
  float true_jet_e;
  float true_jet_p;
  float true_jet_pt;
  float true_jet_emfrac;
  float reco_jet_e;
  float reco_jet_p;
  float reco_jet_pt;
  float reco_jet_clcemc_hadEsum;
  float reco_jet_clcemc_emEsum;
  float reco_jet_cemcEsum;
  float reco_jet_ihcalEsum;
  float reco_jet_ohcalEsum;
  out_tree->SetBranchAddress("evt",&evt);
  out_tree->SetBranchAddress("true_jet_e",&true_jet_e);
  out_tree->SetBranchAddress("true_jet_p",&true_jet_p);
  out_tree->SetBranchAddress("true_jet_pt",&true_jet_pt);
  out_tree->SetBranchAddress("true_jet_emfrac",&true_jet_emfrac);
  out_tree->SetBranchAddress("reco_jet_e",&reco_jet_e);
  out_tree->SetBranchAddress("reco_jet_p",&reco_jet_p);
  out_tree->SetBranchAddress("reco_jet_pt",&reco_jet_pt);
  out_tree->SetBranchAddress("reco_jet_cemcEsum",&reco_jet_cemcEsum);
  out_tree->SetBranchAddress("reco_jet_clcemc_hadEsum",&reco_jet_clcemc_hadEsum);
  out_tree->SetBranchAddress("reco_jet_clcemc_emEsum",&reco_jet_clcemc_emEsum);
  out_tree->SetBranchAddress("reco_jet_ihcalEsum",&reco_jet_ihcalEsum);
  out_tree->SetBranchAddress("reco_jet_ohcalEsum",&reco_jet_ohcalEsum);

  float reco_jet_caliE;
  float ihasymm, ihfrac;
  float ehasymm, ehfrac;
  float totehasymm, totehfrac;
 
  float true_jet_m;
  float reco_jet_m;
   
  int nevt = out_tree->GetEntries();
  cout << "nevt = " << nevt << endl;

  for (int ievt=0; ievt <nevt; ievt++){
    out_tree->GetEvent(ievt);
    
    //// scaling
    reco_jet_clcemc_hadEsum = reco_jet_clcemc_hadEsum*cemc_sf;
//    if (ihcalType.compare("Alframe") == 0) { reco_jet_ihcalEsum = 0; } 
//    else { reco_jet_ihcalEsum = reco_jet_ihcalEsum*ihcal_sf; }
    reco_jet_ihcalEsum = reco_jet_ihcalEsum*ihcal_sf;
    reco_jet_ohcalEsum = reco_jet_ohcalEsum*ohcal_sf;
    
    //// calculation
    true_jet_m = sqrt(pow(true_jet_e,2)-pow(true_jet_p,2));
    reco_jet_m = sqrt(pow(reco_jet_e,2)-pow(reco_jet_p,2));
     
    //h_emfrac -> Fill(true_jet_emfrac,( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum )/true_jet_e);
    //h_JES_JPTS -> Fill(( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum )/true_jet_e,reco_jet_pt/true_jet_pt);
    h_JER_JPR -> Fill((( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum ) - true_jet_e ) /true_jet_e,(reco_jet_p-true_jet_p)/true_jet_p);
    h_JER_JPR_abs -> Fill(fabs(( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum ) - true_jet_e ) /true_jet_e,fabs(reco_jet_p-true_jet_p)/true_jet_p);
    h_JER_JPTR -> Fill((( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum ) - true_jet_e ) /true_jet_e,(reco_jet_pt-true_jet_pt)/true_jet_pt);
    h_JER_JPTR_abs -> Fill(fabs(( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum ) - true_jet_e ) /true_jet_e,fabs(reco_jet_pt-true_jet_pt)/true_jet_pt);
    h_JER_JMR -> Fill((( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum ) - true_jet_e ) /true_jet_e,(reco_jet_m-true_jet_m)/true_jet_m);
    h_JER_JMR_abs -> Fill(fabs(( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum ) - true_jet_e ) /true_jet_e,fabs(reco_jet_m-true_jet_m)/true_jet_m);
  }
 
  //h_emfrac->SetOption("colz");
  h_JER_JPR->SetOption("colz");
  h_JER_JPR_abs->SetOption("colz");
  h_JER_JPTR->SetOption("colz");
  h_JER_JPTR_abs->SetOption("colz");
  h_JER_JMR->SetOption("colz");
  h_JER_JMR_abs->SetOption("colz");

  //TCanvas* c1 = new TCanvas("c1","",1200,800);
  TCanvas* c1 = new TCanvas("c1","",1000,1000);
  c1->Divide(2,2);
  c1->cd(1);
  h_JER_JPR->GetXaxis()->CenterTitle(1);
  h_JER_JPR->GetYaxis()->CenterTitle(1);
  h_JER_JPR->Draw();
  c1->cd(2);
  h_JER_JPTR->GetXaxis()->CenterTitle(1);
  h_JER_JPTR->GetYaxis()->CenterTitle(1);
  h_JER_JPTR->Draw();
  c1->cd(3);
  h_JER_JMR->GetXaxis()->CenterTitle(1);
  h_JER_JMR->GetYaxis()->CenterTitle(1);
  h_JER_JMR->Draw();
 
  //TCanvas* c2 = new TCanvas("c2","",1200,800);
  TCanvas* c2 = new TCanvas("c2","",1000,1000);
  c2->Divide(2,2);
  c2->cd(1);
  h_JER_JPR_abs->GetXaxis()->CenterTitle(1);
  h_JER_JPR_abs->GetYaxis()->CenterTitle(1);
  h_JER_JPR_abs->Draw();
  c2->cd(2);
  h_JER_JPTR_abs->GetXaxis()->CenterTitle(1);
  h_JER_JPTR_abs->GetYaxis()->CenterTitle(1);
  h_JER_JPTR_abs->Draw();
  c2->cd(3);
  h_JER_JMR_abs->GetXaxis()->CenterTitle(1);
  h_JER_JMR_abs->GetYaxis()->CenterTitle(1);
  h_JER_JMR_abs->Draw();
 
   
  return; 
}
////////////////////////////////////////

void recursiveFit(TH1D* t_h1, TF1* t_tf1, float t_fitmin, float t_fitmax, float* t_mean, float* t_sig, float* t_sigErr){
  string tfname = t_tf1->GetName();
  //t_h1[ih]->Fit(Form("tf1_%d",ih),"NR"); 
  //t_h1->Fit(tfname.c_str(),"NR"); 
  t_tf1->SetRange(t_fitmin,t_fitmax);
  t_h1->Fit(tfname.c_str(),"NR","",t_fitmin,t_fitmax); 
  //sig[ih] = tf1[ih]->GetParameter(2);
  //sigErr[ih] = tf1[ih]->GetParError(2);
  *t_mean = t_tf1->GetParameter(1);
  *t_sig = t_tf1->GetParameter(2);
  *t_sigErr =  t_tf1->GetParError(2);

  return;
}
