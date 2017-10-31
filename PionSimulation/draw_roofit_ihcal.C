
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
#include <RooLandau.h>
#include <RooDataHist.h>

using namespace RooFit;

//// fitOpt
// 0: single gaussian
// 1: double gaussian
// 2: landau
// 3: landau + gaussian
 
void draw_roofit_ihcal(string inType = "Al_PiPl_25GeV", int fitOpt = 0)
{
  float true_p;
  string ihcalType;
  string ptlType;
  if (inType.compare("SS310_PiPl_25GeV")==0){ 
    true_p  = 25;
    ihcalType = "SS310";
    ptlType = "#pi^{+}";
  }
  else if (inType.compare("SS310_PiMi_25GeV")==0){ 
    true_p  = 25;
    ihcalType = "SS310";
    ptlType = "#pi^{-}";
  }
  else if (inType.compare("Al_PiPl_25GeV")==0){ 
    true_p  = 25;
    ihcalType = "Al";
    ptlType = "#pi^{+}";
  }
  else if (inType.compare("Al_PiMi_25GeV")==0){ 
    true_p  = 25;
    ihcalType = "Al";
    ptlType = "#pi^{-}";
  }
  else if (inType.compare("SS310_PiPl_10GeV")==0){ 
    true_p  = 10;
    ihcalType = "SS310";
    ptlType = "#pi^{+}";
  }
  else if (inType.compare("SS310_PiMi_10GeV")==0){ 
    true_p  = 10;
    ihcalType = "SS310";
    ptlType = "#pi^{-}";
  }
  else if (inType.compare("Al_PiPl_10GeV")==0){ 
    true_p  = 10;
    ihcalType = "Al";
    ptlType = "#pi^{+}";
  }
  else if (inType.compare("Al_PiMi_10GeV")==0){ 
    true_p  = 10;
    ihcalType = "Al";
    ptlType = "#pi^{-}";
  }
  else {
    cout<<"WARNING! select correct inType" <<endl; return;
  } 
  
  float fitmin, fitmax;
  if (ihcalType.compare("SS310")==0){ 
    if (fitOpt==0) { fitmin = 0.15; fitmax = 0.65; }
    else if (fitOpt==1) { fitmin = 0.2; fitmax = 1.2; }
    else if (fitOpt==2) { fitmin = 0.15; fitmax = 1.6; }
    else { fitmin = 0.15; fitmax = 1.6; }
  } else if (ihcalType.compare("Al")==0){ 
    if (fitOpt==0) { fitmin = 0.05; fitmax = 0.3; }
    else if (fitOpt==1) { fitmin = 0.05; fitmax = 0.6; }
    else if (fitOpt==2) { fitmin = 0.075; fitmax = 1.0; }
    else { fitmin = 0.0625; fitmax = 1.0; }
  }

  TFile* fin = new TFile(Form("out_scale/%s_scale_0_0_1.root",inType.c_str()),"READ");
  cout << "fin: " << fin << endl;
  TH1D* h01 = (TH1D*)fin->Get("afterCut/ihcal_frac_E3x3_cut");
  //TH1D* h01 = (TH1D*)fin->Get("afterCut/outerTwo_frac_E3x3_cut");
  cout << "h01: " << h01 << endl;
  h01->Rebin(4);
  h01->GetXaxis()->SetRangeUser(0,1.6);
  double ymax = h01->GetMaximum();
  h01->GetYaxis()->SetRangeUser(0,ymax*1.3);
  SetHistStyle(h01,1,0); 

  RooRealVar* frac = new RooRealVar("frac","frac",0,1.6);
  RooDataHist* dh01 = new RooDataHist("dh01","dh01",*frac,Import(*h01)) ;
   
  RooPlot* fracframe = frac->frame();
  fracframe->GetXaxis()->SetTitle("E_{IHCAL} / (E_{Truth} - E_{CEMC} - E_{OHCAL})");
  fracframe->GetXaxis()->CenterTitle(1);
  fracframe->GetYaxis()->SetRangeUser(0,ymax*1.2);
  dh01->plotOn(fracframe);
  //dh01->plotOn(fracframe,MarkerColor(kRed+2),MarkerStyle(kOpenCircle));

  //// Gaussian
  RooRealVar* mean = new RooRealVar("mean","mean",0.4,0.01,0.99) ;
  RooRealVar* sigma01= new RooRealVar("sigma01","sigma01",0.2,0.01,0.9) ;
  RooRealVar* sigma02= new RooRealVar("sigma02","sigma02",0.7,0.02,1.9) ;
  RooGaussian* gaus01 =new RooGaussian("gaus01","gaus01",*frac,*mean,*sigma01) ;
  RooGaussian* gaus02 =new RooGaussian("gaus02","gaus02",*frac,*mean,*sigma02) ;
  RooRealVar* frac01 = new RooRealVar("frac01","frac01",0.8,0.,1.) ;
  RooAddPdf* sig = new RooAddPdf("sig","sig",RooArgList(*gaus01,*gaus02),*frac01) ;

  //// Landau
  //RooRealVar* meanL = new RooRealVar("meanL","meanL",0.4,0.01,0.99) ; 
  RooRealVar* sigmaL= new RooRealVar("sigmaL","sigmaL",0.2,0.01,0.9) ;
  RooLandau* landau = new RooLandau("landau","landau",*frac,*mean,*sigmaL); 
  
  RooRealVar* frac02 = new RooRealVar("frac02","frac02",0.8,0.,1.) ;
  RooAddPdf* sig02 = new RooAddPdf("sig02","sig02",RooArgList(*landau,*gaus01),*frac02) ;

  double sigmaTot=0;
  double sigmaTotErr=0;
  if (fitOpt ==0) {
    gaus01->fitTo(*dh01,Range(fitmin,fitmax)) ;
    gaus01->plotOn(fracframe,LineColor(kRed+1)) ;
  }
  else if (fitOpt ==1){
    sig->fitTo(*dh01,Range(fitmin,fitmax)) ;
    //sig->plotOn(fracframe,LineColor(kPink+3)) ;
    sig->plotOn(fracframe,LineColor(kRed+1)) ;
    sig->plotOn(fracframe,Components("gaus01"),LineColor(kAzure+4),LineStyle(kDashed)) ;
    sig->plotOn(fracframe,Components("gaus02"),LineColor(kSpring+3),LineStyle(kDashed)) ;
    sigmaTot = sqrt( frac01->getVal()*pow(sigma01->getVal(),2) + (1-frac01->getVal())*pow(sigma02->getVal(),2) );
    sigmaTotErr = (0.5/sigmaTot) *
      sqrt ( 
      pow( frac01->getVal()*sigma01->getVal()*sigma01->getError(),2 ) +
      pow( (1-frac01->getVal())*sigma02->getVal()*sigma02->getError(),2 ) +
      pow( 0.5*frac01->getError()*( pow(sigma01->getVal(),2)-pow(sigma02->getVal(),2) ),2) ); 
  }
  else if (fitOpt==2) {
    landau->fitTo(*dh01,Range(fitmin,fitmax)) ;
    landau->plotOn(fracframe,LineColor(kRed+1)) ;
  }
  else if (fitOpt==3) {
    sig02->fitTo(*dh01,Range(fitmin,fitmax)) ;
    //sig02->plotOn(fracframe,LineColor(kPink+3)) ;
    sig02->plotOn(fracframe,LineColor(kRed+1)) ;
    sig02->plotOn(fracframe,Components("landau"),LineColor(kAzure+4),LineStyle(kDashed)) ;
    sig02->plotOn(fracframe,Components("gaus01"),LineColor(kSpring+3),LineStyle(kDashed)) ;
    sigmaTot = sqrt( frac02->getVal()*pow(sigmaL->getVal(),2) + (1-frac02->getVal())*pow(sigma01->getVal(),2) );
    sigmaTotErr = (0.5/sigmaTot) *
      sqrt ( 
      pow( frac02->getVal()*sigmaL->getVal()*sigmaL->getError(),2 ) +
      pow( (1-frac02->getVal())*sigma01->getVal()*sigma01->getError(),2 ) +
      pow( 0.5*frac02->getError()*( pow(sigmaL->getVal(),2)-pow(sigma01->getVal(),2) ),2) ); 
  }
  else {
    cout << "Select correct fitOpt" << endl; return;
  }
  //////////////////////////////////////////////
  //
  TCanvas*c1 = new TCanvas("c1","",500,500);
  fracframe->Draw();
  
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(32);
  latex->SetTextSize(0.050);
  //latex->SetTextColor(kRed);
  latex->DrawLatex(0.90,0.89,Form("%s, %2.f GeV %s gun, |#eta|<0.8",ihcalType.c_str(),true_p, ptlType.c_str()));
  latex->SetTextSize(0.045);
  if (fitOpt==0) {
    latex->DrawLatex(0.90,0.80,Form("#mu: %.3f #pm %.3f", mean->getVal(),mean->getError()));
    latex->DrawLatex(0.90,0.75,Form("#sigma: %.3f #pm %.3f", sigma01->getVal(),sigma01->getError()));
    //latex->DrawLatex(0.73,0.87,Form("#mu: %.3f", mean->getVal()));
    //latex->DrawLatex(0.73,0.82,Form("#sigma: %.3f", sigma01->getVal()));
  } else if (fitOpt==1){
    latex->DrawLatex(0.90,0.80,Form("#mu: %.3f #pm %.3f", mean->getVal(),mean->getError()));
    latex->DrawLatex(0.90,0.75,Form("#sigma: %.3f #pm %.3f", sigmaTot,sigmaTotErr));
    //latex->DrawLatex(0.73,0.87,Form("#mu: %.3f", mean->getVal()));
    //latex->DrawLatex(0.73,0.82,Form("#sigma: %.3f", sigmaTot));
  } else if (fitOpt==2){
    latex->DrawLatex(0.90,0.80,Form("#mu: %.3f #pm %.3f", mean->getVal(),mean->getError()));
    latex->DrawLatex(0.90,0.75,Form("#sigma: %.3f #pm %.3f", sigmaL->getVal(),sigmaL->getError()));
  } else {
    latex->DrawLatex(0.90,0.80,Form("#mu: %.3f #pm %.3f", mean->getVal(),mean->getError()));
    latex->DrawLatex(0.90,0.75,Form("#sigma: %.3f #pm %.3f", sigmaTot,sigmaTotErr));
  }
  latex->Draw();
  
  c1->SaveAs(Form("out_scale/%s_ihcal_fitOpt%d.pdf",inType.c_str(),fitOpt));
  return; 
}
