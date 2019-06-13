#include "TMinuit.h"

void recursiveFit(TH1D* t_h1, TF1* t_tf1, float t_fitmin, float t_fitmax, float* t_mean, float* t_sig, float* t_sigErr);

void final_qcdjet(string sampleType = "QCDJet",
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

  TH2D* h_IHAsymm = new TH2D("h_IHAsymm",";Reco Jet E_{IHCAL}-E_{OHCAL} asymmetry;Jet E_{HCAL} / (Jet E_{Truth} - Jet E_{EMCAL})",101,-1.05,1.05,100,0,2);
  TH2D* h_EHAsymm = new TH2D("h_EHAsymm",";Reco Jet E_{EMCAL}^{had}-E_{HCAL} asymmetry;(Jet E_{HCAL}+Jet E_{EMCAL}^{had}) / (Jet E_{Truth} - Jet E_{EMCAL}^{EM})",101,-1.05,1.05,100,0,2);
  TH2D* h_totEHAsymm = new TH2D("h_totEHAsymm",";Reco Jet E_{EMCAL}-E_{HCAL} asymmetry;Jet E_{Reco} / Jet E_{Truth}",101,-1.05,1.05,100,0,2);
  TH2D* h_emfrac = new TH2D("h_emfrac",";Truth EM fraction;Jet E_{Reco} / Jet E_{Truth}",101,-0.05,1.05,100,0,2);
 
  TH1D* h_JES = new TH1D("h_JES",";E_{Jet}^{Reco}/E_{Jet}^{Truth};Counts",100,0,2);
  TH1D* h_JPTS = new TH1D("h_JPTS",";p_{T,Jet}^{Reco}/p_{T,Jet}^{Truth};Counts",100,0,2);
  TH1D* h_JMS = new TH1D("h_JMS",";M_{Jet}^{Reco}/M_{Jet}^{Truth};Counts",100,0,2); 
  //TH1D* h_JES = new TH1D("h_JES",";Jet E_{Reco} / Jet E_{Truth};Counts",100,0,2);
  //TH1D* h_JER = new TH1D("h_JER",";(Jet E_{Reco}-Jet E_{Truth}) / Jet E_{Reco};Counts",100,-1,1);
  
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
  //int nevt = 500;
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
    ihasymm = ( reco_jet_ihcalEsum - reco_jet_ohcalEsum ) / ( reco_jet_ihcalEsum + reco_jet_ohcalEsum );
    ihfrac = ( reco_jet_ihcalEsum + reco_jet_ohcalEsum ) / ( true_jet_e - (reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum) );
    
    ehasymm = ( reco_jet_clcemc_hadEsum - (reco_jet_ihcalEsum + reco_jet_ohcalEsum) ) / ( reco_jet_clcemc_hadEsum + (reco_jet_ihcalEsum + reco_jet_ohcalEsum) );
    ehfrac = ( reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum ) / ( true_jet_e - reco_jet_clcemc_emEsum );

    totehasymm = ( (reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum) - (reco_jet_ihcalEsum + reco_jet_ohcalEsum) ) / ( (reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum) + (reco_jet_ihcalEsum + reco_jet_ohcalEsum) );
    totehfrac = ( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum ) / ( true_jet_e );
    
    true_jet_m = sqrt(pow(true_jet_e,2)-pow(true_jet_p,2));
    reco_jet_m = sqrt(pow(reco_jet_e,2)-pow(reco_jet_p,2));
     
    h_IHAsymm -> Fill(ihasymm,ihfrac); 
    h_EHAsymm -> Fill(ehasymm,ehfrac); 
    h_totEHAsymm -> Fill(totehasymm,totehfrac); 
    h_emfrac -> Fill(true_jet_emfrac,( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum )/true_jet_e);
    
    h_JES->Fill(( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum )/true_jet_e);
    //h_JER->Fill(( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum - true_jet_e)/ ( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum ));
    h_JPTS->Fill(reco_jet_pt/true_jet_pt);
    h_JMS->Fill(reco_jet_m/true_jet_m);
  }
 
  h_IHAsymm->SetOption("colz");
  h_EHAsymm->SetOption("colz");
  h_totEHAsymm->SetOption("colz");
  h_emfrac->SetOption("colz");

  //TCanvas* c1 = new TCanvas("c1","",1600,1200);
  TCanvas* c1 = new TCanvas("c1","",1200,800);
  c1->Divide(2,2);
  c1->cd(1);
  h_IHAsymm->Draw();
  c1->cd(2);
  h_EHAsymm->Draw();
  c1->cd(3);
  h_totEHAsymm->Draw();
  c1->cd(4);
  h_emfrac->Draw();

  TPaletteAxis *palette;
  double margin;
  double x1ndc;
  double x2ndc;

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
  
  ////////////////////////////////////////////////
  c1->cd(1);
  margin = gPad->GetRightMargin();
  //gPad->SetRightMargin(margin*3);
  gPad->SetRightMargin(0.13);
  
  TProfile *prof_IHAsymm = h_IHAsymm->ProfileX();
  prof_IHAsymm->Fit("pol1","","",fitmin,fitmax); 
  gStyle->SetOptFit(0);
  //gStyle->SetOptFit(1);
  //prof_IHAsymm->SetMarkerColor(kCyan+1);
  //prof_IHAsymm->SetLineColor(kCyan+1);
  prof_IHAsymm->SetMarkerColor(kPink+1);
  prof_IHAsymm->SetLineColor(kPink+1);
  prof_IHAsymm->SetMarkerStyle(kFullCircle);
  prof_IHAsymm->SetMarkerSize(0.8);
  TF1 *f_IHAsymm = (TF1*)prof_IHAsymm->GetListOfFunctions()->FindObject("pol1");
  if(f_IHAsymm){
    f_IHAsymm->SetLineColor(kRed-4);
    f_IHAsymm->SetLineWidth(4);
    f_IHAsymm->SetRange(fitmin,fitmax);
    h_IHAsymm->GetXaxis()->CenterTitle(1);
    h_IHAsymm->GetYaxis()->CenterTitle(1);
    h_IHAsymm->Draw();
    prof_IHAsymm->Draw("ep same"); 
    
    latex->SetTextAlign(12);
    latex->SetTextSize(0.040);
    latex->DrawLatex(0.63,0.88,Form("#chi^{2}/ndf: %.2f/%d",f_IHAsymm->GetChisquare(),f_IHAsymm->GetNDF()));
    latex->DrawLatex(0.63,0.82,Form("p0: %.3f #pm %.3f",f_IHAsymm->GetParameter(0),f_IHAsymm->GetParError(0)));
    latex->DrawLatex(0.63,0.76,Form("p1: %.3f #pm %.3f",f_IHAsymm->GetParameter(1),f_IHAsymm->GetParError(1)));  
  }
  c1->Update(); //mandatory
  palette = (TPaletteAxis*)h_IHAsymm->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
