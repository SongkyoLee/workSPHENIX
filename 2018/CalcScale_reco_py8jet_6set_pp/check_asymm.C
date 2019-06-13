#include "TMinuit.h"

void recursiveFit(TH1D* t_h1, TF1* t_tf1, float t_fitmin, float t_fitmax, float* t_mean, float* t_sig, float* t_sigErr);

void check_asymm(string sampleType = "QCDJet",
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

  TH2D* h_emfrac = new TH2D("h_emfrac",";Truth EM fraction;E_{Jet}^{Reco} / E_{Jet}^{Truth}",101,-0.05,1.05,100,0,2);
  TH2D* h_emfrac_low = new TH2D("h_emfrac_low",";Truth EM fraction;E_{Jet}^{Reco} / E_{Jet}^{Truth}",101,-0.05,1.05,100,0,2);
  TH2D* h_emfrac_high = new TH2D("h_emfrac_high",";Truth EM fraction;E_{Jet}^{Reco} / E_{Jet}^{Truth}",101,-0.05,1.05,100,0,2);
 
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
    true_jet_m = sqrt(pow(true_jet_e,2)-pow(true_jet_p,2));
    reco_jet_m = sqrt(pow(reco_jet_e,2)-pow(reco_jet_p,2));
     
    h_emfrac -> Fill(true_jet_emfrac,( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum )/true_jet_e);
    
  }
 
  h_emfrac->SetOption("colz");
  h_emfrac_low->SetOption("colz");
  h_emfrac_high->SetOption("colz");

  TCanvas* c1 = new TCanvas("c1","",1200,800);
  c1->Divide(2,2);
  c1->cd(1);
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
  
  int entries = h_emfrac->GetEntries();
  cout << "entries = " << entries << endl;
  myLatex(0.2,0.88,Form("Entries = %d",entries));

  //////// calculate low and high

  //for (int ievt=0; ievt <10; ievt++){
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
    double tmpy = f_emfrac->Eval(true_jet_emfrac);
    //cout << tmpy << endl;
    if ( ( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum )/true_jet_e < tmpy) {
      h_emfrac_low -> Fill(true_jet_emfrac,( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum )/true_jet_e);
    }
    else {
      h_emfrac_high -> Fill(true_jet_emfrac,( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum )/true_jet_e);
    }
  } 
  
  c1->cd(3);
  margin = gPad->GetRightMargin();
  //gPad->SetRightMargin(margin*3);
  gPad->SetRightMargin(0.13);
  h_emfrac_low->GetXaxis()->CenterTitle(1);
  h_emfrac_low->GetYaxis()->CenterTitle(1);
  h_emfrac_low->Draw();
  int entries_low = h_emfrac_low->GetEntries();
  cout << "entries_low = " << entries_low << endl;
  myLatex(0.2,0.88,Form("Entries = %d (%.0f %%)",entries_low, (double)100*entries_low/entries));

  c1->cd(4);
  margin = gPad->GetRightMargin();
  //gPad->SetRightMargin(margin*3);
  gPad->SetRightMargin(0.13);
  h_emfrac_high->GetXaxis()->CenterTitle(1);
  h_emfrac_high->GetYaxis()->CenterTitle(1);
  h_emfrac_high->Draw();
  int entries_high = h_emfrac_high->GetEntries();
  cout << "entries_high = " << entries_high << endl;
  myLatex(0.2,0.88,Form("Entries = %d (%.0f %%)",entries_high, (double)100*entries_high/entries));
  
  ////////////////////////////////////////
  if (total_sf !=1) {
//    c1->SaveAs(Form("outFinal%s/final2D_wTot%.0f_%s_%s_%s_%s_%.0f_%.0f_%.0f.pdf",ihcalType.c_str(),100*total_sf,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),note.c_str(),100*cemc_sf,100*ihcal_sf,100*ohcal_sf));
  } else {
//    c1->SaveAs(Form("outFinal%s/final2D_%s_%s_%s_%s_%.0f_%.0f_%.0f.pdf",ihcalType.c_str(),sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),note.c_str(),100*cemc_sf,100*ihcal_sf,100*ohcal_sf));
  }
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
