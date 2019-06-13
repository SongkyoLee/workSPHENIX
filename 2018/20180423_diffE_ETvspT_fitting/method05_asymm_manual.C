#include "TMinuit.h"

void recursiveFit(TH1D* t_h1, TF1* t_tf1, float t_fitmin, float t_fitmax, float* t_mean, float* t_sig, float* t_sigErr);

void method05_asymm_manual(string sampleType = "GammaJet",
                              string ihcalType = "Alframe",
                              string jetE = "20GeV",
                              int initfile=0,
                              int endfile =1000,
                              bool doetopcut=true,
                              bool dodphicut=true)
{
  //// E = E_cemc^em + c_a ( c_b1 * E_cemc + c_b2 ( c_c * E_ihcal + E_ohcal) )
  //float cemc_sf = c_a*c_b1;
  //float ihcal_sf = c_a*c_b2*c_c;
  //float ohcal_sf = c_a*c_b2;
  float c_a = 1.00;
  float c_b1 = 1.00;
  float c_b2 = 1.00;
  float c_c = 1.00;

  //// https://root.cern.ch/doc/master/classTColor.html 
  //gStyle->SetPalette(kBird); //default
  gStyle->SetPalette(52); //black
  //gStyle->SetPalette(55); //rainbow color
  //gStyle->SetPalette(kFuchsia); //warm violet
  //gStyle->SetPalette(kLake); //cool violet
  //TColor::InvertPalette(); 
  
  //string fname = Form("./2ndSortedRootFiles/2ndSorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_doetopcut%d_dodphicut%d.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),(int)doetopcut,(int)dodphicut);
  string fname = Form("./2ndSortedRootFiles/2ndSorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_doetopcut%d_dodphicut%d_pt.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),(int)doetopcut,(int)dodphicut);
  TFile* fin = new TFile( fname.c_str(), "READ");
  TTree* out_tree = (TTree*)fin->Get("out_tree");

  TH2D* h_origIHAsymm = new TH2D("h_origIHAsymm",";Reco Jet E_{IHCAL}-E_{OHCAL} asymmetry;Jet E_{HCAL} / (#gamma E_{Reco} - Jet E_{EMCAL})",101,-1.05,1.05,100,0,2);
  TH2D* h_origEHAsymm = new TH2D("h_origEHAsymm",";Reco Jet E_{EMCAL}^{had}-E_{HCAL} asymmetry;(Jet E_{HCAL}+Jet E_{EMCAL}^{had}) / (#gamma E_{Reco} - Jet E_{EMCAL}^{EM})",101,-1.05,1.05,100,0,2);
  TH2D* h_origtotEHAsymm = new TH2D("h_origtotEHAsymm",";Reco Jet E_{EMCAL}-E_{HCAL} asymmetry;Jet E_{Reco} / #gamma E_{Reco}",101,-1.05,1.05,100,0,2);
  TH2D* h_origemfrac = new TH2D("h_origemfrac",";Truth EM fraction;Jet E_{Reco} / Jet E_{Truth}",101,-0.05,1.05,100,0,2);
  TH2D* h_origJtoG_emfrac = new TH2D("h_origJtoG_emfrac",";Truth EM fraction;Jet E_{Reco} / #gamma E_{Reco}",101,-0.05,1.05,100,0,2);
  TH1D* h_origJtoG_had = new TH1D("h_origJtoG_had",";(Jet E_{HCAL}+Jet E_{EMCAL}^{had}) / (#gamma E_{Reco} - Jet E_{EMCAL}^{EM});Counts",100,0,2);
  
  TH2D* h_IHAsymm = new TH2D("h_IHAsymm",";Reco Jet E_{IHCAL}-E_{OHCAL} asymmetry;Jet E_{HCAL} / (#gamma E_{Reco} - Jet E_{EMCAL})",101,-1.05,1.05,100,0,2);
  TH2D* h_EHAsymm = new TH2D("h_EHAsymm",";Reco Jet E_{EMCAL}^{had}-E_{HCAL} asymmetry;(Jet E_{HCAL}+Jet E_{EMCAL}^{had}) / (#gamma E_{Reco} - Jet E_{EMCAL}^{EM})",101,-1.05,1.05,100,0,2);
  TH2D* h_totEHAsymm = new TH2D("h_totEHAsymm",";Reco Jet E_{EMCAL}-E_{HCAL} asymmetry;Jet E_{Reco} / #gamma E_{Reco}",101,-1.05,1.05,100,0,2);
  TH2D* h_emfrac = new TH2D("h_emfrac",";Truth EM fraction;Jet E_{Reco} / Jet E_{Truth}",101,-0.05,1.05,100,0,2);
  TH2D* h_JtoG_emfrac = new TH2D("h_JtoG_emfrac",";Truth EM fraction;Jet E_{Reco} / #gamma E_{Reco}",101,-0.05,1.05,100,0,2);
  TH1D* h_JtoG_had = new TH1D("h_JtoG_had",";(Jet E_{HCAL}+Jet E_{EMCAL}^{had}) / (#gamma E_{Reco} - Jet E_{EMCAL}^{EM});Counts",100,0,2);
  
  h_origIHAsymm->SetOption("colz");
  h_origEHAsymm->SetOption("colz");
  h_origtotEHAsymm->SetOption("colz");
  h_origemfrac->SetOption("colz");
  h_origJtoG_emfrac->SetOption("colz");
  h_IHAsymm->SetOption("colz");
  h_EHAsymm->SetOption("colz");
  h_totEHAsymm->SetOption("colz");
  h_emfrac->SetOption("colz");
  h_JtoG_emfrac->SetOption("colz");

  
  int evt;
  float true_gamma_e;
  float true_jet_e;
  float true_jet_emfrac;
  float reco_gamma_cemcEsum;
  float reco_jet_clcemc_hadEsum;
  float reco_jet_clcemc_emEsum;
  float reco_jet_ihcalEsum;
  float reco_jet_ohcalEsum;
  out_tree->SetBranchAddress("evt",&evt);
  out_tree->SetBranchAddress("true_gamma_e",&true_gamma_e);
  out_tree->SetBranchAddress("true_jet_e",&true_jet_e);
  out_tree->SetBranchAddress("true_jet_emfrac",&true_jet_emfrac);
  out_tree->SetBranchAddress("reco_gamma_cemcEsum",&reco_gamma_cemcEsum);
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
  
  /////////////////////////////////////////////////////////////////////////
  ////// STEP01) c_c
  /////////////////////////////////////////////////////////////////////////

  for (int ievt=0; ievt <nevt; ievt++){
    out_tree->GetEvent(ievt);
    
    //// calculation
    ihasymm = ( reco_jet_ihcalEsum - reco_jet_ohcalEsum ) / ( reco_jet_ihcalEsum + reco_jet_ohcalEsum );
    ihfrac = ( reco_jet_ihcalEsum + reco_jet_ohcalEsum ) / ( reco_gamma_cemcEsum - (reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum) );
    
    //// fill
    h_origIHAsymm -> Fill(ihasymm,ihfrac); 
    h_origemfrac -> Fill(true_jet_emfrac,( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum )/true_jet_e);
    h_origJtoG_emfrac -> Fill(true_jet_emfrac,( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum )/reco_gamma_cemcEsum);
  }
 
  TCanvas* c1 = new TCanvas("c1","",1200,800);
  c1->Divide(2,2);
  /*
  c1->cd(1);
  h_origIHAsymm->Draw();
  c1->cd(2);
  h_origEHAsymm->Draw();
  c1->cd(3);
  h_origtotEHAsymm->Draw();
  c1->cd(4);
  h_origemfrac->Draw();
*/
  TPaletteAxis *palette;
  double margin;
  double x1ndc;
  double x2ndc;
  
  float yval_min;
  float yval_max;

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
  
  c1->cd(1);
  h_origIHAsymm->Draw();
  margin = gPad->GetRightMargin();
  //gPad->SetRightMargin(margin*3);
  gPad->SetRightMargin(0.13);
  
  TProfile *prof_origIHAsymm = h_origIHAsymm->ProfileX();
  prof_origIHAsymm->Fit("pol1","","",fitmin,fitmax); 
  gStyle->SetOptFit(0);
  //gStyle->SetOptFit(1);
  //prof_origIHAsymm->SetMarkerColor(kYellow-7);
  prof_origIHAsymm->SetMarkerColor(kCyan+1);
  prof_origIHAsymm->SetLineColor(kCyan+1);
  prof_origIHAsymm->SetMarkerSize(1.);
  TF1 *f_origIHAsymm = (TF1*)prof_origIHAsymm->GetListOfFunctions()->FindObject("pol1");
  if(f_origIHAsymm){
    f_origIHAsymm->SetLineColor(kRed-4);
    //f_origIHAsymm->SetLineWidth(5);
    f_origIHAsymm->SetLineWidth(4);
    f_origIHAsymm->SetRange(fitmin,fitmax);
    h_origIHAsymm->GetXaxis()->CenterTitle(1);
    h_origIHAsymm->GetYaxis()->CenterTitle(1);
    h_origIHAsymm->Draw();
    prof_origIHAsymm->Draw("ep same"); 
    
    latex->SetTextAlign(12);
    latex->SetTextSize(0.040);
    latex->DrawLatex(0.63,0.88,Form("#chi^{2}/ndf: %.2f/%d",f_origIHAsymm->GetChisquare(),f_origIHAsymm->GetNDF()));
    latex->DrawLatex(0.63,0.82,Form("p0: %.3f #pm %.3f",f_origIHAsymm->GetParameter(0),f_origIHAsymm->GetParError(0)));
    latex->DrawLatex(0.63,0.76,Form("p1: %.3f #pm %.3f",f_origIHAsymm->GetParameter(1),f_origIHAsymm->GetParError(1)));  
  }
  c1->Update(); //mandatory
  palette = (TPaletteAxis*)h_origIHAsymm->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
//  palette->SetX1NDC(1-margin*4);
//  palette->SetX2NDC(1-margin*3);
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
  c1->Update();
  
  //////// manual
  yval_min = f_origIHAsymm->Eval(-1);
  yval_max = f_origIHAsymm->Eval(+1);
  cout << "yval_min = " << yval_min << endl;
  cout << "yval_max = " << yval_max << endl;
  c_c = yval_min/yval_max;
  if (ihcalType.compare("Alframe")==0) { c_c = 1; }
  cout << "c_c = " << c_c<< endl;
  /////////////////////////////////////////////////////////////////////////
  ////// STEP02) c_b1, c_b2
  /////////////////////////////////////////////////////////////////////////

  for (int ievt=0; ievt <nevt; ievt++){
    out_tree->GetEvent(ievt);
    
    //// scaling
    reco_jet_ihcalEsum = reco_jet_ihcalEsum*c_c;
    
    //// calculation
    ihasymm = ( reco_jet_ihcalEsum - reco_jet_ohcalEsum ) / ( reco_jet_ihcalEsum + reco_jet_ohcalEsum );
    ihfrac = ( reco_jet_ihcalEsum + reco_jet_ohcalEsum ) / ( reco_gamma_cemcEsum - (reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum) );
    ehasymm = ( reco_jet_clcemc_hadEsum - (reco_jet_ihcalEsum + reco_jet_ohcalEsum) ) / ( reco_jet_clcemc_hadEsum + (reco_jet_ihcalEsum + reco_jet_ohcalEsum) );
    ehfrac = ( reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum ) / ( reco_gamma_cemcEsum - reco_jet_clcemc_emEsum );
    
    //// fill
    h_IHAsymm -> Fill(ihasymm,ihfrac); 
    h_origEHAsymm -> Fill(ehasymm,ehfrac); 
  }

  c1->cd(2);
  h_IHAsymm->Draw();
  margin = gPad->GetRightMargin();
  //gPad->SetRightMargin(margin*3);
  gPad->SetRightMargin(0.13);
  
  TProfile *prof_IHAsymm = h_IHAsymm->ProfileX();
  prof_IHAsymm->Fit("pol1","","",fitmin,fitmax); 
  gStyle->SetOptFit(0);
  //gStyle->SetOptFit(1);
  //prof_IHAsymm->SetMarkerColor(kYellow-7);
  prof_IHAsymm->SetMarkerColor(kCyan+1);
  prof_IHAsymm->SetLineColor(kCyan+1);
  prof_IHAsymm->SetMarkerSize(1.);
  TF1 *f_IHAsymm = (TF1*)prof_IHAsymm->GetListOfFunctions()->FindObject("pol1");
  if(f_IHAsymm){
    f_IHAsymm->SetLineColor(kRed-4);
    //f_IHAsymm->SetLineWidth(5);
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

  c1->cd(3);
  h_origEHAsymm->Draw();
  margin = gPad->GetRightMargin();
  //gPad->SetRightMargin(margin*3);
  gPad->SetRightMargin(0.13);
  
  TProfile *prof_origEHAsymm = h_origEHAsymm->ProfileX();
  prof_origEHAsymm->Fit("pol1","","",fitmin2,fitmax2); 
  gStyle->SetOptFit(0);
  prof_origEHAsymm->SetMarkerColor(kCyan+1);
  prof_origEHAsymm->SetLineColor(kCyan+1);
  prof_origEHAsymm->SetMarkerSize(1.);
  TF1 *f_origEHAsymm = (TF1*)prof_origEHAsymm->GetListOfFunctions()->FindObject("pol1");
  if(f_origEHAsymm){
    f_origEHAsymm->SetLineColor(kRed-4);
    f_origEHAsymm->SetLineWidth(4);
    f_origEHAsymm->SetRange(fitmin2,fitmax2);
    h_origEHAsymm->GetXaxis()->CenterTitle(1);
    h_origEHAsymm->GetYaxis()->CenterTitle(1);
    h_origEHAsymm->Draw();
    prof_origEHAsymm->Draw("ep same"); 
    
    latex->SetTextAlign(12);
    latex->SetTextSize(0.040);
    latex->DrawLatex(0.63,0.88,Form("#chi^{2}/ndf: %.2f/%d",f_origEHAsymm->GetChisquare(),f_origEHAsymm->GetNDF()));
    latex->DrawLatex(0.63,0.82,Form("p0: %.3f #pm %.3f",f_origEHAsymm->GetParameter(0),f_origEHAsymm->GetParError(0)));
    latex->DrawLatex(0.63,0.76,Form("p1: %.3f #pm %.3f",f_origEHAsymm->GetParameter(1),f_origEHAsymm->GetParError(1)));  
  }
  c1->Update(); //mandatory
  palette = (TPaletteAxis*)h_origEHAsymm->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
//  palette->SetX1NDC(1-margin*4);
//  palette->SetX2NDC(1-margin*3);
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
  c1->Update(); 
  
  //////// manual
  yval_min = f_origEHAsymm->Eval(-1);
  yval_max = f_origEHAsymm->Eval(+1);
  cout << "yval_min = " << yval_min << endl;
  cout << "yval_max = " << yval_max << endl;
  if ( yval_min < yval_max) {c_b1=1; c_b2 = yval_max/yval_min; }
  else { c_b1=yval_min/yval_max; c_b2=1;}
  cout << "c_b1 = " << c_b1 << endl;
  cout << "c_b2 = " << c_b2 << endl;
 
  /////////////////////////////////////////////////////////////////////////
  ////// STEP03) c_a
  /////////////////////////////////////////////////////////////////////////
  
  for (int ievt=0; ievt <nevt; ievt++){
    out_tree->GetEvent(ievt);
    
    //// scaling
    reco_jet_clcemc_hadEsum = reco_jet_clcemc_hadEsum*c_b1;
    reco_jet_ihcalEsum = reco_jet_ihcalEsum*c_b2*c_c;
    reco_jet_ohcalEsum = reco_jet_ohcalEsum*c_b2;
    
    //// calculation
    ehasymm = ( reco_jet_clcemc_hadEsum - (reco_jet_ihcalEsum + reco_jet_ohcalEsum) ) / ( reco_jet_clcemc_hadEsum + (reco_jet_ihcalEsum + reco_jet_ohcalEsum) );
    ehfrac = ( reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum ) / ( reco_gamma_cemcEsum - reco_jet_clcemc_emEsum );
    
    //// fill
    h_EHAsymm -> Fill(ehasymm,ehfrac); 
    h_origJtoG_had -> Fill(ehfrac);
  }

  c1->cd(4);
  h_EHAsymm->Draw();
  margin = gPad->GetRightMargin();
  //gPad->SetRightMargin(margin*3);
  gPad->SetRightMargin(0.13);
  
  TProfile *prof_EHAsymm = h_EHAsymm->ProfileX();
  prof_EHAsymm->Fit("pol1","","",fitmin2,fitmax2); 
  gStyle->SetOptFit(0);
  prof_EHAsymm->SetMarkerColor(kCyan+1);
  prof_EHAsymm->SetLineColor(kCyan+1);
  prof_EHAsymm->SetMarkerSize(1.);
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
//  palette->SetX1NDC(1-margin*4);
//  palette->SetX2NDC(1-margin*3);
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
  c1->Update(); 
  
  c1->SaveAs(Form("method05%s/asymm2D_%d_%d_%s_%s_%s.pdf",ihcalType.c_str(),initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str()));

  ///////////////////////////

  TCanvas* c2 = new TCanvas("c2","",1000,500);
	c2->Divide(2,1);
	
  c2->cd(1);
	h_origJtoG_had->GetXaxis()->CenterTitle(1);
	h_origJtoG_had->GetYaxis()->CenterTitle(1);
	h_origJtoG_had->Draw("ep");

	int repeat = 30; 
	float fitmin_jtog = 0.5;
	float fitmax_jtog = 0.5;
	float mean_jtog;
	float sig_jtog;
	float sigErr_jtog;
	TF1* tf_origJtoG= new TF1("TF_JES","gaus",fitmin_jtog,fitmax_jtog);
	//col = h_origJtoG_had->GetMarkerColor();
	//tf_origJtoG->SetLineColor(col);
	tf_origJtoG->SetLineColor(kRed-4);
	recursiveFit(h_origJtoG_had, tf_origJtoG,fitmin_jtog, fitmax_jtog, &mean_jtog, &sig_jtog, &sigErr_jtog);
	for (int ir=0; ir<repeat; ir++){
	  fitmin_jtog = mean_jtog-2*sig_jtog;
	  fitmax_jtog = mean_jtog+2*sig_jtog;
	  recursiveFit(h_origJtoG_had, tf_origJtoG,fitmin_jtog, fitmax_jtog, &mean_jtog, &sig_jtog, &sigErr_jtog);
	}
	tf_origJtoG->Draw("same");
  
  c_a = 1/mean_jtog;
  cout << "c_b2 = " << c_a << endl;
 
  /////////////////////////////////////////////////////////////////////////
  ////// STEP04) final
  /////////////////////////////////////////////////////////////////////////
  
  for (int ievt=0; ievt <nevt; ievt++){
    out_tree->GetEvent(ievt);
    
    //// scaling
    reco_jet_clcemc_hadEsum = reco_jet_clcemc_hadEsum*c_a*c_b1;
    reco_jet_ihcalEsum = reco_jet_ihcalEsum*c_a*c_b2*c_c;
    reco_jet_ohcalEsum = reco_jet_ohcalEsum*c_a*c_b2;
    
    //// calculation
    //ihasymm = ( reco_jet_ihcalEsum - reco_jet_ohcalEsum ) / ( reco_jet_ihcalEsum + reco_jet_ohcalEsum );
    //ihfrac = ( reco_jet_ihcalEsum + reco_jet_ohcalEsum ) / ( reco_gamma_cemcEsum - (reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum) );
    //ehasymm = ( reco_jet_clcemc_hadEsum - (reco_jet_ihcalEsum + reco_jet_ohcalEsum) ) / ( reco_jet_clcemc_hadEsum + (reco_jet_ihcalEsum + reco_jet_ohcalEsum) );
    ehfrac = ( reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum ) / ( reco_gamma_cemcEsum - reco_jet_clcemc_emEsum );
    //totehasymm = ( (reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum) - (reco_jet_ihcalEsum + reco_jet_ohcalEsum) ) / ( (reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum) + (reco_jet_ihcalEsum + reco_jet_ohcalEsum) );
    //totehfrac = ( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum ) / ( reco_gamma_cemcEsum );
    
    //// fill
    h_emfrac -> Fill(true_jet_emfrac,( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum )/true_jet_e);
    h_JtoG_emfrac -> Fill(true_jet_emfrac,( reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum )/reco_gamma_cemcEsum);
    h_JtoG_had -> Fill(ehfrac);
  }
  
  c2->cd(2);
	h_JtoG_had->GetXaxis()->CenterTitle(1);
	h_JtoG_had->GetYaxis()->CenterTitle(1);
	h_JtoG_had->Draw("ep");

	repeat = 30; 
	fitmin_jtog = 0.5;
	fitmax_jtog = 0.5;
	//mean_jtog;
	//sig_jtog;
	//sigErr_jtog;
	TF1* tf_JtoG= new TF1("TF_JES","gaus",fitmin_jtog,fitmax_jtog);
	//col = h_JtoG_had->GetMarkerColor();
	//tf_JtoG->SetLineColor(col);
	tf_JtoG->SetLineColor(kRed-4);
	recursiveFit(h_JtoG_had, tf_JtoG,fitmin_jtog, fitmax_jtog, &mean_jtog, &sig_jtog, &sigErr_jtog);
	for (int ir=0; ir<repeat; ir++){
	  fitmin_jtog = mean_jtog-2*sig_jtog;
	  fitmax_jtog = mean_jtog+2*sig_jtog;
	  recursiveFit(h_JtoG_had, tf_JtoG,fitmin_jtog, fitmax_jtog, &mean_jtog, &sig_jtog, &sigErr_jtog);
	}
	tf_JtoG->Draw("same");
  cout << "1/mean_jtog = " << 1/mean_jtog<< endl;
  
  c2->SaveAs(Form("method05%s/asymm1D_%d_%d_%s_%s_%s.pdf",ihcalType.c_str(),initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str()));


  TCanvas* c3 = new TCanvas("c3","",1200,800);
	c3->Divide(2,2);

  c3->cd(1);
  margin = gPad->GetRightMargin();
  //gPad->SetRightMargin(margin*3);
  gPad->SetRightMargin(0.13);
  
  TProfile *prof_origemfrac = h_origemfrac->ProfileX();
  prof_origemfrac->Fit("pol1","","",fitmin4,fitmax4); 
  gStyle->SetOptFit(0);
  prof_origemfrac->SetMarkerColor(kCyan+1);
  prof_origemfrac->SetLineColor(kCyan+1);
  prof_origemfrac->SetMarkerSize(1.);
  TF1 *f_origemfrac = (TF1*)prof_origemfrac->GetListOfFunctions()->FindObject("pol1");
  if(f_origemfrac){
    f_origemfrac->SetLineColor(kRed-4);
    f_origemfrac->SetLineWidth(4);
    f_origemfrac->SetRange(fitmin4,fitmax4);
    h_origemfrac->GetXaxis()->CenterTitle(1);
    h_origemfrac->GetYaxis()->CenterTitle(1);
    h_origemfrac->Draw();
    prof_origemfrac->Draw("ep same"); 
    
    latex->SetTextAlign(12);
    latex->SetTextSize(0.040);
    latex->DrawLatex(0.63,0.88,Form("#chi^{2}/ndf: %.2f/%d",f_origemfrac->GetChisquare(),f_origemfrac->GetNDF()));
    latex->DrawLatex(0.63,0.82,Form("p0: %.3f #pm %.3f",f_origemfrac->GetParameter(0),f_origemfrac->GetParError(0)));
    latex->DrawLatex(0.63,0.76,Form("p1: %.3f #pm %.3f",f_origemfrac->GetParameter(1),f_origemfrac->GetParError(1)));  
  }
  c3->Update(); //mandatory
  palette = (TPaletteAxis*)h_origemfrac->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
//  palette->SetX1NDC(1-margin*4);
//  palette->SetX2NDC(1-margin*3);
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
  c3->Update();
	
  c3->cd(2);
  margin = gPad->GetRightMargin();
  //gPad->SetRightMargin(margin*3);
  gPad->SetRightMargin(0.13);
  
  TProfile *prof_emfrac = h_emfrac->ProfileX();
  prof_emfrac->Fit("pol1","","",fitmin4,fitmax4); 
  gStyle->SetOptFit(0);
  prof_emfrac->SetMarkerColor(kCyan+1);
  prof_emfrac->SetLineColor(kCyan+1);
  prof_emfrac->SetMarkerSize(1.);
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
  c3->Update(); //mandatory
  palette = (TPaletteAxis*)h_emfrac->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
//  palette->SetX1NDC(1-margin*4);
//  palette->SetX2NDC(1-margin*3);
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
  c3->Update();

  c3->cd(3);
  margin = gPad->GetRightMargin();
  //gPad->SetRightMargin(margin*3);
  gPad->SetRightMargin(0.13);
  
  TProfile *prof_origJtoG_emfrac = h_origJtoG_emfrac->ProfileX();
  prof_origJtoG_emfrac->Fit("pol1","","",fitmin4,fitmax4); 
  gStyle->SetOptFit(0);
  prof_origJtoG_emfrac->SetMarkerColor(kCyan+1);
  prof_origJtoG_emfrac->SetLineColor(kCyan+1);
  prof_origJtoG_emfrac->SetMarkerSize(1.);
  TF1 *f_origJtoG_emfrac = (TF1*)prof_origJtoG_emfrac->GetListOfFunctions()->FindObject("pol1");
  if(f_origJtoG_emfrac){
    f_origJtoG_emfrac->SetLineColor(kRed-4);
    f_origJtoG_emfrac->SetLineWidth(4);
    f_origJtoG_emfrac->SetRange(fitmin4,fitmax4);
    h_origJtoG_emfrac->GetXaxis()->CenterTitle(1);
    h_origJtoG_emfrac->GetYaxis()->CenterTitle(1);
    h_origJtoG_emfrac->Draw();
    prof_origJtoG_emfrac->Draw("ep same"); 
    
    latex->SetTextAlign(12);
    latex->SetTextSize(0.040);
    latex->DrawLatex(0.63,0.88,Form("#chi^{2}/ndf: %.2f/%d",f_origJtoG_emfrac->GetChisquare(),f_origJtoG_emfrac->GetNDF()));
    latex->DrawLatex(0.63,0.82,Form("p0: %.3f #pm %.3f",f_origJtoG_emfrac->GetParameter(0),f_origJtoG_emfrac->GetParError(0)));
    latex->DrawLatex(0.63,0.76,Form("p1: %.3f #pm %.3f",f_origJtoG_emfrac->GetParameter(1),f_origJtoG_emfrac->GetParError(1)));  
  }
  c3->Update(); //mandatory
  palette = (TPaletteAxis*)h_origJtoG_emfrac->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
//  palette->SetX1NDC(1-margin*4);
//  palette->SetX2NDC(1-margin*3);
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
  c3->Update();
	
  c3->cd(4);
  margin = gPad->GetRightMargin();
  //gPad->SetRightMargin(margin*3);
  gPad->SetRightMargin(0.13);
  
  TProfile *prof_JtoG_emfrac = h_JtoG_emfrac->ProfileX();
  prof_JtoG_emfrac->Fit("pol1","","",fitmin4,fitmax4); 
  gStyle->SetOptFit(0);
  prof_JtoG_emfrac->SetMarkerColor(kCyan+1);
  prof_JtoG_emfrac->SetLineColor(kCyan+1);
  prof_JtoG_emfrac->SetMarkerSize(1.);
  TF1 *f_JtoG_emfrac = (TF1*)prof_JtoG_emfrac->GetListOfFunctions()->FindObject("pol1");
  if(f_JtoG_emfrac){
    f_JtoG_emfrac->SetLineColor(kRed-4);
    f_JtoG_emfrac->SetLineWidth(4);
    f_JtoG_emfrac->SetRange(fitmin4,fitmax4);
    h_JtoG_emfrac->GetXaxis()->CenterTitle(1);
    h_JtoG_emfrac->GetYaxis()->CenterTitle(1);
    h_JtoG_emfrac->Draw();
    prof_JtoG_emfrac->Draw("ep same"); 
    
    latex->SetTextAlign(12);
    latex->SetTextSize(0.040);
    latex->DrawLatex(0.63,0.88,Form("#chi^{2}/ndf: %.2f/%d",f_JtoG_emfrac->GetChisquare(),f_JtoG_emfrac->GetNDF()));
    latex->DrawLatex(0.63,0.82,Form("p0: %.3f #pm %.3f",f_JtoG_emfrac->GetParameter(0),f_JtoG_emfrac->GetParError(0)));
    latex->DrawLatex(0.63,0.76,Form("p1: %.3f #pm %.3f",f_JtoG_emfrac->GetParameter(1),f_JtoG_emfrac->GetParError(1)));  
  }
  c3->Update(); //mandatory
  palette = (TPaletteAxis*)h_JtoG_emfrac->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
//  palette->SetX1NDC(1-margin*4);
//  palette->SetX2NDC(1-margin*3);
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
  c3->Update();
  
  c3->SaveAs(Form("method05%s/asymm1D_%d_%d_%s_%s_%s_v2.pdf",ihcalType.c_str(),initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str()));

  float cemc_sf = c_a*c_b1;
  float ihcal_sf = c_a*c_b2*c_c;
  if (ihcalType.compare("Alframe")==0) { ihcal_sf = 1; }
  float ohcal_sf = c_a*c_b2;
  cout << " cemcScale = " << cemc_sf << endl;
  cout << " ihcalScale = " << ihcal_sf << endl;
  cout << " ohcalScale = " << ohcal_sf << endl;

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
