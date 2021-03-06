void draw_JESJERvsE_merged(string ihcalType="Al", string note="R04"){
  
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
  
  //const int nMeth = 1;
  //cout << "nMeth = " << nMeth << endl;
  //string szMeth[nMeth];
  //szMeth[0]= "truthjet";
  const int nMeth = 2;
  cout << "nMeth = " << nMeth << endl;
  string szMeth[nMeth];
  szMeth[0]= "truthjet";
  szMeth[1]= "truthjet_2par";

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
  infile_jesBefore.open(Form("txt_jet_merged/jes_before_%s_%s.out",ihcalType.c_str(),note.c_str()));
  ijes=0;
  if (infile_jesBefore.is_open()){
    while( getline (infile_jesBefore,szjes) ) { jesBefore[ijes] = atof(szjes.c_str()); ijes++; }
  }
  ifstream infile_jesAfter[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_jesAfter[im].open(Form("txt_jet_merged/jes_%s_%s_%s.out",szMeth[im].c_str(),ihcalType.c_str(),note.c_str()));
    ijes=0;
    if (infile_jesAfter[im].is_open()){
      while( getline (infile_jesAfter[im],szjes) ) { jesAfter[im][ijes] = atof(szjes.c_str()); ijes++; }
    }
  }

  ////// check
  for (int ie=0; ie<nEBin; ie++){
    cout << "jesBefore["<<ie<<"] = " << jesBefore[ie] << endl;
    for (int im=0; im<nMeth; im++){
      cout << "jesAfter[im]["<<ie<<"] = " << jesAfter[im][ie] << endl;
    }
  }
  
  TGraphAsymmErrors* gjesBefore = new TGraphAsymmErrors(nEBin, xPnt, jesBefore, ex, ex, ey, ey);
  gjesBefore->SetName("gjesBefore");
  TGraphAsymmErrors* gjesAfter[nMeth];
  for (int im=0; im<nMeth; im++){
    gjesAfter[im] = new TGraphAsymmErrors(nEBin, xPnt, jesAfter[im], ex, ex, ey, ey);
    gjesAfter[im]->SetName(Form("gjesAfter_%d",im));
  }

  SetSoftGraphStyle(gjesBefore,2,0);
  SetSoftGraphStyle(gjesAfter[0],2,10);
  SetSoftGraphStyle(gjesAfter[1],2,11);
  
  //gjesBefore->GetXaxis()->SetTitle("E_{Jet}^{Truth} [GeV]");
  //gjesBefore->GetXaxis()->SetTitle("p_{T,Jet}^{Truth} [GeV]");
  gjesBefore->GetXaxis()->SetTitle("p_{T,Jet}^{Reco} [GeV]");
  gjesBefore->GetXaxis()->CenterTitle(1);
  gjesBefore->GetYaxis()->SetTitle("<E_{Jet}^{Reco}/E_{Jet}^{Truth}>");
  gjesBefore->GetYaxis()->CenterTitle(1);
  gjesBefore->GetXaxis()->SetLimits(0,70.);
  gjesBefore->SetMinimum(jesymin); 
  gjesBefore->SetMaximum(jesymax); 

  gjesBefore->Draw("AP");
  gjesAfter[0]->Draw("P");
  //gjesAfter[1]->Draw("P");
  
  leg1->AddEntry(gjesBefore,"Before calib.","lp");
  //leg1->AddEntry(gjesAfter[0],"After calib.","lp");
  leg1->AddEntry(gjesAfter[0],"After calib. (3par)","lp");
  //leg1->AddEntry(gjesAfter[1],"After calib. (2par)","lp");
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
  
  c_JES->SaveAs(Form("outvsEMerged/JESvsE_%s_%s.pdf",ihcalType.c_str(),note.c_str()));
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
  infile_jerBefore.open(Form("txt_jet_merged/jer_before_%s_%s.out",ihcalType.c_str(),note.c_str()));
  ijer=0;
  if (infile_jerBefore.is_open()){
    while( getline (infile_jerBefore,szjer) ) { jerBefore[ijer] = atof(szjer.c_str()); ijer++; }
  }
  ifstream infile_jerAfter[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_jerAfter[im].open(Form("txt_jet_merged/jer_%s_%s_%s.out",szMeth[im].c_str(),ihcalType.c_str(),note.c_str()));
    ijer=0;
    if (infile_jerAfter[im].is_open()){
      while( getline (infile_jerAfter[im],szjer) ) { jerAfter[im][ijer] = atof(szjer.c_str()); ijer++; }
    }
  }

  ////// check
  for (int ie=0; ie<nEBin; ie++){
    cout << "jerBefore["<<ie<<"] = " << jerBefore[ie] << endl;
    for (int im=0; im<nMeth; im++){
      cout << "jerAfter[im]["<<ie<<"] = " << jerAfter[im][ie] << endl;
    }
  }
  
  TGraphAsymmErrors* gjerBefore = new TGraphAsymmErrors(nEBin, xPnt, jerBefore, ex, ex, ey, ey);
  gjerBefore->SetName("gjerBefore");
  TGraphAsymmErrors* gjerAfter[nMeth];
  for (int im=0; im<nMeth; im++){
    gjerAfter[im] = new TGraphAsymmErrors(nEBin, xPnt, jerAfter[im], ex, ex, ey, ey);
    gjerAfter[im]->SetName(Form("gjerAfter_%d",im));
  }

  SetSoftGraphStyle(gjerBefore,2,0);
  SetSoftGraphStyle(gjerAfter[0],2,10);
  SetSoftGraphStyle(gjerAfter[1],2,11);
  
  //gjerBefore->GetXaxis()->SetTitle("E_{Jet}^{Truth} [GeV]");
  //gjerBefore->GetXaxis()->SetTitle("p_{T,Jet}^{Truth} [GeV]");
  gjerBefore->GetXaxis()->SetTitle("p_{T,Jet}^{Reco} [GeV]");
  gjerBefore->GetXaxis()->CenterTitle(1);
  gjerBefore->GetYaxis()->SetTitle("#sigma[E_{Jet}^{Reco}/E_{Jet}^{Truth}] / <E_{Jet}^{Reco}/E_{Jet}^{Truth}>");
  gjerBefore->GetYaxis()->CenterTitle(1);
  gjerBefore->GetXaxis()->SetLimits(0,70.);
  gjerBefore->SetMinimum(jerymin); 
  gjerBefore->SetMaximum(jerymax); 

  gjerBefore->Draw("AP");
  gjerAfter[0]->Draw("P");
  //gjerAfter[1]->Draw("P");
  
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
  
  c_JER->SaveAs(Form("outvsEMerged/JERvsE_%s_%s.pdf",ihcalType.c_str(),note.c_str()));
  ///////////////////////////////////////
  
  TFile* fout =  new TFile(Form("outvsEMerged/JESvsE_%s_%s.root",ihcalType.c_str(),note.c_str()),"RECREATE");
  fout->cd();
  gjesBefore->Write();
  for (int im=0; im<nMeth; im++){   
    gjesAfter[im]->Write();
  } 
  gjerBefore->Write();
  for (int im=0; im<nMeth; im++){   
    gjerAfter[im]->Write();
  } 
  fout->Close();
   
  return;
}

