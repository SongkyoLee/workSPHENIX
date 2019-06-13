void draw_SFvsE(string ihcalType="SS310"){
  
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
  ymin =0.9; ymax = 5.0;
  
  ///////////////////////////////////////
  //// Get Hist for different pT range
  const int nEBin = 4;
  cout << "nEBin = " << nEBin << endl;
  const int nMeth = 5;
  cout << "nMeth = " << nMeth << endl;
  double xPnt[nEBin] = {25, 35, 45, 55};
  //double ex[nEBin] = {5, 5, 5, 5};
  double ex[nEBin] = {0, 0, 0, 0};
  double ey[nEBin] = {0, 0, 0, 0};

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
    infile_cemc[im].open(Form("txt_scale/cemcScale_method0%d_%s.out",im+1,ihcalType.c_str()));
    icemc=0;
    if (infile_cemc[im].is_open()){
      while( getline (infile_cemc[im],szcemc) ) { cemc[im][icemc] = atof(szcemc.c_str()); icemc++; }
    }
  }
  string szihcal;
  int iihcal=0;
  ifstream infile_ihcal[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_ihcal[im].open(Form("txt_scale/ihcalScale_method0%d_%s.out",im+1,ihcalType.c_str()));
    iihcal=0;
    if (infile_ihcal[im].is_open()){
      while( getline (infile_ihcal[im],szihcal) ) { ihcal[im][iihcal] = atof(szihcal.c_str()); iihcal++; }
    }
  }
  string szohcal;
  int iohcal=0;
  ifstream infile_ohcal[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_ohcal[im].open(Form("txt_scale/ohcalScale_method0%d_%s.out",im+1,ihcalType.c_str()));
    iohcal=0;
    if (infile_ohcal[im].is_open()){
      while( getline (infile_ohcal[im],szohcal) ) { ohcal[im][iohcal] = atof(szohcal.c_str()); iohcal++; }
    }
  }
 
  for (int im=0; im<nMeth; im++){   
    for (int ie=0; ie<nEBin; ie++){
      //cout << "cemc["<<im<<"]["<<ie<<"] = " << cemc[im][ie] << endl;
      //cout << "ihcal["<<im<<"]["<<ie<<"] = " << ihcal[im][ie] << endl;
      //cout << "ohcal["<<im<<"]["<<ie<<"] = " << ohcal[im][ie] << endl;
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
  
  //SetGraphStyle(gcemc[0],1,10);
  //SetGraphStyle(gcemc[1],1,11);
  SetGraphStyle(gcemc[2],1,11);
  //SetGraphStyle(gcemc[3],1,12);
  //SetGraphStyle(gcemc[4],1,10);
  //SetGraphStyle(gihcal[0],2,10);
  //SetGraphStyle(gihcal[1],2,11);
  SetGraphStyle(gihcal[2],2,11);
  //SetGraphStyle(gihcal[3],2,12);
  //SetGraphStyle(gihcal[4],2,10);
  //SetGraphStyle(gohcal[0],3,10);
  //SetGraphStyle(gohcal[1],3,11);
  SetGraphStyle(gohcal[2],3,11);
  //SetGraphStyle(gohcal[3],3,12);
  //SetGraphStyle(gohcal[4],3,10);
  
  gcemc[2]->GetXaxis()->SetTitle("#gamma E_{Reco} [GeV]");
  gcemc[2]->GetXaxis()->CenterTitle(1);
  gcemc[2]->GetYaxis()->SetTitle("Scale factor");
  gcemc[2]->GetYaxis()->CenterTitle(1);
  gcemc[2]->GetXaxis()->SetLimits(0,70.);
  gcemc[2]->SetMinimum(ymin); 
  gcemc[2]->SetMaximum(ymax); 
  
  gcemc[4]->GetXaxis()->SetTitle("#gamma E_{Reco} [GeV]");
  gcemc[4]->GetXaxis()->CenterTitle(1);
  gcemc[4]->GetYaxis()->SetTitle("Scale factor");
  gcemc[4]->GetYaxis()->CenterTitle(1);
  gcemc[4]->GetXaxis()->SetLimits(0,70.);
  gcemc[4]->SetMinimum(ymin); 
  gcemc[4]->SetMaximum(ymax); 
  
  //gcemc[0]->Draw("APL");
  //gcemc[1]->Draw("PL");
  //gcemc[2]->Draw("PL");
  //gcemc[3]->Draw("PL");
  //gcemc[4]->Draw("PL");
  
  //gcemc[4]->Draw("APL");
  gcemc[2]->Draw("APL");
  //if(ihcalType.compare("Alframe")!=0) gcemc[3]->Draw("PL");
  
  //if(ihcalType.compare("Alframe")!=0) gihcal[4]->Draw("PL");
  if(ihcalType.compare("Alframe")!=0) gihcal[2]->Draw("PL");
  //if(ihcalType.compare("Alframe")!=0) gihcal[3]->Draw("PL");
  
  //gohcal[4]->Draw("PL");
  gohcal[2]->Draw("PL");
  //gohcal[3]->Draw("PL");

  dashedLine(0.0,1.0,70.0,1.0,1,1);
  
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.2,0.82,"PYTHIA8 #gamma-jet, anti-k_{T} R=0.4, |#eta| < 0.45");
  //latex->DrawLatex(0.2,0.77,"#gamma |#eta^{truth}| < 0.6");
  
  c_sf->SaveAs(Form("outvsE/SFvsE_%s.pdf",ihcalType.c_str()));

  ///////////////////////////////////////
  
  TFile* fout =  new TFile(Form("outvsE/SFvsE_%s.root",ihcalType.c_str()),"RECREATE");
  fout->cd();
  for (int im=0; im<nMeth; im++){   
    gcemc[im]->Write();
    gihcal[im]->Write();
    gohcal[im]->Write();
  } 
  fout->Close();
   
  return;
}

