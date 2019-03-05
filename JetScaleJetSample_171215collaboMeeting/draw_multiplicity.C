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

void draw_multiplicity(TString ihcalType = "SS310", TString jetE = "20GeV", float cemc_sf=1.00, float ihcal_sf=1.00, float ohcal_sf=1.00)
{

  gStyle->SetPalette(51);
  //gStyle->SetPalette(55);
  //gStyle->SetPalette(1);
  
  //TString fname = Form("outModi/%s_ByGammaJet_%.0f_%.0f_%.0f.root",ihcalType.Data(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100);
  TString fname = Form("outJet4QG/%s_%s_ByGammaJet_%.0f_%.0f_%.0f.root",jetE.Data(), ihcalType.Data(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100);
  TFile* fin = new TFile(fname,"READ");
 
  TH1D* hjet_p_nconstit = (TH1D*)fin->Get("hjet_p_nconstit");
  TH1D* hjet_p_nconstit_quark = (TH1D*)fin->Get("hjet_p_nconstit_quark");
  TH1D* hjet_p_nconstit_gluon = (TH1D*)fin->Get("hjet_p_nconstit_gluon");
  TH1D* hjet_p_nconstit_else = (TH1D*)fin->Get("hjet_p_nconstit_else");
  TH1D* hjet_twr_nconstit = (TH1D*)fin->Get("hjet_twr_nconstit");
  TH1D* hjet_twr_nconstit_quark = (TH1D*)fin->Get("hjet_twr_nconstit_quark");
  TH1D* hjet_twr_nconstit_gluon = (TH1D*)fin->Get("hjet_twr_nconstit_gluon");
  TH1D* hjet_twr_nconstit_else = (TH1D*)fin->Get("hjet_twr_nconstit_else");

  ////////////////////////////////////////////////////////
  TLegend* leg = new TLegend(0.71, 0.54, 0.89, 0.74);
  TLegend* leg2 = new TLegend(0.68, 0.64, 0.89, 0.74);
  TLegend* leg3 = new TLegend(0.19, 0.71, 0.59, 0.81);
  TLegend* leg4 = new TLegend(0.21, 0.24, 0.49, 0.54);
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(32);
  latex->SetTextSize(0.045);
  
  ////////////////////////////////////////////////////////
  TPaletteAxis* pal = new TPaletteAxis();
        
  ////////////////////////////////////////////////////////
  //// 1) multiplicity
  ////////////////////////////////////////////////////////
  double max = 0;
  
  ////////////////////////////////////////////////////////
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
  c_p_nconstit->SaveAs(Form("outMultiplicity/%s_%s_p_nconstit.pdf",jetE.Data(),ihcalType.Data()));
  
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
  c_p_nconstit_norm->SaveAs(Form("outMultiplicity/%s_%s_p_nconstit_norm.pdf",jetE.Data(),ihcalType.Data()));
  
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
  c_twr_nconstit->SaveAs(Form("outMultiplicity/%s_%s_twr_nconstit.pdf",jetE.Data(),ihcalType.Data()));
  
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
  c_twr_nconstit_norm->SaveAs(Form("outMultiplicity/%s_%s_twr_nconstit_norm.pdf",jetE.Data(),ihcalType.Data()));
  
 /* 
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
  c_p_nconstit_frac->SaveAs(Form("outMultiplicity/%s_p_nconstit_frac.pdf",ihcalType.Data()));
 
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
  c_trk_nconstit_frac->SaveAs(Form("outMultiplicity/%s_trk_nconstit_frac.pdf",ihcalType.Data()));
 
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
  c_twr_nconstit_frac->SaveAs(Form("outMultiplicity/%s_twr_nconstit_frac.pdf",ihcalType.Data()));
 
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
  c_p_pTD_frac->SaveAs(Form("outMultiplicity/%s_p_pTD_frac.pdf",ihcalType.Data()));
 
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
  c_trk_pTD_frac->SaveAs(Form("outMultiplicity/%s_trk_pTD_frac.pdf",ihcalType.Data()));
 
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
  c_twr_pTD_frac->SaveAs(Form("outMultiplicity/%s_twr_pTD_frac.pdf",ihcalType.Data()));
 
  
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
  c_p_nconstit_roc->SaveAs(Form("outMultiplicity/%s_p_nconstit_roc.pdf",ihcalType.Data()));
  
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
  c_p_pTD_roc->SaveAs(Form("outMultiplicity/%s_p_pTD_roc.pdf",ihcalType.Data()));
  
  ////////////////////////////////////////////////////////
  TFile* fout = new TFile(Form("outMultiplicity/%s_ROC.root",ihcalType.Data()),"RECREATE");
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
