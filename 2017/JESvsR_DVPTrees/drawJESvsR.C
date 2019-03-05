void drawJESvsR(double ptmin = 50, double ptmax = 531, double etacut = 0.6, double dRcut = 0.4, string varType="pt"){

  //// select hist x axis range
  int numbin;
  double binmin, binmax;
  double ymin, ymax; 
  numbin = 50; binmin = 0; binmax = 1.6;
  //numbin = 50; binmin = -1.0; binmax = 0.6;
  ymin =0.; ymax = 0.2;
  
  ///////////////////////////////////////
  //// Get Hist for different pT range
  const int nRBin = 4;
  //Double_t ptBinArr[nRBin+1] = {10., 50., 80.}; 
  cout << "nRBin = " << nRBin << endl;
  int jetCone[nRBin] = {2, 3, 4, 5};
  /*
  Double_t xPnt[nRBin];
  for (int ipt=0; ipt<nRBin; ipt++){
    xPnt[ipt] = (ptBinArr[ipt]+ptBinArr[ipt+1])/2; 
  }
  */
  Double_t xPnt[nRBin];
  for (int ipt=0; ipt<nRBin; ipt++){
    xPnt[ipt] = 0.1*(double*)jetCone[ipt];
  }

  //// hist
  TH1D* hReco01[nRBin];
  TH1D* hReco02[nRBin];
  TH1D* hReco03[nRBin];
  //// Fit params
  double fitmin = binmin+0.5;
  double fitmax = binmax-0.5;
  TF1* f01[nRBin];
  TF1* f02[nRBin];
  TF1* f03[nRBin];
  //// for Drawing
  TCanvas *c01[nRBin];
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.040);
  TLegend* leg = new TLegend(0.63, 0.60, 0.93, 0.80);
  TLegend* leg2 = new TLegend(0.65, 0.65, 0.90, 0.85);
  //// JES and JER
  double mean01[nRBin], mean02[nRBin], mean03[nRBin]; 
  double sigma01[nRBin], sigma02[nRBin], sigma03[nRBin]; 
  double res01[nRBin], res02[nRBin], res03[nRBin]; 
 
  double meanRatio01[nRBin], meanRatio02[nRBin], meanRatio03[nRBin]; 
  double sigmaRatio01[nRBin], sigmaRatio02[nRBin], sigmaRatio03[nRBin]; 
  
  for (int ipt =0; ipt < nRBin; ipt ++) { 
//    cout << "TEST!!!!!!"<< ipt<<"th jetCone = " << jetCone[ipt] << endl;
    hReco01[ipt] = new TH1D(Form("hReco01_%d",ipt),"",numbin,binmin,binmax);
    hReco01[ipt]->Sumw2();
    fillHist(hReco01[ipt],jetCone[ipt],varType,ptmin,ptmax,etacut,dRcut,"DefaultIHCal"); 
    hReco01[ipt]->SetLineColor(kGray+2); //SetHistStyle add!
    hReco01[ipt]->SetMarkerColor(kGray+3);
    //hReco01[ipt]->SetMarkerStyle(kOpenCircle);
    hReco01[ipt]->Scale(1./hReco01[ipt]->Integral());
   
    hReco02[ipt] = new TH1D(Form("hReco02_%d",ipt),"",numbin,binmin,binmax);
    hReco02[ipt]->Sumw2();
    fillHist(hReco02[ipt],jetCone[ipt],varType,ptmin,ptmax,etacut,dRcut,"AlIHCal"); 
    hReco02[ipt]->SetLineColor(kRed+1);
    hReco02[ipt]->SetMarkerColor(kRed+1);
//    hReco02[ipt]->SetMarkerStyle(kOpenCircle);
    hReco02[ipt]->Scale(1./hReco02[ipt]->Integral());
  
    hReco03[ipt] = new TH1D(Form("hReco03[ipt]_%d",ipt),"",numbin,binmin,binmax);
    hReco03[ipt]->Sumw2();
    fillHist(hReco03[ipt],jetCone[ipt],varType,ptmin,ptmax,etacut,dRcut,"AlIHCalNoReadout"); 
    hReco03[ipt]->SetLineColor(kGreen+1);
    hReco03[ipt]->SetMarkerColor(kGreen+2);
//    hReco03[ipt]->SetMarkerStyle(kOpenSquare);
    hReco03[ipt]->Scale(1./hReco03[ipt]->Integral());
 
    //TF1* f01 = new TF1("f01", fGaus,fitmin,fitmax,3);
    f01[ipt] = new TF1(Form("f01_%d",ipt), "gaus",fitmin,fitmax);
    //Double_t par01[3];
    f01[ipt]->SetLineColor(kGray+2);
    f01[ipt]->SetLineStyle(1);
    f02[ipt] = new TF1(Form("f02_%d",ipt), "gaus",fitmin,fitmax);
    //Double_t par02[3];
    f02[ipt]->SetLineColor(kRed+1);
    f02[ipt]->SetLineStyle(2);
    f03[ipt] = new TF1(Form("f03_%d",ipt), "gaus",fitmin,fitmax);
    //Double_t par03[3];
    f03[ipt]->SetLineColor(kGreen+1);
    f03[ipt]->SetLineStyle(4);
  
    //// Fill and Fit 
    c01[ipt] = new TCanvas(Form("c01_%d",ipt),"",600,600);
    c01[ipt]->cd();
/*
    if (varType.compare("pt")==0) { hReco01[ipt]->GetXaxis()->SetTitle("Jet (p_{T}^{reco} - p_{T}^{truth}) / p_{T}^{truth}");}
    else if (varType.compare("eta")==0) { hReco01[ipt]->GetXaxis()->SetTitle("Jet (#eta^{reco}-#eta^{truth})/#eta^{truth}");}
    else if (varType.compare("phi")==0) { hReco01[ipt]->GetXaxis()->SetTitle("Jet (#phi^{reco}-#phi^{truth})/#phi^{truth}");}
    else { cout << "Error: choose among pt, eta, or phi"<<endl; return; }
*/
    if (varType.compare("pt")==0) { hReco01[ipt]->GetXaxis()->SetTitle("Jet p_{T}^{reco} / p_{T}^{truth}");}
    else if (varType.compare("eta")==0) { hReco01[ipt]->GetXaxis()->SetTitle("Jet #eta^{reco} / #eta^{truth}");}
    else if (varType.compare("phi")==0) { hReco01[ipt]->GetXaxis()->SetTitle("Jet #phi^{reco} / #phi^{truth}");}
    else { cout << "Error: choose among pt, eta, or phi"<<endl; return; }
    hReco01[ipt]->GetXaxis()->CenterTitle(1);
    hReco01[ipt]->SetMinimum(ymin);
    hReco01[ipt]->SetMaximum(ymax);
  
    hReco01[ipt]->Draw("pe");
    //f01->SetParLimits(0,parC[0]-0.1,parC[0]+0.1);
    //f01->SetParLimits(1,parC[1]-0.1,parC[1]+0.1);
    //f01->SetParLimits(2,parC[2]-0.1,parC[2]+0.1);
    hReco01[ipt]->Fit(Form("f01_%d",ipt),"N");
    f01[ipt]->Draw("same");
    mean01[ipt] = f01[ipt]->GetParameter(1);
    sigma01[ipt] = f01[ipt]->GetParameter(2);
    res01[ipt] = sigma01[ipt]/mean01[ipt];
    //f01[ipt]->GetParameters(&par01[0]);
  
    hReco02[ipt]->Draw("pe same");
    hReco02[ipt]->Fit(Form("f02_%d",ipt),"N");
    f02[ipt]->Draw("same");
    mean02[ipt] = f02[ipt]->GetParameter(1);
    sigma02[ipt] = f02[ipt]->GetParameter(2);
    res02[ipt] = sigma02[ipt]/mean02[ipt];
//    f02[ipt]->GetParameters(&par02[0]);
  
    hReco03[ipt]->Draw("pe same");
    hReco03[ipt]->Fit(Form("f03_%d",ipt),"N");
    f03[ipt]->Draw("same");
    mean03[ipt] = f03[ipt]->GetParameter(1);
    sigma03[ipt] = f03[ipt]->GetParameter(2);
    res03[ipt] = sigma03[ipt]/mean03[ipt];
//    f03[ipt]->GetParameters(&par03[0]);

    latex->SetTextSize(0.040);
    latex->SetTextColor(kBlack);
    if (ptmax==531) {
      latex->DrawLatex(0.2,0.89,Form("Jet p_{T}^{truth} > %.0f GeV/c, |#eta^{truth}| < %.1f",ptmin,etacut));
    } else {
      latex->DrawLatex(0.2,0.89,Form("%.0f <Jet p_{T}^{truth}< %.0f GeV/c, |#eta^{truth}| < %.1f",ptmin,ptmax,etacut));
    }
    latex->DrawLatex(0.2,0.83,Form("Anti-k_{T} R=0.%d",jetCone[ipt]));
    latex->SetTextSize(0.030);
    latex->SetTextColor(kGray+3);
    latex->DrawLatex(0.2,0.75,Form("#mu=%.2f, #sigma=%.2f",mean01[ipt],sigma01[ipt]));
    latex->SetTextColor(kRed+1);
    latex->DrawLatex(0.2,0.70,Form("#mu=%.2f, #sigma=%.2f",mean02[ipt],sigma02[ipt]));
    latex->SetTextColor(kGreen+2);
    latex->DrawLatex(0.2,0.65,Form("#mu=%.2f, #sigma=%.2f",mean03[ipt],sigma03[ipt]));
    
    if (ipt ==0) {
      leg->AddEntry(hReco01[ipt],"SS301","lp");
      leg->AddEntry(hReco02[ipt],"Al","lp");
      leg->AddEntry(hReco03[ipt],"Al, no readout","lp");
    }
    leg->Draw();
    c01[ipt]->SaveAs(Form("outJESvsR/cone%d_%s_ptmin%.1f_ptmax%.1f_etacut%.1f_dRcut%.1f.pdf",jetCone[ipt],varType.c_str(),ptmin,ptmax,etacut,dRcut));
    
    //// calculate pull
    meanRatio01[ipt] = mean01[ipt]/mean01[ipt];
    meanRatio02[ipt] = mean02[ipt]/mean01[ipt];
    meanRatio03[ipt] = mean03[ipt]/mean01[ipt];
    sigmaRatio01[ipt] = sigma01[ipt]/sigma01[ipt];
    sigmaRatio02[ipt] = sigma02[ipt]/sigma01[ipt];
    sigmaRatio03[ipt] = sigma03[ipt]/sigma01[ipt];
    cout << "meanRatio01["<<ipt<<"] = "<< meanRatio01[ipt]<<endl;
    cout << "meanRatio02["<<ipt<<"] = "<< meanRatio02[ipt]<<endl;
    cout << "meanRatio03["<<ipt<<"] = "<< meanRatio03[ipt]<<endl;
    cout << "sigmaRatio01["<<ipt<<"] = "<< sigmaRatio01[ipt]<<endl;
    cout << "sigmaRatio02["<<ipt<<"] = "<< sigmaRatio02[ipt]<<endl;
    cout << "sigmaRatio03["<<ipt<<"] = "<< sigmaRatio03[ipt]<<endl;
  }
  
  ///////////////////////////////////////
  //// Draw JES
  //TCanvas* c_JES = new TCanvas("c_JES","",800,600);
  TCanvas* c_JES = new TCanvas("c_JES","",600,600);
  c_JES->cd();
  TPad* padJES01 = new TPad("padJES01","",0.,0.5,1.,1.);
  padJES01->SetBottomMargin(0);
