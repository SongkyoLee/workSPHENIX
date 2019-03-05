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

void draw_qg_emfrac_jet4(string ihcalType = "SS310", string jetE="50GeV", int initfile=0,int endfile =300, float cemc_sf=1.00, float ihcal_sf=1.00, float ohcal_sf=1.00)
{

  //gStyle->SetOptStat(1);
  //gStyle->SetPalette(51);
  gStyle->SetPalette(55);

  //float cal_min = -0.6;
  //float cal_max = 0.6;
  //float em_min = 0.05;
  //float em_max = 0.6;
  float cal_min = -0.6;
  float cal_max = 0.6;
//  float em_min = 0.05;
//  float em_max = 0.6;
  float em_min = 0.0;
  //float em_max = 0.8;
  float em_max = 1.0;
    
  TString fname = Form("./outJet4QG/%s_%s_ByGammaJet_%.0f_%.0f_%.0f.root",jetE.c_str(),ihcalType.c_str(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100);
  TFile* fin = new TFile(fname);
  cout << "fin : " << fin << endl; 
  TH2D* h2D_JES_calfrac = (TH2D*)fin->Get("h2D_JES_calfrac");
  TH2D* h2D_JES_emfrac = (TH2D*)fin->Get("h2D_JES_emfrac");
  TH2D* h2D_modi_JES_calfrac = (TH2D*)fin->Get("h2D_modi_JES_calfrac");
  TH2D* h2D_modi_JES_emfrac = (TH2D*)fin->Get("h2D_modi_JES_emfrac");
  TH2D* h2D_quark_JES_calfrac = (TH2D*)fin->Get("h2D_quark_JES_calfrac");
  TH2D* h2D_quark_JES_emfrac = (TH2D*)fin->Get("h2D_quark_JES_emfrac");
  TH2D* h2D_quark_modi_JES_calfrac = (TH2D*)fin->Get("h2D_quark_modi_JES_calfrac");
  TH2D* h2D_quark_modi_JES_emfrac = (TH2D*)fin->Get("h2D_quark_modi_JES_emfrac");
  TH2D* h2D_gluon_JES_calfrac = (TH2D*)fin->Get("h2D_gluon_JES_calfrac");
  TH2D* h2D_gluon_JES_emfrac = (TH2D*)fin->Get("h2D_gluon_JES_emfrac");
  TH2D* h2D_gluon_modi_JES_calfrac = (TH2D*)fin->Get("h2D_gluon_modi_JES_calfrac");
  TH2D* h2D_gluon_modi_JES_emfrac = (TH2D*)fin->Get("h2D_gluon_modi_JES_emfrac");

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
  
  /////////////      
  TCanvas* c_calfrac = new TCanvas("c_calfrac","c_calfrac",700,500);
  gPad->SetRightMargin(3.0);
  h2D_JES_calfrac->GetXaxis()->CenterTitle(1);
  h2D_JES_calfrac->GetYaxis()->CenterTitle(1);
  h2D_JES_calfrac->Draw("colz");
  c_calfrac->Update(); // mandatory to find palette!!
  pal = (TPaletteAxis*)h2D_JES_calfrac->GetListOfFunctions()->FindObject("palette");
  pal->SetX2NDC(0.92);
  //// fit
  TProfile *prof_calfrac = h2D_JES_calfrac->ProfileX();
  prof_calfrac->Fit("pol4","","",cal_min,cal_max); //Fit Range
  gStyle->SetOptFit(1);
  prof_calfrac->SetMarkerColor(kOrange+1);
  prof_calfrac->SetLineColor(kOrange+1);
  prof_calfrac->SetMarkerSize(0.5);
  TF1 *f_calfrac = prof_calfrac->GetListOfFunctions()->FindObject("pol4");
  if (f_calfrac) { 
    f_calfrac->SetLineColor(kRed+1); 
    f_calfrac->SetLineWidth(5); 
    f_calfrac->SetRange(cal_min,cal_max);
  }
  h2D_JES_calfrac->Draw("colz");
  prof_calfrac->Draw("ep same");
  latex->SetTextColor(kBlack);
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.33,0.33,Form("#chi^{2}/ndf: %.2f/%d",f_calfrac->GetChisquare(),f_calfrac->GetNDF()));
  latex->DrawLatex(0.33,0.27,Form("p0: %.3f #pm %.3f",f_calfrac->GetParameter(0),f_calfrac->GetParError(0)));
  latex->DrawLatex(0.33,0.21,Form("p1: %.3f #pm %.3f",f_calfrac->GetParameter(1),f_calfrac->GetParError(1)));
  latex->DrawLatex(0.65,0.33,Form("p2: %.3f #pm %.3f",f_calfrac->GetParameter(2),f_calfrac->GetParError(2)));
  latex->DrawLatex(0.65,0.27,Form("p3: %.3f #pm %.3f",f_calfrac->GetParameter(3),f_calfrac->GetParError(3)));
  latex->DrawLatex(0.65,0.21,Form("p4: %.3f #pm %.3f",f_calfrac->GetParameter(4),f_calfrac->GetParError(4)));
  //c_calfrac->SaveAs(Form("./outJet4QGemfrac%s/%s_JES_calfrac_%.0f_%.0f_%.0f.pdf",jetE.c_str(),ihcalType.c_str(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100));
  ////////////////////////////////////////////////////////
  TCanvas* c_emfrac = new TCanvas("c_emfrac","c_emfrac",700,500);
  gPad->SetRightMargin(3.0);
  h2D_JES_emfrac->GetXaxis()->CenterTitle(1);
  h2D_JES_emfrac->GetYaxis()->CenterTitle(1);
//  h2D_JES_emfrac->SetMaximum(450); //test
  h2D_JES_emfrac->Draw("colz");
  c_emfrac->Update(); // mandatory to find palette!!
  pal = (TPaletteAxis*)h2D_JES_emfrac->GetListOfFunctions()->FindObject("palette");
  pal->SetX2NDC(0.92);
  //// fit
  TProfile *prof_emfrac = h2D_JES_emfrac->ProfileX();
  //prof_emfrac->Fit("pol1");
  prof_emfrac->Fit("pol1","","",em_min,em_max); // Fit Range
  gStyle->SetOptFit(1);
  TPaveStats *stat_emfrac = (TPaveStats*)(prof_emfrac->GetListOfFunctions()->FindObject("stats"));
  cout << "stat_emfrac:" << stat_emfrac << endl;
  stat_emfrac->SetTextColor(kBlack);
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
    f_emfrac->SetLineWidth(5); 
    f_emfrac->SetRange(em_min,em_max);
  }
  h2D_JES_emfrac->Draw("colz");
  prof_emfrac->Draw("ep same");
  latex->SetTextAlign(32);
  latex->SetTextSize(0.050);
  if (jetE.compare("20GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 20 < E_{Truth}^{Jet} < 30 GeV",ihcalType.c_str())); }
  else if (jetE.compare("30GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 30 < E_{Truth}^{Jet} < 40 GeV",ihcalType.c_str())); }
  else if (jetE.compare("40GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 40 < E_{Truth}^{Jet} < 50 GeV",ihcalType.c_str())); }
  else if (jetE.compare("50GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 50 < E_{Truth}^{Jet} < 60 GeV",ihcalType.c_str())); }
  c_emfrac->SaveAs(Form("./outJet4QGemfrac%s/%s_JES_emfrac_%.0f_%.0f_%.0f.pdf",jetE.c_str(),ihcalType.c_str(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100));
  ////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////
  TCanvas* c_modi_calfrac = new TCanvas("c_modi_calfrac","c_modi_calfrac",700,500);
  gPad->SetRightMargin(3.0);
  h2D_modi_JES_calfrac->GetXaxis()->CenterTitle(1);
  h2D_modi_JES_calfrac->GetYaxis()->CenterTitle(1);
  h2D_modi_JES_calfrac->Draw("colz");
  c_modi_calfrac->Update(); // mandatory to find palette!!
  pal = (TPaletteAxis*)h2D_modi_JES_calfrac->GetListOfFunctions()->FindObject("palette");
  pal->SetX2NDC(0.92);
  //// fit
  TProfile *prof_modi_calfrac = h2D_modi_JES_calfrac->ProfileX();
  prof_modi_calfrac->Fit("pol4","","",cal_min,cal_max); //Fit Range
  gStyle->SetOptFit(1);
  prof_modi_calfrac->SetMarkerColor(kOrange+1);
  prof_modi_calfrac->SetLineColor(kOrange+1);
  prof_modi_calfrac->SetMarkerSize(0.5);
  TF1 *f_modi_calfrac = prof_modi_calfrac->GetListOfFunctions()->FindObject("pol4");
  if (f_modi_calfrac) { 
    f_modi_calfrac->SetLineColor(kRed+1); 
    f_modi_calfrac->SetLineWidth(5); 
    f_modi_calfrac->SetRange(cal_min,cal_max);
  }
  h2D_modi_JES_calfrac->Draw("colz");
  prof_modi_calfrac->Draw("ep same");
  latex->SetTextColor(kBlack);
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.33,0.33,Form("#chi^{2}/ndf: %.2f/%d",f_modi_calfrac->GetChisquare(),f_modi_calfrac->GetNDF()));
  latex->DrawLatex(0.33,0.27,Form("p0: %.3f #pm %.3f",f_modi_calfrac->GetParameter(0),f_modi_calfrac->GetParError(0)));
  latex->DrawLatex(0.33,0.21,Form("p1: %.3f #pm %.3f",f_modi_calfrac->GetParameter(1),f_modi_calfrac->GetParError(1)));
  latex->DrawLatex(0.65,0.33,Form("p2: %.3f #pm %.3f",f_modi_calfrac->GetParameter(2),f_modi_calfrac->GetParError(2)));
  latex->DrawLatex(0.65,0.27,Form("p3: %.3f #pm %.3f",f_modi_calfrac->GetParameter(3),f_modi_calfrac->GetParError(3)));
  latex->DrawLatex(0.65,0.21,Form("p4: %.3f #pm %.3f",f_modi_calfrac->GetParameter(4),f_modi_calfrac->GetParError(4)));
  //c_modi_calfrac->SaveAs(Form("./outJet4QGemfrac%s/%s_modi_JES_calfrac_%.0f_%.0f_%.0f.pdf",jetE.c_str(),ihcalType.c_str(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100));
  ////////////////////////////////////////////////////////
  TCanvas* c_modi_emfrac = new TCanvas("c_modi_emfrac","c_modi_emfrac",700,500);
  gPad->SetRightMargin(3.0);
  h2D_modi_JES_emfrac->GetXaxis()->CenterTitle(1);
  h2D_modi_JES_emfrac->GetYaxis()->CenterTitle(1);
