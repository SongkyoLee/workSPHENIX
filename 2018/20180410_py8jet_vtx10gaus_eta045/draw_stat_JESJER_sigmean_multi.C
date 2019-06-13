#include "TMinuit.h"

void recursiveFit(TH1D* t_h1, TF1* t_tf1, float t_fitmin, float t_fitmax, float* t_mean, float* t_sig, float* t_sigErr);

void draw_stat_JESJER_sigmean_multi(string ihcalType="SS310"){
  
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
  jesymin =0.5; jesymax = 1.5;
  double jerymin, jerymax;
  jerymin =0.0; jerymax = 0.35;

  ///////////////////////////////////////
  //// Get Hist for different pT range
  const int nFile = 30;
  cout << "nFile = " << nFile << endl;
  const int nMeth = 5; 
  cout << "nMeth = " << nMeth << endl;

  ///////////////////////////////////////
  ///////////////////////////////////////
  //// Draw JES
  ///////////////////////////////////////
  ///////////////////////////////////////
  double dummy;
  double jes[nMeth][nFile];
  double jer[nMeth][nFile];
  
  //////////////////// read-in jes files
  string szjes;
  int ijes=0;
  ifstream infile_jes[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_jes[im].open(Form("txt_jes/jes_mean_method0%d_%s.out",im+1,ihcalType.c_str()));
    ijes=0;
    if (infile_jes[im].is_open()){
      while( getline (infile_jes[im],szjes) ) { jes[im][ijes] = atof(szjes.c_str()); ijes++; }
    }
  }
  string szjer;
  int ijer=0;
  ifstream infile_jer[nMeth];
  for (int im=0; im<nMeth; im++){
    //infile_jer[im].open(Form("txt_jes/jer_sig_method0%d_%s.out",im+1,ihcalType.c_str()));
    infile_jer[im].open(Form("txt_jes/jes_sigmean_method0%d_%s.out",im+1,ihcalType.c_str()));
    ijer=0;
    if (infile_jer[im].is_open()){
      while( getline (infile_jer[im],szjer) ) { jer[im][ijer] = atof(szjer.c_str()); ijer++; }
    }
  }
  
  //////////////////////////////////////////////////////////////////////
  //// Histogram

  TCanvas* c_1d = new TCanvas("c_1d","",1000,500);
  c_1d->Divide(2,1);
 
  TH1D* h_jes[nMeth];
  TH1D* h_jer[nMeth];
  double jes_binmin;
  double jes_binmax;
  double jer_binmin;
  double jer_binmax;
  
  if (ihcalType.compare("SS310")==0) {
    jes_binmin = 0.9;
    jes_binmax = 1.5;
    jer_binmin = 0.10;
    jer_binmax = 0.22;
  } else if (ihcalType.compare("Alframe")==0) {
    jes_binmin = 0.9;
    jes_binmax = 1.5;
    jer_binmin = 0.16;
    jer_binmax = 0.22;
  } else {
    cout << "ERROR!! select a proper ihcalType"; return;
  }

  for (int im =0; im < nMeth; im ++) {
    //h_jes[im] = new TH1D(Form("h_jes_%d",im),";EMCAL Scale;Counts",60,0.0,5.0);
    //h_jer[im] = new TH1D(Form("h_jer_%d",im),";IHCAL Scale;Counts",60,0.0,8.0);
    //h_ohcal[im] = new TH1D(Form("h_ohcal_%d",im),";OHCAL Scale;Counts",60,0.0,3.0);
      h_jes[im] = new TH1D(Form("h_jes_%d",im),";Jet Energy Scale;Counts",(jes_binmax-jes_binmin)*100*1.8,jes_binmin,jes_binmax);
      if (ihcalType.compare("SS310")==0) { h_jer[im] = new TH1D(Form("h_jer_%d",im),";Jet Energy Resolution;Counts",(jer_binmax-jer_binmin)*200*1.8,jer_binmin,jer_binmax); }
      else { h_jer[im] = new TH1D(Form("h_jer_%d",im),";Jet Energy Resolution;Counts",(jer_binmax-jer_binmin)*300*1.8,jer_binmin,jer_binmax); }
  }  
  for (int im =0; im < nMeth; im ++) {
    for (int ie=0; ie<nFile; ie++){
      h_jes[im]->Fill(jes[im][ie]); 
      h_jer[im]->Fill(jer[im][ie]); 
    }
  }
    
  SetLineHistStyle(h_jes[4],4);
  SetLineHistStyle(h_jer[4],4);
  SetLineHistStyle(h_jes[2],5);
  SetLineHistStyle(h_jer[2],5);
  SetLineHistStyle(h_jes[3],6);
  SetLineHistStyle(h_jer[3],6);
 
  ////////////////////////////////////////////////////////////////////////////////
  float col;
  int repeat=30;
  float fitmin;
  float fitmax;
  float mean;
  float sig;
  float sigErr;
  string sz_mean; 
  string sz_sig; 
  
  TF1* tf_jes;
  TF1* tf_jer;
 
  c_1d->cd(1);
  h_jes[4]->SetMaximum(9);
  h_jes[4]->Draw("hist");
  fitmin = 0.5;
  fitmax = 4.5;
  tf_jes= new TF1("tf_jes","gaus",fitmin,fitmax);
  //col = h_jes->GetMarkerColor();
  //tf_jes->SetLineColor(col);
  //tf_jes->SetLineColor(kPink+2);
  tf_jes->SetLineColor(kOrange+7);
  //tf_jes->SetLineStyle(7);
  tf_jes->SetLineWidth(3);
  //tf_jes->SetParLimits(1,1,3);
  //tf_jes->SetParLimits(2,0.1,0.5);
  recursiveFit(h_jes[4], tf_jes,fitmin, fitmax, &mean, &sig, &sigErr);
  for (int ir=0; ir<repeat; ir++){
    fitmin = mean-2*sig;
    fitmax = mean+2*sig;
    recursiveFit(h_jes[4], tf_jes,fitmin, fitmax, &mean, &sig, &sigErr);
  }
  tf_jes->SetRange(jes_binmin,jes_binmax);
  tf_jes->Draw("same");
  //sz_mean = Form ("#mu = %.3f", mean);
  sz_mean = Form ("%.3f #pm %.3f", mean, sig);
  myLatex(0.65,0.88,sz_mean.c_str(),12,0.045,kOrange+7);
  myLatex(0.22,0.88,ihcalType.c_str(),12,0.045);
  
  h_jes[2]->Draw("hist same");
  fitmin = 0.5;
  fitmax = 4.5;
  tf_jes= new TF1("tf_jes","gaus",fitmin,fitmax);
  //col = h_jes->GetMarkerColor();
  //tf_jes->SetLineColor(col);
  //tf_jes->SetLineColor(kAzure+5);
  tf_jes->SetLineColor(kViolet+2);
  //tf_jes->SetLineStyle(7);
  tf_jes->SetLineWidth(3);
  recursiveFit(h_jes[2], tf_jes,fitmin, fitmax, &mean, &sig, &sigErr);
  for (int ir=0; ir<repeat; ir++){
    fitmin = mean-2*sig;
    fitmax = mean+2*sig;
    recursiveFit(h_jes[2], tf_jes,fitmin, fitmax, &mean, &sig, &sigErr);
  }
  tf_jes->SetRange(jes_binmin,jes_binmax);
  tf_jes->Draw("same");
  sz_mean = Form ("%.3f #pm %.3f", mean, sig);
  myLatex(0.65,0.83,sz_mean.c_str(),12,0.045,kViolet+2);
  
  if (ihcalType.compare("SS310")==0) {
  h_jes[3]->Draw("hist same");
  fitmin = 0.5;
  fitmax = 4.5;
  tf_jes= new TF1("tf_jes","gaus",fitmin,fitmax);
  //col = h_jes->GetMarkerColor();
  //tf_jes->SetLineColor(col);
  //tf_jes->SetLineColor(kSpring+2);
  tf_jes->SetLineColor(kCyan+2);
  //tf_jes->SetLineStyle(7);
  tf_jes->SetLineWidth(3);
  recursiveFit(h_jes[3], tf_jes,fitmin, fitmax, &mean, &sig, &sigErr);
  for (int ir=0; ir<repeat; ir++){
    fitmin = mean-2*sig;
    fitmax = mean+2*sig;
    recursiveFit(h_jes[3], tf_jes,fitmin, fitmax, &mean, &sig, &sigErr);
  }
  tf_jes->SetRange(jes_binmin,jes_binmax);
  tf_jes->Draw("same");
  sz_mean = Form ("%.3f #pm %.3f", mean, sig);
  myLatex(0.65,0.78,sz_mean.c_str(),12,0.045,kCyan+2);
  }
  
  ////////////////////////////////////////////////////////////////////////////////
  
  c_1d->cd(2);
  if (ihcalType.compare("SS310")==0) h_jer[4]->SetMaximum(25);
  else h_jer[4]->SetMaximum(30);
  h_jer[4]->Draw("hist");
  fitmin = 2.;
  fitmax = 6.;
  tf_jer= new TF1("tf_jer","gaus",fitmin,fitmax);
  //col = h_jer->GetMarkerColor();
  //tf_jer->SetLineColor(col);
  //tf_jer->SetLineColor(kPink+2);
  tf_jer->SetLineColor(kOrange+7);
  //tf_jer->SetLineStyle(7);
  tf_jer->SetLineWidth(3);
  recursiveFit(h_jer[4], tf_jer,fitmin, fitmax, &mean, &sig, &sigErr);
  for (int ir=0; ir<repeat; ir++){
    fitmin = mean-2*sig;
    fitmax = mean+2*sig;
    recursiveFit(h_jer[4], tf_jer,fitmin, fitmax, &mean, &sig, &sigErr);
  }
  tf_jer->SetRange(jer_binmin,jer_binmax);
  tf_jer->Draw("same");
  sz_mean = Form ("%.3f #pm %.3f", mean, sig);
  myLatex(0.65,0.88,sz_mean.c_str(),12,0.045,kOrange+7);
  
  h_jer[2]->Draw("hist same");
  fitmin = 2.;
  fitmax = 6.;
  tf_jer= new TF1("tf_jer","gaus",fitmin,fitmax);
  //col = h_jer->GetMarkerColor();
  //tf_jer->SetLineColor(col);
  //tf_jer->SetLineColor(kAzure+5);
  tf_jer->SetLineColor(kViolet+2);
  //ntf_jer->SetLineStyle(7);
  tf_jer->SetLineWidth(3);
  recursiveFit(h_jer[2], tf_jer,fitmin, fitmax, &mean, &sig, &sigErr);
  for (int ir=0; ir<repeat; ir++){
    fitmin = mean-2*sig;
    fitmax = mean+2*sig;
    recursiveFit(h_jer[2], tf_jer,fitmin, fitmax, &mean, &sig, &sigErr);
  }
  tf_jer->SetRange(jer_binmin,jer_binmax);
  tf_jer->Draw("same");
  sz_mean = Form ("%.3f #pm %.3f", mean, sig);
  myLatex(0.65,0.83,sz_mean.c_str(),12,0.045,kViolet+2);
  
  if (ihcalType.compare("SS310")==0) {
  h_jer[3]->Draw("hist same");
  fitmin = 2.;
  fitmax = 6.;
  tf_jer= new TF1("tf_jer","gaus",fitmin,fitmax);
  //col = h_jer->GetMarkerColor();
  //tf_jer->SetLineColor(col);
  //tf_jer->SetLineColor(kSpring+2);
  tf_jer->SetLineColor(kCyan+2);
  //tf_jer->SetLineStyle(7);
  tf_jer->SetLineWidth(3);
  recursiveFit(h_jer[3], tf_jer,fitmin, fitmax, &mean, &sig, &sigErr);
  for (int ir=0; ir<repeat; ir++){
    fitmin = mean-2*sig;
    fitmax = mean+2*sig;
    recursiveFit(h_jer[3], tf_jer,fitmin, fitmax, &mean, &sig, &sigErr);
  }
  tf_jer->SetRange(jer_binmin,jer_binmax);
  tf_jer->Draw("same");
  sz_mean = Form ("%.3f #pm %.3f", mean, sig);
  myLatex(0.65,0.78,sz_mean.c_str(),12,0.045,kCyan+2);
  } 
  c_1d->SaveAs(Form("out_stat/1d_jesjer_multi_%s_sigmean.pdf",ihcalType.c_str()));
  
  //////////////////////////////////////////////////////////////////////////////////////////
  ////FINAL ////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////
  const int newNum = 1; 
  double final_jes_mean[newNum];
  double final_jes_sig[newNum];
  double final_jer_mean[newNum];
  double final_jer_sig[newNum];
  
  TCanvas* c_1d_2 = new TCanvas("c_1d_2","",1000,500);
  c_1d_2->Divide(2,1);
 
  int finalMeth = 2;
  if (ihcalType.compare("SS310")==0) {
    jes_binmin = 1.068-0.13;
    jes_binmax = 1.068+0.13;
    jer_binmin = 0.146-0.025;
    jer_binmax = 0.146+0.025;
  } else {
    jes_binmin = 1.011-0.14;
    jes_binmax = 1.011+0.14;
    jer_binmin = 0.190-0.025;
    jer_binmax = 0.190+0.025;
  }
  TH1D* h_jes_final = new TH1D("h_jes_final",";Jet Energy Scale;Counts",30*1.2,jes_binmin,jes_binmax);
  TH1D* h_jer_final = new TH1D("h_jer_final",";Jet Energy Resolution;Counts",30*1.2,jer_binmin,jer_binmax);
  h_jes_final->Sumw2();
  h_jer_final->Sumw2();
  SetLineHistStyle(h_jes_final,1);
  SetLineHistStyle(h_jer_final,1);
  //SetHistStyle(h_jes_final,0,0);
  //SetHistStyle(h_jer_final,0,0);
   
  for (int ie=0; ie<nFile; ie++){
    h_jes_final->Fill(jes[finalMeth][ie]); 
    h_jer_final->Fill(jer[finalMeth][ie]); 
  }

  ////////////////////////////////////////////////////////////////////////////////
  c_1d_2->cd(1);
  h_jes_final->SetMaximum(10);
  h_jes_final->Draw("hist");
  fitmin = jes_binmin;
  fitmax = jes_binmax;
  TF1* tf_jes_final= new TF1("tf_jes_final","gaus",fitmin,fitmax);
  tf_jes_final->SetLineColor(kRed-4);
//  tf_jes_final->SetLineStyle(7);
  tf_jes_final->SetLineWidth(3);
  //tf_jes_final->SetParLimits(1,1,3);
  recursiveFit(h_jes[4], tf_jes_final,fitmin, fitmax, &mean, &sig, &sigErr);
  for (int ir=0; ir<repeat; ir++){
    fitmin = mean-2*sig;
    fitmax = mean+2*sig;
    recursiveFit(h_jes_final, tf_jes_final,fitmin, fitmax, &mean, &sig, &sigErr);
  }
  tf_jes_final->SetRange(jes_binmin,jes_binmax);
  tf_jes_final->Draw("same");
  sz_mean = Form ("%.3f #pm %.3f", mean, sig);
  myLatex(0.65,0.88,sz_mean.c_str(),12,0.045,kRed-4);
  myLatex(0.22,0.88,ihcalType.c_str(),12,0.045);
  final_jes_mean[0] = mean;
  final_jes_sig[0] = 1*sig;

  ////////////////////////////////////////////////////////////////////////////////
  c_1d_2->cd(2);
  if (ihcalType.compare("SS310")==0) h_jer_final->SetMaximum(10);
  else h_jer_final->SetMaximum(15);
  h_jer_final->Draw("hist");
  fitmin = jer_binmin;
  fitmax = jer_binmax;
  TF1* tf_jer_final= new TF1("tf_jer_final","gaus",fitmin,fitmax);
  tf_jer_final->SetLineColor(kRed-4);
//  tf_jer_final->SetLineStyle(7);
  tf_jer_final->SetLineWidth(3);
  //tf_jer_final->SetParLimits(1,1,3);
  recursiveFit(h_jer[4], tf_jer_final,fitmin, fitmax, &mean, &sig, &sigErr);
  for (int ir=0; ir<repeat; ir++){
    fitmin = mean-2*sig;
    fitmax = mean+2*sig;
    recursiveFit(h_jer_final, tf_jer_final,fitmin, fitmax, &mean, &sig, &sigErr);
  }
  tf_jer_final->SetRange(jer_binmin,jer_binmax);
  tf_jer_final->Draw("same");
  sz_mean = Form ("%.3f #pm %.3f", mean, sig);
  myLatex(0.65,0.88,sz_mean.c_str(),12,0.045,kRed-4);
  final_jer_mean[0] = mean;
  final_jer_sig[0] = 1*sig;


  c_1d_2->SaveAs(Form("out_stat/1d_jesjer_multi_final_%s_sigmean.pdf",ihcalType.c_str()));
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //// TGraph //////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////
  
  double xPnt[newNum];
  double exlow[newNum];
  double exhigh[newNum];
  double ey[newNum];
 
  for (int ie=0; ie<newNum; ie++){
    xPnt[ie] = 22.5;
    exlow[ie] =  2.5;
    exhigh[ie] =  7.5;
    ey[ie] = 0;
  }
  
  TCanvas* c_jes = new TCanvas("c_jes","",600,600);
  c_jes->cd();
  
  TGraphAsymmErrors* gjes_final = new TGraphAsymmErrors(newNum, xPnt, final_jes_mean, exlow, exhigh, final_jes_sig, final_jes_sig);
  gjes_final->SetName("gjes_final");

  SetGraphStyle(gjes_final,0,13);

  gjes_final->GetXaxis()->SetTitle("Jet E [GeV]");
  gjes_final->GetXaxis()->CenterTitle(1);
  gjes_final->GetYaxis()->SetTitle("Jet Energy Scale");
  gjes_final->GetYaxis()->CenterTitle(1);
  gjes_final->GetXaxis()->SetLimits(0,70.);
  gjes_final->SetMinimum(jesymin); 
  gjes_final->SetMaximum(jesymax); 
  
  gjes_final->Draw("APL");

  dashedLine(0.0,1.0,70.0,1.0,1,1);
  
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4");
  //latex->DrawLatex(0.2,0.77,"#gamma |#eta^{truth}| < 0.6");
  
  //c_jes->SaveAs(Form("outvsE/JESJERvsE_%s.pdf",ihcalType.c_str()));
  c_jes->SaveAs(Form("out_stat/JESJERvsE_final_%s_sigmean.pdf",ihcalType.c_str()));
  
  TCanvas* c_jer = new TCanvas("c_jer","",600,600);
  c_jer->cd();
  
  TGraphAsymmErrors* gjer_final = new TGraphAsymmErrors(newNum, xPnt, final_jer_mean, exlow, exhigh, final_jer_sig, final_jer_sig);
  gjer_final->SetName("gjer_final");

  SetGraphStyle(gjer_final,0,13);

  gjer_final->GetXaxis()->SetTitle("Jet E [GeV]");
  gjer_final->GetXaxis()->CenterTitle(1);
  gjer_final->GetYaxis()->SetTitle("Jet Energy Resolution");
  gjer_final->GetYaxis()->CenterTitle(1);
  gjer_final->GetXaxis()->SetLimits(0,70.);
  gjer_final->SetMinimum(jerymin); 
  gjer_final->SetMaximum(jerymax); 
  
  gjer_final->Draw("APL");

  dashedLine(0.0,1.0,70.0,1.0,1,1);
  
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"G4Simulation");
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, anti-k_{T} R=0.4");
  //latex->DrawLatex(0.2,0.77,"#gamma |#eta^{truth}| < 0.6");
  
  //c_jer->SaveAs(Form("outvsE/JESJERvsE_%s.pdf",ihcalType.c_str()));
  c_jer->SaveAs(Form("out_stat/JESJERvsE_final_%s_sigmean.pdf",ihcalType.c_str()));


  
   
  TFile* fout =  new TFile(Form("out_stat/JESJERvsE_final_%s_sigmean.root",ihcalType.c_str()),"RECREATE");
  fout->cd();
  gjes_final->Write();
  gjer_final->Write();
  fout->Close(); 
  return;
  
}

////////////////////////////////////////

void recursiveFit(TH1D* t_h1, TF1* t_tf1, float t_fitmin, float t_fitmax, float* t_mean, float* t_sig, float* t_sigErr){
  string tfname = t_tf1->GetName();
  //t_h1[ih]->Fit(Form("tf1_%d",ih),"NR"); 
  //t_h1->Fit(tfname.c_str(),"NR"); 
  t_tf1->SetRange(t_fitmin,t_fitmax);
  t_h1->Fit(tfname.c_str(),"NR","",t_fitmin,t_fitmax); 
  //t_h1->Fit(tfname.c_str(),"NRB","",t_fitmin,t_fitmax); 
  //sig[ih] = tf1[ih]->GetParameter(2);
  //sigErr[ih] = tf1[ih]->GetParError(2);
  *t_mean = t_tf1->GetParameter(1);
  *t_sig = t_tf1->GetParameter(2);
  *t_sigErr =  t_tf1->GetParError(2);

  return;
}
