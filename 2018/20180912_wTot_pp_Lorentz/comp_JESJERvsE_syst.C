void comp_JESJERvsE_syst(string ihcalType="Al", string note="R02"){
  
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
  double ex[nEBin] = {0, 0, 0, 0};
  double exSys[nEBin] = {5, 5, 5, 5};
  double ey[nEBin] = {0, 0, 0, 0};
  
  //const int nMeth = 1;
  //cout << "nMeth = " << nMeth << endl;
  //string szMeth[nMeth];
  //szMeth[0]= "truthjet";
  const int nMeth = 9;
  cout << "nMeth = " << nMeth << endl;
  string szMeth[nMeth];
  //szMeth[0]= "truthjet";
  //szMeth[1]= "truthjet_2par";
  szMeth[0]= "truthjet";
  szMeth[1]= "truthjet";
  szMeth[2]= "truthjet";
  for (int im=0; im<nMeth; im++){
    szMeth[im] = "truthjet";
  }

  ///////////////////////////////////////
  ///////////////////////////////////////
  //// Draw JES
  ///////////////////////////////////////
  ///////////////////////////////////////
  
  TCanvas* c_JES = new TCanvas("c_JES","",600,600);
  c_JES->cd();
  
  double jesBefore[nEBin];
  double jesAfter[nMeth][nEBin];
  
  //////////////////// read-in jes_files
  string szjes;
  int ijes=0;
  ifstream infile_jesBefore;
  infile_jesBefore.open(Form("txt_jet/jes_before_%s_%s.out",ihcalType.c_str(),note.c_str()));
  ijes=0;
  if (infile_jesBefore.is_open()){
    while( getline (infile_jesBefore,szjes) ) { jesBefore[ijes] = atof(szjes.c_str()); ijes++; }
  }
  
  ifstream infile_jesAfter[nMeth];
  infile_jesAfter[0].open(Form("txt_jet/jes_%s_%s_%s.out",szMeth[0].c_str(),ihcalType.c_str(),note.c_str()));
  ijes=0;
  if (infile_jesAfter[0].is_open()){
    while( getline (infile_jesAfter[0],szjes) ) { jesAfter[0][ijes] = atof(szjes.c_str()); ijes++; }
  }
  //infile_jesAfter[1].open(Form("txt_jet_systup/jes_%s_%s_%s.out",szMeth[1].c_str(),ihcalType.c_str(),note.c_str()));
  //ijes=0;
  //if (infile_jesAfter[1].is_open()){
  //  while( getline (infile_jesAfter[1],szjes) ) { jesAfter[1][ijes] = atof(szjes.c_str()); ijes++; }
  //}
  //infile_jesAfter[2].open(Form("txt_jet_systdown/jes_%s_%s_%s.out",szMeth[2].c_str(),ihcalType.c_str(),note.c_str()));
  //ijes=0;
  //if (infile_jesAfter[2].is_open()){
  //  while( getline (infile_jesAfter[2],szjes) ) { jesAfter[2][ijes] = atof(szjes.c_str()); ijes++; }
  //}
  for (int im=0; im<nMeth; im++){
    cout << "txt_jet_syst0"<<im+1<<endl;
    infile_jesAfter[im+1].open(Form("txt_jet_syst0%d/jes_%s_%s_%s.out",im+1,szMeth[im+1].c_str(),ihcalType.c_str(),note.c_str()));
    ijes=0;
    if (infile_jesAfter[im+1].is_open()){
      while( getline (infile_jesAfter[im+1],szjes) ) { jesAfter[im+1][ijes] = atof(szjes.c_str()); ijes++; }
    }
  }

  ////// check
  for (int ie=0; ie<nEBin; ie++){
    cout << "jesBefore["<<ie<<"] = " << jesBefore[ie] << endl;
    for (int im=0; im<nMeth; im++){
      cout << "jesAfter["<<im<<"]["<<ie<<"] = " << jesAfter[im][ie] << endl;
    }
  }
  
  TGraphAsymmErrors* gjesBefore = new TGraphAsymmErrors(nEBin, xPnt, jesBefore, exSys, exSys, ey, ey);
  gjesBefore->SetName("gjesBefore");
  TGraphAsymmErrors* gjesAfter[nMeth];
  for (int im=0; im<nMeth; im++){
    gjesAfter[im] = new TGraphAsymmErrors(nEBin, xPnt, jesAfter[im], ex, ex, ey, ey);
    gjesAfter[im]->SetName(Form("gjesAfter_%d",im));
  }

  SetSoftGraphStyle(gjesBefore,2,10);
  SetSoftGraphStyle(gjesAfter[0],2,0);
  SetSoftGraphStyle(gjesAfter[1],3,11);
  SetSoftGraphStyle(gjesAfter[2],3,12);
  SetSoftGraphStyle(gjesAfter[3],3,13);
  SetSoftGraphStyle(gjesAfter[4],3,14);
  SetSoftGraphStyle(gjesAfter[5],3,11);
  SetSoftGraphStyle(gjesAfter[6],3,12);
  SetSoftGraphStyle(gjesAfter[7],3,13);
  SetSoftGraphStyle(gjesAfter[8],3,14);
  
  //gjesBefore->GetXaxis()->SetTitle("E_{Jet}^{Truth} [GeV]");
  gjesBefore->GetXaxis()->SetTitle("p_{T,Jet}^{Truth} [GeV]");
  gjesBefore->GetXaxis()->CenterTitle(1);
  gjesBefore->GetYaxis()->SetTitle("<E_{Jet}^{Reco}/E_{Jet}^{Truth}>");
  gjesBefore->GetYaxis()->CenterTitle(1);
  gjesBefore->GetXaxis()->SetLimits(0,70.);
  gjesBefore->SetMinimum(jesymin); 
  gjesBefore->SetMaximum(jesymax); 

  gjesBefore->Draw("AP");
  for (int im=0; im<nMeth; im++){
    gjesAfter[im]->Draw("P");
  }
  
  leg1->AddEntry(gjesBefore,"Before calib.","lp");
  leg1->AddEntry(gjesAfter[0],"After calib.","lp");
  leg1->AddEntry(gjesAfter[1],"For Systematics","lp");
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
  if (ihcalType.compare("SS310")==0) { latex->DrawLatex(0.2,0.77,"Instrumented Steel");  }
  else if (ihcalType.compare("Al")==0) { latex->DrawLatex(0.2,0.77,"Instrumented Aluminum");  }
  else if (ihcalType.compare("Alframe")==0) { latex->DrawLatex(0.2,0.77,"Uninstrumented Aluminum");  }
  else { cout << "WARNING!! Al, Alframe, or SS310"<<endl; return; };
  
  c_JES->SaveAs(Form("outvsE_Syst/comp_JESvsE_%s_%s.pdf",ihcalType.c_str(),note.c_str()));
  ///////////////////////////////////////

  ///////////////////////////////////////
  ///////////////////////////////////////
  //// Draw JER
  ///////////////////////////////////////
  ///////////////////////////////////////
  
  TCanvas* c_JER = new TCanvas("c_JER","",600,600);
  c_JER->cd();
  
  double jerBefore[nEBin];
  double jerAfter[nMeth][nEBin];
  
  //////////////////// read-in jer_files
  string szjer;
  int ijer=0;
  ifstream infile_jerBefore;
  infile_jerBefore.open(Form("txt_jet/jer_before_%s_%s.out",ihcalType.c_str(),note.c_str()));
  ijer=0;
  if (infile_jerBefore.is_open()){
    while( getline (infile_jerBefore,szjer) ) { jerBefore[ijer] = atof(szjer.c_str()); ijer++; }
  }
  
  ifstream infile_jerAfter[nMeth];
  infile_jerAfter[0].open(Form("txt_jet/jer_%s_%s_%s.out",szMeth[0].c_str(),ihcalType.c_str(),note.c_str()));
  ijer=0;
  if (infile_jerAfter[0].is_open()){
    while( getline (infile_jerAfter[0],szjer) ) { jerAfter[0][ijer] = atof(szjer.c_str()); ijer++; }
  }
  //infile_jerAfter[1].open(Form("txt_jet_wTot/jer_%s_%s_%s.out",szMeth[1].c_str(),ihcalType.c_str(),note.c_str()));
  //ijer=0;
  //if (infile_jerAfter[1].is_open()){
  //  while( getline (infile_jerAfter[1],szjer) ) { jerAfter[1][ijer] = atof(szjer.c_str()); ijer++; }
  //}
  //infile_jerAfter[2].open(Form("txt_jet_wTot/jer_%s_%s_%s.out",szMeth[2].c_str(),ihcalType.c_str(),note.c_str()));
  //ijer=0;
  //if (infile_jerAfter[2].is_open()){
  //  while( getline (infile_jerAfter[2],szjer) ) { jerAfter[2][ijer] = atof(szjer.c_str()); ijer++; }
  //}
  for (int im=0; im<nMeth; im++){
    cout << "txt_jet_syst0"<<im+1<<endl;
    infile_jerAfter[im+1].open(Form("txt_jet_syst0%d/jer_%s_%s_%s.out",im+1,szMeth[im+1].c_str(),ihcalType.c_str(),note.c_str()));
    ijer=0;
    if (infile_jerAfter[im+1].is_open()){
      while( getline (infile_jerAfter[im+1],szjer) ) { jerAfter[im+1][ijer] = atof(szjer.c_str()); ijer++; }
    }
  } 

  ////// check
  for (int ie=0; ie<nEBin; ie++){
    cout << "jerBefore["<<ie<<"] = " << jerBefore[ie] << endl;
    for (int im=0; im<nMeth; im++){
      cout << "jerAfter["<<im<<"]["<<ie<<"] = " << jerAfter[im][ie] << endl;
    }
  }
  
  TGraphAsymmErrors* gjerBefore = new TGraphAsymmErrors(nEBin, xPnt, jerBefore, exSys, exSys, ey, ey);
  gjerBefore->SetName("gjerBefore");
  TGraphAsymmErrors* gjerAfter[nMeth];
  for (int im=0; im<nMeth; im++){
    gjerAfter[im] = new TGraphAsymmErrors(nEBin, xPnt, jerAfter[im], ex, ex, ey, ey);
    gjerAfter[im]->SetName(Form("gjerAfter_%d",im));
  }

  SetSoftGraphStyle(gjerBefore,2,10);
  SetSoftGraphStyle(gjerAfter[0],2,0);
  SetSoftGraphStyle(gjerAfter[1],3,11);
  SetSoftGraphStyle(gjerAfter[2],3,12);
  SetSoftGraphStyle(gjerAfter[3],3,13);
  SetSoftGraphStyle(gjerAfter[4],3,14);
  SetSoftGraphStyle(gjerAfter[5],3,11);
  SetSoftGraphStyle(gjerAfter[6],3,12);
  SetSoftGraphStyle(gjerAfter[7],3,13);
  SetSoftGraphStyle(gjerAfter[8],3,14);
  
  //gjerBefore->GetXaxis()->SetTitle("E_{Jet}^{Truth} [GeV]");
  gjerBefore->GetXaxis()->SetTitle("p_{T,Jet}^{Truth} [GeV]");
  gjerBefore->GetXaxis()->CenterTitle(1);
  gjerBefore->GetYaxis()->SetTitle("#sigma[E_{Jet}^{Reco}/E_{Jet}^{Truth}] / <E_{Jet}^{Reco}/E_{Jet}^{Truth}>");
  gjerBefore->GetYaxis()->CenterTitle(1);
  gjerBefore->GetXaxis()->SetLimits(0,70.);
  gjerBefore->SetMinimum(jerymin); 
  gjerBefore->SetMaximum(jerymax); 

  gjerBefore->Draw("AP");
  for (int im=0; im<nMeth; im++){
    gjerAfter[im]->Draw("P");
  }
  
  //leg1->AddEntry(gjerBefore,"Before calib.","lp");
  ////leg1->AddEntry(gjerAfter[0],"After calib.","lp");
  //leg1->AddEntry(gjerAfter[0],"After calib. (3par)","lp");
  //leg1->AddEntry(gjerAfter[1],"After calib. (2par)","lp");
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
  if (ihcalType.compare("SS310")==0) { latex->DrawLatex(0.2,0.77,"Instrumented Steel");  }
  else if (ihcalType.compare("Al")==0) { latex->DrawLatex(0.2,0.77,"Instrumented Aluminum");  }
  else if (ihcalType.compare("Alframe")==0) { latex->DrawLatex(0.2,0.77,"Uninstrumented Aluminum");  }
  else { cout << "WARNING!! Al, Alframe, or SS310"<<endl; return; };
  
  c_JER->SaveAs(Form("outvsE_Syst/comp_JERvsE_%s_%s.pdf",ihcalType.c_str(),note.c_str()));

  ///////////////////////////////////////
  ////Get Syst
  double jerSys[nEBin];
  for (int ie=0; ie<nEBin; ie++){
    jerSys[ie]= -999;
    cout << "jerSys["<<ie<<"] = " << jerSys[ie] << endl;
    for (int im=1; im<nMeth; im++){
      if (jerSys[ie] < fabs(jerAfter[im][ie]-jerAfter[0][ie]) ) { jerSys[ie] = fabs(jerAfter[im][ie]-jerAfter[0][ie]); } 
      cout << im<<"th im"<<endl;
      cout << "jerAfter[0][ie] = " << jerAfter[0][ie] << endl;
      cout << "jerAfter[im][ie] = " << jerAfter[im][ie] << endl;
      cout << "jerSys["<<ie<<"] = " << jerSys[ie] << endl;
    }
  }
  
  TGraphAsymmErrors* gjerSys = new TGraphAsymmErrors(nEBin, xPnt, jerAfter[0], exSys, exSys, jerSys, jerSys);
  gjerSys->SetName("gjerSys");
  gjerSys->SetFillColorAlpha(kBlue-10,0.5); 
  gjerSys->SetLineColor(kAzure+4); 
  
  TCanvas* c_Sys = new TCanvas("c_Sys","",600,600);
  c_Sys->cd();
  SetSoftGraphStyle(gjerBefore,2,10);
  SetSoftGraphStyle(gjerAfter[0],2,0);
  SetSoftGraphStyle(gjerSys,2,0);
  gjerBefore->Draw("AP");
  gjerSys->Draw("5");
  gjerAfter[0]->Draw("P");
  
  ///////////////////////////////////////
  
  TFile* fout =  new TFile(Form("outvsE_Syst/JESvsE_%s_%s.root",ihcalType.c_str(),note.c_str()),"RECREATE");
  fout->cd();
  gjesBefore->Write();
  for (int im=0; im<nMeth; im++){   
    gjesAfter[im]->Write();
  } 
  gjerBefore->Write();
  for (int im=0; im<nMeth; im++){   
    gjerAfter[im]->Write();
  } 
  gjerSys->Write();
  fout->Close();
   
  return;
}

