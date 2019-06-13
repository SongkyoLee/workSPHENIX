#include "TMinuit.h"

Double_t func(Double_t x1, Double_t x2, Double_t x3, Double_t x4, Double_t *par);
void fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *param, Int_t iflag);

using namespace std;
vector<float> gamma_E;
vector<float> cemc_emE;
vector<float> cemc_hadE;
vector<float> ihcal_E;
vector<float> ohcal_E;

void method03_minuit_over_recogamma(string sampleType = "GammaJet",
                              	string ihcalType = "Al",
                              	string jetE = "50GeV",
                              	int initfile=0,
                              	int endfile =1000,
                              	bool doetopcut=true,
                              	bool dodphicut=true) 
{  
  gStyle->SetPalette(52); //black
	
  //string fname = Form("./2ndSortedRootFiles/2ndSorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_doetopcut%d_dodphicut%d.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),(int)doetopcut,(int)dodphicut);
  string fname = Form("./2ndSortedRootFiles/2ndSorted_%dto%d_%s_G4sPHENIX_jet4_%s_%s_doetopcut%d_dodphicut%d_pt.root",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str(),(int)doetopcut,(int)dodphicut);
  TFile* fin = new TFile( fname.c_str(), "READ");
  TTree* out_tree = (TTree*)fin->Get("out_tree");

  //// before 
  TH1D* h_origRESRecoGamma = new TH1D("h_origRESRecoGamma",";(Jet E_{Reco} - #gamma E_{Reco}) / #gamma E_{Reco};Counts",100,-1.5,1.5);
  TH2D* h_emfrac_origRESRecoGamma = new TH2D("h_emfrac_origRESRecoGamma",";Truth Jet EM fraction;(Jet E_{Reco} - #gamma E_{Reco}) / #gamma E_{Reco}",101,-0.05,1.05,100,-1.5,1.5);
  TH2D* h_emfrac_origJESRecoGamma = new TH2D("h_emfrac_origJESRecoGamma",";Truth Jet EM fraction;Jet E_{Reco} / #gamma E_{Reco}",101,-0.05,1.05,100,0,2);
  TH2D* h_emfrac_origJESTruthJet = new TH2D("h_emfrac_origJESTruthJet",";Truth Jet EM fraction;Jet E_{Reco} / Jet E_{Truth}",101,-0.05,1.05,100,0,2);
  //// after
  TH1D* h_RESRecoGamma = new TH1D("h_RESRecoGamma",";(Jet E_{Reco} - #gamma E_{Reco}) / #gamma E_{Reco};Counts",100,-1.5,1.5);
  TH2D* h_emfrac_RESRecoGamma = new TH2D("h_emfrac_RESRecoGamma",";Truth Jet EM fraction;(Jet E_{Reco} - #gamma E_{Reco}) / #gamma E_{Reco}",101,-0.05,1.05,100,-1.5,1.5);
  TH2D* h_emfrac_JESRecoGamma = new TH2D("h_emfrac_JESRecoGamma",";Truth Jet EM fraction;Jet E_{Reco} / #gamma E_{Reco}",101,-0.05,1.05,100,0,2);
  TH2D* h_emfrac_JESTruthJet = new TH2D("h_emfrac_JESTruthJet",";Truth Jet EM fraction;Jet E_{Reco} / Jet E_{Truth}",101,-0.05,1.05,100,0,2);

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

  float reco_jet_origE;
  float reco_jet_caliE;
  
  int nevt = out_tree->GetEntries();
  //int nevt = 500;
  cout << "nevt = " << nevt << endl;

  for (int ievt=0; ievt <nevt; ievt++){
    
    out_tree->GetEvent(ievt);
    
    reco_jet_origE = reco_jet_clcemc_emEsum + reco_jet_clcemc_hadEsum + reco_jet_ihcalEsum + reco_jet_ohcalEsum;

    h_origRESRecoGamma->Fill( (reco_jet_origE-reco_gamma_cemcEsum)/reco_gamma_cemcEsum );    
    h_emfrac_origRESRecoGamma->Fill( true_jet_emfrac, (reco_jet_origE-reco_gamma_cemcEsum)/reco_gamma_cemcEsum );
    h_emfrac_origJESRecoGamma->Fill( true_jet_emfrac, reco_jet_origE/reco_gamma_cemcEsum );
    h_emfrac_origJESTruthJet->Fill( true_jet_emfrac, reco_jet_origE/true_jet_e );

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
  static Double_t vstart[3] = {1.0, 1.0, 1.0}; //staring values
  static Double_t step[3] = {0.01, 0.01, 0.01}; //step
  gMinuit->mnparm(0, "cemc_scale", vstart[0], step[0], 0,0,ierflg);
  gMinuit->mnparm(1, "ihcal_scale", vstart[1], step[1], 0,0,ierflg);
  gMinuit->mnparm(2, "ohcal_scale", vstart[2], step[2], 0,0,ierflg);

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
  gMinuit->GetParameter(0,cemcScale,cemcScaleErr);
  gMinuit->GetParameter(1,ihcalScale,ihcalScaleErr);
  gMinuit->GetParameter(2,ohcalScale,ohcalScaleErr);
  //cemcScale = 1; ihcalScale = 1; ohcalScale = 1;
  
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

  SetHistStyle(h_origRESRecoGamma,0,0); 
  h_emfrac_origRESRecoGamma->SetOption("colz");
  h_emfrac_origJESRecoGamma->SetOption("colz");
  h_emfrac_origJESTruthJet->SetOption("colz");
  
  SetHistStyle(h_RESRecoGamma,0,0); 
  h_emfrac_RESRecoGamma->SetOption("colz");
  h_emfrac_JESRecoGamma->SetOption("colz");
  h_emfrac_JESTruthJet->SetOption("colz");
  
  ///////////////////////////////////////////////////////////////////////////////

  TCanvas* c1 = new TCanvas("c1","",1200,800);
  c1->Divide(2,2);
  c1->cd(1);
  h_origRESRecoGamma->Draw("pe");
  c1->cd(2);
  h_RESRecoGamma->Draw("pe");
  c1->cd(3);
  h_emfrac_origRESRecoGamma->Draw();
  c1->cd(4);
  h_emfrac_RESRecoGamma->Draw();
 
  TPaletteAxis *palette;
  double margin;
  double x1ndc;
  double x2ndc;
    
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
  
  c1->cd(3);
  margin = gPad->GetRightMargin();
  gPad->SetRightMargin(0.13);

  TProfile *prof_emfrac_origRESRecoGamma = h_emfrac_origRESRecoGamma->ProfileX();
  prof_emfrac_origRESRecoGamma->Fit("pol1","","",fitmin4,fitmax4); 
  gStyle->SetOptFit(0);
  //gStyle->SetOptFit(1);
  prof_emfrac_origRESRecoGamma->SetMarkerColor(kCyan+1);
  prof_emfrac_origRESRecoGamma->SetLineColor(kCyan+1);
  prof_emfrac_origRESRecoGamma->SetMarkerSize(1.);
  TF1 *f_emfrac_origRESRecoGamma = (TF1*)prof_emfrac_origRESRecoGamma->GetListOfFunctions()->FindObject("pol1");
  if(f_emfrac_origRESRecoGamma){
    f_emfrac_origRESRecoGamma->SetLineColor(kRed-4);
    f_emfrac_origRESRecoGamma->SetLineWidth(4);
    f_emfrac_origRESRecoGamma->SetRange(fitmin4,fitmax4);
    h_emfrac_origRESRecoGamma->Draw();
    prof_emfrac_origRESRecoGamma->Draw("ep same"); 
    
    latex->SetTextAlign(12);
    latex->SetTextSize(0.040);
    latex->DrawLatex(0.63,0.88,Form("#chi^{2}/ndf: %.2f/%d",f_emfrac_origRESRecoGamma->GetChisquare(),f_emfrac_origRESRecoGamma->GetNDF()));
    latex->DrawLatex(0.63,0.82,Form("p0: %.3f #pm %.3f",f_emfrac_origRESRecoGamma->GetParameter(0),f_emfrac_origRESRecoGamma->GetParError(0)));
    latex->DrawLatex(0.63,0.76,Form("p1: %.3f #pm %.3f",f_emfrac_origRESRecoGamma->GetParameter(1),f_emfrac_origRESRecoGamma->GetParError(1)));  
  }
  c1->Update(); //mandatory
  palette = (TPaletteAxis*)h_emfrac_origRESRecoGamma->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
  
  c1->cd(4);
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
  c1->Update(); //mandatory
  palette = (TPaletteAxis*)h_emfrac_RESRecoGamma->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
  
  c1->SaveAs(Form("method03/over_recogamma_%d_%d_%s_%s_%s.pdf",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str()));

  ///////////////////////////////////////////////////////////////////////////////
  
  TCanvas* c2 = new TCanvas("c2","",1200,800);
  c2->Divide(2,2);
  c2->cd(1);
  h_emfrac_origJESRecoGamma->Draw();
  c2->cd(2);
  h_emfrac_JESRecoGamma->Draw();
  c2->cd(3);
  h_emfrac_origJESTruthJet->Draw();
  c2->cd(4);
  h_emfrac_JESTruthJet->Draw();
  
  c2->cd(1);
  margin = gPad->GetRightMargin();
  gPad->SetRightMargin(0.13);

  TProfile *prof_emfrac_origJESRecoGamma = h_emfrac_origJESRecoGamma->ProfileX();
  prof_emfrac_origJESRecoGamma->Fit("pol1","","",fitmin4,fitmax4); 
  gStyle->SetOptFit(0);
  //gStyle->SetOptFit(1);
  prof_emfrac_origJESRecoGamma->SetMarkerColor(kCyan+1);
  prof_emfrac_origJESRecoGamma->SetLineColor(kCyan+1);
  prof_emfrac_origJESRecoGamma->SetMarkerSize(1.);
  TF1 *f_emfrac_origJESRecoGamma = (TF1*)prof_emfrac_origJESRecoGamma->GetListOfFunctions()->FindObject("pol1");
  if(f_emfrac_origJESRecoGamma){
    f_emfrac_origJESRecoGamma->SetLineColor(kRed-4);
    f_emfrac_origJESRecoGamma->SetLineWidth(4);
    f_emfrac_origJESRecoGamma->SetRange(fitmin4,fitmax4);
    h_emfrac_origJESRecoGamma->Draw();
    prof_emfrac_origJESRecoGamma->Draw("ep same"); 
    
    latex->SetTextAlign(12);
    latex->SetTextSize(0.040);
    latex->DrawLatex(0.63,0.88,Form("#chi^{2}/ndf: %.2f/%d",f_emfrac_origJESRecoGamma->GetChisquare(),f_emfrac_origJESRecoGamma->GetNDF()));
    latex->DrawLatex(0.63,0.82,Form("p0: %.3f #pm %.3f",f_emfrac_origJESRecoGamma->GetParameter(0),f_emfrac_origJESRecoGamma->GetParError(0)));
    latex->DrawLatex(0.63,0.76,Form("p1: %.3f #pm %.3f",f_emfrac_origJESRecoGamma->GetParameter(1),f_emfrac_origJESRecoGamma->GetParError(1)));  
  }
  c2->Update(); //mandatory
  palette = (TPaletteAxis*)h_emfrac_origJESRecoGamma->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
 
  c2->cd(2);
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
  c2->Update(); //mandatory
  palette = (TPaletteAxis*)h_emfrac_JESRecoGamma->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
 
  c2->cd(3);
  margin = gPad->GetRightMargin();
  gPad->SetRightMargin(0.13);

  TProfile *prof_emfrac_origJESTruthJet = h_emfrac_origJESTruthJet->ProfileX();
  prof_emfrac_origJESTruthJet->Fit("pol1","","",fitmin4,fitmax4); 
  gStyle->SetOptFit(0);
  //gStyle->SetOptFit(1);
  prof_emfrac_origJESTruthJet->SetMarkerColor(kCyan+1);
  prof_emfrac_origJESTruthJet->SetLineColor(kCyan+1);
  prof_emfrac_origJESTruthJet->SetMarkerSize(1.);
  TF1 *f_emfrac_origJESTruthJet = (TF1*)prof_emfrac_origJESTruthJet->GetListOfFunctions()->FindObject("pol1");
  if(f_emfrac_origJESTruthJet){
    f_emfrac_origJESTruthJet->SetLineColor(kRed-4);
    f_emfrac_origJESTruthJet->SetLineWidth(4);
    f_emfrac_origJESTruthJet->SetRange(fitmin4,fitmax4);
    h_emfrac_origJESTruthJet->Draw();
    prof_emfrac_origJESTruthJet->Draw("ep same"); 
    
    latex->SetTextAlign(12);
    latex->SetTextSize(0.040);
    latex->DrawLatex(0.63,0.88,Form("#chi^{2}/ndf: %.2f/%d",f_emfrac_origJESTruthJet->GetChisquare(),f_emfrac_origJESTruthJet->GetNDF()));
    latex->DrawLatex(0.63,0.82,Form("p0: %.3f #pm %.3f",f_emfrac_origJESTruthJet->GetParameter(0),f_emfrac_origJESTruthJet->GetParError(0)));
    latex->DrawLatex(0.63,0.76,Form("p1: %.3f #pm %.3f",f_emfrac_origJESTruthJet->GetParameter(1),f_emfrac_origJESTruthJet->GetParError(1)));  
  }
  c2->Update(); //mandatory
  palette = (TPaletteAxis*)h_emfrac_origJESTruthJet->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
 
  c2->cd(4);
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
  c2->Update(); //mandatory
  palette = (TPaletteAxis*)h_emfrac_JESTruthJet->GetListOfFunctions()->FindObject("palette");
  x1ndc = palette->GetX1NDC();
  x2ndc = palette->GetX2NDC();
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.92);
 
  c2->SaveAs(Form("method03/over_recogamma_%d_%d_%s_%s_%s_v2.pdf",initfile,endfile,sampleType.c_str(),jetE.c_str(),ihcalType.c_str()));
 
  cout << "cemcScale = " << cemcScale << endl;
  cout << "ihcalScale = " << ihcalScale << endl;
  cout << "ohcalScale = " << ohcalScale << endl;
  
  return;
}
////////////////////////////////////////

Double_t func(Double_t x1, Double_t x2, Double_t x3, Double_t x4, Double_t *par){
  //// cemc^EM + A*cemc^HAD + B*ihcal + C*ohcal;
  return x1 + par[0]*x2 + par[1]*x3 + par[2]*x4; //SS310
  //return x1 + par[0]*x2 + par[1]*x4; //Alframe
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