//  h2D_modi_JES_emfrac->SetMaximum(450); //test
  h2D_modi_JES_emfrac->Draw("colz");
  c_modi_emfrac->Update(); // mandatory to find palette!!
  pal = (TPaletteAxis*)h2D_modi_JES_emfrac->GetListOfFunctions()->FindObject("palette");
  pal->SetX2NDC(0.92);
  //// fit
  TProfile *prof_modi_emfrac = h2D_modi_JES_emfrac->ProfileX();
  //prof_modi_emfrac->Fit("pol1");
  prof_modi_emfrac->Fit("pol1","","",em_min,em_max); // Fit Range
  gStyle->SetOptFit(1);
  TPaveStats *stat_modi_emfrac = (TPaveStats*)(prof_modi_emfrac->GetListOfFunctions()->FindObject("stats"));
  cout << "stat_modi_emfrac:" << stat_modi_emfrac << endl;
  stat_modi_emfrac->SetTextColor(kBlack);
  stat_modi_emfrac->SetTextSize(0.040);
  stat_modi_emfrac->SetLineColor(0);
  stat_modi_emfrac->SetBorderSize(0);
//  stat_modi_emfrac->SetFillColor(0);
  stat_modi_emfrac->SetFillStyle(1001);
  stat_modi_emfrac->SetX1NDC(0.5); 
  stat_modi_emfrac->SetX2NDC(0.85);
  stat_modi_emfrac->SetY1NDC(0.2); 
  stat_modi_emfrac->SetY2NDC(0.4);
  prof_modi_emfrac->SetMarkerColor(kOrange+1);
  prof_modi_emfrac->SetLineColor(kOrange+1);
  prof_modi_emfrac->SetMarkerSize(0.5);
  TF1 *f_modi_emfrac = prof_modi_emfrac->GetListOfFunctions()->FindObject("pol1");
  if (f_modi_emfrac) { 
    f_modi_emfrac->SetLineColor(kRed+1); 
    f_modi_emfrac->SetLineWidth(5); 
    f_modi_emfrac->SetRange(em_min,em_max);
  }
  h2D_modi_JES_emfrac->Draw("colz");
  prof_modi_emfrac->Draw("ep same");
  latex->SetTextAlign(32);
  latex->SetTextSize(0.050);
  if (jetE.compare("20GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 20 < E_{Truth}^{Jet} < 30 GeV",ihcalType.c_str())); }
  else if (jetE.compare("30GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 30 < E_{Truth}^{Jet} < 40 GeV",ihcalType.c_str())); }
  else if (jetE.compare("40GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 40 < E_{Truth}^{Jet} < 50 GeV",ihcalType.c_str())); }
  else if (jetE.compare("50GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 50 < E_{Truth}^{Jet} < 60 GeV",ihcalType.c_str())); }
  c_modi_emfrac->SaveAs(Form("./outJet4QGemfrac%s/%s_modi_JES_emfrac_%.0f_%.0f_%.0f.pdf",jetE.c_str(),ihcalType.c_str(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100));
  ////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////

  /////////////     quark    ////////////////////// 
  TCanvas* c_quark_calfrac = new TCanvas("c_quark_calfrac","c_quark_calfrac",700,500);
  gPad->SetRightMargin(3.0);
  h2D_quark_JES_calfrac->GetXaxis()->CenterTitle(1);
  h2D_quark_JES_calfrac->GetYaxis()->CenterTitle(1);
  h2D_quark_JES_calfrac->Draw("colz");
  c_quark_calfrac->Update(); // mandatory to find palette!!
  pal = (TPaletteAxis*)h2D_quark_JES_calfrac->GetListOfFunctions()->FindObject("palette");
  pal->SetX2NDC(0.92);
  //// fit
  TProfile *prof_quark_calfrac = h2D_quark_JES_calfrac->ProfileX();
  prof_quark_calfrac->Fit("pol4","","",cal_min,cal_max); //Fit Range
  gStyle->SetOptFit(1);
  prof_quark_calfrac->SetMarkerColor(kOrange+1);
  prof_quark_calfrac->SetLineColor(kOrange+1);
  prof_quark_calfrac->SetMarkerSize(0.5);
  TF1 *f_quark_calfrac = prof_quark_calfrac->GetListOfFunctions()->FindObject("pol4");
  if (f_quark_calfrac) { 
    f_quark_calfrac->SetLineColor(kRed+1); 
    f_quark_calfrac->SetLineWidth(5); 
    f_quark_calfrac->SetRange(cal_min,cal_max);
  }
  h2D_quark_JES_calfrac->Draw("colz");
  prof_quark_calfrac->Draw("ep same");
  latex->SetTextColor(kBlack);
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.33,0.33,Form("#chi^{2}/ndf: %.2f/%d",f_quark_calfrac->GetChisquare(),f_quark_calfrac->GetNDF()));
  latex->DrawLatex(0.33,0.27,Form("p0: %.3f #pm %.3f",f_quark_calfrac->GetParameter(0),f_quark_calfrac->GetParError(0)));
  latex->DrawLatex(0.33,0.21,Form("p1: %.3f #pm %.3f",f_quark_calfrac->GetParameter(1),f_quark_calfrac->GetParError(1)));
  latex->DrawLatex(0.65,0.33,Form("p2: %.3f #pm %.3f",f_quark_calfrac->GetParameter(2),f_quark_calfrac->GetParError(2)));
  latex->DrawLatex(0.65,0.27,Form("p3: %.3f #pm %.3f",f_quark_calfrac->GetParameter(3),f_quark_calfrac->GetParError(3)));
  latex->DrawLatex(0.65,0.21,Form("p4: %.3f #pm %.3f",f_quark_calfrac->GetParameter(4),f_quark_calfrac->GetParError(4)));
