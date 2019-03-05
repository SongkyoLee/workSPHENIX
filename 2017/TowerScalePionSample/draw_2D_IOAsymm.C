
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

void draw_2D_IOAsymm(string outDir = "out2D_IOAsymm",
                           string inType = "SS310_25GeV", 
                           bool do_cemcScale=false,
                           bool do_ihcalScale=false,
                           bool do_ohcalScale=false,
                           bool do_cemcMipCut=false,
                           bool do_ihcalMipCut=false,
                           bool do_totMipCut=false,
                           bool do_profile=true)
{
  //gStyle->SetOptStat(1);
  //gStyle->SetPalette(51);
  gStyle->SetPalette(55);
  
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.045);
 
  //// string and sf setup 
  float true_p;
  string ihcalType;
  string ptlType;
  
  float cemc_sf=1.;
  float ihcal_sf=1.;
  float ohcal_sf=1.;
  
  if (inType.compare("SS310_25GeV")==0){ 
    true_p  = 25;
    ihcalType = "SS310";
    ptlType = "#pi^{#pm}";
    if (do_cemcScale) cemc_sf=1.;
    if (do_ihcalScale) ihcal_sf=1.81;
    if (do_ohcalScale) ohcal_sf=1.;
  }
  else if (inType.compare("Al_25GeV")==0){ 
    true_p  = 25;
    ihcalType = "Al";
    ptlType = "#pi^{#pm}";
    if (do_cemcScale) cemc_sf=1.;
    if (do_ihcalScale) ihcal_sf=4.37;
    if (do_ohcalScale) ohcal_sf=1.;
  }
  else if (inType.compare("SS310_10GeV")==0){ 
    true_p  = 10;
    ihcalType = "SS310";
    ptlType = "#pi^{#pm}";
    if (do_cemcScale) cemc_sf=1.;
    if (do_ihcalScale) ihcal_sf=1.78;
    if (do_ohcalScale) ohcal_sf=1.;
  }
  else if (inType.compare("Al_10GeV")==0){ 
    true_p  = 10;
    ihcalType = "Al";
    ptlType = "#pi^{#pm}";
    if (do_cemcScale) cemc_sf=1.;
    if (do_ihcalScale) ihcal_sf=5.74;
    if (do_ohcalScale) ohcal_sf=1.;
  }
  else {
    cout<<"WARNING! select correct inType" <<endl; return;
  } 
 
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
  
  float xmin = 0;
  float xmax = true_p*2.0;
  float ymin = 0;
  //float ymax = 2.;
  float ymax = true_p*2.;
   
  //// read-in file 
  TFile* fin01 = new TFile(Form("mergedRootFiles/merged_0to200_ParticleGun_G4sPHENIX_%s_PiPl_%.0fGeV.root",ihcalType.c_str(),true_p),"READ");
  TFile* fin02 = new TFile(Form("mergedRootFiles/merged_0to200_ParticleGun_G4sPHENIX_%s_PiMi_%.0fGeV.root",ihcalType.c_str(),true_p),"READ");
  cout << "fin01 =" << fin01 << endl;
  cout << "fin02 =" << fin02 << endl;
  
  int nbin=400;  
  //TH2D* h2d01 = new TH2D("h2d01",";(E_{IHCAL} - E_{OHCAL})/(E_{IHCAL} + E_{OHCAL});E_{CEMC} + E_{IHCAL} + E_{OHCAL}",nbin,-1,1,nbin,ymin,ymax); 
  TH2D* h2d01 = new TH2D("h2d01",";(E_{IHCAL} - E_{OHCAL})/(E_{IHCAL} + E_{OHCAL});E_{IHCAL} + E_{OHCAL}",nbin,-1,1,nbin,ymin,ymax); 
 
  //// fill the histo!
  TTree* tr01 = (TTree*)fin01->Get("track_tree");
  TTree* tr02 = (TTree*)fin02->Get("track_tree");
  tr01->Draw(Form("(ihcal_E3x3*%2f+ohcal_E3x3*%2f):((ihcal_E3x3*%2f-ohcal_E3x3*%2f)/(ihcal_E3x3*%2f+ohcal_E3x3*%2f))>>h2d01",ihcal_sf,ohcal_sf,ihcal_sf,ohcal_sf,ihcal_sf,ohcal_sf),Form("cemc_E3x3<%2f && ihcal_E3x3<%2f && (cemc_E3x3+ihcal_E3x3+ohcal_E3x3)>%2f",cemcCut,ihcalCut,totCut));
  tr02->Draw(Form("(ihcal_E3x3*%2f+ohcal_E3x3*%2f):((ihcal_E3x3*%2f-ohcal_E3x3*%2f)/(ihcal_E3x3*%2f+ohcal_E3x3*%2f))>>+h2d01",ihcal_sf,ohcal_sf,ihcal_sf,ohcal_sf,ihcal_sf,ohcal_sf),Form("cemc_E3x3<%2f && ihcal_E3x3<%2f && (cemc_E3x3+ihcal_E3x3+ohcal_E3x3)>%2f",cemcCut,ihcalCut,totCut));
  //tr01->Draw(Form("(cemc_E3x3*%2f+ihcal_E3x3*%2f+ohcal_E3x3*%2f):((ihcal_E3x3*%2f-ohcal_E3x3*%2f)/(ihcal_E3x3*%2f+ohcal_E3x3*%2f))>>h2d01",cemc_sf,ihcal_sf,ohcal_sf,ihcal_sf,ohcal_sf,ihcal_sf,ohcal_sf),Form("cemc_E3x3<%2f && ihcal_E3x3<%2f && (cemc_E3x3+ihcal_E3x3+ohcal_E3x3)>%2f",cemcCut,ihcalCut,totCut));
  //tr02->Draw(Form("(cemc_E3x3*%2f+ihcal_E3x3*%2f+ohcal_E3x3*%2f):((ihcal_E3x3*%2f-ohcal_E3x3*%2f)/(ihcal_E3x3*%2f+ohcal_E3x3*%2f))>>+h2d01",cemc_sf,ihcal_sf,ohcal_sf,ihcal_sf,ohcal_sf,ihcal_sf,ohcal_sf),Form("cemc_E3x3<%2f && ihcal_E3x3<%2f && (cemc_E3x3+ihcal_E3x3+ohcal_E3x3)>%2f",cemcCut,ihcalCut,totCut));
  
  ///////////////////////////////////////////////////////////////////////
  
  TPaletteAxis* pal = new TPaletteAxis();
  double fitmin=0;
  double fitmax=0;
 
  ///////////////////////////////////////////////////////////////////////
   
  TCanvas* c01 = new TCanvas("c01","c01",700,500);
  gPad->SetRightMargin(3.0);
  h2d01->GetXaxis()->CenterTitle(1);
  h2d01->GetYaxis()->CenterTitle(1);
  h2d01->Draw("colz");
  c01->Update(); // mandatory to find palette!!
  pal = (TPaletteAxis*)h2d01->GetListOfFunctions()->FindObject("palette");
  pal->SetX2NDC(0.92);
  
  if (do_profile){
    if (ihcalType.compare("SS310")==0) {
      //fitmin= -0.8;
      fitmin= -0.2;
      fitmax= 0.8;
    } else if (ihcalType.compare("Al")==0) {
      //fitmin= -0.8;
      fitmin= -0.2;
      fitmax= 0.6;
    }
    TProfile *prof01 = h2d01->ProfileX();
    prof01->Fit("pol1","","",fitmin,fitmax); //Fit Range
    gStyle->SetOptFit(0);
    prof01->SetMarkerColor(kGreen+1);
    prof01->SetLineColor(kGreen+1);
    prof01->SetMarkerSize(0.5);
    TF1 *form02 = prof01->GetListOfFunctions()->FindObject("pol1");
    if (form02) { 
      form02->SetLineColor(kRed+1); 
      form02->SetLineWidth(6); 
      form02->SetRange(fitmin,fitmax);
      h2d01->Draw("colz");
      prof01->Draw("ep same");
      
      latex->SetTextAlign(32);
      latex->SetTextSize(0.050);
      latex->DrawLatex(0.85,0.88,Form("%s, %s %2.f GeV, |#eta|<0.8",ihcalType.c_str(),ptlType.c_str(),true_p));
      latex->DrawLatex(0.85,0.81,"(3x3 towers)");
      latex->SetTextAlign(12);
      latex->SetTextSize(0.040);
      latex->SetTextColor(kRed);
      latex->DrawLatex(0.20,0.88,Form("#chi^{2}/ndf: %.2f/%d",form02->GetChisquare(),form02->GetNDF()));
      latex->DrawLatex(0.20,0.82,Form("p0: %.3f #pm %.3f",form02->GetParameter(0),form02->GetParError(0)));
      latex->DrawLatex(0.20,0.76,Form("p1: %.3f #pm %.3f",form02->GetParameter(1),form02->GetParError(1)));
    }
  }
  c01->SaveAs(Form("%s/%s_2D_HCALE_vs_IOAsymm_%.0f_%.0f_%.0f_mipcut_%.0f_%.0f_totcut%.0f.pdf",outDir.c_str(),inType.c_str(),(int)do_cemcScale*cemc_sf*100,(int)do_ihcalScale*ihcal_sf*100,(int)do_ohcalScale*ohcal_sf*100,(int)do_cemcMipCut*cemcCut*100,(int)do_ihcalMipCut*ihcalCut*100,(int)do_totMipCut*totCut*100));
  
  return; 
}
