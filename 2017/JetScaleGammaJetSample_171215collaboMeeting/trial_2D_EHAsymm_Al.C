void trial_2D_EHAsymm_Al(string ihcalType = "Al",
                            string outDir01="outMatched", 
                            string outDir02="out2D_EHAsymm",
                            int initfile=0,
                            int endfile =1000)
{
  //// load macros
  gROOT->LoadMacro("rootAna_matching.C");
  gROOT->LoadMacro("draw_2D_EHAsymm.C");

  //// basic setting
  //if (ihcalType.compare("gammajet")==0){}
  double xmin = 1.05;
  double xmax = 1.35;
  double ymin = -0.05;
  double ymax = 0.05;

  const int ntr = 10;
  float c_tot=1.00;
  float c_em=1.00;
  float c_h[ntr]={1.00, 1.10, 1.15, 1.18, 1.19, 1.20, 1.21, 1.22, 1.25, 1.30};
  float c_ih = 7.73;
  float param[ntr];
  float error[ntr];

  //// trials
  for (int it=0; it<ntr; it++){
    rootAna_matching(ihcalType.c_str(),outDir01.c_str(),initfile,endfile,c_tot,c_em,c_h[it],c_ih);
    param[it] = draw_2D_EHAsymm(ihcalType.c_str(),outDir01.c_str(),outDir02.c_str(),initfile,endfile,c_tot,c_em,c_h[it],c_ih);
    error[it] = 0.;
    cout << it<<"th param = " << param[it] << endl;
  }
  
  //// pol1 fits 
  TGraphErrors* gr = new TGraphErrors(ntr, c_h, param, error, error);
  cout << "gr = " << gr << endl;
  SetGraphStyle(gr,0,0);
  gr->GetXaxis()->SetTitle("HCAL scale ( c_{H} )");
  gr->GetYaxis()->SetTitle("Slope (p1)");
  gr->GetXaxis()->CenterTitle(1);
  gr->GetYaxis()->CenterTitle(1);
  gr->GetXaxis()->SetLimits(xmin,xmax);
  gr->SetMinimum(ymin);
  gr->SetMaximum(ymax);

  TF1 *fit = new TF1("fit","pol1", xmin, xmax); 
  gr->Fit(fit,"NR");
  fit->SetLineColor(kRed);

  TCanvas* c01 = new TCanvas("c01","c01",500,500);
  gr->Draw("ap");
  fit->Draw("same");
  dashedLine(xmin,0,xmax,0);
      
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.050);
  latex->DrawLatex(0.2,0.88,ihcalType.c_str());
  latex->DrawLatex(0.2,0.81,Form("Scale = %.2f",-(fit->GetParameter(0)/fit->GetParameter(1))));
  cout << "scale = "<<-(fit->GetParameter(0)/fit->GetParameter(1))<<endl;  
  c01->SaveAs(Form("%s/slope_vs_cH_%s.pdf",outDir02.c_str(),ihcalType.c_str()));

  return;
}
