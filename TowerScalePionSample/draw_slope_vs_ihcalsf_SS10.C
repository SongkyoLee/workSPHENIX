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

void draw_slope_vs_ihcalsf_SS10(){

  double xmin = 1.1;
  double xmax = 2.4;
  double ymin = -3.5;
  double ymax = 3.5;

  const int ntr = 10;
  double px[ntr] = {1.00, 1.30, 1.60, 1.70, 1.75, 1.80, 1.85, 1.90, 2.20, 2.50};
  double ex[ntr] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  double py[ntr] = {-1.146, -0.765, -0.304, -0.118, -0.050, 0.038, 0.089, 0.193, 0.702, 1.207};  
  double ey[ntr] = {0.034, 0.040, 0.045, 0.046, 0.047, 0.048, 0.049, 0.049, 0.053, 0.056}; 


  TGraphErrors* gr = new TGraphErrors(ntr, px, py, ex, ey);
  cout << "gr = " << gr << endl;
  SetGraphStyle(gr,0,0);
  gr->GetXaxis()->SetTitle("IHCAL scale ( c_{IH} )");
  gr->GetYaxis()->SetTitle("Slope (p1)");
  gr->GetXaxis()->CenterTitle();
  gr->GetYaxis()->CenterTitle();
  gr->GetXaxis()->SetLimits(xmin,xmax);
  gr->SetMinimum(ymin);
  gr->SetMaximum(ymax);

  TF1 *fit = new TF1("fit","pol1", xmin, xmax); 
  gr->Fit(fit,"NR");
  fit->SetLineColor(kRed);

  TCanvas* c01 = new TCanvas("c01","c01",500,500);
  gr->Draw("ap");
  fit->Draw("same");
  dashedLine(xmin,0,xmax,0);
      
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.050);
  latex->DrawLatex(0.2,0.88,"SS310 10GeV");
  latex->DrawLatex(0.2,0.81,Form("Scale = %.2f",-(fit->GetParameter(0)/fit->GetParameter(1))));
  cout << "scale = "<<-(fit->GetParameter(0)/fit->GetParameter(1))<<endl;  
  c01->SaveAs("out2D_IOAsymm_Trial/slope_vs_ihcalsf_SS10.pdf");
  return;

}
