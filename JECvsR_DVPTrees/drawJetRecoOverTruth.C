void drawJetRecoOverTruth(int jetCone =4, double etacut = 0.6, double dRcut = 0.4, string varType="pt"){

  //// select jet and varialbes
  string szCone = Form("%d",jetCone);
  string szVar = "jet" + szCone + "_" + varType;
  cout << "******* szVar : " << szVar.c_str() << endl;
 
  //// select hist x axis range
  int numbin;
  double binmin, binmax;
  //numbin = 100; binmin = 0; binmax = 1.5;
  numbin = 60; binmin = 0; binmax = 1.6;
 
  ///////////////////////////////////////
  //// Get Hist for different pT range
  const int nPtBin = 2;
  Double_t ptBinArr[nPtBin+1] = {10., 50., 80.}; 
  //Double_t ptBinArr[nPtBin+1] = {50., 70.}; 
//  const int nPtBin = sizeof(ptBinArr)/sizeof(double)-1;
  cout << "nPtBin = " << nPtBin << endl;
  Double_t xPnt[nPtBin];
  for (int ipt=0; ipt<nPtBin; ipt++){
    xPnt[ipt] = (ptBinArr[ipt]+ptBinArr[ipt+1])/2; 
  }

  //// hist
  TH1D* hReco01[nPtBin];
  TH1D* hReco02[nPtBin];
  TH1D* hReco03[nPtBin];
  //// Fit params
  double fitmin = 0.5;
  double fitmax = 1.2;
  TF1* f01[nPtBin];
  TF1* f02[nPtBin];
  TF1* f03[nPtBin];
  //// for Drawing
  TCanvas *c01[nPtBin];
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.040);
  TLegend* leg = new TLegend(0.63, 0.73, 0.93, 0.93);
  //// JES and JER
  double mean01[nPtBin], mean02[nPtBin], mean03[nPtBin]; 
  double sigma01[nPtBin], sigma02[nPtBin], sigma03[nPtBin]; 
  double res01[nPtBin], res02[nPtBin], res03[nPtBin]; 
 
  for (int ipt =0; ipt < nPtBin; ipt ++) { 
    hReco01[ipt] = new TH1D(Form("hReco01_%d",ipt),"",numbin,binmin,binmax);
    hReco01[ipt]->Sumw2();
    fillHist(hReco01[ipt],jetCone,varType,ptBinArr[ipt],ptBinArr[ipt+1],etacut,dRcut,"DefaultIHCal"); 
    hReco01[ipt]->SetLineColor(kGray+2); //SetHistStyle add!
    hReco01[ipt]->SetMarkerColor(kGray+3);
    //hReco01[ipt]->SetMarkerStyle(kOpenCircle);
    hReco01[ipt]->Scale(1./hReco01[ipt]->Integral());
   
    hReco02[ipt] = new TH1D(Form("hReco02_%d",ipt),"",numbin,binmin,binmax);
    hReco02[ipt]->Sumw2();
    fillHist(hReco02[ipt],jetCone,varType,ptBinArr[ipt],ptBinArr[ipt+1],etacut,dRcut,"AlIHCal"); 
    hReco02[ipt]->SetLineColor(kRed+1);
    hReco02[ipt]->SetMarkerColor(kRed+1);
//    hReco02[ipt]->SetMarkerStyle(kOpenCircle);
    hReco02[ipt]->Scale(1./hReco02[ipt]->Integral());
  
    hReco03[ipt] = new TH1D(Form("hReco03[ipt]_%d",ipt),"",numbin,binmin,binmax);
    hReco03[ipt]->Sumw2();
    fillHist(hReco03[ipt],jetCone,varType,ptBinArr[ipt],ptBinArr[ipt+1],etacut,dRcut,"AlIHCalNoReadout"); 
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
    if (varType.compare("pt")==0) { hReco01[ipt]->GetXaxis()->SetTitle("Jet p_{T}^{reco}/p_{T}^{truth}");}
    else if (varType.compare("eta")==0) { hReco01[ipt]->GetXaxis()->SetTitle("Jet #eta^{reco}/#eta^{truth}");}
    else if (varType.compare("phi")==0) { hReco01[ipt]->GetXaxis()->SetTitle("Jet #phi^{reco}/#phi^{truth}");}
    else { cout << "Error: choose among pt, eta, or phi"<<endl; return; }
    hReco01[ipt]->GetXaxis()->CenterTitle(1);
    hReco01[ipt]->SetMaximum(0.16);
  
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
    latex->DrawLatex(0.2,0.87,Form("%.0f <Jet p_{T}^{truth}< %.0f GeV/c",ptBinArr[ipt],ptBinArr[ipt+1]));
    latex->SetTextSize(0.035);
    latex->DrawLatex(0.2,0.81,Form("R=0.%d, |#eta| < %.1f",jetCone,etacut));
    latex->DrawLatex(0.2,0.75,Form("01) #mu=%.2f, #sigma=%.2f",mean01[ipt],sigma01[ipt]));
    latex->DrawLatex(0.2,0.70,Form("02) #mu=%.2f, #sigma=%.2f",mean02[ipt],sigma02[ipt]));
    latex->DrawLatex(0.2,0.65,Form("03) #mu=%.2f, #sigma=%.2f",mean03[ipt],sigma03[ipt]));
    
    if (ipt ==0) {
      leg->AddEntry(hReco01[ipt],"SS301","lp");
      leg->AddEntry(hReco02[ipt],"Al","lp");
      leg->AddEntry(hReco03[ipt],"Al, no readout","lp");
    }
    leg->Draw();
    c01[ipt]->SaveAs(Form("OutJetRecoOverTruth/cone%d_%s_ptmin%.1f_ptmax%.1f_etacut%.1f_dRcut%.1f.pdf",jetCone,varType.c_str(),ptBinArr[ipt],ptBinArr[ipt+1],etacut,dRcut));
  }
  
  ///////////////////////////////////////
  //// Draw JES and JER
  TCanvas* c_JES = new TCanvas("c_JES","",600,600);
  c_JES->cd();
  
  TGraphAsymmErrors* gJES01 = new TGraphAsymmErrors(nPtBin, xPnt, mean01);
  TGraphAsymmErrors* gJES02 = new TGraphAsymmErrors(nPtBin, xPnt, mean02);
  TGraphAsymmErrors* gJES03 = new TGraphAsymmErrors(nPtBin, xPnt, mean03);
  gJES01->SetLineColor(kGray+2);
  gJES01->SetMarkerColor(kGray+3);
  gJES02->SetLineColor(kRed+1);
  gJES02->SetMarkerColor(kRed+1);
  gJES02->SetMarkerStyle(kOpenCircle);
  gJES03->SetLineColor(kGreen+1);
  gJES03->SetMarkerColor(kGreen+1);

  gJES01->GetXaxis()->SetTitle("Jet p_{T}^{truth} [GeV/c]");
  gJES01->GetXaxis()->CenterTitle(1);
  gJES01->GetYaxis()->SetTitle("#mu");
  gJES01->GetYaxis()->CenterTitle(1);
  gJES01->GetXaxis()->SetLimits(0.,90);
  gJES01->SetMinimum(0.5); 
  gJES01->SetMaximum(1.0); 
  gJES01->Draw("AP");
  gJES02->Draw("P");
  gJES03->Draw("P");
  leg->Draw();
    
  c_JES->SaveAs(Form("OutJetRecoOverTruth/JES_cone%d_%s_etacut%.1f_dRcut%.1f.pdf",jetCone,varType.c_str(),etacut,dRcut));
  
  TCanvas* c_JER = new TCanvas("c_JER","",600,600);
  c_JER->cd();
  
  TGraphAsymmErrors* gJER01 = new TGraphAsymmErrors(nPtBin, xPnt, res01);
  TGraphAsymmErrors* gJER02 = new TGraphAsymmErrors(nPtBin, xPnt, res02);
  TGraphAsymmErrors* gJER03 = new TGraphAsymmErrors(nPtBin, xPnt, res03);
  gJER01->SetLineColor(kGray+2);
  gJER01->SetMarkerColor(kGray+3);
  gJER02->SetLineColor(kRed+1);
  gJER02->SetMarkerColor(kRed+1);
  gJER02->SetMarkerStyle(kOpenCircle);
  gJER03->SetLineColor(kGreen+1);
  gJER03->SetMarkerColor(kGreen+1);
  
  gJER01->GetXaxis()->SetTitle("Jet p_{T}^{truth} [GeV/c]");
  gJER01->GetXaxis()->CenterTitle(1);
  gJER01->GetYaxis()->SetTitle("#sigma/#mu");
  gJER01->GetYaxis()->CenterTitle(1);
  gJER01->GetXaxis()->SetLimits(0.,90);
  gJER01->SetMinimum(0.); 
  gJER01->SetMaximum(0.3); 
  gJER01->Draw("AP");
  gJER02->Draw("P");
  gJER03->Draw("P");
  leg->Draw();
  
  c_JER->SaveAs(Form("OutJetRecoOverTruth/JER_cone%d_%s_etacut%.1f_dRcut%.1f.pdf",jetCone,varType.c_str(),etacut,dRcut));