//  padJES01->SetGridx();
  padJES01->Draw();
  TPad* padJES02 = new TPad("padJES02","",0.,0.0,1.,0.5);
  padJES02->SetTopMargin(0);
//  padJES02->SetGridx();
  padJES02->Draw();
  
  for (int ipt =0; ipt < nRBin; ipt ++) { 
    cout << "TEST!!!!!!"<< ipt<<"th jetCone = " << jetCone[ipt] << endl;
    cout << "TEST!!!!!!"<< ipt<<"th xPnt = " << xPnt[ipt] << endl;
  }

  TGraphAsymmErrors* gJES01 = new TGraphAsymmErrors(nRBin, xPnt, mean01);
  TGraphAsymmErrors* gJES02 = new TGraphAsymmErrors(nRBin, xPnt, mean02);
  TGraphAsymmErrors* gJES03 = new TGraphAsymmErrors(nRBin, xPnt, mean03);
  
  gJES01->SetLineColor(kGray+2);
  gJES01->SetMarkerColor(kGray+3);
  gJES02->SetLineColor(kRed+1);
  gJES02->SetMarkerColor(kRed+1);
  gJES02->SetMarkerStyle(kOpenCircle);
  gJES03->SetLineColor(kGreen+1);
  gJES03->SetMarkerColor(kGreen+1);

  gJES01->GetXaxis()->SetTitle("Anti-k_{T} R");
  gJES01->GetXaxis()->CenterTitle(1);
  gJES01->GetYaxis()->SetTitle("Scale (#mu)");
