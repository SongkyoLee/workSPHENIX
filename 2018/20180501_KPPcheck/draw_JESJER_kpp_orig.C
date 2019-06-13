void draw_JESJER_kpp(){
 
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.038);

  //TLegend* leg1 = new TLegend(0.18, 0.19, 0.40, 0.28);
  TLegend* leg1 = new TLegend(0.18, 0.19, 0.45, 0.35);
  leg1->SetTextSize(0.038);
  TLegend* leg2 = new TLegend(0.66, 0.80, 0.88, 0.91);
  leg2->SetTextSize(0.031);
  TLegend* leg3 = new TLegend(0.66, 0.68, 0.88, 0.79);
  leg3->SetTextSize(0.031);
  TLegend* leg4 = new TLegend(0.66, 0.56, 0.88, 0.67);
  leg4->SetTextSize(0.031);
  TLegend* leg5 = new TLegend(0.66, 0.19, 0.88, 0.30);
  leg5->SetTextSize(0.031);
  TLegend* leg6 = new TLegend(0.66, 0.80, 0.88, 0.91);
  leg6->SetTextSize(0.031);
  TLegend* leg7 = new TLegend(0.18, 0.19, 0.60, 0.28);
  leg7->SetTextSize(0.031);
  
  //// select hist x axis range
  double jesymin, jesymax; 
  //jesymin =0.35; jesymax = 1.65;
  jesymin =0.5; jesymax = 1.0;
  double jerymin, jerymax; 
  //jerymin =0.0; jerymax = 0.4;
  jerymin =0.0; jerymax = 0.35;
  
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
  double jesSS310Before[nEBin];
  //// Alframe
  double jesAlframeBefore[nEBin];
  //// Al
  double jesAlBefore[nEBin];
  
  //////////////////// read-in jes_SS310_ files
  string szjes_SS310;
  int ijes_SS310=0;
  ifstream infile_jesSS310Before;
  infile_jesSS310Before.open("txt_jes/jes_mean_QCDJet_R04.out");
  ijes_SS310=0;
  if (infile_jesSS310Before.is_open()){
    while( getline (infile_jesSS310Before,szjes_SS310) ) { jesSS310Before[ijes_SS310] = atof(szjes_SS310.c_str()); ijes_SS310++; }
  }
  //////////////////// read-in jes_Alframe_ files
  string szjes_Alframe;
  int ijes_Alframe=0;
  ifstream infile_jesAlframeBefore;
  infile_jesAlframeBefore.open("txt_jes/jes_mean_ObjectiveKPP_R04.out");
  ijes_Alframe=0;
  if (infile_jesAlframeBefore.is_open()){
    while( getline (infile_jesAlframeBefore,szjes_Alframe) ) { jesAlframeBefore[ijes_Alframe] = atof(szjes_Alframe.c_str()); ijes_Alframe++; }
  }
  //////////////////// read-in jes_Al_ files
  string szjes_Al;
  int ijes_Al=0;
  ifstream infile_jesAlBefore;
  infile_jesAlBefore.open("txt_jes/jes_mean_ThresholdKPP_R04.out");
  ijes_Al=0;
  if (infile_jesAlBefore.is_open()){
    while( getline (infile_jesAlBefore,szjes_Al) ) { jesAlBefore[ijes_Al] = atof(szjes_Al.c_str()); ijes_Al++; }
  }
  /////////////////////////////////////////////////
  
  TGraphAsymmErrors* gjesSS310Before = new TGraphAsymmErrors(nEBin, xPnt, jesSS310Before, ex, ex, ey, ey);
  gjesSS310Before->SetName("gjesSS310Before");
  TGraphAsymmErrors* gjesAlframeBefore = new TGraphAsymmErrors(nEBin, xPnt, jesAlframeBefore, ex, ex, ey, ey);
  gjesAlframeBefore->SetName("gjesAlframeBefore");
  TGraphAsymmErrors* gjesAlBefore = new TGraphAsymmErrors(nEBin, xPnt, jesAlBefore, ex, ex, ey, ey);
  gjesAlBefore->SetName("gjesAlBefore");

  SetGraphStyle(gjesSS310Before,1,10);
  SetGraphStyle(gjesAlframeBefore,2,11);
  SetGraphStyle(gjesAlBefore,3,12);
  
  gjesSS310Before->GetXaxis()->SetTitle("p_{T, Jet}^{Truth} [GeV]");
  gjesSS310Before->GetXaxis()->CenterTitle(1);
  gjesSS310Before->GetYaxis()->SetTitle("<E_{T, Jet}^{Reco}/ E_{T, Jet}^{Truth}>");
  gjesSS310Before->GetYaxis()->SetTitleOffset(1.5);
  gjesSS310Before->GetYaxis()->CenterTitle(1);
  gjesSS310Before->GetXaxis()->SetLimits(0,70.);
  gjesSS310Before->SetMinimum(jesymin); 
  gjesSS310Before->SetMaximum(jesymax); 
  
  gjesSS310Before->Draw("AP");
  gjesAlframeBefore->Draw("P");
  gjesAlBefore->Draw("P");
  
  leg1->AddEntry(gjesSS310Before,"Default","lp");
  leg1->AddEntry(gjesAlframeBefore,"Objective KPP","lp");
  leg1->AddEntry(gjesAlBefore,"Threshold KPP","lp");
  leg1->Draw();