//  c_quark_calfrac->SaveAs(Form("./outJet4QGemfrac%s/%s_quark_JES_calfrac_%.0f_%.0f_%.0f.pdf",jetE.c_str(),ihcalType.c_str(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100));
  ////////////////////////////////////////////////////////
  TCanvas* c_quark_emfrac = new TCanvas("c_quark_emfrac","c_quark_emfrac",700,500);
  gPad->SetRightMargin(3.0);
  h2D_quark_JES_emfrac->GetXaxis()->CenterTitle(1);
  h2D_quark_JES_emfrac->GetYaxis()->CenterTitle(1);
  h2D_quark_JES_emfrac->Draw("colz");
  c_quark_emfrac->Update(); // mandatory to find palette!!
  pal = (TPaletteAxis*)h2D_quark_JES_emfrac->GetListOfFunctions()->FindObject("palette");
  pal->SetX2NDC(0.92);
  //// fit
  TProfile *prof_quark_emfrac = h2D_quark_JES_emfrac->ProfileX();
  //prof_quark_emfrac->Fit("pol1");
  prof_quark_emfrac->Fit("pol1","","",em_min,em_max); // Fit Range
  gStyle->SetOptFit(1);
  TPaveStats *stat_quark_emfrac = (TPaveStats*)(prof_quark_emfrac->GetListOfFunctions()->FindObject("stats"));
  cout << "stat_quark_emfrac:" << stat_quark_emfrac << endl;
  stat_quark_emfrac->SetTextColor(kBlack);
  stat_quark_emfrac->SetTextSize(0.040);
  stat_quark_emfrac->SetLineColor(0);
  stat_quark_emfrac->SetBorderSize(0);
