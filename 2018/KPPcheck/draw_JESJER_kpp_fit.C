void draw_JESJER_kpp_fit(string res="R04"){
 
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.038);

  TLegend* leg1 = new TLegend(0.18, 0.19, 0.45, 0.35);
  leg1->SetTextSize(0.033);
  TLegend* leg2 = new TLegend(0.18, 0.59, 0.45, 0.75);
  leg2->SetTextSize(0.038);
  //TLegend* leg7 = new TLegend(0.18, 0.19, 0.60, 0.28);
  TLegend* leg7 = new TLegend(0.50, 0.19, 0.75, 0.35);
  leg7->SetTextSize(0.033);
  
  //// select hist x axis range
  double jesymin, jesymax; 
  //jesymin =0.35; jesymax = 1.65;
  if (res.compare("R04")==0) { jesymin =0.5; jesymax = 1.0; }
  else if (res.compare("R02")==0) { jesymin =0.5; jesymax = 1.0; }
  else {cout << "Warning:: select  R04 or R02" << endl; return; }
  double jerymin, jerymax; 
  //jerymin =0.0; jerymax = 0.4;
  if (res.compare("R04")==0) { jerymin =0.0; jerymax = 0.35; }
  else if (res.compare("R02")==0) { jerymin =0.0; jerymax = 0.45; }
  else {cout << "Warning:: select  R04 or R02" << endl; return; }

  ///////////////////////////////////////
  //// Get Hist for different pT range
  const int nEBin = 4;
  cout << "nEBin = " << nEBin << endl;
  double xPnt[nEBin] = {25, 35, 45, 55};
  double ex[nEBin] = {5, 5, 5, 5};
  double ey[nEBin] = {0, 0, 0, 0};

  ///////////////////////////////////////
  ///////////////////////////////////////
  //// Draw JES
  ///////////////////////////////////////
  ///////////////////////////////////////
  
  TCanvas* c_JES = new TCanvas("c_JES","",600,600);
  c_JES->cd();
  
  //// SS310 (Inner HCal)
  double jesDefault[nEBin];
  //// Alframe
  double jesObjective[nEBin];
  //// Al
  double jesThreshold[nEBin];
  
  //////////////////// read-in jes_SS310_ files
  string szjes_SS310;
  int ijes_SS310=0;
  ifstream infile_jesDefault;
  infile_jesDefault.open(Form("txt_jes/jes_mean_QCDJet_%s.out",res.c_str()));
  ijes_SS310=0;
  if (infile_jesDefault.is_open()){
    while( getline (infile_jesDefault,szjes_SS310) ) { jesDefault[ijes_SS310] = atof(szjes_SS310.c_str()); ijes_SS310++; }
  }
  //////////////////// read-in jes_Alframe_ files
  string szjes_Alframe;
  int ijes_Alframe=0;
  ifstream infile_jesObjective;
  infile_jesObjective.open(Form("txt_jes/jes_mean_ObjectiveKPP_%s.out",res.c_str()));
  ijes_Alframe=0;
  if (infile_jesObjective.is_open()){
    while( getline (infile_jesObjective,szjes_Alframe) ) { jesObjective[ijes_Alframe] = atof(szjes_Alframe.c_str()); ijes_Alframe++; }
  }
  //////////////////// read-in jes_Al_ files
  string szjes_Al;
  int ijes_Al=0;
  ifstream infile_jesThreshold;
  infile_jesThreshold.open(Form("txt_jes/jes_mean_ThresholdKPP_%s.out",res.c_str()));
  ijes_Al=0;
  if (infile_jesThreshold.is_open()){
    while( getline (infile_jesThreshold,szjes_Al) ) { jesThreshold[ijes_Al] = atof(szjes_Al.c_str()); ijes_Al++; }
  }
  /////////////////////////////////////////////////
  
  TGraphAsymmErrors* gjesDefault = new TGraphAsymmErrors(nEBin, xPnt, jesDefault, ex, ex, ey, ey);
  gjesDefault->SetName("gjesDefault");
  TGraphAsymmErrors* gjesObjective = new TGraphAsymmErrors(nEBin, xPnt, jesObjective, ex, ex, ey, ey);
  gjesObjective->SetName("gjesObjective");
  TGraphAsymmErrors* gjesThreshold = new TGraphAsymmErrors(nEBin, xPnt, jesThreshold, ex, ex, ey, ey);
  gjesThreshold->SetName("gjesThreshold");

  SetGraphStyle(gjesDefault,1,10);
  SetGraphStyle(gjesObjective,2,11);
  SetGraphStyle(gjesThreshold,3,12);
  
  gjesDefault->GetXaxis()->SetTitle("p_{T, Jet}^{Truth} [GeV]");
  gjesDefault->GetXaxis()->CenterTitle(1);
  gjesDefault->GetYaxis()->SetTitle("<E_{T, Jet}^{Reco}/ E_{T, Jet}^{Truth}>");
  gjesDefault->GetYaxis()->SetTitleOffset(1.5);
  gjesDefault->GetYaxis()->CenterTitle(1);
  gjesDefault->GetXaxis()->SetLimits(0,70.);
  gjesDefault->SetMinimum(jesymin); 
  gjesDefault->SetMaximum(jesymax); 
  
  gjesDefault->Draw("AP");
  gjesObjective->Draw("P");
  gjesThreshold->Draw("P");
  
  leg1->AddEntry(gjesDefault,"Default","lp");
  leg1->AddEntry(gjesObjective,"Objective KPP","lp");
  leg1->AddEntry(gjesThreshold,"Threshold KPP","lp");
  leg2->AddEntry(gjesDefault,"Default","lp");
  leg2->AddEntry(gjesObjective,"Objective KPP","lp");
  leg2->AddEntry(gjesThreshold,"Threshold KPP","lp");
  if (res.compare("R04")==0) leg1->Draw();
  else leg2->Draw();

