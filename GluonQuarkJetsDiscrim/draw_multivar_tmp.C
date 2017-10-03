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
// SS310_FRAME_RSCALE_OUTPUT (Not yet finished??)
//
//////////////

void draw_multivar(TString ihcalType = "SS310_OUTPUT")
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
  //h2d_prim_trk->Draw("");
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
  //// 1) # of constituents
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
  
  
  return; 
}
