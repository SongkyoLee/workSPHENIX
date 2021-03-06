void decompose_HI(string ihcalType="Al", string note1st="04fm", string note2nd="R02", int nTerm = 0){
  
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.038);
  
  TLegend* leg1 = new TLegend(0.18, 0.18, 0.40, 0.33);
  //leg1->SetTextSize(0.045);
  leg1->SetTextSize(0.025);

  TFile* fin01 =  new TFile(Form("../z_20180912_wTot_pp_Lorentz/outvsE/JESvsE_%s_%s.root",ihcalType.c_str(),note2nd.c_str()),"READ");
  cout << fin01 << endl;
  if(!fin01->IsOpen()) { cout <<"WARNING!! fin01 not found"<<endl; return;};
  TFile* fin02 =  new TFile(Form("../z_20180912_wTot_HIJet_Lorentz/outvsE/JESvsE_%s_%s%s.root",ihcalType.c_str(),note1st.c_str(),note2nd.c_str()),"READ");
  if(!fin02->IsOpen()) { cout <<"WARNING!! fin02 not found"<<endl; return;};
  
  TGraphAsymmErrors* pp_before = (TGraphAsymmErrors*)fin01->Get("gjerBefore");
  TGraphAsymmErrors* pp_after = (TGraphAsymmErrors*)fin01->Get("gjerAfter_0");
  TGraphAsymmErrors* hi_before = (TGraphAsymmErrors*)fin02->Get("gjerBefore");
  TGraphAsymmErrors* hi_after = (TGraphAsymmErrors*)fin02->Get("gjerAfter_0");
 
  ////// HI-pp to double-check decomposition  
  int npnt =  pp_before->GetN();
  double subt_x[npnt];
  double hi_y_before[npnt];
  double hi_y_after[npnt];
  double pp_y_before[npnt];
  double pp_y_after[npnt];
  double subt_y_before[npnt];
  double subt_y_after[npnt];
  double subt_x_err[npnt];
  double subt_y_err[npnt];
  cout << "pp_before->GetN()" << pp_before->GetN() <<endl;
  
  for (int ip=0; ip<npnt; ip++) {
    pp_before->GetPoint(ip,subt_x[ip],pp_y_before[ip]);
    hi_before->GetPoint(ip,subt_x[ip],hi_y_before[ip]);
    pp_after->GetPoint(ip,subt_x[ip],pp_y_after[ip]);
    hi_after->GetPoint(ip,subt_x[ip],hi_y_after[ip]);
//    cout << "*****" << ip << endl;
//    cout << "x = " << subt_x[ip]<<endl;
//    cout << "pp_y_before = " << pp_y_before[ip]<<endl;
//    cout << "hi_y_before = " << hi_y_before[ip]<<endl;
    subt_y_before[ip] = sqrt(pow(hi_y_before[ip],2)-pow(pp_y_before[ip],2)); 
    subt_y_after[ip] = sqrt(pow(hi_y_after[ip],2)-pow(pp_y_after[ip],2)); 
    subt_x_err[ip]=5;
    subt_y_err[ip]=0;
//    subt_x[ip] = hi_before->GetX(ip+1);
//    subt_y_before[ip] = hi_before->GetY(ip+1)-pp_before->GetY(ip+1);
  }
  TGraphAsymmErrors* subt_before = new TGraphAsymmErrors(npnt,subt_x,subt_y_before,subt_x_err,subt_x_err,subt_y_err,subt_y_err);
  TGraphAsymmErrors* subt_after = new TGraphAsymmErrors(npnt,subt_x,subt_y_after,subt_x_err,subt_x_err,subt_y_err,subt_y_err);
  
  SetSoftGraphStyle(pp_before,1,11);
  SetSoftGraphStyle(pp_after,1,10);
  SetSoftGraphStyle(hi_before,2,11);
  SetSoftGraphStyle(hi_after,2,10);
  SetSoftGraphStyle(subt_before,3,1);
  SetSoftGraphStyle(subt_after,4,0);
  
  TCanvas* c1 = new TCanvas("c1","c1",600,600);
  if(note2nd.compare("R04")==0) pp_before->SetMaximum(0.6);
  else pp_before->SetMaximum(0.6);
  pp_before->Draw("ap"); 
  hi_before->Draw("p"); 
  pp_after->Draw("p"); 
  hi_after->Draw("p"); 

  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->SetTextSize(0.040);
  //latex->DrawLatex(0.2,0.82,"PYTHIA8+HIJING b = 4-8 fm dijet"); 
  if (note2nd.compare("R02")==0) { latex->DrawLatex(0.2,0.82,"anti-k_{T} R=0.2, |#eta| < 0.45"); }
  else if (note2nd.compare("R04")==0) { latex->DrawLatex(0.2,0.82,"anti-k_{T} R=0.4, |#eta| < 0.45"); }
  else { cout << "WARNING!! R02 or R04"<<endl; return; };
  if (ihcalType.compare("Al")==0) { latex->DrawLatex(0.2,0.77,"Instrumented Al IHCAL");  }
  else if (ihcalType.compare("Alframe")==0) { latex->DrawLatex(0.2,0.77,"Uninstrumented Aluminum");  }
  else if (ihcalType.compare("SS310")==0) { latex->DrawLatex(0.2,0.77,"Uninstrumented Steel");  }
  else { cout << "WARNING!! Al, Alframe, or SS310"<<endl; return; };

