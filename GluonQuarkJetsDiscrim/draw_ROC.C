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
  TCanvas* c_ROC = new TCanvas("c_ROC","c_ROC",500,500);
  TLegend* leg = new TLegend(0.21, 0.24, 0.64, 0.59);
  leg->SetHeader("Multiplicity");
   
  for (int ih=0; ih<iNum; ih++){
    fname = Form("./outMultivar/%s_ROC.root",ihcalType[ih].Data());
    fin[ih] = new TFile(fname);
    //cout <<"fname : "<<fname<<endl;
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
  leg->Draw();
  dashedLine(0,1,1,0);
  c_ROC->SaveAs("outMultivar/COMP_nconstit_roc.pdf");

  return;
  return; 
}
