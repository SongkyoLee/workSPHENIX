void draw_SFvsE(string ihcalType="Al", string note="R04"){
  
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
  ymin =0.9; ymax = 3.5;
  if(ihcalType.compare("Al")==0) { 
    if (note.compare("R02")==0) {ymax = 12.0; }
    else if (note.compare("R04")==0) {ymax = 6.0; }
  }
   
  ///////////////////////////////////////
  //// Get Hist for different pT range
  const int nEBin = 4;
  cout << "nEBin = " << nEBin << endl;
  double xPnt[nEBin] = {25, 35, 45, 55};
  //double ex[nEBin] = {5, 5, 5, 5};
  double ex[nEBin] = {0, 0, 0, 0};
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
  
  TCanvas* c_sf = new TCanvas("c_sf","",600,600);
  c_sf->cd();
  
  double dummy;
  double cemc[nMeth][nEBin];
  double ihcal[nMeth][nEBin];
  double ohcal[nMeth][nEBin];

  //////////////////// read-in cemc files
  string szcemc;
  int icemc = 0;
  ifstream infile_cemc[nMeth];
  for (int im=0; im<nMeth; im++){
    //infile_cemc[im].open(Form("txt_scale/cemcScale_method0%d_%s.out",im+1,cemcType.c_str()));
    //infile_cemc[im].open(Form("txt_scale/cemcScale_%s_%s.out",szMeth[im].c_str(),ihcalType.c_str()));
    infile_cemc[im].open(Form("txt_scale/cemcScale_%s_%s_%s.out",szMeth[im].c_str(),ihcalType.c_str(),note.c_str()));
    icemc=0;
    if (infile_cemc[im].is_open()){
      while( getline (infile_cemc[im],szcemc) ) { cemc[im][icemc] = atof(szcemc.c_str()); icemc++; }
    }
  }
  string szihcal;
  int iihcal=0;
  ifstream infile_ihcal[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_ihcal[im].open(Form("txt_scale/ihcalScale_%s_%s_%s.out",szMeth[im].c_str(),ihcalType.c_str(),note.c_str()));
    iihcal=0;
    if (infile_ihcal[im].is_open()){
      while( getline (infile_ihcal[im],szihcal) ) { ihcal[im][iihcal] = atof(szihcal.c_str()); iihcal++; }
    }
  }
  string szohcal;
  int iohcal=0;
  ifstream infile_ohcal[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_ohcal[im].open(Form("txt_scale/ohcalScale_%s_%s_%s.out",szMeth[im].c_str(),ihcalType.c_str(),note.c_str()));
    iohcal=0;
    if (infile_ohcal[im].is_open()){
      while( getline (infile_ohcal[im],szohcal) ) { ohcal[im][iohcal] = atof(szohcal.c_str()); iohcal++; }
    }
  }
 
  for (int im=0; im<nMeth; im++){   
    for (int ie=0; ie<nEBin; ie++){
      cout << "cemc["<<im<<"]["<<ie<<"] = " << cemc[im][ie] << endl;
      cout << "ihcal["<<im<<"]["<<ie<<"] = " << ihcal[im][ie] << endl;
      cout << "ohcal["<<im<<"]["<<ie<<"] = " << ohcal[im][ie] << endl;
    }
  }
  
  TGraphAsymmErrors* gcemc[nMeth];
  TGraphAsymmErrors* gihcal[nMeth];
  TGraphAsymmErrors* gohcal[nMeth];
  for (int im=0; im<nMeth; im++){   
    gcemc[im] = new TGraphAsymmErrors(nEBin, xPnt, cemc[im], ex, ex, ey, ey);
    gihcal[im] = new TGraphAsymmErrors(nEBin, xPnt, ihcal[im], ex, ex, ey, ey);
    gohcal[im] = new TGraphAsymmErrors(nEBin, xPnt, ohcal[im], ex, ex, ey, ey);
    gcemc[im]->SetName(Form("gcemc_%d",im));
    gihcal[im]->SetName(Form("gihcal_%d",im));
    gohcal[im]->SetName(Form("gohcal_%d",im));
  } 
  
  SetGraphStyle(gcemc[0],1,0);
  SetGraphStyle(gcemc[1],1,10);
  SetGraphStyle(gihcal[0],2,0);
  SetGraphStyle(gihcal[1],2,10);
  SetGraphStyle(gohcal[0],3,0);
  SetGraphStyle(gohcal[1],3,10);
  
  gcemc[0]->GetXaxis()->SetTitle("#gamma E_{Reco} [GeV]");
  gcemc[0]->GetXaxis()->CenterTitle(1);
  gcemc[0]->GetYaxis()->SetTitle("Scale factor");
  gcemc[0]->GetYaxis()->CenterTitle(1);
  gcemc[0]->GetXaxis()->SetLimits(0,70.);
  gcemc[0]->SetMinimum(ymin); 
  gcemc[0]->SetMaximum(ymax); 
  
  gcemc[0]->Draw("APL");
  gcemc[1]->Draw("PL");
  
  if(ihcalType.compare("Alframe")!=0) {
    gihcal[0]->Draw("PL");
    gihcal[1]->Draw("PL");
  }
  gohcal[0]->Draw("PL");
  gohcal[1]->Draw("PL");

  dashedLine(0.0,1.0,70.0,1.0,1,1);
  
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->SetTextSize(0.040);
  if (note.compare("R02")==0) { latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.2, |#eta| < 0.45"); }
  else if (note.compare("R04")==0) { latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4, |#eta| < 0.45"); }
  else { cout << "WARNING!! R02 or R04"<<endl; return; };
  //latex->DrawLatex(0.2,0.77,"#gamma |#eta^{truth}| < 0.6");
  
  c_sf->SaveAs(Form("outvsE/SFvsE_%s_%s.pdf",ihcalType.c_str(),note.c_str()));

  ///////////////////////////////////////
  
  TFile* fout =  new TFile(Form("outvsE/SFvsE_%s_%s.root",ihcalType.c_str(),note.c_str()),"RECREATE");
  fout->cd();
  for (int im=0; im<nMeth; im++){   
    gcemc[im]->Write();
    gihcal[im]->Write();
    gohcal[im]->Write();
  } 
  fout->Close();
   
  return;
}

