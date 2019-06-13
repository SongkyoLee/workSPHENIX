#include "TMinuit.h"

void recursiveFit(TH1D* t_h1, TF1* t_tf1, float t_fitmin, float t_fitmax, float* t_mean, float* t_sig, float* t_sigErr);

void jes_threefile(string sampleType = "QCDJet",
                 string ihcalType = "Al",
                 string jetE = "30GeV",
                 string vtxType = "R02",
                 int initfile=0,
                 int endfile =1000,
                 bool doetopcut=true,
                 bool dodphicut=true)
{
  int nbin = 50;
  double binmin = 0.; 
  double binmax = 1.5; 
  ////////////////////////////////////////
  int repeat=30;
  float fitmin_jes = 0.5;
  float fitmax_jes = 1.5;
  
  /////////////////////////////////////////////////////////
  //// check 01
  /////////////////////////////////////////////////////////
  sampleType = "QCDJet";
  //sampleType = "ObjectiveKPP";
  //sampleType = "ThresholdKPP";
  ihcalType = "Al";

  string fname01 = Form("./2ndSortedRootFiles/2ndSorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_%s_doetopcut%d_dodphicut%d.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),vtxType.c_str(),(int)doetopcut,(int)dodphicut);
  TFile* fin01 = new TFile( fname01.c_str(), "READ");
  TTree* out_tree01 = (TTree*)fin01->Get("out_tree");

  TH1D* h_JES_01 = new TH1D("h_JES_01",";E_{T, Jet}^{Reco} / E_{T, Jet}^{Truth};Normalized counts",nbin,binmin,binmax);
  h_JES_01->Sumw2();
  
  int evt_01;
  float true_jet_e_01;
  float true_jet_pt_01;
  float true_jet_eta_01;
  float reco_jet_e_01;
  float reco_jet_pt_01;
  float reco_jet_eta_01;
  float reco_jet_cemcEsum_01;
  float reco_jet_ihcalEsum_01;
  float reco_jet_ohcalEsum_01;
  out_tree01->SetBranchAddress("evt",&evt_01);
  out_tree01->SetBranchAddress("true_jet_e",&true_jet_e_01);
  out_tree01->SetBranchAddress("true_jet_pt",&true_jet_pt_01);
  out_tree01->SetBranchAddress("true_jet_eta",&true_jet_eta_01);
  out_tree01->SetBranchAddress("true_jet_e",&true_jet_e_01);
  out_tree01->SetBranchAddress("reco_jet_pt",&reco_jet_pt_01);
  out_tree01->SetBranchAddress("reco_jet_eta",&reco_jet_eta_01);
  out_tree01->SetBranchAddress("reco_jet_cemcEsum",&reco_jet_cemcEsum_01);
  out_tree01->SetBranchAddress("reco_jet_ihcalEsum",&reco_jet_ihcalEsum_01);
  out_tree01->SetBranchAddress("reco_jet_ohcalEsum",&reco_jet_ohcalEsum_01);

  int nevt_01 = out_tree01->GetEntries();
  cout << "nevt_01 = " << nevt_01 << endl;

  for (int ievt=0; ievt <nevt_01; ievt++){
    out_tree01->GetEvent(ievt);
    //h_JES_01->Fill( reco_jet_pt_01 / true_jet_pt_01 ); //instrumente pt
    //h_JES_01->Fill( (reco_jet_cemcEsum_01+reco_jet_ohcalEsum_01) / true_jet_e_01 ); //uninstrumente E
    h_JES_01->Fill( (reco_jet_cemcEsum_01+reco_jet_ohcalEsum_01)/(cosh(reco_jet_eta_01)) / true_jet_e_01/(cosh(true_jet_eta_01)) ); //uninstrumented E_T
  }
  h_JES_01->Scale(1./h_JES_01->Integral());
 
  ////////////////////////////////////////
  float mean_jes_01;
  float sig_jes_01;
  float sigErr_jes_01;

  TF1* tf_JES_01= new TF1("TF_JES","gaus",fitmin_jes,fitmax_jes);
  recursiveFit(h_JES_01, tf_JES_01,fitmin_jes, fitmax_jes, &mean_jes_01, &sig_jes_01, &sigErr_jes_01);
  for (int ir=0; ir<repeat; ir++){
    fitmin_jes = mean_jes_01-2*sig_jes_01;
    fitmax_jes = mean_jes_01+2*sig_jes_01;
    recursiveFit(h_JES_01, tf_JES_01,fitmin_jes, fitmax_jes, &mean_jes_01, &sig_jes_01, &sigErr_jes_01);
  }

  cout  << "" << endl;
  cout  << sampleType.c_str()<<" "<<ihcalType.c_str() << " "<< jetE.c_str() << " "<<vtxType.c_str() << endl;
  cout << "jes_mean = " << mean_jes_01 << endl;
  cout << "jes_sig = " << sig_jes_01 << endl;
  cout << "jes_sig/mean = " << sig_jes_01/mean_jes_01 << endl;
  
  /////////////////////////////////////////////////////////
  //// check 02
  /////////////////////////////////////////////////////////
  //sampleType = "QCDJet";
  sampleType = "ObjectiveKPP";
  //sampleType = "ThresholdKPP";
  ihcalType = "Al";

  string fname02 = Form("./2ndSortedRootFiles/2ndSorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_%s_doetopcut%d_dodphicut%d.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),vtxType.c_str(),(int)doetopcut,(int)dodphicut);
  TFile* fin02 = new TFile( fname02.c_str(), "READ");
  TTree* out_tree02 = (TTree*)fin02->Get("out_tree");

  TH1D* h_JES_02 = new TH1D("h_JES_02",";E_{T, Jet}^{Reco} / E_{T, Jet}^{Truth};Normalized counts",nbin,binmin,binmax);
  h_JES_02->Sumw2();
  
  int evt_02;
  float true_jet_e_02;
  float true_jet_pt_02;
  float true_jet_eta_02;
  float reco_jet_e_02;
  float reco_jet_pt_02;
  float reco_jet_eta_02;
  float reco_jet_cemcEsum_02;
  float reco_jet_ihcalEsum_02;
  float reco_jet_ohcalEsum_02;
  out_tree02->SetBranchAddress("evt",&evt_02);
  out_tree02->SetBranchAddress("true_jet_e",&true_jet_e_02);
  out_tree02->SetBranchAddress("true_jet_pt",&true_jet_pt_02);
  out_tree02->SetBranchAddress("true_jet_eta",&true_jet_eta_02);
  out_tree02->SetBranchAddress("true_jet_e",&true_jet_e_02);
  out_tree02->SetBranchAddress("reco_jet_pt",&reco_jet_pt_02);
  out_tree02->SetBranchAddress("reco_jet_eta",&reco_jet_eta_02);
  out_tree02->SetBranchAddress("reco_jet_cemcEsum",&reco_jet_cemcEsum_02);
  out_tree02->SetBranchAddress("reco_jet_ihcalEsum",&reco_jet_ihcalEsum_02);
  out_tree02->SetBranchAddress("reco_jet_ohcalEsum",&reco_jet_ohcalEsum_02);

  int nevt_02 = out_tree02->GetEntries();
  cout << "nevt_02 = " << nevt_02 << endl;

  for (int ievt=0; ievt <nevt_02; ievt++){
    out_tree02->GetEvent(ievt);
    //h_JES_02->Fill( reco_jet_pt_02 / true_jet_pt_02 ); //instrumente pt
    //h_JES_02->Fill( (reco_jet_cemcEsum_02+reco_jet_ohcalEsum_02) / true_jet_e_02 ); //uninstrumente E
    h_JES_02->Fill( (reco_jet_cemcEsum_02+reco_jet_ohcalEsum_02)/(cosh(reco_jet_eta_02)) / true_jet_e_02/(cosh(true_jet_eta_02)) ); //uninstrumented E_T
  }
  h_JES_02->Scale(1./h_JES_02->Integral());
 
  ////////////////////////////////////////
  float mean_jes_02;
  float sig_jes_02;
  float sigErr_jes_02;

  TF1* tf_JES_02= new TF1("TF_JES","gaus",fitmin_jes,fitmax_jes);
  recursiveFit(h_JES_02, tf_JES_02,fitmin_jes, fitmax_jes, &mean_jes_02, &sig_jes_02, &sigErr_jes_02);
  for (int ir=0; ir<repeat; ir++){
    fitmin_jes = mean_jes_02-2*sig_jes_02;
    fitmax_jes = mean_jes_02+2*sig_jes_02;
    recursiveFit(h_JES_02, tf_JES_02,fitmin_jes, fitmax_jes, &mean_jes_02, &sig_jes_02, &sigErr_jes_02);
  }

  cout  << "" << endl;
  cout  << sampleType.c_str()<<" "<<ihcalType.c_str() << " "<< jetE.c_str() << " "<<vtxType.c_str() << endl;
  cout << "jes_mean = " << mean_jes_02 << endl;
  cout << "jes_sig = " << sig_jes_02 << endl;
  cout << "jes_sig/mean = " << sig_jes_02/mean_jes_02 << endl;
  
  /////////////////////////////////////////////////////////
  //// check 03
  /////////////////////////////////////////////////////////
  //sampleType = "QCDJet";
  //sampleType = "ObjectiveKPP";
  sampleType = "ThresholdKPP";
  ihcalType = "Al";

  string fname03 = Form("./2ndSortedRootFiles/2ndSorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_%s_doetopcut%d_dodphicut%d.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),vtxType.c_str(),(int)doetopcut,(int)dodphicut);
  TFile* fin03 = new TFile( fname03.c_str(), "READ");
  TTree* out_tree03 = (TTree*)fin03->Get("out_tree");

  TH1D* h_JES_03 = new TH1D("h_JES_03",";E_{T, Jet}^{Reco} / E_{T, Jet}^{Truth};Normalized counts",nbin,binmin,binmax);
  h_JES_03->Sumw2();
  
  int evt_03;
  float true_jet_e_03;
  float true_jet_pt_03;
  float true_jet_eta_03;
  float reco_jet_e_03;
  float reco_jet_pt_03;
  float reco_jet_eta_03;
  float reco_jet_cemcEsum_03;
  float reco_jet_ihcalEsum_03;
  float reco_jet_ohcalEsum_03;
  out_tree03->SetBranchAddress("evt",&evt_03);
  out_tree03->SetBranchAddress("true_jet_e",&true_jet_e_03);
  out_tree03->SetBranchAddress("true_jet_pt",&true_jet_pt_03);
  out_tree03->SetBranchAddress("true_jet_eta",&true_jet_eta_03);
  out_tree03->SetBranchAddress("true_jet_e",&true_jet_e_03);
  out_tree03->SetBranchAddress("reco_jet_pt",&reco_jet_pt_03);
  out_tree03->SetBranchAddress("reco_jet_eta",&reco_jet_eta_03);
  out_tree03->SetBranchAddress("reco_jet_cemcEsum",&reco_jet_cemcEsum_03);
  out_tree03->SetBranchAddress("reco_jet_ihcalEsum",&reco_jet_ihcalEsum_03);
  out_tree03->SetBranchAddress("reco_jet_ohcalEsum",&reco_jet_ohcalEsum_03);

  int nevt_03 = out_tree03->GetEntries();
  cout << "nevt_03 = " << nevt_03 << endl;

  for (int ievt=0; ievt <nevt_03; ievt++){
    out_tree03->GetEvent(ievt);
    //h_JES_03->Fill( reco_jet_pt_03 / true_jet_pt_03 ); //instrumente pt
    //h_JES_03->Fill( (reco_jet_cemcEsum_03+reco_jet_ohcalEsum_03) / true_jet_e_03 ); //uninstrumente E
    h_JES_03->Fill( (reco_jet_cemcEsum_03+reco_jet_ohcalEsum_03)/(cosh(reco_jet_eta_03)) / true_jet_e_03/(cosh(true_jet_eta_03)) ); //uninstrumented E_T
  }
  h_JES_03->Scale(1./h_JES_03->Integral());
 
  ////////////////////////////////////////
  float mean_jes_03;
  float sig_jes_03;
  float sigErr_jes_03;

  TF1* tf_JES_03= new TF1("TF_JES","gaus",fitmin_jes,fitmax_jes);
  recursiveFit(h_JES_03, tf_JES_03,fitmin_jes, fitmax_jes, &mean_jes_03, &sig_jes_03, &sigErr_jes_03);
  for (int ir=0; ir<repeat; ir++){
    fitmin_jes = mean_jes_03-2*sig_jes_03;
    fitmax_jes = mean_jes_03+2*sig_jes_03;
    recursiveFit(h_JES_03, tf_JES_03,fitmin_jes, fitmax_jes, &mean_jes_03, &sig_jes_03, &sigErr_jes_03);
  }

  cout  << "" << endl;
  cout  << sampleType.c_str()<<" "<<ihcalType.c_str() << " "<< jetE.c_str() << " "<<vtxType.c_str() << endl;
  cout << "jes_mean = " << mean_jes_03 << endl;
  cout << "jes_sig = " << sig_jes_03 << endl;
  cout << "jes_sig/mean = " << sig_jes_03/mean_jes_03 << endl;
  
  /////////////////////////////////////////////////////////
  //// compare JES
  /////////////////////////////////////////////////////////

  TCanvas* c3 = new TCanvas("c3","",500,500);
  c3->cd();
  SetHistStyle(h_JES_01,1,0);
  SetHistStyle(h_JES_02,2,11);
  SetHistStyle(h_JES_03,3,12);
  h_JES_01->SetMaximum(0.18);
  h_JES_01->GetYaxis()->SetTitleOffset(1.6);
  h_JES_01->Draw("pe");
  h_JES_02->Draw("pe same");
  h_JES_03->Draw("pe same");
  
  tf_JES_01->SetLineColor(kRed-7);
  tf_JES_01->SetLineStyle(7);
  tf_JES_01->SetRange(binmin,binmax);
  tf_JES_01->Draw("same");
  tf_JES_02->SetLineColor(kBlue-9);
  tf_JES_02->SetLineStyle(7);
  tf_JES_02->SetRange(binmin,binmax);
  tf_JES_02->Draw("same");
  tf_JES_03->SetLineColor(kGreen+2);
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
  latex->DrawLatex(0.42,0.89,"MIE Simulation");
  latex->SetTextSize(0.040);
  if (vtxType.compare("R04")==0) latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, R=0.4");
  else if (vtxType.compare("R02")==0) latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, R=0.2");
  double tmpE = atof(jetE.c_str());
  cout << "tmpE = " << tmpE << endl;
  latex->DrawLatex(0.2,0.76,Form("%.0f <p_{T, Jet}^{Truth} < %.0f GeV",tmpE,tmpE+10));
  
  TLegend* leg1 = new TLegend(0.64, 0.54, 0.93, 0.70);
  leg1->SetTextSize(0.038);
  TH1D* h_dummy = (TH1D*)h_JES_02->Clone("h_dummy");
  h_dummy->SetFillColor(kWhite);
  h_dummy->SetLineColor(kWhite);
  leg1->AddEntry(h_JES_01,"Default","lp");
  leg1->AddEntry(h_JES_02,"Objective KPP","lp");
  leg1->AddEntry(h_JES_03,"Threshold KPP","lp");
  leg1->Draw();
 
  c3->SaveAs(Form("out_figs/threefile_%s_%s.pdf",jetE.c_str(),vtxType.c_str())); 
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