//  dashedLine(0.0,1.0,70.0,1.0,1,1);
  
	latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"MIE Simulation");
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, R=0.4"); 
  
  c_JES->SaveAs("out_figs/JESvsE_kpp.pdf");


  ///////////////////////////////////////
  ///////////////////////////////////////
  //// Draw JER
  ///////////////////////////////////////
  ///////////////////////////////////////
  
  TCanvas* c_JER = new TCanvas("c_JER","",600,600);
  c_JER->cd();
  
  //// SS310 (Inner HCal)
  double jerSS310Before[nEBin];
  //// Alframe
  double jerAlframeBefore[nEBin];
  //// Al
  double jerAlBefore[nEBin];
  
  //////////////////// read-in jer_SS310_ files
  string szjer_SS310;
  int ijer_SS310=0;
  ifstream infile_jerSS310Before;
  infile_jerSS310Before.open("txt_jes/jes_sigmean_QCDJet_R04.out");
  ijer_SS310=0;
  if (infile_jerSS310Before.is_open()){
    while( getline (infile_jerSS310Before,szjer_SS310) ) { jerSS310Before[ijer_SS310] = atof(szjer_SS310.c_str()); ijer_SS310++; }
  }
  //////////////////// read-in jer_Alframe_ files
  string szjer_Alframe;
  int ijer_Alframe=0;
  ifstream infile_jerAlframeBefore;
  infile_jerAlframeBefore.open("txt_jes/jes_sigmean_ObjectiveKPP_R04.out");
  ijer_Alframe=0;
  if (infile_jerAlframeBefore.is_open()){
    while( getline (infile_jerAlframeBefore,szjer_Alframe) ) { jerAlframeBefore[ijer_Alframe] = atof(szjer_Alframe.c_str()); ijer_Alframe++; }
  }
  //////////////////// read-in jer_Al_ files
  string szjer_Al;
  int ijer_Al=0;
  ifstream infile_jerAlBefore;
  infile_jerAlBefore.open("txt_jes/jes_sigmean_ThresholdKPP_R04.out");
  ijer_Al=0;
  if (infile_jerAlBefore.is_open()){
    while( getline (infile_jerAlBefore,szjer_Al) ) { jerAlBefore[ijer_Al] = atof(szjer_Al.c_str()); ijer_Al++; }
  }
  /////////////////////////////////////////////////
  
  TGraphAsymmErrors* gjerSS310Before = new TGraphAsymmErrors(nEBin, xPnt, jerSS310Before, ex, ex, ey, ey);
  gjerSS310Before->SetName("gjerSS310Before");
  TGraphAsymmErrors* gjerAlframeBefore = new TGraphAsymmErrors(nEBin, xPnt, jerAlframeBefore, ex, ex, ey, ey);
  gjerAlframeBefore->SetName("gjerAlframeBefore");
  TGraphAsymmErrors* gjerAlBefore = new TGraphAsymmErrors(nEBin, xPnt, jerAlBefore, ex, ex, ey, ey);
  gjerAlBefore->SetName("gjerAlBefore");

  SetGraphStyle(gjerSS310Before,1,10);
  SetGraphStyle(gjerAlframeBefore,2,11);
  SetGraphStyle(gjerAlBefore,3,12);
  
  gjerSS310Before->GetXaxis()->SetTitle("p_{T, Jet}^{Truth} [GeV]");
  gjerSS310Before->GetXaxis()->CenterTitle(1);
  gjerSS310Before->GetYaxis()->SetTitle("#sigma[E_{T, Jet}^{Reco}/ E_{T, Jet}^{Truth}]/<E_{T, Jet}^{Reco}/ E_{T, Jet}^{Truth}>");
  gjerSS310Before->GetYaxis()->SetTitleOffset(1.5);
  gjerSS310Before->GetYaxis()->CenterTitle(1);
  gjerSS310Before->GetXaxis()->SetLimits(0,70.);
  gjerSS310Before->SetMinimum(jerymin); 
  gjerSS310Before->SetMaximum(jerymax); 
  
  gjerSS310Before->Draw("AP");
  gjerAlframeBefore->Draw("P");
  gjerAlBefore->Draw("P");
  
  //leg1->AddEntry(gjerSS310Before,"Default","lp");
  //leg1->AddEntry(gjerAlframeBefore,"Objective KPP","lp");
  //leg1->AddEntry(gjerAlBefore,"Threshold KPP","lp");
  leg1->Draw();