//  palette->SetX1NDC(1-margin*4);
//  palette->SetX2NDC(1-margin*3);
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
  c1->Update();
  
  ////////////////////////////////////////////////
  c1->cd(2);
  margin = gPad->GetRightMargin();
  //gPad->SetRightMargin(margin*3);
  gPad->SetRightMargin(0.13);
  
  TProfile *prof_EHAsymm = h_EHAsymm->ProfileX();
  prof_EHAsymm->Fit("pol1","","",fitmin2,fitmax2); 
  gStyle->SetOptFit(0);
  prof_EHAsymm->SetMarkerColor(kPink+1);
  prof_EHAsymm->SetLineColor(kPink+1);
  prof_EHAsymm->SetMarkerStyle(kFullCircle);
  prof_EHAsymm->SetMarkerSize(0.8);
  TF1 *f_EHAsymm = (TF1*)prof_EHAsymm->GetListOfFunctions()->FindObject("pol1");
  if(f_EHAsymm){
    f_EHAsymm->SetLineColor(kRed-4);
    f_EHAsymm->SetLineWidth(4);
    f_EHAsymm->SetRange(fitmin2,fitmax2);
    h_EHAsymm->GetXaxis()->CenterTitle(1);
    h_EHAsymm->GetYaxis()->CenterTitle(1);
    h_EHAsymm->Draw();
    prof_EHAsymm->Draw("ep same"); 
    
    latex->SetTextAlign(12);
    latex->SetTextSize(0.040);
    latex->DrawLatex(0.63,0.88,Form("#chi^{2}/ndf: %.2f/%d",f_EHAsymm->GetChisquare(),f_EHAsymm->GetNDF()));
    latex->DrawLatex(0.63,0.82,Form("p0: %.3f #pm %.3f",f_EHAsymm->GetParameter(0),f_EHAsymm->GetParError(0)));
    latex->DrawLatex(0.63,0.76,Form("p1: %.3f #pm %.3f",f_EHAsymm->GetParameter(1),f_EHAsymm->GetParError(1)));  
  }
  c1->Update(); //mandatory
  palette = (TPaletteAxis*)h_EHAsymm->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
  //palette->SetX2NDC(0.8);
  c1->Update();

  ////////////////////////////////////////////////
  c1->cd(3);
  margin = gPad->GetRightMargin();
  //gPad->SetRightMargin(margin*3);
  gPad->SetRightMargin(0.13);
  
  TProfile *prof_totEHAsymm = h_totEHAsymm->ProfileX();
  prof_totEHAsymm->Fit("pol1","","",fitmin3,fitmax3); 
  gStyle->SetOptFit(0);
  prof_totEHAsymm->SetMarkerColor(kPink+1);
  prof_totEHAsymm->SetLineColor(kPink+1);
  prof_totEHAsymm->SetMarkerStyle(kFullCircle);
  prof_totEHAsymm->SetMarkerSize(0.8);
  TF1 *f_totEHAsymm = (TF1*)prof_totEHAsymm->GetListOfFunctions()->FindObject("pol1");
  if(f_totEHAsymm){
    f_totEHAsymm->SetLineColor(kRed-4);
    f_totEHAsymm->SetLineWidth(4);
    f_totEHAsymm->SetRange(fitmin3,fitmax3);
    h_totEHAsymm->GetXaxis()->CenterTitle(1);
    h_totEHAsymm->GetYaxis()->CenterTitle(1);
    h_totEHAsymm->Draw();
    prof_totEHAsymm->Draw("ep same"); 
    
    latex->SetTextAlign(12);
    latex->SetTextSize(0.040);
    latex->DrawLatex(0.63,0.88,Form("#chi^{2}/ndf: %.2f/%d",f_totEHAsymm->GetChisquare(),f_totEHAsymm->GetNDF()));
    latex->DrawLatex(0.63,0.82,Form("p0: %.3f #pm %.3f",f_totEHAsymm->GetParameter(0),f_totEHAsymm->GetParError(0)));
    latex->DrawLatex(0.63,0.76,Form("p1: %.3f #pm %.3f",f_totEHAsymm->GetParameter(1),f_totEHAsymm->GetParError(1)));  
  }
  c1->Update(); //mandatory
  palette = (TPaletteAxis*)h_totEHAsymm->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
  c1->Modified();
  c1->Update();

  ////////////////////////////////////////////////
  c1->cd(4);
  margin = gPad->GetRightMargin();
  //gPad->SetRightMargin(margin*3);
  gPad->SetRightMargin(0.13);
  
  TProfile *prof_emfrac = h_emfrac->ProfileX();
  prof_emfrac->Fit("pol1","","",fitmin4,fitmax4); 
  gStyle->SetOptFit(0);
  prof_emfrac->SetMarkerColor(kPink+1);
  prof_emfrac->SetLineColor(kPink+1);
  prof_emfrac->SetMarkerStyle(kFullCircle);
  prof_emfrac->SetMarkerSize(0.8);
  TF1 *f_emfrac = (TF1*)prof_emfrac->GetListOfFunctions()->FindObject("pol1");
  if(f_emfrac){
    f_emfrac->SetLineColor(kRed-4);
    f_emfrac->SetLineWidth(4);
    f_emfrac->SetRange(fitmin4,fitmax4);
    h_emfrac->GetXaxis()->CenterTitle(1);
    h_emfrac->GetYaxis()->CenterTitle(1);
    h_emfrac->Draw();
    prof_emfrac->Draw("ep same"); 
    
    latex->SetTextAlign(12);
    latex->SetTextSize(0.040);
    latex->DrawLatex(0.63,0.88,Form("#chi^{2}/ndf: %.2f/%d",f_emfrac->GetChisquare(),f_emfrac->GetNDF()));
    latex->DrawLatex(0.63,0.82,Form("p0: %.3f #pm %.3f",f_emfrac->GetParameter(0),f_emfrac->GetParError(0)));
    latex->DrawLatex(0.63,0.76,Form("p1: %.3f #pm %.3f",f_emfrac->GetParameter(1),f_emfrac->GetParError(1)));  
  }
  c1->Update(); //mandatory
  palette = (TPaletteAxis*)h_emfrac->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
  c1->Update();
  
  ////////////////////////////////////////
  if (total_sf !=1) {
    c1->SaveAs(Form("outFinal%s/final2D_wTot%.0f_%s_%s_%s_%s_%.0f_%.0f_%.0f.pdf",ihcalType.c_str(),100*total_sf,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),note.c_str(),100*cemc_sf,100*ihcal_sf,100*ohcal_sf));
  } else {
    c1->SaveAs(Form("outFinal%s/final2D_%s_%s_%s_%s_%.0f_%.0f_%.0f.pdf",ihcalType.c_str(),sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),note.c_str(),100*cemc_sf,100*ihcal_sf,100*ohcal_sf));
  }
  
  ////////////////////////////////////////
  float col;
  int repeat=30;
  float fitmin_jes = 0.5;
  float fitmax_jes = 1.5;
  float mean_jes;
  float sig_jes;
  float sigErr_jes;
  float fitmin_jpts = 0.5;
  float fitmax_jpts = 1.5;
  float mean_jpts;
  float sig_jpts;
  float sigErr_jpts;
  float fitmin_jms = 0.5;
  float fitmax_jms = 1.5;
  float mean_jms;
  float sig_jms;
  float sigErr_jms;
  //float fitmin_jer = -1.0;
  //float fitmax_jer = 1.5;
  //float mean_jer;
  //float sig_jer;
  //float sigErr_jer;

  TCanvas* c2 = new TCanvas("c2","",1500,500);
  c2->Divide(3,1);
  
  c2->cd(1);
  SetHistStyle(h_JES,0,0);
  h_JES->GetXaxis()->CenterTitle(1);
  h_JES->GetYaxis()->CenterTitle(1);
  h_JES->Draw("pe");
  TF1* tf_JES= new TF1("TF_JES","gaus",fitmin_jes,fitmax_jes);
  //col = h_JES->GetMarkerColor();
  //tf_JES->SetLineColor(col);
  tf_JES->SetLineColor(kRed-4);
  recursiveFit(h_JES, tf_JES,fitmin_jes, fitmax_jes, &mean_jes, &sig_jes, &sigErr_jes);
  for (int ir=0; ir<repeat; ir++){
    fitmin_jes = mean_jes-2*sig_jes;
    fitmax_jes = mean_jes+2*sig_jes;
    recursiveFit(h_JES, tf_JES,fitmin_jes, fitmax_jes, &mean_jes, &sig_jes, &sigErr_jes);
  }
  tf_JES->Draw("same");

  c2->cd(2);
  SetHistStyle(h_JPTS,0,0);
  h_JPTS->GetXaxis()->CenterTitle(1);
  h_JPTS->GetYaxis()->CenterTitle(1);
  h_JPTS->Draw("pe");
  TF1* tf_JPTS= new TF1("TF_JPTS","gaus",fitmin_jpts,fitmax_jpts);
  //col = h_JPTS->GetMarkerColor();
  //tf_JPTS->SetLineColor(col);
  tf_JPTS->SetLineColor(kRed-4);
  recursiveFit(h_JPTS, tf_JPTS,fitmin_jpts, fitmax_jpts, &mean_jpts, &sig_jpts, &sigErr_jpts);
  for (int ir=0; ir<repeat; ir++){
    fitmin_jpts = mean_jpts-2*sig_jpts;
    fitmax_jpts = mean_jpts+2*sig_jpts;
    recursiveFit(h_JPTS, tf_JPTS,fitmin_jpts, fitmax_jpts, &mean_jpts, &sig_jpts, &sigErr_jpts);
  }
  tf_JPTS->Draw("same");
  
  c2->cd(3);
  SetHistStyle(h_JMS,0,0);
  h_JMS->GetXaxis()->CenterTitle(1);
  h_JMS->GetYaxis()->CenterTitle(1);
  h_JMS->Draw("pe");
  TF1* tf_JMS= new TF1("TF_JMS","gaus",fitmin_jms,fitmax_jms);
  //col = h_JMS->GetMarkerColor();
  //tf_JMS->SetLineColor(col);
  tf_JMS->SetLineColor(kRed-4);
  recursiveFit(h_JMS, tf_JMS,fitmin_jms, fitmax_jms, &mean_jms, &sig_jms, &sigErr_jms);
  for (int ir=0; ir<repeat; ir++){
    fitmin_jms = mean_jms-2*sig_jms;
    fitmax_jms = mean_jms+2*sig_jms;
    recursiveFit(h_JMS, tf_JMS,fitmin_jms, fitmax_jms, &mean_jms, &sig_jms, &sigErr_jms);
  }
  tf_JMS->Draw("same");

