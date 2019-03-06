void draw_SFvsE_Alframe(string inputType="final_qcd"){
  
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
  double cemc01[nEBin];
  double cemc02[nEBin];
  double cemc03[nEBin];
  double cemc04[nEBin];
  double cemc05[nEBin];
  double ihcal01[nEBin];
  double ihcal02[nEBin];
  double ihcal03[nEBin];
  double ihcal04[nEBin];
  double ihcal05[nEBin];
  double ohcal01[nEBin];
  double ohcal02[nEBin];
  double ohcal03[nEBin];
  double ohcal04[nEBin];
  double ohcal05[nEBin];

  //////////////////// read-in cemc files
  ifstream infile_cemc;  
  infile_cemc.open(Form("SCALEcemc_%s.out",inputType.c_str()));
  int icemc=0;
  double tmpcemc;
  string szcemc;
  if (infile_cemc.is_open()){
    while( getline (infile_cemc,szcemc) ) {
      tmpcemc = atof(szcemc.c_str());
      if (icemc <nEBin){
        dummy = tmpcemc; //no scale
      } else if (icemc < 2*nEBin){
        dummy = tmpcemc;
      } else if (icemc < 3*nEBin){
        dummy = tmpcemc;
      } else if (icemc < 4*nEBin){
        dummy = tmpcemc;
      } else if (icemc < 5*nEBin){
        dummy = tmpcemc;
      } else if (icemc < 6*nEBin){
        dummy = tmpcemc;
      } else if (icemc < 7*nEBin){
        dummy = tmpcemc;
      } else if (icemc < 8*nEBin){
        cemc01[icemc-7*nEBin] = tmpcemc;
      } else if (icemc < 9*nEBin){
        cemc02[icemc-8*nEBin] = tmpcemc;
      } else if (icemc < 10*nEBin){
        cemc03[icemc-9*nEBin] = tmpcemc;
      } else if (icemc < 11*nEBin){
        cemc04[icemc-10*nEBin] = tmpcemc;
      } else if (icemc < 12*nEBin){
        cemc05[icemc-11*nEBin] = tmpcemc;
      } else {
        cout << "ERROR!! in filling scale arrats" << endl; return;
      }
      //cout << "icemc = " << icemc << endl;  
      //cout << "szcemc = " << szcemc.c_str() << endl;  
      icemc++;
    }
  }
  //////////////////// read-in ihcal files
  ifstream infile_ihcal;  
  infile_ihcal.open(Form("SCALEihcal_%s.out",inputType.c_str()));
  int iihcal=0;
  double tmpihcal;
  string szihcal;
  if (infile_ihcal.is_open()){
    while( getline (infile_ihcal,szihcal) ) {
      tmpihcal = atof(szihcal.c_str());
      if (iihcal <nEBin){
        dummy = tmpihcal;
      } else if (iihcal < 2*nEBin){
        dummy = tmpihcal;
      } else if (iihcal < 3*nEBin){
        dummy = tmpihcal;
      } else if (iihcal < 4*nEBin){
        dummy = tmpihcal;
      } else if (iihcal < 5*nEBin){
        dummy = tmpihcal;
      } else if (iihcal < 6*nEBin){
        dummy = tmpihcal;
      } else if (iihcal < 7*nEBin){
        dummy = tmpihcal;
      } else if (iihcal < 8*nEBin){
        ihcal01[iihcal-7*nEBin] = tmpihcal;
      } else if (iihcal < 9*nEBin){
        ihcal02[iihcal-8*nEBin] = tmpihcal;
      } else if (iihcal < 10*nEBin){
        ihcal03[iihcal-9*nEBin] = tmpihcal;
      } else if (iihcal < 11*nEBin){
        ihcal04[iihcal-10*nEBin] = tmpihcal;
      } else if (iihcal < 12*nEBin){
        ihcal05[iihcal-11*nEBin] = tmpihcal;
      } else {
        cout << "ERROR!! in filling scale arrats" << endl; return;
      }
      //cout << "iihcal = " << iihcal << endl;  
      //cout << "szihcal = " << szihcal.c_str() << endl;  
      iihcal++;
    }
  }
  //////////////////// read-in ohcal files
  ifstream infile_ohcal;  
  infile_ohcal.open(Form("SCALEohcal_%s.out",inputType.c_str()));
  int iohcal=0;
  double tmpohcal;
  string szohcal;
  if (infile_ohcal.is_open()){
    while( getline (infile_ohcal,szohcal) ) {
      tmpohcal = atof(szohcal.c_str());
      if (iohcal <nEBin){
        dummy = tmpohcal;
      } else if (iohcal < 2*nEBin){
        dummy = tmpohcal;
      } else if (iohcal < 3*nEBin){
        dummy = tmpohcal;
      } else if (iohcal < 4*nEBin){
        dummy = tmpohcal;
      } else if (iohcal < 5*nEBin){
        dummy = tmpohcal;
      } else if (iohcal < 6*nEBin){
        dummy = tmpohcal;
      } else if (iohcal < 7*nEBin){
        dummy = tmpohcal;
      } else if (iohcal < 8*nEBin){
        ohcal01[iohcal-7*nEBin] = tmpohcal;
      } else if (iohcal < 9*nEBin){
        ohcal02[iohcal-8*nEBin] = tmpohcal;
      } else if (iohcal < 10*nEBin){
        ohcal03[iohcal-9*nEBin] = tmpohcal;
      } else if (iohcal < 11*nEBin){
        ohcal04[iohcal-10*nEBin] = tmpohcal;
      } else if (iohcal < 12*nEBin){
        ohcal05[iohcal-11*nEBin] = tmpohcal;
      } else {
        cout << "ERROR!! in filling scale arrats" << endl; return;
      }
      //cout << "iohcal = " << iohcal << endl;  
      //cout << "szohcal = " << szohcal.c_str() << endl;  
      iohcal++;
    }
  }
  for (int ie=0; ie<nEBin; ie++){
    cout << "cemc01["<<ie<<"] = " << cemc01[ie] << endl;
    cout << "cemc02["<<ie<<"] = " << cemc02[ie] << endl;
    cout << "cemc03["<<ie<<"] = " << cemc03[ie] << endl;
    cout << "cemc04["<<ie<<"] = " << cemc04[ie] << endl;
    cout << "cemc05["<<ie<<"] = " << cemc05[ie] << endl;
    cout << "ihcal01["<<ie<<"] = " << ihcal01[ie] << endl;
    cout << "ihcal02["<<ie<<"] = " << ihcal02[ie] << endl;
    cout << "ihcal03["<<ie<<"] = " << ihcal03[ie] << endl;
    cout << "ihcal04["<<ie<<"] = " << ihcal04[ie] << endl;
    cout << "ihcal05["<<ie<<"] = " << ihcal05[ie] << endl;
    cout << "ohcal01["<<ie<<"] = " << ohcal01[ie] << endl;
    cout << "ohcal02["<<ie<<"] = " << ohcal02[ie] << endl;
    cout << "ohcal03["<<ie<<"] = " << ohcal03[ie] << endl;
    cout << "ohcal04["<<ie<<"] = " << ohcal04[ie] << endl;
    cout << "ohcal05["<<ie<<"] = " << ohcal05[ie] << endl;
  }

  TGraphErrors* gcemc01 = new TGraphErrors(nEBin, xPnt, cemc01, ex, ey);
  TGraphErrors* gihcal01 = new TGraphErrors(nEBin, xPnt, ihcal01, ex, ey);
  TGraphErrors* gohcal01 = new TGraphErrors(nEBin, xPnt, ohcal01, ex, ey);
  TGraphErrors* gcemc02 = new TGraphErrors(nEBin, xPnt, cemc02, ex, ey);
  TGraphErrors* gihcal02 = new TGraphErrors(nEBin, xPnt, ihcal02, ex, ey);
  TGraphErrors* gohcal02 = new TGraphErrors(nEBin, xPnt, ohcal02, ex, ey);
  TGraphErrors* gcemc03 = new TGraphErrors(nEBin, xPnt, cemc03, ex, ey);
  TGraphErrors* gihcal03 = new TGraphErrors(nEBin, xPnt, ihcal03, ex, ey);
  TGraphErrors* gohcal03 = new TGraphErrors(nEBin, xPnt, ohcal03, ex, ey);
  TGraphErrors* gcemc04 = new TGraphErrors(nEBin, xPnt, cemc04, ex, ey);
  TGraphErrors* gihcal04 = new TGraphErrors(nEBin, xPnt, ihcal04, ex, ey);
  TGraphErrors* gohcal04 = new TGraphErrors(nEBin, xPnt, ohcal04, ex, ey);
  TGraphErrors* gcemc05 = new TGraphErrors(nEBin, xPnt, cemc05, ex, ey);
  TGraphErrors* gihcal05 = new TGraphErrors(nEBin, xPnt, ihcal05, ex, ey);
  TGraphErrors* gohcal05 = new TGraphErrors(nEBin, xPnt, ohcal05, ex, ey);
  
  //SetGraphStyle(gcemc01,1,10);
  //SetGraphStyle(gcemc02,1,11);
  //SetGraphStyle(gcemc03,1,12);
  //SetGraphStyle(gcemc04,1,13);
  //SetGraphStyle(gcemc05,1,14);
  
  SetGraphStyle(gcemc01,1,10);
  SetGraphStyle(gcemc02,1,11);
  SetGraphStyle(gcemc03,1,11);
  SetGraphStyle(gcemc04,1,12);
  SetGraphStyle(gcemc05,1,10);
  SetGraphStyle(gihcal01,2,10);
  SetGraphStyle(gihcal02,2,11);
  SetGraphStyle(gihcal03,2,11);
  SetGraphStyle(gihcal04,2,12);
  SetGraphStyle(gihcal05,2,10);
  SetGraphStyle(gohcal01,3,10);
  SetGraphStyle(gohcal02,3,11);
  SetGraphStyle(gohcal03,3,11);
  SetGraphStyle(gohcal04,3,12);
  SetGraphStyle(gohcal05,3,10);
  
  gcemc01->GetXaxis()->SetTitle("#gamma E_{Reco} [GeV]");
  gcemc01->GetXaxis()->CenterTitle(1);
  gcemc01->GetYaxis()->SetTitle("Scale factor");
  gcemc01->GetYaxis()->CenterTitle(1);
  gcemc01->GetXaxis()->SetLimits(0,70.);
  gcemc01->SetMinimum(ymin); 
  gcemc01->SetMaximum(ymax); 
 
  gcemc05->GetXaxis()->SetTitle("#gamma E_{Reco} [GeV]");
  gcemc05->GetXaxis()->CenterTitle(1);
  gcemc05->GetYaxis()->SetTitle("Scale factor");
  gcemc05->GetYaxis()->CenterTitle(1);
  gcemc05->GetXaxis()->SetLimits(0,70.);
  gcemc05->SetMinimum(ymin); 
  gcemc05->SetMaximum(ymax); 
  
  //gcemc01->Draw("APL");
  //gcemc02->Draw("PL");
  //gcemc03->Draw("PL");
  //gcemc04->Draw("PL");
  //gcemc05->Draw("PL");
  
  gcemc05->Draw("APL");
  gcemc03->Draw("PL");
  //gcemc04->Draw("PL");
  
  //gihcal05->Draw("PL");
  //gihcal03->Draw("PL");
  //gihcal04->Draw("PL");
  
  gohcal05->Draw("PL");
  gohcal03->Draw("PL");
  //gohcal04->Draw("PL");

  dashedLine(0.0,1.0,70.0,1.0,1,1);
  
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4");
  //latex->DrawLatex(0.2,0.77,"#gamma |#eta^{truth}| < 0.6");
  
  c_sf->SaveAs(Form("outvsE/SFvsE_Alframe_%s.pdf",inputType.c_str()));
   
  return;
}

