
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

float draw_2D_EHAsymm_Al_uninst_first(string ihcalType = "Al_uninst",
                    string jetE = "50GeV",
                    string inDir = "outJet4Matched",
                    string outDir = "outJet4_2DEHAsymm_Al_uninst",
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
  gStyle->SetPalette(51);
  //gStyle->SetPalette(55);
  
  TFile* fin = new TFile(Form("%s%s/gammajet_matched_%s_%s_scale_%.0f_%.0f_%.0f.root",inDir.c_str(),jetE.c_str(),jetE.c_str(),ihcalType.c_str(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100),"READ");
  cout << fin << endl;
  //TLegend* leg = new TLegend(0.71, 0.54, 0.89, 0.74);
  TLegend* leg = new TLegend(0.23, 0.75, 0.50, 0.90);

  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(32);
  latex->SetTextSize(0.045);
  
  TPaletteAxis* pal = new TPaletteAxis();

  //// 1) totE_EHAsymm
  //float io_min=-0.8;
  //float io_max=0.8;
  float io_min=-1.0;
  //float io_max=1.0;
  float io_max=1.0;
  //if (ihcalType.compare("Al_uninst")==0){
  //  io_min=-0.6;
  //  io_max=0.6;
  //}
  TH2D* h2D_EHAsymm = (TH2D*)fin->Get("h2D_JtoG_totE_EHAsymm"); 
  cout << h2D_EHAsymm << endl;
 
  TCanvas* c_EHAsymm = new TCanvas("c_EHAsymm","",700,500);
  gPad->SetRightMargin(3.0);
  h2D_EHAsymm->GetXaxis()->CenterTitle(1);
  h2D_EHAsymm->GetXaxis()->SetRangeUser(-1,1);
  h2D_EHAsymm->GetYaxis()->CenterTitle(1);
  h2D_EHAsymm->Draw("colz");
  c_EHAsymm->Update(); // mandatory to find palette!!
  pal = (TPaletteAxis*)h2D_EHAsymm->GetListOfFunctions()->FindObject("palette");
  pal->SetX2NDC(0.92);
  
  TProfile *prof_EHAsymm = h2D_EHAsymm->ProfileX(); 
  //prof_EHAsymm->Fit("pol1","","",io_min,io_max); //Fit Range
  prof_EHAsymm->Fit("pol4","","",io_min,io_max); //Fit Range
  gStyle->SetOptFit(0);
  prof_EHAsymm->SetMarkerColor(kOrange+1); 
  prof_EHAsymm->SetMarkerSize(1.); 
  //TF1 *f_EHAsymm = prof_EHAsymm->GetListOfFunctions()->FindObject("pol1");
  TF1 *f_EHAsymm = prof_EHAsymm->GetListOfFunctions()->FindObject("pol4");
  if(f_EHAsymm){
    f_EHAsymm->SetLineColor(kRed+1);
    f_EHAsymm->SetLineWidth(5);
    f_EHAsymm->SetRange(io_min,io_max);
    h2D_EHAsymm->Draw("colz");
    prof_EHAsymm->Draw("ep same");
    
    latex->SetTextAlign(32);
    latex->SetTextSize(0.050);
    //latex->DrawLatex(0.85,0.88,Form("%s",ihcalType.c_str()));
    if (jetE.compare("20GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 20 < E_{Truth}^{#gamma} < 30 GeV",ihcalType.c_str())); }
else if (jetE.compare("30GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 30 < E_{Truth}^{#gamma} < 40 GeV",ihcalType.c_str())); }
else if (jetE.compare("40GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 40 < E_{Truth}^{#gamma} < 50 GeV",ihcalType.c_str())); }
else if (jetE.compare("50GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 50 < E_{Truth}^{#gamma} < 60 GeV",ihcalType.c_str())); } 
    latex->SetTextAlign(12);
    //latex->SetTextSize(0.040);
    latex->SetTextSize(0.035);
    latex->DrawLatex(0.20,0.88,Form("#chi^{2}/ndf: %.2f/%d",f_EHAsymm->GetChisquare(),f_EHAsymm->GetNDF()));
    latex->DrawLatex(0.20,0.83,Form("p0: %.3f #pm %.3f",f_EHAsymm->GetParameter(0),f_EHAsymm->GetParError(0)));
    latex->DrawLatex(0.20,0.78,Form("p1: %.3f #pm %.3f",f_EHAsymm->GetParameter(1),f_EHAsymm->GetParError(1)));
    latex->DrawLatex(0.20,0.73,Form("p2: %.3f #pm %.3f",f_EHAsymm->GetParameter(2),f_EHAsymm->GetParError(2)));
    latex->DrawLatex(0.20,0.68,Form("p3: %.3f #pm %.3f",f_EHAsymm->GetParameter(3),f_EHAsymm->GetParError(3)));
    latex->DrawLatex(0.20,0.63,Form("p4: %.3f #pm %.3f",f_EHAsymm->GetParameter(4),f_EHAsymm->GetParError(4)));
  }

  c_EHAsymm->SaveAs(Form("./%s/%s_%s_totE_EHAsymm_scale_%.0f_%.0f_%.0f.pdf",outDir.c_str(),jetE.c_str(),ihcalType.c_str(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100));
 
  TFile* fout = new TFile(Form("./%s/%s_%s_totE_EHAsymm_scale_%.0f_%.0f_%.0f.root",outDir.c_str(),jetE.c_str(),ihcalType.c_str(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100),"RECREATE");
  fout->cd();
  f_EHAsymm->Write();
  fout->Close(); 
  
  delete h2D_EHAsymm; 
  delete c_EHAsymm;
  
  return f_EHAsymm->GetParameter(1);
   
}