/*
  c2->cd(2);
  SetHistStyle(h_JER,0,0);
  h_JER->GetXaxis()->CenterTitle(1);
  h_JER->GetYaxis()->CenterTitle(1);
  h_JER->Draw("pe");
  TF1* tf_JER= new TF1("TF_JER","gaus",fitmin_jer,fitmax_jer);
  //col = h_JER->GetMarkerColor();
  //tf_JER->SetLineColor(col);
  tf_JER->SetLineColor(kRed-4);
  recursiveFit(h_JER, tf_JER,fitmin_jer, fitmax_jer, &mean_jer, &sig_jer, &sigErr_jer);
  for (int ir=0; ir<repeat; ir++){
    fitmin_jer = mean_jer-2*sig_jer;
    fitmax_jer = mean_jer+2*sig_jer;
    recursiveFit(h_JER, tf_JER,fitmin_jer, fitmax_jer, &mean_jer, &sig_jer, &sigErr_jer);
  }
  tf_JER->Draw("same");
*/
  
  if (total_sf !=1) {
    c2->SaveAs(Form("outFinal%s/final1D_wTot%.0f_%s_%s_%s_%s_%.0f_%.0f_%.0f.pdf",ihcalType.c_str(),100*total_sf,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),note.c_str(),100*cemc_sf,100*ihcal_sf,100*ohcal_sf));
  } else {
    c2->SaveAs(Form("outFinal%s/final1D_%s_%s_%s_%s_%.0f_%.0f_%.0f.pdf",ihcalType.c_str(),sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),note.c_str(),100*cemc_sf,100*ihcal_sf,100*ohcal_sf));
  }

  cout  << "" << endl;
  cout  << ihcalType.c_str() << " "<< jetE.c_str() << " "<<note.c_str() << endl;
  cout << "cemcScale = " << cemc_sf << endl;
  cout << "ihcalScale = " << ihcal_sf << endl;
  cout << "ohcalScale = " << ohcal_sf << endl;
  cout << "total_sf = " << total_sf << endl;
  cout << "jes_mean = " << mean_jes << endl;
  cout << "jes_sig = " << sig_jes << endl;
  cout << "jes_sig/mean = " << sig_jes/mean_jes << endl;
  //cout << "jer_mean = " << mean_jer << endl;
  //cout << "jer_sig = " << sig_jer << endl;
  cout << "jpts_mean = " << mean_jpts << endl;
  cout << "jpts_sig = " << sig_jpts << endl;
  cout << "jpts_sig/mean = " << sig_jpts/mean_jpts << endl;
  cout << "jms_mean = " << mean_jms << endl;
  cout << "jms_sig = " << sig_jms << endl;
  cout << "jms_sig/mean = " << sig_jms/mean_jms << endl;
  
  if (total_sf !=1) {
    c2->SaveAs(Form("outFinal%s/final1D_wTot%.0f_%s_%s_%s_%s_%.0f_%.0f_%.0f.pdf",ihcalType.c_str(),100*total_sf,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),note.c_str(),100*cemc_sf,100*ihcal_sf,100*ohcal_sf));
  } else {
    c2->SaveAs(Form("outFinal%s/final1D_%s_%s_%s_%s_%.0f_%.0f_%.0f.pdf",ihcalType.c_str(),sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),note.c_str(),100*cemc_sf,100*ihcal_sf,100*ohcal_sf));
  }

  TFile* fout;
  //fout = new TFile(Form("outFinal%s/final_%s_%s_%s_%s_%.0f_%.0f_%.0f.root",ihcalType.c_str(),sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),note.c_str(),100*cemc_sf,100*ihcal_sf,100*ohcal_sf),"RECREATE");
  if (cemc_sf==1 && ihcal_sf==1 && ohcal_sf==1) { fout = new TFile(Form("outFinal%s/final_%s_%s_%s_%s_before.root",ihcalType.c_str(),sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),note.c_str()),"RECREATE"); }
  else { fout = new TFile(Form("outFinal%s/final_%s_%s_%s_%s_truthjet.root",ihcalType.c_str(),sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),note.c_str()),"RECREATE"); }
  
  fout->cd();
  h_emfrac->Write();
  prof_emfrac->Write();
  f_emfrac->Write();
  h_JES->Write();
  tf_JES->Write();
  h_JPTS->Write();
  tf_JPTS->Write();
  h_JMS->Write();
  tf_JMS->Write();
  fout->Close();

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
