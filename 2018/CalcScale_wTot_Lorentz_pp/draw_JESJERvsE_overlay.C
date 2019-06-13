void draw_JESJERvsE_overlay(string note="R02"){
  
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.038);

  //TLegend* leg1 = new TLegend(0.20, 0.21, 0.50, 0.35);
  TLegend* leg1 = new TLegend(0.18, 0.20, 0.40, 0.30);
  //leg1->SetTextSize(0.045);
  leg1->SetTextSize(0.035);
  TLegend* leg2 = new TLegend(0.18, 0.20, 0.40, 0.30);
  leg2->SetTextSize(0.045);
  
  //// select hist x axis range
  double jesymin, jesymax; 
  //jesymin =0.5; jesymax = 1.5;
  jesymin =0.4; jesymax = 1.5;
  double jerymin, jerymax; 
  //jerymin =0.0; jerymax = 0.35;
  jerymin =0.0; jerymax = 0.4;
  
  ///////////////////////////////////////
  //// Get Hist for different pT range
  const int nEBin = 4;
  cout << "nEBin = " << nEBin << endl;
  double xPnt[nEBin] = {25, 35, 45, 55};
  double ex[nEBin] = {5, 5, 5, 5};
  double ey[nEBin] = {0, 0, 0, 0};
  
  const int nMeth = 1;
  cout << "nMeth = " << nMeth << endl;
  string szMeth[nMeth];
  szMeth[0]= "truthjet";

  ///////////////////////////////////////
  ///////////////////////////////////////
  //// Draw JES
  ///////////////////////////////////////
  ///////////////////////////////////////
  
  TCanvas* c_JES = new TCanvas("c_JES","",600,600);
  c_JES->cd();
  
  //// SS310 (Inner HCal)
  double jesSS310Before[nEBin];
  double jesSS310After[nMeth][nEBin];
  //// Al
  double jesAlBefore[nEBin];
  double jesAlAfter[nMeth][nEBin];
  //// Alframe
  double jesAlframeBefore[nEBin];
  double jesAlframeAfter[nMeth][nEBin];
  
  //////////////////// read-in jes_files SS310
  string szjes_SS310;
  int ijes_SS310=0;
  ifstream infile_jesSS310Before;
  infile_jesSS310Before.open(Form("txt_jet/jes_before_SS310_%s.out",note.c_str()));
  ijes_SS310=0;
  if (infile_jesSS310Before.is_open()){
    while( getline (infile_jesSS310Before,szjes_SS310) ) { jesSS310Before[ijes_SS310] = atof(szjes_SS310.c_str()); ijes_SS310++; }
  }
  ifstream infile_jesSS310After[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_jesSS310After[im].open(Form("txt_jet/jes_%s_SS310_%s.out",szMeth[im].c_str(),note.c_str()));
    ijes_SS310=0;
    if (infile_jesSS310After[im].is_open()){
      while( getline (infile_jesSS310After[im],szjes_SS310) ) { jesSS310After[im][ijes_SS310] = atof(szjes_SS310.c_str()); ijes_SS310++; }
    }
  }
  //////////////////// read-in jes_files Al
  string szjes_Al;
  int ijes_Al=0;
  ifstream infile_jesAlBefore;
  infile_jesAlBefore.open(Form("txt_jet/jes_before_Al_%s.out",note.c_str()));
  ijes_Al=0;
  if (infile_jesAlBefore.is_open()){
    while( getline (infile_jesAlBefore,szjes_Al) ) { jesAlBefore[ijes_Al] = atof(szjes_Al.c_str()); ijes_Al++; }
  }
  ifstream infile_jesAlAfter[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_jesAlAfter[im].open(Form("txt_jet/jes_%s_Al_%s.out",szMeth[im].c_str(),note.c_str()));
    ijes_Al=0;
    if (infile_jesAlAfter[im].is_open()){
      while( getline (infile_jesAlAfter[im],szjes_Al) ) { jesAlAfter[im][ijes_Al] = atof(szjes_Al.c_str()); ijes_Al++; }
    }
  }
  //////////////////// read-in jes_files Alframe
  string szjes_Alframe;
  int ijes_Alframe=0;
  ifstream infile_jesAlframeBefore;
  infile_jesAlframeBefore.open(Form("txt_jet/jes_before_Alframe_%s.out",note.c_str()));
  ijes_Alframe=0;
  if (infile_jesAlframeBefore.is_open()){
    while( getline (infile_jesAlframeBefore,szjes_Alframe) ) { jesAlframeBefore[ijes_Alframe] = atof(szjes_Alframe.c_str()); ijes_Alframe++; }
  }
  ifstream infile_jesAlframeAfter[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_jesAlframeAfter[im].open(Form("txt_jet/jes_%s_Alframe_%s.out",szMeth[im].c_str(),note.c_str()));
    ijes_Alframe=0;
    if (infile_jesAlframeAfter[im].is_open()){
      while( getline (infile_jesAlframeAfter[im],szjes_Alframe) ) { jesAlframeAfter[im][ijes_Alframe] = atof(szjes_Alframe.c_str()); ijes_Alframe++; }
    }
  }

  ////// check
  for (int ie=0; ie<nEBin; ie++){
    cout << "jesSS310Before["<<ie<<"] = " << jesSS310Before[ie] << endl;
    for (int im=0; im<nMeth; im++){
      cout << "jesSS310After[im]["<<ie<<"] = " << jesSS310After[im][ie] << endl;
    }
  }
  for (int ie=0; ie<nEBin; ie++){
    cout << "jesAlBefore["<<ie<<"] = " << jesAlBefore[ie] << endl;
    for (int im=0; im<nMeth; im++){
      cout << "jesAlAfter[im]["<<ie<<"] = " << jesAlAfter[im][ie] << endl;
    }
  }
  for (int ie=0; ie<nEBin; ie++){
    cout << "jesAlframeBefore["<<ie<<"] = " << jesAlframeBefore[ie] << endl;
    for (int im=0; im<nMeth; im++){
      cout << "jesAlframeAfter[im]["<<ie<<"] = " << jesAlframeAfter[im][ie] << endl;
    }
  }
  
  TGraphAsymmErrors* gjesSS310Before = new TGraphAsymmErrors(nEBin, xPnt, jesSS310Before, ex, ex, ey, ey);
  gjesSS310Before->SetName("gjesSS310Before");
  TGraphAsymmErrors* gjesSS310After[nMeth];
  for (int im=0; im<nMeth; im++){
    gjesSS310After[im] = new TGraphAsymmErrors(nEBin, xPnt, jesSS310After[im], ex, ex, ey, ey);
    gjesSS310After[im]->SetName(Form("gjesSS310_%d",im));
  }
  TGraphAsymmErrors* gjesAlBefore = new TGraphAsymmErrors(nEBin, xPnt, jesAlBefore, ex, ex, ey, ey);
  gjesAlBefore->SetName("gjesAlBefore");
  TGraphAsymmErrors* gjesAlAfter[nMeth];
  for (int im=0; im<nMeth; im++){
    gjesAlAfter[im] = new TGraphAsymmErrors(nEBin, xPnt, jesAlAfter[im], ex, ex, ey, ey);
    gjesAlAfter[im]->SetName(Form("gjesAl_%d",im));
  }
  TGraphAsymmErrors* gjesAlframeBefore = new TGraphAsymmErrors(nEBin, xPnt, jesAlframeBefore, ex, ex, ey, ey);
  gjesAlframeBefore->SetName("gjesAlframeBefore");
  TGraphAsymmErrors* gjesAlframeAfter[nMeth];
  for (int im=0; im<nMeth; im++){
    gjesAlframeAfter[im] = new TGraphAsymmErrors(nEBin, xPnt, jesAlframeAfter[im], ex, ex, ey, ey);
    gjesAlframeAfter[im]->SetName(Form("gjesAlframe_%d",im));
  }

  SetSoftGraphStyle(gjesSS310Before,1,0);
  SetSoftGraphStyle(gjesAlBefore,2,0);
  SetSoftGraphStyle(gjesAlframeBefore,3,0);
  SetSoftGraphStyle(gjesSS310After[0],1,10);
  SetSoftGraphStyle(gjesAlAfter[0],2,10);
  SetSoftGraphStyle(gjesAlframeAfter[0],3,10);
  
  //gjesSS310Before->GetXaxis()->SetTitle("E_{Jet}^{Truth} [GeV]");
  gjesSS310Before->GetXaxis()->SetTitle("p_{T,Jet}^{Truth} [GeV]");
  gjesSS310Before->GetXaxis()->CenterTitle(1);
  gjesSS310Before->GetYaxis()->SetTitle("<E_{Jet}^{Reco}/E_{Jet}^{Truth}>");
