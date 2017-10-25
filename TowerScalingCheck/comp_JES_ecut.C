void comp_JES_ecut(double ptmin = 50, double ptmax = 531, double etacut = 0.6, double dRcut = 0.4, string varType="pt"){

  double fitmin = 0.2;
  double fitmax = 1.5;
  
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.040);
  TLegend* leg = new TLegend(0.63, 0.73, 0.93, 0.85);
  
  TFile* f01 = new TFile("./out_emfrac/emfrac_MyTree_Al_NotScaled.root","READ");
  TH2D* h2D_JES_emfrac = (TH2D*)f01->Get("h2D_JES_emfrac");
  TH1D * proj01 =  h2D_JES_emfrac->ProjectionY();
  proj01->Sumw2();
  proj01->Rebin(2);
  proj01->Scale(1./proj01->Integral());

  TFile* f02 = new TFile("./out_emfrac/emfrac_100MeV_Al_NotScaled_v3.root","READ");
  TH2D* h2D_JES_emfrac = (TH2D*)f02->Get("h2D_JES_emfrac");
  TH1D * proj02 =  h2D_JES_emfrac->ProjectionY();
  proj02->Sumw2();
  proj02->Rebin(2);
  proj02->Scale(1./proj02->Integral());
  
  TF1* gaus01 = new TF1("gaus01","gaus",fitmin,fitmax);
  //Double_t par01[3];
  gaus01->SetLineColor(kGray+2);
  gaus01->SetLineStyle(1);
  TF2* gaus02 = new TF1("gaus02","gaus",fitmin,fitmax);
  //Double_t par02[3];
  gaus02->SetLineColor(kRed+1);
  gaus02->SetLineStyle(2);
  
  TCanvas*c1 = new TCanvas("c1","",500,500);
  SetHistStyle(proj01,0,0);
  SetHistStyle(proj02,1,0);
  proj01->GetXaxis()->CenterTitle(1);
  proj01->SetMaximum(0.2);
  proj01->Draw("pe");
  proj02->Draw("pe same");
  proj01->Fit("gaus01","N");
  gaus01->Draw("same");
  proj02->Fit("gaus02","N");
  gaus02->Draw("same");
  
  float mean01 = gaus01->GetParameter(1);
  float sigma01 = gaus01->GetParameter(2);
  float res01 = sigma01/mean01;
  float mean02 = gaus02->GetParameter(1);
  float sigma02 = gaus02->GetParameter(2);
  float res02 = sigma02/mean02;
    
  latex->SetTextSize(0.040);
  latex->SetTextColor(kBlack);
  latex->DrawLatex(0.2,0.89,"Jet E^{Truth} > 50 GeV/c, |#eta^{Truth}| < 0.6");
  latex->DrawLatex(0.2,0.83,"Anti-k_{T} R=0.4");
  latex->SetTextSize(0.030);
  latex->SetTextColor(kGray+3);
  latex->DrawLatex(0.2,0.76,Form("#mu=%.2f, #sigma=%.2f, #sigma/#mu=%.2f",mean01,sigma01,res01));
  latex->SetTextColor(kRed+1);
  latex->DrawLatex(0.2,0.72,Form("#mu=%.2f, #sigma=%.2f, #sigma/#mu=%.2f",mean02,sigma02,res02));
      
  leg->AddEntry(proj01,"No cut","lp");
  leg->AddEntry(proj02,"100 MeV cut","lp");
  leg->Draw();

  cout << "mean01: " << mean01 << endl; 
  cout << "sigma01: " << sigma01 << endl; 
  cout << "res01: " << res01 << endl; 
  cout << "mean02: " << mean02 << endl; 
  cout << "sigma02: " << sigma02 << endl; 
  cout << "res02: " << res02 << endl; 
  c1->SaveAs("./out_emfrac/JEScomparsion_ecut.pdf"); 

  return; 
/*
    //hReco01[ipt]->SetMinimum(ymin);
    //hReco01[ipt]->SetMaximum(ymax);
  
    
    if (ipt ==0) {
      leg->AddEntry(hReco03[ipt],"Al, no readout","lp");
    }
    c01[ipt]->SaveAs(Form("OutJetvsR/cone%d_%s_ptmin%.1f_ptmax%.1f_etacut%.1f_dRcut%.1f.pdf",jetCone[ipt],varType.c_str(),ptmin,ptmax,etacut,dRcut));
    
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
  
  c_JES->SaveAs(Form("OutJetvsR/JES_cone%d_%s_etacut%.1f_dRcut%.1f.pdf",jetCone,varType.c_str(),etacut,dRcut));

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
  
  c_JER->SaveAs(Form("OutJetvsR/JER_cone%d_%s_etacut%.1f_dRcut%.1f.pdf",jetCone,varType.c_str(),etacut,dRcut));
*/

  return;
}
