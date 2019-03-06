void draw_JESJERvsE(string inputType="final_qcd"){
  
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.038);

  TLegend* leg1 = new TLegend(0.20, 0.21, 0.50, 0.35);
  leg1->SetTextSize(0.045);
  TLegend* leg2 = new TLegend(0.20, 0.21, 0.50, 0.35);
  leg2->SetTextSize(0.045);
  
  //// select hist x axis range
  double jesymin, jesymax; 
  jesymin =0.35; jesymax = 1.65;
  double jerymin, jerymax; 
  jerymin =0.0; jerymax = 0.4;
  
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
  double jesSS310After01[nEBin];
  double jesSS310After02[nEBin];
  double jesSS310After03[nEBin];
  double jesSS310After04[nEBin];
  double jesSS310After05[nEBin];
  //// Alframe
  double jesAlframeBefore[nEBin];
  double jesAlframeAfter01[nEBin];
  double jesAlframeAfter02[nEBin];
  double jesAlframeAfter03[nEBin];
  double jesAlframeAfter04[nEBin];
  double jesAlframeAfter05[nEBin];
 
  //////////////////// read-in JES files
  ifstream infileJES;  
  //infileJES.open("JES_final_qcd.out");
  infileJES.open(Form("JES_%s.out",inputType.c_str()));
  int ijes=0;
  double tmpjes;
  string szjes;
  if (infileJES.is_open()){
    while( getline (infileJES,szjes) ) {
      tmpjes = atof(szjes.c_str());
      if (ijes <nEBin){
        jesSS310Before[ijes] = tmpjes;
      } else if (ijes < 2*nEBin){
        jesSS310After01[ijes-1*nEBin] = tmpjes;
      } else if (ijes < 3*nEBin){
        jesSS310After02[ijes-2*nEBin] = tmpjes;
      } else if (ijes < 4*nEBin){
        jesSS310After03[ijes-3*nEBin] = tmpjes;
      } else if (ijes < 5*nEBin){
        jesSS310After04[ijes-4*nEBin] = tmpjes;
      } else if (ijes < 6*nEBin){
        jesSS310After05[ijes-5*nEBin] = tmpjes;
      } else if (ijes < 7*nEBin){
        jesAlframeBefore[ijes-6*nEBin] = tmpjes;
      } else if (ijes < 8*nEBin){
        jesAlframeAfter01[ijes-7*nEBin] = tmpjes;
      } else if (ijes < 9*nEBin){
        jesAlframeAfter02[ijes-8*nEBin] = tmpjes;
      } else if (ijes < 10*nEBin){
        jesAlframeAfter03[ijes-9*nEBin] = tmpjes;
      } else if (ijes < 11*nEBin){
        jesAlframeAfter04[ijes-10*nEBin] = tmpjes;
      } else if (ijes < 12*nEBin){
        jesAlframeAfter05[ijes-11*nEBin] = tmpjes;
      } else {
        cout << "ERROR!! in filling jes arrays" << endl; return;
      }
      //cout << "ijes = " << ijes << endl;  
      //cout << "szjes = " << szjes.c_str() << endl;  
      ijes++;
    }
  }
  for (int ie=0; ie<nEBin; ie++){
    cout << "jesSS310Before["<<ie<<"] = " << jesSS310Before[ie] << endl;
    cout << "jesSS310After01["<<ie<<"] = " << jesSS310After01[ie] << endl;
    cout << "jesSS310After02["<<ie<<"] = " << jesSS310After02[ie] << endl;
    cout << "jesSS310After03["<<ie<<"] = " << jesSS310After03[ie] << endl;
    cout << "jesSS310After04["<<ie<<"] = " << jesSS310After04[ie] << endl;
    cout << "jesSS310After05["<<ie<<"] = " << jesSS310After05[ie] << endl;
  }
  for (int ie=0; ie<nEBin; ie++){
    cout << "jesAlframeBefore["<<ie<<"] = " << jesAlframeBefore[ie] << endl;
    cout << "jesAlframeAfter01["<<ie<<"] = " << jesAlframeAfter01[ie] << endl;
    cout << "jesAlframeAfter02["<<ie<<"] = " << jesAlframeAfter02[ie] << endl;
    cout << "jesAlframeAfter03["<<ie<<"] = " << jesAlframeAfter03[ie] << endl;
    cout << "jesAlframeAfter04["<<ie<<"] = " << jesAlframeAfter04[ie] << endl;
    cout << "jesAlframeAfter05["<<ie<<"] = " << jesAlframeAfter05[ie] << endl;
  }
  
  TGraphErrors* gjesSS310Before = new TGraphErrors(nEBin, xPnt, jesSS310Before, ex, ey);
  TGraphErrors* gjesSS310After01 = new TGraphErrors(nEBin, xPnt, jesSS310After01, ex, ey);
  TGraphErrors* gjesSS310After02 = new TGraphErrors(nEBin, xPnt, jesSS310After02, ex, ey);
  TGraphErrors* gjesSS310After03 = new TGraphErrors(nEBin, xPnt, jesSS310After03, ex, ey);
  TGraphErrors* gjesSS310After04 = new TGraphErrors(nEBin, xPnt, jesSS310After04, ex, ey);
  TGraphErrors* gjesSS310After05 = new TGraphErrors(nEBin, xPnt, jesSS310After05, ex, ey);
  TGraphErrors* gjesAlframeBefore = new TGraphErrors(nEBin, xPnt, jesAlframeBefore, ex, ey);
  TGraphErrors* gjesAlframeAfter01 = new TGraphErrors(nEBin, xPnt, jesAlframeAfter01, ex, ey);
  TGraphErrors* gjesAlframeAfter02 = new TGraphErrors(nEBin, xPnt, jesAlframeAfter02, ex, ey);
  TGraphErrors* gjesAlframeAfter03 = new TGraphErrors(nEBin, xPnt, jesAlframeAfter03, ex, ey);
  TGraphErrors* gjesAlframeAfter04 = new TGraphErrors(nEBin, xPnt, jesAlframeAfter04, ex, ey);
  TGraphErrors* gjesAlframeAfter05 = new TGraphErrors(nEBin, xPnt, jesAlframeAfter05, ex, ey);

  //SetGraphStyle(gjesSS310Before,0,0);
  //SetGraphStyle(gjesSS310After01,0,10);
  //SetGraphStyle(gjesSS310After02,0,11);
  //SetGraphStyle(gjesSS310After03,0,12);
  //SetGraphStyle(gjesSS310After04,0,13);
  //SetGraphStyle(gjesSS310After05,0,14);
  
  SetGraphStyle(gjesSS310Before,0,0);
  SetGraphStyle(gjesSS310After01,0,10);
  SetGraphStyle(gjesSS310After02,0,11);
  SetGraphStyle(gjesSS310After03,0,11);
  SetGraphStyle(gjesSS310After04,0,12);
  SetGraphStyle(gjesSS310After05,0,10);
  
  SetGraphStyle(gjesAlframeBefore,1,0);
  SetGraphStyle(gjesAlframeAfter01,1,10);
  SetGraphStyle(gjesAlframeAfter02,1,11);
  SetGraphStyle(gjesAlframeAfter03,1,11);
  SetGraphStyle(gjesAlframeAfter04,1,12);
  SetGraphStyle(gjesAlframeAfter05,1,10);

  gjesSS310Before->GetXaxis()->SetTitle("Jet E [GeV]");
  gjesSS310Before->GetXaxis()->CenterTitle(1);
  gjesSS310Before->GetYaxis()->SetTitle("Jet Energy Scale");
