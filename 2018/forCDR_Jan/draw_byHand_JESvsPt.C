void draw_byHand_JESvsPt(){
  
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  //latex->SetTextSize(0.040);
  latex->SetTextSize(0.038);

  TLegend* leg1 = new TLegend(0.20, 0.57, 0.50, 0.71);
  leg1->SetTextSize(0.045);
  TLegend* leg2 = new TLegend(0.20, 0.21, 0.50, 0.35);
  leg2->SetTextSize(0.045);
  
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
  //// SS310
  double jesBefore01[nPtBin] = {7.62868e-01, 7.83131e-01};
  double jesAfter01[nPtBin] = {8.68162e-01, 9.69310e-01};
  //// Al
  double jesBefore02[nPtBin] = {7.42785e-01, 7.66041e-01};
  double jesAfter02[nPtBin] = {8.70827e-01, 1.01932e+00};
  //// Al_uninst
  double jesBefore03[nPtBin] = {7.22914e-01, 7.27940e-01};
  double jesAfter03[nPtBin] = {8.14807e-01, 8.91837e-01};
  
  double ex[nPtBin] = {5, 5};
  double ey[nPtBin] = {0, 0};

  TGraphErrors* gJESBefore01 = new TGraphErrors(nPtBin, xPnt, jesBefore01, ex, ey);
  TGraphErrors* gJESAfter01 = new TGraphErrors(nPtBin, xPnt, jesAfter01, ex, ey);
  TGraphErrors* gJESBefore02 = new TGraphErrors(nPtBin, xPnt, jesBefore02, ex, ey);
  TGraphErrors* gJESAfter02 = new TGraphErrors(nPtBin, xPnt, jesAfter02, ex, ey);
  TGraphErrors* gJESBefore03 = new TGraphErrors(nPtBin, xPnt, jesBefore03, ex, ey);
  TGraphErrors* gJESAfter03 = new TGraphErrors(nPtBin, xPnt, jesAfter03, ex, ey);
  
  gJESBefore01->SetLineColor(kGray+2);
  gJESBefore01->SetMarkerColor(kGray+3);
  gJESBefore01->SetMarkerSize(1.5);
  gJESBefore01->SetMarkerStyle(kFullCircle);
  gJESAfter01->SetLineColor(kGray+2);
  gJESAfter01->SetMarkerColor(kGray+3);
  gJESAfter01->SetMarkerSize(1.5);
  gJESAfter01->SetMarkerStyle(kFullCircle);
  //
  gJESBefore02->SetLineColor(kRed+1);
  gJESBefore02->SetMarkerColor(kRed+1);
  gJESBefore02->SetMarkerSize(1.5);
  gJESBefore02->SetMarkerStyle(kFullSquare);
  gJESAfter02->SetLineColor(kRed+1);
  gJESAfter02->SetMarkerColor(kRed+1);
  gJESAfter02->SetMarkerSize(1.5);
  gJESAfter02->SetMarkerStyle(kFullSquare);

  gJESBefore03->SetLineColor(kBlue+1);
  gJESBefore03->SetMarkerColor(kBlue+1);
  gJESBefore03->SetMarkerSize(2.0);
  gJESBefore03->SetMarkerStyle(kFullDiamond);
  gJESAfter03->SetLineColor(kBlue+1);
  gJESAfter03->SetMarkerColor(kBlue+1);
  gJESAfter03->SetMarkerSize(2.0);
  gJESAfter03->SetMarkerStyle(kFullDiamond);

  gJESBefore01->GetXaxis()->SetTitle("Jet E [GeV]");
  gJESBefore01->GetXaxis()->CenterTitle(1);
  gJESBefore01->GetYaxis()->SetTitle("Jet Energy Scale");
//  gJESBefore01->GetYaxis()->SetTitleOffset(1.8);
  gJESBefore01->GetYaxis()->CenterTitle(1);
  gJESBefore01->GetXaxis()->SetLimits(0,70.);
  //gJESBefore01->SetMinimum(0.5); 
  //gJESBefore01->SetMaximum(1.5); 
  gJESBefore01->SetMinimum(0.6); 
  gJESBefore01->SetMaximum(1.15); 
  
  //padJES01->cd(); 
  gJESBefore01->Draw("AP");
  gJESBefore02->Draw("P");
  gJESBefore03->Draw("P");
  //gJESAfter01->Draw("P");
  //gJESAfter02->Draw("P");
  //gJESAfter03->Draw("P");
//  gJESBefore03->Draw("P");
  leg1->AddEntry(gJESBefore01,"SS310","lp");
  leg1->AddEntry(gJESBefore02,"Al","lp");
  leg1->AddEntry(gJESBefore03,"Al uninst.","lp");
  leg1->Draw();

  dashedLine(0.0,1.0,70.0,1.0,1,1);
  
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4");
  latex->DrawLatex(0.2,0.77,"Jet |#eta^{truth}| < 1.1");
  
  c_JES->SaveAs("outJESvsPt/hcal_JESvsPt.pdf");
  ///////////////////////////////////////
  ///////////////////////////////////////
  //// Draw JER
  ///////////////////////////////////////
  ///////////////////////////////////////
  
  TCanvas* c_JER = new TCanvas("c_JER","",600,600);
  c_JER->cd();
  
  //// SS310
  double jerBefore01[nPtBin] = {1.87771e-01, 1.44306e-01};
  double jerAfter01[nPtBin] = {1.57522e-01, 1.13832e-01};
  //// Al
  double jerBefore02[nPtBin] = {2.10700e-01, 1.56483e-01};
  double jerAfter02[nPtBin] = {1.72768e-01, 1.21132e-01};
  //// Al_uninst
  double jerBefore03[nPtBin] = {2.21714e-01, 1.86735e-01};
  double jerAfter03[nPtBin] = {1.89848e-01, 1.34251e-01};

  TGraphErrors* gJERBefore01 = new TGraphErrors(nPtBin, xPnt, jerBefore01, ex, ey);
  TGraphErrors* gJERAfter01 = new TGraphErrors(nPtBin, xPnt, jerAfter01, ex, ey);
  TGraphErrors* gJERBefore02 = new TGraphErrors(nPtBin, xPnt, jerBefore02, ex, ey);
  TGraphErrors* gJERAfter02 = new TGraphErrors(nPtBin, xPnt, jerAfter02, ex, ey);
  TGraphErrors* gJERBefore03 = new TGraphErrors(nPtBin, xPnt, jerBefore03, ex, ey);
  TGraphErrors* gJERAfter03 = new TGraphErrors(nPtBin, xPnt, jerAfter03, ex, ey);
  
  gJERBefore01->SetLineColor(kGray+2);
  gJERBefore01->SetMarkerColor(kGray+3);
  gJERBefore01->SetMarkerSize(1.5);
  gJERBefore01->SetMarkerStyle(kFullCircle);
  gJERAfter01->SetLineColor(kGray+2);
  gJERAfter01->SetMarkerColor(kGray+3);
  gJERAfter01->SetMarkerSize(1.5);
  gJERAfter01->SetMarkerStyle(kFullCircle);
  //
  gJERBefore02->SetLineColor(kRed+1);
  gJERBefore02->SetMarkerColor(kRed+1);
  gJERBefore02->SetMarkerSize(1.5);
  gJERBefore02->SetMarkerStyle(kFullSquare);
  gJERAfter02->SetLineColor(kRed+1);
  gJERAfter02->SetMarkerColor(kRed+1);
  gJERAfter02->SetMarkerSize(1.5);
  gJERAfter02->SetMarkerStyle(kFullSquare);
//  gJERBefore02->SetMarkerStyle(kFullCircle);

  gJERBefore03->SetLineColor(kBlue+1);
  gJERBefore03->SetMarkerColor(kBlue+1);
  gJERBefore03->SetMarkerSize(2.0);
  gJERBefore03->SetMarkerStyle(kFullDiamond);
  gJERAfter03->SetLineColor(kBlue+1);
  gJERAfter03->SetMarkerColor(kBlue+1);
  gJERAfter03->SetMarkerSize(2.0);
  gJERAfter03->SetMarkerStyle(kFullDiamond);

  gJERBefore01->GetXaxis()->SetTitle("Jet E [GeV]");
  gJERBefore01->GetXaxis()->CenterTitle(1);
  gJERBefore01->GetYaxis()->SetTitle("Jet Energy Resolution");
//  gJERBefore01->GetYaxis()->SetTitleOffset(1.8);
  gJERBefore01->GetYaxis()->CenterTitle(1);
  gJERBefore01->GetXaxis()->SetLimits(0,70.);
  gJERBefore01->SetMinimum(0.10); 
  //gJERBefore01->SetMaximum(1.1); 
  gJERBefore01->SetMaximum(0.30); 
  
  //padJER01->cd(); 
  gJERBefore01->Draw("AP");
  gJERBefore02->Draw("P");
  gJERBefore03->Draw("P");
  //gJERAfter01->Draw("P");
  //gJERAfter02->Draw("P");
  //gJERAfter03->Draw("P");
  leg2->AddEntry(gJESBefore01,"SS310","lp");
  leg2->AddEntry(gJESBefore02,"Al","lp");
  leg2->AddEntry(gJESBefore03,"Al uninst.","lp");
  leg2->Draw();
    
  dashedLine(0.0,1.0,70.0,1.0,1,1);
  
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4");
  latex->DrawLatex(0.2,0.77,"Jet |#eta^{truth}| < 1.1");
  
  c_JER->SaveAs("outJESvsPt/hcal_JERvsPt.pdf");

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
