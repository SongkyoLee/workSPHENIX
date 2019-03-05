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
#include "TPaveStats.h"
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
#include "TError.h"

#include "TPaletteAxis.h"
#include "TLegend.h"
#include "TLatex.h"

///////// Al //////
//1p00_6p03_1p38
//1p38_6p03_1p38
//1p00_7p75_1p35
//2p00_7p75_1p35
///////////////////
///////// SS //////
//1p00_2p52_1p39
//1p39_2p52_1p39
//1p00_2p57_1p50
//1p74_2p57_1p50
///////////////////

//void draw_modi(TString ihcalType = "MyTree_SS310_NotScaled")
void draw_modi(TString ihcalType = "MyTree_Al_NotScaled")
{

  //gStyle->SetOptStat(1);
  gStyle->SetPalette(51);
  //gStyle->SetPalette(55);
  //gStyle->SetPalette(1);

  //float cal_min = -0.6;
  //float cal_max = 0.6;
  //float em_min = 0.05;
  //float em_max = 0.6;
  float cal_min = -0.6;
  float cal_max = 0.6;
  float em_min = 0.05;
  float em_max = 0.6;
    
  TString fname = Form("./outModi/CEMCscale_%s_1p00_7p75_1p35.root",ihcalType.Data());
  TFile* fin = new TFile(fname);
  cout << "fin : " << fin << endl; 
  TH2D* h2D_modi_JES_calfrac = (TH2D*)fin->Get("h2D_modi_JES_calfrac");
  TH2D* h2D_modi_JES_emfrac = (TH2D*)fin->Get("h2D_modi_JES_emfrac");

  ////////////////////////////////////////////////////////
  TLegend* leg = new TLegend(0.71, 0.54, 0.89, 0.74);
  TLegend* leg2 = new TLegend(0.68, 0.64, 0.89, 0.74);
  TLegend* leg3 = new TLegend(0.19, 0.71, 0.59, 0.81);
  TLegend* leg4 = new TLegend(0.21, 0.24, 0.49, 0.54);
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.045);
  
  ////////////////////////////////////////////////////////
  TPaletteAxis* pal = new TPaletteAxis();
        
  TCanvas* c_calfrac = new TCanvas("c_calfrac","c_calfrac",700,500);
  gPad->SetRightMargin(3.0);
  h2D_modi_JES_calfrac->GetXaxis()->CenterTitle(1);
  h2D_modi_JES_calfrac->GetYaxis()->CenterTitle(1);
  h2D_modi_JES_calfrac->Draw("colz");
  c_calfrac->Update(); // mandatory to find palette!!
  pal = (TPaletteAxis*)h2D_modi_JES_calfrac->GetListOfFunctions()->FindObject("palette");
  pal->SetX2NDC(0.92);
  //// fit
  TProfile *prof_calfrac = h2D_modi_JES_calfrac->ProfileX();
  prof_calfrac->Fit("pol4","","",cal_min,cal_max); //Fit Range
  gStyle->SetOptFit(1);
  prof_calfrac->SetMarkerColor(kOrange+1);
  prof_calfrac->SetLineColor(kOrange+1);
  prof_calfrac->SetMarkerSize(0.5);
  TF1 *f_calfrac = prof_calfrac->GetListOfFunctions()->FindObject("pol4");
  if (f_calfrac) { 
    f_calfrac->SetLineColor(kRed+1); 
    f_calfrac->SetLineWidth(2); 
    f_calfrac->SetRange(cal_min,cal_max);
  }
  h2D_modi_JES_calfrac->Draw("colz");
  prof_calfrac->Draw("ep same");
  latex->SetTextColor(kRed);
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.33,0.33,Form("#chi^{2}/ndf: %.2f/%d",f_calfrac->GetChisquare(),f_calfrac->GetNDF()));
  latex->DrawLatex(0.33,0.27,Form("p0: %.3f #pm %.3f",f_calfrac->GetParameter(0),f_calfrac->GetParError(0)));
  latex->DrawLatex(0.33,0.21,Form("p1: %.3f #pm %.3f",f_calfrac->GetParameter(1),f_calfrac->GetParError(1)));
  latex->DrawLatex(0.65,0.33,Form("p2: %.3f #pm %.3f",f_calfrac->GetParameter(2),f_calfrac->GetParError(2)));
  latex->DrawLatex(0.65,0.27,Form("p3: %.3f #pm %.3f",f_calfrac->GetParameter(3),f_calfrac->GetParError(3)));
  latex->DrawLatex(0.65,0.21,Form("p4: %.3f #pm %.3f",f_calfrac->GetParameter(4),f_calfrac->GetParError(4)));
  c_calfrac->SaveAs(Form("./outModi/%s_modi_JES_calfrac_1p00_7p75_1p35.pdf",ihcalType.Data()));
  
  ////////////////////////////////////////////////////////

  TCanvas* c_emfrac = new TCanvas("c_emfrac","c_emfrac",700,500);
  gPad->SetRightMargin(3.0);
  h2D_modi_JES_emfrac->GetXaxis()->CenterTitle(1);
  h2D_modi_JES_emfrac->GetYaxis()->CenterTitle(1);
  h2D_modi_JES_emfrac->Draw("colz");
  c_emfrac->Update(); // mandatory to find palette!!
  pal = (TPaletteAxis*)h2D_modi_JES_emfrac->GetListOfFunctions()->FindObject("palette");
  pal->SetX2NDC(0.92);
  //// fit
  TProfile *prof_emfrac = h2D_modi_JES_emfrac->ProfileX();
  //prof_emfrac->Fit("pol1");
  prof_emfrac->Fit("pol1","","",em_min,em_max); // Fit Range
  gStyle->SetOptFit(1);
  TPaveStats *stat_emfrac = (TPaveStats*)(prof_emfrac->GetListOfFunctions()->FindObject("stats"));
  cout << "stat_emfrac:" << stat_emfrac << endl;
  stat_emfrac->SetTextColor(kRed);
  stat_emfrac->SetTextSize(0.040);
  stat_emfrac->SetLineColor(0);
  stat_emfrac->SetBorderSize(0);
//  stat_emfrac->SetFillColor(0);
  stat_emfrac->SetFillStyle(1001);
  stat_emfrac->SetX1NDC(0.5); 
  stat_emfrac->SetX2NDC(0.85);
  stat_emfrac->SetY1NDC(0.2); 
  stat_emfrac->SetY2NDC(0.4);
  prof_emfrac->SetMarkerColor(kOrange+1);
  prof_emfrac->SetLineColor(kOrange+1);
  prof_emfrac->SetMarkerSize(0.5);
  TF1 *f_emfrac = prof_emfrac->GetListOfFunctions()->FindObject("pol1");
  if (f_emfrac) { 
    f_emfrac->SetLineColor(kRed+1); 
    f_emfrac->SetLineWidth(2); 
    f_emfrac->SetRange(em_min,em_max);
  }
  h2D_modi_JES_emfrac->Draw("colz");
  prof_emfrac->Draw("ep same");
  c_emfrac->SaveAs(Form("./outModi/%s_modi_JES_emfrac_1p00_7p75_1p35.pdf",ihcalType.Data()));

  return; 
}