//  gjesSS310Before->GetYaxis()->SetTitleOffset(1.8);
  gjesSS310Before->GetYaxis()->CenterTitle(1);
  gjesSS310Before->GetXaxis()->SetLimits(0,70.);
  gjesSS310Before->SetMinimum(jesymin); 
  gjesSS310Before->SetMaximum(jesymax); 
  
  gjesSS310Before->Draw("AP");
  //gjesSS310After01->Draw("P");
  //gjesSS310After02->Draw("P");
  gjesSS310After03->Draw("P");
  gjesSS310After04->Draw("P");
  gjesSS310After05->Draw("P");
  gjesAlframeBefore->Draw("P");
  //gjesAlframeAfter01->Draw("P");
  //gjesAlframeAfter02->Draw("P");
  gjesAlframeAfter03->Draw("P");
  //gjesAlframeAfter04->Draw("P");
  gjesAlframeAfter05->Draw("P");
  
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
  latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4");
  latex->DrawLatex(0.2,0.77,"Jet |#eta^{truth}| < 0.6");
  
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
  double jerSS310After01[nEBin];
  double jerSS310After02[nEBin];
  double jerSS310After03[nEBin];
  double jerSS310After04[nEBin];
  double jerSS310After05[nEBin];
  //// Alframe
  double jerAlframeBefore[nEBin];
  double jerAlframeAfter01[nEBin];
  double jerAlframeAfter02[nEBin];
  double jerAlframeAfter03[nEBin];
  double jerAlframeAfter04[nEBin];
  double jerAlframeAfter05[nEBin];
 
  //////////////////// read-in JER files
  ifstream infileJER;  
  //infileJER.open("JER_final_qcd.out");
  infileJER.open(Form("JER_%s.out",inputType.c_str()));
  int ijer=0;
  double tmpjer;
  string szjer;
  if (infileJER.is_open()){
    while( getline (infileJER,szjer) ) {
      tmpjer = atof(szjer.c_str());
      if (ijer <nEBin){
        jerSS310Before[ijer] = tmpjer;
      } else if (ijer < 2*nEBin){
        jerSS310After01[ijer-1*nEBin] = tmpjer;
      } else if (ijer < 3*nEBin){
        jerSS310After02[ijer-2*nEBin] = tmpjer;
      } else if (ijer < 4*nEBin){
        jerSS310After03[ijer-3*nEBin] = tmpjer;
      } else if (ijer < 5*nEBin){
        jerSS310After04[ijer-4*nEBin] = tmpjer;
      } else if (ijer < 6*nEBin){
        jerSS310After05[ijer-5*nEBin] = tmpjer;
      } else if (ijer < 7*nEBin){
        jerAlframeBefore[ijer-6*nEBin] = tmpjer;
      } else if (ijer < 8*nEBin){
        jerAlframeAfter01[ijer-7*nEBin] = tmpjer;
      } else if (ijer < 9*nEBin){
        jerAlframeAfter02[ijer-8*nEBin] = tmpjer;
      } else if (ijer < 10*nEBin){
        jerAlframeAfter03[ijer-9*nEBin] = tmpjer;
      } else if (ijer < 11*nEBin){
        jerAlframeAfter04[ijer-10*nEBin] = tmpjer;
      } else if (ijer < 12*nEBin){
        jerAlframeAfter05[ijer-11*nEBin] = tmpjer;
      } else {
        cout << "ERROR!! in filling jer arrats" << endl; return;
      }
      //cout << "ijer = " << ijer << endl;  
      //cout << "szjer = " << szjer.c_str() << endl;  
      ijer++;
    }
  }
  for (int ie=0; ie<nEBin; ie++){
    cout << "jerSS310Before["<<ie<<"] = " << jerSS310Before[ie] << endl;
    cout << "jerSS310After01["<<ie<<"] = " << jerSS310After01[ie] << endl;
    cout << "jerSS310After02["<<ie<<"] = " << jerSS310After02[ie] << endl;
    cout << "jerSS310After03["<<ie<<"] = " << jerSS310After03[ie] << endl;
    cout << "jerSS310After04["<<ie<<"] = " << jerSS310After04[ie] << endl;
    cout << "jerSS310After05["<<ie<<"] = " << jerSS310After05[ie] << endl;
  }
  for (int ie=0; ie<nEBin; ie++){
    cout << "jerAlframeBefore["<<ie<<"] = " << jerAlframeBefore[ie] << endl;
    cout << "jerAlframeAfter01["<<ie<<"] = " << jerAlframeAfter01[ie] << endl;
    cout << "jerAlframeAfter02["<<ie<<"] = " << jerAlframeAfter02[ie] << endl;
    cout << "jerAlframeAfter03["<<ie<<"] = " << jerAlframeAfter03[ie] << endl;
    cout << "jerAlframeAfter04["<<ie<<"] = " << jerAlframeAfter04[ie] << endl;
    cout << "jerAlframeAfter05["<<ie<<"] = " << jerAlframeAfter05[ie] << endl;
  }
  
  TGraphErrors* gjerSS310Before = new TGraphErrors(nEBin, xPnt, jerSS310Before, ex, ey);
  TGraphErrors* gjerSS310After01 = new TGraphErrors(nEBin, xPnt, jerSS310After01, ex, ey);
  TGraphErrors* gjerSS310After02 = new TGraphErrors(nEBin, xPnt, jerSS310After02, ex, ey);
  TGraphErrors* gjerSS310After03 = new TGraphErrors(nEBin, xPnt, jerSS310After03, ex, ey);
  TGraphErrors* gjerSS310After04 = new TGraphErrors(nEBin, xPnt, jerSS310After04, ex, ey);
  TGraphErrors* gjerSS310After05 = new TGraphErrors(nEBin, xPnt, jerSS310After05, ex, ey);
  TGraphErrors* gjerAlframeBefore = new TGraphErrors(nEBin, xPnt, jerAlframeBefore, ex, ey);
  TGraphErrors* gjerAlframeAfter01 = new TGraphErrors(nEBin, xPnt, jerAlframeAfter01, ex, ey);
  TGraphErrors* gjerAlframeAfter02 = new TGraphErrors(nEBin, xPnt, jerAlframeAfter02, ex, ey);
  TGraphErrors* gjerAlframeAfter03 = new TGraphErrors(nEBin, xPnt, jerAlframeAfter03, ex, ey);
  TGraphErrors* gjerAlframeAfter04 = new TGraphErrors(nEBin, xPnt, jerAlframeAfter04, ex, ey);
  TGraphErrors* gjerAlframeAfter05 = new TGraphErrors(nEBin, xPnt, jerAlframeAfter05, ex, ey);

  SetGraphStyle(gjerSS310Before,0,0);
  SetGraphStyle(gjerSS310After01,0,10);
  SetGraphStyle(gjerSS310After02,0,11);
  SetGraphStyle(gjerSS310After03,0,11);
  SetGraphStyle(gjerSS310After04,0,12);
  SetGraphStyle(gjerSS310After05,0,10);
  
  SetGraphStyle(gjerAlframeBefore,1,0);
  SetGraphStyle(gjerAlframeAfter01,1,10);
  SetGraphStyle(gjerAlframeAfter02,1,11);
  SetGraphStyle(gjerAlframeAfter03,1,11);
  SetGraphStyle(gjerAlframeAfter04,1,12);
  SetGraphStyle(gjerAlframeAfter05,1,10);

  gjerSS310Before->GetXaxis()->SetTitle("Jet E [GeV]");
  gjerSS310Before->GetXaxis()->CenterTitle(1);
  gjerSS310Before->GetYaxis()->SetTitle("Jet Energy Scale");
//  gjerSS310Before->GetYaxis()->SetTitleOffset(1.8);
  gjerSS310Before->GetYaxis()->CenterTitle(1);
  gjerSS310Before->GetXaxis()->SetLimits(0,70.);
  gjerSS310Before->SetMinimum(jerymin); 
  gjerSS310Before->SetMaximum(jerymax); 
  
  gjerSS310Before->Draw("AP");
  //gjerSS310After01->Draw("P");
  //gjerSS310After02->Draw("P");
  gjerSS310After03->Draw("P");
  gjerSS310After04->Draw("P");
  gjerSS310After05->Draw("P");
  gjerAlframeBefore->Draw("P");
  //gjerAlframeAfter01->Draw("P");
  //gjerAlframeAfter02->Draw("P");
  gjerAlframeAfter03->Draw("P");
  //gjerAlframeAfter04->Draw("P");
  gjerAlframeAfter05->Draw("P");

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
  latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4");
  latex->DrawLatex(0.2,0.77,"Jet |#eta^{truth}| < 0.6");
  
  c_JER->SaveAs(Form("outvsE/JERvsE_%s.pdf",inputType.c_str()));
  
  return;
}

