#include "TMinuit.h"

Double_t func(Double_t x, Double_t *par)
{
  return par[0] + par[1]*x + par[2]*x*x;
}
void fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{
  gROOT->SetStyle("Plain");
  gROOT->ForceStyle();
  Int_t n = 4;
  Double_t *x1 = new Double_t[n];
  Double_t *y1 = new Double_t[n];
  Double_t *e1 = new Double_t[n];
  x1[0] = -0.6; x1[1] = -0.2; x1[2] =  0.2; x1[3] =  0.6;
  y1[0] =  5.0; y1[1] =  3.0; y1[2] =  5.0; y1[3] =  8.0;
  e1[0] =  2.0; e1[1] =  1.0; e1[2] =  1.0; e1[3] =  2.0;
  
  Int_t i;
  Double_t chisq = 0;
  Double_t delta;
  for (i=0;i<n; i++) {
    delta  = (y1[i]-func(x1[i],par))/e1[i];
    chisq += delta*delta;
  }
f = chisq;
}

void eunil_minuit()
{
   //
   // calls MINUIT explicitly
   //
   TMinuit *gMinuit = new TMinuit(3);  // initialize with a maximum of 3 params
   gMinuit->SetFCN(fcn);               // set the minimization function
   Double_t arglist[10];
   Int_t ierflg = 0;
   arglist[0] = 1;
   gMinuit->mnexcm("SET ERR", arglist ,1,ierflg);
   static Double_t vstart[3] = {0.0, 0.0, 0.0};
   static Double_t step[3] = {0.001, 0.001, 0.001};
   gMinuit->mnparm(0, "theta_0", vstart[0], step[0], 0,0,ierflg);
   gMinuit->mnparm(1, "theta_1", vstart[1], step[1], 0,0,ierflg);
   gMinuit->mnparm(2, "theta_2", vstart[2], step[2], 0,0,ierflg);
   arglist[0] = 5000;
   arglist[1] = 1.;
   gMinuit->mnexcm("MINUIT", arglist ,2,ierflg);
}

