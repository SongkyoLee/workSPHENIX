#include "TMinuit.h"

void recursiveFit(TH1D* t_h1, TF1* t_tf1, float t_fitmin, float t_fitmax, float* t_mean, float* t_sig, float* t_sigErr);

void draw_crosssection(string sampleType = "GammaJet",
                              string ihcalType = "SS310",
                              string jetE = "20GeV",
                              string version = "nover",
                              int initfile=0,
                              int endfile =1000,
                              bool doetopcut=true,
                              bool dodphicut=true)
{
  //// https://root.cern.ch/doc/master/classTColor.html 
  gStyle->SetPalette(52); //black
 
  string fname;
  if (version.compare("nover")==0) fname = Form("./2ndSortedRootFiles/2ndSorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_doetopcut%d_dodphicut%d.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),(int)doetopcut,(int)dodphicut);
  else fname = Form("./2ndSortedRootFiles/2ndSorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_doetopcut%d_dodphicut%d_%s.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),(int)doetopcut,(int)dodphicut,version.c_str());
  TFile* fin = new TFile( fname.c_str(), "READ");
  TTree* out_tree = (TTree*)fin->Get("out_tree");
  
  int nbin, binmin, binmax;
  if (jetE.compare("allGeV")==0) { nbin=50; binmin=0; binmax = 50;}
  else {nbin=60; binmin=10; binmax=70;}
  
  TH1D* h_trueg_yield = new TH1D("h_trueg_yield",";E_{True}^{#gamma} [GeV];Counts",nbin,binmin,binmax);
  TH1D* h_trueg_cross = new TH1D("h_trueg_cross",";p_{T}^{Truth #gamma} [GeV];#frac{1}{2#pi p_{T}} #frac{d^{2}#sigma}{dp_{T}dy} [pb GeV^{-2}]",nbin,binmin,binmax);
  TH1D* h_gamma_yield = new TH1D("h_gamma_yield",";E_{Reco}^{#gamma} [GeV];Counts",nbin,binmin,binmax);
  TH1D* h_gamma_cross = new TH1D("h_gamma_cross",";p_{T}^{Reco #gamma} [GeV];#frac{1}{2#pi p_{T}} #frac{d^{2}#sigma}{dp_{T}dy} [pb GeV^{-2}]",nbin,binmin,binmax);
  TH1D* h_gamma_RtoT = new TH1D("h_gamma_RtoT",";E_{Reco}^{#gamma}/E_{Truth}^{#gamma};Counts",50,0.0,2.0);
  TH1D* h_jet_RtoT = new TH1D("h_jet_RtoT",";E_{Reco}^{Jet}/E_{Truth}^{Jet};Counts",50,0.0,2.0);
  TH1D* h_jet_JtoG = new TH1D("h_jet_JtoG",";E_{Reco}^{Jet}/E_{Reco}^{#gamma};Counts",50,0.0,2.0);
  TH1D* h_trueE_JtoG = new TH1D("h_trueE_JtoG",";E_{Truth}^{Jet}/E_{Truth}^{#gamma};Counts",50,0.0,2.0);
  h_trueg_yield->Sumw2();
  h_trueg_cross->Sumw2();
  h_gamma_yield->Sumw2();
  h_gamma_cross->Sumw2();
  h_gamma_RtoT->Sumw2();
  h_jet_RtoT->Sumw2();
  h_jet_JtoG->Sumw2();
  h_trueE_JtoG->Sumw2();
  
  int evt;
  float true_gamma_e;
  float true_gamma_pt;
  float true_jet_e;
  float true_jet_pt;
  float true_jet_emfrac;

  float reco_gamma_e;
  float reco_gamma_pt;
  float reco_gamma_cemcEsum;
  float reco_jet_e;
  float reco_jet_pt;
  float reco_jet_clcemc_hadEsum;
  float reco_jet_clcemc_emEsum;
  float reco_jet_ihcalEsum;
  float reco_jet_ohcalEsum;
  
  out_tree->SetBranchAddress("evt",&evt);
  out_tree->SetBranchAddress("true_gamma_e",&true_gamma_e);
  out_tree->SetBranchAddress("true_gamma_pt",&true_gamma_pt);
  out_tree->SetBranchAddress("true_jet_e",&true_jet_e);
  out_tree->SetBranchAddress("true_jet_pt",&true_jet_pt);
  out_tree->SetBranchAddress("true_jet_emfrac",&true_jet_emfrac);
  
  out_tree->SetBranchAddress("reco_gamma_e",&reco_gamma_e);
  out_tree->SetBranchAddress("reco_gamma_pt",&reco_gamma_pt);
  out_tree->SetBranchAddress("reco_gamma_cemcEsum",&reco_gamma_cemcEsum);
  out_tree->SetBranchAddress("reco_jet_e",&reco_jet_e);
  out_tree->SetBranchAddress("reco_jet_pt",&reco_jet_pt);
  out_tree->SetBranchAddress("reco_jet_clcemc_hadEsum",&reco_jet_clcemc_hadEsum);
  out_tree->SetBranchAddress("reco_jet_clcemc_emEsum",&reco_jet_clcemc_emEsum);
  out_tree->SetBranchAddress("reco_jet_ihcalEsum",&reco_jet_ihcalEsum);
  out_tree->SetBranchAddress("reco_jet_ohcalEsum",&reco_jet_ohcalEsum);
  
  int nevt = out_tree->GetEntries();
  cout << "nevt = " << nevt << endl;

  for (int ievt=0; ievt <nevt; ievt++){
    out_tree->GetEvent(ievt);
    
    h_trueg_yield->Fill(true_gamma_e);
    //h_trueg_cross->Fill(true_gamma_e,1./true_gamma_pt);
    h_trueg_cross->Fill(true_gamma_pt,1./true_gamma_pt);
    h_gamma_yield->Fill(reco_gamma_cemcEsum);
    //h_gamma_cross->Fill(reco_gamma_cemcEsum,1./reco_gamma_pt);
    h_gamma_cross->Fill(reco_gamma_pt,1./reco_gamma_pt); //N.B. pT <- CEMC+IHCAL+OHCAL
    h_gamma_RtoT->Fill(reco_gamma_cemcEsum/true_gamma_e);
    h_jet_RtoT->Fill(reco_jet_e/true_jet_e);
    h_jet_JtoG->Fill(reco_jet_e/reco_gamma_cemcEsum);
    h_trueE_JtoG->Fill(true_jet_e/true_gamma_e);
  }

  //cross-section calculation 
  //double drap = 2*0.6; // ||y|<0.6
  double drap = 2*0.45; // ||y|<0.45
  //double lumi = 48.0519; //pb^-1 (file 0--310)
  double lumi = 46.5135; //pb^-1 (file 0--300)
  //double dpt = 1; //pT binwitdh
  double dpt = (binmax-binmin)/nbin; //pT binwitdh
  
  //h_gamma_cross=(TH1D*)h_gamma_yield->Clone("h_gamma_cross");
  h_trueg_cross->Scale(1./(2*TMath::Pi()*dpt*drap*lumi));
  h_gamma_cross->Scale(1./(2*TMath::Pi()*dpt*drap*lumi));
  
  double tmpcenter = -999;
  for (Int_t ib=0;ib<h_trueg_cross->GetSize()-2; ib++){
    tmpcenter = h_trueg_yield->GetBinCenter(ib+1);
    if (tmpcenter<20) h_trueg_cross->SetBinContent(ib+1,0);
  }  
  for (Int_t ib=0;ib<h_gamma_cross->GetSize()-2; ib++){
    tmpcenter = h_gamma_yield->GetBinCenter(ib+1);
    if (tmpcenter<20) h_gamma_cross->SetBinContent(ib+1,0);
  }  

  SetLineHistStyle(h_gamma_RtoT,1);
  SetLineHistStyle(h_jet_RtoT,2);
  SetLineHistStyle(h_jet_JtoG,3);
  SetHistStyle(h_trueg_cross,3,0);
  SetHistStyle(h_gamma_cross,4,0);
  SetLineHistStyle(h_trueE_JtoG,4);
  h_trueg_cross->SetMarkerSize(1);
  h_gamma_cross->SetMarkerSize(1);
  
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

  TCanvas* c1 = new TCanvas("c1","",1000,500);
  c1->Divide(2,1);
  
  c1->cd(1);
  gPad->SetLogy(0);
  h_gamma_RtoT->GetXaxis()->SetTitle("E_{Reco}/E_{Truth}");
  h_gamma_RtoT->Draw("hist e");
  h_jet_RtoT->Draw("hist e same");
  
  TF1* tf_gamma_RtoT= new TF1("TF_gamma_RtoT","gaus",fitmin,fitmax);
  //col = h_gamma_RtoT->GetMarkerColor();
  //tf_gamma_RtoT->SetLineColor(col);
  tf_gamma_RtoT->SetLineColor(kPink+2);
  recursiveFit(h_gamma_RtoT, tf_gamma_RtoT,fitmin, fitmax, &mean, &sig, &sigErr);
  for (int ir=0; ir<repeat; ir++){
    fitmin = mean-2*sig;
    fitmax = mean+2*sig;
    recursiveFit(h_gamma_RtoT, tf_gamma_RtoT,fitmin, fitmax, &mean, &sig, &sigErr);
  }
  tf_gamma_RtoT->Draw("same");
  sz_mean = Form ("#mu = %.2f", mean);
  myLatex(0.65,0.88,"E_{Reco}^{#gamma} / E_{Truth}^{#gamma}",12,0.045,kPink+3);
  myLatex(0.65,0.82,sz_mean.c_str(),12,0.045,kPink+3);
  
  TF1* tf_jet_RtoT= new TF1("TF_jet_RtoT","gaus",fitmin,fitmax);
  //col = h_jet_RtoT->GetMarkerColor();
  //tf_jet_RtoT->SetLineColor(col);
  tf_jet_RtoT->SetLineColor(kAzure+5);
  recursiveFit(h_jet_RtoT, tf_jet_RtoT,fitmin, fitmax, &mean, &sig, &sigErr);
  for (int ir=0; ir<repeat; ir++){
    fitmin = mean-2*sig;
    fitmax = mean+2*sig;
    recursiveFit(h_jet_RtoT, tf_jet_RtoT,fitmin, fitmax, &mean, &sig, &sigErr);
  }
  tf_jet_RtoT->Draw("same");
  sz_mean = Form ("#mu = %.2f", mean);
  myLatex(0.65,0.74,"E_{Reco}^{Jet} / E_{Truth}^{Jet}",12,0.045,kAzure+4);
  myLatex(0.65,0.68,sz_mean.c_str(),12,0.045,kAzure+4);
  
  c1->cd(2);
  gPad->SetLogy(0);
  h_jet_RtoT->GetXaxis()->SetTitle("E_{Reco}^{Jet}/E_{Reference}");
  h_jet_RtoT->Draw("hist e");
  h_jet_JtoG->Draw("hist e same");
  
  recursiveFit(h_jet_RtoT, tf_jet_RtoT,fitmin, fitmax, &mean, &sig, &sigErr);
  for (int ir=0; ir<repeat; ir++){
    fitmin = mean-2*sig;
    fitmax = mean+2*sig;
    recursiveFit(h_jet_RtoT, tf_jet_RtoT,fitmin, fitmax, &mean, &sig, &sigErr);
  }
  tf_jet_RtoT->Draw("same");
  sz_mean = Form ("#mu = %.2f", mean);
  myLatex(0.65,0.88,"E_{Reco}^{Jet} / E_{Truth}^{Jet}",12,0.045,kAzure+4);
  myLatex(0.65,0.82,sz_mean.c_str(),12,0.045,kAzure+4);
  float muRtoT = mean;
  cout  << "E_{Reco}^{Jet} / E_{Truth}^{Jet} = " << muRtoT << endl;

  TF1* tf_jet_JtoG= new TF1("TF_jet_JtoG","gaus",fitmin,fitmax);
  //col = h_jet_JtoG->GetMarkerColor();
  //tf_jet_JtoG->SetLineColor(col);
  tf_jet_JtoG->SetLineColor(kSpring+2);
  recursiveFit(h_jet_JtoG, tf_jet_JtoG,fitmin, fitmax, &mean, &sig, &sigErr);
  for (int ir=0; ir<repeat; ir++){
    fitmin = mean-2*sig;
    fitmax = mean+2*sig;
    recursiveFit(h_jet_JtoG, tf_jet_JtoG,fitmin, fitmax, &mean, &sig, &sigErr);
  }
  tf_jet_JtoG->Draw("same");
  sz_mean = Form ("#mu = %.2f", mean);
  myLatex(0.65,0.74,"E_{Reco}^{Jet} / E_{Reco}^{#gamma}",12,0.045,kSpring+3);
  myLatex(0.65,0.68,sz_mean.c_str(),12,0.045,kSpring+3);
  double muJtoG = mean;
  cout  << "E_{Reco}^{Jet} / E_{Reco}^{#gamma} = " << muJtoG << endl;
  cout  << "Ratio of mean = " << muRtoT/muJtoG << endl;
 
  c1->SaveAs(Form("out_cross/JES_%s_%s.pdf",jetE.c_str(),version.c_str())); 
  

  //////////////////////////////////////////////////////

  string sz_integ;

  TCanvas* c2 = new TCanvas("c2","",1200,800);
  c2->Divide(2,2);
  
  c2->cd(1);
  gPad->SetLogy(0);
  h_trueg_yield->GetXaxis()->CenterTitle(1);
  h_trueg_yield->GetYaxis()->CenterTitle(1);
  h_trueg_yield->Draw("hist e");
  cout << "h_trueg_yield->Integral() = " << h_trueg_yield->Integral() << endl;
  sz_integ = Form("Entries: %.0f",h_trueg_yield->Integral());
  myLatex(0.7,0.8,sz_integ.c_str(),12,0.045);
  
  c2->cd(2);
  //gPad->SetLogy(0);
  gPad->SetLogy(1);
  cout << gPad->GetLeftMargin() << endl;
  gPad->SetLeftMargin(0.2);
  if (jetE.compare("allGeV")==0) h_trueg_cross->GetXaxis()->SetRangeUser(10,40);
  //h_trueg_cross->SetMinimum(10e-5);
  //h_trueg_cross->SetMaximum(10e1);
  h_trueg_cross->GetYaxis()->SetRangeUser(10e-5,10e1);
  cout << h_trueg_cross->GetYaxis()->GetTitleOffset() << endl;
  h_trueg_cross->GetYaxis()->SetTitleOffset(1.8); 
  h_trueg_cross->Draw("pe");
  
  c2->cd(3);
  gPad->SetLogy(0);
  h_gamma_yield->GetXaxis()->CenterTitle(1);
  h_gamma_yield->GetYaxis()->CenterTitle(1);
  h_gamma_yield->Draw("hist e");
  cout << "h_gamma_yield->Integral() = " << h_gamma_yield->Integral() << endl;
  sz_integ = Form("Entries: %.0f",h_gamma_yield->Integral());
  myLatex(0.7,0.8,sz_integ.c_str(),12,0.045);
  
  c2->cd(4);
  //gPad->SetLogy(0);
  gPad->SetLogy(1);
  gPad->SetLeftMargin(0.2);
  if (jetE.compare("allGeV")==0) h_gamma_cross->GetXaxis()->SetRangeUser(10,40);
  h_gamma_cross->SetMinimum(10e-5);
  h_gamma_cross->SetMaximum(10e1);
  //h_gamma_cross->GetYaxis()->SetRangeUser(10e-4,10e2);
  h_gamma_cross->GetYaxis()->SetTitleOffset(1.8); 
  h_gamma_cross->Draw("pe");

  c2->SaveAs(Form("out_cross/cross_%s_%s.pdf",jetE.c_str(),version.c_str())); 

  TCanvas* c3 = new TCanvas("c3","",500,500);
  h_trueE_JtoG->GetXaxis()->SetTitle("E_{Truth}^{Jet}/E_{Truth}^{#gamma}");
  h_trueE_JtoG->Draw("hist e");
  
  fitmin = 0.0;
  fitmax = 2.0;
  TF1* tf_trueE_JtoG= new TF1("TF_gamma_RtoT","gaus",fitmin,fitmax);
  //col = h_trueE_JtoG->GetMarkerColor();
  //tf_trueE_JtoG->SetLineColor(col);
  tf_trueE_JtoG->SetLineColor(kOrange+2);
  recursiveFit(h_trueE_JtoG, tf_trueE_JtoG,fitmin, fitmax, &mean, &sig, &sigErr);
  for (int ir=0; ir<repeat; ir++){
    fitmin = mean-2*sig;
    fitmax = mean+2*sig;
    recursiveFit(h_trueE_JtoG, tf_trueE_JtoG,fitmin, fitmax, &mean, &sig, &sigErr);
  }
  tf_trueE_JtoG->Draw("same");
  sz_mean = Form ("#mu = %.2f", mean);
  myLatex(0.65,0.88,"E_{Truth}^{Jet} / E_{Truth}^{#gamma}",12,0.045,kOrange+2);
  myLatex(0.65,0.82,sz_mean.c_str(),12,0.045,kOrange+2);
  double muTrueE = mean;
  
  c3->SaveAs(Form("out_cross/truth_%s_%s.pdf",jetE.c_str(),version.c_str())); 
  cout  << "E_{Truth}^{Jet} / E_{Truth}^{Jet} ratio_of_truth = " << muTrueE << endl;
  
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
