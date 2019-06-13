#include "TMinuit.h"

void recursiveFit(TH1D* t_h1, TF1* t_tf1, float t_fitmin, float t_fitmax, float* t_mean, float* t_sig, float* t_sigErr);

void jes_onefile(string sampleType = "QCDJet",
                 string ihcalType = "Al",
                 string jetE = "20GeV",
                 string vtxType = "R04",
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
  //sampleType = "QCDJet";
  //sampleType = "ObjectiveKPP";
  //sampleType = "ThresholdKPP";
  //ihcalType = "Al";
  //vtxType = "R04";

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
  //// compare JES
  /////////////////////////////////////////////////////////

  TCanvas* c3 = new TCanvas("c3","",500,500);
  c3->cd();
  SetHistStyle(h_JES_01,1,0);
  h_JES_01->SetMaximum(0.18);
  h_JES_01->GetYaxis()->SetTitleOffset(1.6);
  h_JES_01->Draw("pe");
  
  tf_JES_01->SetLineColor(kRed-7);
  tf_JES_01->SetLineStyle(7);
  tf_JES_01->SetRange(binmin,binmax);
  tf_JES_01->Draw("same");
 
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
  
  c3->SaveAs(Form("out_figs/onefile_%s_%s.pdf",jetE.c_str(),vtxType.c_str())); 
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