//  dashedLine(0.0,1.0,70.0,1.0,1,1);
  
	latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"MIE Simulation");
  latex->SetTextSize(0.040);
  if (res.compare("R04")==0) latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, R=0.4"); 
  else latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, R=0.2"); 
  
  c_JES->SaveAs(Form("out_figs/JESvsE_kpp_fit_%s.pdf",res.c_str()));


  ///////////////////////////////////////
  ///////////////////////////////////////
  //// Draw JER
  ///////////////////////////////////////
  ///////////////////////////////////////
  
  TCanvas* c_JER = new TCanvas("c_JER","",600,600);
  c_JER->cd();
  
  //// SS310 (Inner HCal)
  double jerDefault[nEBin];
  //// Alframe
  double jerObjective[nEBin];
  //// Al
  double jerThreshold[nEBin];
  
  //////////////////// read-in jer_SS310_ files
  string szjer_SS310;
  int ijer_SS310=0;
  ifstream infile_jerDefault;
  infile_jerDefault.open(Form("txt_jes/jes_sigmean_QCDJet_%s.out",res.c_str()));
  ijer_SS310=0;
  if (infile_jerDefault.is_open()){
    while( getline (infile_jerDefault,szjer_SS310) ) { jerDefault[ijer_SS310] = atof(szjer_SS310.c_str()); ijer_SS310++; }
  }
  //////////////////// read-in jer_Alframe_ files
  string szjer_Alframe;
  int ijer_Alframe=0;
  ifstream infile_jerObjective;
  infile_jerObjective.open(Form("txt_jes/jes_sigmean_ObjectiveKPP_%s.out",res.c_str()));
  ijer_Alframe=0;
  if (infile_jerObjective.is_open()){
    while( getline (infile_jerObjective,szjer_Alframe) ) { jerObjective[ijer_Alframe] = atof(szjer_Alframe.c_str()); ijer_Alframe++; }
  }
  //////////////////// read-in jer_Al_ files
  string szjer_Al;
  int ijer_Al=0;
  ifstream infile_jerThreshold;
  infile_jerThreshold.open(Form("txt_jes/jes_sigmean_ThresholdKPP_%s.out",res.c_str()));
  ijer_Al=0;
  if (infile_jerThreshold.is_open()){
    while( getline (infile_jerThreshold,szjer_Al) ) { jerThreshold[ijer_Al] = atof(szjer_Al.c_str()); ijer_Al++; }
  }
  /////////////////////////////////////////////////
  
  TGraphAsymmErrors* gjerDefault = new TGraphAsymmErrors(nEBin, xPnt, jerDefault, ex, ex, ey, ey);
  gjerDefault->SetName("gjerDefault");
  TGraphAsymmErrors* gjerObjective = new TGraphAsymmErrors(nEBin, xPnt, jerObjective, ex, ex, ey, ey);
  gjerObjective->SetName("gjerObjective");
  TGraphAsymmErrors* gjerThreshold = new TGraphAsymmErrors(nEBin, xPnt, jerThreshold, ex, ex, ey, ey);
  gjerThreshold->SetName("gjerThreshold");

  SetGraphStyle(gjerDefault,1,10);
  SetGraphStyle(gjerObjective,2,11);
  SetGraphStyle(gjerThreshold,3,12);
  
  gjerDefault->GetXaxis()->SetTitle("p_{T, Jet}^{Truth} [GeV]");
  gjerDefault->GetXaxis()->CenterTitle(1);
  gjerDefault->GetYaxis()->SetTitle("#sigma[E_{T, Jet}^{Reco}/ E_{T, Jet}^{Truth}]/<E_{T, Jet}^{Reco}/ E_{T, Jet}^{Truth}>");
  gjerDefault->GetYaxis()->SetTitleOffset(1.5);
  gjerDefault->GetYaxis()->CenterTitle(1);
  gjerDefault->GetXaxis()->SetLimits(0,70.);
  gjerDefault->SetMinimum(jerymin); 
  gjerDefault->SetMaximum(jerymax); 
  
  gjerDefault->Draw("AP");
  gjerObjective->Draw("P");
  gjerThreshold->Draw("P");
  
  //TF1* func01 = new TF1("func01","[0]/TMath::Sqrt(x)",20,60);
  TF1* func01 = new TF1("func01","sqrt(([0]/TMath::Sqrt(x))*([0]/TMath::Sqrt(x))+[1]*[1])",20,60);
  func01->SetName("gjerDefault");
  func01->SetParameter(0, 1.20 );
  func01->SetParameter(1, 0.1 );
  func01->SetParLimits(1, 0,1 );
  func01->SetLineColor(kRed-7);
  func01->SetLineStyle(1);
  gjerDefault->Fit(func01,"NR");
  func01->Draw("same");
  double paramDefault_1 = func01->GetParameter(0);
  double paramDefault_2 = func01->GetParameter(1);
  
  TF1* func02 = new TF1("func02","sqrt(([0]/TMath::Sqrt(x))*([0]/TMath::Sqrt(x))+[1]*[1])",20,60);
  func02->SetName("gjerObjective");
  func02->SetParameter(0, 1.20 );
  func02->SetParameter(1, 0.1 );
  func02->SetParLimits(1, 0,1 );
  func02->SetLineColor(kAzure+1);
  func02->SetLineStyle(1);
  gjerObjective->Fit(func02,"NR");
  func02->Draw("same");
  double paramObjective_1 = func02->GetParameter(0);
  double paramObjective_2 = func02->GetParameter(1);
 
  TF1* func03 = new TF1("func03","sqrt(([0]/TMath::Sqrt(x))*([0]/TMath::Sqrt(x))+[1]*[1])",20,60);
  func03->SetName("gjerThreshold");
  func03->SetParameter(0, 1.20 );
  func03->SetParameter(1, 1.20 );
  func02->SetParLimits(2, 0,1 );
  func03->SetLineColor(kGreen-6);
  func03->SetLineStyle(1);
  gjerThreshold->Fit(func03,"NR");
  func03->Draw("same");
  double paramThreshold_1 = func03->GetParameter(0);
  double paramThreshold_2 = func03->GetParameter(1);
  
  gjerDefault->Draw("P");
  gjerObjective->Draw("P");
  gjerThreshold->Draw("P");
  
  //leg1->AddEntry(gjerDefault,"Default","lp");
  //leg1->AddEntry(gjerObjective,"Objective KPP","lp");
  //leg1->AddEntry(gjerThreshold,"Threshold KPP","lp");
  leg1->Draw();
  
  leg7->AddEntry(func01,Form(" %.0f%% / #sqrt{(p_{T}[GeV])} #oplus %.0f%%",100*paramDefault_1,100*paramDefault_2),"l");
  leg7->AddEntry(func02,Form(" %.0f%% / #sqrt{(p_{T}[GeV])} #oplus %.0f%%",100*paramObjective_1,100*paramObjective_2),"l");
  leg7->AddEntry(func03,Form(" %.0f%% / #sqrt{(p_{T}[GeV])} #oplus %.0f%%",100*paramThreshold_1,100*paramThreshold_2),"l");
  leg7->Draw(); 

