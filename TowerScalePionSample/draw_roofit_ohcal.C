
#include <iostream>
#include <sstream>
#include <string> 
#include "TROOT.h"
#include "TChain.h"
#include "TMath.h"
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
 
void draw_roofit_ohcal_PlMiComb(string outDir = "outScaleTrackP", string inType = "SS310_25GeV", bool do_doubleGaus = true, bool ohcalScaled =false)
{
  float true_p;
  string ihcalType;
  string ptlType;
  if (inType.compare("SS310_25GeV")==0){ 
    true_p  = 25;
    ihcalType = "SS310";
    ptlType = "#pi^{#pm}";
  }
  else if (inType.compare("Al_25GeV")==0){ 
    true_p  = 25;
    ihcalType = "Al";
    ptlType = "#pi^{#pm}";
  }
  else if (inType.compare("SS310_10GeV")==0){ 
    true_p  = 10;
    ihcalType = "SS310";
    ptlType = "#pi^{#pm}";
  }
  else if (inType.compare("Al_10GeV")==0){ 
    true_p  = 10;
    ihcalType = "Al";
    ptlType = "#pi^{#pm}";
  }
  else {
    cout<<"WARNING! select correct inType" <<endl; return;
  } 
   
  float xmin, xmax; 
  float fitmin, fitmax;
  if (true_p ==25) {
    if (ohcalScaled) {
      if (do_doubleGaus) { xmin=0.; xmax=2.0; fitmin = 0.4; fitmax = 1.6; }
      else { xmin=0.; xmax=2.0; fitmin = 0.4; fitmax = 1.6; } //to be determined
    } else {
      if (do_doubleGaus) { xmin=0.; xmax=1.6; fitmin = 0.35; fitmax = 1.25; }
      else { xmin=0.; xmax=1.6; fitmin = 0.4; fitmax = 0.95; }
    }
  } else if (true_p ==10) {
    if (ohcalScaled) {
      if (do_doubleGaus) { xmin=0.; xmax=2.0; fitmin = 0.4; fitmax = 1.6; }
      else { xmin=0.; xmax=2.0; fitmin = 0.4; fitmax = 1.6; } //to be determined
    } else {
      if (do_doubleGaus) { xmin=0.; xmax=1.6; fitmin = 0.3; fitmax = 1.2; }
      else { xmin=0.; xmax=1.6; fitmin = 0.35; fitmax = 0.9; }
    }
  }

  TFile* fin01 = new TFile(Form("%s/%s_PiPl_%.0fGeV_scale_0_0_%d.root",outDir.c_str(),ihcalType.c_str(),true_p,(int)ohcalScaled),"READ");
  TFile* fin02 = new TFile(Form("%s/%s_PiMi_%.0fGeV_scale_0_0_%d.root",outDir.c_str(),ihcalType.c_str(),true_p,(int)ohcalScaled),"READ");
  TH1D* h01;
  TH1D* h02;
  if (ohcalScaled){
    //h01 = (TH1D*)fin01->Get("cemcMipCut/ohcal_frac_E3x3_cut1");
    //h02 = (TH1D*)fin02->Get("cemcMipCut/ohcal_frac_E3x3_cut1");
    h01 = (TH1D*)fin01->Get("innerTwoMipCut/ohcal_frac_E3x3_cut2");
    h02 = (TH1D*)fin02->Get("innerTwoMipCut/ohcal_frac_E3x3_cut2");
  } else {
    h01 = (TH1D*)fin01->Get("innerTwoMipCut/ohcal_frac_E3x3_cut2");
    h02 = (TH1D*)fin02->Get("innerTwoMipCut/ohcal_frac_E3x3_cut2");
  }
  cout << "h01: " << h01 << endl;
  cout << "h02: " << h02 << endl;
  h01->Add(h02);
  h01->Rebin(4);
  h01->GetXaxis()->SetRangeUser(xmin,xmax);
  double ymax = h01->GetMaximum();
  h01->GetYaxis()->SetRangeUser(0,ymax*1.3);
  SetHistStyle(h01,1,0); 

  RooRealVar* xvar = new RooRealVar("xvar","xvar",xmin,xmax);
  RooDataHist* dh01 = new RooDataHist("dh01","dh01",*xvar,Import(*h01)) ;
   
  RooPlot* xvarframe = xvar->frame();
  xvarframe->GetXaxis()->SetTitle("E_{OHCAL} / (E_{Truth} - E_{CEMC} - E_{IHCAL})");
  xvarframe->GetXaxis()->CenterTitle(1);
  xvarframe->GetYaxis()->SetRangeUser(0,ymax*1.2);
  dh01->plotOn(xvarframe);
  //dh01->plotOn(xvarframe,MarkerColor(kRed+2),MarkerStyle(kOpenCircle));

  RooRealVar* mean = new RooRealVar("mean","mean",0.7,0.5,1.5) ;
  RooRealVar* sigma01= new RooRealVar("sigma01","sigma01",0.2,0.1,1.5) ;
  RooRealVar* sigma02= new RooRealVar("sigma02","sigma02",0.7,0.2,2.0) ;
  RooGaussian* gaus01 =new RooGaussian("gaus01","gaus01",*xvar,*mean,*sigma01) ;
  RooGaussian* gaus02 =new RooGaussian("gaus02","gaus02",*xvar,*mean,*sigma02) ;
  RooRealVar* frac01 = new RooRealVar("frac01","frac01",0.8,0.,1.) ;
  RooAddPdf* sig = new RooAddPdf("sig","sig",RooArgList(*gaus01,*gaus02),*frac01) ;
  
  double sigmaTot = 0;
  double sigmaTotErr = 0;
  if (do_doubleGaus){
    sig->fitTo(*dh01,Range(fitmin,fitmax)) ;
    //sig->plotOn(xvarframe,LineColor(kPink+3)) ;
    sig->plotOn(xvarframe,LineColor(kRed+1)) ;
    sig->plotOn(xvarframe,Components("gaus01"),LineColor(kAzure+4),LineStyle(kDashed)) ;
    sig->plotOn(xvarframe,Components("gaus02"),LineColor(kSpring+3),LineStyle(kDashed)) ;
    sigmaTot = sqrt( frac01->getVal()*pow(sigma01->getVal(),2) + (1-frac01->getVal())*pow(sigma02->getVal(),2) );
    sigmaTotErr = (0.5/sigmaTot) *
      sqrt ( 
      pow( frac01->getVal()*sigma01->getVal()*sigma01->getError(),2 ) +
      pow( (1-frac01->getVal())*sigma02->getVal()*sigma02->getError(),2 ) +
      pow( 0.5*frac01->getError()*( pow(sigma01->getVal(),2)-pow(sigma02->getVal(),2) ),2) ); 
  }
  else{
    gaus01->fitTo(*dh01,Range(fitmin,fitmax)) ;
    gaus01->plotOn(xvarframe,LineColor(kRed+1)) ;
  }
  //////////////////////////////////////////////
  //
  TCanvas*c1 = new TCanvas("c1","",500,500);
  xvarframe->Draw();
  
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(32);
  latex->SetTextSize(0.050);
  //latex->SetTextColor(kRed);
  latex->DrawLatex(0.90,0.89,Form("%s, %2.f GeV %s gun, |#eta|<0.8",ihcalType.c_str(),true_p, ptlType.c_str()));
  latex->SetTextSize(0.045);
  if (ohcalScaled){
    if (do_doubleGaus){
      latex->DrawLatex(0.90,0.80,Form("#mu: %.2f #pm %.2f", mean->getVal(),mean->getError()));
      latex->DrawLatex(0.90,0.75,Form("#sigma: %.2f #pm %.2f", sigmaTot,sigmaTotErr));
    } else {
      latex->DrawLatex(0.90,0.80,Form("#mu: %.2f #pm %.2f", mean->getVal(),mean->getError()));
      latex->DrawLatex(0.90,0.75,Form("#sigma: %.2f #pm %.2f", sigma01->getVal(),sigma01->getError()));
    }
  } else {
    if (do_doubleGaus){
      latex->DrawLatex(0.90,0.80,Form("#mu: %.3f #pm %.3f", mean->getVal(),mean->getError()));
      latex->DrawLatex(0.90,0.75,Form("#sigma: %.3f #pm %.3f", sigmaTot,sigmaTotErr));
    } else {
      latex->DrawLatex(0.90,0.80,Form("#mu: %.3f #pm %.3f", mean->getVal(),mean->getError()));
      latex->DrawLatex(0.90,0.75,Form("#sigma: %.3f #pm %.3f", sigma01->getVal(),sigma01->getError()));
    }
  }
  latex->Draw();
  
  c1->SaveAs(Form("%s/%s_ohcal_doubleGaus%d_ohcalScaled%d.pdf",outDir.c_str(),inType.c_str(),(int)do_doubleGaus,(int)ohcalScaled));
  return; 
}
