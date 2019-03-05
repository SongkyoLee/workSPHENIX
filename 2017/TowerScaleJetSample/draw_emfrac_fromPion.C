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

//void draw_emfrac(TString ihcalType = "MyTree_Al_NotScaled")
//void draw_emfrac(TString ihcalType = "MyTree_Al_Scaled_v3")
//void draw_emfrac(TString ihcalType = "100MeV_Al_NotScaled")
void draw_emfrac_fromPion(TString ihcalType = "100MeV_Al_Scaled")
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
  float em_min = 0.01;
  float em_max = 0.65;
    
  TString fname = Form("./outEMfracFromPion/emfrac_%s.root",ihcalType.Data());
  TFile* fin = new TFile(fname);
  
  TH2D* h2D_JES_calfrac = (TH2D*)fin->Get("h2D_JES_calfrac");
  TH2D* h2D_JES_emfrac = (TH2D*)fin->Get("h2D_JES_emfrac");
  /*
  TH1D* hjet_p_nconstit = (TH1D*)fin->Get("hjet_p_nconstit");
  TH1D* hjet_p_nconstit_quark = (TH1D*)fin->Get("hjet_p_nconstit_quark");
  TH1D* hjet_p_nconstit_gluon = (TH1D*)fin->Get("hjet_p_nconstit_gluon");
  TH1D* hjet_p_nconstit_else = (TH1D*)fin->Get("hjet_p_nconstit_else");
  TH1D* hjet_trk_nconstit = (TH1D*)fin->Get("hjet_trk_nconstit");
  TH1D* hjet_trk_nconstit_quark = (TH1D*)fin->Get("hjet_trk_nconstit_quark");
  TH1D* hjet_trk_nconstit_gluon = (TH1D*)fin->Get("hjet_trk_nconstit_gluon");
  TH1D* hjet_trk_nconstit_else = (TH1D*)fin->Get("hjet_trk_nconstit_else");
  TH1D* hjet_twr_nconstit = (TH1D*)fin->Get("hjet_twr_nconstit");
  TH1D* hjet_twr_nconstit_quark = (TH1D*)fin->Get("hjet_twr_nconstit_quark");
  TH1D* hjet_twr_nconstit_gluon = (TH1D*)fin->Get("hjet_twr_nconstit_gluon");
  TH1D* hjet_twr_nconstit_else = (TH1D*)fin->Get("hjet_twr_nconstit_else");
  TH1D* hjet_p_pTD = (TH1D*)fin->Get("hjet_p_pTD");
  TH1D* hjet_p_pTD_quark = (TH1D*)fin->Get("hjet_p_pTD_quark");
  TH1D* hjet_p_pTD_gluon = (TH1D*)fin->Get("hjet_p_pTD_gluon");
  TH1D* hjet_p_pTD_else = (TH1D*)fin->Get("hjet_p_pTD_else");
  TH1D* hjet_trk_pTD = (TH1D*)fin->Get("hjet_trk_pTD");
  TH1D* hjet_trk_pTD_quark = (TH1D*)fin->Get("hjet_trk_pTD_quark");
  TH1D* hjet_trk_pTD_gluon = (TH1D*)fin->Get("hjet_trk_pTD_gluon");
  TH1D* hjet_trk_pTD_else = (TH1D*)fin->Get("hjet_trk_pTD_else");
  TH1D* hjet_twr_pTD = (TH1D*)fin->Get("hjet_twr_pTD");
  TH1D* hjet_twr_pTD_quark = (TH1D*)fin->Get("hjet_twr_pTD_quark");
  TH1D* hjet_twr_pTD_gluon = (TH1D*)fin->Get("hjet_twr_pTD_gluon");
  TH1D* hjet_twr_pTD_else = (TH1D*)fin->Get("hjet_twr_pTD_else");
*/

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
    f_calfrac->SetLineWidth(2); 
    f_calfrac->SetRange(cal_min,cal_max);
  }
  h2D_JES_calfrac->Draw("colz");
  prof_calfrac->Draw("ep same");
  latex->SetTextColor(kRed);
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.33,0.33,Form("#chi^{2}/ndf: %.2f/%d",f_calfrac->GetChisquare(),f_calfrac->GetNDF()));
  latex->DrawLatex(0.33,0.27,Form("p0: %.3f #pm %.3f",f_calfrac->GetParameter(0),f_calfrac->GetParError(0)));
  latex->DrawLatex(0.33,0.21,Form("p1: %.3f #pm %.3f",f_calfrac->GetParameter(1),f_calfrac->GetParError(1)));
  latex->DrawLatex(0.65,0.33,Form("p2: %.3f #pm %.3f",f_calfrac->GetParameter(2),f_calfrac->GetParError(2)));
  latex->DrawLatex(0.65,0.27,Form("p3: %.3f #pm %.3f",f_calfrac->GetParameter(3),f_calfrac->GetParError(3)));
  latex->DrawLatex(0.65,0.21,Form("p4: %.3f #pm %.3f",f_calfrac->GetParameter(4),f_calfrac->GetParError(4)));
  c_calfrac->SaveAs(Form("./outEMfracFromPion/%s_JES_calfrac.pdf",ihcalType.Data()));
  
  ////////////////////////////////////////////////////////

  TCanvas* c_emfrac = new TCanvas("c_emfrac","c_emfrac",700,500);
  gPad->SetRightMargin(3.0);
  h2D_JES_emfrac->GetXaxis()->CenterTitle(1);
  h2D_JES_emfrac->GetYaxis()->CenterTitle(1);
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
  h2D_JES_emfrac->Draw("colz");
  prof_emfrac->Draw("ep same");
  c_emfrac->SaveAs(Form("./outEMfracFromPion/%s_JES_emfrac.pdf",ihcalType.Data()));

 /* 
  ////////////////////////////////////////////////////////
  //// 1) multiplicity
  ////////////////////////////////////////////////////////
  double max = 0;

  SetLineHistStyle(hjet_p_nconstit,0);
  SetLineHistStyle(hjet_p_nconstit_quark,1);
  SetLineHistStyle(hjet_p_nconstit_gluon,2);
  SetLineHistStyle(hjet_p_nconstit_else,3);
  
  leg->AddEntry(hjet_p_nconstit,"Total","l");
  leg->AddEntry(hjet_p_nconstit_quark,"Light quark","l");
  leg->AddEntry(hjet_p_nconstit_gluon,"Gluon","l");
  leg->AddEntry(hjet_p_nconstit_else,"Else","l");
  leg2->AddEntry(hjet_p_nconstit_quark,"Light quark","l");
  leg2->AddEntry(hjet_p_nconstit_gluon,"Gluon","l");
  leg3->AddEntry(hjet_p_nconstit_quark,"Quark jet efficiency","l");
  leg3->AddEntry(hjet_p_nconstit_gluon,"Gluon jet rejection","l");
  
  TCanvas* c_p_nconstit = new TCanvas("c_p_nconstit","c_p_nconstit",500,500);
  hjet_p_nconstit->GetXaxis()->SetTitle("# of primary particles in a jet");
  max = hjet_p_nconstit->GetMaximum();
  hjet_p_nconstit->SetMaximum(max*1.4);
  hjet_p_nconstit->Draw("hist");
  hjet_p_nconstit_quark->Draw("hist same");
  hjet_p_nconstit_gluon->Draw("hist same");
  hjet_p_nconstit_else->Draw("hist same");
  leg->Draw();
  latex->DrawLatex(0.91,0.87,ihcalType.Data());
  latex->DrawLatex(0.91,0.80,"Primary jet");
  c_p_nconstit->SaveAs(Form("outMultivar/%s_p_nconstit.pdf",ihcalType.Data()));
  
  TCanvas* c_p_nconstit_norm = new TCanvas("c_p_nconstit_norm","c_p_nconstit_norm",500,500);
  TH1D* norm_hjet_p_nconstit = (TH1D*) hjet_p_nconstit->Clone();
  TH1D* norm_hjet_p_nconstit_quark = (TH1D*) hjet_p_nconstit_quark->Clone();
  TH1D* norm_hjet_p_nconstit_gluon = (TH1D*) hjet_p_nconstit_gluon->Clone();
  TH1D* norm_hjet_p_nconstit_else = (TH1D*) hjet_p_nconstit_else->Clone();
  norm_hjet_p_nconstit->Scale(1./norm_hjet_p_nconstit->Integral());
  norm_hjet_p_nconstit_quark->Scale(1./norm_hjet_p_nconstit_quark->Integral());
  norm_hjet_p_nconstit_gluon->Scale(1./norm_hjet_p_nconstit_gluon->Integral());
  norm_hjet_p_nconstit_else->Scale(1./norm_hjet_p_nconstit_else->Integral());
  norm_hjet_p_nconstit_quark->GetXaxis()->SetTitle("# of primary particles in a jet");
  if(norm_hjet_p_nconstit_quark->GetMaximum() > norm_hjet_p_nconstit_gluon->GetMaximum()) {
    max = norm_hjet_p_nconstit_quark->GetMaximum();
  }else { max = norm_hjet_p_nconstit_gluon->GetMaximum();}
  norm_hjet_p_nconstit_quark->SetMaximum(max*1.4);
  norm_hjet_p_nconstit_quark->Draw("hist");
  norm_hjet_p_nconstit_gluon->Draw("hist same");
  leg2->Draw();
  latex->DrawLatex(0.91,0.87,ihcalType.Data());
  latex->DrawLatex(0.91,0.80,"Primary jet");
  c_p_nconstit_norm->SaveAs(Form("outMultivar/%s_p_nconstit_norm.pdf",ihcalType.Data()));
  
  ////////////////////////////////////////////////////////
  SetLineHistStyle(hjet_trk_nconstit,0);
  SetLineHistStyle(hjet_trk_nconstit_quark,1);
  SetLineHistStyle(hjet_trk_nconstit_gluon,2);
  SetLineHistStyle(hjet_trk_nconstit_else,3);
  
  TCanvas* c_trk_nconstit = new TCanvas("c_trk_nconstit","c_trk_nconstit",500,500);
  hjet_trk_nconstit->GetXaxis()->SetTitle("# of tracks in a jet");
  max = hjet_trk_nconstit->GetMaximum();
  hjet_trk_nconstit->SetMaximum(max*1.4);
  hjet_trk_nconstit->Draw("hist");
  hjet_trk_nconstit_quark->Draw("hist same");
  hjet_trk_nconstit_gluon->Draw("hist same");
  hjet_trk_nconstit_else->Draw("hist same");
  leg->Draw();
  latex->DrawLatex(0.91,0.87,ihcalType.Data());
  latex->DrawLatex(0.91,0.80,"Track jet");
  c_trk_nconstit->SaveAs(Form("outMultivar/%s_trk_nconstit.pdf",ihcalType.Data()));
  
  TCanvas* c_trk_nconstit_norm = new TCanvas("c_trk_nconstit_norm","c_trk_nconstit_norm",500,500);
  TH1D* norm_hjet_trk_nconstit = (TH1D*) hjet_trk_nconstit->Clone();
  TH1D* norm_hjet_trk_nconstit_quark = (TH1D*) hjet_trk_nconstit_quark->Clone();
  TH1D* norm_hjet_trk_nconstit_gluon = (TH1D*) hjet_trk_nconstit_gluon->Clone();
  TH1D* norm_hjet_trk_nconstit_else = (TH1D*) hjet_trk_nconstit_else->Clone();
  norm_hjet_trk_nconstit->Scale(1./norm_hjet_trk_nconstit->Integral());
  norm_hjet_trk_nconstit_quark->Scale(1./norm_hjet_trk_nconstit_quark->Integral());
  norm_hjet_trk_nconstit_gluon->Scale(1./norm_hjet_trk_nconstit_gluon->Integral());
  norm_hjet_trk_nconstit_else->Scale(1./norm_hjet_trk_nconstit_else->Integral());
  norm_hjet_trk_nconstit_quark->GetXaxis()->SetTitle("# of tracks in a jet");
  if(norm_hjet_trk_nconstit_quark->GetMaximum() > norm_hjet_trk_nconstit_gluon->GetMaximum()) {
    max = norm_hjet_trk_nconstit_quark->GetMaximum();
  }else { max = norm_hjet_trk_nconstit_gluon->GetMaximum();}
  norm_hjet_trk_nconstit_quark->SetMaximum(max*1.4);
  norm_hjet_trk_nconstit_quark->Draw("hist");
  norm_hjet_trk_nconstit_gluon->Draw("hist same");
  leg2->Draw();
  latex->DrawLatex(0.91,0.87,ihcalType.Data());
  latex->DrawLatex(0.91,0.80,"Track jet");
  c_trk_nconstit_norm->SaveAs(Form("outMultivar/%s_trk_nconstit_norm.pdf",ihcalType.Data()));
  
  ////////////////////////////////////////////////////////
  SetLineHistStyle(hjet_twr_nconstit,0);
  SetLineHistStyle(hjet_twr_nconstit_quark,1);
  SetLineHistStyle(hjet_twr_nconstit_gluon,2);
  SetLineHistStyle(hjet_twr_nconstit_else,3);
  
  TCanvas* c_twr_nconstit = new TCanvas("c_twr_nconstit","c_twr_nconstit",500,500);
  hjet_twr_nconstit->GetXaxis()->SetTitle("# of towers in a jet");
  max = hjet_twr_nconstit->GetMaximum();
  hjet_twr_nconstit->SetMaximum(max*1.4);
  hjet_twr_nconstit->Draw("hist");
  hjet_twr_nconstit_quark->Draw("hist same");
  hjet_twr_nconstit_gluon->Draw("hist same");
  hjet_twr_nconstit_else->Draw("hist same");
  leg->Draw();
  latex->DrawLatex(0.91,0.87,ihcalType.Data());
  latex->DrawLatex(0.91,0.80,"Tower jet");
  c_twr_nconstit->SaveAs(Form("outMultivar/%s_twr_nconstit.pdf",ihcalType.Data()));
  
  TCanvas* c_twr_nconstit_norm = new TCanvas("c_twr_nconstit_norm","c_twr_nconstit_norm",500,500);
  TH1D* norm_hjet_twr_nconstit = (TH1D*) hjet_twr_nconstit->Clone();
  TH1D* norm_hjet_twr_nconstit_quark = (TH1D*) hjet_twr_nconstit_quark->Clone();
  TH1D* norm_hjet_twr_nconstit_gluon = (TH1D*) hjet_twr_nconstit_gluon->Clone();
  TH1D* norm_hjet_twr_nconstit_else = (TH1D*) hjet_twr_nconstit_else->Clone();
  norm_hjet_twr_nconstit->Scale(1./norm_hjet_twr_nconstit->Integral());
  norm_hjet_twr_nconstit_quark->Scale(1./norm_hjet_twr_nconstit_quark->Integral());
  norm_hjet_twr_nconstit_gluon->Scale(1./norm_hjet_twr_nconstit_gluon->Integral());
  norm_hjet_twr_nconstit_else->Scale(1./norm_hjet_twr_nconstit_else->Integral());
  norm_hjet_twr_nconstit_quark->GetXaxis()->SetTitle("# of towers in a jet");
  if(norm_hjet_twr_nconstit_quark->GetMaximum() > norm_hjet_twr_nconstit_gluon->GetMaximum()) {
    max = norm_hjet_twr_nconstit_quark->GetMaximum();
  }else { max = norm_hjet_twr_nconstit_gluon->GetMaximum();}
  norm_hjet_twr_nconstit_quark->SetMaximum(max*1.4);
  norm_hjet_twr_nconstit_quark->Draw("hist");
  norm_hjet_twr_nconstit_gluon->Draw("hist same");
  leg2->Draw();
  latex->DrawLatex(0.91,0.87,ihcalType.Data());
  latex->DrawLatex(0.91,0.80,"Tower jet");
  c_twr_nconstit_norm->SaveAs(Form("outMultivar/%s_twr_nconstit_norm.pdf",ihcalType.Data()));
  
  ////////////////////////////////////////////////////////
  //// 2) pTD
  ////////////////////////////////////////////////////////
  SetLineHistStyle(hjet_p_pTD,0);
  SetLineHistStyle(hjet_p_pTD_quark,1);
  SetLineHistStyle(hjet_p_pTD_gluon,2);
  SetLineHistStyle(hjet_p_pTD_else,3);
  
  TCanvas* c_p_pTD = new TCanvas("c_p_pTD","c_p_pTD",500,500);
  hjet_p_pTD->GetXaxis()->SetTitle("p_{T}D");
  max = hjet_p_pTD->GetMaximum();
  hjet_p_pTD->SetMaximum(max*1.4);
  hjet_p_pTD->Draw("hist");
  hjet_p_pTD_quark->Draw("hist same");
  hjet_p_pTD_gluon->Draw("hist same");
  hjet_p_pTD_else->Draw("hist same");
  leg->Draw();
  latex->DrawLatex(0.91,0.87,ihcalType.Data());
  latex->DrawLatex(0.91,0.80,"Primary jet");
  c_p_pTD->SaveAs(Form("outMultivar/%s_p_pTD.pdf",ihcalType.Data()));
  
  TCanvas* c_p_pTD_norm = new TCanvas("c_p_pTD_norm","c_p_pTD_norm",500,500);
  TH1D* norm_hjet_p_pTD = (TH1D*) hjet_p_pTD->Clone();
  TH1D* norm_hjet_p_pTD_quark = (TH1D*) hjet_p_pTD_quark->Clone();
  TH1D* norm_hjet_p_pTD_gluon = (TH1D*) hjet_p_pTD_gluon->Clone();
  TH1D* norm_hjet_p_pTD_else = (TH1D*) hjet_p_pTD_else->Clone();
  norm_hjet_p_pTD->Scale(1./norm_hjet_p_pTD->Integral());
  norm_hjet_p_pTD_quark->Scale(1./norm_hjet_p_pTD_quark->Integral());
  norm_hjet_p_pTD_gluon->Scale(1./norm_hjet_p_pTD_gluon->Integral());
  norm_hjet_p_pTD_else->Scale(1./norm_hjet_p_pTD_else->Integral());
  norm_hjet_p_pTD_quark->GetXaxis()->SetTitle("p_{T}D");
  if(norm_hjet_p_pTD_quark->GetMaximum() > norm_hjet_p_pTD_gluon->GetMaximum()) {
    max = norm_hjet_p_pTD_quark->GetMaximum();
  }else { max = norm_hjet_p_pTD_gluon->GetMaximum();}
  norm_hjet_p_pTD_quark->SetMaximum(max*1.4);
  norm_hjet_p_pTD_quark->Draw("hist");
  norm_hjet_p_pTD_gluon->Draw("hist same");
  leg2->Draw();
  latex->DrawLatex(0.91,0.87,ihcalType.Data());
  latex->DrawLatex(0.91,0.80,"Primary jet");
  c_p_pTD_norm->SaveAs(Form("outMultivar/%s_p_pTD_norm.pdf",ihcalType.Data()));
  
  ////////////////////////////////////////////////////////
  SetLineHistStyle(hjet_trk_pTD,0);
  SetLineHistStyle(hjet_trk_pTD_quark,1);
  SetLineHistStyle(hjet_trk_pTD_gluon,2);
  SetLineHistStyle(hjet_trk_pTD_else,3);
  
  TCanvas* c_trk_pTD = new TCanvas("c_trk_pTD","c_trk_pTD",500,500);
  hjet_trk_pTD->GetXaxis()->SetTitle("p_{T}D");
  max = hjet_trk_pTD->GetMaximum();
  hjet_trk_pTD->SetMaximum(max*1.4);
  hjet_trk_pTD->Draw("hist");
  hjet_trk_pTD_quark->Draw("hist same");
  hjet_trk_pTD_gluon->Draw("hist same");
  hjet_trk_pTD_else->Draw("hist same");
  leg->Draw();
  latex->DrawLatex(0.91,0.87,ihcalType.Data());
  latex->DrawLatex(0.91,0.80,"Track jet");
  c_trk_pTD->SaveAs(Form("outMultivar/%s_trk_pTD.pdf",ihcalType.Data()));
  
  TCanvas* c_trk_pTD_norm = new TCanvas("c_trk_pTD_norm","c_trk_pTD_norm",500,500);
  TH1D* norm_hjet_trk_pTD = (TH1D*) hjet_trk_pTD->Clone();
  TH1D* norm_hjet_trk_pTD_quark = (TH1D*) hjet_trk_pTD_quark->Clone();
  TH1D* norm_hjet_trk_pTD_gluon = (TH1D*) hjet_trk_pTD_gluon->Clone();
  TH1D* norm_hjet_trk_pTD_else = (TH1D*) hjet_trk_pTD_else->Clone();
  norm_hjet_trk_pTD->Scale(1./norm_hjet_trk_pTD->Integral());
  norm_hjet_trk_pTD_quark->Scale(1./norm_hjet_trk_pTD_quark->Integral());
  norm_hjet_trk_pTD_gluon->Scale(1./norm_hjet_trk_pTD_gluon->Integral());
  norm_hjet_trk_pTD_else->Scale(1./norm_hjet_trk_pTD_else->Integral());
  norm_hjet_trk_pTD_quark->GetXaxis()->SetTitle("p_{T}D");
  if(norm_hjet_trk_pTD_quark->GetMaximum() > norm_hjet_trk_pTD_gluon->GetMaximum()) {
    max = norm_hjet_trk_pTD_quark->GetMaximum();
  }else { max = norm_hjet_trk_pTD_gluon->GetMaximum();}
  norm_hjet_trk_pTD_quark->SetMaximum(max*1.4);
  norm_hjet_trk_pTD_quark->Draw("hist");
  norm_hjet_trk_pTD_gluon->Draw("hist same");
  leg2->Draw();
  latex->DrawLatex(0.91,0.87,ihcalType.Data());
  latex->DrawLatex(0.91,0.80,"Track jet");
  c_trk_pTD_norm->SaveAs(Form("outMultivar/%s_trk_pTD_norm.pdf",ihcalType.Data()));
  
  ////////////////////////////////////////////////////////
  SetLineHistStyle(hjet_twr_pTD,0);
  SetLineHistStyle(hjet_twr_pTD_quark,1);
  SetLineHistStyle(hjet_twr_pTD_gluon,2);
  SetLineHistStyle(hjet_twr_pTD_else,3);
  
  TCanvas* c_twr_pTD = new TCanvas("c_twr_pTD","c_twr_pTD",500,500);
  hjet_twr_pTD->GetXaxis()->SetTitle("p_{T}D");
  max = hjet_twr_pTD->GetMaximum();
  hjet_twr_pTD->SetMaximum(max*1.4);
  hjet_twr_pTD->Draw("hist");
  hjet_twr_pTD_quark->Draw("hist same");
  hjet_twr_pTD_gluon->Draw("hist same");
  hjet_twr_pTD_else->Draw("hist same");
  leg->Draw();
  latex->DrawLatex(0.91,0.87,ihcalType.Data());
  latex->DrawLatex(0.91,0.80,"Tower jet");
  c_twr_pTD->SaveAs(Form("outMultivar/%s_twr_pTD.pdf",ihcalType.Data()));
  
  TCanvas* c_twr_pTD_norm = new TCanvas("c_twr_pTD_norm","c_twr_pTD_norm",500,500);
  TH1D* norm_hjet_twr_pTD = (TH1D*) hjet_twr_pTD->Clone();
  TH1D* norm_hjet_twr_pTD_quark = (TH1D*) hjet_twr_pTD_quark->Clone();
  TH1D* norm_hjet_twr_pTD_gluon = (TH1D*) hjet_twr_pTD_gluon->Clone();
  TH1D* norm_hjet_twr_pTD_else = (TH1D*) hjet_twr_pTD_else->Clone();
  norm_hjet_twr_pTD->Scale(1./norm_hjet_twr_pTD->Integral());
  norm_hjet_twr_pTD_quark->Scale(1./norm_hjet_twr_pTD_quark->Integral());
  norm_hjet_twr_pTD_gluon->Scale(1./norm_hjet_twr_pTD_gluon->Integral());
  norm_hjet_twr_pTD_else->Scale(1./norm_hjet_twr_pTD_else->Integral());
  norm_hjet_twr_pTD_quark->GetXaxis()->SetTitle("p_{T}D");
  if(norm_hjet_twr_pTD_quark->GetMaximum() > norm_hjet_twr_pTD_gluon->GetMaximum()) {
    max = norm_hjet_twr_pTD_quark->GetMaximum();
  }else { max = norm_hjet_twr_pTD_gluon->GetMaximum();}
  norm_hjet_twr_pTD_quark->SetMaximum(max*1.4);
  norm_hjet_twr_pTD_quark->Draw("hist");
  norm_hjet_twr_pTD_gluon->Draw("hist same");
  leg2->Draw();
  latex->DrawLatex(0.91,0.87,ihcalType.Data());
  latex->DrawLatex(0.91,0.80,"Tower jet");
  c_twr_pTD_norm->SaveAs(Form("outMultivar/%s_twr_pTD_norm.pdf",ihcalType.Data()));
  
  ////////////////////////////////////////////////////////
  //// 3) check the efficiency/rejection
  ////////////////////////////////////////////////////////
  double quark_tot =0;
  double gluon_tot =0;
  double quark_sum =0;
  double gluon_sum =0;
  double quark_purity = 0;
  //int xbin=0; int ybin=0;
  double lowEdge = 0;
  double binSize = 0;
  double highEdge = 0;
   
  ////////////////////////////////////////////////////////
  //// 3-1) multiplicity
  ////////////////////////////////////////////////////////
  std::vector<double> p_nconstit_px = 0; 
  std::vector<double> p_nconstit_py = 0; 
  
  TCanvas* c_p_nconstit_frac = new TCanvas("c_p_nconstit_frac","c_p_nconstit_frac",500,500);
  TH1D* h_p_nconstit_frac_quark = (TH1D*) hjet_p_nconstit_quark->Clone();
  TH1D* h_p_nconstit_frac_gluon = (TH1D*) hjet_p_nconstit_gluon->Clone();
  TH1D* h_p_nconstit_purity_quark = (TH1D*) hjet_p_nconstit_quark->Clone();
  lowEdge = h_p_nconstit_frac_quark->GetBinLowEdge(1);
  binSize = h_p_nconstit_frac_quark->GetSize()-2;
  highEdge = h_p_nconstit_frac_quark->GetBinLowEdge(binSize+1);
  quark_tot = h_p_nconstit_frac_quark->Integral();
  gluon_tot = h_p_nconstit_frac_gluon->Integral();
  quark_sum = 0;
  gluon_sum = 0;
  //cout << "*** p_nconstit:" << binSize << endl;
  for (int ib =0; ib < binSize; ib++){
    quark_sum += h_p_nconstit_frac_quark->GetBinContent(ib+1)/quark_tot;
    gluon_sum += h_p_nconstit_frac_gluon->GetBinContent(ib+1)/gluon_tot;
    p_nconstit_px.push_back(quark_sum); // efficiency
    p_nconstit_py.push_back(1-gluon_sum); // rejection
    if (quark_sum+gluon_sum==0) { quark_purity=0;}
    else { quark_purity = quark_sum/(quark_sum+gluon_sum);}
    //cout << ib <<"th p_nconstit_px: " << p_nconstit_px.at(ib) << ", p_nconstit_py: " << p_nconstit_py.at(ib) << endl;
    h_p_nconstit_frac_quark->SetBinContent(ib+1,quark_sum); // efficiency
    h_p_nconstit_frac_gluon->SetBinContent(ib+1,1-gluon_sum); // rejection
    h_p_nconstit_purity_quark->SetBinContent(ib+1,quark_purity); 
  } 
  //cout << "Check size:" << p_nconstit_px.size() << p_nconstit_py.size() << endl;
  
  h_p_nconstit_frac_quark->GetXaxis()->SetTitle("# of primary particles in a jet");
  h_p_nconstit_frac_quark->SetMaximum(1.5);
  h_p_nconstit_frac_quark->SetFillStyle(0);
//  h_p_nconstit_frac_quark->SetLineStyle(7);
  h_p_nconstit_frac_gluon->SetFillStyle(0);
//  h_p_nconstit_frac_gluon->SetLineStyle(7);
  h_p_nconstit_purity_quark->SetLineColor(kGray+3);
  h_p_nconstit_purity_quark->SetFillStyle(0);
  h_p_nconstit_frac_quark->Draw("hist");
  h_p_nconstit_frac_gluon->Draw("hist same");
  //h_p_nconstit_purity_quark->Draw("hist same");
  leg3->Draw();
  latex->DrawLatex(0.91,0.87,ihcalType.Data());
  latex->DrawLatex(0.91,0.80,"Primary jet");
  dashedLine(lowEdge,1,highEdge,1);
  c_p_nconstit_frac->SaveAs(Form("outMultivar/%s_p_nconstit_frac.pdf",ihcalType.Data()));
 
  ////////////////////////////////////////////////////////
  std::vector<double> trk_nconstit_px = 0; 
  std::vector<double> trk_nconstit_py = 0; 
  
  TCanvas* c_trk_nconstit_frac = new TCanvas("c_trk_nconstit_frac","c_trk_nconstit_frac",500,500);
  TH1D* h_trk_nconstit_frac_quark = (TH1D*) hjet_trk_nconstit_quark->Clone();
  TH1D* h_trk_nconstit_frac_gluon = (TH1D*) hjet_trk_nconstit_gluon->Clone();
  TH1D* h_trk_nconstit_purity_quark = (TH1D*) hjet_trk_nconstit_quark->Clone();
  lowEdge = h_trk_nconstit_frac_quark->GetBinLowEdge(1);
  binSize = h_trk_nconstit_frac_quark->GetSize()-2;
  highEdge = h_trk_nconstit_frac_quark->GetBinLowEdge(binSize+1);
  quark_tot = h_trk_nconstit_frac_quark->Integral();
  gluon_tot = h_trk_nconstit_frac_gluon->Integral();
  quark_sum = 0;
  gluon_sum = 0;
  //cout << "*** trk_nconstit:" << binSize << endl;
  for (int ib =0; ib < binSize; ib++){
    quark_sum += h_trk_nconstit_frac_quark->GetBinContent(ib+1)/quark_tot;
    gluon_sum += h_trk_nconstit_frac_gluon->GetBinContent(ib+1)/gluon_tot;
    trk_nconstit_px.push_back(quark_sum); // efficiency
    trk_nconstit_py.push_back(1-gluon_sum); // rejection
    if (quark_sum+gluon_sum==0) { quark_purity=0;}
    else { quark_purity = quark_sum/(quark_sum+gluon_sum);}
    h_trk_nconstit_frac_quark->SetBinContent(ib+1,quark_sum); // efficiency
    h_trk_nconstit_frac_gluon->SetBinContent(ib+1,1-gluon_sum); // rejection
    h_trk_nconstit_purity_quark->SetBinContent(ib+1,quark_purity); 
  } 
  
  h_trk_nconstit_frac_quark->GetXaxis()->SetTitle("# of tracks in a jet");
  h_trk_nconstit_frac_quark->SetMaximum(1.5);
  h_trk_nconstit_frac_quark->SetFillStyle(0);
//  h_trk_nconstit_frac_quark->SetLineStyle(7);
  h_trk_nconstit_frac_gluon->SetFillStyle(0);
//  h_trk_nconstit_frac_gluon->SetLineStyle(7);
  h_trk_nconstit_purity_quark->SetLineColor(kGray+3);
  h_trk_nconstit_purity_quark->SetFillStyle(0);
  h_trk_nconstit_frac_quark->Draw("hist");
  h_trk_nconstit_frac_gluon->Draw("hist same");
  leg3->Draw();
  latex->DrawLatex(0.91,0.87,ihcalType.Data());
  latex->DrawLatex(0.91,0.80,"Track jet");
  dashedLine(lowEdge,1,highEdge,1);
  c_trk_nconstit_frac->SaveAs(Form("outMultivar/%s_trk_nconstit_frac.pdf",ihcalType.Data()));
 
  ////////////////////////////////////////////////////////
  std::vector<double> twr_nconstit_px = 0; 
  std::vector<double> twr_nconstit_py = 0; 
  
  TCanvas* c_twr_nconstit_frac = new TCanvas("c_twr_nconstit_frac","c_twr_nconstit_frac",500,500);
  TH1D* h_twr_nconstit_frac_quark = (TH1D*) hjet_twr_nconstit_quark->Clone();
  TH1D* h_twr_nconstit_frac_gluon = (TH1D*) hjet_twr_nconstit_gluon->Clone();
  TH1D* h_twr_nconstit_purity_quark = (TH1D*) hjet_twr_nconstit_quark->Clone();
  lowEdge = h_twr_nconstit_frac_quark->GetBinLowEdge(1);
  binSize = h_twr_nconstit_frac_quark->GetSize()-2;
  highEdge = h_twr_nconstit_frac_quark->GetBinLowEdge(binSize+1);
  quark_tot = h_twr_nconstit_frac_quark->Integral();
  gluon_tot = h_twr_nconstit_frac_gluon->Integral();
  quark_sum = 0;
  gluon_sum = 0;
  //cout << "*** twr_nconstit:" << binSize << endl;
  for (int ib =0; ib < binSize; ib++){
    quark_sum += h_twr_nconstit_frac_quark->GetBinContent(ib+1)/quark_tot;
    gluon_sum += h_twr_nconstit_frac_gluon->GetBinContent(ib+1)/gluon_tot;
    twr_nconstit_px.push_back(quark_sum); // efficiency
    twr_nconstit_py.push_back(1-gluon_sum); // rejection
    if (quark_sum+gluon_sum==0) { quark_purity=0;}
    else { quark_purity = quark_sum/(quark_sum+gluon_sum);}
    h_twr_nconstit_frac_quark->SetBinContent(ib+1,quark_sum); // efficiency
    h_twr_nconstit_frac_gluon->SetBinContent(ib+1,1-gluon_sum); // rejection
    h_twr_nconstit_purity_quark->SetBinContent(ib+1,quark_purity); 
  } 
  
  h_twr_nconstit_frac_quark->GetXaxis()->SetTitle("# of towers in a jet");
  h_twr_nconstit_frac_quark->SetMaximum(1.5);
  h_twr_nconstit_frac_quark->SetFillStyle(0);
//  h_twr_nconstit_frac_quark->SetLineStyle(7);
  h_twr_nconstit_frac_gluon->SetFillStyle(0);
//  h_twr_nconstit_frac_gluon->SetLineStyle(7);
  h_twr_nconstit_purity_quark->SetLineColor(kGray+3);
  h_twr_nconstit_purity_quark->SetFillStyle(0);
  h_twr_nconstit_frac_quark->Draw("hist");
  h_twr_nconstit_frac_gluon->Draw("hist same");
  //h_twr_nconstit_purity_quark->Draw("hist same");
  leg3->Draw();
  latex->DrawLatex(0.91,0.87,ihcalType.Data());
  latex->DrawLatex(0.91,0.80,"Tower jet");
  dashedLine(lowEdge,1,highEdge,1);
  c_twr_nconstit_frac->SaveAs(Form("outMultivar/%s_twr_nconstit_frac.pdf",ihcalType.Data()));
 
  ////////////////////////////////////////////////////////
  //// 3-2) pTD
  ////////////////////////////////////////////////////////
  std::vector<double> p_pTD_px = 0; 
  std::vector<double> p_pTD_py = 0; 
  
  TCanvas* c_p_pTD_frac = new TCanvas("c_p_pTD_frac","c_p_pTD_frac",500,500);
  TH1D* h_p_pTD_frac_quark = (TH1D*) hjet_p_pTD_quark->Clone();
  TH1D* h_p_pTD_frac_gluon = (TH1D*) hjet_p_pTD_gluon->Clone();
  TH1D* h_p_pTD_purity_quark = (TH1D*) hjet_p_pTD_quark->Clone();
  lowEdge = h_p_pTD_frac_quark->GetBinLowEdge(1);
  binSize = h_p_pTD_frac_quark->GetSize()-2;
  highEdge = h_p_pTD_frac_quark->GetBinLowEdge(binSize+1);
  quark_tot = h_p_pTD_frac_quark->Integral();
  gluon_tot = h_p_pTD_frac_gluon->Integral();
  quark_sum = 0;
  gluon_sum = 0;
  //cout << "lowEdge = " << lowEdge << endl;
  //cout << "binSize = " << binSize << endl;
  //cout << "highEdge = " << highEdge << endl;
  //for (int ib =0; ib < binSize; ib++){
  for (int ib = binSize-1; ib>=0; ib--){
    quark_sum += h_p_pTD_frac_quark->GetBinContent(ib+1)/quark_tot;
    gluon_sum += h_p_pTD_frac_gluon->GetBinContent(ib+1)/gluon_tot;
    p_pTD_px.push_back(quark_sum); // efficiency
    p_pTD_py.push_back(1-gluon_sum); // rejection
    if (quark_sum+gluon_sum==0) { quark_purity=0;}
    else { quark_purity = quark_sum/(quark_sum+gluon_sum);}
    h_p_pTD_frac_quark->SetBinContent(ib+1,quark_sum); // efficiency
    h_p_pTD_frac_gluon->SetBinContent(ib+1,1-gluon_sum); // rejection
    h_p_pTD_purity_quark->SetBinContent(ib+1,quark_purity); 
  } 
  //cout << "Check size:" << p_pTD_px.size() << p_pTD_py.size() << endl;
  //for (int ib = binSize-1; ib>=0; ib--){
  //  cout << "trk_pTD_px: " << trk_pTD_px.at(ib) << ", trk_pTD_py: " << trk_pTD_py.at(ib) << endl;
  //} 
  
  h_p_pTD_frac_quark->GetXaxis()->SetTitle("p_{T}D");
  h_p_pTD_frac_quark->SetMaximum(1.5);
  h_p_pTD_frac_quark->SetFillStyle(0);
//  h_p_pTD_frac_quark->SetLineStyle(7);
  h_p_pTD_frac_gluon->SetFillStyle(0);
//  h_p_pTD_frac_gluon->SetLineStyle(7);
  h_p_pTD_purity_quark->SetLineColor(kGray+3);
  h_p_pTD_purity_quark->SetFillStyle(0);
  h_p_pTD_frac_quark->Draw("hist");
  h_p_pTD_frac_gluon->Draw("hist same");
  //h_p_pTD_purity_quark->Draw("hist same");
  leg3->Draw();
  latex->DrawLatex(0.91,0.87,ihcalType.Data());
  latex->DrawLatex(0.91,0.80,"Primary jet");
  dashedLine(lowEdge,1,highEdge,1);
  c_p_pTD_frac->SaveAs(Form("outMultivar/%s_p_pTD_frac.pdf",ihcalType.Data()));
 
  ////////////////////////////////////////////////////////
  std::vector<double> trk_pTD_px = 0; 
  std::vector<double> trk_pTD_py = 0; 
  
  TCanvas* c_trk_pTD_frac = new TCanvas("c_trk_pTD_frac","c_trk_pTD_frac",500,500);
  TH1D* h_trk_pTD_frac_quark = (TH1D*) hjet_trk_pTD_quark->Clone();
  TH1D* h_trk_pTD_frac_gluon = (TH1D*) hjet_trk_pTD_gluon->Clone();
  TH1D* h_trk_pTD_purity_quark = (TH1D*) hjet_trk_pTD_quark->Clone();
  lowEdge = h_trk_pTD_frac_quark->GetBinLowEdge(1);
  binSize = h_trk_pTD_frac_quark->GetSize()-2;
  highEdge = h_trk_pTD_frac_quark->GetBinLowEdge(binSize+1);
  quark_tot = h_trk_pTD_frac_quark->Integral();
  gluon_tot = h_trk_pTD_frac_gluon->Integral();
  quark_sum = 0;
  gluon_sum = 0;
  //for (int ib =0; ib < binSize; ib++){
  for (int ib = binSize-1; ib>=0; ib--){
    quark_sum += h_trk_pTD_frac_quark->GetBinContent(ib+1)/quark_tot;
    gluon_sum += h_trk_pTD_frac_gluon->GetBinContent(ib+1)/gluon_tot;
    trk_pTD_px.push_back(quark_sum); // efficiency
    trk_pTD_py.push_back(1-gluon_sum); // rejection
    if (quark_sum+gluon_sum==0) { quark_purity=0;}
    else { quark_purity = quark_sum/(quark_sum+gluon_sum);}
    h_trk_pTD_frac_quark->SetBinContent(ib+1,quark_sum); // efficiency
    h_trk_pTD_frac_gluon->SetBinContent(ib+1,1-gluon_sum); // rejection
    h_trk_pTD_purity_quark->SetBinContent(ib+1,quark_purity); 
  } 
  
  h_trk_pTD_frac_quark->GetXaxis()->SetTitle("p_{T}D");
  h_trk_pTD_frac_quark->SetMaximum(1.5);
  h_trk_pTD_frac_quark->SetFillStyle(0);
//  h_trk_pTD_frac_quark->SetLineStyle(7);
  h_trk_pTD_frac_gluon->SetFillStyle(0);
//  h_trk_pTD_frac_gluon->SetLineStyle(7);
  h_trk_pTD_purity_quark->SetLineColor(kGray+3);
  h_trk_pTD_purity_quark->SetFillStyle(0);
  h_trk_pTD_frac_quark->Draw("hist");
  h_trk_pTD_frac_gluon->Draw("hist same");
  leg3->Draw();
  latex->DrawLatex(0.91,0.87,ihcalType.Data());
  latex->DrawLatex(0.91,0.80,"Track jet");
  dashedLine(lowEdge,1,highEdge,1);
  c_trk_pTD_frac->SaveAs(Form("outMultivar/%s_trk_pTD_frac.pdf",ihcalType.Data()));
 
  ////////////////////////////////////////////////////////
  std::vector<double> twr_pTD_px = 0; 
  std::vector<double> twr_pTD_py = 0; 
  
  TCanvas* c_twr_pTD_frac = new TCanvas("c_twr_pTD_frac","c_twr_pTD_frac",500,500);
  TH1D* h_twr_pTD_frac_quark = (TH1D*) hjet_twr_pTD_quark->Clone();
  TH1D* h_twr_pTD_frac_gluon = (TH1D*) hjet_twr_pTD_gluon->Clone();
  TH1D* h_twr_pTD_purity_quark = (TH1D*) hjet_twr_pTD_quark->Clone();
  lowEdge = h_twr_pTD_frac_quark->GetBinLowEdge(1);
  binSize = h_twr_pTD_frac_quark->GetSize()-2;
  highEdge = h_twr_pTD_frac_quark->GetBinLowEdge(binSize+1);
  quark_tot = h_twr_pTD_frac_quark->Integral();
  gluon_tot = h_twr_pTD_frac_gluon->Integral();
  quark_sum = 0;
  gluon_sum = 0;
  //for (int ib =0; ib < binSize; ib++){
  for (int ib = binSize-1; ib>=0; ib--){
    quark_sum += h_twr_pTD_frac_quark->GetBinContent(ib+1)/quark_tot;
    gluon_sum += h_twr_pTD_frac_gluon->GetBinContent(ib+1)/gluon_tot;
    twr_pTD_px.push_back(quark_sum); // efficiency
    twr_pTD_py.push_back(1-gluon_sum); // rejection
    if (quark_sum+gluon_sum==0) { quark_purity=0;}
    else { quark_purity = quark_sum/(quark_sum+gluon_sum);}
    h_twr_pTD_frac_quark->SetBinContent(ib+1,quark_sum); // efficiency
    h_twr_pTD_frac_gluon->SetBinContent(ib+1,1-gluon_sum); // rejection
    h_twr_pTD_purity_quark->SetBinContent(ib+1,quark_purity); 
  } 
  
  h_twr_pTD_frac_quark->GetXaxis()->SetTitle("p_{T}D");
  h_twr_pTD_frac_quark->SetMaximum(1.5);
  h_twr_pTD_frac_quark->SetFillStyle(0);
//  h_twr_pTD_frac_quark->SetLineStyle(7);
  h_twr_pTD_frac_gluon->SetFillStyle(0);
//  h_twr_pTD_frac_gluon->SetLineStyle(7);
  h_twr_pTD_purity_quark->SetLineColor(kGray+3);
  h_twr_pTD_purity_quark->SetFillStyle(0);
  h_twr_pTD_frac_quark->Draw("hist");
  h_twr_pTD_frac_gluon->Draw("hist same");
  //h_twr_pTD_purity_quark->Draw("hist same");
  leg3->Draw();
  latex->DrawLatex(0.91,0.87,ihcalType.Data());
  latex->DrawLatex(0.91,0.80,"Tower jet");
  dashedLine(lowEdge,1,highEdge,1);
  c_twr_pTD_frac->SaveAs(Form("outMultivar/%s_twr_pTD_frac.pdf",ihcalType.Data()));
 
  
  ////////////////////////////////////////////////////////
  //// 4) ROC plots
  ////////////////////////////////////////////////////////
  //// 4-1) multiplicity
  TCanvas* c_p_nconstit_roc = new TCanvas("c_p_nconstit_roc","c_p_nconstit_roc",500,500);
  TGraph* g_p_nconstit_roc = new TGraph((int)p_nconstit_px.size(),&p_nconstit_px[0],&p_nconstit_py[0]);
  TGraph* g_trk_nconstit_roc = new TGraph((int)trk_nconstit_px.size(),&trk_nconstit_px[0],&trk_nconstit_py[0]);
  TGraph* g_twr_nconstit_roc = new TGraph((int)twr_nconstit_px.size(),&twr_nconstit_px[0],&twr_nconstit_py[0]);
  g_p_nconstit_roc->SetName("g_p_nconstit_roc");
  g_trk_nconstit_roc->SetName("g_trk_nconstit_roc");
  g_twr_nconstit_roc->SetName("g_twr_nconstit_roc");
  SetGraphStyle(g_p_nconstit_roc,1,10);
  SetGraphStyle(g_trk_nconstit_roc,2,10);
  SetGraphStyle(g_twr_nconstit_roc,3,10);
  
  g_p_nconstit_roc->GetXaxis()->SetTitle("Quark jet efficiency");
  g_p_nconstit_roc->GetYaxis()->SetTitle("Gluon jet rejection");
  g_p_nconstit_roc->GetXaxis()->SetLimits(0,1);
  g_p_nconstit_roc->SetMinimum(0);
  g_p_nconstit_roc->SetMaximum(1);
  g_p_nconstit_roc->Draw("alp");
  g_trk_nconstit_roc->Draw("lp");
  g_twr_nconstit_roc->Draw("lp");
  //leg4->SetHeader(Form("Multiplicity (%s)",ihcalType.Data()));
  leg4->SetHeader("Multiplicity");
  leg4->AddEntry(g_p_nconstit_roc,"Primary jet","lp");
  leg4->AddEntry(g_trk_nconstit_roc,"Track jet","lp");
  leg4->AddEntry(g_twr_nconstit_roc,"Tower jet","lp");
  leg4->Draw();
  dashedLine(0,1,1,0);
  c_p_nconstit_roc->SaveAs(Form("outMultivar/%s_p_nconstit_roc.pdf",ihcalType.Data()));
  
  //// 4-2) pTD
  TCanvas* c_p_pTD_roc = new TCanvas("c_p_pTD_roc","c_p_pTD_roc",500,500);
  TGraph* g_p_pTD_roc = new TGraph((int)p_pTD_px.size(),&p_pTD_px[0],&p_pTD_py[0]);
  TGraph* g_trk_pTD_roc = new TGraph((int)trk_pTD_px.size(),&trk_pTD_px[0],&trk_pTD_py[0]);
  TGraph* g_twr_pTD_roc = new TGraph((int)twr_pTD_px.size(),&twr_pTD_px[0],&twr_pTD_py[0]);
  g_p_pTD_roc->SetName("g_p_pTD_roc");
  g_trk_pTD_roc->SetName("g_trk_pTD_roc");
  g_twr_pTD_roc->SetName("g_twr_pTD_roc");
  SetGraphStyle(g_p_pTD_roc,1,10);
  SetGraphStyle(g_trk_pTD_roc,2,10);
  SetGraphStyle(g_twr_pTD_roc,3,10);
  
  g_p_pTD_roc->GetXaxis()->SetTitle("Quark jet efficiency");
  g_p_pTD_roc->GetYaxis()->SetTitle("Gluon jet rejection");
  g_p_pTD_roc->GetXaxis()->SetLimits(0,1);
  g_p_pTD_roc->SetMinimum(0);
  g_p_pTD_roc->SetMaximum(1);
  g_p_pTD_roc->Draw("alp");
  g_trk_pTD_roc->Draw("lp");
  g_twr_pTD_roc->Draw("lp");
  leg4->SetHeader("p_{T}D");
//  leg4->AddEntry(g_p_pTD_roc,"Primary jet","lp");
//  leg4->AddEntry(g_trk_pTD_roc,"Track jet","lp");
//  leg4->AddEntry(g_twr_pTD_roc,"Tower jet","lp");
  leg4->Draw();
  dashedLine(0,1,1,0);
  c_p_pTD_roc->SaveAs(Form("outMultivar/%s_p_pTD_roc.pdf",ihcalType.Data()));
  
  ////////////////////////////////////////////////////////
  TFile* fout = new TFile(Form("outMultivar/%s_ROC.root",ihcalType.Data()),"RECREATE");
  fout->cd();
  g_p_nconstit_roc->Write();
  g_trk_nconstit_roc->Write();
  g_twr_nconstit_roc->Write();
  g_p_pTD_roc->Write();
  g_trk_pTD_roc->Write();
  g_twr_pTD_roc->Write();
  fout->Close();
   
  
  //TH2D* h2d_p_nconstit_roc = new TH2D("h2d_p_nconstit_roc",";light-quark jet efficiency;gluon jet rejection",41,0,1,41,0,1);
  //for (int ib=0; ib<h_p_nconstit_frac_quark->GetSize()-2; ib++){
  //  xbin = h2d_p_nconstit_roc->GetXaxis()->FindBin(h_p_nconstit_frac_quark->GetBinContent(ib+1));
  //  ybin = h2d_p_nconstit_roc->GetYaxis()->FindBin(h_p_nconstit_frac_gluon->GetBinContent(ib+1));
  //  h2d_p_nconstit_roc->SetBinContent(xbin,ybin,1);
  //}
*/
  return; 
}
