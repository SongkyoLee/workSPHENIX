void compare_SF(string ihcalType="SS310"){
  
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.038);

  TLegend* leg1 = new TLegend(0.20, 0.21, 0.50, 0.35);
  leg1->SetTextSize(0.045);
  TLegend* leg2 = new TLegend(0.20, 0.21, 0.50, 0.35);
  leg2->SetTextSize(0.045);
  
  //// select hist x axis range
  double ymin, ymax; 
  //ymin =0.9; ymax = 5.0;
  ymin =0.9; ymax = 3.5;

  TFile* inFile01 = new TFile(Form("../z_20180410_diffE_vtx0_gammaEMCALonly_eta045/outvsE/SFvsE_%s.root",ihcalType.c_str()),"READ"); 
  TFile* inFile02 = new TFile(Form("../z_20180410_py_vtx10gaus_gammaEMCALonly_eta045/out_stat/SFvsE_final_%s.root",ihcalType.c_str()),"READ"); 
  
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
  
  gcemc_2->GetXaxis()->SetTitle("#gamma E_{Reco} [GeV]");
  gcemc_2->GetXaxis()->CenterTitle(1);
  gcemc_2->GetYaxis()->SetTitle("Scale factor");
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
  if(ihcalType.compare("Alframe")!=0) gihcal_2->Draw("PL");
  gohcal_2->Draw("PL");
  gcemc_final->Draw("PL");
  if(ihcalType.compare("Alframe")!=0) gihcal_final->Draw("PL");
  gohcal_final->Draw("PL");

  dashedLine(0.0,1.0,70.0,1.0,1,1);
  
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.2,0.82,"PYTHIA8 #gamma-jet, anti-k_{T} R=0.4, |#eta| < 0.45");
  //latex->DrawLatex(0.2,0.77,"#gamma |#eta^{truth}| < 0.6");
  
  c_sf->SaveAs(Form("outvsE/compare_SFvsE_%s.pdf",ihcalType.c_str()));
   
  return;
}

