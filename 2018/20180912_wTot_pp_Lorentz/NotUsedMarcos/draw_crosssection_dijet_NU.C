#include "TMinuit.h"

void recursiveFit(TH1D* t_h1, TF1* t_tf1, float t_fitmin, float t_fitmax, float* t_mean, float* t_sig, float* t_sigErr);

void draw_crosssection_dijet(string sampleType = "QCDJet",
                              string ihcalType = "SS310",
                              string jetE = "20GeV",
                              string note = "R04",
                              int initfile=0,
                              int endfile =400,
                              bool doetopcut=true,
                              bool dodphicut=true)
{
  //// https://root.cern.ch/doc/master/classTColor.html 
  //gStyle->SetPalette(52); //black
  gStyle->SetPalette(55); // rainbow
   
  string fname;
  fname = Form("./2ndSortedRootFiles/2ndSorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_%s_doetopcut%d_dodphicut%d.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),note.c_str(),(int)doetopcut,(int)dodphicut);
  TFile* fin = new TFile( fname.c_str(), "READ");
  TTree* out_tree = (TTree*)fin->Get("out_tree");
  
  int nbin, binmin, binmax;
  if (jetE.compare("allGeV")==0) { nbin=50; binmin=0; binmax = 50;}
  else {nbin=60; binmin=10; binmax=70;}
  
  TH1D* h_truth_yield = new TH1D("h_truth_yield",";E_{True}^{Jet} [GeV];Counts",nbin,binmin,binmax);
  TH1D* h_truth_cross = new TH1D("h_truth_cross",";p_{T,Truth}^{Jet} [GeV];#frac{1}{2#pi p_{T}} #frac{d^{2}#sigma}{dp_{T}dy} [pb GeV^{-2}]",nbin,binmin,binmax);
  TH1D* h_reco_yield = new TH1D("h_reco_yield",";E_{Reco}^{Jet} [GeV];Counts",nbin,binmin,binmax);
  TH1D* h_reco_cross = new TH1D("h_reco_cross",";p_{T,Reco}^{Jet} [GeV];#frac{1}{2#pi p_{T}} #frac{d^{2}#sigma}{dp_{T}dy} [pb GeV^{-2}]",nbin,binmin,binmax);
  h_truth_yield->Sumw2();
  h_truth_cross->Sumw2();
  h_reco_yield->Sumw2();
  h_reco_cross->Sumw2();
  
  int evt;
  float true_jet_e;
  float true_jet_pt;

  float reco_jet_e;
  float reco_jet_pt;
  float reco_jet_clcemc_hadEsum;
  float reco_jet_clcemc_emEsum;
  float reco_jet_cemcEsum;
  float reco_jet_ihcalEsum;
  float reco_jet_ohcalEsum;
  
  out_tree->SetBranchAddress("evt",&evt);
  out_tree->SetBranchAddress("true_jet_e",&true_jet_e);
  out_tree->SetBranchAddress("true_jet_pt",&true_jet_pt);
  
  out_tree->SetBranchAddress("reco_jet_e",&reco_jet_e);
  out_tree->SetBranchAddress("reco_jet_pt",&reco_jet_pt);
  out_tree->SetBranchAddress("reco_jet_clcemc_hadEsum",&reco_jet_clcemc_hadEsum);
  out_tree->SetBranchAddress("reco_jet_clcemc_emEsum",&reco_jet_clcemc_emEsum);
  out_tree->SetBranchAddress("reco_jet_cemcEsum",&reco_jet_cemcEsum);
  out_tree->SetBranchAddress("reco_jet_ihcalEsum",&reco_jet_ihcalEsum);
  out_tree->SetBranchAddress("reco_jet_ohcalEsum",&reco_jet_ohcalEsum);
 
  int nevt = out_tree->GetEntries();
  cout << "nevt = " << nevt << endl;

  for (int ievt=0; ievt <nevt; ievt++){
    out_tree->GetEvent(ievt);
    h_truth_yield->Fill(true_jet_e);
    h_truth_cross->Fill(true_jet_pt,1./true_jet_pt);
    h_reco_yield->Fill(reco_jet_e);
    h_reco_cross->Fill(reco_jet_pt,1./reco_jet_pt); //N.B. pT <- CEMC+IHCAL+OHCAL
  }

  //cross-section calculation 
  //double drap = 2*0.6; // ||y|<0.6
  double drap = 2*0.45; // ||y|<0.45
  //double lumi = 48.0519; //pb^-1 (file 0--310)
  double lumi = 46.5135; //pb^-1 (file 0--300) <- should be calculated as rcas!!
  //double dpt = 1; //pT binwitdh
  double dpt = (binmax-binmin)/nbin; //pT binwitdh
  
  h_truth_cross->Scale(1./(2*TMath::Pi()*dpt*drap*lumi));
  h_reco_cross->Scale(1./(2*TMath::Pi()*dpt*drap*lumi));
  
  double tmpcenter = -999;
  for (Int_t ib=0;ib<h_truth_cross->GetSize()-2; ib++){
    tmpcenter = h_truth_yield->GetBinCenter(ib+1);
    if (tmpcenter<20) h_truth_cross->SetBinContent(ib+1,0);
  }  
  for (Int_t ib=0;ib<h_reco_cross->GetSize()-2; ib++){
    tmpcenter = h_reco_yield->GetBinCenter(ib+1);
    if (tmpcenter<20) h_reco_cross->SetBinContent(ib+1,0);
  }  

  SetHistStyle(h_truth_cross,3,0);
  SetHistStyle(h_reco_cross,4,0);
  h_truth_cross->SetMarkerSize(1);
  h_reco_cross->SetMarkerSize(1);
  
  float col;
  int repeat=30;
  float fitmin = 0.0;
  float fitmax = 1.5;
  float mean;
  float sig;
  float sigErr;
  string sz_mean;

  //////////////////////////////////////////////////////
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(32);
  //////////////////////////////////////////////////////

  string sz_integ;

  TCanvas* c_cross = new TCanvas("c_cross","",1200,800);
  c_cross->Divide(2,2);
  
  c_cross->cd(1);
  gPad->SetLogy(0);
  h_truth_yield->GetXaxis()->CenterTitle(1);
  h_truth_yield->GetYaxis()->CenterTitle(1);
  h_truth_yield->Draw("hist e");
  cout << "h_truth_yield->Integral() = " << h_truth_yield->Integral() << endl;
  sz_integ = Form("Entries: %.0f",h_truth_yield->Integral());
  myLatex(0.7,0.8,sz_integ.c_str(),12,0.045);
  
  c_cross->cd(2);
  //gPad->SetLogy(0);
  gPad->SetLogy(1);
  cout << gPad->GetLeftMargin() << endl;
  gPad->SetLeftMargin(0.2);
  if (jetE.compare("allGeV")==0) h_truth_cross->GetXaxis()->SetRangeUser(10,40);
  //h_truth_cross->SetMinimum(10e-5);
  //h_truth_cross->SetMaximum(10e1);
  h_truth_cross->GetYaxis()->SetRangeUser(10e-5,10e1);
  cout << h_truth_cross->GetYaxis()->GetTitleOffset() << endl;
  h_truth_cross->GetYaxis()->SetTitleOffset(1.8); 
  //h_truth_cross->Draw("pe");
  h_truth_cross->Draw("hist e");
  
  c_cross->cd(3);
  gPad->SetLogy(0);
  h_reco_yield->GetXaxis()->CenterTitle(1);
  h_reco_yield->GetYaxis()->CenterTitle(1);
  h_reco_yield->Draw("hist e");
  cout << "h_reco_yield->Integral() = " << h_reco_yield->Integral() << endl;
  sz_integ = Form("Entries: %.0f",h_reco_yield->Integral());
  myLatex(0.7,0.8,sz_integ.c_str(),12,0.045);
  
  c_cross->cd(4);
  //gPad->SetLogy(0);
  gPad->SetLogy(1);
  gPad->SetLeftMargin(0.2);
  if (jetE.compare("allGeV")==0) h_reco_cross->GetXaxis()->SetRangeUser(10,40);
  h_reco_cross->SetMinimum(10e-5);
  h_reco_cross->SetMaximum(10e1);
  //h_reco_cross->GetYaxis()->SetRangeUser(10e-4,10e2);
  h_reco_cross->GetYaxis()->SetTitleOffset(1.8); 
  //h_reco_cross->Draw("pe");
  h_reco_cross->Draw("hist e");

  c_cross->SaveAs(Form("out_cross/cross_%s_%s.pdf",jetE.c_str(),note.c_str())); 
  
  return;
}
////////////////////////////////////////

void recursiveFit(TH1D* t_h1, TF1* t_tf1, float t_fitmin, float t_fitmax, float* t_mean, float* t_sig, float* t_sigErr){
  string tfname = t_tf1->GetName();
  //t_h1[ih]->Fit(Form("tf1_%d",ih),"NR"); 
  //t_h1->Fit(tfname.c_str(),"NR"); 
  t_tf1->SetRange(t_fitmin,t_fitmax);
  //t_h1->Fit(tfname.c_str(),"NR","",t_fitmin,t_fitmax); 
  t_h1->Fit(tfname.c_str(),"NRQ","",t_fitmin,t_fitmax); //Quiet mode 
  //sig[ih] = tf1[ih]->GetParameter(2);
  //sigErr[ih] = tf1[ih]->GetParError(2);
  *t_mean = t_tf1->GetParameter(1);
  *t_sig = t_tf1->GetParameter(2);
  *t_sigErr =  t_tf1->GetParError(2);

  return;
}
