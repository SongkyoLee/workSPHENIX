
#include <iostream>
#include <sstream>
#include <string> 
#include "TROOT.h"
#include "TChain.h"
#include "TMath.h"
#include <math.h>
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TNtuple.h"
#include "TRandom3.h"
#include "TVector.h"
#include "TRotation.h"
#include "TF1.h"
#include "TFile.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TSpectrum.h"
#include "TVector3.h"

#include <string.h>
#include "TChain.h"
#include "TTree.h"
#include "TKey.h"
#include "Riostream.h"

#include "TError.h"

#include <vector>

#include <RooGaussian.h>
#include <RooDataHist.h>

using namespace RooFit;

void draw_1D_var(string ihcalType = "SS310",string jetE="50GeV", int initfile=0,int endfile =1000, bool do_fit=false)
//void draw_1D_var(string ihcalType = "Al",int initfile=0,int endfile =1000, bool do_fit=false)
{
 
  TFile* fin = new TFile(Form("outJet4Matched%s/gammajet_matched_%s_%s_scale_100_100_100.root",jetE.c_str(),jetE.c_str(),ihcalType.c_str()),"READ");
  cout << fin << endl;
  //TLegend* leg = new TLegend(0.71, 0.54, 0.89, 0.74);
  //TLegend* leg = new TLegend(0.23, 0.75, 0.50, 0.90);
  TLegend* leg = new TLegend(0.23, 0.65, 0.50, 0.90);

  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(32);
  latex->SetTextSize(0.045);

  //// 1) EM fraction 
  TH1D* h1D_true_gamma_emfrac = (TH1D*)fin->Get("h1D_true_gamma_emfrac"); 
  TH1D* h1D_true_jet_emfrac = (TH1D*)fin->Get("h1D_true_jet_emfrac"); 
  cout << h1D_true_gamma_emfrac << endl;
  cout << h1D_true_jet_emfrac << endl;
  SetLineHistStyle(h1D_true_gamma_emfrac,1);
  SetLineHistStyle(h1D_true_jet_emfrac,2);
  leg->SetHeader(ihcalType.c_str());
  leg->AddEntry(h1D_true_gamma_emfrac,"#gamma","l");
  leg->AddEntry(h1D_true_jet_emfrac,"Jet","l");
 
  TCanvas* c_emfrac = new TCanvas("c_emfrac","",500,500);
  h1D_true_gamma_emfrac->GetXaxis()->SetTitle("True EM fraction");
  h1D_true_gamma_emfrac->GetXaxis()->CenterTitle(1);
  h1D_true_gamma_emfrac->GetYaxis()->CenterTitle(1);
  h1D_true_gamma_emfrac->SetMinimum(1.0);
  h1D_true_gamma_emfrac->Draw("hist");
  h1D_true_jet_emfrac->Draw("hist same");
  leg->Draw();
  
  //latex->SetTextAlign(32);
  //latex->SetTextSize(0.050);
  //latex->DrawLatex(0.90,0.88,Form("%s",ihcalType.c_str()));
  //latex->DrawLatex(0.90,0.88,Form("%s, E_{#gamma}^{Truth} > 50 GeV, |#eta|<0.6",ihcalType.c_str()));
  gPad->SetLogy(1); 
  c_emfrac->SaveAs(Form("./out1DVar/%s_%s_emfrac.pdf",jetE.c_str(),ihcalType.c_str()));
  
  //// 2) Leading particle z
  TH1D* h1D_true_gamma_lcons_z = (TH1D*)fin->Get("h1D_true_gamma_lcons_z"); 
  TH1D* h1D_true_jet_lcons_z = (TH1D*)fin->Get("h1D_true_jet_lcons_z"); 
  cout << h1D_true_gamma_lcons_z << endl;
  cout << h1D_true_jet_lcons_z << endl;
  SetLineHistStyle(h1D_true_gamma_lcons_z,1);
  SetLineHistStyle(h1D_true_jet_lcons_z,2);
  //leg->AddEntry(h1D_true_gamma_lcons_z,"#gamma","l");
  //leg->AddEntry(h1D_true_jet_lcons_z,"Jet","l");
 
  TCanvas* c_lcons_z = new TCanvas("c_lcons_z","",500,500);
  h1D_true_gamma_lcons_z->GetXaxis()->SetTitle("Leadinf particle z=p_{||}/p_{Jet}");
  h1D_true_gamma_lcons_z->GetXaxis()->CenterTitle(1);
  h1D_true_gamma_lcons_z->GetYaxis()->CenterTitle(1);
  h1D_true_gamma_lcons_z->SetMinimum(1.0);
  h1D_true_gamma_lcons_z->Draw("hist");
  h1D_true_jet_lcons_z->Draw("hist same");
  leg->Draw();
  
  gPad->SetLogy(1); 
  c_lcons_z->SaveAs(Form("./out1DVar/%s_%s_lcons_z.pdf",jetE.c_str(),ihcalType.c_str()));
  
  //// 3) JES
  TH1D* h1D_gamma_JES = (TH1D*)fin->Get("h1D_gamma_JES"); 
  TH1D* h1D_jet_JES = (TH1D*)fin->Get("h1D_jet_JES"); 
  cout << h1D_gamma_JES << endl;
  cout << h1D_jet_JES << endl;
  SetLineHistStyle(h1D_gamma_JES,1);
  SetLineHistStyle(h1D_jet_JES,2);
  //leg->AddEntry(h1D_gamma_JES,"#gamma","l");
  //leg->AddEntry(h1D_jet_JES,"Jet","l");
 
  TCanvas* c_JES = new TCanvas("c_JES","",500,500);
  h1D_gamma_JES->GetXaxis()->SetTitle("E_{Reco}/E_{Truth}");
  h1D_gamma_JES->GetXaxis()->CenterTitle(1);
  h1D_gamma_JES->GetYaxis()->CenterTitle(1);
  h1D_gamma_JES->Draw("hist");
  h1D_jet_JES->Draw("hist same");
  leg->Draw();
 
  if (do_fit){
    double fitmin, fitmax;
    fitmin = 0.7;
    fitmax = 1.2;
    TF1* fitfunc01 = new TF1("fitfunc01","gaus",fitmin,fitmax);
    fitfunc01->SetLineColor(kGray+3);
    fitfunc01->SetLineStyle(2);
    fitmin = 0.4;
    fitmax = 1.2;
    TF1* fitfunc02 = new TF1("fitfunc02","gaus",fitmin,fitmax);
    fitfunc02->SetLineColor(kGray+3);
    fitfunc02->SetLineStyle(2);
    h1D_gamma_JES->Fit("fitfunc01","N");
    fitfunc01->Draw("same");
    h1D_jet_JES->Fit("fitfunc02","N");
    fitfunc02->Draw("same");
  
    //float mean01 = fitfunc01->GetParameter(1);
    //float sigma01 = fitfunc01->GetParameter(2);
    //float res01 = sigma01/mean01;
    //float mean02 = fitfunc02->GetParameter(1);
    //float sigma02 = fitfunc02->GetParameter(2);
    //float res02 = sigma02/mean02;
  } 
  
  gPad->SetLogy(0); 
  c_JES->SaveAs(Form("./out1DVar/%s_%s_JES_dofit%d.pdf",jetE.c_str(),ihcalType.c_str(),(int)do_fit));

  return; 
}
