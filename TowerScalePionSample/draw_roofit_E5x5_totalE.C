
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

void draw_roofit_E5x5_totalE(string outDir = "outTotalE_E5x5",
                           string inType = "SS310_10GeV", 
                           bool do_cemcScale=true,
                           bool do_ihcalScale=true,
                           bool do_totalScale=false,
                           bool do_cemcMipCut=false,
                           bool do_ihcalMipCut=false,
                           bool do_totMipCut=false,
                           bool do_doubleGaus = true)
{
 
  //// string and sf setup 
  float true_p;
  string ihcalType;
  string ptlType;
  
  float c1st=1.; //ihcal
  float c2nd=1.; //cemc
  float c3rd=1.; //total
  
  float cemc_sf=1.;
  float ihcal_sf=1.;
  float ohcal_sf=1.;
  
  if (inType.compare("SS310_25GeV")==0){ 
    true_p  = 25;
    ihcalType = "SS310";
    ptlType = "#pi^{#pm}";
    if (do_cemcScale) c2nd=1.00;
    if (do_ihcalScale) c1st=1.86;
    if (do_totalScale) c3rd=1.24;
  }
  else if (inType.compare("Al_25GeV")==0){ 
    true_p  = 25;
    ihcalType = "Al";
    ptlType = "#pi^{#pm}";
    if (do_cemcScale) c2nd=1.00;
    if (do_ihcalScale) c1st=4.24;
    if (do_totalScale) c3rd=1.24;
  }
  else if (inType.compare("SS310_10GeV")==0){ 
    true_p  = 10;
    ihcalType = "SS310";
    ptlType = "#pi^{#pm}";
    if (do_cemcScale) c2nd=1.;
    if (do_ihcalScale) c1st=1.;
    if (do_totalScale) c3rd=1.;
  }
  else if (inType.compare("Al_10GeV")==0){ 
    true_p  = 10;
    ihcalType = "Al";
    ptlType = "#pi^{#pm}";
    if (do_cemcScale) c2nd=1.;
    if (do_ihcalScale) c1st=1.;
    if (do_totalScale) c3rd=1.;
  }
  else {
    cout<<"WARNING! select correct inType" <<endl; return;
  } 
  cemc_sf=c2nd*c3rd;;
  ihcal_sf=c1st*c3rd;
  ohcal_sf=c3rd;
 
  //// mip cut setup
  float cemcCut= 9999; // select event < mipcut
  float ihcalCut =9999; // select event < mipcut
  if (do_cemcMipCut) { cemcCut = 0.5;}
  if (do_ihcalMipCut) { 
    if (ihcalType.compare("SS310")==0) ihcalCut = 0.3;
    else if (ihcalType.compare("Al")==0) ihcalCut = 0.15;
  }
  float totCut =0.; // N.B. select event > mipcut
  if (do_totMipCut) { 
    if (true_p==25) totCut = 4.5;
    else if (true_p==10) totCut = 1.5;
  }
  
  //// x-axis and fitting range setup
  float xmin, xmax; 
  float fitmin, fitmax;
  if (do_totalScale){
    if (true_p ==25) { xmin=0.; xmax=50.0; fitmin = 17.5; fitmax = 50;} 
    else if (true_p ==10) {
      if (ihcalType.compare("SS310")==0) { xmin=0.; xmax=25.0; fitmin = 6.; fitmax = 25; }
      else if (ihcalType.compare("Al")==0) {xmin=0.; xmax=25.0; fitmin = 7.0; fitmax = 25;} 
    }
  } else {
    if (true_p ==25) { xmin=0.; xmax=50.0; fitmin = 10; fitmax = 40.;} 
    else if (true_p ==10) {
      if (ihcalType.compare("SS310")==0) {xmin=0.; xmax=25.0; fitmin = 4.; fitmax = 15;} 
      else if (ihcalType.compare("Al")==0) {xmin=0.; xmax=25.0; fitmin = 5.; fitmax = 18;} 
    }
  }
 
  //// read-in file 
  TFile* fin01 = new TFile(Form("mergedRootFiles/merged_0to200_ParticleGun_G4sPHENIX_%s_PiPl_%.0fGeV.root",ihcalType.c_str(),true_p),"READ");
  TFile* fin02 = new TFile(Form("mergedRootFiles/merged_0to200_ParticleGun_G4sPHENIX_%s_PiMi_%.0fGeV.root",ihcalType.c_str(),true_p),"READ");
  cout << "fin01 =" << fin01 << endl;
  cout << "fin02 =" << fin02 << endl;
  
  int nbin=400;  
  TH1D* h01 = new TH1D("h01",";;",nbin,xmin,xmax); 
  h01->Sumw2();
 
  //// fill the histo!
  TTree* tr01 = (TTree*)fin01->Get("track_tree");
  TTree* tr02 = (TTree*)fin02->Get("track_tree");
  cout << "tr01 =" << tr01 << endl;
  cout << "tr02 =" << tr02 << endl;
  tr01->Draw(Form("(cemc_E5x5*%2f+ihcal_E5x5*%2f+ohcal_E5x5*%2f)>>h01",cemc_sf,ihcal_sf,ohcal_sf),Form("cemc_E5x5<%2f && ihcal_E5x5<%2f && (cemc_E5x5+ihcal_E5x5+ohcal_E5x5) > %2f",cemcCut,ihcalCut,totCut));
  tr02->Draw(Form("(cemc_E5x5*%2f+ihcal_E5x5*%2f+ohcal_E5x5*%2f)>>+h01",cemc_sf,ihcal_sf,ohcal_sf),Form("cemc_E5x5<%2f && ihcal_E5x5<%2f && (cemc_E5x5+ihcal_E5x5+ohcal_E5x5) > %2f",cemcCut,ihcalCut,totCut));
  
  ////h01->Add(h02);
  h01->Rebin(4);
  h01->GetXaxis()->SetRangeUser(xmin,xmax);
  double ymax = h01->GetMaximum();
  h01->GetYaxis()->SetRangeUser(0,ymax*1.3);
  SetHistStyle(h01,1,0); 
 
  //// for roofit  
  RooRealVar* xvar = new RooRealVar("xvar","xvar",xmin,xmax);
  RooDataHist* dh01 = new RooDataHist("dh01","dh01",*xvar,Import(*h01)) ;
  dh01->Print(); 
  RooPlot* xvarframe = xvar->frame();
  xvarframe->GetXaxis()->SetTitle("Total Reconstucted Energy (GeV)");
  xvarframe->GetXaxis()->CenterTitle(1);
  xvarframe->GetXaxis()->SetRangeUser(xmin,xmax);
  xvarframe->GetYaxis()->SetRangeUser(0,ymax*1.2);
  dh01->plotOn(xvarframe);
  //dh01->plotOn(xvarframe,MarkerColor(kRed+2),MarkerStyle(kOpenCircle));
  
  RooRealVar* mean = new RooRealVar("mean","mean",true_p,true_p*0.5,true_p*1.5) ;
  RooRealVar* sigma01= new RooRealVar("sigma01","sigma01",true_p*0.2,true_p*0.1,true_p*1.5) ;
  RooRealVar* sigma02= new RooRealVar("sigma02","sigma02",true_p*0.7,true_p*0.2,true_p*2.0) ;
  RooGaussian* gaus01 =new RooGaussian("gaus01","gaus01",*xvar,*mean,*sigma01) ;
  RooGaussian* gaus02 =new RooGaussian("gaus02","gaus02",*xvar,*mean,*sigma02) ;
  RooRealVar* frac01 = new RooRealVar("frac01","frac01",0.8,0.,1.) ;
  RooAddPdf* sig = new RooAddPdf("sig","sig",RooArgList(*gaus01,*gaus02),*frac01) ;
  
  double sigmaTot = 0;
  double sigmaTotErr = 0;
  if (do_doubleGaus){
    sig->fitTo(*dh01,Range(fitmin,fitmax)) ;
    //sig->plotOn(xvarframe,LineColor(kPink+3)) ;
    sig->plotOn(xvarframe,LineColor(kRed+1),LineWidth(5)) ;
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
    gaus01->plotOn(xvarframe,LineColor(kRed+1),LineWidth(5)) ;
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
  if (do_doubleGaus){
    latex->DrawLatex(0.90,0.80,Form("#mu: %.2f #pm %.2f", mean->getVal(),mean->getError()));
    latex->DrawLatex(0.90,0.75,Form("#sigma: %.2f #pm %.2f", sigmaTot,sigmaTotErr));
    latex->DrawLatex(0.90,0.70,Form("(#sigma/#mu: %.2f)", sigmaTot/mean->getVal()));
    latex->SetTextSize(0.030);
    latex->DrawLatex(0.90,0.65,Form("Scale = %.2f", true_p/mean->getVal()));
    latex->DrawLatex(0.90,0.62,Form("cemc_sf = %.2f", cemc_sf));
    latex->DrawLatex(0.90,0.59,Form("ihcal_sf = %.2f", ihcal_sf));
    latex->DrawLatex(0.90,0.56,Form("ohcal_sf = %.2f", ohcal_sf));
  } else {
    latex->DrawLatex(0.90,0.80,Form("#mu: %.2f #pm %.2f", mean->getVal(),mean->getError()));
    latex->DrawLatex(0.90,0.75,Form("#sigma: %.2f #pm %.2f", sigma01->getVal(),sigma01->getError()));
    latex->DrawLatex(0.90,0.70,Form("(#sigma/#mu: %.2f)", sigma01->getVal()/mean->getVal()));
    latex->SetTextSize(0.030);
    latex->DrawLatex(0.90,0.65,Form("Scale = %.2f", true_p/mean->getVal()));
    latex->DrawLatex(0.90,0.62,Form("cemc_sf = %.2f", cemc_sf));
    latex->DrawLatex(0.90,0.59,Form("ihcal_sf = %.2f", ihcal_sf));
    latex->DrawLatex(0.90,0.56,Form("ohcal_sf = %.2f", ohcal_sf));
  }
  latex->Draw();
  cout << "Scale = " << true_p/mean->getVal() << endl;
  
  c1->SaveAs(Form("%s/%s_totalE_%.0f_%.0f_%.0f_mipcut_%.0f_%.0f_dblG%d.pdf",outDir.c_str(),inType.c_str(),(int)do_cemcScale*c2nd*100,(int)do_ihcalScale*c1st*100,(int)do_totalScale*c3rd*100,(int)do_cemcMipCut*cemcCut*100,(int)do_ihcalMipCut*ihcalCut*100,(int)do_doubleGaus));

  return; 
}
