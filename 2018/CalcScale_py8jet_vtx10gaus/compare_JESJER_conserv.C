void compare_JESJER_conserv(){
  
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

  ////// read txt and make TGraphAsymmErrors
  const int newMeth=1;
  double jesSS310min[newMeth];
  double jesSS310max[newMeth];
  double jesAlframemin[newMeth];
  double jesAlframemax[newMeth];
  
  string szjes;
  int ijes=0;
  ifstream infile_jesSS310min;
  infile_jesSS310min.open("txt_conserv/jes_mean_minimum_SS310.out");
  ijes=0;
  if (infile_jesSS310min.is_open()){
    while( getline (infile_jesSS310min,szjes) ) { jesSS310min[ijes] = atof(szjes.c_str()); ijes++; }
  }
  cout << "jesSS310min[ijes] = " << jesSS310min[ijes] << endl;
  ifstream infile_jesSS310max;
  infile_jesSS310max.open("txt_conserv/jes_mean_maximum_SS310.out");
  ijes=0;
  if (infile_jesSS310max.is_open()){
    while( getline (infile_jesSS310max,szjes) ) { jesSS310max[ijes] = atof(szjes.c_str()); ijes++; }
  }
  cout << "jesSS310max[ijes] = " << jesSS310max[ijes] << endl;
  ifstream infile_jesAlframemin;
  infile_jesAlframemin.open("txt_conserv/jes_mean_minimum_Alframe.out");
  ijes=0;
  if (infile_jesAlframemin.is_open()){
    while( getline (infile_jesAlframemin,szjes) ) { jesAlframemin[ijes] = atof(szjes.c_str()); ijes++; }
  }
  cout << "jesAlframemin[ijes] = " << jesAlframemin[ijes] << endl;
  ifstream infile_jesAlframemax;
  infile_jesAlframemax.open("txt_conserv/jes_mean_maximum_Alframe.out");
  ijes=0;
  if (infile_jesAlframemax.is_open()){
    while( getline (infile_jesAlframemax,szjes) ) { jesAlframemax[ijes] = atof(szjes.c_str()); ijes++; }
  }
  cout << "jesAlframemax[ijes] = " << jesAlframemax[ijes] << endl;

  double tmpjesx, tmpjesy;
  gjesSS310_final->GetPoint(0,tmpjesx,tmpjesy);
  gjesSS310_final->SetPointError(0,0,0,fabs(tmpjesy-jesSS310min[0]),fabs(tmpjesy-jesSS310max[0]));
  gjesAlframe_final->GetPoint(0,tmpjesx,tmpjesy);
  gjesAlframe_final->SetPointError(0,0,0,fabs(tmpjesy-jesAlframemin[0]),fabs(tmpjesy-jesAlframemax[0]));

  ///////////////////////////////////////
  
  TCanvas* c_jes = new TCanvas("c_jes","",600,600);
  c_jes->cd();
  
  SetGraphStyle(gjesSS310Before,0,0);
  SetGraphStyle(gjesSS310_2,0,10);
  SetGraphStyle(gjesSS310_final,0,13);
  SetGraphStyle(gjesAlframeBefore,1,0);
  SetGraphStyle(gjesAlframe_2,1,10);
  SetGraphStyle(gjesAlframe_final,1,13);
  
  gjesSS310Before->GetXaxis()->SetTitle("Jet E [GeV]");
  gjesSS310Before->GetXaxis()->CenterTitle(1);
  gjesSS310Before->GetYaxis()->SetTitle("Jet Energy Scale");
  gjesSS310Before->GetYaxis()->CenterTitle(1);
  gjesSS310Before->GetXaxis()->SetLimits(0,70.);
  gjesSS310Before->SetMinimum(jesymin); 
  gjesSS310Before->SetMaximum(jesymax); 
  
  gjesSS310Before->Draw("AP");
  gjesSS310_2->Draw("P");
  gjesSS310_final->Draw("P");
  gjesAlframeBefore->Draw("P");
  gjesAlframe_2->Draw("P");
  gjesAlframe_final->Draw("P");

  leg1->AddEntry(gjesSS310Before,"SS310","lp");
  leg1->AddEntry(gjesAlframeBefore,"Al frame","lp");
  leg1->Draw();
  
  dashedLine(0.0,1.0,70.0,1.0,1,1);
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.2,0.82,"PYTHIA8 #gamma-jet, anti-k_{T} R=0.4, |#eta| < 0.45");
  
  c_jes->SaveAs("outvsE/compare_JESvsE_conserv.pdf");
 
  ///////////////////////////////////////
  //// Draw JER
  ///////////////////////////////////////

  TGraphAsymmErrors* gjerSS310Before = (TGraphAsymmErrors*)inFile01->Get("gjerSS310Before");
  TGraphAsymmErrors* gjerSS310_2 = (TGraphAsymmErrors*)inFile01->Get("gjerSS310_2");
  TGraphAsymmErrors* gjerSS310_final = (TGraphAsymmErrors*)inFile02->Get("gjer_final");
  TGraphAsymmErrors* gjerAlframeBefore = (TGraphAsymmErrors*)inFile01->Get("gjerAlframeBefore");
  TGraphAsymmErrors* gjerAlframe_2 = (TGraphAsymmErrors*)inFile01->Get("gjerAlframe_2");
  TGraphAsymmErrors* gjerAlframe_final = (TGraphAsymmErrors*)inFile03->Get("gjer_final");
  
  ////// read txt and make TGraphAsymmErrors
  //const int newMeth=1;
  double jerSS310min[newMeth];
  double jerSS310max[newMeth];
  double jerAlframemin[newMeth];
  double jerAlframemax[newMeth];
  
  string szjer;
  int ijer=0;
  ifstream infile_jerSS310min;
  infile_jerSS310min.open("txt_conserv/jer_sig_minimum_SS310.out");
  ijer=0;
  if (infile_jerSS310min.is_open()){
    while( getline (infile_jerSS310min,szjer) ) { jerSS310min[ijer] = atof(szjer.c_str()); ijer++; }
  }
  cout << "jerSS310min[ijer] = " << jerSS310min[ijer] << endl;
  ifstream infile_jerSS310max;
  infile_jerSS310max.open("txt_conserv/jer_sig_maximum_SS310.out");
  ijer=0;
  if (infile_jerSS310max.is_open()){
    while( getline (infile_jerSS310max,szjer) ) { jerSS310max[ijer] = atof(szjer.c_str()); ijer++; }
  }
  cout << "jerSS310max[ijer] = " << jerSS310max[ijer] << endl;
  ifstream infile_jerAlframemin;
  infile_jerAlframemin.open("txt_conserv/jer_sig_minimum_Alframe.out");
  ijer=0;
  if (infile_jerAlframemin.is_open()){
    while( getline (infile_jerAlframemin,szjer) ) { jerAlframemin[ijer] = atof(szjer.c_str()); ijer++; }
  }
  cout << "jerAlframemin[ijer] = " << jerAlframemin[ijer] << endl;
  ifstream infile_jerAlframemax;
  infile_jerAlframemax.open("txt_conserv/jer_sig_maximum_Alframe.out");
  ijer=0;
  if (infile_jerAlframemax.is_open()){
    while( getline (infile_jerAlframemax,szjer) ) { jerAlframemax[ijer] = atof(szjer.c_str()); ijer++; }
  }
  cout << "jerAlframemax[ijer] = " << jerAlframemax[ijer] << endl;

  double tmpjerx, tmpjery;
  gjerSS310_final->GetPoint(0,tmpjerx,tmpjery);
  gjerSS310_final->SetPointError(0,0,0,fabs(tmpjery-jerSS310min[0]),fabs(tmpjery-jerSS310max[0]));
  gjerAlframe_final->GetPoint(0,tmpjerx,tmpjery);
  gjerAlframe_final->SetPointError(0,0,0,fabs(tmpjery-jerAlframemin[0]),fabs(tmpjery-jerAlframemax[0]));

  ///////////////////////////////////////

  TCanvas* c_jer = new TCanvas("c_jer","",600,600);
  c_jer->cd();
  
  SetGraphStyle(gjerSS310Before,0,0);
  SetGraphStyle(gjerSS310_2,0,10);
  SetGraphStyle(gjerSS310_final,0,13);
  SetGraphStyle(gjerAlframeBefore,1,0);
  SetGraphStyle(gjerAlframe_2,1,10);
  SetGraphStyle(gjerAlframe_final,1,13);
  
  gjerSS310Before->GetXaxis()->SetTitle("Jet E [GeV]");
  gjerSS310Before->GetXaxis()->CenterTitle(1);
  gjerSS310Before->GetYaxis()->SetTitle("Jet Energy Resolution");
  gjerSS310Before->GetYaxis()->CenterTitle(1);
  gjerSS310Before->GetXaxis()->SetLimits(0,70.);
  gjerSS310Before->SetMinimum(jerymin); 
  gjerSS310Before->SetMaximum(jerymax); 
  
  gjerSS310Before->Draw("AP");
  gjerSS310_2->Draw("P");
  gjerSS310_final->Draw("P");
  gjerAlframeBefore->Draw("P");
  gjerAlframe_2->Draw("P");
  gjerAlframe_final->Draw("P");

  //leg1->AddEntry(gjesSS310Before,"SS310","lp");
  //leg1->AddEntry(gjesAlframeBefore,"Al frame","lp");
  leg1->Draw();
  
  dashedLine(0.0,1.0,70.0,1.0,1,1);
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.2,0.82,"PYTHIA8 #gamma-jet, anti-k_{T} R=0.4, |#eta| < 0.45");
  
  c_jer->SaveAs("outvsE/compare_JERvsE_conserv.pdf");
  
  return;
}

