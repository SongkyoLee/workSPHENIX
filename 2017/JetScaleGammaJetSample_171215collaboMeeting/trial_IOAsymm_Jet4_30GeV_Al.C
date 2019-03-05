void trial_IOAsymm_Jet4_30GeV_Al(string ihcalType = "Al",
                            string jetE="30GeV",
                            string outDir01="outJet4Matched", 
                            string outDir02="outJet4_2DIOAsymm",
                            int initfile=0,
                            int endfile =1000)
{
  //// load macros
  gROOT->LoadMacro("rootAna_matching_jet4.C");
  gROOT->LoadMacro("draw_2D_IOAsymm.C");
  
  //// basic setting
  double xmin = 7.3;
  double xmax = 8.2;
  double ymin = -0.05;
  double ymax = 0.05;

  const int ntr = 11;
  float c_tot=1.00;
  float c_em=1.00;
  float c_h = 1.00;
  float c_ih[ntr]={1.00, 7.0, 7.50, 7.70, 7.71, 7.72, 7.73, 7.74, 7.75, 7.80, 8.00};
  float param[ntr];
  float error[ntr];

  //// trials
  for (int it=0; it<ntr; it++){
    rootAna_matching_jet4(ihcalType.c_str(),jetE.c_str(),outDir01.c_str(),initfile,endfile,c_tot,c_em,c_h,c_ih[it]);
    param[it] = draw_2D_IOAsymm(ihcalType.c_str(),jetE.c_str(),outDir01.c_str(),outDir02.c_str(),initfile,endfile,c_tot,c_em,c_h,c_ih[it]);
    error[it] = 0.;
    cout << it<<"th param = " << param[it] << endl;
  }
  
  //// pol1 fits 
  TGraphErrors* gr = new TGraphErrors(ntr, c_ih, param, error, error);
  cout << "gr = " << gr << endl;
  SetGraphStyle(gr,0,0);
  gr->GetXaxis()->SetTitle("IHCAL scale ( c_{IH} )");
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
  c01->SaveAs(Form("%s%s/slope_vs_cIH_%s.pdf",outDir02.c_str(),jetE.c_str(),ihcalType.c_str()));

  return;
}
