void recursiveFit(TH1D* t_h1, TF1* t_tf1, float t_fitmin, float t_fitmax, float* t_mean, float* t_sig, float* t_sigErr);

void draw_fit_emi_etop(){
  //TFile* f1 = new TFile("sorted_0to2000_G4sPHENIX_emi_0to25GeV_SS310.root","READ");
  TFile* f1 = new TFile("sortedRootFiles_old/sorted_0to2000_G4sPHENIX_emi_0to25GeV_SS310.root","READ");
  TTree* trk_tree = (TTree*)f1->Get("trk_tree");
  
  float fitmin = 0.5;
  float fitmax = 1.5;

  //const int nhist = 8;
  //float xval[nhist] = {1.5, 4.5, 7.5, 10.5, 13.5, 16.5, 19.5, 22.5};
  //float xmin[nhist];
  //float xmax[nhist];
  //const int nhist = 25;
  const int nhist = 17;
  float xval[nhist];
  float xmin[nhist] = {0,1,2,3,4,5,6,7,8,9,10,12,14,16,18,20,22};
  float xmax[nhist] = {1,2,3,4,5,6,7,8,9,10,12,14,16,18,20,22,24};
  //const int nhist = 12;
  //float xval[nhist];
  //float xmin[nhist] = {0,2,4,6,8,10,12,14,16,18,20,22};
  //float xmax[nhist] = {2,4,6,8,10,12,14,16,18,20,22,24};
  float ex[nhist]; 
  float mean[nhist]; 
  float sig[nhist]; 
  float sigErr[nhist]; 

  TH1D*h01[nhist];
  
  TCanvas* ctmp = new TCanvas("ctmp","",100,100);
  
  for (int ih=0; ih<nhist; ih++){
    h01[ih] = new TH1D(Form("h01_%.0fto%.0f",xmin[ih],xmax[ih]),"",50,0.0,2.0);
    //xmin[ih] = xval[ih]-1.5;
    //xmax[ih] = xval[ih]+1.5;
    //xval[ih] = 0.5+ih;
    //ex[ih] = 0.5;
    //xmin[ih] = xval[ih]-ex[ih];
    //xmax[ih] = xval[ih]+ex[ih];
    xval[ih] = (xmin[ih]+xmax[ih])/2.; 
    ex[ih] = (xmax[ih]-xmin[ih])/2.;
    cout << ih<< "th bin from " << xmin[ih] << ", to "<<xmax[ih] << endl;
    trk_tree->Draw(Form("trk_cemc_cl_E/trk_p>>h01_%.0fto%.0f",xmin[ih],xmax[ih]),Form("trk_p>%f && trk_p < %f",xmin[ih],xmax[ih])); 
  }
 
  TCanvas* c1 = new TCanvas("c1","",500,500);
  gPad->SetLogy(0);
  
  for (int ih=0; ih<nhist; ih++){
    if (ih<10) { SetHistStyle(h01[ih],ih,10);}
    else if (ih<20) { SetHistStyle(h01[ih],ih-10,11);}
    else if (ih<30) { SetHistStyle(h01[ih],ih-20,12);}
    else{ SetHistStyle(h01[ih],ih,10);}
    h01[ih]->GetXaxis()->CenterTitle(1);
    h01[ih]->GetXaxis()->SetTitle("E_{EMCAL}/p_{track}");
    h01[ih]->SetMarkerSize(0.5);
    h01[ih]->Scale(1./h01[ih]->Integral()); 
    if (ih==0) {
      //h01[ih]->SetMaximum(0.15);
      h01[ih]->SetMaximum(0.23);
      h01[ih]->Draw("ep");
    } else {
      h01[ih]->Draw("ep same");
      //if (ih<5) h01[ih]->Draw("same");
      //if (ih>=5 && ih < 10) h01[ih]->Draw("same");
      //if (ih>=10 && ih < 15) h01[ih]->Draw("same");
    }
  }
  
  TF1* tf1[nhist];
  float col;
  
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextSize(0.04);
  
  for (int ih=0; ih<nhist; ih++){
    tf1[ih]= new TF1(Form("tf1_%d",ih),"gaus",fitmin,fitmax);
    col = h01[ih]->GetMarkerColor();
    tf1[ih]->SetLineColor(col);
    //h01[ih]->Fit(Form("tf1_%d",ih),"NR"); 
    //tf1[ih]->Draw("same");
    //sig[ih] = tf1[ih]->GetParameter(2);
    //sigErr[ih] = tf1[ih]->GetParError(2);
    //cout << "sig[ih] = " << sig[ih] << endl;
    //cout << "sigErr[ih] = " << sigErr[ih] << endl;
    recursiveFit(h01[ih], tf1[ih],fitmin, fitmax, &mean[ih], &sig[ih], &sigErr[ih]);
    int repeat = 30;
    for (int ir=0; ir<repeat; ir++){
      fitmin = mean[ih]-2*sig[ih];
      fitmax = mean[ih]+2*sig[ih];
      recursiveFit(h01[ih], tf1[ih],fitmin, fitmax, &mean[ih], &sig[ih], &sigErr[ih]);
    }
    tf1[ih]->Draw("same");
    
    //latex->DrawLatex(0.67,0.9-0.05*ih,Form("sig[%d] = %.4f",ih,sig[ih])); 
    cout << "h01["<<ih<<"] : " << h01[ih]->GetEntries() << endl;
  }
  c1->SaveAs("emi_etop.pdf");
  //c1->SaveAs("out1DHist/emi_etop.pdf");
 
  ////////////////////////////////
   
  TGraphErrors* g1 = new TGraphErrors(nhist,xval, mean, ex, sigErr);  
  TCanvas* c2 = new TCanvas("c2","",500,500);
  g1->GetXaxis()->SetTitle("track p (GeV/c)");
  g1->GetXaxis()->CenterTitle(1);
  g1->GetYaxis()->SetTitle("#mu");
  g1->GetYaxis()->CenterTitle(1);
  g1->SetMinimum(0.9);
  g1->SetMaximum(1.1);
  g1->Draw("ap");
  c2->SaveAs("emi_etop_mean.pdf");
  //c2->SaveAs("out1DHist/emi_etop_mean.pdf");
  
  ////////////////////////////////
   
  TGraphErrors* g2 = new TGraphErrors(nhist,xval, sig, ex, sigErr);  
  TCanvas* c3 = new TCanvas("c3","",500,500);
  g2->GetXaxis()->SetTitle("track p (GeV/c)");
  g2->GetXaxis()->CenterTitle(1);
  g2->GetYaxis()->SetTitle("#sigma");
  g2->GetYaxis()->CenterTitle(1);
  g2->SetMinimum(0.);
  g2->SetMaximum(0.2);
  g2->Draw("ap");

//  TF1* tf2 = new TF1("tf2","[0]/(exp((-x+[1])/[2]+1)+[3])+[4]",0,10);
//  TF1* tf2 = new TF1("tf2","[0]*x*x*x+[1]*x*x+[2]*x+[3]",0,5);
//  TF1* tf2 = new TF1("tf2","exp([0]+[1]*x/[2])",0.1,24);
//  TF1* tf2 = new TF1("tf2","exp(([0]+[1]*x)/[2])",0.1,24);
//  TF1* tf2 = new TF1("tf2","exp(([0]+[1]*x)/[2])",0.1,24);
//  tf2->SetParLimits(0,-10,10);
//  tf2->SetParLimits(1,-10, 10);
//  tf2->SetParLimits(2,-10,10);
//  tf2->SetParLimits(3,-10,10);
//  tf2->SetParLimits(4,-10,10);
//  tf2->SetParameter(2,10);
//  tf2->SetLineColor(kRed);
//  g2->Fit("tf2","R"); 
//  g2->Draw("ap");
//  tf2->Draw(""); 
//  g2->Draw("p");

  c3->SaveAs("emi_etop_sigma.pdf");
  //c3->SaveAs("out1DHist/emi_etop_sigma.pdf");
   
  TFile* fout = new TFile("emi_etop.root","RECREATE");
  //TFile* fout = new TFile("out1DHist/emi_etop.root","RECREATE");
  for (int ih=0; ih<nhist;ih++) {
    h01[ih]->Write();
  } 
  g1->SetName("g_mean");
  g2->SetName("g_sigma");
  g1->Write();
  g2->Write();
   
  return;
}

void recursiveFit(TH1D* t_h1, TF1* t_tf1, float t_fitmin, float t_fitmax, float* t_mean, float* t_sig, float* t_sigErr){
  string tfname = t_tf1->GetName();
  //t_h1[ih]->Fit(Form("tf1_%d",ih),"NR"); 
  //t_h1->Fit(tfname.c_str(),"NR"); 
  t_tf1->SetRange(t_fitmin,t_fitmax);
  t_h1->Fit(tfname.c_str(),"NR","",t_fitmin,t_fitmax); 
  //sig[ih] = tf1[ih]->GetParameter(2);
  //sigErr[ih] = tf1[ih]->GetParError(2);
  *t_mean = t_tf1->GetParameter(1);
  *t_sig = t_tf1->GetParameter(2);
  *t_sigErr =  t_tf1->GetParError(2);

  return;
}
