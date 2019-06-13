#include "TMinuit.h"

void recursiveFit(TH1D* t_h1, TF1* t_tf1, float t_fitmin, float t_fitmax, float* t_mean, float* t_sig, float* t_sigErr);

void check_compartment(string sampleType = "QCDJet",
                              string ihcalType = "SS310",
                              string jetE = "20GeV",
                              string note = "R04",
                              int initfile=0,
                              int endfile =400,
                              float total_sf = 1.00,
                              //float cemc_sf = 1.00,
                              //float ihcal_sf = 1.00,
                              //float ohcal_sf = 1.00,
                              float cemc_sf = 1.20972,
                              float ihcal_sf = 2.13263,
                              float ohcal_sf = 1.37705,
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

  double tmpymax = 1.5;
  //double tmpymax = 2.0;
  //TH2D* h_emfrac_cemc = new TH2D("h_emfrac_cemc",";Truth EM fraction;E_{Jet}^{Reco} / E_{Jet}^{Truth}",101,-0.05,1.05,100,0,2);
  //TH2D* h_emfrac_ihcal = new TH2D("h_emfrac_ihcal",";Truth EM fraction;E_{Jet}^{Reco} / E_{Jet}^{Truth}",101,-0.05,1.05,100,0,2);
  //TH2D* h_emfrac_ohcal = new TH2D("h_emfrac_ohcal",";Truth EM fraction;E_{Jet}^{Reco} / E_{Jet}^{Truth}",101,-0.05,1.05,100,0,2);
  TH2D* h_emfrac = new TH2D("h_emfrac",";Truth EM fraction;E_{Jet}^{Reco} / E_{Jet}^{Truth}",101,-0.05,1.05,100,0,tmpymax);
  TH2D* h_emfrac_cemc = new TH2D("h_emfrac_cemc",";Truth EM fraction;E_{Jet}^{Reco} / E_{Jet}^{Truth}",101,-0.05,1.05,100,0,tmpymax);
  TH2D* h_emfrac_clcemc = new TH2D("h_emfrac_clcemc",";Truth EM fraction;E_{Jet}^{Reco} / E_{Jet}^{Truth}",101,-0.05,1.05,100,0,tmpymax);
  TH2D* h_emfrac_clcemc_em = new TH2D("h_emfrac_clcemc_em",";Truth EM fraction;E_{Jet}^{Reco} / E_{Jet}^{Truth}",101,-0.05,1.05,100,0,tmpymax);
  TH2D* h_emfrac_clcemc_had = new TH2D("h_emfrac_clcemc_had",";Truth EM fraction;E_{Jet}^{Reco} / E_{Jet}^{Truth}",101,-0.05,1.05,100,0,tmpymax);
  TH2D* h_emfrac_ihcal = new TH2D("h_emfrac_ihcal",";Truth EM fraction;E_{Jet}^{Reco} / E_{Jet}^{Truth}",101,-0.05,1.05,100,0,tmpymax);
  TH2D* h_emfrac_ohcal = new TH2D("h_emfrac_ohcal",";Truth EM fraction;E_{Jet}^{Reco} / E_{Jet}^{Truth}",101,-0.05,1.05,100,0,tmpymax);
 
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
    h_emfrac_cemc -> Fill(true_jet_emfrac,( reco_jet_cemcEsum )/true_jet_e);
    h_emfrac_clcemc -> Fill(true_jet_emfrac,( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum  )/true_jet_e);
    h_emfrac_clcemc_em -> Fill(true_jet_emfrac,( reco_jet_clcemc_emEsum )/true_jet_e);
    h_emfrac_clcemc_had -> Fill(true_jet_emfrac,( reco_jet_clcemc_hadEsum  )/true_jet_e);
    h_emfrac_ihcal -> Fill(true_jet_emfrac,( reco_jet_ihcalEsum )/true_jet_e);
    h_emfrac_ohcal -> Fill(true_jet_emfrac,( reco_jet_ohcalEsum )/true_jet_e);
    
  }
 
  h_emfrac->SetOption("colz");
  h_emfrac_cemc->SetOption("colz");
  h_emfrac_clcemc->SetOption("colz");
  h_emfrac_clcemc_em->SetOption("colz");
  h_emfrac_clcemc_had->SetOption("colz");
  h_emfrac_ihcal->SetOption("colz");
  h_emfrac_ohcal->SetOption("colz");

  TCanvas* c1 = new TCanvas("c1","",1200,400);
  c1->Divide(2,1);

  TPaletteAxis *palette;
  double margin;
  double x1ndc;
  double x2ndc;

  //////////////////////////////////////////////////////
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(32);
  latex->SetTextSize(0.045);
     
  ////////////////////////////////////////////////
  c1->cd(1);
  margin = gPad->GetRightMargin();
  //gPad->SetRightMargin(margin*3);
  gPad->SetRightMargin(0.13);
  h_emfrac->Draw();
  int entries = h_emfrac->GetEntries();
  cout << "entries = " << entries << endl;
  //myLatex(0.2,0.88,Form("Entries = %d",entries));