//  gJES01->GetYaxis()->SetTitleOffset(1.8);
  gJES01->GetYaxis()->CenterTitle(1);
  gJES01->GetXaxis()->SetLimits(0.1,0.6);
  gJES01->SetMinimum(0.5); 
  gJES01->SetMaximum(1.1); 
  
  padJES01->cd(); 
  gJES01->Draw("AP");
  gJES02->Draw("P");
  gJES03->Draw("P");
  leg2->AddEntry(gJES01,"SS301","lp");
  leg2->AddEntry(gJES02,"Al","lp");
  leg2->AddEntry(gJES03,"Al, no readout","lp");
  leg2->Draw();

  dashedLine(0.1,1.,0.6,1.,4,1);
   
  // JES pull
  TGraphAsymmErrors* gJESRatio01 = new TGraphAsymmErrors(nRBin, xPnt, meanRatio01);
  TGraphAsymmErrors* gJESRatio02 = new TGraphAsymmErrors(nRBin, xPnt, meanRatio02);
  TGraphAsymmErrors* gJESRatio03 = new TGraphAsymmErrors(nRBin, xPnt, meanRatio03);
  
  gJESRatio01->SetLineColor(kGray+2);
  gJESRatio01->SetMarkerColor(kGray+3);
  gJESRatio02->SetLineColor(kRed+1);
  gJESRatio02->SetMarkerColor(kRed+1);
  gJESRatio02->SetMarkerStyle(kOpenCircle);
  gJESRatio03->SetLineColor(kGreen+1);
  gJESRatio03->SetMarkerColor(kGreen+1);

  gJESRatio01->GetXaxis()->SetTitle("Anti-k_{T} R");
  gJESRatio01->GetXaxis()->CenterTitle(1);
  gJESRatio01->GetYaxis()->SetTitle("XX / SS301");
