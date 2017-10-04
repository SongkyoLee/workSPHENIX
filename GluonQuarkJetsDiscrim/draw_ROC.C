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

void draw_ROC()
{

  gStyle->SetPalette(51);
  //gStyle->SetPalette(55);
  //gStyle->SetPalette(1);
  
  const int iNum = 5;
  TString ihcalType[iNum] = {
   "SS310_RSCALE_OUTPUT",
   "AL_RSCALE_OUTPUT",
   "AL_NOIHCAL_RSCALE_OUTPUT",
   "SS310_FRAME_RSCALE3_OUTPUT",
   "SS310_NOIHCAL_RSCALE2_OUTPUT" 
  };
  TString readableType[iNum] = {
   "SS310",
   "AL",
   "AL (no readout)",
   "SS310 frame",
   "SS310 (no readout)" 
  };
  
  TString fname;
  TFile* fin[iNum];
  TGraph* g_twr_nconstit_roc[iNum];
  TGraph* g_twr_pTD_roc[iNum];
  TLegend* leg = new TLegend(0.21, 0.24, 0.64, 0.59);
  TLegend* leg2 = new TLegend(0.21, 0.24, 0.64, 0.44);
  
  for (int ih=0; ih<iNum; ih++){
    fname = Form("./outMultivar/%s_ROC.root",ihcalType[ih].Data());
    fin[ih] = new TFile(fname);
    //cout <<"fname : "<<fname<<endl;
  }

  //// 1) Multiplicity 
  TCanvas* c_nconstit_ROC = new TCanvas("c_nconstit_ROC","c_nconstit_ROC",500,500);
  for (int ih=0; ih<iNum; ih++){
    g_twr_nconstit_roc[ih] = (TGraph*)fin[ih]->Get("g_twr_nconstit_roc");
    g_twr_nconstit_roc[ih]->SetName(Form("g_twr_nconstit_roc_%d",ih)); 
    SetGraphStyle(g_twr_nconstit_roc[ih],ih+1,10);
    //g_twr_nconstit_roc[ih]->SetMarkerStyle(7);
    g_twr_nconstit_roc[ih]->SetMarkerSize(0.5);
    g_twr_nconstit_roc[ih]->SetLineWidth(2);
    g_twr_nconstit_roc[ih]->GetXaxis()->SetLimits(0,1);
    g_twr_nconstit_roc[ih]->SetMinimum(0);
    g_twr_nconstit_roc[ih]->SetMaximum(1);
    g_twr_nconstit_roc[ih]->GetXaxis()->SetTitle("Quark jet efficiency");
    g_twr_nconstit_roc[ih]->GetYaxis()->SetTitle("Gluon jet rejection");
    leg->AddEntry(g_twr_nconstit_roc[ih],readableType[ih].Data(),"lp");
    if(ih==0) g_twr_nconstit_roc[ih]->Draw("alp");
    else g_twr_nconstit_roc[ih]->Draw("lp");
  }
  leg->SetHeader("Multiplicity");
  leg->Draw();
  dashedLine(0,1,1,0);
  c_nconstit_ROC->SaveAs("outMultivar/COMP_nconstit_roc.pdf");

  //// 2) pTD
  TCanvas* c_pTD_ROC = new TCanvas("c_pTD_ROC","c_pTD_ROC",500,500);
  for (int ih=0; ih<iNum; ih++){
    //fname = Form("./outMultivar/%s_ROC.root",ihcalType[ih].Data());
    //fin[ih] = new TFile(fname);
    //cout <<"fname : "<<fname<<endl;
    g_twr_pTD_roc[ih] = (TGraph*)fin[ih]->Get("g_twr_pTD_roc");
    g_twr_pTD_roc[ih]->SetName(Form("g_twr_pTD_roc_%d",ih)); 
    SetGraphStyle(g_twr_pTD_roc[ih],ih+1,10);
    //g_twr_pTD_roc[ih]->SetMarkerStyle(7);
    g_twr_pTD_roc[ih]->SetMarkerSize(0.5);
    g_twr_pTD_roc[ih]->SetLineWidth(2);
    g_twr_pTD_roc[ih]->GetXaxis()->SetLimits(0,1);
    g_twr_pTD_roc[ih]->SetMinimum(0);
    g_twr_pTD_roc[ih]->SetMaximum(1);
    g_twr_pTD_roc[ih]->GetXaxis()->SetTitle("Quark jet efficiency");
    g_twr_pTD_roc[ih]->GetYaxis()->SetTitle("Gluon jet rejection");
    //leg->AddEntry(g_twr_pTD_roc[ih],readableType[ih].Data(),"lp");
    if(ih==0) g_twr_pTD_roc[ih]->Draw("alp");
    else g_twr_pTD_roc[ih]->Draw("lp");
  }
  leg->SetHeader("p_{T}D");
  leg->Draw();
  dashedLine(0,1,1,0);
  c_pTD_ROC->SaveAs("outMultivar/COMP_pTD_roc.pdf");

  //// 3) Multiplicity vs. pTD
  //int cType = 2; // choose config type (2: AL_NOIHCAL, for example)
  TCanvas* c_comp[iNum];
  for (int cType=0; cType<iNum; cType++){
    c_comp[cType]= new TCanvas(Form("c_comp_%d",cType),"c_comp",500,500);
    SetGraphStyle(g_twr_nconstit_roc[cType],6,0);
    SetGraphStyle(g_twr_pTD_roc[cType],7,10);
    g_twr_nconstit_roc[cType]->SetMarkerSize(0.5);
    g_twr_nconstit_roc[cType]->SetLineWidth(2);
    g_twr_pTD_roc[cType]->SetMarkerSize(0.5);
    g_twr_pTD_roc[cType]->SetLineWidth(2);
    g_twr_nconstit_roc[cType]->Draw("alp");
    g_twr_pTD_roc[cType]->Draw("lp");
    leg2->SetHeader(readableType[cType].Data());
    if (cType==0){
      leg2->AddEntry(g_twr_nconstit_roc[cType],"Multiplicity","lp");
      leg2->AddEntry(g_twr_pTD_roc[cType],"p_{T}D","lp");
    }
    leg2->Draw();
    dashedLine(0,1,1,0);
    c_comp[cType]->SaveAs(Form("outMultivar/COMP_%s_roc.pdf",ihcalType[cType].Data()));
  }
  return; 
}
