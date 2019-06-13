#include "TMinuit.h"

Double_t func(Double_t x1, Double_t x2, Double_t x3, Double_t x4, Double_t *par);
void fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *param, Int_t iflag);

using namespace std;
vector<float> gamma_E;
vector<float> cemc_emE;
vector<float> cemc_hadE;
vector<float> ihcal_E;
vector<float> ohcal_E;

float yval_min;  
float yval_max;  
float c_c;

void method04_hybrid_alframe(string sampleType = "GammaJet",
                              	string ihcalType = "Alframe",
                              	string jetE = "50GeV",
                              	int initfile=0,
                              	int endfile =1000,
                              	bool doetopcut=true,
                              	bool dodphicut=true) 
{  
  //// E = E_cemc^em + A*E_cemc + B*(c_c*E_ihcal + E_ohcal) )

  gStyle->SetPalette(52); //black
	
  //string fname = Form("./2ndSortedRootFiles/2ndSorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_doetopcut%d_dodphicut%d.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),(int)doetopcut,(int)dodphicut);
  string fname = Form("./2ndSortedRootFiles/2ndSorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_doetopcut%d_dodphicut%d_pt.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),(int)doetopcut,(int)dodphicut);
  TFile* fin = new TFile( fname.c_str(), "READ");
  TTree* out_tree = (TTree*)fin->Get("out_tree");

  //// STEP01) IH asymm (get c_c)
  TH2D* h_step01IHAsymm = new TH2D("h_step01IHAsymm",";Reco Jet E_{IHCAL}-E_{OHCAL} asymmetry;Jet E_{HCAL} / (#gamma E_{Reco} - Jet E_{EMCAL})",101,-1.05,1.05,100,0,2);
  TH2D* h_IHAsymm = new TH2D("h_IHAsymm",";Reco Jet E_{IHCAL}-E_{OHCAL} asymmetry;Jet E_{HCAL} / (#gamma E_{Reco} - Jet E_{EMCAL})",101,-1.05,1.05,100,0,2);
  
  //// STEP02) Minuit (get A, B)
  //// before 
  TH1D* h_step02RESRecoGamma = new TH1D("h_step02RESRecoGamma",";(Jet E_{Reco} - #gamma E_{Reco}) / #gamma E_{Reco};Counts",100,-1.5,1.5);
  TH2D* h_emfrac_step02RESRecoGamma = new TH2D("h_emfrac_step02RESRecoGamma",";Truth Jet EM fraction;(Jet E_{Reco} - #gamma E_{Reco}) / #gamma E_{Reco}",101,-0.05,1.05,100,-1.5,1.5);
  TH2D* h_emfrac_step02JESRecoGamma = new TH2D("h_emfrac_step02JESRecoGamma",";Truth Jet EM fraction;Jet E_{Reco} / #gamma E_{Reco}",101,-0.05,1.05,100,0,2);
  TH2D* h_emfrac_step02JESTruthJet = new TH2D("h_emfrac_step02JESTruthJet",";Truth Jet EM fraction;Jet E_{Reco} / Jet E_{Truth}",101,-0.05,1.05,100,0,2);
  //// after
  TH1D* h_RESRecoGamma = new TH1D("h_RESRecoGamma",";(Jet E_{Reco} - #gamma E_{Reco}) / #gamma E_{Reco};Counts",100,-1.5,1.5);
  TH2D* h_emfrac_RESRecoGamma = new TH2D("h_emfrac_RESRecoGamma",";Truth Jet EM fraction;(Jet E_{Reco} - #gamma E_{Reco}) / #gamma E_{Reco}",101,-0.05,1.05,100,-1.5,1.5);
  TH2D* h_emfrac_JESRecoGamma = new TH2D("h_emfrac_JESRecoGamma",";Truth Jet EM fraction;Jet E_{Reco} / #gamma E_{Reco}",101,-0.05,1.05,100,0,2);
  TH2D* h_emfrac_JESTruthJet = new TH2D("h_emfrac_JESTruthJet",";Truth Jet EM fraction;Jet E_{Reco} / Jet E_{Truth}",101,-0.05,1.05,100,0,2);
  
  h_step01IHAsymm->SetOption("colz");
  h_IHAsymm->SetOption("colz");

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

  float reco_jet_step01E;
  float step01_ihasymm, step01_ihfrac;
  float reco_jet_step02E;
  float step02_ihasymm, step02_ihfrac;
  float reco_jet_caliE;
  float ihasymm, ihfrac;
  
  int nevt = out_tree->GetEntries();
  //int nevt = 500;
  cout << "nevt = " << nevt << endl;

  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////
  ////// STEP01)
  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////
  for (int ievt=0; ievt <nevt; ievt++){
    
    out_tree->GetEvent(ievt);
    
    step01_ihasymm = ( reco_jet_ihcalEsum - reco_jet_ohcalEsum ) / ( reco_jet_ihcalEsum + reco_jet_ohcalEsum );
    step01_ihfrac = ( reco_jet_ihcalEsum + reco_jet_ohcalEsum ) / ( reco_gamma_cemcEsum - (reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum) );
  
    h_step01IHAsymm -> Fill(step01_ihasymm,step01_ihfrac);
  }

  TCanvas* c1 = new TCanvas("c1","",1200,400);
  c1->Divide(2,1);
  c1->cd(1);
  h_step01IHAsymm->Draw();

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
  
  c1->cd(1);
  margin = gPad->GetRightMargin();
  //gPad->SetRightMargin(margin*3);
  gPad->SetRightMargin(0.13);
  
  TProfile *prof_step01IHAsymm = h_step01IHAsymm->ProfileX();
  prof_step01IHAsymm->Fit("pol1","","",fitmin,fitmax); 
  gStyle->SetOptFit(0);
  //gStyle->SetOptFit(1);
  //prof_step01IHAsymm->SetMarkerColor(kYellow-7);
  prof_step01IHAsymm->SetMarkerColor(kCyan+1);
  prof_step01IHAsymm->SetLineColor(kCyan+1);
  prof_step01IHAsymm->SetMarkerSize(1.);
  TF1 *f_step01IHAsymm = (TF1*)prof_step01IHAsymm->GetListOfFunctions()->FindObject("pol1");
  if(f_step01IHAsymm){
    f_step01IHAsymm->SetLineColor(kRed-4);
    //f_step01IHAsymm->SetLineWidth(5);
    f_step01IHAsymm->SetLineWidth(4);
    f_step01IHAsymm->SetRange(fitmin,fitmax);
    h_step01IHAsymm->GetXaxis()->CenterTitle(1);
    h_step01IHAsymm->GetYaxis()->CenterTitle(1);
    h_step01IHAsymm->Draw();
    prof_step01IHAsymm->Draw("ep same"); 
    
    latex->SetTextAlign(12);
    latex->SetTextSize(0.040);
    latex->DrawLatex(0.63,0.88,Form("#chi^{2}/ndf: %.2f/%d",f_step01IHAsymm->GetChisquare(),f_step01IHAsymm->GetNDF()));
    latex->DrawLatex(0.63,0.82,Form("p0: %.3f #pm %.3f",f_step01IHAsymm->GetParameter(0),f_step01IHAsymm->GetParError(0)));
    latex->DrawLatex(0.63,0.76,Form("p1: %.3f #pm %.3f",f_step01IHAsymm->GetParameter(1),f_step01IHAsymm->GetParError(1)));  
  }
  c1->Update(); //mandatory
  palette = (TPaletteAxis*)h_step01IHAsymm->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
//  palette->SetX1NDC(1-margin*4);
//  palette->SetX2NDC(1-margin*3);
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
  c1->Update();

  yval_min = f_step01IHAsymm->Eval(-1);
  yval_max = f_step01IHAsymm->Eval(+1);
  c_c = yval_min/yval_max;
  if (ihcalType.compare("Alframe")==0) { c_c = 1; } 
  cout << "yval_min = " << yval_min << endl;
  cout << "yval_max = " << yval_max << endl;
  cout << "c_c = " << c_c << endl;
  
  ///////////////////////////////////////////////////////////////////////////////
  //// Balancing IHAsymm using yval_min/yval_max 
  ///////////////////////////////////////////////////////////////////////////////
  for (int ievt=0; ievt <nevt; ievt++){
    
    out_tree->GetEvent(ievt);
   
    //// scaling
    //reco_jet_ihcalEsum = reco_jet_ihcalEsum*ihcal_sf; 
    reco_jet_ihcalEsum = reco_jet_ihcalEsum*c_c;
     
    ihasymm = ( reco_jet_ihcalEsum - reco_jet_ohcalEsum ) / ( reco_jet_ihcalEsum + reco_jet_ohcalEsum );
    ihfrac = ( reco_jet_ihcalEsum + reco_jet_ohcalEsum ) / ( reco_gamma_cemcEsum - (reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum) );
  
    h_IHAsymm -> Fill(ihasymm,ihfrac);
  }

  c1->cd(2);
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
  
  c1->SaveAs(Form("method04/asymm2D_%d_%d_%s_%s_%s.pdf",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str()));
  
  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////
  ////// STEP02)
  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////
  for (int ievt=0; ievt <nevt; ievt++){
    
    out_tree->GetEvent(ievt);
  
    //scaling
    reco_jet_ihcalEsum = reco_jet_ihcalEsum*c_c;  
    reco_jet_step02E = reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum;

    h_step02RESRecoGamma->Fill( (reco_jet_step02E-reco_gamma_cemcEsum)/reco_gamma_cemcEsum );    
    h_emfrac_step02RESRecoGamma->Fill( true_jet_emfrac, (reco_jet_step02E-reco_gamma_cemcEsum)/reco_gamma_cemcEsum );
    h_emfrac_step02JESRecoGamma->Fill( true_jet_emfrac, reco_jet_step02E/reco_gamma_cemcEsum );
    h_emfrac_step02JESTruthJet->Fill( true_jet_emfrac, reco_jet_step02E/true_jet_e );

    gamma_E.push_back(reco_gamma_cemcEsum); 
    cemc_emE.push_back(reco_jet_clcemc_emEsum); 
    cemc_hadE.push_back(reco_jet_clcemc_hadEsum); 
    ihcal_E.push_back(reco_jet_ihcalEsum); 
    ohcal_E.push_back(reco_jet_ohcalEsum); 
  }
  cout << "gamma_E.size() = " << gamma_E.size() << endl;

  TMinuit *gMinuit = new TMinuit(5); //initialize TMinuit with a maximum of 5 params
  gMinuit->SetFCN(fcn);
  Double_t arglist[10]; //KYO, what is it?
  Int_t ierflg = 0;
  arglist[0] = 1;
  gMinuit->mnexcm("SET ERR", arglist ,1,ierflg);

  //// Set starting values and step size for parameters
  static Double_t vstart[2] = {1.0, 1.0}; //staring values
  static Double_t step[2] = {0.01, 0.01}; //step
  gMinuit->mnparm(0, "cemc_scale", vstart[0], step[0], 0,0,ierflg);
  gMinuit->mnparm(1, "B_hcal", vstart[1], step[1], 0,0,ierflg);

  //// Now ready for minimization step
  arglist[0] = 500; //KYO
  arglist[1] = 1.;
  gMinuit->mnexcm("MINUIT", arglist ,2,ierflg); //KYO

  Double_t cemcScale;
  Double_t cemcScaleErr;
  Double_t ihcalScale;
  Double_t ihcalScaleErr;
  Double_t ohcalScale;
  Double_t ohcalScaleErr;
  Double_t B_hcal;
  Double_t B_hcalErr;
  gMinuit->GetParameter(0,cemcScale,cemcScaleErr);
  gMinuit->GetParameter(1,B_hcal,B_hcalErr);
  //cemcScale = 1; ihcalScale = 1; ohcalScale = 1;
  //ihcalScale = B_hcal*c_c; // SS310
  ihcalScale = 1; //Alframe
  ihcalScaleErr = 0;
  ohcalScale = B_hcal;
  ohcalScaleErr = 0;

  //// Print results
  //Double_t amin,edm,errdef;
  //Int_t nvpar,nparx,icstat;
  //gMinuit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
  
  for (int ievt=0; ievt <nevt; ievt++){
    
    out_tree->GetEvent(ievt);
    
    reco_jet_caliE = reco_jet_clcemc_emEsum + cemcScale*reco_jet_clcemc_hadEsum + ihcalScale*reco_jet_ihcalEsum + ohcalScale*reco_jet_ohcalEsum;
    
    h_RESRecoGamma->Fill( (reco_jet_caliE-reco_gamma_cemcEsum)/reco_gamma_cemcEsum );    
    h_emfrac_RESRecoGamma->Fill( true_jet_emfrac, (reco_jet_caliE-reco_gamma_cemcEsum)/reco_gamma_cemcEsum );
    h_emfrac_JESRecoGamma->Fill( true_jet_emfrac, reco_jet_caliE/reco_gamma_cemcEsum );
    h_emfrac_JESTruthJet->Fill( true_jet_emfrac, reco_jet_caliE/true_jet_e );
  
  }

  SetHistStyle(h_step02RESRecoGamma,0,0); 
  h_emfrac_step02RESRecoGamma->SetOption("colz");
  h_emfrac_step02JESRecoGamma->SetOption("colz");
  h_emfrac_step02JESTruthJet->SetOption("colz");
  
  SetHistStyle(h_RESRecoGamma,0,0); 
  h_emfrac_RESRecoGamma->SetOption("colz");
  h_emfrac_JESRecoGamma->SetOption("colz");
  h_emfrac_JESTruthJet->SetOption("colz");
  
  ///////////////////////////////////////////////////////////////////////////////

  TCanvas* c2 = new TCanvas("c2","",1200,800);
  c2->Divide(2,2);
  c2->cd(1);
  h_step02RESRecoGamma->Draw("pe");
  c2->cd(2);
  h_RESRecoGamma->Draw("pe");
  c2->cd(3);
  h_emfrac_step02RESRecoGamma->Draw();
  c2->cd(4);
  h_emfrac_RESRecoGamma->Draw();
 
  c2->cd(3);
  margin = gPad->GetRightMargin();
  gPad->SetRightMargin(0.13);

  TProfile *prof_emfrac_step02RESRecoGamma = h_emfrac_step02RESRecoGamma->ProfileX();
  prof_emfrac_step02RESRecoGamma->Fit("pol1","","",fitmin4,fitmax4); 
  gStyle->SetOptFit(0);
  //gStyle->SetOptFit(1);
  prof_emfrac_step02RESRecoGamma->SetMarkerColor(kCyan+1);
  prof_emfrac_step02RESRecoGamma->SetLineColor(kCyan+1);
  prof_emfrac_step02RESRecoGamma->SetMarkerSize(1.);
  TF1 *f_emfrac_step02RESRecoGamma = (TF1*)prof_emfrac_step02RESRecoGamma->GetListOfFunctions()->FindObject("pol1");
  if(f_emfrac_step02RESRecoGamma){
    f_emfrac_step02RESRecoGamma->SetLineColor(kRed-4);
    f_emfrac_step02RESRecoGamma->SetLineWidth(4);
    f_emfrac_step02RESRecoGamma->SetRange(fitmin4,fitmax4);
    h_emfrac_step02RESRecoGamma->Draw();
    prof_emfrac_step02RESRecoGamma->Draw("ep same"); 
    
    latex->SetTextAlign(12);
    latex->SetTextSize(0.040);
    latex->DrawLatex(0.63,0.88,Form("#chi^{2}/ndf: %.2f/%d",f_emfrac_step02RESRecoGamma->GetChisquare(),f_emfrac_step02RESRecoGamma->GetNDF()));
    latex->DrawLatex(0.63,0.82,Form("p0: %.3f #pm %.3f",f_emfrac_step02RESRecoGamma->GetParameter(0),f_emfrac_step02RESRecoGamma->GetParError(0)));
    latex->DrawLatex(0.63,0.76,Form("p1: %.3f #pm %.3f",f_emfrac_step02RESRecoGamma->GetParameter(1),f_emfrac_step02RESRecoGamma->GetParError(1)));  
  }
  c2->Update(); //mandatory
  palette = (TPaletteAxis*)h_emfrac_step02RESRecoGamma->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
  
  c2->cd(4);
  margin = gPad->GetRightMargin();
  gPad->SetRightMargin(0.13);

  TProfile *prof_emfrac_RESRecoGamma = h_emfrac_RESRecoGamma->ProfileX();
  prof_emfrac_RESRecoGamma->Fit("pol1","","",fitmin4,fitmax4); 
  gStyle->SetOptFit(0);
  //gStyle->SetOptFit(1);
  prof_emfrac_RESRecoGamma->SetMarkerColor(kCyan+1);
  prof_emfrac_RESRecoGamma->SetLineColor(kCyan+1);
  prof_emfrac_RESRecoGamma->SetMarkerSize(1.);
  TF1 *f_emfrac_RESRecoGamma = (TF1*)prof_emfrac_RESRecoGamma->GetListOfFunctions()->FindObject("pol1");
  if(f_emfrac_RESRecoGamma){
    f_emfrac_RESRecoGamma->SetLineColor(kRed-4);
    f_emfrac_RESRecoGamma->SetLineWidth(4);
    f_emfrac_RESRecoGamma->SetRange(fitmin4,fitmax4);
    h_emfrac_RESRecoGamma->Draw();
    prof_emfrac_RESRecoGamma->Draw("ep same"); 
    
    latex->SetTextAlign(12);
    latex->SetTextSize(0.040);
    latex->DrawLatex(0.63,0.88,Form("#chi^{2}/ndf: %.2f/%d",f_emfrac_RESRecoGamma->GetChisquare(),f_emfrac_RESRecoGamma->GetNDF()));
    latex->DrawLatex(0.63,0.82,Form("p0: %.3f #pm %.3f",f_emfrac_RESRecoGamma->GetParameter(0),f_emfrac_RESRecoGamma->GetParError(0)));
    latex->DrawLatex(0.63,0.76,Form("p1: %.3f #pm %.3f",f_emfrac_RESRecoGamma->GetParameter(1),f_emfrac_RESRecoGamma->GetParError(1)));  
  }
  c2->Update(); //mandatory
  palette = (TPaletteAxis*)h_emfrac_RESRecoGamma->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
  
  c2->SaveAs(Form("method04/over_recogamma_%d_%d_%s_%s_%s.pdf",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str()));

  ///////////////////////////////////////////////////////////////////////////////
  
  TCanvas* c3 = new TCanvas("c3","",1200,800);
  c3->Divide(2,2);
  c3->cd(1);
  h_emfrac_step02JESRecoGamma->Draw();
  c3->cd(2);
  h_emfrac_JESRecoGamma->Draw();
  c3->cd(3);
  h_emfrac_step02JESTruthJet->Draw();
  c3->cd(4);
  h_emfrac_JESTruthJet->Draw();
  
  c3->cd(1);
  margin = gPad->GetRightMargin();
  gPad->SetRightMargin(0.13);

  TProfile *prof_emfrac_step02JESRecoGamma = h_emfrac_step02JESRecoGamma->ProfileX();
  prof_emfrac_step02JESRecoGamma->Fit("pol1","","",fitmin4,fitmax4); 
  gStyle->SetOptFit(0);
  //gStyle->SetOptFit(1);
  prof_emfrac_step02JESRecoGamma->SetMarkerColor(kCyan+1);
  prof_emfrac_step02JESRecoGamma->SetLineColor(kCyan+1);
  prof_emfrac_step02JESRecoGamma->SetMarkerSize(1.);
  TF1 *f_emfrac_step02JESRecoGamma = (TF1*)prof_emfrac_step02JESRecoGamma->GetListOfFunctions()->FindObject("pol1");
  if(f_emfrac_step02JESRecoGamma){
    f_emfrac_step02JESRecoGamma->SetLineColor(kRed-4);
    f_emfrac_step02JESRecoGamma->SetLineWidth(4);
    f_emfrac_step02JESRecoGamma->SetRange(fitmin4,fitmax4);
    h_emfrac_step02JESRecoGamma->Draw();
    prof_emfrac_step02JESRecoGamma->Draw("ep same"); 
    
    latex->SetTextAlign(12);
    latex->SetTextSize(0.040);
    latex->DrawLatex(0.63,0.88,Form("#chi^{2}/ndf: %.2f/%d",f_emfrac_step02JESRecoGamma->GetChisquare(),f_emfrac_step02JESRecoGamma->GetNDF()));
    latex->DrawLatex(0.63,0.82,Form("p0: %.3f #pm %.3f",f_emfrac_step02JESRecoGamma->GetParameter(0),f_emfrac_step02JESRecoGamma->GetParError(0)));
    latex->DrawLatex(0.63,0.76,Form("p1: %.3f #pm %.3f",f_emfrac_step02JESRecoGamma->GetParameter(1),f_emfrac_step02JESRecoGamma->GetParError(1)));  
  }
  c3->Update(); //mandatory
  palette = (TPaletteAxis*)h_emfrac_step02JESRecoGamma->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
 
  c3->cd(2);
  margin = gPad->GetRightMargin();
  gPad->SetRightMargin(0.13);

  TProfile *prof_emfrac_JESRecoGamma = h_emfrac_JESRecoGamma->ProfileX();
  prof_emfrac_JESRecoGamma->Fit("pol1","","",fitmin4,fitmax4); 
  gStyle->SetOptFit(0);
  //gStyle->SetOptFit(1);
  prof_emfrac_JESRecoGamma->SetMarkerColor(kCyan+1);
  prof_emfrac_JESRecoGamma->SetLineColor(kCyan+1);
  prof_emfrac_JESRecoGamma->SetMarkerSize(1.);
  TF1 *f_emfrac_JESRecoGamma = (TF1*)prof_emfrac_JESRecoGamma->GetListOfFunctions()->FindObject("pol1");
  if(f_emfrac_JESRecoGamma){
    f_emfrac_JESRecoGamma->SetLineColor(kRed-4);
    f_emfrac_JESRecoGamma->SetLineWidth(4);
    f_emfrac_JESRecoGamma->SetRange(fitmin4,fitmax4);
    h_emfrac_JESRecoGamma->Draw();
    prof_emfrac_JESRecoGamma->Draw("ep same"); 
    
    latex->SetTextAlign(12);
    latex->SetTextSize(0.040);
    latex->DrawLatex(0.63,0.88,Form("#chi^{2}/ndf: %.2f/%d",f_emfrac_JESRecoGamma->GetChisquare(),f_emfrac_JESRecoGamma->GetNDF()));
    latex->DrawLatex(0.63,0.82,Form("p0: %.3f #pm %.3f",f_emfrac_JESRecoGamma->GetParameter(0),f_emfrac_JESRecoGamma->GetParError(0)));
    latex->DrawLatex(0.63,0.76,Form("p1: %.3f #pm %.3f",f_emfrac_JESRecoGamma->GetParameter(1),f_emfrac_JESRecoGamma->GetParError(1)));  
  }
  c3->Update(); //mandatory
  palette = (TPaletteAxis*)h_emfrac_JESRecoGamma->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
 
  c3->cd(3);
  margin = gPad->GetRightMargin();
  gPad->SetRightMargin(0.13);

  TProfile *prof_emfrac_step02JESTruthJet = h_emfrac_step02JESTruthJet->ProfileX();
  prof_emfrac_step02JESTruthJet->Fit("pol1","","",fitmin4,fitmax4); 
  gStyle->SetOptFit(0);
  //gStyle->SetOptFit(1);
  prof_emfrac_step02JESTruthJet->SetMarkerColor(kCyan+1);
  prof_emfrac_step02JESTruthJet->SetLineColor(kCyan+1);
  prof_emfrac_step02JESTruthJet->SetMarkerSize(1.);
  TF1 *f_emfrac_step02JESTruthJet = (TF1*)prof_emfrac_step02JESTruthJet->GetListOfFunctions()->FindObject("pol1");
  if(f_emfrac_step02JESTruthJet){
    f_emfrac_step02JESTruthJet->SetLineColor(kRed-4);
    f_emfrac_step02JESTruthJet->SetLineWidth(4);
    f_emfrac_step02JESTruthJet->SetRange(fitmin4,fitmax4);
    h_emfrac_step02JESTruthJet->Draw();
    prof_emfrac_step02JESTruthJet->Draw("ep same"); 
    
    latex->SetTextAlign(12);
    latex->SetTextSize(0.040);
    latex->DrawLatex(0.63,0.88,Form("#chi^{2}/ndf: %.2f/%d",f_emfrac_step02JESTruthJet->GetChisquare(),f_emfrac_step02JESTruthJet->GetNDF()));
    latex->DrawLatex(0.63,0.82,Form("p0: %.3f #pm %.3f",f_emfrac_step02JESTruthJet->GetParameter(0),f_emfrac_step02JESTruthJet->GetParError(0)));
    latex->DrawLatex(0.63,0.76,Form("p1: %.3f #pm %.3f",f_emfrac_step02JESTruthJet->GetParameter(1),f_emfrac_step02JESTruthJet->GetParError(1)));  
  }
  c3->Update(); //mandatory
  palette = (TPaletteAxis*)h_emfrac_step02JESTruthJet->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
 
  c3->cd(4);
  margin = gPad->GetRightMargin();
  gPad->SetRightMargin(0.13);

  TProfile *prof_emfrac_JESTruthJet = h_emfrac_JESTruthJet->ProfileX();
  prof_emfrac_JESTruthJet->Fit("pol1","","",fitmin4,fitmax4); 
  gStyle->SetOptFit(0);
  //gStyle->SetOptFit(1);
  prof_emfrac_JESTruthJet->SetMarkerColor(kCyan+1);
  prof_emfrac_JESTruthJet->SetLineColor(kCyan+1);
  prof_emfrac_JESTruthJet->SetMarkerSize(1.);
  TF1 *f_emfrac_JESTruthJet = (TF1*)prof_emfrac_JESTruthJet->GetListOfFunctions()->FindObject("pol1");
  if(f_emfrac_JESTruthJet){
    f_emfrac_JESTruthJet->SetLineColor(kRed-4);
    f_emfrac_JESTruthJet->SetLineWidth(4);
    f_emfrac_JESTruthJet->SetRange(fitmin4,fitmax4);
    h_emfrac_JESTruthJet->Draw();
    prof_emfrac_JESTruthJet->Draw("ep same"); 
    
    latex->SetTextAlign(12);
    latex->SetTextSize(0.040);
    latex->DrawLatex(0.63,0.88,Form("#chi^{2}/ndf: %.2f/%d",f_emfrac_JESTruthJet->GetChisquare(),f_emfrac_JESTruthJet->GetNDF()));
    latex->DrawLatex(0.63,0.82,Form("p0: %.3f #pm %.3f",f_emfrac_JESTruthJet->GetParameter(0),f_emfrac_JESTruthJet->GetParError(0)));
    latex->DrawLatex(0.63,0.76,Form("p1: %.3f #pm %.3f",f_emfrac_JESTruthJet->GetParameter(1),f_emfrac_JESTruthJet->GetParError(1)));  
  }
  c3->Update(); //mandatory
  palette = (TPaletteAxis*)h_emfrac_JESTruthJet->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
 
  c3->SaveAs(Form("method04/over_recogamma_%d_%d_%s_%s_%s_v2.pdf",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str()));
 
  cout << "cemcScale = " << cemcScale << endl;
  cout << "ihcalScale = " << ihcalScale << endl;
  cout << "ohcalScale = " << ohcalScale << endl;
  
  return;
}
////////////////////////////////////////

Double_t func(Double_t x1, Double_t x2, Double_t x3, Double_t x4, Double_t *par){
  //// cemc^EM + A*cemc^HAD + B*(ihcal_scaled + ohcal);
  //return x1 + par[0]*x2 + par[1]*(x3 + x4); //SS310
  return x1 + par[0]*x2 + par[1]*x4; //Alframe
}

void fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *param, Int_t iflag){
  int ir;
  float chisq = 0;
  float delta;
  for (ir=0;ir<gamma_E.size(); ir++){
    delta = (func(cemc_emE[ir],cemc_hadE[ir],ihcal_E[ir],ohcal_E[ir],param)-gamma_E[ir])/gamma_E[ir];
    ////delta = (func(cemc_emE[ir],cemc_hadE[ir],ihcal_E[ir],ohcal_E[ir],param)-gamma_E[ir])/(cemc_emE[ir]+cemc_hadE[ir]+ihcal_E[ir]+ohcal_E[ir]); //wrong
    //delta = (func(cemc_emE[ir],cemc_hadE[ir],ihcal_E[ir],ohcal_E[ir],param)-gamma_E[ir])/func(cemc_emE[ir],cemc_hadE[ir],ihcal_E[ir],ohcal_E[ir],param);
    chisq += delta*delta;
  } 
  f = chisq;
}