//  gJESRatio01->GetYaxis()->SetTitleOffset(1.8);
  gJESRatio01->GetYaxis()->CenterTitle(1);
  gJESRatio01->GetXaxis()->SetLimits(0.1,0.6);
  gJESRatio01->SetMinimum(0.9); 
  gJESRatio01->SetMaximum(1.1); 
  
  padJES02->cd(); 
  gJESRatio01->Draw("AP");
  gJESRatio02->Draw("P");
  gJESRatio03->Draw("P");
  dashedLine(0.1,1.,0.6,1.,1,1);
  
  c_JES->SaveAs(Form("outJESvsR/JES_cone%d_%s_etacut%.1f_dRcut%.1f.pdf",jetCone,varType.c_str(),etacut,dRcut));

  ///////////////////////////////////////
  //// Draw JER
  TCanvas* c_JER = new TCanvas("c_JER","",600,600);
  c_JER->cd();
  TPad* padJER01 = new TPad("padJER01","",0.,0.5,1.,1.);
  padJER01->SetBottomMargin(0);
//  padJER01->SetGridx();
  padJER01->Draw();
  TPad* padJER02 = new TPad("padJER02","",0.,0.0,1.,0.5);
  padJER02->SetTopMargin(0);
//  padJER02->SetGridx();
  padJER02->Draw();
  
  TGraphAsymmErrors* gJER01 = new TGraphAsymmErrors(nRBin, xPnt, res01);
  TGraphAsymmErrors* gJER02 = new TGraphAsymmErrors(nRBin, xPnt, res02);
  TGraphAsymmErrors* gJER03 = new TGraphAsymmErrors(nRBin, xPnt, res03);
