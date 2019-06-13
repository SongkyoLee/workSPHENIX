#include "TMinuit.h"

void recursiveFit(TH1D* t_h1, TF1* t_tf1, float t_fitmin, float t_fitmax, float* t_mean, float* t_sig, float* t_sigErr);

void fig05_jes_diffE(int isOpt = 10,
                              string sampleType = "QCDJet",
                              string ihcalType = "SS310",
                              string jetE = "50GeV",
                              int initfile=0,
                              int endfile =1000,
                              float total_sf = 1.00,
                              float cemc_sf = 1.00,
                              float ihcal_sf = 1.00,
                              float ohcal_sf =1.00,
                              bool doetopcut=true,
                              bool dodphicut=true)
{

  int nbin = 50;
  double binmin = 0.; 
  double binmax = 1.6; 
  /////////////////////////////////////////////////////////
  //// check 01
  /////////////////////////////////////////////////////////
  ihcalType = "SS310";
  
  if (isOpt==10){
    total_sf = 1.;
    cemc_sf = 1.;
    ihcal_sf = 1.;
    ohcal_sf = 1.;
  }
  else if (isOpt==30){
    total_sf = 1.;
    cemc_sf = 1.58337;
    ihcal_sf = 2.64023;
    ohcal_sf = 1.43979;
  } else {
    cout << "ERROR! select proper isOpt "; return;
  }


  cemc_sf = cemc_sf*total_sf;
  if (ihcalType.compare("SS310") == 0) { ihcal_sf = ihcal_sf*total_sf; }
  ohcal_sf = ohcal_sf*total_sf;
 
  //// https://root.cern.ch/doc/master/classTColor.html 
  gStyle->SetPalette(52); //black
  
  //string fname01 = Form("./2ndSortedRootFiles_py/2ndSorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_doetopcut%d_dodphicut%d.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),(int)doetopcut,(int)dodphicut);
  string fname01 = Form("./2ndSortedRootFiles/2ndSorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_doetopcut%d_dodphicut%d.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),(int)doetopcut,(int)dodphicut);
  TFile* fin01 = new TFile( fname01.c_str(), "READ");
  TTree* out_tree01 = (TTree*)fin01->Get("out_tree");

  TH1D* h_JES_01 = new TH1D("h_JES_01",";E_{Jet}^{Reco} / E_{Jet}^{Truth};Normalized counts",nbin,binmin,binmax);
  TH1D* h_JER_01 = new TH1D("h_JER_01",";(E_{Jet}^{Reco}-E_{Jet}^{Truth}) / E_{Jet}^{Reco};Normalized counts",nbin,-1.3,0.7);
  h_JES_01->Sumw2();
  h_JER_01->Sumw2();
  
  int evt_01;
  float true_jet_e_01;
  float reco_jet_clcemc_hadEsum_01;
  float reco_jet_clcemc_emEsum_01;
  float reco_jet_ihcalEsum_01;
  float reco_jet_ohcalEsum_01;
  out_tree01->SetBranchAddress("evt",&evt_01);
  out_tree01->SetBranchAddress("true_jet_e",&true_jet_e_01);
  out_tree01->SetBranchAddress("reco_jet_clcemc_hadEsum",&reco_jet_clcemc_hadEsum_01);
  out_tree01->SetBranchAddress("reco_jet_clcemc_emEsum",&reco_jet_clcemc_emEsum_01);
  out_tree01->SetBranchAddress("reco_jet_ihcalEsum",&reco_jet_ihcalEsum_01);
  out_tree01->SetBranchAddress("reco_jet_ohcalEsum",&reco_jet_ohcalEsum_01);

  int nevt_01 = out_tree01->GetEntries();
  //int nevt_01 = 500;
  cout << "nevt_01 = " << nevt_01 << endl;

  for (int ievt=0; ievt <nevt_01; ievt++){
    out_tree01->GetEvent(ievt);
    
    //// scaling
    reco_jet_clcemc_hadEsum_01 = reco_jet_clcemc_hadEsum_01*cemc_sf;
    reco_jet_ihcalEsum_01 = reco_jet_ihcalEsum_01*ihcal_sf;
    reco_jet_ohcalEsum_01 = reco_jet_ohcalEsum_01*ohcal_sf;
    if (isOpt==11) {
      h_JES_01->Fill(( reco_jet_clcemc_emEsum_01*total_sf + reco_jet_clcemc_hadEsum_01 + reco_jet_ihcalEsum_01 + reco_jet_ohcalEsum_01 )/true_jet_e_01);
      h_JER_01->Fill(( reco_jet_clcemc_emEsum_01*total_sf + reco_jet_clcemc_hadEsum_01 + reco_jet_ihcalEsum_01 + reco_jet_ohcalEsum_01 - true_jet_e_01)/ ( reco_jet_clcemc_emEsum_01 + reco_jet_clcemc_hadEsum_01 + reco_jet_ihcalEsum_01 + reco_jet_ohcalEsum_01 ));
    } else {     
      h_JES_01->Fill(( reco_jet_clcemc_emEsum_01 + reco_jet_clcemc_hadEsum_01 + reco_jet_ihcalEsum_01 + reco_jet_ohcalEsum_01 )/true_jet_e_01);
      h_JER_01->Fill(( reco_jet_clcemc_emEsum_01 + reco_jet_clcemc_hadEsum_01 + reco_jet_ihcalEsum_01 + reco_jet_ohcalEsum_01 - true_jet_e_01)/ ( reco_jet_clcemc_emEsum_01 + reco_jet_clcemc_hadEsum_01 + reco_jet_ihcalEsum_01 + reco_jet_ohcalEsum_01 ));
    }
  }
  
  h_JES_01->Scale(1./h_JES_01->Integral());
  h_JER_01->Scale(1./h_JER_01->Integral());
 
  //////////////////////////////////////////////////////
  double fitmin = -1.0;
  double fitmax = 1.0;
  double fitmin2 = -1.0;
  double fitmax2 = 1.0;
  double fitmin3 = -1.0;
  double fitmax3 = 1.0;
  double fitmin4 = 0.0;
  double fitmax4 = 1.0;
  
  ////////////////////////////////////////
  float col;
  int repeat=30;
  float fitmin_jes = 0.5;
  float fitmax_jes = 1.5;
  float mean_jes_01;
  float sig_jes_01;
  float sigErr_jes_01;
  float fitmin_jer_01 = -1.0;
  float fitmax_jer_01 = 1.5;
  float mean_jer_01;
  float sig_jer_01;
  float sigErr_jer_01;

//  TCanvas* c1 = new TCanvas("c1","",1000,500);
//  c1->Divide(2,1);
  
//  c1->cd(1);
  SetHistStyle(h_JES_01,0,0);
  h_JES_01->GetXaxis()->CenterTitle(1);
  h_JES_01->GetYaxis()->CenterTitle(1);
//  h_JES_01->Draw("pe");
  TF1* tf_JES_01= new TF1("TF_JES","gaus",fitmin_jes,fitmax_jes);
  //col = h_JES_01->GetMarkerColor();
  //tf_JES_01->SetLineColor(col);
  tf_JES_01->SetLineColor(kRed-4);
  recursiveFit(h_JES_01, tf_JES_01,fitmin_jes, fitmax_jes, &mean_jes_01, &sig_jes_01, &sigErr_jes_01);
  for (int ir=0; ir<repeat; ir++){
    fitmin_jes = mean_jes_01-2*sig_jes_01;
    fitmax_jes = mean_jes_01+2*sig_jes_01;
    recursiveFit(h_JES_01, tf_JES_01,fitmin_jes, fitmax_jes, &mean_jes_01, &sig_jes_01, &sigErr_jes_01);
  }
//  tf_JES_01->Draw("same");


//  c1->cd(2);
  SetHistStyle(h_JER_01,0,0);
  h_JER_01->GetXaxis()->CenterTitle(1);
  h_JER_01->GetYaxis()->CenterTitle(1);
//  h_JER_01->Draw("pe");
  TF1* tf_JER_01= new TF1("TF_JER","gaus",fitmin_jer_01,fitmax_jer_01);
  //col = h_JER_01->GetMarkerColor();
  //tf_JER_01->SetLineColor(col);
  tf_JER_01->SetLineColor(kRed-4);
  recursiveFit(h_JER_01, tf_JER_01,fitmin_jer_01, fitmax_jer_01, &mean_jer_01, &sig_jer_01, &sigErr_jer_01);
  for (int ir=0; ir<repeat; ir++){
    fitmin_jer_01 = mean_jer_01-2*sig_jer_01;
    fitmax_jer_01 = mean_jer_01+2*sig_jer_01;
    recursiveFit(h_JER_01, tf_JER_01,fitmin_jer_01, fitmax_jer_01, &mean_jer_01, &sig_jer_01, &sigErr_jer_01);
  }
//  tf_JER_01->Draw("same");
  
  cout  << "" << endl;
  cout  << ihcalType.c_str() << " "<< jetE.c_str() << endl;
  cout << "cemcScale = " << cemc_sf << endl;
  cout << "ihcalScale = " << ihcal_sf << endl;
  cout << "ohcalScale = " << ohcal_sf << endl;
  cout << "total_sf = " << total_sf << endl;
  cout << "jes_mean = " << mean_jes_01 << endl;
  cout << "jes_sig = " << sig_jes_01 << endl;
  cout << "jes_sig/mean = " << sig_jes_01/mean_jes_01 << endl;
  cout << "jer_mean = " << mean_jer_01 << endl;
  cout << "jer_sig = " << sig_jer_01 << endl;
  
  
  /////////////////////////////////////////////////////////
  //// check 02
  /////////////////////////////////////////////////////////
  ihcalType = "Alframe";
  //string fname02 = Form("./2ndSortedRootFiles_py/2ndSorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_doetopcut%d_dodphicut%d.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),(int)doetopcut,(int)dodphicut);
  string fname02 = Form("./2ndSortedRootFiles/2ndSorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_doetopcut%d_dodphicut%d.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),(int)doetopcut,(int)dodphicut);
  TFile* fin02 = new TFile( fname02.c_str(), "READ");
  TTree* out_tree02 = (TTree*)fin02->Get("out_tree");
  
  if (isOpt==10){
    total_sf = 1.;
    cemc_sf = 1.;
    ihcal_sf = 1.;
    ohcal_sf = 1.;
  } else if (isOpt==30){
    total_sf = 1.;
    cemc_sf = 2.1035;
    ihcal_sf = 1.;
    ohcal_sf = 1.86363;
  }

  cemc_sf = cemc_sf*total_sf;
  if (ihcalType.compare("SS310") == 0) { ihcal_sf = ihcal_sf*total_sf; }
  ohcal_sf = ohcal_sf*total_sf;
    
  TH1D* h_JES_02 = new TH1D("h_JES_02",";E_{Jet}^{Reco} / E_{Jet}^{Truth};Normalized counts",nbin,binmin,binmax);
  TH1D* h_JER_02 = new TH1D("h_JER_02",";(E_{Jet}^{Reco}-E_{Jet}^{Truth}) / E_{Jet}^{Reco};Normalized counts",nbin,-1.3,0.7);
  h_JES_02->Sumw2();
  h_JER_02->Sumw2();
  
  int evt_02;
  float true_jet_e_02;
  float reco_jet_clcemc_hadEsum_02;
  float reco_jet_clcemc_emEsum_02;
  float reco_jet_ihcalEsum_02;
  float reco_jet_ohcalEsum_02;
  out_tree02->SetBranchAddress("evt",&evt_02);
  out_tree02->SetBranchAddress("true_jet_e",&true_jet_e_02);
  out_tree02->SetBranchAddress("reco_jet_clcemc_hadEsum",&reco_jet_clcemc_hadEsum_02);
  out_tree02->SetBranchAddress("reco_jet_clcemc_emEsum",&reco_jet_clcemc_emEsum_02);
  out_tree02->SetBranchAddress("reco_jet_ihcalEsum",&reco_jet_ihcalEsum_02);
  out_tree02->SetBranchAddress("reco_jet_ohcalEsum",&reco_jet_ohcalEsum_02);

  int nevt_02 = out_tree02->GetEntries();
  //int nevt_02 = 500;
  cout << "nevt_02 = " << nevt_02 << endl;

  for (int ievt=0; ievt <nevt_02; ievt++){
    out_tree02->GetEvent(ievt);
    
    //// scaling
    reco_jet_clcemc_hadEsum_02 = reco_jet_clcemc_hadEsum_02*cemc_sf;
    reco_jet_ihcalEsum_02 = reco_jet_ihcalEsum_02*ihcal_sf;
    reco_jet_ohcalEsum_02 = reco_jet_ohcalEsum_02*ohcal_sf;
    
    if (isOpt==11) {
      h_JES_02->Fill(( reco_jet_clcemc_emEsum_02*total_sf + reco_jet_clcemc_hadEsum_02 + reco_jet_ihcalEsum_02 + reco_jet_ohcalEsum_02 )/true_jet_e_02);
      h_JER_02->Fill(( reco_jet_clcemc_emEsum_02*total_sf + reco_jet_clcemc_hadEsum_02 + reco_jet_ihcalEsum_02 + reco_jet_ohcalEsum_02 - true_jet_e_02)/ ( reco_jet_clcemc_emEsum_02 + reco_jet_clcemc_hadEsum_02 + reco_jet_ihcalEsum_02 + reco_jet_ohcalEsum_02 ));
    } else {     
      h_JES_02->Fill(( reco_jet_clcemc_emEsum_02 + reco_jet_clcemc_hadEsum_02 + reco_jet_ihcalEsum_02 + reco_jet_ohcalEsum_02 )/true_jet_e_02);
      h_JER_02->Fill(( reco_jet_clcemc_emEsum_02 + reco_jet_clcemc_hadEsum_02 + reco_jet_ihcalEsum_02 + reco_jet_ohcalEsum_02 - true_jet_e_02)/ ( reco_jet_clcemc_emEsum_02 + reco_jet_clcemc_hadEsum_02 + reco_jet_ihcalEsum_02 + reco_jet_ohcalEsum_02 ));
    }
  }
  
  h_JES_02->Scale(1./h_JES_02->Integral());
  h_JER_02->Scale(1./h_JER_02->Integral());
  
  ////////////////////////////////////////
  float mean_jes_02;
  float sig_jes_02;
  float sigErr_jes_02;
  float fitmin_jer_02 = -1.0;
  float fitmax_jer_02 = 1.5;
  float mean_jer_02;
  float sig_jer_02;
  float sigErr_jer_02;
 
//  TCanvas* c2 = new TCanvas("c2","",1000,500);
//  c2->Divide(2,1);
  
//  c2->cd(1);
  SetHistStyle(h_JES_02,0,0);
  h_JES_02->GetXaxis()->CenterTitle(1);
  h_JES_02->GetYaxis()->CenterTitle(1);
//  h_JES_02->Draw("pe");
  TF1* tf_JES_02= new TF1("TF_JES","gaus",fitmin_jes,fitmax_jes);
  //col = h_JES_02->GetMarkerColor();
  //tf_JES_02->SetLineColor(col);
  tf_JES_02->SetLineColor(kRed-4);
  recursiveFit(h_JES_02, tf_JES_02,fitmin_jes, fitmax_jes, &mean_jes_02, &sig_jes_02, &sigErr_jes_02);
  for (int ir=0; ir<repeat; ir++){
    fitmin_jes = mean_jes_02-2*sig_jes_02;
    fitmax_jes = mean_jes_02+2*sig_jes_02;
    recursiveFit(h_JES_02, tf_JES_02,fitmin_jes, fitmax_jes, &mean_jes_02, &sig_jes_02, &sigErr_jes_02);
  }
//  tf_JES_02->Draw("same");

//  c2->cd(2);
  SetHistStyle(h_JER_02,0,0);
  h_JER_02->GetXaxis()->CenterTitle(1);
  h_JER_02->GetYaxis()->CenterTitle(1);
//  h_JER_02->Draw("pe");
  TF1* tf_JER_02= new TF1("TF_JER","gaus",fitmin_jer_02,fitmax_jer_02);
  //col = h_JER_02->GetMarkerColor();
  //tf_JER_02->SetLineColor(col);
  tf_JER_02->SetLineColor(kRed-4);
  recursiveFit(h_JER_02, tf_JER_02,fitmin_jer_02, fitmax_jer_02, &mean_jer_02, &sig_jer_02, &sigErr_jer_02);
  for (int ir=0; ir<repeat; ir++){
    fitmin_jer_02 = mean_jer_02-2*sig_jer_02;
    fitmax_jer_02 = mean_jer_02+2*sig_jer_02;
    recursiveFit(h_JER_02, tf_JER_02,fitmin_jer_02, fitmax_jer_02, &mean_jer_02, &sig_jer_02, &sigErr_jer_02);
  }
//  tf_JER_02->Draw("same");
  
  cout  << "" << endl;
  cout  << ihcalType.c_str() << " "<< jetE.c_str() << endl;
  cout << "cemcScale = " << cemc_sf << endl;
  cout << "ihcalScale = " << ihcal_sf << endl;
  cout << "ohcalScale = " << ohcal_sf << endl;
  cout << "total_sf = " << total_sf << endl;
  cout << "jes_mean = " << mean_jes_02 << endl;
  cout << "jes_sig = " << sig_jes_02 << endl;
  cout << "jes_sig/mean = " << sig_jes_02/mean_jes_02 << endl;
  cout << "jer_mean = " << mean_jer_02 << endl;
  cout << "jer_sig = " << sig_jer_02 << endl;
 
  /////////////////////////////////////////////////////////
  //// check 03
  /////////////////////////////////////////////////////////
  ihcalType = "Al";
  //string fname03 = Form("./2ndSortedRootFiles_py/2ndSorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_doetopcut%d_dodphicut%d.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),(int)doetopcut,(int)dodphicut);
  string fname03 = Form("./2ndSortedRootFiles/2ndSorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_doetopcut%d_dodphicut%d.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),(int)doetopcut,(int)dodphicut);
  TFile* fin03 = new TFile( fname03.c_str(), "READ");
  TTree* out_tree03 = (TTree*)fin03->Get("out_tree");
  
  if (isOpt==10){
    total_sf = 1.;
    cemc_sf = 1.;
    ihcal_sf = 1.;
    ohcal_sf = 1.;
  } else if (isOpt==30){
    total_sf = 1.;
    cemc_sf = 2.1035;
    ihcal_sf = 1.;
    ohcal_sf = 1.86363;
  }

  cemc_sf = cemc_sf*total_sf;
  if (ihcalType.compare("SS310") == 0) { ihcal_sf = ihcal_sf*total_sf; }
  ohcal_sf = ohcal_sf*total_sf;
    
  TH1D* h_JES_03 = new TH1D("h_JES_03",";E_{Jet}^{Reco} / E_{Jet}^{Truth};Normalized counts",nbin,binmin,binmax);
  TH1D* h_JER_03 = new TH1D("h_JER_03",";(E_{Jet}^{Reco}-E_{Jet}^{Truth}) / E_{Jet}^{Reco};Normalized counts",nbin,-1.3,0.7);
  h_JES_03->Sumw2();
  h_JER_03->Sumw2();
  
  int evt_03;
  float true_jet_e_03;
  float reco_jet_clcemc_hadEsum_03;
  float reco_jet_clcemc_emEsum_03;
  float reco_jet_ihcalEsum_03;
  float reco_jet_ohcalEsum_03;
  out_tree03->SetBranchAddress("evt",&evt_03);
  out_tree03->SetBranchAddress("true_jet_e",&true_jet_e_03);
  out_tree03->SetBranchAddress("reco_jet_clcemc_hadEsum",&reco_jet_clcemc_hadEsum_03);
  out_tree03->SetBranchAddress("reco_jet_clcemc_emEsum",&reco_jet_clcemc_emEsum_03);
  out_tree03->SetBranchAddress("reco_jet_ihcalEsum",&reco_jet_ihcalEsum_03);
  out_tree03->SetBranchAddress("reco_jet_ohcalEsum",&reco_jet_ohcalEsum_03);

  int nevt_03 = out_tree03->GetEntries();
  //int nevt_03 = 500;
  cout << "nevt_03 = " << nevt_03 << endl;

  for (int ievt=0; ievt <nevt_03; ievt++){
    out_tree03->GetEvent(ievt);
    
    //// scaling
    reco_jet_clcemc_hadEsum_03 = reco_jet_clcemc_hadEsum_03*cemc_sf;
    reco_jet_ihcalEsum_03 = reco_jet_ihcalEsum_03*ihcal_sf;
    reco_jet_ohcalEsum_03 = reco_jet_ohcalEsum_03*ohcal_sf;
    if (isOpt==11) {
      h_JES_03->Fill(( reco_jet_clcemc_emEsum_03*total_sf + reco_jet_clcemc_hadEsum_03 + reco_jet_ihcalEsum_03 + reco_jet_ohcalEsum_03 )/true_jet_e_03);
      h_JER_03->Fill(( reco_jet_clcemc_emEsum_03*total_sf + reco_jet_clcemc_hadEsum_03 + reco_jet_ihcalEsum_03 + reco_jet_ohcalEsum_03 - true_jet_e_03)/ ( reco_jet_clcemc_emEsum_03 + reco_jet_clcemc_hadEsum_03 + reco_jet_ihcalEsum_03 + reco_jet_ohcalEsum_03 ));
    } else {     
      h_JES_03->Fill(( reco_jet_clcemc_emEsum_03 + reco_jet_clcemc_hadEsum_03 + reco_jet_ihcalEsum_03 + reco_jet_ohcalEsum_03 )/true_jet_e_03);
      h_JER_03->Fill(( reco_jet_clcemc_emEsum_03 + reco_jet_clcemc_hadEsum_03 + reco_jet_ihcalEsum_03 + reco_jet_ohcalEsum_03 - true_jet_e_03)/ ( reco_jet_clcemc_emEsum_03 + reco_jet_clcemc_hadEsum_03 + reco_jet_ihcalEsum_03 + reco_jet_ohcalEsum_03 ));
    }
    //h_JES_03->Fill(( reco_jet_clcemc_emEsum_03 + reco_jet_clcemc_hadEsum_03 + reco_jet_ohcalEsum_03 )/true_jet_e_03);
    //h_JER_03->Fill(( reco_jet_clcemc_emEsum_03 + reco_jet_clcemc_hadEsum_03 + reco_jet_ohcalEsum_03 - true_jet_e_03)/ ( reco_jet_clcemc_emEsum_03 + reco_jet_clcemc_hadEsum_03 + reco_jet_ohcalEsum_03 ));
  }
  h_JES_03->Scale(1./h_JES_03->Integral());
  h_JER_03->Scale(1./h_JER_03->Integral());
  
  ////////////////////////////////////////
  float mean_jes_03;
  float sig_jes_03;
  float sigErr_jes_03;
  float fitmin_jer_03 = -1.0;
  float fitmax_jer_03 = 1.5;
  float mean_jer_03;
  float sig_jer_03;
  float sigErr_jer_03;
 
//  TCanvas* c2 = new TCanvas("c2","",1000,500);
//  c2->Divide(2,1);
  
//  c2->cd(1);
  SetHistStyle(h_JES_03,0,0);
  h_JES_03->GetXaxis()->CenterTitle(1);
  h_JES_03->GetYaxis()->CenterTitle(1);
//  h_JES_03->Draw("pe");
  TF1* tf_JES_03= new TF1("TF_JES","gaus",fitmin_jes,fitmax_jes);
  //col = h_JES_03->GetMarkerColor();
  //tf_JES_03->SetLineColor(col);
  tf_JES_03->SetLineColor(kRed-4);
  recursiveFit(h_JES_03, tf_JES_03,fitmin_jes, fitmax_jes, &mean_jes_03, &sig_jes_03, &sigErr_jes_03);
  for (int ir=0; ir<repeat; ir++){
    fitmin_jes = mean_jes_03-2*sig_jes_03;
    fitmax_jes = mean_jes_03+2*sig_jes_03;
    recursiveFit(h_JES_03, tf_JES_03,fitmin_jes, fitmax_jes, &mean_jes_03, &sig_jes_03, &sigErr_jes_03);
  }
//  tf_JES_03->Draw("same");

//  c2->cd(2);
  SetHistStyle(h_JER_03,0,0);
  h_JER_03->GetXaxis()->CenterTitle(1);
  h_JER_03->GetYaxis()->CenterTitle(1);
//  h_JER_03->Draw("pe");
  TF1* tf_JER_03= new TF1("TF_JER","gaus",fitmin_jer_03,fitmax_jer_03);
  //col = h_JER_03->GetMarkerColor();
  //tf_JER_03->SetLineColor(col);
  tf_JER_03->SetLineColor(kRed-4);
  recursiveFit(h_JER_03, tf_JER_03,fitmin_jer_03, fitmax_jer_03, &mean_jer_03, &sig_jer_03, &sigErr_jer_03);
  for (int ir=0; ir<repeat; ir++){
    fitmin_jer_03 = mean_jer_03-2*sig_jer_03;
    fitmax_jer_03 = mean_jer_03+2*sig_jer_03;
    recursiveFit(h_JER_03, tf_JER_03,fitmin_jer_03, fitmax_jer_03, &mean_jer_03, &sig_jer_03, &sigErr_jer_03);
  }
//  tf_JER_03->Draw("same");
  
  cout  << "" << endl;
  cout  << ihcalType.c_str() << " "<< jetE.c_str() << endl;
  cout << "cemcScale = " << cemc_sf << endl;
  cout << "ihcalScale = " << ihcal_sf << endl;
  cout << "ohcalScale = " << ohcal_sf << endl;
  cout << "total_sf = " << total_sf << endl;
  cout << "jes_mean = " << mean_jes_03 << endl;
  cout << "jes_sig = " << sig_jes_03 << endl;
  cout << "jes_sig/mean = " << sig_jes_03/mean_jes_03 << endl;
  cout << "jer_mean = " << mean_jer_03 << endl;
  cout << "jer_sig = " << sig_jer_03 << endl;
 
 
 
 
  
  /////////////////////////////////////////////////////////
  //// compare JES
  /////////////////////////////////////////////////////////

  TCanvas* c3 = new TCanvas("c3","",500,500);
  c3->cd();
  SetHistStyle(h_JES_01,2,11);
  SetHistStyle(h_JES_02,1,0);
  SetHistStyle(h_JES_03,3,12);
  h_JES_01->SetMaximum(0.18);
  h_JES_01->GetYaxis()->SetTitleOffset(1.6);
  h_JES_01->Draw("pe");
  h_JES_02->Draw("pe same");
  h_JES_03->Draw("pe same");
  //SetLineHistStyle(h_JES_01,1);
  //SetLineHistStyle(h_JES_02,2);
  //h_JES_01->Draw("hist");
  //h_JES_02->Draw("hist same");
  tf_JES_01->SetLineColor(kBlue-9);
  tf_JES_01->SetLineStyle(7);
  tf_JES_01->SetRange(binmin,binmax);
  tf_JES_01->Draw("same");
  tf_JES_02->SetLineColor(kRed-7);
  tf_JES_02->SetLineStyle(7);
  tf_JES_02->SetRange(binmin,binmax);
  tf_JES_02->Draw("same");
  tf_JES_03->SetLineColor(kGreen+1);
  tf_JES_03->SetLineStyle(7);
  tf_JES_03->SetRange(binmin,binmax);
  tf_JES_03->Draw("same");
  //myLatex(0.7,0.85,"SS310",12,0.05,kPink+3);
  //myLatex(0.7,0.75,"Alframe",12,0.05,kAzure+3);
 
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  //latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->DrawLatex(0.42,0.89,"MIE 2018");
  latex->SetTextSize(0.040);
  //latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4, |#eta_{Jet}^{truth}| < 0.45");
  //latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4,");
  //latex->DrawLatex(0.2,0.77,"|#eta_{Jet}^{truth}| < 0.45, 20 <|E_{Jet}^{truth}| < 30 GeV");
  latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, R=0.4");
  double tmpE = atof(jetE.c_str());
  cout << "tmpE = " << tmpE << endl;
  latex->DrawLatex(0.2,0.76,Form("%.0f <E_{Jet}^{Truth} < %.0f GeV",tmpE,tmpE+10));
  
  //TLegend* leg1 = new TLegend(0.63, 0.56, 0.93, 0.70);
  TLegend* leg1 = new TLegend(0.62, 0.48, 0.93, 0.70);
  leg1->SetTextSize(0.042);
  //leg1->AddEntry(h_JES_02,"CD-1 Configuration","lp");
  //leg1->AddEntry(h_JES_01,"Instrumented Steel","lp");
  TH1D* h_dummy = (TH1D*)h_JES_02->Clone("h_dummy");
  h_dummy->SetFillColor(kWhite);
  h_dummy->SetLineColor(kWhite);
  leg1->AddEntry(h_JES_02,"CD-1","lp");
  leg1->AddEntry(h_dummy,"Configuration","f");
  leg1->AddEntry(h_JES_01,"Instrumented","lp");
  leg1->AddEntry(h_dummy,"Steel","f");
  leg1->AddEntry(h_JES_03,"Instrumented","lp");
  leg1->AddEntry(h_dummy,"Aluminum","f");
  leg1->Draw();
    

  ////// check low-side tails
  
  //double integ_tot_01 = h_JES_01->Integral();
  double integ_tot_01 = h_JES_01->Integral(0,nbin);
  double lowbin_01 = h_JES_01->GetXaxis()->FindBin(mean_jes_01-2*sig_jes_01);
  double highbin_01 = h_JES_01->GetXaxis()->FindBin(mean_jes_01+2*sig_jes_01);
  double integ_low_01 = h_JES_01->Integral(0,lowbin_01);
  double integ_mid_01 = h_JES_01->Integral(lowbin_01+1,highbin_01-1);
  double integ_high_01 = h_JES_01->Integral(highbin_01,nbin);
  double errlow01 = (1/integ_tot_01)*sqrt(integ_low_01*(1-integ_low_01/integ_tot_01)); 
  double errhigh01 = (1/integ_tot_01)*sqrt(integ_high_01*(1-integ_high_01/integ_tot_01)); 
  cout << "integ_tot_01 = " << integ_tot_01 << endl;
  //cout << "lowbin_01 = " << lowbin_01 << endl;
  //cout << "highbin_01 = " << highbin_01 << endl;
  cout << "integ_low_01 = " << integ_low_01 << endl;
  //cout << "integ_mid_01 = " << integ_mid_01 << endl;
  //cout << "integ_high_01 = " << integ_high_01 << endl;
  cout << "************* SS310 ***********" << endl;
  cout << "low tail fraction = " << integ_low_01/integ_tot_01<<" +- " << errlow01 << endl;
  cout << "high tail fraction = " << integ_high_01/integ_tot_01<<" +- " << errhigh01 << endl;

  //double integ_tot_02 = h_JES_02->Integral();
  double integ_tot_02 = h_JES_02->Integral(0,nbin);
  double lowbin_02 = h_JES_02->GetXaxis()->FindBin(mean_jes_02-2*sig_jes_02);
  double highbin_02 = h_JES_02->GetXaxis()->FindBin(mean_jes_02+2*sig_jes_02);
  double integ_low_02 = h_JES_02->Integral(0,lowbin_02);
  double integ_mid_02 = h_JES_02->Integral(lowbin_02+1,highbin_02-1);
  double integ_high_02 = h_JES_02->Integral(highbin_02,nbin);
  double errlow02 = (1/integ_tot_02)*sqrt(integ_low_02*(1-integ_low_02/integ_tot_02)); 
  double errhigh02 = (1/integ_tot_02)*sqrt(integ_high_02*(1-integ_high_02/integ_tot_02)); 
  //cout << "integ_tot_02 = " << integ_tot_02 << endl;
  //cout << "lowbin_02 = " << lowbin_02 << endl;
  //cout << "highbin_02 = " << highbin_02 << endl;
  //cout << "integ_low_02 = " << integ_low_02 << endl;
  //cout << "integ_mid_02 = " << integ_mid_02 << endl;
  //cout << "integ_high_02 = " << integ_high_02 << endl;
  cout << "************* Alframe ***********" << endl;
  cout << "low tail fraction = " << integ_low_02/integ_tot_02<<" +- " << errlow02 << endl;
  cout << "high tail fraction = " << integ_high_02/integ_tot_02<<" +- " << errhigh02 << endl;
  
  //double integ_tot_03 = h_JES_03->Integral();
  double integ_tot_03 = h_JES_03->Integral(0,nbin);
  double lowbin_03 = h_JES_03->GetXaxis()->FindBin(mean_jes_03-2*sig_jes_03);
  double highbin_03 = h_JES_03->GetXaxis()->FindBin(mean_jes_03+2*sig_jes_03);
  double integ_low_03 = h_JES_03->Integral(0,lowbin_03);
  double integ_mid_03 = h_JES_03->Integral(lowbin_03+1,highbin_03-1);
  double integ_high_03 = h_JES_03->Integral(highbin_03,nbin);
  double errlow03 = (1/integ_tot_03)*sqrt(integ_low_03*(1-integ_low_03/integ_tot_03)); 
  double errhigh03 = (1/integ_tot_03)*sqrt(integ_high_03*(1-integ_high_03/integ_tot_03)); 
  //cout << "integ_tot_03 = " << integ_tot_03 << endl;
  //cout << "lowbin_03 = " << lowbin_03 << endl;
  //cout << "highbin_03 = " << highbin_03 << endl;
  //cout << "integ_low_03 = " << integ_low_03 << endl;
  //cout << "integ_mid_03 = " << integ_mid_03 << endl;
  //cout << "integ_high_03 = " << integ_high_03 << endl;
  cout << "************* Al ***********" << endl;
  cout << "low tail fraction = " << integ_low_03/integ_tot_03<<" +- " << errlow03 << endl;
  cout << "high tail fraction = " << integ_high_03/integ_tot_03<<" +- " << errhigh03 << endl;
 
  if (isOpt==10){
//    c3->SaveAs(Form("out_figs/hcal_jes_1D_%s.pdf",jetE.c_str()));
  } else {
//    c3->SaveAs(Form("out_figs/hcal_jes_1D_%s_calib.pdf",jetE.c_str()));
  }
 /* 
  /////////////////////////////////////////////////////////
  //// compare JER
  /////////////////////////////////////////////////////////

  TCanvas* c4 = new TCanvas("c4","",500,500);
  c4->cd();
  SetHistStyle(h_JER_01,2,11);
  SetHistStyle(h_JER_02,1,0);
  h_JER_01->SetMaximum(0.12);
  h_JER_01->Draw("pe");
  h_JER_02->Draw("pe same");
  //SetLineHistStyle(h_JER_01,1);
  //SetLineHistStyle(h_JER_02,2);
  //h_JER_01->Draw("hist");
  //h_JER_02->Draw("hist same");
  tf_JER_01->SetLineColor(kBlue-9);
  tf_JER_01->SetLineStyle(7);
//  tf_JER_01->SetRange(binmin,binmax);
  tf_JER_01->Draw("same");
  tf_JER_02->SetLineColor(kRed-7);
  tf_JER_02->SetLineStyle(7);
//  tf_JER_02->SetRange(binmin,binmax);
  tf_JER_02->Draw("same");
  //myLatex(0.7,0.85,"SS310",12,0.05,kPink+3);
  //myLatex(0.7,0.75,"Alframe",12,0.05,kAzure+3);
 
  //TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  //latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->DrawLatex(0.42,0.89,"MIE 2018");
  latex->SetTextSize(0.040);
  //latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4, |#eta_{Jet}^{truth}| < 0.45");
  //latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4,");
  //latex->DrawLatex(0.2,0.77,"|#eta_{Jet}^{truth}| < 0.45, 20 <|E_{Jet}^{truth}| < 30 GeV");
  latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, R=0.4");
  latex->DrawLatex(0.2,0.76,Form("%.0f <E_{Jet}^{Truth} < %.0f GeV",tmpE,tmpE+10));
  
  //TLegend* leg1 = new TLegend(0.63, 0.56, 0.93, 0.70);
  //leg1->SetTextSize(0.043);
  //leg1->AddEntry(h_JES_01,"Instrumented Steel","lp");
  //leg1->AddEntry(h_JES_02,"CD-1 Configuration","lp");
  leg1->Draw();
  
  if (isOpt==10){
    c4->SaveAs(Form("out_figs/hcal_jer_1D_%s.pdf",jetE.c_str()));
  } else {
    c4->SaveAs(Form("out_figs/hcal_jer_1D_%s_calib.pdf",jetE.c_str()));
  }
*/  
  return;
}
////////////////////////////////////////

void recursiveFit(TH1D* t_h1, TF1* t_tf1, float t_fitmin, float t_fitmax, float* t_mean, float* t_sig, float* t_sigErr){
  string tfname = t_tf1->GetName();
  //t_h1[ih]->Fit(Form("tf1_%d",ih),"NR"); 
  //t_h1->Fit(tfname.c_str(),"NR"); 
  t_tf1->SetRange(t_fitmin,t_fitmax);
  t_h1->Fit(tfname.c_str(),"NRQ","",t_fitmin,t_fitmax); 
  //sig[ih] = tf1[ih]->GetParameter(2);
  //sigErr[ih] = tf1[ih]->GetParError(2);
  *t_mean = t_tf1->GetParameter(1);
  *t_sig = t_tf1->GetParameter(2);
  *t_sigErr =  t_tf1->GetParError(2);

  return;
}
