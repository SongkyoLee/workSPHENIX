void tdr_fig07_compare_JESJER(){
  
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.038);

  TLegend* leg01 = new TLegend(0.18, 0.61, 0.40, 0.76);
  leg01->SetTextSize(0.035);
  TLegend* leg02 = new TLegend(0.18, 0.21, 0.40, 0.36);
  leg02->SetTextSize(0.035);
  
  //// select hist x axis range
  double jesymin, jesymax;
  //jesymin =0.5; jesymax = 1.5;
  jesymin =0.6; jesymax = 1.5;
  double jerymin, jerymax;
  //jerymin =0.0; jerymax = 0.38;
  jerymin =0.0; jerymax = 0.35;

  //TFile* inFile01 = new TFile("../z_20180410_diffE_vtx0_gammaEMCALonly_eta045/outvsE/JESvsE_final_qcd_sigmean.root","READ"); 
  //TFile* inFile02 = new TFile("../z_20180410_py_vtx10gaus_gammaEMCALonly_eta045/out_stat/JESJERvsE_final_SS310_sigmean.root","READ"); 
  //TFile* inFile03 = new TFile("../z_20180410_py_vtx10gaus_gammaEMCALonly_eta045/out_stat/JESJERvsE_final_Alframe_sigmean.root","READ"); 
  TFile* inFile01 = new TFile("../../2018/CalcScale_diffE_vtx0/outvsE/JESvsE_final_qcd_sigmean.root","READ"); 
  TFile* inFile02 = new TFile("../../2018/CalcScale_py8jet_vtx10gaus/out_stat/JESJERvsE_final_SS310_sigmean.root","READ"); 
  TFile* inFile03 = new TFile("../../2018/CalcScale_py8jet_vtx10gaus/out_stat/JESJERvsE_final_Alframe_sigmean.root","READ"); 
 
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
  //c_jes->SetLeftMargin(0.18);
  c_jes->cd();
  
  SetGraphStyle(gjesSS310Before,0,11);
  SetGraphStyle(gjesSS310_2,0,0);
  SetGraphStyle(gjesSS310_final,0,3);
  SetGraphStyle(gjesAlframeBefore,1,11);
  SetGraphStyle(gjesAlframe_2,1,0);
  SetGraphStyle(gjesAlframe_final,1,3);
  SetSoftGraphStyle(gjesAlframeBefore,1,11);
  SetSoftGraphStyle(gjesAlframe_2,1,0);
  SetSoftGraphStyle(gjesAlframe_final,2,3);
  
  gjesAlframeBefore->GetXaxis()->SetTitle("E_{Jet}^{Truth} [GeV]");
  gjesAlframeBefore->GetXaxis()->CenterTitle(1);
  //gjesAlframeBefore->GetYaxis()->SetTitle("Jet Energy Scale");
  gjesAlframeBefore->GetYaxis()->SetTitle("<E_{Jet}^{Reco}/ E_{Jet}^{Truth}>");
  //gjesAlframeBefore->GetYaxis()->SetTitleOffset(1.7);
  gjesAlframeBefore->GetYaxis()->SetTitleOffset(1.5);
  gjesAlframeBefore->GetYaxis()->CenterTitle(1);
  gjesAlframeBefore->GetXaxis()->SetLimits(0,70.);
  gjesAlframeBefore->SetMinimum(jesymin); 
  gjesAlframeBefore->SetMaximum(jesymax); 
  
  gjesAlframeBefore->Draw("AP");
  gjesAlframe_2->Draw("P");
  gjesAlframeBefore->Draw("P");
  gjesAlframe_final->Draw("P");
  
  leg01->AddEntry(gjesAlframeBefore,"Before calibration","lp");
  leg01->AddEntry(gjesAlframe_2,"After calibration (with enough statistics)","lp");
  leg01->AddEntry(gjesAlframe_final,"After calibration (with realistic statistics)","lp");
  leg01->Draw();
  
  dashedLine(0.0,1.0,70.0,1.0,1,1);
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"MIE 2018");
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, R=0.4");
  
  c_jes->SaveAs("out_tdr_figs/hcal_JESvsE.pdf");
  
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
  //c_jer->SetLeftMargin(0.18);
  c_jer->cd();
  
  SetGraphStyle(gjerSS310Before,0,11);
  SetGraphStyle(gjerSS310_2,0,0);
  SetGraphStyle(gjerSS310_final,0,3);
  SetSoftGraphStyle(gjerAlframeBefore,1,11);
  SetSoftGraphStyle(gjerAlframe_2,1,0);
  SetSoftGraphStyle(gjerAlframe_final,2,3);
  
  gjerAlframeBefore->GetXaxis()->SetTitle("E_{Jet}^{Truth} [GeV]");
  gjerAlframeBefore->GetXaxis()->CenterTitle(1);
  //gjerAlframeBefore->GetYaxis()->SetTitle("Jet Energy Resolution");
  gjerAlframeBefore->GetYaxis()->SetTitle("#sigma[E_{Jet}^{Reco}/E_{Jet}^{Truth}] / <E_{Jet}^{Reco}/ E_{Jet}^{Truth}>");
  //gjerAlframeBefore->GetYaxis()->SetTitleOffset(1.7);
  gjerAlframeBefore->GetYaxis()->SetTitleOffset(1.5);
  gjerAlframeBefore->GetYaxis()->CenterTitle(1);
  gjerAlframeBefore->GetXaxis()->SetLimits(0,70.);
  gjerAlframeBefore->SetMinimum(jerymin); 
  gjerAlframeBefore->SetMaximum(jerymax); 
  
  gjerAlframeBefore->Draw("AP");
  gjerAlframe_2->Draw("P");
  gjerAlframe_final->Draw("P");

  leg02->AddEntry(gjerAlframeBefore,"Before calibration","lp");
  leg02->AddEntry(gjerAlframe_2,"After calibration (with enough statistics)","lp");
  leg02->AddEntry(gjerAlframe_final,"After calibration (with realistic statistics)","lp");
  leg02->Draw();
  
  dashedLine(0.0,1.0,70.0,1.0,1,1);
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"MIE 2018");
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, R=0.4");
  
  c_jer->SaveAs("out_tdr_figs/hcal_JERvsE.pdf");
   
  return;
}

