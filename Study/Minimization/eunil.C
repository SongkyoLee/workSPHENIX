Double_t f1(Double_t *x, Double_t *par)
{
  return par[0] + par[1]*x[0] + par[2]*x[0]*x[0];
}
void eunil() {
   gROOT->SetStyle("Plain");
   gROOT->ForceStyle();
   Int_t n = 4;
   Double_t *x1 = new Double_t[n];
   Double_t *y1 = new Double_t[n];
   Double_t *e1 = new Double_t[n];
   // save the data
   x1[0] = -0.6; x1[1] = -0.2; x1[2] =  0.2; x1[3] =  0.6;
   y1[0] =  5.0; y1[1] =  3.0; y1[2] =  5.0; y1[3] =  8.0;
   e1[0] =  2.0; e1[1] =  1.0; e1[2] =  1.0; e1[3] =  2.0;
   TF1 *func = new TF1("func",f1,-1.0,1.0,3);
   //create the graphs and set their drawing options
   TGraphErrors *gr1 = new TGraphErrors(n, x1, y1, 0, e1);
   gr1->SetLineColor(kRed);
   gr1->SetLineWidth(3.0);
   gr1->SetMarkerStyle(20);
   gr1->SetMarkerSize(3.0);
   TCanvas *myc = new TCanvas("myc","Fitting a parabola");
   gr1->Draw("ap");
   gr1->Fit("func");
 }
