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

void draw_E5x5_slope_vs_ihcalsf_Al25(){

  double xmin = 3.1;
  double xmax = 5.9;
  double ymin = -2.5;
  double ymax = 2.5;

  const int ntr = 11;
  double px[ntr] = {1.00, 2.00, 3.00, 3.50, 4.00, 4.25, 4.50, 4.75, 5.00, 5.50, 6.00};
  double ex[ntr] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  double py[ntr] = {-5.857, -5.002, -2.728, -1.521, -0.501, 0.016, 0.566, 1.129, 1.552, 2.532, 3.523}; 
  double ey[ntr] = {0.110, 0.143, 0.154, 0.158, 0.163, 0.166, 0.167, 0.169, 0.171, 0.176, 0.181};

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
  latex->DrawLatex(0.2,0.88,"Al 25GeV");
  latex->DrawLatex(0.2,0.81,Form("Scale = %.2f",-(fit->GetParameter(0)/fit->GetParameter(1))));
  cout << "scale = "<<-(fit->GetParameter(0)/fit->GetParameter(1))<<endl;  
  c01->SaveAs("out2D_E5x5_IOAsymm_Trial/slope_vs_ihcalsf_Al25.pdf");
  return;

}
