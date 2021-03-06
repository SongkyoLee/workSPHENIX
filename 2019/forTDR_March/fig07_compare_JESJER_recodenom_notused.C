void fig07_compare_JESJER_recodenom(){
  
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.038);

  //TLegend* leg1 = new TLegend(0.20, 0.21, 0.50, 0.35);
  TLegend* leg1 = new TLegend(0.18, 0.19, 0.40, 0.28);
  leg1->SetTextSize(0.035);
  TLegend* leg2 = new TLegend(0.66, 0.79, 0.89, 0.91);
  leg2->SetTextSize(0.031);
  TLegend* leg3 = new TLegend(0.66, 0.65, 0.89, 0.77);
  leg3->SetTextSize(0.031);
  
  //// select hist x axis range
  double jesymin, jesymax;
  jesymin =0.5; jesymax = 1.5;
  double jerymin, jerymax;
  jerymin =0.0; jerymax = 0.38;

  TFile* inFile01 = new TFile("../z_20180410_diffE_vtx0_gammaEMCALonly_eta045/outvsE/JESvsE_final_qcd.root","READ"); 
  TFile* inFile02 = new TFile("../z_20180410_py_vtx10gaus_gammaEMCALonly_eta045/out_stat/JESJERvsE_final_SS310.root","READ"); 
  TFile* inFile03 = new TFile("../z_20180410_py_vtx10gaus_gammaEMCALonly_eta045/out_stat/JESJERvsE_final_Alframe.root","READ"); 
 
  ///////////////////////////////////////
  //// Draw JES
  ///////////////////////////////////////

  TGraphAsymmErrors* gjesSS310Before = (TGraphAsymmErrors*)inFile01->Get("gjesSS310Before");
  TGraphAsymmErrors* gjesSS310_2 = (TGraphAsymmErrors*)inFile01->Get("gjesSS310_2");
  TGraphAsymmErrors* gjesSS310_final = (TGraphAsymmErrors*)inFile02->Get("gjes_final");
  TGraphAsymmErrors* gjesAlframeBefore = (TGraphAsymmErrors*)inFile01->Get("gjesAlframeBefore");
  TGraphAsymmErrors* gjesAlframe_2 = (TGraphAsymmErrors*)inFile01->Get("gjesAlframe_2");
  TGraphAsymmErrors* gjesAlframe_final = (TGraphAsymmErrors*)inFile03->Get("gjes_final");

  TCanvas* c_jes = new TCanvas("c_jes","",650,600);
  //c_jes->SetLeftMargin(0.18);
  c_jes->cd();
  
  SetGraphStyle(gjesSS310Before,0,11);
  SetGraphStyle(gjesSS310_2,0,0);
  SetGraphStyle(gjesSS310_final,0,3);
  SetGraphStyle(gjesAlframeBefore,1,11);
  SetGraphStyle(gjesAlframe_2,1,0);
  SetGraphStyle(gjesAlframe_final,1,3);
  
  
  gjesSS310Before->GetXaxis()->SetTitle("E_{Jet}^{Truth} [GeV]");
  gjesSS310Before->GetXaxis()->CenterTitle(1);
  //gjesSS310Before->GetYaxis()->SetTitle("Jet Energy Scale");
  gjesSS310Before->GetYaxis()->SetTitle("<E_{Jet}^{Reco}/ E_{Jet}^{Truth}>");
  //gjesSS310Before->GetYaxis()->SetTitleOffset(1.7);
  gjesSS310Before->GetYaxis()->SetTitleOffset(1.5);
  gjesSS310Before->GetYaxis()->CenterTitle(1);
  gjesSS310Before->GetXaxis()->SetLimits(0,70.);
  gjesSS310Before->SetMinimum(jesymin); 
  gjesSS310Before->SetMaximum(jesymax); 
  
  gjesSS310Before->Draw("AP");
  gjesSS310_2->Draw("P");
  gjesAlframeBefore->Draw("P");
  gjesAlframe_2->Draw("P");
  gjesSS310Before->Draw("P");
  gjesAlframeBefore->Draw("P");
  gjesSS310_final->Draw("P");
  gjesAlframe_final->Draw("P");

  TLegendEntry *le1 = leg1->AddEntry("leg1"," Realistic statistics (L_{int} #approx 45 pb^{-1})","lp");
  le1->SetMarkerStyle(kFullCross);
  le1->SetMarkerSize(1.8);
  TLegendEntry *le2 = leg1->AddEntry("leg2"," Enough statistics (50k events)","lp");
  le2->SetMarkerStyle(kFullCircle);
  le2->SetMarkerSize(1.2);
  leg1->Draw();
