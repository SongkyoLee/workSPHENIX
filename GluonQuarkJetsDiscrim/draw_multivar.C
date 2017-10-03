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
#include "TError.h"

#include "TPaletteAxis.h"
#include "TLegend.h"
#include "TLatex.h"

////////////// ihcalType
//
// SS310_OUTPUT
// AL_OUTPUT
// AL_NOIHCAL_OUTPUT
// SS310_FRAME_OUTPUT
//
// SS310_RSCALE_OUTPUT
// AL_RSCALE_OUTPUT
// AL_NOIHCAL_RSCALE_OUTPUT
// SS310_FRAME_RSCALE3_OUTPUT
// SS310_NOIHCAL_RSCALE2_OUTPUT 
//
//////////////

void draw_multivar(TString ihcalType = "SS310_RSCALE_OUTPUT")
{

  gStyle->SetPalette(51);
  //gStyle->SetPalette(55);
  //gStyle->SetPalette(1);
  
  TString fname = Form("./outMultivar/%s_multivar.root",ihcalType.Data());
  TFile* fin = new TFile(fname);
 
  TH2D* h2d_prim_trk = (TH2D*)fin->Get("h2d_prim_trk");
  TH2D* h2d_prim_twr = (TH2D*)fin->Get("h2d_prim_twr");
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

  ////////////////////////////////////////////////////////
  TLegend* leg = new TLegend(0.71, 0.54, 0.89, 0.74);
  TLegend* leg2 = new TLegend(0.71, 0.64, 0.89, 0.74);
  TLegend* leg3 = new TLegend(0.21, 0.74, 0.49, 0.84);
  TLegend* leg4 = new TLegend(0.21, 0.24, 0.49, 0.44);
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(32);
  latex->SetTextSize(0.045);
  
  ////////////////////////////////////////////////////////
  TPaletteAxis* pal = new TPaletteAxis();
        
  TCanvas* c_cor_prim_trk = new TCanvas("c_cor_prim_trk","c_cor_prim_trk",500,500);
  gPad->SetRightMargin(3.0);
  h2d_prim_trk->GetXaxis()->CenterTitle(1);
  h2d_prim_trk->GetYaxis()->CenterTitle(1);
  h2d_prim_trk->Draw("colz");
  c_cor_prim_trk->Update(); // mandatory to find palette!!
  pal = (TPaletteAxis*)h2d_prim_trk->GetListOfFunctions()->FindObject("palette");
  pal->SetX2NDC(0.92);
  h2d_prim_trk->Draw("colz");
  c_cor_prim_trk->SaveAs(Form("outMultivar/%s_cor_prim_trk.pdf",ihcalType.Data()));
  
  TCanvas* c_cor_prim_twr = new TCanvas("c_cor_prim_twr","c_cor_prim_twr",500,500);
  gPad->SetRightMargin(3.0);
  h2d_prim_twr->GetXaxis()->CenterTitle(1);
  h2d_prim_twr->GetYaxis()->CenterTitle(1);
  h2d_prim_twr->Draw("colz");
  c_cor_prim_twr->Update(); // mandatory to find palette!!
  pal = (TPaletteAxis*)h2d_prim_twr->GetListOfFunctions()->FindObject("palette");
  pal->SetX2NDC(0.92);
//  h2d_prim_twr->Draw("colz");
  c_cor_prim_twr->Modified();
  c_cor_prim_twr->Update();
  c_cor_prim_twr->SaveAs(Form("outMultivar/%s_cor_prim_twr.pdf",ihcalType.Data()));
  
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
  leg3->AddEntry(hjet_p_nconstit_quark,"Light quark","l");
  leg3->AddEntry(hjet_p_nconstit_gluon,"Gluon","l");
  
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
  for (int ib =0; ib < h_p_nconstit_frac_quark->GetSize()-2; ib++){
    quark_sum += h_p_nconstit_frac_quark->GetBinContent(ib+1)/quark_tot;
    gluon_sum += h_p_nconstit_frac_gluon->GetBinContent(ib+1)/gluon_tot;
    p_nconstit_px.push_back(quark_sum); // efficiency
    p_nconstit_py.push_back(1-gluon_sum); // rejection
    if (quark_sum+gluon_sum==0) { quark_purity=0;}
    else { quark_purity = quark_sum/(quark_sum+gluon_sum);}
    //cout << "p_nconstit_px: " << p_nconstit_px.at(ib) << ", p_nconstit_py: " << p_nconstit_py.at(ib) << endl;
    h_p_nconstit_frac_quark->SetBinContent(ib+1,quark_sum); // efficiency
    h_p_nconstit_frac_gluon->SetBinContent(ib+1,1-gluon_sum); // rejection
    h_p_nconstit_purity_quark->SetBinContent(ib+1,quark_purity); 
  } 
  //cout << "Check size:" << p_nconstit_px.size() << p_nconstit_py.size() << endl;
  
  h_p_nconstit_frac_quark->GetXaxis()->SetTitle("# of primary particles in a jet");
  h_p_nconstit_frac_quark->SetMaximum(1.4);
  h_p_nconstit_frac_quark->SetFillStyle(0);
  h_p_nconstit_frac_quark->SetLineStyle(7);
  h_p_nconstit_frac_gluon->SetFillStyle(0);
  h_p_nconstit_frac_gluon->SetLineStyle(7);
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
  for (int ib =0; ib < h_trk_nconstit_frac_quark->GetSize()-2; ib++){
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
  h_trk_nconstit_frac_quark->SetMaximum(1.4);
  h_trk_nconstit_frac_quark->SetFillStyle(0);
  h_trk_nconstit_frac_quark->SetLineStyle(7);
  h_trk_nconstit_frac_gluon->SetFillStyle(0);
  h_trk_nconstit_frac_gluon->SetLineStyle(7);
  h_trk_nconstit_purity_quark->SetLineColor(kGray+3);
  h_trk_nconstit_purity_quark->SetFillStyle(0);
  h_trk_nconstit_frac_quark->Draw("hist");
  h_trk_nconstit_frac_gluon->Draw("hist same");
  leg3->Draw();
  latex->DrawLatex(0.91,0.87,ihcalType.Data());
  latex->DrawLatex(0.91,0.80,"Primary jet");
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
  for (int ib =0; ib < h_twr_nconstit_frac_quark->GetSize()-2; ib++){
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
  h_twr_nconstit_frac_quark->SetMaximum(1.4);
  h_twr_nconstit_frac_quark->SetFillStyle(0);
  h_twr_nconstit_frac_quark->SetLineStyle(7);
  h_twr_nconstit_frac_gluon->SetFillStyle(0);
  h_twr_nconstit_frac_gluon->SetLineStyle(7);
  h_twr_nconstit_purity_quark->SetLineColor(kGray+3);
  h_twr_nconstit_purity_quark->SetFillStyle(0);
  h_twr_nconstit_frac_quark->Draw("hist");
  h_twr_nconstit_frac_gluon->Draw("hist same");
  //h_twr_nconstit_purity_quark->Draw("hist same");
  leg3->Draw();
  latex->DrawLatex(0.91,0.87,ihcalType.Data());
  latex->DrawLatex(0.91,0.80,"Primary jet");
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
  for (int ib =0; ib < h_p_pTD_frac_quark->GetSize()-2; ib++){
    quark_sum += h_p_pTD_frac_quark->GetBinContent(ib+1)/quark_tot;
    gluon_sum += h_p_pTD_frac_gluon->GetBinContent(ib+1)/gluon_tot;
    p_pTD_px.push_back(quark_sum); // efficiency
    p_pTD_py.push_back(1-gluon_sum); // rejection
    if (quark_sum+gluon_sum==0) { quark_purity=0;}
    else { quark_purity = quark_sum/(quark_sum+gluon_sum);}
    //cout << "p_pTD_px: " << p_pTD_px.at(ib) << ", p_pTD_py: " << p_pTD_py.at(ib) << endl;
    h_p_pTD_frac_quark->SetBinContent(ib+1,quark_sum); // efficiency
    h_p_pTD_frac_gluon->SetBinContent(ib+1,1-gluon_sum); // rejection
    h_p_pTD_purity_quark->SetBinContent(ib+1,quark_purity); 
  } 
  //cout << "Check size:" << p_pTD_px.size() << p_pTD_py.size() << endl;
  
  h_p_pTD_frac_quark->GetXaxis()->SetTitle("p_{T}D");
  h_p_pTD_frac_quark->SetMaximum(1.4);
  h_p_pTD_frac_quark->SetFillStyle(0);
  h_p_pTD_frac_quark->SetLineStyle(7);
  h_p_pTD_frac_gluon->SetFillStyle(0);
  h_p_pTD_frac_gluon->SetLineStyle(7);
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
  for (int ib =0; ib < h_trk_pTD_frac_quark->GetSize()-2; ib++){
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
  h_trk_pTD_frac_quark->SetMaximum(1.4);
  h_trk_pTD_frac_quark->SetFillStyle(0);
  h_trk_pTD_frac_quark->SetLineStyle(7);
  h_trk_pTD_frac_gluon->SetFillStyle(0);
  h_trk_pTD_frac_gluon->SetLineStyle(7);
  h_trk_pTD_purity_quark->SetLineColor(kGray+3);
  h_trk_pTD_purity_quark->SetFillStyle(0);
  h_trk_pTD_frac_quark->Draw("hist");
  h_trk_pTD_frac_gluon->Draw("hist same");
  leg3->Draw();
  latex->DrawLatex(0.91,0.87,ihcalType.Data());
  latex->DrawLatex(0.91,0.80,"Primary jet");
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
  for (int ib =0; ib < h_twr_pTD_frac_quark->GetSize()-2; ib++){
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
  h_twr_pTD_frac_quark->SetMaximum(1.4);
  h_twr_pTD_frac_quark->SetFillStyle(0);
  h_twr_pTD_frac_quark->SetLineStyle(7);
  h_twr_pTD_frac_gluon->SetFillStyle(0);
  h_twr_pTD_frac_gluon->SetLineStyle(7);
  h_twr_pTD_purity_quark->SetLineColor(kGray+3);
  h_twr_pTD_purity_quark->SetFillStyle(0);
  h_twr_pTD_frac_quark->Draw("hist");
  h_twr_pTD_frac_gluon->Draw("hist same");
  //h_twr_pTD_purity_quark->Draw("hist same");
  leg3->Draw();
  latex->DrawLatex(0.91,0.87,ihcalType.Data());
  latex->DrawLatex(0.91,0.80,"Primary jet");
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
  
  g_p_nconstit_roc->GetXaxis()->SetTitle("Light-quark jet efficiency");
  g_p_nconstit_roc->GetYaxis()->SetTitle("Gluon jet rejection");
  g_p_nconstit_roc->GetXaxis()->SetLimits(0,1);
  g_p_nconstit_roc->SetMinimum(0);
  g_p_nconstit_roc->SetMaximum(1);
  g_p_nconstit_roc->Draw("alp");
  g_trk_nconstit_roc->Draw("lp");
  g_twr_nconstit_roc->Draw("lp");
  leg4->AddEntry(g_p_nconstit_roc,"primary jet","lp");
  leg4->AddEntry(g_trk_nconstit_roc,"track jet","lp");
  leg4->AddEntry(g_twr_nconstit_roc,"tower jet","lp");
  leg4->Draw();
  c_p_nconstit_roc->SaveAs(Form("outMultivar/%s_p_nconstit_roc.pdf",ihcalType.Data()));
  
  ////////////////////////////////////////////////////////
  TFile* fout = new TFile(Form("outMultivar/%s_ROC.root",ihcalType.Data()),"RECREATE");
  fout->cd();
  g_p_nconstit_roc->Write();
  g_trk_nconstit_roc->Write();
  g_twr_nconstit_roc->Write();
  fout->Close();
   
  
  //TH2D* h2d_p_nconstit_roc = new TH2D("h2d_p_nconstit_roc",";light-quark jet efficiency;gluon jet rejection",41,0,1,41,0,1);
  //for (int ib=0; ib<h_p_nconstit_frac_quark->GetSize()-2; ib++){
  //  xbin = h2d_p_nconstit_roc->GetXaxis()->FindBin(h_p_nconstit_frac_quark->GetBinContent(ib+1));
  //  ybin = h2d_p_nconstit_roc->GetYaxis()->FindBin(h_p_nconstit_frac_gluon->GetBinContent(ib+1));
  //  h2d_p_nconstit_roc->SetBinContent(xbin,ybin,1);
  //}

  return; 
}