//  dashedLine(0.0,1.0,70.0,1.0,1,1);
  
	latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"MIE Simulation");
  latex->SetTextSize(0.040);
  if (res.compare("R04")==0) latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, R=0.4"); 
  else latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, R=0.2"); 
  
  c_JER->SaveAs(Form("out_figs/JERvsE_kpp_fit_%s.pdf",res.c_str()));
/*
//  TF1* funcUPP = new TF1("funcUPP","[0]/TMath::Sqrt(x)",20,60);
//  funcUPP->SetParameter(0, 1.20 ); //120% for pp jet?
//  funcUPP->SetLineColor(kRed-9);
//  funcUPP->SetLineStyle(1);
//  funcUPP->Draw("same");
  TF1* func01 = new TF1("func01","[0]/TMath::Sqrt(x)",20,6Alfunc01->SetName("fit_jerAlframe_2");
  TLegend* leg7 = new TLegend(0.18, 0.19, 0.60, 0.28);
  leg7->SetTextSize(0.031);
  func01->SetParameter(0, 1.20 );
  func01->SetLineColor(kRed-7);
  func01->SetLineStyle(2);
  gjerAlframe[2]->Fit(func01,"NR");
  func01->Draw("same");
  double paramBefore = func01->GetParameter(0);
  TF1* func02 = new TF1("func02","[0]/TMath::Sqrt(x)",20,60);
  func02->SetName("fit_jerAl_2");
  func02->SetParameter(0, 1.20 );
  func02->SetLineColor(kAzure+1);
  func02->SetLineStyle(9);
  gjerAl[2]->Fit(func02,"NR");
  func02->Draw("same");
  double param_2 = func02->GetParameter(0);
  
  leg6->SetHeader("After calibration");
  leg6->AddEntry(gjesAlframe[2],"Uninstrumted Al","lp");
  leg6->AddEntry(gjesAl[2],"Instrumented Al","lp");
  leg6->Draw(); 
  leg7->AddEntry(func01,Form(" %.0f%% / #sqrt{(p_{T}[GeV])}",100*paramBefore),"l");
  leg7->AddEntry(func02,Form(" %.0f%% / #sqrt{(p_{T}[GeV])}",100*param_2),"l");
  leg7->Draw(); 
 */ 
