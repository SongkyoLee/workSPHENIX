void draw_JESJERvsE(string ihcalType="Al", string note="48fmR02"){
  
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.038);

  //TLegend* leg1 = new TLegend(0.20, 0.21, 0.50, 0.35);
  TLegend* leg1 = new TLegend(0.18, 0.57, 0.40, 0.67);
  leg1->SetTextSize(0.045);
  TLegend* leg2 = new TLegend(0.18, 0.20, 0.40, 0.30);
  leg2->SetTextSize(0.045);
  
  //// select hist x axis range
  double jesymin, jesymax; 
  //jesymin =0.35; jesymax = 1.65;
  jesymin =0.5; jesymax = 1.5;
  double jerymin, jerymax; 
  //jerymin =0.0; jerymax = 0.4;
  //jerymin =0.0; jerymax = 0.35;
  jerymin =0.0; jerymax = 0.55;
  
  ///////////////////////////////////////
  //// Get Hist for different pT range
  const int nEBin = 4;
  cout << "nEBin = " << nEBin << endl;
  double xPnt[nEBin] = {25, 35, 45, 55};
  double ex[nEBin] = {5, 5, 5, 5};
  double ey[nEBin] = {0, 0, 0, 0};
  
  const int nMeth = 2;
  cout << "nMeth = " << nMeth << endl;
  string szMeth[nMeth];
  szMeth[0]= "recogamma";
  szMeth[1]= "truthjet";

  ///////////////////////////////////////
  ///////////////////////////////////////
  //// Draw JES
  ///////////////////////////////////////
  ///////////////////////////////////////
  
  TCanvas* c_JES = new TCanvas("c_JES","",600,600);
  c_JES->cd();
  
  //// Al
  double jesAlBefore[nEBin];
  double jesAl[nMeth][nEBin];
  
  //////////////////// read-in jes_Al_files
  string szjes_Al;
  int ijes_Al=0;
  ifstream infile_jesAlBefore;
  infile_jesAlBefore.open(Form("txt_jes/jes_mean_before_%s_%s.out",ihcalType.c_str(),note.c_str()));
  ijes_Al=0;
  if (infile_jesAlBefore.is_open()){
    while( getline (infile_jesAlBefore,szjes_Al) ) { jesAlBefore[ijes_Al] = atof(szjes_Al.c_str()); ijes_Al++; }
  }
  ifstream infile_jesAl[nMeth];
  for (int im=0; im<nMeth; im++){
    //infile_jesAl[im].open(Form("txt_jes/jes_mean_method0%d_Al.out",im+1));
    //infile_jesAl[im].open(Form("txt_jes/jes_mean_%s_Al.out",szMeth[im].c_str()));
    infile_jesAl[im].open(Form("txt_jes/jes_mean_%s_%s_%s.out",szMeth[im].c_str(),ihcalType.c_str(),note.c_str()));
    ijes_Al=0;
    if (infile_jesAl[im].is_open()){
      while( getline (infile_jesAl[im],szjes_Al) ) { jesAl[im][ijes_Al] = atof(szjes_Al.c_str()); ijes_Al++; }
    }
  }

  ////// check
  for (int ie=0; ie<nEBin; ie++){
    cout << "jesAlBefore["<<ie<<"] = " << jesAlBefore[ie] << endl;
    for (int im=0; im<nMeth; im++){
      cout << "jesAl[im]["<<ie<<"] = " << jesAl[im][ie] << endl;
    }
  }
  
  TGraphAsymmErrors* gjesAlBefore = new TGraphAsymmErrors(nEBin, xPnt, jesAlBefore, ex, ex, ey, ey);
  gjesAlBefore->SetName("gjesAlBefore");
  TGraphAsymmErrors* gjesAl[nMeth];
  for (int im=0; im<nMeth; im++){
    gjesAl[im] = new TGraphAsymmErrors(nEBin, xPnt, jesAl[im], ex, ex, ey, ey);
    gjesAl[im]->SetName(Form("gjesAl_%d",im));
  }

  SetGraphStyle(gjesAlBefore,2,0);
  SetGraphStyle(gjesAl[0],2,10);
  SetGraphStyle(gjesAl[1],2,11);
  
  gjesAlBefore->GetXaxis()->SetTitle("Jet E [GeV]");
  gjesAlBefore->GetXaxis()->CenterTitle(1);
  gjesAlBefore->GetYaxis()->SetTitle("<E_{Jet}^{Reco}/E_{Jet}^{Truth}>");
  gjesAlBefore->GetYaxis()->CenterTitle(1);
  gjesAlBefore->GetXaxis()->SetLimits(0,70.);
  gjesAlBefore->SetMinimum(jesymin); 
  gjesAlBefore->SetMaximum(jesymax); 

  gjesAlBefore->Draw("AP");