//  stat_quark_emfrac->SetFillColor(0);
  stat_quark_emfrac->SetFillStyle(1001);
  stat_quark_emfrac->SetX1NDC(0.5); 
  stat_quark_emfrac->SetX2NDC(0.85);
  stat_quark_emfrac->SetY1NDC(0.2); 
  stat_quark_emfrac->SetY2NDC(0.4);
  prof_quark_emfrac->SetMarkerColor(kOrange+1);
  prof_quark_emfrac->SetLineColor(kOrange+1);
  prof_quark_emfrac->SetMarkerSize(0.5);
  TF1 *f_quark_emfrac = prof_quark_emfrac->GetListOfFunctions()->FindObject("pol1");
  if (f_quark_emfrac) { 
    f_quark_emfrac->SetLineColor(kRed+1); 
    f_quark_emfrac->SetLineWidth(5); 
    f_quark_emfrac->SetRange(em_min,em_max);
  }
  h2D_quark_JES_emfrac->Draw("colz");
  prof_quark_emfrac->Draw("ep same");
  latex->SetTextAlign(32);
  latex->SetTextSize(0.050);
  if (jetE.compare("20GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 20 < E_{Truth}^{Jet} < 30 GeV",ihcalType.c_str())); }
  else if (jetE.compare("30GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 30 < E_{Truth}^{Jet} < 40 GeV",ihcalType.c_str())); }
  else if (jetE.compare("40GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 40 < E_{Truth}^{Jet} < 50 GeV",ihcalType.c_str())); }
  else if (jetE.compare("50GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 50 < E_{Truth}^{Jet} < 60 GeV",ihcalType.c_str())); }
  c_quark_emfrac->SaveAs(Form("./outJet4QGemfrac%s/%s_quark_JES_emfrac_%.0f_%.0f_%.0f.pdf",jetE.c_str(),ihcalType.c_str(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100));
  ////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////
  TCanvas* c_quark_modi_calfrac = new TCanvas("c_quark_modi_calfrac","c_quark_modi_calfrac",700,500);
  gPad->SetRightMargin(3.0);
  h2D_quark_modi_JES_calfrac->GetXaxis()->CenterTitle(1);
  h2D_quark_modi_JES_calfrac->GetYaxis()->CenterTitle(1);
  h2D_quark_modi_JES_calfrac->Draw("colz");
  c_quark_modi_calfrac->Update(); // mandatory to find palette!!
  pal = (TPaletteAxis*)h2D_quark_modi_JES_calfrac->GetListOfFunctions()->FindObject("palette");
  pal->SetX2NDC(0.92);
  //// fit
  TProfile *prof_quark_modi_calfrac = h2D_quark_modi_JES_calfrac->ProfileX();
  prof_quark_modi_calfrac->Fit("pol4","","",cal_min,cal_max); //Fit Range
  gStyle->SetOptFit(1);
  prof_quark_modi_calfrac->SetMarkerColor(kOrange+1);
  prof_quark_modi_calfrac->SetLineColor(kOrange+1);
  prof_quark_modi_calfrac->SetMarkerSize(0.5);
  TF1 *f_quark_modi_calfrac = prof_quark_modi_calfrac->GetListOfFunctions()->FindObject("pol4");
  if (f_quark_modi_calfrac) { 
    f_quark_modi_calfrac->SetLineColor(kRed+1); 
    f_quark_modi_calfrac->SetLineWidth(5); 
    f_quark_modi_calfrac->SetRange(cal_min,cal_max);
  }
  h2D_quark_modi_JES_calfrac->Draw("colz");
  prof_quark_modi_calfrac->Draw("ep same");
  latex->SetTextColor(kBlack);
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.33,0.33,Form("#chi^{2}/ndf: %.2f/%d",f_quark_modi_calfrac->GetChisquare(),f_quark_modi_calfrac->GetNDF()));
  latex->DrawLatex(0.33,0.27,Form("p0: %.3f #pm %.3f",f_quark_modi_calfrac->GetParameter(0),f_quark_modi_calfrac->GetParError(0)));
  latex->DrawLatex(0.33,0.21,Form("p1: %.3f #pm %.3f",f_quark_modi_calfrac->GetParameter(1),f_quark_modi_calfrac->GetParError(1)));
  latex->DrawLatex(0.65,0.33,Form("p2: %.3f #pm %.3f",f_quark_modi_calfrac->GetParameter(2),f_quark_modi_calfrac->GetParError(2)));
  latex->DrawLatex(0.65,0.27,Form("p3: %.3f #pm %.3f",f_quark_modi_calfrac->GetParameter(3),f_quark_modi_calfrac->GetParError(3)));
  latex->DrawLatex(0.65,0.21,Form("p4: %.3f #pm %.3f",f_quark_modi_calfrac->GetParameter(4),f_quark_modi_calfrac->GetParError(4)));
