void draw_byHand_JESvsPt(){

  TLegend* leg1 = new TLegend(0.20, 0.79, 0.50, 0.92);
  TLegend* leg2 = new TLegend(0.50, 0.75, 0.90, 0.92);
  
  //// select hist x axis range
  int numbin;
  double binmin, binmax;
  double ymin, ymax; 
  numbin = 50; binmin = 0; binmax = 1.6;
  //numbin = 50; binmin = -1.0; binmax = 0.6;
  ymin =0.; ymax = 0.2;
  
  ///////////////////////////////////////
  //// Get Hist for different pT range
  //const int nPtBin = 4;
  const int nPtBin = 2;
  cout << "nPtBin = " << nPtBin << endl;
  //int xPnt[nPtBin] = {25, 35, 45, 55};
  double xPnt[nPtBin] = {25, 55};
  //double ex[nPtBin] = {5, 5};
  //double ey[nPtBin] = {0, 0};

  ///////////////////////////////////////
  //// Draw JES
  TCanvas* c_JES = new TCanvas("c_JES","",600,600);
  c_JES->cd();
/*
  TPad* padJES01 = new TPad("padJES01","",0.,0.5,1.,1.);
  padJES01->SetBottomMargin(0);
//  padJES01->SetGridx();
  padJES01->Draw();
  TPad* padJES02 = new TPad("padJES02","",0.,0.0,1.,0.5);
  padJES02->SetTopMargin(0);
//  padJES02->SetGridx();
  padJES02->Draw();
*/  
  //// SS310
  double jesBefore01[nPtBin] = {7.62868e-01, 7.83131e-01};
  double jesAfter01[nPtBin] = {8.68162e-01, 9.69310e-01};
  //// Al
  double jesBefore02[nPtBin] = {7.42785e-01, 7.66041e-01};
  double jesAfter02[nPtBin] = {8.70827e-01, 1.01932e+00};
  //// Al_uninst
  double jesBefore03[nPtBin] = {7.22914e-01, 7.27940e-01};
  double jesAfter03[nPtBin] = {8.14807e-01, 8.91837e-01};

  TGraph* gJESBefore01 = new TGraph(nPtBin, xPnt, jesBefore01);
  TGraph* gJESAfter01 = new TGraph(nPtBin, xPnt, jesAfter01);
  TGraph* gJESBefore02 = new TGraph(nPtBin, xPnt, jesBefore02);
  TGraph* gJESAfter02 = new TGraph(nPtBin, xPnt, jesAfter02);
  TGraph* gJESBefore03 = new TGraph(nPtBin, xPnt, jesBefore03);
  TGraph* gJESAfter03 = new TGraph(nPtBin, xPnt, jesAfter03);
  
  gJESBefore01->SetLineColor(kGray+2);
  gJESBefore01->SetMarkerColor(kGray+3);
  gJESBefore01->SetMarkerSize(2);
  gJESBefore01->SetMarkerStyle(kOpenCircle);
  gJESAfter01->SetLineColor(kGray+2);
  gJESAfter01->SetMarkerColor(kGray+3);
  gJESAfter01->SetMarkerSize(2);
  gJESAfter01->SetMarkerStyle(kFullCircle);
  //
  gJESBefore02->SetLineColor(kRed+1);
  gJESBefore02->SetMarkerColor(kRed+1);
  gJESBefore02->SetMarkerSize(2);
  gJESBefore02->SetMarkerStyle(kOpenSquare);
  gJESAfter02->SetLineColor(kRed+1);
  gJESAfter02->SetMarkerColor(kRed+1);
  gJESAfter02->SetMarkerSize(2);
  gJESAfter02->SetMarkerStyle(kFullSquare);

  gJESBefore03->SetLineColor(kBlue+1);
  gJESBefore03->SetMarkerColor(kBlue+1);
  gJESBefore03->SetMarkerSize(3);
  gJESBefore03->SetMarkerStyle(kOpenDiamond);
  gJESAfter03->SetLineColor(kBlue+1);
  gJESAfter03->SetMarkerColor(kBlue+1);
  gJESAfter03->SetMarkerSize(3);
  gJESAfter03->SetMarkerStyle(kFullDiamond);

  gJESBefore01->GetXaxis()->SetTitle("Jet E (GeV)");
  gJESBefore01->GetXaxis()->CenterTitle(1);
  gJESBefore01->GetYaxis()->SetTitle("JES");
//  gJESBefore01->GetYaxis()->SetTitleOffset(1.8);
  gJESBefore01->GetYaxis()->CenterTitle(1);
  gJESBefore01->GetXaxis()->SetLimits(0,70.);
  //gJESBefore01->SetMinimum(0.5); 
  //gJESBefore01->SetMaximum(1.5); 
  gJESBefore01->SetMinimum(0.6); 
  gJESBefore01->SetMaximum(1.2); 
  
  //padJES01->cd(); 
  gJESBefore01->Draw("AP");
  gJESBefore02->Draw("P");
  gJESBefore03->Draw("P");
  gJESAfter01->Draw("P");
  gJESAfter02->Draw("P");
  gJESAfter03->Draw("P");
//  gJESBefore03->Draw("P");
//  leg1->AddEntry(gJESAfter01,"SS310","lp");
//  leg1->AddEntry(gJESAfter02,"Al","lp");
//  leg1->AddEntry(gJESAfter03,"Al, no readout","lp");
  leg1->AddEntry(gJESBefore01,"Before scale","p");
  leg1->AddEntry(gJESAfter01,"After scale","p");
  leg1->Draw();

  dashedLine(0.0,1.0,70.0,1.0,1,1);
  
  c_JES->SaveAs("outJESvsPt/JESvsPt.pdf");
/*   
  // JES pull
  TGraphAsymmErrors* gJESBeforeRatio01 = new TGraphAsymmErrors(nPtBin, xPnt, meanRatio01);
  TGraphAsymmErrors* gJESBeforeRatio02 = new TGraphAsymmErrors(nPtBin, xPnt, meanRatio02);
  TGraphAsymmErrors* gJESBeforeRatio03 = new TGraphAsymmErrors(nPtBin, xPnt, meanRatio03);
  
  gJESBeforeRatio01->SetLineColor(kGray+2);
  gJESBeforeRatio01->SetMarkerColor(kGray+3);
  gJESBeforeRatio02->SetLineColor(kRed+1);
  gJESBeforeRatio02->SetMarkerColor(kRed+1);
  gJESBeforeRatio02->SetMarkerStyle(kOpenCircle);
  gJESBeforeRatio03->SetLineColor(kBlue+1);
  gJESBeforeRatio03->SetMarkerColor(kBlue+1);

  gJESBeforeRatio01->GetXaxis()->SetTitle("Anti-k_{T} R");
  gJESBeforeRatio01->GetXaxis()->CenterTitle(1);
  gJESBeforeRatio01->GetYaxis()->SetTitle("XX / SS310");
//  gJESBeforeRatio01->GetYaxis()->SetTitleOffset(1.8);
  gJESBeforeRatio01->GetYaxis()->CenterTitle(1);
  gJESBeforeRatio01->GetXaxis()->SetLimits(0.1,0.6);
  gJESBeforeRatio01->SetMinimum(0.9); 
  gJESBeforeRatio01->SetMaximum(1.1); 
  
  padJES02->cd(); 
  gJESBeforeRatio01->Draw("AP");
  gJESBeforeRatio02->Draw("P");
  gJESBeforeRatio03->Draw("P");
  dashedLine(0.1,1.,0.6,1.,1,1);
  
*/
  ///////////////////////////////////////
  ///////////////////////////////////////
  //// Draw JER
  ///////////////////////////////////////
  ///////////////////////////////////////
  
  TCanvas* c_JER = new TCanvas("c_JER","",600,600);
  c_JER->cd();
/*
  TPad* padJER01 = new TPad("padJER01","",0.,0.5,1.,1.);
  padJER01->SetBottomMargin(0);
//  padJER01->SetGridx();
  padJER01->Draw();
  TPad* padJER02 = new TPad("padJER02","",0.,0.0,1.,0.5);
  padJER02->SetTopMargin(0);
//  padJER02->SetGridx();
  padJER02->Draw();
*/  
  //// SS310
  double jerBefore01[nPtBin] = {1.87771e-01, 1.44306e-01};
  double jerAfter01[nPtBin] = {1.57522e-01, 1.13832e-01};
  //// Al
  double jerBefore02[nPtBin] = {2.10700e-01, 1.56483e-01};
  double jerAfter02[nPtBin] = {1.72768e-01, 1.21132e-01};
  //// Al_uninst
  double jerBefore03[nPtBin] = {2.21714e-01, 1.86735e-01};
  double jerAfter03[nPtBin] = {1.89848e-01, 1.34251e-01};

  TGraph* gJERBefore01 = new TGraph(nPtBin, xPnt, jerBefore01);
  TGraph* gJERAfter01 = new TGraph(nPtBin, xPnt, jerAfter01);
  TGraph* gJERBefore02 = new TGraph(nPtBin, xPnt, jerBefore02);
  TGraph* gJERAfter02 = new TGraph(nPtBin, xPnt, jerAfter02);
  TGraph* gJERBefore03 = new TGraph(nPtBin, xPnt, jerBefore03);
  TGraph* gJERAfter03 = new TGraph(nPtBin, xPnt, jerAfter03);
  
  gJERBefore01->SetLineColor(kGray+2);
  gJERBefore01->SetMarkerColor(kGray+3);
  gJERBefore01->SetMarkerSize(2);
  gJERBefore01->SetMarkerStyle(kOpenCircle);
  gJERAfter01->SetLineColor(kGray+2);
  gJERAfter01->SetMarkerColor(kGray+3);
  gJERAfter01->SetMarkerSize(2);
  gJERAfter01->SetMarkerStyle(kFullCircle);
  //
  gJERBefore02->SetLineColor(kRed+1);
  gJERBefore02->SetMarkerColor(kRed+1);
  gJERBefore02->SetMarkerSize(2);
  gJERBefore02->SetMarkerStyle(kOpenSquare);
  gJERAfter02->SetLineColor(kRed+1);
  gJERAfter02->SetMarkerColor(kRed+1);
  gJERAfter02->SetMarkerSize(2);
  gJERAfter02->SetMarkerStyle(kFullSquare);
//  gJERBefore02->SetMarkerStyle(kOpenCircle);

  gJERBefore03->SetLineColor(kBlue+1);
  gJERBefore03->SetMarkerColor(kBlue+1);
  gJERBefore03->SetMarkerSize(3);
  gJERBefore03->SetMarkerStyle(kOpenDiamond);
  gJERAfter03->SetLineColor(kBlue+1);
  gJERAfter03->SetMarkerColor(kBlue+1);
  gJERAfter03->SetMarkerSize(3);
  gJERAfter03->SetMarkerStyle(kFullDiamond);

  gJERBefore01->GetXaxis()->SetTitle("Jet E (GeV)");
  gJERBefore01->GetXaxis()->CenterTitle(1);
  gJERBefore01->GetYaxis()->SetTitle("JER");
//  gJERBefore01->GetYaxis()->SetTitleOffset(1.8);
  gJERBefore01->GetYaxis()->CenterTitle(1);
  gJERBefore01->GetXaxis()->SetLimits(0,70.);
  gJERBefore01->SetMinimum(0.05); 
  //gJERBefore01->SetMaximum(1.1); 
  gJERBefore01->SetMaximum(0.30); 
  
  //padJER01->cd(); 
  gJERBefore01->Draw("AP");
  gJERBefore02->Draw("P");
  gJERBefore03->Draw("P");
  gJERAfter01->Draw("P");
  gJERAfter02->Draw("P");
  gJERAfter03->Draw("P");
  leg2->AddEntry(gJERAfter01,"SS310","p");
  leg2->AddEntry(gJERAfter02,"Al","p");
  leg2->AddEntry(gJERAfter03,"Al, no readout","p");
//  leg2->AddEntry(gJERBefore01,"Before scale","p");
//  leg2->AddEntry(gJERAfter01,"After scale","p");
  leg2->Draw();

  dashedLine(0.0,1.0,70.0,1.0,1,1);
  c_JER->SaveAs("outJESvsPt/JERvsPt.pdf");





  return;
}
/*
void solidLine(double x1=0, double y1=0, double x2=0, double y2=0, int color, int width){
  TLine* t1 = new TLine(x1, y1, x2, y2);
  t1->SetLineStyle(1);
  t1->SetLineColor(color);
  t1->SetLineWidth(width);
  t1->Draw();
}

void dashedLine(double x1=0, double y1=0, double x2=0, double y2=0, int color, int width){
  TLine* t1 = new TLine(x1, y1, x2, y2);
  t1->SetLineStyle(7);
  t1->SetLineColor(color);
  t1->SetLineWidth(width);
  t1->Draw();
}
*/