/* 
  c1->cd(1);
  margin = gPad->GetRightMargin();
  //gPad->SetRightMargin(margin*3);
  gPad->SetRightMargin(0.13);
  h_emfrac_cemc->Draw();
  int entries_cemc = h_emfrac_cemc->GetEntries();
  cout << "entries_cemc = " << entries_cemc << endl;
  //myLatex(0.2,0.88,Form("Entries = %d",entries_cemc));
*/  
  c1->cd(2);
  margin = gPad->GetRightMargin();
  //gPad->SetRightMargin(margin*3);
  gPad->SetRightMargin(0.13);
  h_emfrac_clcemc->Draw();
  int entries_clcemc = h_emfrac_clcemc->GetEntries();
  cout << "entries_clcemc = " << entries_clcemc << endl;
  //myLatex(0.2,0.88,Form("Entries = %d",entries_clcemc));
  
  TCanvas* c2 = new TCanvas("c2","",1200,800);
  c2->Divide(2,2);
  
  c2->cd(1);
  margin = gPad->GetRightMargin();
  //gPad->SetRightMargin(margin*3);
  gPad->SetRightMargin(0.13);
  h_emfrac_clcemc_em->Draw();
  int entries_clcemc_em = h_emfrac_clcemc_em->GetEntries();
  cout << "entries_clcemc_em = " << entries_clcemc_em << endl;
  //myLatex(0.2,0.88,Form("Entries = %d",entries_clcemc_had));
 
  c2->cd(2);
  margin = gPad->GetRightMargin();
  //gPad->SetRightMargin(margin*3);
  gPad->SetRightMargin(0.13);
  h_emfrac_clcemc_had->Draw();
  int entries_clcemc_had = h_emfrac_clcemc_had->GetEntries();
  cout << "entries_clcemc_had = " << entries_clcemc_had << endl;
  //myLatex(0.2,0.88,Form("Entries = %d",entries_clcemc_had));

  c2->cd(3);
  margin = gPad->GetRightMargin();
  //gPad->SetRightMargin(margin*3);
  gPad->SetRightMargin(0.13);
  h_emfrac_ihcal->Draw();
  int entries_ihcal = h_emfrac_ihcal->GetEntries();
  cout << "entries_ihcal = " << entries_ihcal << endl;
  //myLatex(0.2,0.88,Form("Entries = %d (%.0f %%)",entries_ihcal, (double)100*entries_ihcal/entries));

  c2->cd(4);
  margin = gPad->GetRightMargin();
  //gPad->SetRightMargin(margin*3);
  gPad->SetRightMargin(0.13);
  h_emfrac_ohcal->Draw();
  int entries_ohcal = h_emfrac_ohcal->GetEntries();
  cout << "entries_ohcal = " << entries_ohcal << endl;
  //myLatex(0.2,0.88,Form("Entries = %d (%.0f %%)",entries_ohcal, (double)100*entries_ohcal/entries));
  
  ////////////////////////////////////////
  if (total_sf !=1) {
//    c2->SaveAs(Form("outFinal%s/final2D_wTot%.0f_%s_%s_%s_%s_%.0f_%.0f_%.0f.pdf",ihcalType.c_str(),100*total_sf,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),note.c_str(),100*cemc_sf,100*ihcal_sf,100*ohcal_sf));
  } else {
//    c2->SaveAs(Form("outFinal%s/final2D_%s_%s_%s_%s_%.0f_%.0f_%.0f.pdf",ihcalType.c_str(),sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),note.c_str(),100*cemc_sf,100*ihcal_sf,100*ohcal_sf));
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
