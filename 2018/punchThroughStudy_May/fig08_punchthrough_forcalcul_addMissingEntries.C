#include "TMinuit.h"

void recursiveFit(TH1D* t_h1, TF1* t_tf1, float t_fitmin, float t_fitmax, float* t_mean, float* t_sig, float* t_sigErr);

void fig08_punchthrough_forcalcul_addMissingEntries(bool isHighz = false,
                              int isOpt = 10,
                              string sampleType = "QCDJet",
                              string ihcalType = "SS310",
                              string jetE = "40GeV",
                              int initfile=0,
                              int endfile =1000,
                              float total_sf = 1.00,
                              float cemc_sf = 1.00,
                              float ihcal_sf = 1.00,
                              float ohcal_sf =1.00,
                              bool doetopcut=true,
                              bool dodphicut=true)
{

  int nbin = 800;
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
  else if (isOpt==11){
    //total_sf = 1.16/0.77734;
    total_sf = 1./0.77734;
    cemc_sf = 1.;
    ihcal_sf = 1.;
    ohcal_sf = 1.;
  } else if (isOpt==20){
    total_sf = 1;
    cemc_sf = 1.18059;
    ihcal_sf = 2.19145;
    ohcal_sf = 1.47074;
  } else if (isOpt==21){
    total_sf = 0.99/(1.02643);
    cemc_sf = 1.18059;
    ihcal_sf = 2.19145;
    ohcal_sf = 1.47074;
  } else {
    cout << "ERROR! select proper isOpt "; return;
  }


  cemc_sf = cemc_sf*total_sf;
  if (ihcalType.compare("SS310") == 0) { ihcal_sf = ihcal_sf*total_sf; }
  ohcal_sf = ohcal_sf*total_sf;
 
  //// https://root.cern.ch/doc/master/classTColor.html 
  gStyle->SetPalette(52); //black
  
  string fname01;
  if (isHighz) { fname01 = Form("./2ndSortedRootFiles_highz/2ndSorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_doetopcut%d_dodphicut%d_highz.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),(int)doetopcut,(int)dodphicut); }
  else { fname01 = Form("./2ndSortedRootFiles_highz/2ndSorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_doetopcut%d_dodphicut%d.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),(int)doetopcut,(int)dodphicut); }
  TFile* fin01 = new TFile( fname01.c_str(), "READ");
  TTree* out_tree01 = (TTree*)fin01->Get("out_tree");

  TH1D* h_JES_01 = new TH1D("h_JES_01",";E_{Jet}^{Reco} / E_{Jet}^{Truth};Counts",nbin,binmin,binmax);
  TH1D* h_JER_01 = new TH1D("h_JER_01",";(Jet E_{Reco}-Jet E_{Truth}) / Jet E_{Reco};Counts",100,-1,1);
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
  
//  h_JES_01->Scale(1./h_JES_01->Integral());
//  h_JER_01->Scale(1./h_JER_01->Integral());
 
  //////////////////////////////////////////////////////
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(32);
  latex->SetTextSize(0.045);
     
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
  string fname02;
  if (isHighz) { fname02 = Form("./2ndSortedRootFiles_highz/2ndSorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_doetopcut%d_dodphicut%d_highz.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),(int)doetopcut,(int)dodphicut); }
  else { fname02 = Form("./2ndSortedRootFiles_highz/2ndSorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_doetopcut%d_dodphicut%d.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),(int)doetopcut,(int)dodphicut); }
  TFile* fin02 = new TFile( fname02.c_str(), "READ");
  TTree* out_tree02 = (TTree*)fin02->Get("out_tree");
  
  if (isOpt==10){
    total_sf = 1.;
    cemc_sf = 1.;
    ihcal_sf = 1.;
    ohcal_sf = 1.;
  } else if (isOpt==11){
    //total_sf = 1.21/0.720968;
    total_sf = 1./0.720968;
    cemc_sf = 1.;
    ihcal_sf = 1.;
    ohcal_sf = 1.;
  } else if (isOpt==20){
    total_sf = 1./1.00729;
    cemc_sf = 1.50145;
    ihcal_sf = 1.;
    ohcal_sf = 1.80684;
  } else if (isOpt==21){
    total_sf = 1./1.00729;
    cemc_sf = 1.50145;
    ihcal_sf = 1.;
    ohcal_sf = 1.80684;
  }

  cemc_sf = cemc_sf*total_sf;
  if (ihcalType.compare("SS310") == 0) { ihcal_sf = ihcal_sf*total_sf; }
  ohcal_sf = ohcal_sf*total_sf;
    
  TH1D* h_JES_02 = new TH1D("h_JES_02",";Jet E_{Reco} / Jet E_{Truth};Counts",nbin,binmin,binmax);
  TH1D* h_JER_02 = new TH1D("h_JER_02",";(Jet E_{Reco}-Jet E_{Truth}) / Jet E_{Reco};Counts",100,-1,1);
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
  
//  h_JES_02->Scale(1./h_JES_02->Integral());
//  h_JER_02->Scale(1./h_JER_02->Integral());
  
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
  //// compare
  /////////////////////////////////////////////////////////

  TCanvas* c3 = new TCanvas("c3","",500,500);
  gPad->SetLogy(1);
  c3->cd();
  //SetHistStyle(h_JES_01,1,0);
  //SetHistStyle(h_JES_02,2,0);
  SetLineHistStyle(h_JES_01,1);
  SetLineHistStyle(h_JES_02,2);
  
  double tmpmax = h_JES_01->GetMaximum();
  h_JES_01->SetMinimum(0.5);
  //h_JES_01->SetMaximum(50000);
  h_JES_01->SetMaximum(tmpmax*20);
  h_JES_01->Draw("hist");
  h_JES_02->Draw("hist same");
  tf_JES_01->SetLineColor(kPink+3);
  tf_JES_01->SetLineWidth(5);
  tf_JES_01->Draw("same");
  tf_JES_02->SetLineColor(kAzure+3);
  tf_JES_02->SetLineWidth(5);
  tf_JES_02->Draw("same");
  //myLatex(0.7,0.85,"Inner HCal",12,0.05,kPink+3);
  //myLatex(0.7,0.75,"Al frame",12,0.05,kAzure+3);
  //TLegend* leg1 = new TLegend(0.18, 0.19, 0.40, 0.28);
  TLegend* leg1 = new TLegend(0.67, 0.60, 0.93, 0.74);
  leg1->SetTextSize(0.038);
  leg1->AddEntry(h_JES_01,"Inner HCal","lf");
  leg1->AddEntry(h_JES_02,"Al frame","lf");
  leg1->Draw();
  
  latex->SetTextAlign(12);
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"MIE 2018");
  latex->SetTextSize(0.040);
  double tmpE = atof(jetE.c_str());
  latex->DrawLatex(0.2,0.82,Form("PYTHIA8 dijet, R=0.4, %.0f <E_{Jet}^{Truth} < %.0f GeV",tmpE,tmpE+10));
  if (isHighz) myLatex(0.2,0.76,"Leading z > 0.8",12,0.040,kRed);
  //latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, R=0.4");
  //latex->DrawLatex(0.2,0.76,Form("%.0f <E_{Jet}^{Truth} < %.0f GeV",tmpE,tmpE+10));

  ////////////////////////////////// check low-side tails
  
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
  double integ_tot_02 = 277+h_JES_02->Integral(0,nbin);
  double lowbin_02 = h_JES_02->GetXaxis()->FindBin(mean_jes_02-2*sig_jes_02);
  double highbin_02 = h_JES_02->GetXaxis()->FindBin(mean_jes_02+2*sig_jes_02);
  double integ_low_02 = 277+h_JES_02->Integral(0,lowbin_02);
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
 
//  if (isHighz) {
//    c3->SaveAs(Form("out_figs/hcal_punch_highz_isOpt%d.pdf",isOpt));
//  }else {
//    c3->SaveAs(Form("out_figs/hcal_punch_isOpt%d.pdf",isOpt));
//  }
  
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
