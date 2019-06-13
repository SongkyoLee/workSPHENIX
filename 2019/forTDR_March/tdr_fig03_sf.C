#include "TMinuit.h"

void recursiveFit(TH1D* t_h1, TF1* t_tf1, float t_fitmin, float t_fitmax, float* t_mean, float* t_sig, float* t_sigErr);

void tdr_fig03_sf(string ihcalType="Alframe"){
  
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);

  TLegend* leg1 = new TLegend(0.20, 0.21, 0.50, 0.35);
  leg1->SetTextSize(0.045);
  TLegend* leg2 = new TLegend(0.20, 0.21, 0.50, 0.35);
  leg2->SetTextSize(0.045);
  
  //// select hist x axis range
  double nbin = 100;
  double binmin =0.9, binmax = 5.0;
  double ymin, ymax; 
  ymin =0.9; ymax = 5.0;
  
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
  double cemc[nMeth][nFile];
  double ihcal[nMeth][nFile];
  double ohcal[nMeth][nFile];
  
  //////////////////// read-in cemc files
  string szcemc;
  int icemc=0;
  ifstream infile_cemc[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_cemc[im].open(Form("txt_scale/cemcScale_method0%d_%s.out",im+1,ihcalType.c_str()));
    icemc=0;
    if (infile_cemc[im].is_open()){
      while( getline (infile_cemc[im],szcemc) ) { cemc[im][icemc] = atof(szcemc.c_str()); icemc++; }
    }
  }
  string szihcal;
  int iihcal=0;
  ifstream infile_ihcal[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_ihcal[im].open(Form("txt_scale/ihcalScale_method0%d_%s.out",im+1,ihcalType.c_str()));
    iihcal=0;
    if (infile_ihcal[im].is_open()){
      while( getline (infile_ihcal[im],szihcal) ) { ihcal[im][iihcal] = atof(szihcal.c_str()); iihcal++; }
    }
  }
  string szohcal;
  int iohcal=0;
  ifstream infile_ohcal[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_ohcal[im].open(Form("txt_scale/ohcalScale_method0%d_%s.out",im+1,ihcalType.c_str()));
    iohcal=0;
    if (infile_ohcal[im].is_open()){
      while( getline (infile_ohcal[im],szohcal) ) { ohcal[im][iohcal] = atof(szohcal.c_str()); iohcal++; }
    }
  }
  
  //////////////////////////////////////////////////////////////////////
  //// Histogram

  TCanvas* c_1d = new TCanvas("c_1d","",1000,500);
  c_1d->Divide(2,1);
 
  TH1D* h_cemc[nMeth];
  TH1D* h_ihcal[nMeth];
  TH1D* h_ohcal[nMeth];
  double cemc_binmin;
  double cemc_binmax;
  double ihcal_binmin;
  double ihcal_binmax;
  double ohcal_binmin;
  double ohcal_binmax;
 
  if (ihcalType.compare("Alframe")!=0) { cout << "ERROR!! select Alframe for TDR"; return; } 
  cemc_binmin = 0.9;
  cemc_binmax = 4.9;
  ihcal_binmin = 0.0;
  ihcal_binmax = 1.0;
  ohcal_binmin = 0.45;
  ohcal_binmax = 3.45;

  for (int im =0; im < nMeth; im ++) {
      h_cemc[im] = new TH1D(Form("h_cemc_%d",im),";EMCAL Scale;Counts",(cemc_binmax-cemc_binmin)*10*1.8,cemc_binmin,cemc_binmax);
      h_ihcal[im] = new TH1D(Form("h_ihcal_%d",im),";IHCAL Scale;Counts",(ihcal_binmax-ihcal_binmin)*10*1.8,ihcal_binmin,ihcal_binmax);
      h_ohcal[im] = new TH1D(Form("h_ohcal_%d",im),";OHCAL Scale;Counts",(ohcal_binmax-ohcal_binmin)*10*1.8,ohcal_binmin,ohcal_binmax);
  }  
  for (int im =0; im < nMeth; im ++) {
    for (int ie=0; ie<nFile; ie++){
      h_cemc[im]->Fill(cemc[im][ie]); 
      h_ihcal[im]->Fill(ihcal[im][ie]); 
      h_ohcal[im]->Fill(ohcal[im][ie]); 
    }
  }
    
  SetLineHistStyle(h_cemc[4],4);
  SetLineHistStyle(h_ihcal[4],4);
  SetLineHistStyle(h_ohcal[4],4);
  SetLineHistStyle(h_cemc[2],5);
  SetLineHistStyle(h_ihcal[2],5);
  SetLineHistStyle(h_ohcal[2],5);
  SetLineHistStyle(h_cemc[3],6);
  SetLineHistStyle(h_ihcal[3],6);
  SetLineHistStyle(h_ohcal[3],6);
 
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
  
  TF1* tf_cemc;
  TF1* tf_ihcal;
  TF1* tf_ohcal;
 
  c_1d->cd(1);
  h_cemc[4]->SetMaximum(12);
  h_cemc[4]->Draw("hist");
  fitmin = 0.5;
  fitmax = 4.5;
  tf_cemc= new TF1("tf_cemc","gaus",fitmin,fitmax);
  tf_cemc->SetLineColor(kOrange+7);
  tf_cemc->SetLineWidth(3);
  recursiveFit(h_cemc[4], tf_cemc,fitmin, fitmax, &mean, &sig, &sigErr);
  for (int ir=0; ir<repeat; ir++){
    fitmin = mean-2*sig;
    fitmax = mean+2*sig;
    recursiveFit(h_cemc[4], tf_cemc,fitmin, fitmax, &mean, &sig, &sigErr);
  }
  tf_cemc->SetRange(cemc_binmin,cemc_binmax);
  tf_cemc->Draw("same");
  sz_mean = Form ("%.2f #pm %.2f", mean, sig);
  myLatex(0.65,0.88,sz_mean.c_str(),12,0.045,kOrange+7);
  myLatex(0.22,0.88,ihcalType.c_str(),12,0.045);
  
  h_cemc[2]->Draw("hist same");
  fitmin = 0.5;
  fitmax = 4.5;
  tf_cemc= new TF1("tf_cemc","gaus",fitmin,fitmax);
  tf_cemc->SetLineColor(kViolet+2);
  tf_cemc->SetLineWidth(3);
  recursiveFit(h_cemc[2], tf_cemc,fitmin, fitmax, &mean, &sig, &sigErr);
  for (int ir=0; ir<repeat; ir++){
    fitmin = mean-2*sig;
    fitmax = mean+2*sig;
    recursiveFit(h_cemc[2], tf_cemc,fitmin, fitmax, &mean, &sig, &sigErr);
  }
  tf_cemc->SetRange(cemc_binmin,cemc_binmax);
  tf_cemc->Draw("same");
  sz_mean = Form ("%.2f #pm %.2f", mean, sig);
  myLatex(0.65,0.83,sz_mean.c_str(),12,0.045,kViolet+2);
  
  ////////////////////////////////////////////////////////////////////////////////
   
  c_1d->cd(2);
  h_ohcal[4]->SetMaximum(12);
  h_ohcal[4]->Draw("hist");
  fitmin = 0.;
  fitmax = 2.5;
  tf_ohcal= new TF1("tf_ohcal","gaus",fitmin,fitmax);
  tf_ohcal->SetLineColor(kOrange+7);
  tf_ohcal->SetLineWidth(3);
  recursiveFit(h_ohcal[4], tf_ohcal,fitmin, fitmax, &mean, &sig, &sigErr);
  for (int ir=0; ir<repeat; ir++){
    fitmin = mean-2*sig;
    fitmax = mean+2*sig;
    recursiveFit(h_ohcal[4], tf_ohcal,fitmin, fitmax, &mean, &sig, &sigErr);
  }
  tf_ohcal->SetRange(ohcal_binmin,ohcal_binmax);
  tf_ohcal->Draw("same");
  sz_mean = Form ("%.2f #pm %.2f", mean, sig);
  myLatex(0.65,0.88,sz_mean.c_str(),12,0.045,kOrange+7);
  
  h_ohcal[2]->Draw("hist same");
  fitmin = 0.;
  fitmax = 2.5;
  tf_ohcal= new TF1("tf_ohcal","gaus",fitmin,fitmax);
  tf_ohcal->SetLineColor(kViolet+2);
  tf_ohcal->SetLineWidth(3);
  recursiveFit(h_ohcal[2], tf_ohcal,fitmin, fitmax, &mean, &sig, &sigErr);
  for (int ir=0; ir<repeat; ir++){
    fitmin = mean-2*sig;
    fitmax = mean+2*sig;
    recursiveFit(h_ohcal[2], tf_ohcal,fitmin, fitmax, &mean, &sig, &sigErr);
  }
  tf_ohcal->SetRange(ohcal_binmin,ohcal_binmax);
  tf_ohcal->Draw("same");
  sz_mean = Form ("%.2f #pm %.2f", mean, sig);
  myLatex(0.65,0.83,sz_mean.c_str(),12,0.045,kViolet+2);
  
  c_1d->SaveAs(Form("out_stat/1d_sf_multi_%s.pdf",ihcalType.c_str()));

  //////////////////////////////////////////////////////////////////////////////////////////
  ////FINAL ////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////
 
  const int newNum = 1; 
  double final_cemc_mean[newNum];
  double final_cemc_sig[newNum];
  double final_ihcal_mean[newNum];
  double final_ihcal_sig[newNum];
  double final_ohcal_mean[newNum];
  double final_ohcal_sig[newNum];
   
  TCanvas* c_1d_2 = new TCanvas("c_1d_2","",1000,500);
  c_1d_2->Divide(2,1);
 
  int finalMeth = 2;
  cemc_binmin = 2.15-1.0;
  cemc_binmax = 2.15+1.0;
  ihcal_binmin = 0.0-1.0;
  ihcal_binmax = 0.0+1.0;
  ohcal_binmin = 1.9-1.0;
  ohcal_binmax = 1.9+1.0;
  TH1D* h_cemc_final = new TH1D("h_cemc_final",";EMCal^{had} calibration factor A;Counts",30*1.2,cemc_binmin,cemc_binmax);
  TH1D* h_ihcal_final = new TH1D("h_ihcal_final",";B (Inner HCal Scale);Counts",30*1.2,ihcal_binmin,ihcal_binmax);
  TH1D* h_ohcal_final = new TH1D("h_ohcal_final",";HCal calibration factor B;Counts",30*1.2,ohcal_binmin,ohcal_binmax);
  h_cemc_final->Sumw2();
  h_ihcal_final->Sumw2();
  h_ohcal_final->Sumw2();
  SetLineHistStyle(h_cemc_final,1);
  SetLineHistStyle(h_ihcal_final,2);
  SetLineHistStyle(h_ohcal_final,3);
   
  for (int ie=0; ie<nFile; ie++){
    h_cemc_final->Fill(cemc[finalMeth][ie]); 
    h_ihcal_final->Fill(ihcal[finalMeth][ie]); 
    h_ohcal_final->Fill(ohcal[finalMeth][ie]); 
  }

  ////////////////////////////////////////////////////////////////////////////////
  c_1d_2->cd(1);
  h_cemc_final->SetMaximum(11);
  h_cemc_final->Draw("hist");
  fitmin = cemc_binmin;
  fitmax = cemc_binmax;
  TF1* tf_cemc_final= new TF1("tf_cemc_final","gaus",fitmin,fitmax);
  tf_cemc_final->SetLineColor(kRed-4);
  tf_cemc_final->SetLineWidth(3);
  recursiveFit(h_cemc[4], tf_cemc_final,fitmin, fitmax, &mean, &sig, &sigErr);
  for (int ir=0; ir<repeat; ir++){
    fitmin = mean-2*sig;
    fitmax = mean+2*sig;
    recursiveFit(h_cemc_final, tf_cemc_final,fitmin, fitmax, &mean, &sig, &sigErr);
  }
  tf_cemc_final->SetRange(cemc_binmin,cemc_binmax);
  tf_cemc_final->Draw("same");
  
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"MIE 2018");
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, R=0.4");
  //double tmpE = atof(jetE.c_str());
  //cout << "tmpE = " << tmpE << endl;
  //latex->DrawLatex(0.2,0.76,Form("%.0f <E_{Jet}^{Truth} < %.0f GeV",tmpE,tmpE+10));
 
  sz_mean = Form ("#mu = %.2f", mean);
  myLatex(0.70,0.76,sz_mean.c_str(),12,0.042,kBlack);
  sz_mean = Form ("#sigma/#mu = %.2f", sig/mean);
  myLatex(0.70,0.70,sz_mean.c_str(),12,0.042,kBlack);
  final_cemc_mean[0] = mean;
  final_cemc_sig[0] = 1*sig;
  cout << "final_cemc_mean[0] = " << final_cemc_mean[0] << endl;
  cout << "final_cemc_sig[0] = " << final_cemc_sig[0] << endl;
  cout << "cemc_mean-sig = " << final_cemc_mean[0]-final_cemc_sig[0] << endl;
  cout << "cemc_mean+sig = " << final_cemc_mean[0]+final_cemc_sig[0] << endl;
  
  ////////////////////////////////////////////////////////////////////////////////
  c_1d_2->cd(2);
  h_ohcal_final->SetMaximum(15);
  h_ohcal_final->Draw("hist");
  fitmin = ohcal_binmin;
  fitmax = ohcal_binmax;
  TF1* tf_ohcal_final= new TF1("tf_ohcal_final","gaus",fitmin,fitmax);
  tf_ohcal_final->SetLineColor(kGreen+2);
  tf_ohcal_final->SetLineWidth(3);
  recursiveFit(h_ohcal[4], tf_ohcal_final,fitmin, fitmax, &mean, &sig, &sigErr);
  for (int ir=0; ir<repeat; ir++){
    fitmin = mean-2*sig;
    fitmax = mean+2*sig;
    recursiveFit(h_ohcal_final, tf_ohcal_final,fitmin, fitmax, &mean, &sig, &sigErr);
  }
  tf_ohcal_final->SetRange(ohcal_binmin,ohcal_binmax);
  tf_ohcal_final->Draw("same");
  
  latex->SetTextSize(0.053);
  latex->SetTextFont(72);
  latex->DrawLatex(0.2,0.89,"sPHENIX");
  latex->SetTextFont(42);
  latex->DrawLatex(0.42,0.89,"MIE 2018");
  latex->SetTextSize(0.040);
  latex->DrawLatex(0.2,0.82,"PYTHIA8 dijet, R=0.4");
  //double tmpE = atof(jetE.c_str());
  //cout << "tmpE = " << tmpE << endl;
  //latex->DrawLatex(0.2,0.76,Form("%.0f <E_{Jet}^{Truth} < %.0f GeV",tmpE,tmpE+10));
  
  sz_mean = Form ("#mu = %.2f", mean);
  myLatex(0.70,0.76,sz_mean.c_str(),12,0.042,kBlack);
  sz_mean = Form ("#sigma/#mu = %.2f", sig/mean);
  myLatex(0.70,0.70,sz_mean.c_str(),12,0.042,kBlack);
  final_ohcal_mean[0] = mean;
  final_ohcal_sig[0] = 1*sig;
  cout << "final_ohcal_mean[0] = " << final_ohcal_mean[0] << endl;
  cout << "final_ohcal_sig[0] = " << final_ohcal_sig[0] << endl;
  cout << "ohcal_mean-sig = " << final_ohcal_mean[0]-final_ohcal_sig[0] << endl;
  cout << "ohcal_mean+sig = " << final_ohcal_mean[0]+final_ohcal_sig[0] << endl;

  c_1d_2->SaveAs(Form("out_tdr_figs/hcal_sf_%s.pdf",ihcalType.c_str()));

  return;
}

////////////////////////////////////////

void recursiveFit(TH1D* t_h1, TF1* t_tf1, float t_fitmin, float t_fitmax, float* t_mean, float* t_sig, float* t_sigErr){
  string tfname = t_tf1->GetName();
  //t_h1[ih]->Fit(Form("tf1_%d",ih),"NR"); 
  //t_h1->Fit(tfname.c_str(),"NR"); 
  t_tf1->SetRange(t_fitmin,t_fitmax);
  //t_h1->Fit(tfname.c_str(),"NR","",t_fitmin,t_fitmax); 
  t_h1->Fit(tfname.c_str(),"NRQ","",t_fitmin,t_fitmax); 
  //t_h1->Fit(tfname.c_str(),"NRB","",t_fitmin,t_fitmax); 
  //sig[ih] = tf1[ih]->GetParameter(2);
  //sigErr[ih] = tf1[ih]->GetParError(2);
  *t_mean = t_tf1->GetParameter(1);
  *t_sig = t_tf1->GetParameter(2);
  *t_sigErr =  t_tf1->GetParError(2);

  return;
}
