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

void draw_slope_vs_ihcalsf_Al10(){

  double xmin = 3.5;
  double xmax = 7.5;
  double ymin = -2.5;
  double ymax = 2.5;

  const int ntr = 11;
  double px[ntr] = {1.00, 2.00, 3.00, 4.00, 5.00, 5.25, 5.50, 5.75, 6.00, 6.50, 7.00};
  double ex[ntr] = {0, 0, 0, 0, 0, 0, 0};
  double py[ntr] = {-1.214, -1.311, -1.120, -0.807, -0.382, -0.282, -0.141, -0.042, 0.109, 0.405, 0.676};
  double ey[ntr] = {0.028, 0.045, 0.057, 0.067, 0.074, 0.075, 0.077, 0.079, 0.080, 0.084, 0.087};

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
  latex->DrawLatex(0.2,0.88,"Al 10GeV");
  latex->DrawLatex(0.2,0.81,Form("Scale = %.2f",-(fit->GetParameter(0)/fit->GetParameter(1))));
  cout << "scale = "<<-(fit->GetParameter(0)/fit->GetParameter(1))<<endl;  
  c01->SaveAs("out2D_IOAsymm_Trial/slope_vs_ihcalsf_Al10.pdf");
  return;

}