//  gPad->SetLogy(1); 

  return;
}

//////////////////////////////////////////////
/*
void fillHist(TH1D* hist, int jetCone, string varType, string szInput){
      
  if (varType.compare("rad")==0) {cout << "Error: choose among pt, eta, or phi"<<endl; return; }
  // open file
  TFile* f01 = new TFile(Form("sPHENIX-Sep17-QCD35-10k-%s.root",szInput.c_str()));
  TTree* ttree = (TTree*)f01->Get("ttree");
  int treeEntry = ttree->GetEntries();
  //int treeEntry = 10;

  // set branch address && read
  int num;
  float var[531];
  TBranch *b_num;
  TBranch *b_var;

  ttree->SetBranchAddress(Form("jet%d_n",jetCone),&num,&b_num);
  ttree->SetBranchAddress(Form("jet%d_%s",jetCone,varType.c_str()),&var,&b_var);

  // fill
  for (int iev=0;iev<treeEntry;iev++){
    ttree->GetEntry(iev);
   // cout << iev<<"th number of jet = " << num << endl;
    for (int ij=0; ij<num; ij++){
      //cout << "  *" << ij << "th var = " << var[ij] << endl;    
      hist->Fill(var[ij]);
    }
  }
  f01->Close();
  delete f01;
}
*/

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
    ttree->GetEntry(iev);
   // cout << iev<<"th number of true jet = " << truenum << endl;
    for (int ij=0; ij<truenum; ij++){
      //cout << "  *" << ij << "th var = " << var[ij] << endl;    
      // pt, eta, dR cut
      dR[ij] = TMath::Sqrt( TMath::Power(trueeta[ij]-recoeta[ij],2) + TMath::Power(truephi[ij]-recophi[ij],2) );
      if (! (truept[ij]>ptmin && truept[ij] < ptmax && TMath::Abs(trueeta[ij])<etacut && dR[ij]<dRcut) ) continue;
      
      // fill
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