//  TGraphAsymmErrors* gJER01 = new TGraphAsymmErrors(nRBin, xPnt, sigma01);
//  TGraphAsymmErrors* gJER02 = new TGraphAsymmErrors(nRBin, xPnt, sigma02);
//  TGraphAsymmErrors* gJER03 = new TGraphAsymmErrors(nRBin, xPnt, sigma03);
  gJER01->SetLineColor(kGray+2);
  gJER01->SetMarkerColor(kGray+3);
  gJER02->SetLineColor(kRed+1);
  gJER02->SetMarkerColor(kRed+1);
  gJER02->SetMarkerStyle(kOpenCircle);
  gJER03->SetLineColor(kGreen+1);
  gJER03->SetMarkerColor(kGreen+1);
  
  gJER01->GetXaxis()->SetTitle("Anti-k_{T} R");
  gJER01->GetXaxis()->CenterTitle(1);
//  gJER01->GetYaxis()->SetTitle("#sigma/#mu");
  gJER01->GetYaxis()->SetTitle("Resolution (#sigma/#mu)");
//  gJER01->GetYaxis()->SetTitleOffset(1.8);
  gJER01->GetYaxis()->CenterTitle(1);
  gJER01->GetXaxis()->SetLimits(0.1,0.6);
  gJER01->SetMinimum(0.); 
  gJER01->SetMaximum(0.3); 
  
  padJER01->cd();
  gJER01->Draw("AP");
  gJER02->Draw("P");
  gJER03->Draw("P");
  leg2->Draw();
  dashedLine(0.1,0.,0.6,0.,1,1);
  
  // JER pull
  TGraphAsymmErrors* gJERRatio01 = new TGraphAsymmErrors(nRBin, xPnt, sigmaRatio01);
  TGraphAsymmErrors* gJERRatio02 = new TGraphAsymmErrors(nRBin, xPnt, sigmaRatio02);
  TGraphAsymmErrors* gJERRatio03 = new TGraphAsymmErrors(nRBin, xPnt, sigmaRatio03);
  
  gJERRatio01->SetLineColor(kGray+2);
  gJERRatio01->SetMarkerColor(kGray+3);
  gJERRatio02->SetLineColor(kRed+1);
  gJERRatio02->SetMarkerColor(kRed+1);
  gJERRatio02->SetMarkerStyle(kOpenCircle);
  gJERRatio03->SetLineColor(kGreen+1);
  gJERRatio03->SetMarkerColor(kGreen+1);

  gJERRatio01->GetXaxis()->SetTitle("Anti-k_{T} R");
  gJERRatio01->GetXaxis()->CenterTitle(1);
  gJERRatio01->GetYaxis()->SetTitle("XX / SS301");
//  gJERRatio01->GetYaxis()->SetTitleOffset(1.8);
  gJERRatio01->GetYaxis()->CenterTitle(1);
  gJERRatio01->GetXaxis()->SetLimits(0.1,0.6);
  gJERRatio01->SetMinimum(0.8); 
  gJERRatio01->SetMaximum(1.2); 
  
  padJER02->cd();
  gJERRatio01->Draw("AP");
  gJERRatio02->Draw("P");
  gJERRatio03->Draw("P");
  dashedLine(0.1,1.,0.6,1.,1,1);
  
  c_JER->SaveAs(Form("outJESvsR/JER_cone%d_%s_etacut%.1f_dRcut%.1f.pdf",jetCone,varType.c_str(),etacut,dRcut));

  return;
}