/*
  leg1->AddEntry(pp_before,"p+p before calib. ","lp");
  leg1->AddEntry(pp_after,"p+p after calib. ","lp");
  leg1->AddEntry(hi_before,"Au+Au before calib. ","lp");
  leg1->AddEntry(hi_after,"Au+Au after calib. ","lp");
  leg1->Draw(); 
*/

  string sz_term;
  if (nTerm==0 || nTerm==1) { sz_term = "sqrt( pow([0]/sqrt(x),2) )"; }
  else if (nTerm==2) { sz_term = "sqrt( pow([0]/sqrt(x),2) + pow([1],2) )"; }
  else if (nTerm==3) { sz_term = "sqrt( pow([2]/x,2) + pow([0]/sqrt(x),2) + pow([1],2) )";}
  else { cout << "collect term b/w 0, 1, 2, and 3" << endl; return; }

  TF1* func_pp_before = new TF1("func_pp_before",sz_term.c_str(),20,60);
  func_pp_before->SetLineColor(kRed-7);
  func_pp_before->SetLineStyle(1);
  if (nTerm >=1) {
    func_pp_before->SetParameter(0, 1.20 );
    func_pp_before->SetParLimits(0, 0.0, 7.00 );
  }
  if (nTerm >=2) {
    func_pp_before->SetParameter(1, 0.1 );
    func_pp_before->SetParLimits(1, 0.0, 0.50 );
  }
  if (nTerm >=3) {
    func_pp_before->SetParameter(2, 1.20 );
    func_pp_before->SetParLimits(2, 0.0, 7.00 );
  }
  if(nTerm!=0) pp_before->Fit(func_pp_before,"NR");
  if(nTerm!=0) func_pp_before->Draw("same"); 
  double param_pp_before_0;
  double param_pp_before_1;
  double param_pp_before_2;
  if (nTerm >=1) param_pp_before_0 = func_pp_before->GetParameter(0);
  if (nTerm >=2) param_pp_before_1  = func_pp_before->GetParameter(1);
  if (nTerm >=3) param_pp_before_2  = func_pp_before->GetParameter(2);
  
  TF1* func_pp_after = new TF1("func_pp_after",sz_term.c_str(),20,60);
  func_pp_after->SetLineColor(kRed-7);
  func_pp_after->SetLineStyle(2);
  if (nTerm >=1) {
    func_pp_after->SetParameter(0, 1.20 );
    func_pp_after->SetParLimits(0, 0.0, 7.00 );
  }
  if (nTerm >=2) {
    func_pp_after->SetParameter(1, 0.1 );
    func_pp_after->SetParLimits(1, 0.0, 0.50 );
  }
  if (nTerm >=3) {
    func_pp_after->SetParameter(2, 1.20 );
    func_pp_after->SetParLimits(2, 0.0, 7.00 );
  }
  if(nTerm!=0) pp_after->Fit(func_pp_after,"NR");
  if(nTerm!=0) func_pp_after->Draw("same"); 
  double param_pp_after_0;
  double param_pp_after_1;
  double param_pp_after_2;
  if (nTerm >=1) param_pp_after_0 = func_pp_after->GetParameter(0);
  if (nTerm >=2) param_pp_after_1  = func_pp_after->GetParameter(1);
  if (nTerm >=3) param_pp_after_2  = func_pp_after->GetParameter(2);

  TF1* func_hi_before = new TF1("func_hi_before",sz_term.c_str(),20,60);
  func_hi_before->SetLineColor(kAzure+1);
  func_hi_before->SetLineStyle(1);
  if (nTerm >=1) {
    func_hi_before->SetParameter(0, 1.20 );
    func_hi_before->SetParLimits(0, 0.0, 7.00 );
  }
  if (nTerm >=2) {
    func_hi_before->SetParameter(1, 0.1 );
    func_hi_before->SetParLimits(1, 0.0, 0.50 );
  }
  if (nTerm >=3) {
    func_hi_before->SetParameter(2, 1.20 );
    func_hi_before->SetParLimits(2, 0.0, 7.00 );
  }
  if(nTerm!=0) hi_before->Fit(func_hi_before,"NR");
  if(nTerm!=0) func_hi_before->Draw("same"); 
  double param_hi_before_0;
  double param_hi_before_1;
  double param_hi_before_2;
  if (nTerm >=1) param_hi_before_0 = func_hi_before->GetParameter(0);
  if (nTerm >=2) param_hi_before_1  = func_hi_before->GetParameter(1);
  if (nTerm >=3) param_hi_before_2  = func_hi_before->GetParameter(2);
  
  TF1* func_hi_after = new TF1("func_hi_after",sz_term.c_str(),20,60);
  func_hi_after->SetLineColor(kAzure+1);
  func_hi_after->SetLineStyle(2);
  func_hi_after->SetParameter(0, 1.20 );
  func_hi_after->SetParameter(1, 0.1 );
  func_hi_after->SetParameter(2, 1.20 );
  func_hi_after->SetParLimits(0, 0.0, 7.00 );
  func_hi_after->SetParLimits(1, 0.0, 0.50 );
  func_hi_after->SetParLimits(2, 0.0, 7.00 );
  if (nTerm >=1) {
    func_hi_after->SetParameter(0, 1.20 );
    func_hi_after->SetParLimits(0, 0.0, 7.00 );
  }
  if (nTerm >=2) {
    func_hi_after->SetParameter(1, 0.1 );
    func_hi_after->SetParLimits(1, 0.0, 0.50 );
  }
  if (nTerm >=3) {
    func_hi_after->SetParameter(2, 1.20 );
    func_hi_after->SetParLimits(2, 0.0, 7.00 );
  }
  if(nTerm!=0) hi_after->Fit(func_hi_after,"NR");
  if(nTerm!=0) func_hi_after->Draw("same"); 
  double param_hi_after_0;
  double param_hi_after_1;
  double param_hi_after_2;
  if (nTerm >=1) param_hi_after_0 = func_hi_after->GetParameter(0);
  if (nTerm >=2) param_hi_after_1  = func_hi_after->GetParameter(1);
  if (nTerm >=3) param_hi_after_2  = func_hi_after->GetParameter(2);

  if (nTerm==0) {  
    leg1->AddEntry(pp_before,"p+p before calibration","lp");
    leg1->AddEntry(pp_after,"p+p after calibration","lp");
    if (note1st.compare("04fm")==0) {
      leg1->AddEntry(hi_before,"Au+Au (0-4 fm) before calibration","lp");
      leg1->AddEntry(hi_after,"Au+Au (0-4 fm) after calibration","lp");
    } else if (note1st.compare("48fm")==0) {
      leg1->AddEntry(hi_before,"Au+Au (4-8 fm) before calibration","lp");
      leg1->AddEntry(hi_after,"Au+Au (4-8 fm) after calibration","lp");
    }
  }
  else if (nTerm==1) {  
    leg1->AddEntry(func_pp_before,Form("p+p before:  %.0f%% / #sqrt{(p_{T}[GeV])}",100*param_pp_before_0),"l");
    leg1->AddEntry(func_pp_after,Form("p+p after:  %.0f%% / #sqrt{(p_{T}[GeV])}",100*param_pp_after_0),"l");
    if (note1st.compare("04fm")==0) {
      leg1->AddEntry(func_hi_before,Form("Au+Au (0-4 fm) before:  %.0f%% / #sqrt{(p_{T}[GeV])}",100*param_hi_before_0),"l");
      leg1->AddEntry(func_hi_after,Form("Au+Au (0-4 fm) after:  %.0f%% / #sqrt{(p_{T}[GeV])}",100*param_hi_after_0),"l");
    } else if (note1st.compare("48fm")==0) {
      leg1->AddEntry(func_hi_before,Form("Au+Au (4-8 fm) before:  %.0f%% / #sqrt{(p_{T}[GeV])}",100*param_hi_before_0),"l");
      leg1->AddEntry(func_hi_after,Form("Au+Au (4-8 fm) after:  %.0f%% / #sqrt{(p_{T}[GeV])}",100*param_hi_after_0),"l");
    }
  }
  else if (nTerm==2) {  
    leg1->AddEntry(func_pp_before,Form("p+p before:  %.0f%% / #sqrt{(p_{T}[GeV])} #oplus %.0f%%",100*param_pp_before_0,100*param_pp_before_1),"l");
    leg1->AddEntry(func_pp_after,Form("p+p after:  %.0f%% / #sqrt{(p_{T}[GeV])} #oplus %.0f%%",100*param_pp_after_0,100*param_pp_after_1),"l");
    if (note1st.compare("04fm")==0) {
      leg1->AddEntry(func_hi_before,Form("Au+Au (0-4 fm) before:  %.0f%% / #sqrt{(p_{T}[GeV])} #oplus %.0f%%",100*param_hi_before_0,100*param_hi_before_1),"l");
      leg1->AddEntry(func_hi_after,Form("Au+Au (0-4 fm) after:  %.0f%% / #sqrt{(p_{T}[GeV])} #oplus %.0f%%",100*param_hi_after_0,100*param_hi_after_1),"l");
    } else if (note1st.compare("48fm")==0) {
      leg1->AddEntry(func_hi_before,Form("Au+Au (4-8 fm) before:  %.0f%% / #sqrt{(p_{T}[GeV])} #oplus %.0f%%",100*param_hi_before_0,100*param_hi_before_1),"l");
      leg1->AddEntry(func_hi_after,Form("Au+Au (4-8 fm) after:  %.0f%% / #sqrt{(p_{T}[GeV])} #oplus %.0f%%",100*param_hi_after_0,100*param_hi_after_1),"l");
    }
  }
  else if (nTerm==3) {  
    leg1->AddEntry(func_pp_before,Form("p+p before:  %.0f%% / p_{T}[GeV] #oplus %.0f%% / #sqrt{(p_{T}[GeV])} #oplus %.0f%%",100*param_pp_before_2,100*param_pp_before_0,100*param_pp_before_1),"l");
    leg1->AddEntry(func_pp_after,Form("p+p after:  %.0f%% / p_{T}[GeV] #oplus %.0f%% / #sqrt{(p_{T}[GeV])} #oplus %.0f%%",100*param_pp_after_2,100*param_pp_after_0,100*param_pp_after_1),"l");
    if (note1st.compare("04fm")==0) {
      leg1->AddEntry(func_hi_before,Form("Au+Au (0-4 fm) before:  %.0f%% / p_{T}[GeV] #oplus %.0f%% / #sqrt{(p_{T}[GeV])} #oplus %.0f%%",100*param_hi_before_2,100*param_hi_before_0,100*param_hi_before_1),"l");
      leg1->AddEntry(func_hi_after,Form("Au+Au (0-4 fm) after:  %.0f%% / p_{T}[GeV] #oplus %.0f%% / #sqrt{(p_{T}[GeV])} #oplus %.0f%%",100*param_hi_after_2,100*param_hi_after_0,100*param_hi_after_1),"l");
    } else if (note1st.compare("48fm")==0) {
      leg1->AddEntry(func_hi_before,Form("Au+Au (4-8 fm) before:  %.0f%% / p_{T}[GeV] #oplus %.0f%% / #sqrt{(p_{T}[GeV])} #oplus %.0f%%",100*param_hi_before_2,100*param_hi_before_0,100*param_hi_before_1),"l");
      leg1->AddEntry(func_hi_after,Form("Au+Au (4-8 fm) after:  %.0f%% / p_{T}[GeV] #oplus %.0f%% / #sqrt{(p_{T}[GeV])} #oplus %.0f%%",100*param_hi_after_2,100*param_hi_after_0,100*param_hi_after_1),"l");
    }
  }
  leg1->Draw();