/*
  leg2->SetHeader("Before scaling");
  leg2->AddEntry(gjesSS310Before,"Instrumented Steel","lp");
  leg2->AddEntry(gjesAlframeBefore,"CD-1 Configuration","lp");
  leg2->Draw();
  leg3->SetHeader("After scaling");
  leg3->AddEntry(gjesSS310_2,"Instrumented Steel","lp");
  leg3->AddEntry(gjesAlframe_2,"CD-1 Configuration","lp");
  leg3->Draw();
*/
  leg2->SetHeader("CD-1 Configuration");
  leg2->AddEntry(gjesAlframeBefore,"Before calib.","lp");
  leg2->AddEntry(gjesAlframe_2,"After calib.","lp");
  leg2->Draw();
  leg3->SetHeader("Instrumented Steel");
  leg3->AddEntry(gjesSS310Before,"Before calib.","lp");
  leg3->AddEntry(gjesSS310_2,"After calib.","lp");
  leg3->Draw();
  
  //dashedLine(0.0,1.0,70.0,1.0,1,1);
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"MIE 2018");
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, R=0.4");
  
  c_jes->SaveAs("out_figs/hcal_JESvsE_recodenom.pdf");
  
 
  ///////////////////////////////////////
  //// Draw JER
  ///////////////////////////////////////

  TGraphAsymmErrors* gjerSS310Before = (TGraphAsymmErrors*)inFile01->Get("gjerSS310Before");
  TGraphAsymmErrors* gjerSS310_2 = (TGraphAsymmErrors*)inFile01->Get("gjerSS310_2");
  TGraphAsymmErrors* gjerSS310_final = (TGraphAsymmErrors*)inFile02->Get("gjer_final");
  TGraphAsymmErrors* gjerAlframeBefore = (TGraphAsymmErrors*)inFile01->Get("gjerAlframeBefore");
  TGraphAsymmErrors* gjerAlframe_2 = (TGraphAsymmErrors*)inFile01->Get("gjerAlframe_2");
  TGraphAsymmErrors* gjerAlframe_final = (TGraphAsymmErrors*)inFile03->Get("gjer_final");

  TCanvas* c_jer = new TCanvas("c_jer","",650,600);
  //c_jer->SetLeftMargin(0.18);
  c_jer->cd();
  
  SetGraphStyle(gjerSS310Before,0,11);
  SetGraphStyle(gjerSS310_2,0,0);
  SetGraphStyle(gjerSS310_final,0,3);
  SetGraphStyle(gjerAlframeBefore,1,11);
  SetGraphStyle(gjerAlframe_2,1,0);
  SetGraphStyle(gjerAlframe_final,1,3);
  
  gjerSS310Before->GetXaxis()->SetTitle("E_{Jet}^{Truth} [GeV]");
  gjerSS310Before->GetXaxis()->CenterTitle(1);
  //gjerSS310Before->GetYaxis()->SetTitle("Jet Energy Resolution");
  //gjerSS310Before->GetYaxis()->SetTitle("#sigma[E_{Jet}^{Reco}/E_{Jet}^{Truth}] / <E_{Jet}^{Reco}/ E_{Jet}^{Truth}>");
  gjerSS310Before->GetYaxis()->SetTitle("#sigma[ (E_{Jet}^{Truth} - E_{Jet}^{Reco}) / E_{Jet}^{Reco} ]");
  //gjerSS310Before->GetYaxis()->SetTitleOffset(1.7);
  gjerSS310Before->GetYaxis()->SetTitleOffset(1.5);
  gjerSS310Before->GetYaxis()->CenterTitle(1);
  gjerSS310Before->GetXaxis()->SetLimits(0,70.);
  gjerSS310Before->SetMinimum(jerymin); 
  gjerSS310Before->SetMaximum(jerymax); 
  
  gjerSS310Before->Draw("AP");
  gjerSS310_2->Draw("P");
  gjerAlframeBefore->Draw("P");
  gjerAlframe_2->Draw("P");
  gjerSS310Before->Draw("P");
  gjerAlframeBefore->Draw("P");
  gjerSS310_final->Draw("P");
  gjerAlframe_final->Draw("P");

  //leg1->AddEntry(gjesSS310_final,"SS310","lp");
  //leg1->AddEntry(gjesAlframe_final,"Al frame","lp");
  leg1->Draw();
  leg2->Draw();
  leg3->Draw();
  
  dashedLine(0.0,1.0,70.0,1.0,1,1);
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"MIE 2018");
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, R=0.4");
  
  c_jer->SaveAs("out_figs/hcal_JERvsE_recodenom.pdf");
   
  return;
}