//  c_quark_modi_calfrac->SaveAs(Form("./outJet4QGemfrac%s/%s_quark_modi_JES_calfrac_%.0f_%.0f_%.0f.pdf",jetE.c_str(),ihcalType.c_str(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100));
  ////////////////////////////////////////////////////////
  TCanvas* c_quark_modi_emfrac = new TCanvas("c_quark_modi_emfrac","c_quark_modi_emfrac",700,500);
  gPad->SetRightMargin(3.0);
  h2D_quark_modi_JES_emfrac->GetXaxis()->CenterTitle(1);
  h2D_quark_modi_JES_emfrac->GetYaxis()->CenterTitle(1);
  h2D_quark_modi_JES_emfrac->Draw("colz");
  c_quark_modi_emfrac->Update(); // mandatory to find palette!!
  pal = (TPaletteAxis*)h2D_quark_modi_JES_emfrac->GetListOfFunctions()->FindObject("palette");
  pal->SetX2NDC(0.92);
  //// fit
  TProfile *prof_quark_modi_emfrac = h2D_quark_modi_JES_emfrac->ProfileX();
  //prof_quark_modi_emfrac->Fit("pol1");
  prof_quark_modi_emfrac->Fit("pol1","","",em_min,em_max); // Fit Range
  gStyle->SetOptFit(1);
  TPaveStats *stat_quark_modi_emfrac = (TPaveStats*)(prof_quark_modi_emfrac->GetListOfFunctions()->FindObject("stats"));
  cout << "stat_quark_modi_emfrac:" << stat_quark_modi_emfrac << endl;
  stat_quark_modi_emfrac->SetTextColor(kBlack);
  stat_quark_modi_emfrac->SetTextSize(0.040);
  stat_quark_modi_emfrac->SetLineColor(0);
  stat_quark_modi_emfrac->SetBorderSize(0);