//  dashedLine(0.0,1.0,70.0,1.0,1,1);
  
	latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"MIE Simulation");
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, R=0.4"); 
  
  c_JER->SaveAs("out_figs/JERvsE_kpp.pdf");

/*
//  TF1* funcUPP = new TF1("funcUPP","[0]/TMath::Sqrt(x)",20,60);
//  funcUPP->SetParameter(0, 1.20 ); //120% for pp jet?
//  funcUPP->SetLineColor(kRed-9);
//  funcUPP->SetLineStyle(1);
//  funcUPP->Draw("same");
  TF1* func01 = new TF1("func01","[0]/TMath::Sqrt(x)",20,60);
  func01->SetName("fit_jerAlframe_2");
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
    gjerSS310Before->GetPoint(ie,tx,ty); 
    cout << tx<<" GeV : "<<ty << endl; 
  }
  cout << "AlframeBefore" <<endl;
  for (int ie=0; ie<nEBin; ie++){
    gjerAlframeBefore->GetPoint(ie,tx,ty); 
    cout << tx<<" GeV : "<<ty << endl; 
  }
  cout << "AlBefore" <<endl;
  for (int ie=0; ie<nEBin; ie++){
    gjerAlBefore->GetPoint(ie,tx,ty); 
    cout << tx<<" GeV : "<<ty << endl; 
  }
  ///////////////////////////////////////
 
  TFile* fout =  new TFile(Form("out_figs/JESvsE_%s_sigmean_fit_pt.root",inputType.c_str()),"RECREATE");
  fout->cd();
//  gjesSS310Before->Write();
//  gjesAlframeBefore->Write();
//  gjesAlBefore->Write();
//  for (int im=0; im<nMeth; im++){   
//    if (im!=2) continue;
//    gjesSS310[im]->Write();
//    gjesAlframe[im]->Write();
//    gjesAl[im]->Write();
//  } 
//  gjerSS310Before->Write();
//  gjerAlframeBefore->Write();
//  gjerAlBefore->Write();
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

