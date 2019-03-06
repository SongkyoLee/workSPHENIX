#include "TMinuit.h"

void recursiveFit(TH1D* t_h1, TF1* t_tf1, float t_fitmin, float t_fitmax, float* t_mean, float* t_sig, float* t_sigErr);

void fig01_jes_emfrac(int isOpt=10,
                              string sampleType = "QCDJet",
                              string ihcalType = "Alframe",
                              string jetE = "20GeV",
                              int initfile=0,
                              int endfile =1000,
                              float total_sf = 1.00,
                              float cemc_sf = 1.00,
                              float ihcal_sf = 1.00,
                              float ohcal_sf =1.00,
                              bool doetopcut=true,
                              bool dodphicut=true)
{
  if (ihcalType.compare("SS310")==0) {
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
  } else if (ihcalType.compare("Alframe")==0){   
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
    } else {
      cout << "ERROR! select proper isOpt "; return;
    }
  } else {
    cout << "ERROR! select proper ihcal "; return;
  }


  
  cemc_sf = cemc_sf*total_sf;
  if (ihcalType.compare("SS310") == 0) { ihcal_sf = ihcal_sf*total_sf; }
  ohcal_sf = ohcal_sf*total_sf;
 
  //// https://root.cern.ch/doc/master/classTColor.html 
  //gStyle->SetPalette(1);
  gStyle->SetPalette(kBird); //default
  //gStyle->SetPalette(51); //navy
  //gStyle->SetPalette(52); //black
  //gStyle->SetPalette(55); //rainbow color
  //gStyle->SetPalette(kFuchsia); //warm violet
  //gStyle->SetPalette(kLake); //cool violet
  //TColor::InvertPalette();
  
  string fname = Form("./2ndSortedRootFiles_py/2ndSorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_doetopcut%d_dodphicut%d.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),(int)doetopcut,(int)dodphicut);
  TFile* fin = new TFile( fname.c_str(), "READ");
  TTree* out_tree = (TTree*)fin->Get("out_tree");

  TH2D* h_IHAsymm = new TH2D("h_IHAsymm",";Reco Jet E_{IHCAL}-E_{OHCAL} asymmetry;Jet E_{HCAL} / (Jet E_{Truth} - Jet E_{EMCAL})",101,-1.05,1.05,100,0,2);
  TH2D* h_EHAsymm = new TH2D("h_EHAsymm",";Reco Jet E_{EMCAL}^{had}-E_{HCAL} asymmetry;(Jet E_{HCAL}+Jet E_{EMCAL}^{had}) / (Jet E_{Truth} - Jet E_{EMCAL}^{EM})",101,-1.05,1.05,100,0,2);
  TH2D* h_totEHAsymm = new TH2D("h_totEHAsymm",";Reco Jet E_{EMCAL}-E_{HCAL} asymmetry;Jet E_{Reco} / Jet E_{Truth}",101,-1.05,1.05,100,0,2);
  //TH2D* h_emfrac = new TH2D("h_emfrac",";Truth EM fraction;Jet E_{Reco} / Jet E_{Truth}",101,-0.05,1.05,100,0,2);
  //TH2D* h_emfrac = new TH2D("h_emfrac",";Fraction of EM energy in truth jet;E_{Jet}^{Reco} / E_{Jet}^{Truth}",101,-0.05,1.05,100,0,2);
  TH2D* h_emfrac = new TH2D("h_emfrac",";Fraction of EM energy in truth jet;E_{Jet}^{Reco} / E_{Jet}^{Truth}",101,-0.05,1.05,100,0,2.8);
  
  TH1D* h_JES = new TH1D("h_JES",";Jet E_{Reco} / Jet E_{Truth};Counts",100,0,2);
  TH1D* h_JER = new TH1D("h_JER",";(Jet E_{Reco}-Jet E_{Truth}) / Jet E_{Reco};Counts",100,-1,1);
  
  int evt;
  float true_jet_e;
  float true_jet_emfrac;
  float reco_jet_clcemc_hadEsum;
  float reco_jet_clcemc_emEsum;
  float reco_jet_ihcalEsum;
  float reco_jet_ohcalEsum;
  out_tree->SetBranchAddress("evt",&evt);
  out_tree->SetBranchAddress("true_jet_e",&true_jet_e);
  out_tree->SetBranchAddress("true_jet_emfrac",&true_jet_emfrac);
  out_tree->SetBranchAddress("reco_jet_clcemc_hadEsum",&reco_jet_clcemc_hadEsum);
  out_tree->SetBranchAddress("reco_jet_clcemc_emEsum",&reco_jet_clcemc_emEsum);
  out_tree->SetBranchAddress("reco_jet_ihcalEsum",&reco_jet_ihcalEsum);
  out_tree->SetBranchAddress("reco_jet_ohcalEsum",&reco_jet_ohcalEsum);


  float reco_jet_caliE;
  float ihasymm, ihfrac;
  float ehasymm, ehfrac;
  float totehasymm, totehfrac;
  
  int nevt = out_tree->GetEntries();
  //int nevt = 500;
  cout << "nevt = " << nevt << endl;

  for (int ievt=0; ievt <nevt; ievt++){
    out_tree->GetEvent(ievt);
    
    //// scaling
    reco_jet_clcemc_hadEsum = reco_jet_clcemc_hadEsum*cemc_sf;
    reco_jet_ihcalEsum = reco_jet_ihcalEsum*ihcal_sf;
    reco_jet_ohcalEsum = reco_jet_ohcalEsum*ohcal_sf;
    
    //// calculation
    ihasymm = ( reco_jet_ihcalEsum - reco_jet_ohcalEsum ) / ( reco_jet_ihcalEsum + reco_jet_ohcalEsum );
    ihfrac = ( reco_jet_ihcalEsum + reco_jet_ohcalEsum ) / ( true_jet_e - (reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum) );
    
    ehasymm = ( reco_jet_clcemc_hadEsum - (reco_jet_ihcalEsum + reco_jet_ohcalEsum) ) / ( reco_jet_clcemc_hadEsum + (reco_jet_ihcalEsum + reco_jet_ohcalEsum) );
    ehfrac = ( reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum ) / ( true_jet_e - reco_jet_clcemc_emEsum );

    totehasymm = ( (reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum) - (reco_jet_ihcalEsum + reco_jet_ohcalEsum) ) / ( (reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum) + (reco_jet_ihcalEsum + reco_jet_ohcalEsum) );
    totehfrac = ( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum ) / ( true_jet_e );
    
    h_IHAsymm -> Fill(ihasymm,ihfrac); 
    h_EHAsymm -> Fill(ehasymm,ehfrac); 
    h_totEHAsymm -> Fill(totehasymm,totehfrac); 
    //h_emfrac -> Fill(true_jet_emfrac,( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum )/true_jet_e);
    //if ( ( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum )/true_jet_e < 1.45 ) h_emfrac -> Fill(true_jet_emfrac,( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum )/true_jet_e);
    if ( ( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum )/true_jet_e < 2.0 ) h_emfrac -> Fill(true_jet_emfrac,( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum )/true_jet_e);
    h_JES->Fill(( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum )/true_jet_e);
    h_JER->Fill(( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum - true_jet_e)/ ( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum ));
  }
 
  h_IHAsymm->SetOption("colz");
  h_EHAsymm->SetOption("colz");
  h_totEHAsymm->SetOption("colz");
  h_emfrac->SetOption("colz");

  TCanvas* c1 = new TCanvas("c1","",800,600);
  c1->cd();
  //h_emfrac->SetMaximum(110);
  cout << h_emfrac->GetYaxis()->GetTitleOffset()<<endl;
  h_emfrac->GetYaxis()->SetTitleOffset(1.2);
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
  c1->cd(4);
  margin = gPad->GetRightMargin();
  //gPad->SetRightMargin(margin*3);
  gPad->SetRightMargin(0.13);
  
  TProfile *prof_emfrac = h_emfrac->ProfileX();
  prof_emfrac->Fit("pol1","","",fitmin4,fitmax4); 
  gStyle->SetOptFit(0);
  //prof_emfrac->SetMarkerColor(kCyan+1);
  //prof_emfrac->SetLineColor(kCyan+1);
  prof_emfrac->SetMarkerColor(kMagenta-9);
  prof_emfrac->SetLineColor(kMagenta-9);
  prof_emfrac->SetMarkerStyle(kFullCircle);
  prof_emfrac->SetMarkerSize(1.2);
  TF1 *f_emfrac = (TF1*)prof_emfrac->GetListOfFunctions()->FindObject("pol1");
  if(f_emfrac){
    f_emfrac->SetLineColor(kRed+1);
    //f_emfrac->SetLineColor(kYellow-4);
    //f_emfrac->SetLineWidth(4);
    f_emfrac->SetLineWidth(5);
    f_emfrac->SetRange(fitmin4,fitmax4);
    h_emfrac->GetXaxis()->CenterTitle(1);
    h_emfrac->GetYaxis()->CenterTitle(1);
    h_emfrac->Draw();
    prof_emfrac->Draw("ep same"); 
    
    latex->SetTextAlign(12);
    latex->SetTextSize(0.040);
    //latex->DrawLatex(0.63,0.88,Form("#chi^{2}/ndf: %.2f/%d",f_emfrac->GetChisquare(),f_emfrac->GetNDF()));
    //latex->DrawLatex(0.63,0.82,Form("a: %.3f #pm %.3f",f_emfrac->GetParameter(0),f_emfrac->GetParError(0)));
    //latex->DrawLatex(0.63,0.76,Form("b: %.3f #pm %.3f",f_emfrac->GetParameter(1),f_emfrac->GetParError(1)));  
  }
  c1->Update(); //mandatory
  palette = (TPaletteAxis*)h_emfrac->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
  c1->Update();
  
  //TLatex* latex = new TLatex();
  //latex->SetNDC();
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  //latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->DrawLatex(0.365,0.89,"MIE 2018");
  latex->SetTextSize(0.040);
  //latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4, |#eta_{Jet}^{truth}| < 0.45");
  //latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4,");
  //latex->DrawLatex(0.2,0.77,"|#eta_{Jet}^{truth}| < 0.45, 20 <|E_{Jet}^{truth}| < 30 GeV");
  latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, R=0.4");
  latex->DrawLatex(0.2,0.76,"20 <E_{Jet}^{Truth} < 30 GeV");
  
  TLegend* leg1 = new TLegend(0.61, 0.81, 0.73, 0.91);
  leg1->SetTextSize(0.043);
  leg1->AddEntry(prof_emfrac," Profile","p");
  leg1->AddEntry(f_emfrac," y = a + bx","l");
  leg1->Draw();
    
  //latex->SetTextAlign(12);
  latex->SetTextSize(0.035);
  //latex->DrawLatex(0.63,0.88,Form("#chi^{2}/ndf: %.2f/%d",f_emfrac->GetChisquare(),f_emfrac->GetNDF()));
  latex->DrawLatex(0.65,0.78,Form("a: %.3f #pm %.3f",f_emfrac->GetParameter(0),f_emfrac->GetParError(0)));
  latex->DrawLatex(0.65,0.74,Form("b: %.3f #pm %.3f",f_emfrac->GetParameter(1),f_emfrac->GetParError(1)));  
 
  if (isOpt==10){
    c1->SaveAs("out_figs/hcal_jes_emfrac.pdf");
  } else {
    c1->SaveAs("out_figs/hcal_jes_emfrac_calib.pdf");
  }

 /* 
  ////////////////////////////////////////
  float col;
  int repeat=30;
  float fitmin_jes = 0.5;
  float fitmax_jes = 1.5;
  float mean_jes;
  float sig_jes;
  float sigErr_jes;
  float fitmin_jer = -1.0;
  float fitmax_jer = 1.5;
  float mean_jer;
  float sig_jer;
  float sigErr_jer;

  TCanvas* c2 = new TCanvas("c2","",1000,500);
  c2->Divide(2,1);
  
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
  
  if (total_sf !=1) {
    c2->SaveAs(Form("outFinal%s/final1D_wTot%.0f_%s_%s_%s_%.0f_%.0f_%.0f.pdf",ihcalType.c_str(),100*total_sf,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),100*cemc_sf,100*ihcal_sf,100*ohcal_sf));
  } else {
    c2->SaveAs(Form("outFinal%s/final1D_%s_%s_%s_%.0f_%.0f_%.0f.pdf",ihcalType.c_str(),sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),100*cemc_sf,100*ihcal_sf,100*ohcal_sf));
  }

  cout  << "" << endl;
  cout  << ihcalType.c_str() << " "<< jetE.c_str() << endl;
  cout << "cemcScale = " << cemc_sf << endl;
  cout << "ihcalScale = " << ihcal_sf << endl;
  cout << "ohcalScale = " << ohcal_sf << endl;
  cout << "total_sf = " << total_sf << endl;
  cout << "jes_mean = " << mean_jes << endl;
  cout << "jes_sig = " << sig_jes << endl;
  cout << "jes_sig/mean = " << sig_jes/mean_jes << endl;
  cout << "jer_mean = " << mean_jer << endl;
  cout << "jer_sig = " << sig_jer << endl;
*/
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