//  c1->SaveAs(Form("outPPHI/comp_%s_%s_%s_nTerm%d.pdf",ihcalType.c_str(),note1st.c_str(),note2nd.c_str(),nTerm));  
  
  
  //// Decomposition
  double fitmin = 25;
  double fitmax = 55;
   
  TF1* func_subt_before = new TF1("func_subt_before","[0]/sqrt(x)",fitmin,fitmax);
  TF1* func_subt_after = new TF1("func_subt_after","[0]/sqrt(x)",fitmin,fitmax);
  func_subt_before->SetLineColor(kGreen+1);
  func_subt_before->SetLineStyle(5);
  func_subt_before->SetParameter(0, 1.20 );
  func_subt_before->SetParLimits(0, 0.0, 10.00 );
  func_subt_after->SetLineColor(kOrange+1);
  func_subt_after->SetLineStyle(1);
  func_subt_after->SetParameter(0, 1.20 );
  func_subt_after->SetParLimits(0, 0.0, 10.00 );

  TCanvas* c2 = new TCanvas("c2","c2",600,600);
  subt_before->SetMinimum(0); 
  if(note2nd.compare("R04")==0) subt_before->SetMaximum(0.6);
  else subt_before->SetMaximum(0.6);
  subt_before->GetXaxis()->SetTitle("p_{T,Jet}^{Truth} [GeV]");
  subt_before->GetXaxis()->CenterTitle(1);
  subt_before->GetYaxis()->SetTitle("#sqrt{((#sigma/#mu)^{2}_{AA}-(#sigma/#mu)^{2}_{pp})}");
  subt_before->GetYaxis()->CenterTitle(1);
  subt_before->GetXaxis()->SetLimits(0,70.);
  subt_before->Draw("ap"); 
  subt_after->Draw("p"); 
  
  subt_before->Fit(func_subt_before,"NR");
  //func_subt_before->Draw("same"); 
  subt_after->Fit(func_subt_after,"NR");
  //func_subt_after->Draw("same"); 
  double param_subt_before_0;
  double param_subt_after_0;
  param_subt_before_0 = func_subt_before->GetParameter(0);
  param_subt_after_0 = func_subt_after->GetParameter(0);

  //TLegend* leg2 = new TLegend(0.18, 0.18, 0.40, 0.33);
  TLegend* leg2 = new TLegend(0.18, 0.18, 0.40, 0.28);
  //leg2->SetTextSize(0.045);
  leg2->SetTextSize(0.025);
  leg2->AddEntry(subt_before,"before calibration","lp");
  leg2->AddEntry(subt_after,"after calibration","lp");
  //leg2->AddEntry(func_subt_before,Form("before:  %.0f%% / (p_{T}[GeV])",100*param_subt_before_0),"l");
  //leg2->AddEntry(func_subt_after,Form("after:  %.0f%% / (p_{T}[GeV])",100*param_subt_after_0),"l");
  leg2->Draw();

  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->SetTextSize(0.040);
  //latex->DrawLatex(0.2,0.82,"PYTHIA8+HIJING b = 4-8 fm dijet"); 
  if (note2nd.compare("R02")==0) { latex->DrawLatex(0.2,0.82,"anti-k_{T} R=0.2, |#eta| < 0.45"); }
  else if (note2nd.compare("R04")==0) { latex->DrawLatex(0.2,0.82,"anti-k_{T} R=0.4, |#eta| < 0.45"); }
  else { cout << "WARNING!! R02 or R04"<<endl; return; };
  if (ihcalType.compare("Al")==0) { latex->DrawLatex(0.2,0.77,"Instrumented Al IHCAL");  }
  else if (ihcalType.compare("Alframe")==0) { latex->DrawLatex(0.2,0.77,"Uninstrumented Aluminum");  }
  else if (ihcalType.compare("SS310")==0) { latex->DrawLatex(0.2,0.77,"Uninstrumented Steel");  }
  else { cout << "WARNING!! Al, Alframe, or SS310"<<endl; return; };
  if (note1st.compare("04fm")==0) { latex->DrawLatex(0.2,0.72,"Au+Au 0-4 fm"); }
  else if (note1st.compare("48fm")==0) { latex->DrawLatex(0.2,0.72,"Au+Au 4-8 fm"); }
  
  c2->SaveAs(Form("outPPHI/decompose_%s_%s_%s_nTerm%d.pdf",ihcalType.c_str(),note1st.c_str(),note2nd.c_str(),nTerm));  

  return;
}
