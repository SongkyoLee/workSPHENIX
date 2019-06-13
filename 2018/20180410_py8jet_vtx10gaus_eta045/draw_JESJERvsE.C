void draw_JESJERvsE(string inputType="final_qcd"){
  
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
  //jesymin =0.35; jesymax = 1.65;
  jesymin =0.5; jesymax = 1.5;
  double jerymin, jerymax; 
  //jerymin =0.0; jerymax = 0.4;
  jerymin =0.0; jerymax = 0.35;
  
  ///////////////////////////////////////
  //// Get Hist for different pT range
  const int nEBin = 4;
  cout << "nEBin = " << nEBin << endl;
  const int nMeth = 5;
  cout << "nMeth = " << nMeth << endl;
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
  double jesSS310[nMeth][nEBin];
  //// Alframe
  double jesAlframeBefore[nEBin];
  double jesAlframe[nMeth][nEBin];
  
  //////////////////// read-in jes_SS310_ files
  string szjes_SS310;
  int ijes_SS310=0;
  ifstream infile_jesSS310Before;
  infile_jesSS310Before.open("txt_jes/jes_mean_before_SS310.out");
  ijes_SS310=0;
  if (infile_jesSS310Before.is_open()){
    while( getline (infile_jesSS310Before,szjes_SS310) ) { jesSS310Before[ijes_SS310] = atof(szjes_SS310.c_str()); ijes_SS310++; }
  }
  ifstream infile_jesSS310[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_jesSS310[im].open(Form("txt_jes/jes_mean_method0%d_SS310.out",im+1));
    ijes_SS310=0;
    if (infile_jesSS310[im].is_open()){
      while( getline (infile_jesSS310[im],szjes_SS310) ) { jesSS310[im][ijes_SS310] = atof(szjes_SS310.c_str()); ijes_SS310++; }
    }
  }
  //////////////////// read-in jes_Alframe_ files
  string szjes_Alframe;
  int ijes_Alframe=0;
  ifstream infile_jesAlframeBefore;
  infile_jesAlframeBefore.open("txt_jes/jes_mean_before_Alframe.out");
  ijes_Alframe=0;
  if (infile_jesAlframeBefore.is_open()){
    while( getline (infile_jesAlframeBefore,szjes_Alframe) ) { jesAlframeBefore[ijes_Alframe] = atof(szjes_Alframe.c_str()); ijes_Alframe++; }
  }
  ifstream infile_jesAlframe[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_jesAlframe[im].open(Form("txt_jes/jes_mean_method0%d_Alframe.out",im+1));
    ijes_Alframe=0;
    if (infile_jesAlframe[im].is_open()){
      while( getline (infile_jesAlframe[im],szjes_Alframe) ) { jesAlframe[im][ijes_Alframe] = atof(szjes_Alframe.c_str()); ijes_Alframe++; }
    }
  }

  for (int ie=0; ie<nEBin; ie++){
    cout << "jesSS310Before["<<ie<<"] = " << jesSS310Before[ie] << endl;
    for (int im=0; im<nMeth; im++){
      cout << "jesSS310[im]["<<ie<<"] = " << jesSS310[im][ie] << endl;
    }
  }
  for (int ie=0; ie<nEBin; ie++){
    cout << "jesAlframeBefore["<<ie<<"] = " << jesAlframeBefore[ie] << endl;
    for (int im=0; im<nMeth; im++){
      cout << "jesAlframe[im]["<<ie<<"] = " << jesAlframe[im][ie] << endl;
    }
  }
  
  TGraphAsymmErrors* gjesSS310Before = new TGraphAsymmErrors(nEBin, xPnt, jesSS310Before, ex, ex, ey, ey);
  gjesSS310Before->SetName("gjesSS310Before");
  TGraphAsymmErrors* gjesSS310[nMeth];
  for (int im=0; im<nMeth; im++){
    gjesSS310[im] = new TGraphAsymmErrors(nEBin, xPnt, jesSS310[im], ex, ex, ey, ey);
    gjesSS310[im]->SetName(Form("gjesSS310_%d",im));
  }
  TGraphAsymmErrors* gjesAlframeBefore = new TGraphAsymmErrors(nEBin, xPnt, jesAlframeBefore, ex, ex, ey, ey);
  gjesAlframeBefore->SetName("gjesAlframeBefore");
  TGraphAsymmErrors* gjesAlframe[nMeth];
  for (int im=0; im<nMeth; im++){
    gjesAlframe[im] = new TGraphAsymmErrors(nEBin, xPnt, jesAlframe[im], ex, ex, ey, ey);
    gjesAlframe[im]->SetName(Form("gjesAlframe_%d",im));
  }

  //SetGraphStyle(gjesSS310Before,0,0);
  //SetGraphStyle(gjesSS310After01,0,10);
  //SetGraphStyle(gjesSS310After02,0,11);
  //SetGraphStyle(gjesSS310After03,0,12);
  //SetGraphStyle(gjesSS310After04,0,13);
  //SetGraphStyle(gjesSS310After05,0,14);
  
  SetGraphStyle(gjesSS310Before,0,0);
  SetGraphStyle(gjesSS310[0],0,10);
  SetGraphStyle(gjesSS310[1],0,11);
  SetGraphStyle(gjesSS310[2],0,11);
  SetGraphStyle(gjesSS310[3],0,12);
  SetGraphStyle(gjesSS310[4],0,10);
  SetGraphStyle(gjesAlframeBefore,1,0);
  SetGraphStyle(gjesAlframe[0],1,10);
  SetGraphStyle(gjesAlframe[1],1,11);
  SetGraphStyle(gjesAlframe[2],1,11);
  SetGraphStyle(gjesAlframe[3],1,12);
  SetGraphStyle(gjesAlframe[4],1,10);
  
  gjesSS310Before->GetXaxis()->SetTitle("Jet E [GeV]");
  gjesSS310Before->GetXaxis()->CenterTitle(1);
  gjesSS310Before->GetYaxis()->SetTitle("Jet Energy Scale");
//  gjesSS310Before->GetYaxis()->SetTitleOffset(1.8);
  gjesSS310Before->GetYaxis()->CenterTitle(1);
  gjesSS310Before->GetXaxis()->SetLimits(0,70.);
  gjesSS310Before->SetMinimum(jesymin); 
  gjesSS310Before->SetMaximum(jesymax); 
  
  gjesSS310Before->Draw("AP");
  //gjesSS310[0]->Draw("P");
  //gjesSS310[1]->Draw("P");
  gjesSS310[2]->Draw("P");
  gjesSS310[3]->Draw("P");
  gjesSS310[4]->Draw("P");
  gjesAlframeBefore->Draw("P");
  //gjesAlframe[0]->Draw("P");
  //gjesAlframe[1]->Draw("P");
  gjesAlframe[2]->Draw("P");
  //gjesAlframe[3]->Draw("P");
  gjesAlframe[4]->Draw("P");
  
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
  //latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4");
  //latex->DrawLatex(0.2,0.77,"Jet |#eta^{truth}| < 0.6");
  latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4, |#eta_{Jet}^{truth}| < 0.45");
  
  c_JES->SaveAs(Form("outvsE/JESvsE_%s.pdf",inputType.c_str()));
  
  ///////////////////////////////////////
  ///////////////////////////////////////
  //// Draw JER
  ///////////////////////////////////////
  ///////////////////////////////////////
  
  TCanvas* c_JER = new TCanvas("c_JER","",600,600);
  c_JER->cd();
  
  //// SS310 (Inner HCal)
  double jerSS310Before[nEBin];
  double jerSS310[nMeth][nEBin];
  //// Alframe
  double jerAlframeBefore[nEBin];
  double jerAlframe[nMeth][nEBin];
  
  //////////////////// read-in jer_SS310_ files
  string szjer_SS310;
  int ijer_SS310=0;
  ifstream infile_jerSS310Before;
  infile_jerSS310Before.open("txt_jes/jer_sig_before_SS310.out");
  ijer_SS310=0;
  if (infile_jerSS310Before.is_open()){
    while( getline (infile_jerSS310Before,szjer_SS310) ) { jerSS310Before[ijer_SS310] = atof(szjer_SS310.c_str()); ijer_SS310++; }
  }
  ifstream infile_jerSS310[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_jerSS310[im].open(Form("txt_jes/jer_sig_method0%d_SS310.out",im+1));
    ijer_SS310=0;
    if (infile_jerSS310[im].is_open()){
      while( getline (infile_jerSS310[im],szjer_SS310) ) { jerSS310[im][ijer_SS310] = atof(szjer_SS310.c_str()); ijer_SS310++; }
    }
  }
  //////////////////// read-in jer_Alframe_ files
  string szjer_Alframe;
  int ijer_Alframe=0;
  ifstream infile_jerAlframeBefore;
  infile_jerAlframeBefore.open("txt_jes/jer_sig_before_Alframe.out");
  ijer_Alframe=0;
  if (infile_jerAlframeBefore.is_open()){
    while( getline (infile_jerAlframeBefore,szjer_Alframe) ) { jerAlframeBefore[ijer_Alframe] = atof(szjer_Alframe.c_str()); ijer_Alframe++; }
  }
  ifstream infile_jerAlframe[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_jerAlframe[im].open(Form("txt_jes/jer_sig_method0%d_Alframe.out",im+1));
    ijer_Alframe=0;
    if (infile_jerAlframe[im].is_open()){
      while( getline (infile_jerAlframe[im],szjer_Alframe) ) { jerAlframe[im][ijer_Alframe] = atof(szjer_Alframe.c_str()); ijer_Alframe++; }
    }
  }

  for (int ie=0; ie<nEBin; ie++){
    cout << "jerSS310Before["<<ie<<"] = " << jerSS310Before[ie] << endl;
    for (int im=0; im<nMeth; im++){
      cout << "jerSS310[im]["<<ie<<"] = " << jerSS310[im][ie] << endl;
    }
  }
  for (int ie=0; ie<nEBin; ie++){
    cout << "jerAlframeBefore["<<ie<<"] = " << jerAlframeBefore[ie] << endl;
    for (int im=0; im<nMeth; im++){
      cout << "jerAlframe[im]["<<ie<<"] = " << jerAlframe[im][ie] << endl;
    }
  }
  
  TGraphAsymmErrors* gjerSS310Before = new TGraphAsymmErrors(nEBin, xPnt, jerSS310Before, ex, ex, ey, ey);
  gjerSS310Before->SetName("gjerSS310Before");
  TGraphAsymmErrors* gjerSS310[nMeth];
  for (int im=0; im<nMeth; im++){
    gjerSS310[im] = new TGraphAsymmErrors(nEBin, xPnt, jerSS310[im], ex, ex, ey, ey);
    gjerSS310[im]->SetName(Form("gjerSS310_%d",im));
  }
  TGraphAsymmErrors* gjerAlframeBefore = new TGraphAsymmErrors(nEBin, xPnt, jerAlframeBefore, ex, ex, ey, ey);
  gjerAlframeBefore->SetName("gjerAlframeBefore");
  TGraphAsymmErrors* gjerAlframe[nMeth];
  for (int im=0; im<nMeth; im++){
    gjerAlframe[im] = new TGraphAsymmErrors(nEBin, xPnt, jerAlframe[im], ex, ex, ey, ey);
    gjerAlframe[im]->SetName(Form("gjerAlframe_%d",im));
  }

  //SetGraphStyle(gjerSS310Before,0,0);
  //SetGraphStyle(gjerSS310After01,0,10);
  //SetGraphStyle(gjerSS310After02,0,11);
  //SetGraphStyle(gjerSS310After03,0,12);
  //SetGraphStyle(gjerSS310After04,0,13);
  //SetGraphStyle(gjerSS310After05,0,14);
  
  SetGraphStyle(gjerSS310Before,0,0);
  SetGraphStyle(gjerSS310[0],0,10);
  SetGraphStyle(gjerSS310[1],0,11);
  SetGraphStyle(gjerSS310[2],0,11);
  SetGraphStyle(gjerSS310[3],0,12);
  SetGraphStyle(gjerSS310[4],0,10);
  SetGraphStyle(gjerAlframeBefore,1,0);
  SetGraphStyle(gjerAlframe[0],1,10);
  SetGraphStyle(gjerAlframe[1],1,11);
  SetGraphStyle(gjerAlframe[2],1,11);
  SetGraphStyle(gjerAlframe[3],1,12);
  SetGraphStyle(gjerAlframe[4],1,10);
  
  gjerSS310Before->GetXaxis()->SetTitle("Jet E [GeV]");
  gjerSS310Before->GetXaxis()->CenterTitle(1);
  gjerSS310Before->GetYaxis()->SetTitle("Jet Energy Scale");
//  gjerSS310Before->GetYaxis()->SetTitleOffset(1.8);
  gjerSS310Before->GetYaxis()->CenterTitle(1);
  gjerSS310Before->GetXaxis()->SetLimits(0,70.);
  gjerSS310Before->SetMinimum(jerymin); 
  gjerSS310Before->SetMaximum(jerymax); 
  
  gjerSS310Before->Draw("AP");
  //gjerSS310[0]->Draw("P");
  //gjerSS310[1]->Draw("P");
  gjerSS310[2]->Draw("P");
  gjerSS310[3]->Draw("P");
  gjerSS310[4]->Draw("P");
  gjerAlframeBefore->Draw("P");
  //gjerAlframe[0]->Draw("P");
  //gjerAlframe[1]->Draw("P");
  gjerAlframe[2]->Draw("P");
  //gjerAlframe[3]->Draw("P");
  gjerAlframe[4]->Draw("P");
  
  //leg1->AddEntry(gjerSS310Before,"SS310","lp");
  //leg1->AddEntry(gjerAlframeBefore,"Al frame","lp");
  leg1->Draw();

  dashedLine(0.0,1.0,70.0,1.0,1,1);
  
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->SetTextSize(0.040);
  //latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4");
  //latex->DrawLatex(0.2,0.77,"Jet |#eta^{truth}| < 0.6");
  latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4, |#eta_{Jet}^{truth}| < 0.45");
  
  c_JER->SaveAs(Form("outvsE/JERvsE_%s.pdf",inputType.c_str()));
 
  ///////////////////////////////////////
 
  
  TFile* fout =  new TFile(Form("outvsE/JESvsE_%s.root",inputType.c_str()),"RECREATE");
  fout->cd();
  gjesSS310Before->Write();
  gjesAlframeBefore->Write();
  for (int im=0; im<nMeth; im++){   
    gjesSS310[im]->Write();
    gjesAlframe[im]->Write();
  } 
  gjerSS310Before->Write();
  gjerAlframeBefore->Write();
  for (int im=0; im<nMeth; im++){   
    gjerSS310[im]->Write();
    gjerAlframe[im]->Write();
  } 
  fout->Close();
   
  return;
}

