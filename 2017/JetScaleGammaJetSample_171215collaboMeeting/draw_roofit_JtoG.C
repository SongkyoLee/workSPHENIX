
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

void draw_roofit_JtoG(string ihcalType = "SS310",
                    string jetE = "50GeV",
                    string inDir = "outJet4Matched",
                    string outDir = "outJet4JtoG",
                    int initfile=0,
                    int endfile =1000,
                    float c_tot=1.00,
                    float c_em=1.00,
                    float c_h = 1.00,
                    float c_ih=1.00)
{

  bool do_doubleGaus = true;
  float true_p = 1;

  //// E = c_tot ( c_em * E_cemc + c_h ( c_ih * C_ihcal + C_ohcal) )
  float cemc_sf =c_tot*c_em;
  float ihcal_sf =c_tot*c_h*c_ih;
  float ohcal_sf =c_tot*c_h;

  TFile* fin = new TFile(Form("%s%s/gammajet_matched_%s_%s_scale_%.0f_%.0f_%.0f.root",inDir.c_str(),jetE.c_str(),jetE.c_str(),ihcalType.c_str(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100),"READ");
  cout << fin << endl;

  TH1D* h01 = (TH1D*)fin->Get("h1D_JtoG_totE");
  cout << h01 << endl;
 
  //// setting 
  float xmin, xmax; 
  float fitmin, fitmax;
  xmin=0.0;
  xmax=2.0;
  fitmin=0.2;
  fitmax=1.8;
   
  ////h01->Add(h02);
  ////h01->Rebin(4);
  h01->GetXaxis()->SetRangeUser(xmin,xmax);
  double ymax = h01->GetMaximum();
  h01->GetYaxis()->SetRangeUser(0,ymax*1.3);
  SetHistStyle(h01,1,0); 

  //// for roofit  
  RooRealVar* xvar = new RooRealVar("xvar","xvar",xmin,xmax);
  RooDataHist* dh01 = new RooDataHist("dh01","dh01",*xvar,Import(*h01)) ;
  dh01->Print(); 
  RooPlot* xvarframe = xvar->frame();
  xvarframe->GetXaxis()->SetTitle("Jet E_{Reco} / #gamma E_{Reco}");
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
  
  TCanvas*c1 = new TCanvas("c1","",500,500);
  xvarframe->Draw();
  
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(32);
  latex->SetTextSize(0.050);
  //latex->DrawLatex(0.90,0.88,Form("%s, E_{#gamma}^{Truth} > 50 GeV, |#eta|<0.6",ihcalType.c_str()));
  if (jetE.compare("20GeV")==0) { latex->DrawLatex(0.90,0.88,Form("%s, 20 < E_{Truth}^{#gamma} < 30 GeV",ihcalType.c_str())); }
  else if (jetE.compare("30GeV")==0) { latex->DrawLatex(0.90,0.88,Form("%s, 30 < E_{Truth}^{#gamma} < 40 GeV",ihcalType.c_str())); }
  else if (jetE.compare("40GeV")==0) { latex->DrawLatex(0.90,0.88,Form("%s, 40 < E_{Truth}^{#gamma} < 50 GeV",ihcalType.c_str())); }
  else if (jetE.compare("50GeV")==0) { latex->DrawLatex(0.90,0.88,Form("%s, 50 < E_{Truth}^{#gamma} < 60 GeV",ihcalType.c_str())); }
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
  
  c1->SaveAs(Form("./%s%s/%s_%s_JtoG_scale_%.0f_%.0f_%.0f.pdf",outDir.c_str(),jetE.c_str(),jetE.c_str(),ihcalType.c_str(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100));

  return; 
}