//////////////////////////////////////////////
void fillHist(TH1D* hist, int jetCone, string varType, double ptmin, double ptmax, double etacut, double dRcut, string szInput){
  // open file
  TFile* f01 = new TFile(Form("sPHENIX-Sep17-QCD35-10k-%s.root",szInput.c_str()));
  TTree* ttree = (TTree*)f01->Get("ttree");
  int treeEntry = ttree->GetEntries();
  //int treeEntry = 10;

  // set branch address && read
  int truenum;
  float truept[531];
  float trueeta[531];
  float truephi[531];
  int reconum;
  float recopt[531];
  float recoeta[531];
  float recophi[531];
  
  TBranch *b_truenum;
  TBranch *b_truept;
  TBranch *b_trueeta;
  TBranch *b_truephi;
  TBranch *b_reconum;
  TBranch *b_recopt;
  TBranch *b_recoeta;
  TBranch *b_recophi;

  ttree->SetBranchAddress(Form("truthjet%d_n",jetCone),&truenum,&b_truenum);
  ttree->SetBranchAddress(Form("truthjet%d_pt",jetCone),&truept,&b_truept);
  ttree->SetBranchAddress(Form("truthjet%d_eta",jetCone),&trueeta,&b_trueeta);
  ttree->SetBranchAddress(Form("truthjet%d_phi",jetCone),&truephi,&b_truephi);
  ttree->SetBranchAddress(Form("jet%d_n",jetCone),&reconum,&b_reconum);
  ttree->SetBranchAddress(Form("jet%d_pt",jetCone),&recopt,&b_recopt);
  ttree->SetBranchAddress(Form("jet%d_eta",jetCone),&recoeta,&b_recoeta);
  ttree->SetBranchAddress(Form("jet%d_phi",jetCone),&recophi,&b_recophi);
 
  // new variables 
  float dR[531];

  // event loop
  for (int iev=0;iev<treeEntry;iev++){
    //ttree->GetEntry(iev);
    ttree->GetEvent(iev);
   // cout << iev<<"th number of true jet = " << truenum << endl;
    for (int ij=0; ij<truenum; ij++){
      //cout << "  *" << ij << "th var = " << var[ij] << endl;    
      // pt, eta, dR cut
      dR[ij] = TMath::Sqrt( TMath::Power(trueeta[ij]-recoeta[ij],2) + TMath::Power(truephi[ij]-recophi[ij],2) );
      if (! (truept[ij]>ptmin && truept[ij] < ptmax && TMath::Abs(trueeta[ij])<etacut && dR[ij]<dRcut) ) continue;
      
      // fill
/*
      if (varType.compare("pt")==0) {hist->Fill((recopt[ij]-truept[ij])/truept[ij]);}
      else if (varType.compare("eta")==0) {hist->Fill((recoeta[ij]-trueeta[ij])/trueeta[ij]);}
      else if (varType.compare("phi")==0) {hist->Fill((recophi[ij]-truephi[ij])/truephi[ij]);}
      else { cout << "Error: choose among pt, eta, or phi"<<endl; return; }
*/
      if (varType.compare("pt")==0) {hist->Fill(recopt[ij]/truept[ij]);}
      else if (varType.compare("eta")==0) {hist->Fill(recoeta[ij]/trueeta[ij]);}
      else if (varType.compare("phi")==0) {hist->Fill(recophi[ij]/truephi[ij]);}
      else { cout << "Error: choose among pt, eta, or phi"<<endl; return; }
    }
  }
  f01->Close();
  delete f01;
}


//////////////////////////////////////////////
double fGaus(double *x, double *par) {
  return par[0]*exp( -0.5*( ((x[0]-par[1])*(x[0]-par[1]))/(par[2]*par[2]) ) );
}

void solidLine(double x1=0, double y1=0, double x2=0, double y2=0, int color, int width){
  TLine* t1 = new TLine(x1, y1, x2, y2);
  t1->SetLineStyle(1);
  t1->SetLineColor(color);
  t1->SetLineWidth(width);
  t1->Draw();
}

void dashedLine(double x1=0, double y1=0, double x2=0, double y2=0, int color, int width){
  TLine* t1 = new TLine(x1, y1, x2, y2);
  t1->SetLineStyle(7);
  t1->SetLineColor(color);
  t1->SetLineWidth(width);
  t1->Draw();
}