//  stat_quark_modi_emfrac->SetFillColor(0);
  stat_quark_modi_emfrac->SetFillStyle(1001);
  stat_quark_modi_emfrac->SetX1NDC(0.5); 
  stat_quark_modi_emfrac->SetX2NDC(0.85);
  stat_quark_modi_emfrac->SetY1NDC(0.2); 
  stat_quark_modi_emfrac->SetY2NDC(0.4);
  prof_quark_modi_emfrac->SetMarkerColor(kOrange+1);
  prof_quark_modi_emfrac->SetLineColor(kOrange+1);
  prof_quark_modi_emfrac->SetMarkerSize(0.5);
  TF1 *f_quark_modi_emfrac = prof_quark_modi_emfrac->GetListOfFunctions()->FindObject("pol1");
  if (f_quark_modi_emfrac) { 
    f_quark_modi_emfrac->SetLineColor(kRed+1); 
    f_quark_modi_emfrac->SetLineWidth(5); 
    f_quark_modi_emfrac->SetRange(em_min,em_max);
  }
  h2D_quark_modi_JES_emfrac->Draw("colz");
  prof_quark_modi_emfrac->Draw("ep same");
  latex->SetTextAlign(32);
  latex->SetTextSize(0.050);
  if (jetE.compare("20GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 20 < E_{Truth}^{Jet} < 30 GeV",ihcalType.c_str())); }
  else if (jetE.compare("30GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 30 < E_{Truth}^{Jet} < 40 GeV",ihcalType.c_str())); }
  else if (jetE.compare("40GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 40 < E_{Truth}^{Jet} < 50 GeV",ihcalType.c_str())); }
  else if (jetE.compare("50GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 50 < E_{Truth}^{Jet} < 60 GeV",ihcalType.c_str())); }
  c_quark_modi_emfrac->SaveAs(Form("./outJet4QGemfrac%s/%s_quark_modi_JES_emfrac_%.0f_%.0f_%.0f.pdf",jetE.c_str(),ihcalType.c_str(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100));
  
  ////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////

  /////////////     gluon    ////////////////////// 
  TCanvas* c_gluon_calfrac = new TCanvas("c_gluon_calfrac","c_gluon_calfrac",700,500);
  gPad->SetRightMargin(3.0);
  h2D_gluon_JES_calfrac->GetXaxis()->CenterTitle(1);
  h2D_gluon_JES_calfrac->GetYaxis()->CenterTitle(1);
  h2D_gluon_JES_calfrac->Draw("colz");
  c_gluon_calfrac->Update(); // mandatory to find palette!!
  pal = (TPaletteAxis*)h2D_gluon_JES_calfrac->GetListOfFunctions()->FindObject("palette");
  pal->SetX2NDC(0.92);
  //// fit
  TProfile *prof_gluon_calfrac = h2D_gluon_JES_calfrac->ProfileX();
  prof_gluon_calfrac->Fit("pol4","","",cal_min,cal_max); //Fit Range
  gStyle->SetOptFit(1);
  prof_gluon_calfrac->SetMarkerColor(kOrange+1);
  prof_gluon_calfrac->SetLineColor(kOrange+1);
  prof_gluon_calfrac->SetMarkerSize(0.5);
  TF1 *f_gluon_calfrac = prof_gluon_calfrac->GetListOfFunctions()->FindObject("pol4");
  if (f_gluon_calfrac) { 
    f_gluon_calfrac->SetLineColor(kRed+1); 
    f_gluon_calfrac->SetLineWidth(5); 
    f_gluon_calfrac->SetRange(cal_min,cal_max);
  }
  h2D_gluon_JES_calfrac->Draw("colz");
  prof_gluon_calfrac->Draw("ep same");
  latex->SetTextColor(kBlack);
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.33,0.33,Form("#chi^{2}/ndf: %.2f/%d",f_gluon_calfrac->GetChisquare(),f_gluon_calfrac->GetNDF()));
  latex->DrawLatex(0.33,0.27,Form("p0: %.3f #pm %.3f",f_gluon_calfrac->GetParameter(0),f_gluon_calfrac->GetParError(0)));
  latex->DrawLatex(0.33,0.21,Form("p1: %.3f #pm %.3f",f_gluon_calfrac->GetParameter(1),f_gluon_calfrac->GetParError(1)));
  latex->DrawLatex(0.65,0.33,Form("p2: %.3f #pm %.3f",f_gluon_calfrac->GetParameter(2),f_gluon_calfrac->GetParError(2)));
  latex->DrawLatex(0.65,0.27,Form("p3: %.3f #pm %.3f",f_gluon_calfrac->GetParameter(3),f_gluon_calfrac->GetParError(3)));
  latex->DrawLatex(0.65,0.21,Form("p4: %.3f #pm %.3f",f_gluon_calfrac->GetParameter(4),f_gluon_calfrac->GetParError(4)));
