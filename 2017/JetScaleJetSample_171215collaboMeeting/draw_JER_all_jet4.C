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

void draw_JER_all_jet4(string ihcalType="SS310",string jetE="50GeV", double fitmin=0.2, double fitmax=1.5, float cemc_sf=1.00, float ihcal_sf=1.00, float ohcal_sf=1.00){

  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.040);
  TLegend* leg = new TLegend(0.73, 0.65, 0.93, 0.78);
  
  TFile* f01 = new TFile(Form("outJet4QG/%s_%s_ByGammaJet_%.0f_%.0f_%.0f.root",jetE.c_str(),ihcalType.c_str(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100),"READ");
  TH2D* h01 = (TH2D*)f01->Get("h1D_modi_JER");
  TH2D* h02 = (TH2D*)f01->Get("h1D_quark_modi_JER");
  TH2D* h03 = (TH2D*)f01->Get("h1D_gluon_modi_JER");
  h01->Sumw2();
  h02->Sumw2();
  h03->Sumw2();
  h01->Scale(1./h01->Integral());
  h02->Scale(1./h02->Integral());
  h03->Scale(1./h03->Integral());
  
  TF1* gaus01 = new TF1("gaus01","gaus",fitmin,fitmax);
  gaus01->SetLineColor(kGray+2);
//  gaus01->SetLineColor(kRed+1);
  gaus01->SetLineStyle(1);
  TF2* gaus02 = new TF1("gaus02","gaus",fitmin,fitmax);
  gaus02->SetLineColor(kRed+2);
  //gaus02->SetLineStyle(2);
  TF2* gaus03 = new TF1("gaus03","gaus",fitmin,fitmax);
  gaus03->SetLineColor(kBlue+2);
  
  TCanvas*c01 = new TCanvas("c01","",500,500);
  SetHistStyle(h01,0,10);
  SetHistStyle(h02,1,0);
  SetHistStyle(h03,2,10);
  //h01->GetXaxis()->SetTitle("(E_{Tower jet}-E_{Truth jet}) / E_{Tower jet}");
  //h01->GetXaxis()->CenterTitle(1);
  //double ymax = h01->GetMaximum();
  //h01->SetMaximum(ymax*1.2);
  //h01->Draw("pe");
  h02->GetXaxis()->SetTitle("(E_{Tower jet}-E_{Truth jet}) / E_{Tower jet}");
  h02->GetXaxis()->CenterTitle(1);
  double ymax = h02->GetMaximum();
  h02->SetMaximum(ymax*1.2);
  h02->Draw("pe");
  h03->Draw("pe same");
  h01->Fit("gaus01","NR");
  //gaus01->Draw("same");
  h02->Fit("gaus02","NR");
  gaus02->Draw("same");
  h03->Fit("gaus03","NR");
  gaus03->Draw("same");
  
  float mean01 = gaus01->GetParameter(1);
  float sigma01 = gaus01->GetParameter(2);
  float res01 = sigma01/mean01;
  float mean02 = gaus02->GetParameter(1);
  float sigma02 = gaus02->GetParameter(2);
  float res02 = sigma02/mean02;
  float mean03 = gaus03->GetParameter(1);
  float sigma03 = gaus03->GetParameter(2);
  float res03 = sigma03/mean03;
  
  latex->SetTextSize(0.040);
  latex->SetTextAlign(32);
  latex->SetTextColor(kBlack);
  if (jetE.compare("20GeV")==0) { latex->DrawLatex(0.88,0.89,Form("%s, 20 < Jet E^{Truth}< 30 GeV/c",ihcalType.c_str())); }
  else if (jetE.compare("30GeV")==0) { latex->DrawLatex(0.88,0.89,Form("%s, 30 < Jet E^{Truth}< 40 GeV/c",ihcalType.c_str())); }
  else if (jetE.compare("40GeV")==0) { latex->DrawLatex(0.88,0.89,Form("%s, 40 < Jet E^{Truth}< 50 GeV/c",ihcalType.c_str())); }
  else if (jetE.compare("50GeV")==0) { latex->DrawLatex(0.88,0.89,Form("%s, 50 < Jet E^{Truth}< 60 GeV/c",ihcalType.c_str())); }
  latex->DrawLatex(0.88,0.83,"Anti-k_{T} R=0.4");
  latex->SetTextSize(0.030);
  latex->SetTextColor(kGray+3);
  //latex->DrawLatex(0.88,0.76,Form("#mu=%.2f, #sigma=%.2f, #sigma/#mu=%.2f",mean01,sigma01,res01));
  //latex->SetTextColor(kRed+1);
  //latex->DrawLatex(0.85,0.72,Form("#mu=%.2f, #sigma=%.2f, #sigma/#mu=%.2f",mean02,sigma02,res02));
      
  leg->AddEntry(h02,"Quark","lp");
  leg->AddEntry(h03,"Gluon","lp");
  leg->Draw();

  cout << "mean01: " << mean01 << endl; 
  cout << "sigma01: " << sigma01 << endl; 
  cout << "res01: " << res01 << endl; 
  cout << "mean02: " << mean02 << endl; 
  cout << "sigma02: " << sigma02 << endl; 
  cout << "res02: " << res02 << endl; 
  cout << "mean03: " << mean03 << endl; 
  cout << "sigma03: " << sigma03 << endl; 
  cout << "res03: " << res03 << endl; 
  c01->SaveAs(Form("outJet4JER/%s_%s_JER_all_ByGammaJet_%.0f_%.0f_%.0f.pdf",jetE.c_str(),ihcalType.c_str(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100));
  
  return;
}
  