//  gjesSS310Before->GetYaxis()->SetTitleOffset(1.8);
  gjesSS310Before->GetYaxis()->CenterTitle(1);
  gjesSS310Before->GetXaxis()->SetLimits(0,70.);
  gjesSS310Before->SetMinimum(jesymin); 
  gjesSS310Before->SetMaximum(jesymax); 
/*  
  //gjesAlBefore->GetXaxis()->SetTitle("E_{Jet}^{Truth} [GeV]");
  gjesAlBefore->GetXaxis()->SetTitle("p_{T,Jet}^{Truth} [GeV]");
  gjesAlBefore->GetXaxis()->CenterTitle(1);
  gjesAlBefore->GetYaxis()->SetTitle("<E_{Jet}^{Reco}/E_{Jet}^{Truth}>");
//  gjesAlBefore->GetYaxis()->SetTitleOffset(1.8);
  gjesAlBefore->GetYaxis()->CenterTitle(1);
  gjesAlBefore->GetXaxis()->SetLimits(0,70.);
  gjesAlBefore->SetMinimum(jesymin); 
  gjesAlBefore->SetMaximum(jesymax); 
*/

  gjesSS310Before->Draw("AP");
  gjesAlBefore->Draw("P");
  gjesAlframeBefore->Draw("P");
  gjesSS310After[0]->Draw("P");
  gjesAlAfter[0]->Draw("P");
  gjesAlframeAfter[0]->Draw("P");
  
  leg1->AddEntry(gjesSS310Before,"Instrumented Steel","lp");
  leg1->AddEntry(gjesAlBefore,"Instrumented Aluminum","lp");
  leg1->AddEntry(gjesAlframeBefore,"Uninstrumented Aluminum","lp");
  leg1->Draw();

  dashedLine(0.0,1.0,70.0,1.0,1,1);
  
  latex->SetTextColor(kBlack);
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->SetTextSize(0.040);
  if (note.compare("R02")==0) { latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.2, |#eta| < 0.45"); }
  else if (note.compare("R04")==0) { latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4, |#eta| < 0.45"); }
  else { cout << "WARNING!! R02 or R04"<<endl; return; };
  //latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4, |#eta_{Jet}^{truth}| < 0.45");

  c_JES->SaveAs(Form("outvsE/JESvsE_%s.pdf",note.c_str()));
  ///////////////////////////////////////
  
  ///////////////////////////////////////
  ///////////////////////////////////////
  //// Draw JER
  ///////////////////////////////////////
  ///////////////////////////////////////
  
  TCanvas* c_JER = new TCanvas("c_JER","",600,600);
  c_JER->cd();
  
  //// SS310 (Inner HCal)
  double jerSS310Before[nEBin];
  double jerSS310After[nMeth][nEBin];
  //// Al
  double jerAlBefore[nEBin];
  double jerAlAfter[nMeth][nEBin];
  //// Alframe
  double jerAlframeBefore[nEBin];
  double jerAlframeAfter[nMeth][nEBin];
  
  //////////////////// read-in jer_files SS310
  string szjer_SS310;
  int ijer_SS310=0;
  ifstream infile_jerSS310Before;
  infile_jerSS310Before.open(Form("txt_jet/jer_before_SS310_%s.out",note.c_str()));
  ijer_SS310=0;
  if (infile_jerSS310Before.is_open()){
    while( getline (infile_jerSS310Before,szjer_SS310) ) { jerSS310Before[ijer_SS310] = atof(szjer_SS310.c_str()); ijer_SS310++; }
  }
  ifstream infile_jerSS310After[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_jerSS310After[im].open(Form("txt_jet/jer_%s_SS310_%s.out",szMeth[im].c_str(),note.c_str()));
    ijer_SS310=0;
    if (infile_jerSS310After[im].is_open()){
      while( getline (infile_jerSS310After[im],szjer_SS310) ) { jerSS310After[im][ijer_SS310] = atof(szjer_SS310.c_str()); ijer_SS310++; }
    }
  }
  //////////////////// read-in jer_files Al
  string szjer_Al;
  int ijer_Al=0;
  ifstream infile_jerAlBefore;
  infile_jerAlBefore.open(Form("txt_jet/jer_before_Al_%s.out",note.c_str()));
  ijer_Al=0;
  if (infile_jerAlBefore.is_open()){
    while( getline (infile_jerAlBefore,szjer_Al) ) { jerAlBefore[ijer_Al] = atof(szjer_Al.c_str()); ijer_Al++; }
  }
  ifstream infile_jerAlAfter[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_jerAlAfter[im].open(Form("txt_jet/jer_%s_Al_%s.out",szMeth[im].c_str(),note.c_str()));
    ijer_Al=0;
    if (infile_jerAlAfter[im].is_open()){
      while( getline (infile_jerAlAfter[im],szjer_Al) ) { jerAlAfter[im][ijer_Al] = atof(szjer_Al.c_str()); ijer_Al++; }
    }
  }
  //////////////////// read-in jer_files Alframe
  string szjer_Alframe;
  int ijer_Alframe=0;
  ifstream infile_jerAlframeBefore;
  infile_jerAlframeBefore.open(Form("txt_jet/jer_before_Alframe_%s.out",note.c_str()));
  ijer_Alframe=0;
  if (infile_jerAlframeBefore.is_open()){
    while( getline (infile_jerAlframeBefore,szjer_Alframe) ) { jerAlframeBefore[ijer_Alframe] = atof(szjer_Alframe.c_str()); ijer_Alframe++; }
  }
  ifstream infile_jerAlframeAfter[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_jerAlframeAfter[im].open(Form("txt_jet/jer_%s_Alframe_%s.out",szMeth[im].c_str(),note.c_str()));
    ijer_Alframe=0;
    if (infile_jerAlframeAfter[im].is_open()){
      while( getline (infile_jerAlframeAfter[im],szjer_Alframe) ) { jerAlframeAfter[im][ijer_Alframe] = atof(szjer_Alframe.c_str()); ijer_Alframe++; }
    }
  }

  ////// check
  for (int ie=0; ie<nEBin; ie++){
    cout << "jerSS310Before["<<ie<<"] = " << jerSS310Before[ie] << endl;
    for (int im=0; im<nMeth; im++){
      cout << "jerSS310After[im]["<<ie<<"] = " << jerSS310After[im][ie] << endl;
    }
  }
  for (int ie=0; ie<nEBin; ie++){
    cout << "jerAlBefore["<<ie<<"] = " << jerAlBefore[ie] << endl;
    for (int im=0; im<nMeth; im++){
      cout << "jerAlAfter[im]["<<ie<<"] = " << jerAlAfter[im][ie] << endl;
    }
  }
  for (int ie=0; ie<nEBin; ie++){
    cout << "jerAlframeBefore["<<ie<<"] = " << jerAlframeBefore[ie] << endl;
    for (int im=0; im<nMeth; im++){
      cout << "jerAlframeAfter[im]["<<ie<<"] = " << jerAlframeAfter[im][ie] << endl;
    }
  }
  
  TGraphAsymmErrors* gjerSS310Before = new TGraphAsymmErrors(nEBin, xPnt, jerSS310Before, ex, ex, ey, ey);
  gjerSS310Before->SetName("gjerSS310Before");
  TGraphAsymmErrors* gjerSS310After[nMeth];
  for (int im=0; im<nMeth; im++){
    gjerSS310After[im] = new TGraphAsymmErrors(nEBin, xPnt, jerSS310After[im], ex, ex, ey, ey);
    gjerSS310After[im]->SetName(Form("gjerSS310_%d",im));
  }
  TGraphAsymmErrors* gjerAlBefore = new TGraphAsymmErrors(nEBin, xPnt, jerAlBefore, ex, ex, ey, ey);
  gjerAlBefore->SetName("gjerAlBefore");
  TGraphAsymmErrors* gjerAlAfter[nMeth];
  for (int im=0; im<nMeth; im++){
    gjerAlAfter[im] = new TGraphAsymmErrors(nEBin, xPnt, jerAlAfter[im], ex, ex, ey, ey);
    gjerAlAfter[im]->SetName(Form("gjerAl_%d",im));
  }
  TGraphAsymmErrors* gjerAlframeBefore = new TGraphAsymmErrors(nEBin, xPnt, jerAlframeBefore, ex, ex, ey, ey);
  gjerAlframeBefore->SetName("gjerAlframeBefore");
  TGraphAsymmErrors* gjerAlframeAfter[nMeth];
  for (int im=0; im<nMeth; im++){
    gjerAlframeAfter[im] = new TGraphAsymmErrors(nEBin, xPnt, jerAlframeAfter[im], ex, ex, ey, ey);
    gjerAlframeAfter[im]->SetName(Form("gjerAlframe_%d",im));
  }

  SetSoftGraphStyle(gjerSS310Before,1,0);
  SetSoftGraphStyle(gjerAlBefore,2,0);
  SetSoftGraphStyle(gjerAlframeBefore,3,0);
  SetSoftGraphStyle(gjerSS310After[0],1,10);
  SetSoftGraphStyle(gjerAlAfter[0],2,10);
  SetSoftGraphStyle(gjerAlframeAfter[0],3,10);
  
  //gjerSS310Before->GetXaxis()->SetTitle("E_{Jet}^{Truth} [GeV]");
  gjerSS310Before->GetXaxis()->SetTitle("p_{T,Jet}^{Truth} [GeV]");
  gjerSS310Before->GetXaxis()->CenterTitle(1);
  gjerSS310Before->GetYaxis()->SetTitle("#sigma[E_{Jet}^{Reco}/E_{Jet}^{Truth}] / <E_{Jet}^{Reco}/E_{Jet}^{Truth}>");
//  gjerSS310Before->GetYaxis()->SetTitleOffset(1.8);
  gjerSS310Before->GetYaxis()->CenterTitle(1);
  gjerSS310Before->GetXaxis()->SetLimits(0,70.);
  gjerSS310Before->SetMinimum(jerymin); 
  gjerSS310Before->SetMaximum(jerymax); 
/*  
  //gjerAlBefore->GetXaxis()->SetTitle("E_{Jet}^{Truth} [GeV]");
  gjerAlBefore->GetXaxis()->SetTitle("p_{T,Jet}^{Truth} [GeV]");
  gjerAlBefore->GetXaxis()->CenterTitle(1);
  gjerAlBefore->GetYaxis()->SetTitle("#sigma[E_{Jet}^{Reco}/E_{Jet}^{Truth}] / <E_{Jet}^{Reco}/E_{Jet}^{Truth}>");
//  gjerAlBefore->GetYaxis()->SetTitleOffset(1.8);
  gjerAlBefore->GetYaxis()->CenterTitle(1);
  gjerAlBefore->GetXaxis()->SetLimits(0,70.);
  gjerAlBefore->SetMinimum(jerymin); 
  gjerAlBefore->SetMaximum(jerymax); 
*/

  gjerSS310Before->Draw("AP");
  gjerAlBefore->Draw("P");
  gjerAlframeBefore->Draw("P");
  gjerSS310After[0]->Draw("P");
  gjerAlAfter[0]->Draw("P");
  gjerAlframeAfter[0]->Draw("P");
  
  //leg1->AddEntry(gjerSS310Before,"Instrumented Steel","lp");
  //leg1->AddEntry(gjerAlBefore,"Instrumented Aluminum","lp");
  //leg1->AddEntry(gjerAlframeBefore,"Uninstrumented Aluminum","lp");
  leg1->Draw();

  dashedLine(0.0,1.0,70.0,1.0,1,1);
  
  latex->SetTextColor(kBlack);
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->SetTextSize(0.040);
  if (note.compare("R02")==0) { latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.2, |#eta| < 0.45"); }
  else if (note.compare("R04")==0) { latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4, |#eta| < 0.45"); }
  else { cout << "WARNING!! R02 or R04"<<endl; return; };
  //latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4, |#eta_{Jet}^{truth}| < 0.45");

  c_JER->SaveAs(Form("outvsE/JERvsE_%s.pdf",note.c_str()));
  ///////////////////////////////////////
  
  ///////////////////////////////////////
  
  TFile* fout =  new TFile(Form("outvsE/JESvsE_%s.root",note.c_str()),"RECREATE");
  fout->cd();
  gjesSS310Before->Write();
  gjesAlBefore->Write();
  gjesAlframeBefore->Write();
  for (int im=0; im<nMeth; im++){   
    gjesSS310After[im]->Write();
    gjesAlAfter[im]->Write();
    gjesAlframeAfter[im]->Write();
  } 
  gjerSS310Before->Write();
  gjerAlframeBefore->Write();
  for (int im=0; im<nMeth; im++){   
    gjerSS310After[im]->Write();
    gjerAlAfter[im]->Write();
    gjerAlframeAfter[im]->Write();
  } 
  fout->Close();
   
  return;
}