//  c_gluon_calfrac->SaveAs(Form("./outJet4QGemfrac%s/%s_gluon_JES_calfrac_%.0f_%.0f_%.0f.pdf",jetE.c_str(),ihcalType.c_str(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100));
  ////////////////////////////////////////////////////////
  TCanvas* c_gluon_emfrac = new TCanvas("c_gluon_emfrac","c_gluon_emfrac",700,500);
  gPad->SetRightMargin(3.0);
  h2D_gluon_JES_emfrac->GetXaxis()->CenterTitle(1);
  h2D_gluon_JES_emfrac->GetYaxis()->CenterTitle(1);
  h2D_gluon_JES_emfrac->Draw("colz");
  c_gluon_emfrac->Update(); // mandatory to find palette!!
  pal = (TPaletteAxis*)h2D_gluon_JES_emfrac->GetListOfFunctions()->FindObject("palette");
  pal->SetX2NDC(0.92);
  //// fit
  TProfile *prof_gluon_emfrac = h2D_gluon_JES_emfrac->ProfileX();
  //prof_gluon_emfrac->Fit("pol1");
  prof_gluon_emfrac->Fit("pol1","","",em_min,em_max); // Fit Range
  gStyle->SetOptFit(1);
  TPaveStats *stat_gluon_emfrac = (TPaveStats*)(prof_gluon_emfrac->GetListOfFunctions()->FindObject("stats"));
  cout << "stat_gluon_emfrac:" << stat_gluon_emfrac << endl;
  stat_gluon_emfrac->SetTextColor(kBlack);
  stat_gluon_emfrac->SetTextSize(0.040);
  stat_gluon_emfrac->SetLineColor(0);
  stat_gluon_emfrac->SetBorderSize(0);
//  stat_gluon_emfrac->SetFillColor(0);
  stat_gluon_emfrac->SetFillStyle(1001);
  stat_gluon_emfrac->SetX1NDC(0.5); 
  stat_gluon_emfrac->SetX2NDC(0.85);
  stat_gluon_emfrac->SetY1NDC(0.2); 
  stat_gluon_emfrac->SetY2NDC(0.4);
  prof_gluon_emfrac->SetMarkerColor(kOrange+1);
  prof_gluon_emfrac->SetLineColor(kOrange+1);
  prof_gluon_emfrac->SetMarkerSize(0.5);
  TF1 *f_gluon_emfrac = prof_gluon_emfrac->GetListOfFunctions()->FindObject("pol1");
  if (f_gluon_emfrac) { 
    f_gluon_emfrac->SetLineColor(kRed+1); 
    f_gluon_emfrac->SetLineWidth(5); 
    f_gluon_emfrac->SetRange(em_min,em_max);
  }
  h2D_gluon_JES_emfrac->Draw("colz");
  prof_gluon_emfrac->Draw("ep same");
  latex->SetTextAlign(32);
  latex->SetTextSize(0.050);
  if (jetE.compare("20GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 20 < E_{Truth}^{Jet} < 30 GeV",ihcalType.c_str())); }
  else if (jetE.compare("30GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 30 < E_{Truth}^{Jet} < 40 GeV",ihcalType.c_str())); }
  else if (jetE.compare("40GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 40 < E_{Truth}^{Jet} < 50 GeV",ihcalType.c_str())); }
  else if (jetE.compare("50GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 50 < E_{Truth}^{Jet} < 60 GeV",ihcalType.c_str())); }
  c_gluon_emfrac->SaveAs(Form("./outJet4QGemfrac%s/%s_gluon_JES_emfrac_%.0f_%.0f_%.0f.pdf",jetE.c_str(),ihcalType.c_str(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100));
  ////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////
  TCanvas* c_gluon_modi_calfrac = new TCanvas("c_gluon_modi_calfrac","c_gluon_modi_calfrac",700,500);
  gPad->SetRightMargin(3.0);
  h2D_gluon_modi_JES_calfrac->GetXaxis()->CenterTitle(1);
  h2D_gluon_modi_JES_calfrac->GetYaxis()->CenterTitle(1);
  h2D_gluon_modi_JES_calfrac->Draw("colz");
  c_gluon_modi_calfrac->Update(); // mandatory to find palette!!
  pal = (TPaletteAxis*)h2D_gluon_modi_JES_calfrac->GetListOfFunctions()->FindObject("palette");
  pal->SetX2NDC(0.92);
  //// fit
  TProfile *prof_gluon_modi_calfrac = h2D_gluon_modi_JES_calfrac->ProfileX();
  prof_gluon_modi_calfrac->Fit("pol4","","",cal_min,cal_max); //Fit Range
  gStyle->SetOptFit(1);
  prof_gluon_modi_calfrac->SetMarkerColor(kOrange+1);
  prof_gluon_modi_calfrac->SetLineColor(kOrange+1);
  prof_gluon_modi_calfrac->SetMarkerSize(0.5);
  TF1 *f_gluon_modi_calfrac = prof_gluon_modi_calfrac->GetListOfFunctions()->FindObject("pol4");
  if (f_gluon_modi_calfrac) { 
    f_gluon_modi_calfrac->SetLineColor(kRed+1); 
    f_gluon_modi_calfrac->SetLineWidth(5); 
    f_gluon_modi_calfrac->SetRange(cal_min,cal_max);
  }
  h2D_gluon_modi_JES_calfrac->Draw("colz");
  prof_gluon_modi_calfrac->Draw("ep same");
  latex->SetTextColor(kBlack);
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.33,0.33,Form("#chi^{2}/ndf: %.2f/%d",f_gluon_modi_calfrac->GetChisquare(),f_gluon_modi_calfrac->GetNDF()));
  latex->DrawLatex(0.33,0.27,Form("p0: %.3f #pm %.3f",f_gluon_modi_calfrac->GetParameter(0),f_gluon_modi_calfrac->GetParError(0)));
  latex->DrawLatex(0.33,0.21,Form("p1: %.3f #pm %.3f",f_gluon_modi_calfrac->GetParameter(1),f_gluon_modi_calfrac->GetParError(1)));
  latex->DrawLatex(0.65,0.33,Form("p2: %.3f #pm %.3f",f_gluon_modi_calfrac->GetParameter(2),f_gluon_modi_calfrac->GetParError(2)));
  latex->DrawLatex(0.65,0.27,Form("p3: %.3f #pm %.3f",f_gluon_modi_calfrac->GetParameter(3),f_gluon_modi_calfrac->GetParError(3)));
  latex->DrawLatex(0.65,0.21,Form("p4: %.3f #pm %.3f",f_gluon_modi_calfrac->GetParameter(4),f_gluon_modi_calfrac->GetParError(4)));
