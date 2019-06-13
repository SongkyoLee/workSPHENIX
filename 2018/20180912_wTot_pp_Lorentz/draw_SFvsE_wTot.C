void draw_SFvsE_wTot(string ihcalType="SS310", string note="R04"){
  
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.038);

  //TLegend* leg1 = new TLegend(0.20, 0.21, 0.50, 0.35);
  //leg1->SetTextSize(0.045);
  TLegend* leg1 = new TLegend(0.20, 0.21, 0.35, 0.35);
  leg1->SetTextSize(0.035);
  TLegend* leg2 = new TLegend(0.20, 0.21, 0.50, 0.35);
  leg2->SetTextSize(0.045);
  
  //// select hist x axis range
  double ymin, ymax; 
  ymin =0.9; ymax = 3.5;
  if(ihcalType.compare("Al")==0) { 
    if (note.compare("R02")==0) {ymax = 12.0; }
    else if (note.compare("R04")==0) {ymax = 12.0; }
  }
   
  ///////////////////////////////////////
  //// Get Hist for different pT range
  const int nEBin = 4;
  cout << "nEBin = " << nEBin << endl;
  double xPnt[nEBin] = {25, 35, 45, 55};
  //double ex[nEBin] = {5, 5, 5, 5};
  double ex[nEBin] = {0, 0, 0, 0};
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
  ///////////////////////////////////////
  ///////////////////////////////////////
  
  TCanvas* c_sf = new TCanvas("c_sf","",600,600);
  c_sf->cd();
  
  double dummy;
  double cemc[nMeth][nEBin];
  double ihcal[nMeth][nEBin];
  double ohcal[nMeth][nEBin];
  double total[nMeth][nEBin];

  //////////////////// read-in cemc files
  string szcemc;
  int icemc = 0;
  ifstream infile_cemc[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_cemc[im].open(Form("txt_scale_wTot/cemcScale_%s_%s_%s.out",szMeth[im].c_str(),ihcalType.c_str(),note.c_str()));
    icemc=0;
    if (infile_cemc[im].is_open()){
      while( getline (infile_cemc[im],szcemc) ) { cemc[im][icemc] = atof(szcemc.c_str()); icemc++; }
    }
  }
  //////////////////// read-in ihcal files
  string szihcal;
  int iihcal=0;
  ifstream infile_ihcal[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_ihcal[im].open(Form("txt_scale_wTot/ihcalScale_%s_%s_%s.out",szMeth[im].c_str(),ihcalType.c_str(),note.c_str()));
    iihcal=0;
    if (infile_ihcal[im].is_open()){
      while( getline (infile_ihcal[im],szihcal) ) { ihcal[im][iihcal] = atof(szihcal.c_str()); iihcal++; }
    }
  }
  //////////////////// read-in ohcal files
  string szohcal;
  int iohcal=0;
  ifstream infile_ohcal[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_ohcal[im].open(Form("txt_scale_wTot/ohcalScale_%s_%s_%s.out",szMeth[im].c_str(),ihcalType.c_str(),note.c_str()));
    iohcal=0;
    if (infile_ohcal[im].is_open()){
      while( getline (infile_ohcal[im],szohcal) ) { ohcal[im][iohcal] = atof(szohcal.c_str()); iohcal++; }
    }
  }
  //////////////////// read-in total files
  string sztotal;
  int itotal=0;
  ifstream infile_total[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_total[im].open(Form("txt_scale_wTot/totalScale_%s_%s_%s.out",szMeth[im].c_str(),ihcalType.c_str(),note.c_str()));
    itotal=0;
    if (infile_total[im].is_open()){
      while( getline (infile_total[im],sztotal) ) { total[im][itotal] = atof(sztotal.c_str()); itotal++; }
    }
  }
 
  for (int im=0; im<nMeth; im++){   
    for (int ie=0; ie<nEBin; ie++){
      cout << "cemc["<<im<<"]["<<ie<<"] = " << cemc[im][ie] << endl;
      cout << "ihcal["<<im<<"]["<<ie<<"] = " << ihcal[im][ie] << endl;
      cout << "ohcal["<<im<<"]["<<ie<<"] = " << ohcal[im][ie] << endl;
      cout << "total["<<im<<"]["<<ie<<"] = " << total[im][ie] << endl;
    }
  }
  
  TGraphAsymmErrors* gcemc[nMeth];
  TGraphAsymmErrors* gihcal[nMeth];
  TGraphAsymmErrors* gohcal[nMeth];
  TGraphAsymmErrors* gtotal[nMeth];
  for (int im=0; im<nMeth; im++){   
    gcemc[im] = new TGraphAsymmErrors(nEBin, xPnt, cemc[im], ex, ex, ey, ey);
    gihcal[im] = new TGraphAsymmErrors(nEBin, xPnt, ihcal[im], ex, ex, ey, ey);
    gohcal[im] = new TGraphAsymmErrors(nEBin, xPnt, ohcal[im], ex, ex, ey, ey);
    gtotal[im] = new TGraphAsymmErrors(nEBin, xPnt, total[im], ex, ex, ey, ey);
    gcemc[im]->SetName(Form("gcemc_%d",im));
    gihcal[im]->SetName(Form("gihcal_%d",im));
    gohcal[im]->SetName(Form("gohcal_%d",im));
    gtotal[im]->SetName(Form("gtotal_%d",im));
  } 
  
  SetGraphStyle(gcemc[0],1,0);
  SetGraphStyle(gihcal[0],2,0);
  SetGraphStyle(gohcal[0],3,0);
  SetGraphStyle(gtotal[0],4,0);
  SetGraphStyle(gcemc[1],1,10);
  SetGraphStyle(gihcal[1],2,10);
  SetGraphStyle(gohcal[1],3,10);
  SetGraphStyle(gtotal[1],4,10);
  
  //gcemc[0]->GetXaxis()->SetTitle("E_{#gamma}^{Reco} or E_{Jet}^{Truth} [GeV]");
  gcemc[0]->GetXaxis()->SetTitle("p_{T,Jet}^{Truth} [GeV]");
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
  gtotal[0]->Draw("PL");
  gtotal[1]->Draw("PL");

  leg1->AddEntry(gcemc[0],"EMCAL","lp");
  if (ihcalType.compare("Alframe")!=0) leg1->AddEntry(gihcal[0],"IHCAL","lp");
  leg1->AddEntry(gohcal[0],"OHCAL","lp");
  leg1->AddEntry(gtotal[0],"Total","lp");
  leg1->Draw();
  
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
  if (ihcalType.compare("SS310")==0) { latex->DrawLatex(0.2,0.77,"Instrumented Steel");  }
  else if (ihcalType.compare("Al")==0) { latex->DrawLatex(0.2,0.77,"Instrumented Aluminum");  }
  else if (ihcalType.compare("Alframe")==0) { latex->DrawLatex(0.2,0.77,"Uninstrumented Aluminum");  }
  else { cout << "WARNING!! Al, Alframe, or SS310"<<endl; return; };
  
  c_sf->SaveAs(Form("outvsE_wTot/SFvsE_%s_%s.pdf",ihcalType.c_str(),note.c_str()));

  ///////////////////////////////////////
  
  TFile* fout =  new TFile(Form("outvsE_wTot/SFvsE_%s_%s.root",ihcalType.c_str(),note.c_str()),"RECREATE");
  fout->cd();
  for (int im=0; im<nMeth; im++){   
    gcemc[im]->Write();
    gihcal[im]->Write();
    gohcal[im]->Write();
  } 
  fout->Close();
   
  return;
}

