void comp_JES(double ptmin = 50, double ptmax = 531, double etacut = 0.6, double dRcut = 0.4, string varType="pt"){

  double fitmin = 0.2;
  double fitmax = 1.5;
  
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.040);
  TLegend* leg = new TLegend(0.63, 0.73, 0.93, 0.85);
  
  TFile* f01 = new TFile("./outEMfrac/emfrac_100MeV_Al_NotScaled_v3.root","READ");
  TH2D* h2D_JES_emfrac = (TH2D*)f01->Get("h2D_JES_emfrac");
  TH1D * proj01 =  h2D_JES_emfrac->ProjectionY();
  proj01->Sumw2();
  proj01->Rebin(2);
  proj01->Scale(1./proj01->Integral());

  TFile* f02 = new TFile("./outEMfrac/emfrac_OnlyHcalScaled.root","READ");
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
      
  leg->AddEntry(proj01,"Not Scaled","lp");
  leg->AddEntry(proj02,"Scaled","lp");
  leg->Draw();

  cout << "mean01: " << mean01 << endl; 
  cout << "sigma01: " << sigma01 << endl; 
  cout << "res01: " << res01 << endl; 
  cout << "mean02: " << mean02 << endl; 
  cout << "sigma02: " << sigma02 << endl; 
  cout << "res02: " << res02 << endl; 
  c1->SaveAs("./outEMfrac/JEScomparsion.pdf"); 

  return;
}
