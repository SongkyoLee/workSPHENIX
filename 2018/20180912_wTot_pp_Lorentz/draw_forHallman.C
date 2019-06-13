#include "TMinuit.h"

void recursiveFit(TH1D* t_h1, TF1* t_tf1, float t_fitmin, float t_fitmax, float* t_mean, float* t_sig, float* t_sigErr);

void draw_forHallman(bool isCalib=true,
                              string sampleType = "QCDJet",
                              string jetE = "20GeV",
                              string note = "R04",
                              bool doetopcut=true,
                              bool dodphicut=true)
{
  //// https://root.cern.ch/doc/master/classTColor.html 
  //gStyle->SetPalette(1);
  //gStyle->SetPalette(kBird); //default
  //gStyle->SetPalette(51); //navy
  //gStyle->SetPalette(52); //black
  gStyle->SetPalette(55); //rainbow color
  //gStyle->SetPalette(kFuchsia); //warm violet
  //gStyle->SetPalette(kLake); //cool violet
  //TColor::InvertPalette();
  
  string fname_SS310;
  string fname_Al;
  string fname_Alframe;
  
  if (isCalib){ 
  fname_SS310 = Form("./outFinalSS310/final_%s_%s_SS310_%s_truthjet.root",sampleType.c_str(),jetE.c_str(),note.c_str());
  fname_Al = Form("./outFinalAl/final_%s_%s_Al_%s_truthjet.root",sampleType.c_str(),jetE.c_str(),note.c_str());
  fname_Alframe = Form("./outFinalAlframe/final_%s_%s_Alframe_%s_truthjet.root",sampleType.c_str(),jetE.c_str(),note.c_str());
  } else {
  fname_SS310 = Form("./outFinalSS310/final_%s_%s_SS310_%s_before.root",sampleType.c_str(),jetE.c_str(),note.c_str());
  fname_Al = Form("./outFinalAl/final_%s_%s_Al_%s_before.root",sampleType.c_str(),jetE.c_str(),note.c_str());
  fname_Alframe = Form("./outFinalAlframe/final_%s_%s_Alframe_%s_before.root",sampleType.c_str(),jetE.c_str(),note.c_str());
  }
  
  TFile* fin_SS310 = new TFile( fname_SS310.c_str(), "READ");
  if(!fin_SS310->IsOpen()) { cout <<"WARNING!! file not found"<<endl; return;};
  TH2D* h_emfrac_SS310 = (TH2D*)fin_SS310->Get("h_emfrac");
  TProfile* prof_emfrac_SS310 = (TProfile*)fin_SS310->Get("h_emfrac_pfx");
  TF1* func_emfrac_SS310 = (TF1*)fin_SS310->Get("pol1");
  TH1D* h_JES_SS310 = (TH1D*)fin_SS310->Get("h_JES");
  TH1D* h_JPTS_SS310 = (TH1D*)fin_SS310->Get("h_JPTS");
  TH1D* h_JMS_SS310 = (TH1D*)fin_SS310->Get("h_JMS");
 
  
  TFile* fin_Al = new TFile( fname_Al.c_str(), "READ");
  if(!fin_Al->IsOpen()) { cout <<"WARNING!! file not found"<<endl; return;};
  TH2D* h_emfrac_Al = (TH2D*)fin_Al->Get("h_emfrac");
  TProfile* prof_emfrac_Al = (TProfile*)fin_Al->Get("h_emfrac_pfx");
  TF1* func_emfrac_Al = (TF1*)fin_Al->Get("pol1");
  TH1D* h_JES_Al = (TH1D*)fin_Al->Get("h_JES");
  TH1D* h_JPTS_Al = (TH1D*)fin_Al->Get("h_JPTS");
  TH1D* h_JMS_Al = (TH1D*)fin_Al->Get("h_JMS");
  
  TFile* fin_Alframe = new TFile( fname_Alframe.c_str(), "READ");
  if(!fin_Alframe->IsOpen()) { cout <<"WARNING!! file not found"<<endl; return;};
  TTree* out_tree_Alframe = (TTree*)fin_Alframe->Get("out_tree");
  TH2D* h_emfrac_Alframe = (TH2D*)fin_Alframe->Get("h_emfrac");
  TProfile* prof_emfrac_Alframe = (TProfile*)fin_Alframe->Get("h_emfrac_pfx");
  TF1* func_emfrac_Alframe = (TF1*)fin_Alframe->Get("pol1");
  TH1D* h_JES_Alframe = (TH1D*)fin_Alframe->Get("h_JES");
  TH1D* h_JPTS_Alframe = (TH1D*)fin_Alframe->Get("h_JPTS");
  TH1D* h_JMS_Alframe = (TH1D*)fin_Alframe->Get("h_JMS");

/*
  TH1D* h_JES_SS310 = new TH1D("h_JES_SS310",";E_{Jet}^{Reco}/E_{Jet}^{Truth};Counts",100,0,2);
  TH1D* h_JES_Al = new TH1D("h_JES_Al",";E_{Jet}^{Reco}/E_{Jet}^{Truth};Counts",100,0,2);
  TH1D* h_JES_Alframe = new TH1D("h_JES_Alframe",";E_{Jet}^{Reco}/E_{Jet}^{Truth};Counts",100,0,2);
  
  TH1D* h_JPTS_SS310 = new TH1D("h_JPTS_SS310",";p_{T,Jet}^{Reco}/p_{T,Jet}^{Truth};Counts",100,0,2);
  TH1D* h_JPTS_Al = new TH1D("h_JPTS_Al",";p_{T,Jet}^{Reco}/p_{T,Jet}^{Truth};Counts",100,0,2);
  TH1D* h_JPTS_Alframe = new TH1D("h_JPTS_Alframe",";p_{T,Jet}^{Reco}/p_{T,Jet}^{Truth};Counts",100,0,2);
 
  TH1D* h_JMS_SS310 = new TH1D("h_JMS_SS310",";M_{Jet}^{Reco}/M_{Jet}^{Truth};Counts",100,0,2);
  TH1D* h_JMS_Al = new TH1D("h_JMS_Al",";M_{Jet}^{Reco}/M_{Jet}^{Truth};Counts",100,0,2);
  TH1D* h_JMS_Alframe = new TH1D("h_JMS_Alframe",";M_{Jet}^{Reco}/M_{Jet}^{Truth};Counts",100,0,2);
  
  ////////// fill SS310 ////////// 
  int evt_SS310;
  float true_jet_e_SS310;
  float true_jet_p_SS310;
  float true_jet_pt_SS310;
  float true_jet_emfrac_SS310;
  float reco_jet_e_SS310;
  float reco_jet_p_SS310;
  float reco_jet_pt_SS310;
  float reco_jet_cemcEsum_SS310;
  float reco_jet_ihcalEsum_SS310;
  float reco_jet_ohcalEsum_SS310;
  out_tree_SS310->SetBranchAddress("evt",&evt_SS310);
  out_tree_SS310->SetBranchAddress("true_jet_e",&true_jet_e_SS310);
  out_tree_SS310->SetBranchAddress("true_jet_p",&true_jet_p_SS310);
  out_tree_SS310->SetBranchAddress("true_jet_pt",&true_jet_pt_SS310);
  out_tree_SS310->SetBranchAddress("true_jet_emfrac",&true_jet_emfrac_SS310);
  out_tree_SS310->SetBranchAddress("reco_jet_e",&reco_jet_e_SS310);
  out_tree_SS310->SetBranchAddress("reco_jet_p",&reco_jet_p_SS310);
  out_tree_SS310->SetBranchAddress("reco_jet_pt",&reco_jet_pt_SS310);
  out_tree_SS310->SetBranchAddress("reco_jet_cemcEsum",&reco_jet_cemcEsum_SS310);
  out_tree_SS310->SetBranchAddress("reco_jet_ihcalEsum",&reco_jet_ihcalEsum_SS310);
  out_tree_SS310->SetBranchAddress("reco_jet_ohcalEsum",&reco_jet_ohcalEsum_SS310);
  
  float true_jet_m_SS310;
  float reco_jet_m_SS310;

  int nevt_SS310 = out_tree_SS310->GetEntries();
  cout << "nevt_SS310 = " << nevt_SS310 << endl;

  for (int ievt=0; ievt <nevt_SS310; ievt++){
    out_tree_SS310->GetEvent(ievt);
    h_emfrac_SS310 -> Fill(true_jet_emfrac_SS310, reco_jet_e_SS310/true_jet_e_SS310);
    //h_emfrac_SS310 -> Fill(true_jet_emfrac_SS310,( reco_jet_cemcEsum_SS310 + reco_jet_ihcalEsum_SS310 + reco_jet_ohcalEsum_SS310 )/true_jet_e_SS310);

    h_JES_SS310->Fill(reco_jet_e_SS310/true_jet_e_SS310);
    h_JPTS_SS310->Fill(reco_jet_pt_SS310/true_jet_pt_SS310);
    true_jet_m_SS310 = sqrt(pow(true_jet_e_SS310,2)-pow(true_jet_p_SS310,2));
    reco_jet_m_SS310 = sqrt(pow(reco_jet_e_SS310,2)-pow(reco_jet_p_SS310,2));
    h_JMS_SS310->Fill(reco_jet_m_SS310/true_jet_m_SS310);
  }

  ////////// fill Al ////////// 
  int evt_Al;
  float true_jet_e_Al;
  float true_jet_p_Al;
  float true_jet_pt_Al;
  float true_jet_emfrac_Al;
  float reco_jet_e_Al;
  float reco_jet_p_Al;
  float reco_jet_pt_Al;
  float reco_jet_cemcEsum_Al;
  float reco_jet_ihcalEsum_Al;
  float reco_jet_ohcalEsum_Al;
  out_tree_Al->SetBranchAddress("evt",&evt_Al);
  out_tree_Al->SetBranchAddress("true_jet_e",&true_jet_e_Al);
  out_tree_Al->SetBranchAddress("true_jet_p",&true_jet_p_Al);
  out_tree_Al->SetBranchAddress("true_jet_pt",&true_jet_pt_Al);
  out_tree_Al->SetBranchAddress("true_jet_emfrac",&true_jet_emfrac_Al);
  out_tree_Al->SetBranchAddress("reco_jet_e",&reco_jet_e_Al);
  out_tree_Al->SetBranchAddress("reco_jet_p",&reco_jet_p_Al);
  out_tree_Al->SetBranchAddress("reco_jet_pt",&reco_jet_pt_Al);
  out_tree_Al->SetBranchAddress("reco_jet_cemcEsum",&reco_jet_cemcEsum_Al);
  out_tree_Al->SetBranchAddress("reco_jet_ihcalEsum",&reco_jet_ihcalEsum_Al);
  out_tree_Al->SetBranchAddress("reco_jet_ohcalEsum",&reco_jet_ohcalEsum_Al);
  
  float true_jet_m_Al;
  float reco_jet_m_Al;

  int nevt_Al = out_tree_Al->GetEntries();
  cout << "nevt_Al = " << nevt_Al << endl;

  for (int ievt=0; ievt <nevt_Al; ievt++){
    out_tree_Al->GetEvent(ievt);
    h_emfrac_Al -> Fill(true_jet_emfrac_Al, reco_jet_e_Al/true_jet_e_Al);
    //h_emfrac_Al -> Fill(true_jet_emfrac_Al,( reco_jet_cemcEsum_Al + reco_jet_ihcalEsum_Al + reco_jet_ohcalEsum_Al )/true_jet_e_Al);
    
    h_JES_Al->Fill(reco_jet_e_Al/true_jet_e_Al);
    h_JPTS_Al->Fill(reco_jet_pt_Al/true_jet_pt_Al);
    true_jet_m_Al = sqrt(pow(true_jet_e_Al,2)-pow(true_jet_p_Al,2));
    reco_jet_m_Al = sqrt(pow(reco_jet_e_Al,2)-pow(reco_jet_p_Al,2));
    h_JMS_Al->Fill(reco_jet_m_Al/true_jet_m_Al);
  }
 
  ////////// fill Alframe ////////// 
  int evt_Alframe;
  float true_jet_e_Alframe;
  float true_jet_p_Alframe;
  float true_jet_pt_Alframe;
  float true_jet_emfrac_Alframe;
  float reco_jet_e_Alframe;
  float reco_jet_p_Alframe;
  float reco_jet_pt_Alframe;
  float reco_jet_cemcEsum_Alframe;
  float reco_jet_ihcalEsum_Alframe;
  float reco_jet_ohcalEsum_Alframe;
  out_tree_Alframe->SetBranchAddress("evt",&evt_Alframe);
  out_tree_Alframe->SetBranchAddress("true_jet_e",&true_jet_e_Alframe);
  out_tree_Alframe->SetBranchAddress("true_jet_p",&true_jet_p_Alframe);
  out_tree_Alframe->SetBranchAddress("true_jet_pt",&true_jet_pt_Alframe);
  out_tree_Alframe->SetBranchAddress("true_jet_emfrac",&true_jet_emfrac_Alframe);
  out_tree_Alframe->SetBranchAddress("reco_jet_e",&reco_jet_e_Alframe);
  out_tree_Alframe->SetBranchAddress("reco_jet_p",&reco_jet_p_Alframe);
  out_tree_Alframe->SetBranchAddress("reco_jet_pt",&reco_jet_pt_Alframe);
  out_tree_Alframe->SetBranchAddress("reco_jet_cemcEsum",&reco_jet_cemcEsum_Alframe);
  out_tree_Alframe->SetBranchAddress("reco_jet_ihcalEsum",&reco_jet_ihcalEsum_Alframe);
  out_tree_Alframe->SetBranchAddress("reco_jet_ohcalEsum",&reco_jet_ohcalEsum_Alframe);

  float true_jet_m_Alframe;
  float reco_jet_m_Alframe;

  int nevt_Alframe = out_tree_Alframe->GetEntries();
  cout << "nevt_Alframe = " << nevt_Alframe << endl;

  for (int ievt=0; ievt <nevt_Alframe; ievt++){
    out_tree_Alframe->GetEvent(ievt);
    h_emfrac_Alframe -> Fill(true_jet_emfrac_Alframe, reco_jet_e_Alframe/true_jet_e_Alframe);
    //h_emfrac_Alframe -> Fill(true_jet_emfrac_Alframe,( reco_jet_cemcEsum_Alframe + reco_jet_ihcalEsum_Alframe + reco_jet_ohcalEsum_Alframe )/true_jet_e_Alframe);
    //h_emfrac_Alframe -> Fill(true_jet_emfrac_Alframe,( reco_jet_cemcEsum_Alframe + reco_jet_ohcalEsum_Alframe )/true_jet_e_Alframe);
    
    h_JES_Alframe->Fill(reco_jet_e_Alframe/true_jet_e_Alframe);
    h_JPTS_Alframe->Fill(reco_jet_pt_Alframe/true_jet_pt_Alframe);
    true_jet_m_Alframe = sqrt(pow(true_jet_e_Alframe,2)-pow(true_jet_p_Alframe,2));
    reco_jet_m_Alframe = sqrt(pow(reco_jet_e_Alframe,2)-pow(reco_jet_p_Alframe,2));
    h_JMS_Alframe->Fill(reco_jet_m_Alframe/true_jet_m_Alframe);
  }
*/
  
  h_JES_SS310->Scale(1./h_JES_SS310->Integral());
  h_JES_Al->Scale(1./h_JES_Al->Integral());
  h_JES_Alframe->Scale(1./h_JES_Alframe->Integral());
  h_JPTS_SS310->Scale(1./h_JPTS_SS310->Integral());
  h_JPTS_Al->Scale(1./h_JPTS_Al->Integral());
  h_JPTS_Alframe->Scale(1./h_JPTS_Alframe->Integral());
  h_JMS_SS310->Scale(1./h_JMS_SS310->Integral());
  h_JMS_Al->Scale(1./h_JMS_Al->Integral());
  h_JMS_Alframe->Scale(1./h_JMS_Alframe->Integral());
 
  h_emfrac_SS310->SetOption("colz");
  h_emfrac_Al->SetOption("colz");
  h_emfrac_Alframe->SetOption("colz");

  //////////////////////////////////////////////////////
  //////////////////////////////////////////////////////
  TLatex* latex = new TLatex();
  latex->SetNDC();
  //latex->SetTextAlign(32);
  latex->SetTextAlign(12);
  latex->SetTextSize(0.045);
  
  TLegend* leg_1D = new TLegend(0.40, 0.77, 0.80, 0.92);
  leg_1D->SetTextSize(0.040);
  leg_1D->AddEntry(h_JES_SS310,"Instrumented Steel","lp"); 
  leg_1D->AddEntry(h_JES_Al,"Instrumented Aluminum","lp"); 
  leg_1D->AddEntry(h_JES_Alframe,"Uninstrumented Aluminum","lp"); 
  
  //////////////////////////////////////////////////////
  float col;
  int repeat=30;
  float fitmin_jes = 0.5;
  float fitmax_jes = 1.5;
  float mean_jes;
  float sig_jes;
  float sigErr_jes;

  double jesymax =0.12;
  double jptsymax =0.12;
  double jmsymax =0.07;
  
  //TCanvas* c_JES = new TCanvas("c_JES","",1500,500);
  //c_JES->Divide(3,1);
  
  ////////////////////////////////////////////////
  TCanvas* c_JES = new TCanvas("c_JES","",600,600);
  c_JES->cd();

  SetHistStyle(h_JES_SS310,0,10);
  h_JES_SS310->SetLineColor(kPink+2);
  h_JES_SS310->SetMarkerColor(kPink+2);
  h_JES_SS310->GetXaxis()->CenterTitle(1);
  h_JES_SS310->GetYaxis()->CenterTitle(1);
  h_JES_SS310->SetMaximum(jesymax);
  h_JES_SS310->Draw("pe");
  TF1* tf_JES_SS310= new TF1("TF_JES_SS310","gaus",fitmin_jes,fitmax_jes);
  //col = h_JES_SS310->GetMarkerColor();
  //tf_JES_SS310->SetLineColor(col);
  tf_JES_SS310->SetLineColor(kPink+3);
  recursiveFit(h_JES_SS310, tf_JES_SS310,fitmin_jes, fitmax_jes, &mean_jes, &sig_jes, &sigErr_jes);
  for (int ir=0; ir<repeat; ir++){
    fitmin_jes = mean_jes-2*sig_jes;
    fitmax_jes = mean_jes+2*sig_jes;
    recursiveFit(h_JES_SS310, tf_JES_SS310,fitmin_jes, fitmax_jes, &mean_jes, &sig_jes, &sigErr_jes);
  }
  tf_JES_SS310->Draw("same");
  
  latex->SetTextColor(kBlack);  
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4, |#eta| < 0.45");
  if (jetE.compare("20GeV")==0) latex->DrawLatex(0.2,0.77,"20 < E_{Jet}^{Truth} < 30 GeV");
  else if (jetE.compare("30GeV")==0) latex->DrawLatex(0.2,0.77,"30 < E_{Jet}^{Truth} < 40 GeV");
  else if (jetE.compare("40GeV")==0) latex->DrawLatex(0.2,0.77,"40 < E_{Jet}^{Truth} < 50 GeV");
  else if (jetE.compare("50GeV")==0) latex->DrawLatex(0.2,0.77,"50 < E_{Jet}^{Truth} < 60 GeV");
  else { cout << "WARNING!! 20, 30, 40 or 50 GeV"<<endl; return; };
  latex->SetTextColor(kPink+3);  
  latex->DrawLatex(0.59,0.68,Form("#mu = %.2f, #sigma/#mu = %.2f",mean_jes,sig_jes/mean_jes));
  
  SetHistStyle(h_JES_Al,0,10);
  h_JES_Al->SetLineColor(kAzure+5);
  h_JES_Al->SetMarkerColor(kAzure+5);
  h_JES_Al->GetXaxis()->CenterTitle(1);
  h_JES_Al->GetYaxis()->CenterTitle(1);
  h_JES_Al->Draw("pe same");
  TF1* tf_JES_Al= new TF1("TF_JES_Al","gaus",fitmin_jes,fitmax_jes);
  //col = h_JES_Al->GetMarkerColor();
  //tf_JES_Al->SetLineColor(col);
  tf_JES_Al->SetLineColor(kAzure+4);
  recursiveFit(h_JES_Al, tf_JES_Al,fitmin_jes, fitmax_jes, &mean_jes, &sig_jes, &sigErr_jes);
  for (int ir=0; ir<repeat; ir++){
    fitmin_jes = mean_jes-2*sig_jes;
    fitmax_jes = mean_jes+2*sig_jes;
    recursiveFit(h_JES_Al, tf_JES_Al,fitmin_jes, fitmax_jes, &mean_jes, &sig_jes, &sigErr_jes);
  }
  tf_JES_Al->Draw("same");
  latex->SetTextColor(kAzure+4);  
  latex->DrawLatex(0.59,0.62,Form("#mu = %.2f, #sigma/#mu = %.2f",mean_jes,sig_jes/mean_jes));

  SetHistStyle(h_JES_Alframe,0,10);
  h_JES_Alframe->SetLineColor(kSpring+9);
  h_JES_Alframe->SetMarkerColor(kSpring+9);
  h_JES_Alframe->GetXaxis()->CenterTitle(1);
  h_JES_Alframe->GetYaxis()->CenterTitle(1);
  h_JES_Alframe->Draw("pe same");
  TF1* tf_JES_Alframe= new TF1("TF_JES_Alframe","gaus",fitmin_jes,fitmax_jes);
  //col = h_JES_Alframe->GetMarkerColor();
  //tf_JES_Alframe->SetLineColor(col);
  tf_JES_Alframe->SetLineColor(kSpring+3);
  recursiveFit(h_JES_Alframe, tf_JES_Alframe,fitmin_jes, fitmax_jes, &mean_jes, &sig_jes, &sigErr_jes);
  for (int ir=0; ir<repeat; ir++){
    fitmin_jes = mean_jes-2*sig_jes;
    fitmax_jes = mean_jes+2*sig_jes;
    recursiveFit(h_JES_Alframe, tf_JES_Alframe,fitmin_jes, fitmax_jes, &mean_jes, &sig_jes, &sigErr_jes);
  }
  tf_JES_Alframe->Draw("same");
  latex->SetTextColor(kSpring+3);  
  latex->DrawLatex(0.59,0.56,Form("#mu = %.2f, #sigma/#mu = %.2f",mean_jes,sig_jes/mean_jes));
 
  //leg_1D->Draw();
  c_JES->SaveAs(Form("outHallman/JES_%s_%s_isCalib%d.pdf",jetE.c_str(),note.c_str(),(int)isCalib));
  
  ////////////////////////////////////////////////
  TCanvas* c_JPTS = new TCanvas("c_JPTS","",600,600);
  c_JPTS->cd();

  SetHistStyle(h_JPTS_SS310,0,10);
  h_JPTS_SS310->SetLineColor(kPink+2);
  h_JPTS_SS310->SetMarkerColor(kPink+2);
  h_JPTS_SS310->GetXaxis()->CenterTitle(1);
  h_JPTS_SS310->GetYaxis()->CenterTitle(1);
  h_JPTS_SS310->SetMaximum(jptsymax);
  h_JPTS_SS310->Draw("pe");
  TF1* tf_JPTS_SS310= new TF1("TF_JPTS_SS310","gaus",fitmin_jes,fitmax_jes);
  //col = h_JPTS_SS310->GetMarkerColor();
  //tf_JPTS_SS310->SetLineColor(col);
  tf_JPTS_SS310->SetLineColor(kPink+3);
  recursiveFit(h_JPTS_SS310, tf_JPTS_SS310,fitmin_jes, fitmax_jes, &mean_jes, &sig_jes, &sigErr_jes);
  for (int ir=0; ir<repeat; ir++){
    fitmin_jes = mean_jes-2*sig_jes;
    fitmax_jes = mean_jes+2*sig_jes;
    recursiveFit(h_JPTS_SS310, tf_JPTS_SS310,fitmin_jes, fitmax_jes, &mean_jes, &sig_jes, &sigErr_jes);
  }
  tf_JPTS_SS310->Draw("same");
  latex->SetTextColor(kBlack);  
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4, |#eta| < 0.45");
  if (jetE.compare("20GeV")==0) latex->DrawLatex(0.2,0.77,"20 < E_{Jet}^{Truth} < 30 GeV");
  else if (jetE.compare("30GeV")==0) latex->DrawLatex(0.2,0.77,"30 < E_{Jet}^{Truth} < 40 GeV");
  else if (jetE.compare("40GeV")==0) latex->DrawLatex(0.2,0.77,"40 < E_{Jet}^{Truth} < 50 GeV");
  else if (jetE.compare("50GeV")==0) latex->DrawLatex(0.2,0.77,"50 < E_{Jet}^{Truth} < 60 GeV");
  else { cout << "WARNING!! 20, 30, 40 or 50 GeV"<<endl; return; };
  latex->SetTextColor(kPink+3);  
  latex->DrawLatex(0.59,0.68,Form("#mu = %.2f, #sigma/#mu = %.2f",mean_jes,sig_jes/mean_jes));
  
  SetHistStyle(h_JPTS_Al,0,10);
  h_JPTS_Al->SetLineColor(kAzure+5);
  h_JPTS_Al->SetMarkerColor(kAzure+5);
  h_JPTS_Al->GetXaxis()->CenterTitle(1);
  h_JPTS_Al->GetYaxis()->CenterTitle(1);
  h_JPTS_Al->Draw("pe same");
  TF1* tf_JPTS_Al= new TF1("TF_JPTS_Al","gaus",fitmin_jes,fitmax_jes);
  //col = h_JPTS_Al->GetMarkerColor();
  //tf_JPTS_Al->SetLineColor(col);
  tf_JPTS_Al->SetLineColor(kAzure+4);
  recursiveFit(h_JPTS_Al, tf_JPTS_Al,fitmin_jes, fitmax_jes, &mean_jes, &sig_jes, &sigErr_jes);
  for (int ir=0; ir<repeat; ir++){
    fitmin_jes = mean_jes-2*sig_jes;
    fitmax_jes = mean_jes+2*sig_jes;
    recursiveFit(h_JPTS_Al, tf_JPTS_Al,fitmin_jes, fitmax_jes, &mean_jes, &sig_jes, &sigErr_jes);
  }
  tf_JPTS_Al->Draw("same");
  latex->SetTextColor(kAzure+4);  
  latex->DrawLatex(0.59,0.62,Form("#mu = %.2f, #sigma/#mu = %.2f",mean_jes,sig_jes/mean_jes));

  SetHistStyle(h_JPTS_Alframe,0,10);
  h_JPTS_Alframe->SetLineColor(kSpring+9);
  h_JPTS_Alframe->SetMarkerColor(kSpring+9);
  h_JPTS_Alframe->GetXaxis()->CenterTitle(1);
  h_JPTS_Alframe->GetYaxis()->CenterTitle(1);
  h_JPTS_Alframe->Draw("pe same");
  TF1* tf_JPTS_Alframe= new TF1("TF_JPTS_Alframe","gaus",fitmin_jes,fitmax_jes);
  //col = h_JPTS_Alframe->GetMarkerColor();
  //tf_JPTS_Alframe->SetLineColor(col);
  tf_JPTS_Alframe->SetLineColor(kSpring+3);
  recursiveFit(h_JPTS_Alframe, tf_JPTS_Alframe,fitmin_jes, fitmax_jes, &mean_jes, &sig_jes, &sigErr_jes);
  for (int ir=0; ir<repeat; ir++){
    fitmin_jes = mean_jes-2*sig_jes;
    fitmax_jes = mean_jes+2*sig_jes;
    recursiveFit(h_JPTS_Alframe, tf_JPTS_Alframe,fitmin_jes, fitmax_jes, &mean_jes, &sig_jes, &sigErr_jes);
  }
  tf_JPTS_Alframe->Draw("same");
  latex->SetTextColor(kSpring+3);  
  latex->DrawLatex(0.59,0.56,Form("#mu = %.2f, #sigma/#mu = %.2f",mean_jes,sig_jes/mean_jes));
  
  //leg_1D->Draw();
  c_JPTS->SaveAs(Form("outHallman/JPTS_%s_%s_isCalib%d.pdf",jetE.c_str(),note.c_str(),(int)isCalib));
  
  ////////////////////////////////////////////////
  TCanvas* c_JMS = new TCanvas("c_JMS","",600,600);
  c_JMS->cd();

  SetHistStyle(h_JMS_SS310,0,10);
  h_JMS_SS310->SetLineColor(kPink+2);
  h_JMS_SS310->SetMarkerColor(kPink+2);
  h_JMS_SS310->GetXaxis()->CenterTitle(1);
  h_JMS_SS310->GetYaxis()->CenterTitle(1);
  h_JMS_SS310->SetMaximum(jmsymax);
  h_JMS_SS310->Draw("pe");
  TF1* tf_JMS_SS310= new TF1("TF_JMS_SS310","gaus",fitmin_jes,fitmax_jes);
  //col = h_JMS_SS310->GetMarkerColor();
  //tf_JMS_SS310->SetLineColor(col);
  tf_JMS_SS310->SetLineColor(kPink+3);
  recursiveFit(h_JMS_SS310, tf_JMS_SS310,fitmin_jes, fitmax_jes, &mean_jes, &sig_jes, &sigErr_jes);
  for (int ir=0; ir<repeat; ir++){
    fitmin_jes = mean_jes-2*sig_jes;
    fitmax_jes = mean_jes+2*sig_jes;
    recursiveFit(h_JMS_SS310, tf_JMS_SS310,fitmin_jes, fitmax_jes, &mean_jes, &sig_jes, &sigErr_jes);
  }
  tf_JMS_SS310->Draw("same");
  latex->SetTextColor(kBlack);  
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4, |#eta| < 0.45");
  if (jetE.compare("20GeV")==0) latex->DrawLatex(0.2,0.77,"20 < E_{Jet}^{Truth} < 30 GeV");
  else if (jetE.compare("30GeV")==0) latex->DrawLatex(0.2,0.77,"30 < E_{Jet}^{Truth} < 40 GeV");
  else if (jetE.compare("40GeV")==0) latex->DrawLatex(0.2,0.77,"40 < E_{Jet}^{Truth} < 50 GeV");
  else if (jetE.compare("50GeV")==0) latex->DrawLatex(0.2,0.77,"50 < E_{Jet}^{Truth} < 60 GeV");
  else { cout << "WARNING!! 20, 30, 40 or 50 GeV"<<endl; return; };
  latex->SetTextColor(kPink+3);  
  latex->DrawLatex(0.59,0.68,Form("#mu = %.2f, #sigma/#mu = %.2f",mean_jes,sig_jes/mean_jes));
  
  SetHistStyle(h_JMS_Al,0,10);
  h_JMS_Al->SetLineColor(kAzure+5);
  h_JMS_Al->SetMarkerColor(kAzure+5);
  h_JMS_Al->GetXaxis()->CenterTitle(1);
  h_JMS_Al->GetYaxis()->CenterTitle(1);
  h_JMS_Al->Draw("pe same");
  TF1* tf_JMS_Al= new TF1("TF_JMS_Al","gaus",fitmin_jes,fitmax_jes);
  //col = h_JMS_Al->GetMarkerColor();
  //tf_JMS_Al->SetLineColor(col);
  tf_JMS_Al->SetLineColor(kAzure+4);
  recursiveFit(h_JMS_Al, tf_JMS_Al,fitmin_jes, fitmax_jes, &mean_jes, &sig_jes, &sigErr_jes);
  for (int ir=0; ir<repeat; ir++){
    fitmin_jes = mean_jes-2*sig_jes;
    fitmax_jes = mean_jes+2*sig_jes;
    recursiveFit(h_JMS_Al, tf_JMS_Al,fitmin_jes, fitmax_jes, &mean_jes, &sig_jes, &sigErr_jes);
  }
  tf_JMS_Al->Draw("same");
  latex->SetTextColor(kAzure+4);  
  latex->DrawLatex(0.59,0.62,Form("#mu = %.2f, #sigma/#mu = %.2f",mean_jes,sig_jes/mean_jes));

  SetHistStyle(h_JMS_Alframe,0,10);
  h_JMS_Alframe->SetLineColor(kSpring+9);
  h_JMS_Alframe->SetMarkerColor(kSpring+9);
  h_JMS_Alframe->GetXaxis()->CenterTitle(1);
  h_JMS_Alframe->GetYaxis()->CenterTitle(1);
  h_JMS_Alframe->Draw("pe same");
  TF1* tf_JMS_Alframe= new TF1("TF_JMS_Alframe","gaus",fitmin_jes,fitmax_jes);
  //col = h_JMS_Alframe->GetMarkerColor();
  //tf_JMS_Alframe->SetLineColor(col);
  tf_JMS_Alframe->SetLineColor(kSpring+3);
  recursiveFit(h_JMS_Alframe, tf_JMS_Alframe,fitmin_jes, fitmax_jes, &mean_jes, &sig_jes, &sigErr_jes);
  for (int ir=0; ir<repeat; ir++){
    fitmin_jes = mean_jes-2*sig_jes;
    fitmax_jes = mean_jes+2*sig_jes;
    recursiveFit(h_JMS_Alframe, tf_JMS_Alframe,fitmin_jes, fitmax_jes, &mean_jes, &sig_jes, &sigErr_jes);
  }
  tf_JMS_Alframe->Draw("same");
  latex->SetTextColor(kSpring+3);  
  latex->DrawLatex(0.59,0.56,Form("#mu = %.2f, #sigma/#mu = %.2f",mean_jes,sig_jes/mean_jes));
 
  //leg_1D->Draw();
  c_JMS->SaveAs(Form("outHallman/JMS_%s_%s_isCalib%d.pdf",jetE.c_str(),note.c_str(),(int)isCalib));
  
  //////////////////////////////////////////////////////
  
  //TCanvas* c_2D_diff = new TCanvas("c_2D_diff","",1600,1200);
  TCanvas* c_2D_diff = new TCanvas("c_2D_diff","",1200,800);
  c_2D_diff->Divide(2,2);
  c_2D_diff->cd(1);
  h_emfrac_SS310->Draw();
  c_2D_diff->cd(2);
  h_emfrac_Al->Draw();
  c_2D_diff->cd(3);
  h_emfrac_Alframe->Draw();
  
  TPaletteAxis *palette;
  double margin;
  double x1ndc;
  double x2ndc;

  //////////////////////////////////////////////////////
  double fitmin = 0.0;
  double fitmax = 1.0;
  
  ////////////////////////////////////////////////
  ////////////////////////////////////////////////
  c_2D_diff->cd(1);
  margin = gPad->GetRightMargin();
  //gPad->SetRightMargin(margin*3);
  gPad->SetRightMargin(0.13);
  
  //TProfile *prof_emfrac_SS310 = h_emfrac_SS310->ProfileX();
  //prof_emfrac_SS310->Fit("pol1","","",fitmin,fitmax); 
  //gStyle->SetOptFit(0);
  prof_emfrac_SS310->SetMarkerColor(kPink+1);
  prof_emfrac_SS310->SetLineColor(kPink+1);
  prof_emfrac_SS310->SetMarkerStyle(kFullCircle);
  prof_emfrac_SS310->SetMarkerSize(0.8);
  //TF1 *func_emfrac_SS310 = (TF1*)prof_emfrac_SS310->GetListOfFunctions()->FindObject("pol1");
  if(func_emfrac_SS310){
    func_emfrac_SS310->SetLineColor(kRed-4);
    func_emfrac_SS310->SetLineWidth(4);
    func_emfrac_SS310->SetRange(fitmin,fitmax);
    h_emfrac_SS310->GetXaxis()->CenterTitle(1);
    h_emfrac_SS310->GetYaxis()->CenterTitle(1);
    h_emfrac_SS310->GetYaxis()->SetTitle("E_{Jet}^{Reco}/E_{Jet}^{Truth}");
    h_emfrac_SS310->Draw();
    prof_emfrac_SS310->Draw("ep same"); 
    
    latex->SetTextColor(kBlack);  
    latex->SetTextSize(0.053);
    latex->SetTextFont(72);
    latex->DrawLatex(0.2,0.89,"sPHENIX");
    latex->SetTextFont(42);
    latex->DrawLatex(0.34,0.89,"G4Simulation");
    latex->SetTextSize(0.040);
    latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4, |#eta| < 0.45");
    latex->SetTextColor(kRed);
    latex->DrawLatex(0.2,0.77,"Instrumented Steel");
    latex->SetTextColor(kBlack);
    latex->SetTextAlign(12);
    latex->SetTextSize(0.040);
    latex->DrawLatex(0.65,0.88,Form("#chi^{2}/ndf: %.2f/%d",func_emfrac_SS310->GetChisquare(),func_emfrac_SS310->GetNDF()));
    latex->DrawLatex(0.65,0.82,Form("p0: %.3f #pm %.3f",func_emfrac_SS310->GetParameter(0),func_emfrac_SS310->GetParError(0)));
    latex->DrawLatex(0.65,0.76,Form("p1: %.3f #pm %.3f",func_emfrac_SS310->GetParameter(1),func_emfrac_SS310->GetParError(1)));  
  }
  c_2D_diff->Update(); //mandatory
  palette = (TPaletteAxis*)h_emfrac_SS310->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
  c_2D_diff->Update();
  
  ////////////////////////////////////////////////
  c_2D_diff->cd(2);
  margin = gPad->GetRightMargin();
  //gPad->SetRightMargin(margin*3);
  gPad->SetRightMargin(0.13);
  
  //TProfile *prof_emfrac_Al = h_emfrac_Al->ProfileX();
  //prof_emfrac_Al->Fit("pol1","","",fitmin,fitmax); 
  //gStyle->SetOptFit(0);
  prof_emfrac_Al->SetMarkerColor(kPink+1);
  prof_emfrac_Al->SetLineColor(kPink+1);
  prof_emfrac_Al->SetMarkerStyle(kFullCircle);
  prof_emfrac_Al->SetMarkerSize(0.8);
  //TF1 *func_emfrac_Al = (TF1*)prof_emfrac_Al->GetListOfFunctions()->FindObject("pol1");
  if(func_emfrac_Al){
    func_emfrac_Al->SetLineColor(kRed-4);
    func_emfrac_Al->SetLineWidth(4);
    func_emfrac_Al->SetRange(fitmin,fitmax);
    h_emfrac_Al->GetXaxis()->CenterTitle(1);
    h_emfrac_Al->GetYaxis()->CenterTitle(1);
    h_emfrac_Al->GetYaxis()->SetTitle("E_{Jet}^{Reco}/E_{Jet}^{Truth}");
    h_emfrac_Al->Draw();
    prof_emfrac_Al->Draw("ep same"); 
    
    latex->SetTextColor(kBlack);  
    latex->SetTextSize(0.053);
    latex->SetTextFont(72);
    latex->DrawLatex(0.2,0.89,"sPHENIX");
    latex->SetTextFont(42);
    latex->DrawLatex(0.34,0.89,"G4Simulation");
    latex->SetTextSize(0.040);
    latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4, |#eta| < 0.45");
    latex->SetTextColor(kRed);
    latex->DrawLatex(0.2,0.77,"Instrumented Aluminum");
    latex->SetTextColor(kBlack);
    latex->SetTextAlign(12);
    latex->SetTextSize(0.040);
    latex->DrawLatex(0.65,0.88,Form("#chi^{2}/ndf: %.2f/%d",func_emfrac_Al->GetChisquare(),func_emfrac_Al->GetNDF()));
    latex->DrawLatex(0.65,0.82,Form("p0: %.3f #pm %.3f",func_emfrac_Al->GetParameter(0),func_emfrac_Al->GetParError(0)));
    latex->DrawLatex(0.65,0.76,Form("p1: %.3f #pm %.3f",func_emfrac_Al->GetParameter(1),func_emfrac_Al->GetParError(1)));  
  }
  c_2D_diff->Update(); //mandatory
  palette = (TPaletteAxis*)h_emfrac_Al->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
  c_2D_diff->Update();
  
  ////////////////////////////////////////////////
  c_2D_diff->cd(3);
  margin = gPad->GetRightMargin();
  //gPad->SetRightMargin(margin*3);
  gPad->SetRightMargin(0.13);
  
  //TProfile *prof_emfrac_Alframe = h_emfrac_Alframe->ProfileX();
  //prof_emfrac_Alframe->Fit("pol1","","",fitmin,fitmax); 
  //gStyle->SetOptFit(0);
  prof_emfrac_Alframe->SetMarkerColor(kPink+1);
  prof_emfrac_Alframe->SetLineColor(kPink+1);
  prof_emfrac_Alframe->SetMarkerStyle(kFullCircle);
  prof_emfrac_Alframe->SetMarkerSize(0.8);
  //TF1 *func_emfrac_Alframe = (TF1*)prof_emfrac_Alframe->GetListOfFunctions()->FindObject("pol1");
  if(func_emfrac_Alframe){
    func_emfrac_Alframe->SetLineColor(kRed-4);
    func_emfrac_Alframe->SetLineWidth(4);
    func_emfrac_Alframe->SetRange(fitmin,fitmax);
    h_emfrac_Alframe->GetXaxis()->CenterTitle(1);
    h_emfrac_Alframe->GetYaxis()->CenterTitle(1);
    h_emfrac_Alframe->GetYaxis()->SetTitle("E_{Jet}^{Reco}/E_{Jet}^{Truth}");
    h_emfrac_Alframe->Draw();
    prof_emfrac_Alframe->Draw("ep same"); 
    
    latex->SetTextColor(kBlack);  
    latex->SetTextSize(0.053);
    latex->SetTextFont(72);
    latex->DrawLatex(0.2,0.89,"sPHENIX");
    latex->SetTextFont(42);
    latex->DrawLatex(0.34,0.89,"G4Simulation");
    latex->SetTextSize(0.040);
    latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4, |#eta| < 0.45");
    latex->SetTextColor(kRed);
    latex->DrawLatex(0.2,0.77,"Uninstrumented Aluminum");
    latex->SetTextColor(kBlack);
    latex->SetTextAlign(12);
    latex->SetTextSize(0.040);
    latex->DrawLatex(0.65,0.88,Form("#chi^{2}/ndf: %.2f/%d",func_emfrac_Alframe->GetChisquare(),func_emfrac_Alframe->GetNDF()));
    latex->DrawLatex(0.65,0.82,Form("p0: %.3f #pm %.3f",func_emfrac_Alframe->GetParameter(0),func_emfrac_Alframe->GetParError(0)));
    latex->DrawLatex(0.65,0.76,Form("p1: %.3f #pm %.3f",func_emfrac_Alframe->GetParameter(1),func_emfrac_Alframe->GetParError(1)));  
  }
  c_2D_diff->Update(); //mandatory
  palette = (TPaletteAxis*)h_emfrac_Alframe->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
  c_2D_diff->Update();

  c_2D_diff->SaveAs(Form("outHallman/2D_diff_%s_%s_isCalib%d.pdf",jetE.c_str(),note.c_str(),(int)isCalib));
  
  ////////////////////////////////////////
  ////////////////////////////////////////

  double allymin =0.55;
  double allymax =1.23;
 
  TCanvas* c_2D_all = new TCanvas("c_2D_all","",600,600);
  c_2D_all->cd();
  prof_emfrac_SS310->GetXaxis()->CenterTitle(1);
  prof_emfrac_SS310->GetYaxis()->CenterTitle(1);
  prof_emfrac_SS310->GetXaxis()->SetRangeUser(0.,1.);
  prof_emfrac_SS310->GetYaxis()->SetRangeUser(allymin,allymax);
  prof_emfrac_SS310->GetYaxis()->SetTitle("E_{Jet}^{Reco}/E_{Jet}^{Truth}");
  
  prof_emfrac_SS310->SetLineColor(kPink+2);
  prof_emfrac_SS310->SetMarkerColor(kPink+2);
  prof_emfrac_SS310->Draw("ep");
  func_emfrac_SS310->SetLineColor(kPink+3);
  func_emfrac_SS310->Draw("same");
  
  prof_emfrac_Al->SetLineColor(kAzure+5);
  prof_emfrac_Al->SetMarkerColor(kAzure+5);
  prof_emfrac_Al->Draw("ep same");
  func_emfrac_Al->SetLineColor(kAzure+4);
  func_emfrac_Al->Draw("same");
  
  prof_emfrac_Alframe->SetLineColor(kSpring+9);
  prof_emfrac_Alframe->SetMarkerColor(kSpring+9);
  prof_emfrac_Alframe->Draw("ep same");
  func_emfrac_Alframe->SetLineColor(kSpring+3);
  func_emfrac_Alframe->Draw("same");

  //dashedLine(0,1,1,1);
  
  latex->SetTextColor(kBlack);  
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4, |#eta| < 0.45");
  if (jetE.compare("20GeV")==0) latex->DrawLatex(0.2,0.77,"20 < E_{Jet}^{Truth} < 30 GeV");
  else if (jetE.compare("30GeV")==0) latex->DrawLatex(0.2,0.77,"30 < E_{Jet}^{Truth} < 40 GeV");
  else if (jetE.compare("40GeV")==0) latex->DrawLatex(0.2,0.77,"40 < E_{Jet}^{Truth} < 50 GeV");
  else if (jetE.compare("50GeV")==0) latex->DrawLatex(0.2,0.77,"50 < E_{Jet}^{Truth} < 60 GeV");
  else { cout << "WARNING!! 20, 30, 40 or 50 GeV"<<endl; return; };
  
  TLegend* leg_2D = new TLegend(0.44, 0.20, 0.68, 0.35);
  leg_2D->SetTextSize(0.040);
  leg_2D->AddEntry(prof_emfrac_SS310,"Instrumented Steel","lp"); 
  leg_2D->AddEntry(prof_emfrac_Al,"Instrumented Aluminum","lp"); 
  leg_2D->AddEntry(prof_emfrac_Alframe,"Uninstrumented Aluminum","lp"); 
  leg_2D->Draw();
  
  c_2D_all->SaveAs(Form("outHallman/2D_all_%s_%s_isCalib%d.pdf",jetE.c_str(),note.c_str(),(int)isCalib));
  
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