//  c_gluon_modi_calfrac->SaveAs(Form("./outJet4QGemfrac%s/%s_gluon_modi_JES_calfrac_%.0f_%.0f_%.0f.pdf",jetE.c_str(),ihcalType.c_str(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100));
  ////////////////////////////////////////////////////////
  TCanvas* c_gluon_modi_emfrac = new TCanvas("c_gluon_modi_emfrac","c_gluon_modi_emfrac",700,500);
  gPad->SetRightMargin(3.0);
  h2D_gluon_modi_JES_emfrac->GetXaxis()->CenterTitle(1);
  h2D_gluon_modi_JES_emfrac->GetYaxis()->CenterTitle(1);
  h2D_gluon_modi_JES_emfrac->Draw("colz");
  c_gluon_modi_emfrac->Update(); // mandatory to find palette!!
  pal = (TPaletteAxis*)h2D_gluon_modi_JES_emfrac->GetListOfFunctions()->FindObject("palette");
  pal->SetX2NDC(0.92);
  //// fit
  TProfile *prof_gluon_modi_emfrac = h2D_gluon_modi_JES_emfrac->ProfileX();
  //prof_gluon_modi_emfrac->Fit("pol1");
  prof_gluon_modi_emfrac->Fit("pol1","","",em_min,em_max); // Fit Range
  gStyle->SetOptFit(1);
  TPaveStats *stat_gluon_modi_emfrac = (TPaveStats*)(prof_gluon_modi_emfrac->GetListOfFunctions()->FindObject("stats"));
  cout << "stat_gluon_modi_emfrac:" << stat_gluon_modi_emfrac << endl;
  stat_gluon_modi_emfrac->SetTextColor(kBlack);
  stat_gluon_modi_emfrac->SetTextSize(0.040);
  stat_gluon_modi_emfrac->SetLineColor(0);
  stat_gluon_modi_emfrac->SetBorderSize(0);
//  stat_gluon_modi_emfrac->SetFillColor(0);
  stat_gluon_modi_emfrac->SetFillStyle(1001);
  stat_gluon_modi_emfrac->SetX1NDC(0.5); 
  stat_gluon_modi_emfrac->SetX2NDC(0.85);
  stat_gluon_modi_emfrac->SetY1NDC(0.2); 
  stat_gluon_modi_emfrac->SetY2NDC(0.4);
  prof_gluon_modi_emfrac->SetMarkerColor(kOrange+1);
  prof_gluon_modi_emfrac->SetLineColor(kOrange+1);
  prof_gluon_modi_emfrac->SetMarkerSize(0.5);
  TF1 *f_gluon_modi_emfrac = prof_gluon_modi_emfrac->GetListOfFunctions()->FindObject("pol1");
  if (f_gluon_modi_emfrac) { 
    f_gluon_modi_emfrac->SetLineColor(kRed+1); 
    f_gluon_modi_emfrac->SetLineWidth(5); 
    f_gluon_modi_emfrac->SetRange(em_min,em_max);
  }
  h2D_gluon_modi_JES_emfrac->Draw("colz");
  prof_gluon_modi_emfrac->Draw("ep same");
  latex->SetTextAlign(32);
  latex->SetTextSize(0.050);
  if (jetE.compare("20GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 20 < E_{Truth}^{Jet} < 30 GeV",ihcalType.c_str())); }
  else if (jetE.compare("30GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 30 < E_{Truth}^{Jet} < 40 GeV",ihcalType.c_str())); }
  else if (jetE.compare("40GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 40 < E_{Truth}^{Jet} < 50 GeV",ihcalType.c_str())); }
  else if (jetE.compare("50GeV")==0) { latex->DrawLatex(0.85,0.88,Form("%s, 50 < E_{Truth}^{Jet} < 60 GeV",ihcalType.c_str())); }
  c_gluon_modi_emfrac->SaveAs(Form("./outJet4QGemfrac%s/%s_gluon_modi_JES_emfrac_%.0f_%.0f_%.0f.pdf",jetE.c_str(),ihcalType.c_str(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100));

  return; 

}
