void comp_JER(string ihcalType="Al", string note1st="48fm", string note2nd="R02"){
  
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.038);
  
  TLegend* leg1 = new TLegend(0.18, 0.18, 0.40, 0.33);
  //leg1->SetTextSize(0.045);
  leg1->SetTextSize(0.025);

  TFile* fin01 =  new TFile(Form("../z_20180705_truthinfo/outvsE/JESvsE_%s_%s.root",ihcalType.c_str(),note2nd.c_str()),"READ");
  cout << fin01 << endl;
  if(!fin01->IsOpen()) { cout <<"WARNING!! fin01 not found"<<endl; return;};
  TFile* fin02 =  new TFile(Form("../z_20180705_HIJet/outvsE/JESvsE_%s_%s%s.root",ihcalType.c_str(),note1st.c_str(),note2nd.c_str()),"READ");
  if(!fin02->IsOpen()) { cout <<"WARNING!! fin02 not found"<<endl; return;};
  
  TGraphAsymmErrors* pp_before = (TGraphAsymmErrors*)fin01->Get("gjerAlBefore");
  TGraphAsymmErrors* pp_after = (TGraphAsymmErrors*)fin01->Get("gjerAl_1");
  TGraphAsymmErrors* hi_before = (TGraphAsymmErrors*)fin02->Get("gjerAlBefore");
  TGraphAsymmErrors* hi_after = (TGraphAsymmErrors*)fin02->Get("gjerAl_1");

  SetGraphStyle(pp_before,1,0);
  SetGraphStyle(pp_after,1,10);
  SetGraphStyle(hi_before,2,0);
  SetGraphStyle(hi_after,2,10);
  
  TCanvas* c1 = new TCanvas("c1","c1",600,600);
  pp_before->SetMaximum(0.5);
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
  if (ihcalType.compare("Al")==0) { latex->DrawLatex(0.2,0.77,"Instrumented Aluminum");  }
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

  TF1* func_pp_before = new TF1("func_pp_before","sqrt(([0]/TMath::Sqrt(x))*([0]/TMath::Sqrt(x))+[1]*[1])",20,60);
  func_pp_before->SetLineColor(kRed-7);
  func_pp_before->SetLineStyle(1);
  func_pp_before->SetParameter(0, 1.20 );
  func_pp_before->SetParameter(1, 0.1 );
  func_pp_before->SetParLimits(0, 0.0, 1.60 );
  func_pp_before->SetParLimits(1, 0.0, 0.50 );
  pp_before->Fit(func_pp_before,"NR");
  func_pp_before->Draw("same"); 
  double param_pp_before_0 = func_pp_before->GetParameter(0);
  double param_pp_before_1 = func_pp_before->GetParameter(1);
  TF1* func_pp_after = new TF1("func_pp_after","sqrt(([0]/TMath::Sqrt(x))*([0]/TMath::Sqrt(x))+[1]*[1])",20,60);
  func_pp_after->SetLineColor(kRed-7);
  func_pp_after->SetLineStyle(2);
  func_pp_after->SetParameter(0, 1.20 );
  func_pp_after->SetParameter(1, 0.1 );
  func_pp_after->SetParLimits(0, 0.0, 1.60 );
  func_pp_after->SetParLimits(1, 0.0, 0.50 );
  pp_after->Fit(func_pp_after,"NR");
  func_pp_after->Draw("same"); 
  double param_pp_after_0 = func_pp_after->GetParameter(0);
  double param_pp_after_1 = func_pp_after->GetParameter(1);

  TF1* func_hi_before = new TF1("func_hi_before","sqrt(([0]/TMath::Sqrt(x))*([0]/TMath::Sqrt(x))+[1]*[1])",20,60);
  func_hi_before->SetLineColor(kAzure+1);
  func_hi_before->SetLineStyle(1);
  func_hi_before->SetParameter(0, 1.20 );
  func_hi_before->SetParameter(1, 0.1 );
  func_hi_before->SetParLimits(0, 0.0, 1.60 );
  func_hi_before->SetParLimits(1, 0.0, 0.50 );
  hi_before->Fit(func_hi_before,"NR");
  func_hi_before->Draw("same"); 
  double param_hi_before_0 = func_hi_before->GetParameter(0);
  double param_hi_before_1 = func_hi_before->GetParameter(1);
  TF1* func_hi_after = new TF1("func_hi_after","sqrt(([0]/TMath::Sqrt(x))*([0]/TMath::Sqrt(x))+[1]*[1])",20,60);
  func_hi_after->SetLineColor(kAzure+1);
  func_hi_after->SetLineStyle(2);
  func_hi_after->SetParameter(0, 1.20 );
  func_hi_after->SetParameter(1, 0.1 );
  func_hi_after->SetParLimits(0, 0.0, 1.60 );
  func_hi_after->SetParLimits(1, 0.0, 0.50 );
  hi_after->Fit(func_hi_after,"NR");
  func_hi_after->Draw("same"); 
  double param_hi_after_0 = func_hi_after->GetParameter(0);
  double param_hi_after_1 = func_hi_after->GetParameter(1);
  
  leg1->AddEntry(func_pp_before,Form("p+p before:  %.0f%% / #sqrt{(p_{T}[GeV])} #oplus %.0f%%",100*param_pp_before_0,100*param_pp_before_1),"l");
  leg1->AddEntry(func_pp_after,Form("p+p after:  %.0f%% / #sqrt{(p_{T}[GeV])} #oplus %.0f%%",100*param_pp_after_0,100*param_pp_after_1),"l");
  leg1->AddEntry(func_hi_before,Form("Au+Au (4-8 fm) before:  %.0f%% / #sqrt{(p_{T}[GeV])} #oplus %.0f%%",100*param_hi_before_0,100*param_hi_before_1),"l");
  leg1->AddEntry(func_hi_after,Form("Au+Au (4-8 fm) after:  %.0f%% / #sqrt{(p_{T}[GeV])} #oplus %.0f%%",100*param_hi_after_0,100*param_hi_after_1),"l");
  leg1->Draw();

  c1->SaveAs(Form("outvsE/comp_%s_%s_%s.pdf",ihcalType.c_str(),note1st.c_str(),note2nd.c_str()));  

  return;
}
