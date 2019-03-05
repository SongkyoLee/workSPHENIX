void trial_EHAsymm_Jet4_50GeV_Al(string ihcalType = "Al",
                            string jetE="50GeV",
                            string outDir01="outJet4Matched", 
                            string outDir02="outJet4_2DEHAsymm",
                            int initfile=0,
                            int endfile =1000)
{
  //// load macros
  gROOT->LoadMacro("rootAna_matching_jet4.C");
  gROOT->LoadMacro("draw_2D_EHAsymm.C");

  //// basic setting
  double xmin = 1.15;
  double xmax = 1.40;
  double ymin = -0.05;
  double ymax = 0.05;

  const int ntr = 8;
  float c_tot=1.00;
  float c_em=1.00;
  float c_h[ntr]={1.00, 1.20, 1.25, 1.26, 1.27, 1.28, 1.29, 1.35};
  float c_ih = 7.18;
  float param[ntr];
  float error[ntr];

  //// trials
  for (int it=0; it<ntr; it++){
    rootAna_matching_jet4(ihcalType.c_str(),jetE.c_str(),outDir01.c_str(),initfile,endfile,c_tot,c_em,c_h[it],c_ih);
    param[it] = draw_2D_EHAsymm(ihcalType.c_str(),jetE.c_str(),outDir01.c_str(),outDir02.c_str(),initfile,endfile,c_tot,c_em,c_h[it],c_ih);
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
  c01->SaveAs(Form("%s%s/slope_vs_cH_%s.pdf",outDir02.c_str(),jetE.c_str(),ihcalType.c_str()));

  return;
}
