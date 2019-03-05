
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

float draw_2D_JtoG_emfrac(string ihcalType = "SS310",
                    string jetE = "50GeV",
                    string inDir = "outJet4Matched",
                    string outDir = "outJet4_2DJtoGemfrac",
                    int initfile=0,
                    int endfile =1000,
                    float c_tot=1.00,
                    float c_em=1.00,
                    float c_h = 1.00,
                    float c_ih=1.00)
{
  //// E = c_tot ( c_em * E_cemc + c_h ( c_ih * C_ihcal + C_ohcal) )
  float cemc_sf =c_tot*c_em;
  float ihcal_sf =c_tot*c_h*c_ih;
  float ohcal_sf =c_tot*c_h;
  
  //gStyle->SetPalette(1);
  //gStyle->SetPalette(51);
  gStyle->SetPalette(55);
  
  TFile* fin = new TFile(Form("%s%s/gammajet_matched_%s_%s_scale_%.0f_%.0f_%.0f.root",inDir.c_str(),jetE.c_str(),jetE.c_str(),ihcalType.c_str(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100),"READ");
  cout << fin << endl;
  //TLegend* leg = new TLegend(0.71, 0.54, 0.89, 0.74);
  TLegend* leg = new TLegend(0.23, 0.75, 0.50, 0.90);

  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(32);
  latex->SetTextSize(0.045);
  
  TPaletteAxis* pal = new TPaletteAxis();

  //// 1) totE_emfrac
  float fitmin=0.0;
  //float fitmax=0.8;
  float fitmax=1.0;
  //float fitmin=-0.5;
  //float fitmax=0.5;
  TH2D* h2D_emfrac = (TH2D*)fin->Get("h2D_JtoG_totE_emfrac"); 
  cout << h2D_emfrac << endl;
 
  TCanvas* c_emfrac = new TCanvas("c_emfrac","",700,500);
  gPad->SetRightMargin(3.0);
  h2D_emfrac->GetXaxis()->CenterTitle(1);
  h2D_emfrac->GetYaxis()->CenterTitle(1);
  h2D_emfrac->Draw("colz");
  c_emfrac->Update(); // mandatory to find palette!!
  pal = (TPaletteAxis*)h2D_emfrac->GetListOfFunctions()->FindObject("palette");
  pal->SetX2NDC(0.92);
  
  TProfile *prof_emfrac = h2D_emfrac->ProfileX(); 
  prof_emfrac->Fit("pol1","","",fitmin,fitmax); //Fit Range
  gStyle->SetOptFit(0);
  prof_emfrac->SetMarkerColor(kOrange+1); 
  prof_emfrac->SetMarkerSize(1.); 
  TF1 *f_emfrac = prof_emfrac->GetListOfFunctions()->FindObject("pol1");
  if(f_emfrac){
    f_emfrac->SetLineColor(kRed+1);
    f_emfrac->SetLineWidth(5);
    f_emfrac->SetRange(fitmin,fitmax);
    h2D_emfrac->Draw("colz");
    prof_emfrac->Draw("ep same");
    
    latex->SetTextAlign(32);
    latex->SetTextSize(0.050);
    //latex->DrawLatex(0.85,0.88,Form("%s",ihcalType.c_str()));
    if (jetE.compare("20GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 20 < E_{Truth}^{#gamma} < 30 GeV",ihcalType.c_str())); }
    else if (jetE.compare("30GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 30 < E_{Truth}^{#gamma} < 40 GeV",ihcalType.c_str())); }
    else if (jetE.compare("40GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 40 < E_{Truth}^{#gamma} < 50 GeV",ihcalType.c_str())); }
    else if (jetE.compare("50GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 50 < E_{Truth}^{#gamma} < 60 GeV",ihcalType.c_str())); }
    latex->SetTextAlign(12);
    latex->SetTextSize(0.040);
    latex->DrawLatex(0.20,0.88,Form("#chi^{2}/ndf: %.2f/%d",f_emfrac->GetChisquare(),f_emfrac->GetNDF()));
    latex->DrawLatex(0.20,0.82,Form("p0: %.3f #pm %.3f",f_emfrac->GetParameter(0),f_emfrac->GetParError(0)));
    latex->DrawLatex(0.20,0.76,Form("p1: %.3f #pm %.3f",f_emfrac->GetParameter(1),f_emfrac->GetParError(1)));
  }

  c_emfrac->SaveAs(Form("./%s%s/%s_%s_JtoG_emfrac_scale_%.0f_%.0f_%.0f.pdf",outDir.c_str(),jetE.c_str(),jetE.c_str(),ihcalType.c_str(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100));
  
  delete h2D_emfrac; 
  delete c_emfrac;
  
  return f_emfrac->GetParameter(1);
   
}