//  gjesAl[0]->Draw("P");
  gjesAl[1]->Draw("P");
  
  leg1->AddEntry(gjesAlBefore,"Before calib. ","lp");
  //leg1->AddEntry(gjesAl[0],"After calib. ","lp");
  leg1->AddEntry(gjesAl[1],"After calib. ","lp");
  leg1->Draw();

  dashedLine(0.0,1.0,70.0,1.0,1,1);
  
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->SetTextSize(0.040);
  //latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4, |#eta_{Jet}^{truth}| < 0.45");
  //latex->DrawLatex(0.2,0.82,"PYTHIA8+HIJING b = 4-8 fm dijet, anti-k_{T} R=0.4, |#eta| < 0.45"); }
  latex->DrawLatex(0.2,0.82,"PYTHIA8+HIJING b = 4-8 fm dijet"); 
  if (note.compare("04fmR02")==0 || note.compare("48fmR02")==0) { latex->DrawLatex(0.2,0.77,"anti-k_{T} R=0.2, |#eta| < 0.45"); }
  else if (note.compare("04fmR04")==0 || note.compare("48fmR04")==0) { latex->DrawLatex(0.2,0.77,"anti-k_{T} R=0.4, |#eta| < 0.45"); }
  else { cout << "WARNING!! R02 or R04"<<endl; return; };
  if (ihcalType.compare("Al")==0) { latex->DrawLatex(0.2,0.72,"Instrumented Aluminum");  }
  else if (ihcalType.compare("Alframe")==0) { latex->DrawLatex(0.2,0.72,"Uninstrumented Aluminum");  }
  else if (ihcalType.compare("SS310")==0) { latex->DrawLatex(0.2,0.72,"Uninstrumented Steel");  }
  else { cout << "WARNING!! Al, Alframe, or SS310"<<endl; return; };
   
  c_JES->SaveAs(Form("outvsE/JESvsE_%s_%s.pdf",ihcalType.c_str(),note.c_str()));
  ///////////////////////////////////////

  ///////////////////////////////////////
  ///////////////////////////////////////
  //// Draw JER
  ///////////////////////////////////////
  ///////////////////////////////////////
  
  TCanvas* c_JER = new TCanvas("c_JER","",600,600);
  c_JER->cd();
  
  //// Al
  double jerAlBefore[nEBin];
  double jerAl[nMeth][nEBin];
  
  //////////////////// read-in jer_Al_files
  string szjer_Al;
  int ijer_Al=0;
  ifstream infile_jerAlBefore;
  infile_jerAlBefore.open(Form("txt_jes/jes_sigmean_before_%s_%s.out",ihcalType.c_str(),note.c_str()));
  ijer_Al=0;
  if (infile_jerAlBefore.is_open()){
    while( getline (infile_jerAlBefore,szjer_Al) ) { jerAlBefore[ijer_Al] = atof(szjer_Al.c_str()); ijer_Al++; }
  }
  ifstream infile_jerAl[nMeth];
  for (int im=0; im<nMeth; im++){
    //infile_jerAl[im].open(Form("txt_jes/jes_sigmean_method0%d_Al.out",im+1));
    //infile_jerAl[im].open(Form("txt_jes/jes_sigmean_%s_Al.out",szMeth[im].c_str()));
    infile_jerAl[im].open(Form("txt_jes/jes_sigmean_%s_%s_%s.out",szMeth[im].c_str(),ihcalType.c_str(),note.c_str()));
    ijer_Al=0;
    if (infile_jerAl[im].is_open()){
      while( getline (infile_jerAl[im],szjer_Al) ) { jerAl[im][ijer_Al] = atof(szjer_Al.c_str()); ijer_Al++; }
    }
  }

  ////// check
  for (int ie=0; ie<nEBin; ie++){
    cout << "jerAlBefore["<<ie<<"] = " << jerAlBefore[ie] << endl;
    for (int im=0; im<nMeth; im++){
      cout << "jerAl[im]["<<ie<<"] = " << jerAl[im][ie] << endl;
    }
  }
  
  TGraphAsymmErrors* gjerAlBefore = new TGraphAsymmErrors(nEBin, xPnt, jerAlBefore, ex, ex, ey, ey);
  gjerAlBefore->SetName("gjerAlBefore");
  TGraphAsymmErrors* gjerAl[nMeth];
  for (int im=0; im<nMeth; im++){
    gjerAl[im] = new TGraphAsymmErrors(nEBin, xPnt, jerAl[im], ex, ex, ey, ey);
    gjerAl[im]->SetName(Form("gjerAl_%d",im));
  }

  SetGraphStyle(gjerAlBefore,2,0);
  SetGraphStyle(gjerAl[0],2,10);
  SetGraphStyle(gjerAl[1],2,11);
  
  gjerAlBefore->GetXaxis()->SetTitle("Jet E [GeV]");
  gjerAlBefore->GetXaxis()->CenterTitle(1);
  gjerAlBefore->GetYaxis()->SetTitle("#sigma(E_{Jet}^{Reco}/E_{Jet}^{Truth}) / <E_{Jet}^{Reco}/E_{Jet}^{Truth}>");
  gjerAlBefore->GetYaxis()->CenterTitle(1);
  gjerAlBefore->GetXaxis()->SetLimits(0,70.);
  gjerAlBefore->SetMinimum(jerymin); 
  gjerAlBefore->SetMaximum(jerymax); 

  gjerAlBefore->Draw("AP");
