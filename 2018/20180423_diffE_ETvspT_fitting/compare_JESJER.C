void compare_JESJER(){
  
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.038);

  //TLegend* leg1 = new TLegend(0.20, 0.21, 0.50, 0.35);
  TLegend* leg1 = new TLegend(0.18, 0.20, 0.40, 0.30);
  leg1->SetTextSize(0.045);
  TLegend* leg2 = new TLegend(0.18, 0.20, 0.40, 0.30);
  leg2->SetTextSize(0.045);
  
  //// select hist x axis range
  double jesymin, jesymax;
  jesymin =0.5; jesymax = 1.5;
  double jerymin, jerymax;
  jerymin =0.0; jerymax = 0.35;

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

  TCanvas* c_jes = new TCanvas("c_jes","",600,600);
  c_jes->cd();
  
  SetGraphStyle(gjesSS310Before,0,10);
  SetGraphStyle(gjesSS310_2,0,0);
  SetGraphStyle(gjesSS310_final,0,3);
  SetGraphStyle(gjesAlframeBefore,1,10);
  SetGraphStyle(gjesAlframe_2,1,0);
  SetGraphStyle(gjesAlframe_final,1,3);
  
  
  gjesSS310Before->GetXaxis()->SetTitle("Jet E [GeV]");
  gjesSS310Before->GetXaxis()->CenterTitle(1);
  gjesSS310Before->GetYaxis()->SetTitle("Jet Energy Scale");
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

  leg1->AddEntry(gjesSS310_final,"SS310","lp");
  leg1->AddEntry(gjesAlframe_final,"Al frame","lp");
  leg1->Draw();
  
  dashedLine(0.0,1.0,70.0,1.0,1,1);
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.2,0.82,"PYTHIA8 #gamma-jet, anti-k_{T} R=0.4, |#eta| < 0.45");
  
  c_jes->SaveAs("outvsE/compare_JESvsE.pdf");
  
 
  ///////////////////////////////////////
  //// Draw JER
  ///////////////////////////////////////

  TGraphAsymmErrors* gjerSS310Before = (TGraphAsymmErrors*)inFile01->Get("gjerSS310Before");
  TGraphAsymmErrors* gjerSS310_2 = (TGraphAsymmErrors*)inFile01->Get("gjerSS310_2");
  TGraphAsymmErrors* gjerSS310_final = (TGraphAsymmErrors*)inFile02->Get("gjer_final");
  TGraphAsymmErrors* gjerAlframeBefore = (TGraphAsymmErrors*)inFile01->Get("gjerAlframeBefore");
  TGraphAsymmErrors* gjerAlframe_2 = (TGraphAsymmErrors*)inFile01->Get("gjerAlframe_2");
  TGraphAsymmErrors* gjerAlframe_final = (TGraphAsymmErrors*)inFile03->Get("gjer_final");

  TCanvas* c_jer = new TCanvas("c_jer","",600,600);
  c_jer->cd();
  
  SetGraphStyle(gjerSS310Before,0,10);
  SetGraphStyle(gjerSS310_2,0,0);
  SetGraphStyle(gjerSS310_final,0,3);
  SetGraphStyle(gjerAlframeBefore,1,10);
  SetGraphStyle(gjerAlframe_2,1,0);
  SetGraphStyle(gjerAlframe_final,1,3);
  
  gjerSS310Before->GetXaxis()->SetTitle("Jet E [GeV]");
  gjerSS310Before->GetXaxis()->CenterTitle(1);
  gjerSS310Before->GetYaxis()->SetTitle("Jet Energy Resolution");
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
  
  dashedLine(0.0,1.0,70.0,1.0,1,1);
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.2,0.82,"PYTHIA8 #gamma-jet, anti-k_{T} R=0.4, |#eta| < 0.45");
  
  c_jer->SaveAs("outvsE/compare_JERvsE.pdf");
   
  return;
}