/*
  //// print out data points 
  double tx;
  double ty;
  cout << "SS310Before" <<endl;
  for (int ie=0; ie<nEBin; ie++){
    gjerDefault->GetPoint(ie,tx,ty); 
    cout << tx<<" GeV : "<<ty << endl; 
  }
  cout << "AlframeBefore" <<endl;
  for (int ie=0; ie<nEBin; ie++){
    gjerObjective->GetPoint(ie,tx,ty); 
    cout << tx<<" GeV : "<<ty << endl; 
  }
  cout << "AlBefore" <<endl;
  for (int ie=0; ie<nEBin; ie++){
    gjerThreshold->GetPoint(ie,tx,ty); 
    cout << tx<<" GeV : "<<ty << endl; 
  }
  ///////////////////////////////////////
 
  TFile* fout =  new TFile(Form("out_figs/JESvsE_%s_sigmean_fit_pt.root",inputType.c_str()),"RECREATE");
  fout->cd();
//  gjesDefault->Write();
//  gjesObjective->Write();
//  gjesThreshold->Write();
//  for (int im=0; im<nMeth; im++){   
//    if (im!=2) continue;
//    gjesSS310[im]->Write();
//    gjesAlframe[im]->Write();
//    gjesAl[im]->Write();
//  } 
//  gjerDefault->Write();
//  gjerObjective->Write();
//  gjerThreshold->Write();
  for (int im=0; im<nMeth; im++){   
    if (im!=2) continue;
//    gjerSS310[im]->Write();
    gjerAlframe[im]->Write();
    gjerAl[im]->Write();
  } 
  func01->Write();
  func02->Write();
  fout->Close();
  */
   
  return;
}