//  gjerAl[0]->Draw("P");
  gjerAl[1]->Draw("P");
  
  leg2->AddEntry(gjesAlBefore,"Before calib. ","lp");
  //leg2->AddEntry(gjesAl[0],"After calib. ","lp");
  leg2->AddEntry(gjesAl[1],"After calib. ","lp");
  leg2->Draw();

  dashedLine(0.0,1.0,70.0,1.0,1,1);
  
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->SetTextSize(0.040);
  //latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4, |#eta_{Jet}^{truth}| < 0.45");
  latex->DrawLatex(0.2,0.82,"PYTHIA8+HIJING b = 4-8 fm dijet"); 
  if (note.compare("04fmR02")==0 || note.compare("48fmR02")==0) { latex->DrawLatex(0.2,0.77,"anti-k_{T} R=0.2, |#eta| < 0.45"); }
  else if (note.compare("04fmR04")==0 || note.compare("48fmR04")==0) { latex->DrawLatex(0.2,0.77,"anti-k_{T} R=0.4, |#eta| < 0.45"); }
  else { cout << "WARNING!! R02 or R04"<<endl; return; };
  if (ihcalType.compare("Al")==0) { latex->DrawLatex(0.2,0.72,"Instrumented Aluminum");  }
  else if (ihcalType.compare("Alframe")==0) { latex->DrawLatex(0.2,0.72,"Uninstrumented Aluminum");  }
  else if (ihcalType.compare("SS310")==0) { latex->DrawLatex(0.2,0.72,"Uninstrumented Steel");  }
  else { cout << "WARNING!! Al, Alframe, or SS310"<<endl; return; };
  
  c_JER->SaveAs(Form("outvsE/JERvsE_%s_%s.pdf",ihcalType.c_str(),note.c_str()));
  ///////////////////////////////////////
  
  TFile* fout =  new TFile(Form("outvsE/JESvsE_%s_%s.root",ihcalType.c_str(),note.c_str()),"RECREATE");
  fout->cd();
  gjesAlBefore->Write();
  for (int im=0; im<nMeth; im++){   
    gjesAl[im]->Write();
  } 
  gjerAlBefore->Write();
  for (int im=0; im<nMeth; im++){   
    gjerAl[im]->Write();
  } 
  fout->Close();
   
  return;
}

