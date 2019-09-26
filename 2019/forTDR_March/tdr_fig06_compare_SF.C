void tdr_fig06_compare_SF(string ihcalType="Alframe"){
  
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.038);

  TLegend* leg1;
  if (ihcalType.compare("Alframe")!=0) { cout << "ERROR!! select Alframe for TDR"; return; } 
  leg1 = new TLegend(0.20, 0.45, 0.42, 0.55);
  leg1 = new TLegend(0.20, 0.19, 0.42, 0.28);
  //leg1->SetTextSize(0.045);
  leg1->SetTextSize(0.035);
  //TLegend* leg2 = new TLegend(0.78, 0.68, 0.92, 0.86);
  TLegend* leg2 = new TLegend(0.20, 0.47, 0.38, 0.65);
  leg2->SetTextSize(0.040);
  TLegend* leg3 = new TLegend(0.20, 0.53, 0.38, 0.65);
  leg3->SetTextSize(0.040);
  
  //// select hist x axis range
  double ymin, ymax; 
  //ymin =0.9; ymax = 3.5;
  ymin =0.5; ymax = 3.5;

  //TFile* inFile01 = new TFile(Form("../z_20180410_diffE_vtx0_gammaEMCALonly_eta045/outvsE/SFvsE_%s.root",ihcalType.c_str()),"READ"); 
  //TFile* inFile02 = new TFile(Form("../z_20180410_py_vtx10gaus_gammaEMCALonly_eta045/out_stat/SFvsE_final_%s.root",ihcalType.c_str()),"READ"); 
  TFile* inFile01 = new TFile(Form("../../2018/CalcScale_diffE_vtx0/outvsE/SFvsE_%s.root",ihcalType.c_str()),"READ"); 
  TFile* inFile02 = new TFile(Form("../../2018/CalcScale_py8jet_vtx10gaus/out_stat/SFvsE_final_%s.root",ihcalType.c_str()),"READ"); 
  
  TGraphAsymmErrors* gcemc_2 = (TGraphAsymmErrors*)inFile01->Get("gcemc_2");
  TGraphAsymmErrors* gihcal_2 = (TGraphAsymmErrors*)inFile01->Get("gihcal_2");
  TGraphAsymmErrors* gohcal_2 = (TGraphAsymmErrors*)inFile01->Get("gohcal_2");
  TGraphAsymmErrors* gcemc_final = (TGraphAsymmErrors*)inFile02->Get("gcemc_final");
  TGraphAsymmErrors* gihcal_final = (TGraphAsymmErrors*)inFile02->Get("gihcal_final");
  TGraphAsymmErrors* gohcal_final = (TGraphAsymmErrors*)inFile02->Get("gohcal_final");

  TCanvas* c_sf = new TCanvas("c_sf","",600,600);
  c_sf->cd();
  
  SetGraphStyle(gcemc_2,1,0);
  SetGraphStyle(gihcal_2,2,0);
  SetGraphStyle(gohcal_2,3,0);
  SetGraphStyle(gcemc_final,1,3);
  SetGraphStyle(gihcal_final,2,3);
  SetGraphStyle(gohcal_final,3,3);
  
  gcemc_2->GetXaxis()->SetTitle("E_{#gamma}^{Reco} [GeV]");
  gcemc_2->GetXaxis()->CenterTitle(1);
  gcemc_2->GetYaxis()->SetTitle("Scale factors");
  gcemc_2->GetYaxis()->CenterTitle(1);
  gcemc_2->GetXaxis()->SetLimits(0,70.);
  gcemc_2->SetMinimum(ymin); 
  gcemc_2->SetMaximum(ymax); 
  
  //gcemc[0]->Draw("APL");
  //gcemc[1]->Draw("PL");
  //gcemc[2]->Draw("PL");
  //gcemc[3]->Draw("PL");
  //gcemc_2->Draw("PL");
  
  gcemc_2->Draw("APL");
  gohcal_2->Draw("PL");
  gcemc_final->Draw("PL");
  gohcal_final->Draw("PL");

  dashedLine(0.0,1.0,70.0,1.0,1,1);
  
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  //latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->DrawLatex(0.42,0.89,"MIE 2018");
  latex->SetTextSize(0.040);
  //latex->DrawLatex(0.2,0.82,Form("PYTHIA8 #gamma-jet, R=0.4, %s",sz_mean.c_str()));
  latex->DrawLatex(0.2,0.82,"PYTHIA8 #gamma-jet, R=0.4");

  TLegendEntry *le1 = leg1->AddEntry("leg1"," Realistic statistics (L_{int} #approx 45 pb^{-1})","lp");
  le1->SetMarkerStyle(kFullCross);
  le1->SetMarkerSize(1.8);
  TLegendEntry *le2 = leg1->AddEntry("leg2"," Enough statistics (50k events)","lp");
  le2->SetMarkerStyle(kFullCircle);
  le2->SetMarkerSize(1.2);
  leg1->Draw();
  leg2->AddEntry(gcemc_2," A");  
  leg2->AddEntry(gihcal_2," B");  
  leg2->AddEntry(gohcal_2," C");  
  leg3->AddEntry(gcemc_2," A");  
  leg3->AddEntry(gohcal_2," B");  
  leg3->Draw();
  c_sf->SaveAs(Form("out_tdr_figs/hcal_SFvsE_%s.pdf",ihcalType.c_str()));
   
  return;
}

