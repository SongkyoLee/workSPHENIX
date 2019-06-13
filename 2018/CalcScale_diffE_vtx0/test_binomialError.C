{
  double k = 30;
  double n = 100;
  double eff01 = k/n;
  cout << "eff01 : " << eff01 << endl;
  double err01 = (1/n)*sqrt(k*(1-k/n));
  cout << "err01 : " << err01 << endl;

  TH1D* hden = new TH1D("hden","hden",1,0,1);
  TH1D* hnum = new TH1D("hnum","hnum",1,0,1);
  hden->Sumw2();
  hnum->Sumw2();
  //hden->SetBinContent(1,n);
  //hnum->SetBinContent(1,k);
  for (int i=0;i<k;i++){
    hnum->Fill(0.5);
  }
  for (int i=0;i<n;i++){
    hden->Fill(0.5);
  }

  TCanvas* c1 = new TCanvas("c1","c1",1000,500);
  c1->Divide(2,1);
  c1->cd(1);
  hden->SetMinimum(0);
  hden->SetMaximum(120);
  hden->Draw("pe");
  hnum->Draw("pe same");
  c1->cd(2);
  TH1D* heff = (TH1D*)hnum->Clone("heff");
  heff->Divide(hnum,hden,1,1,"B");
  heff->Draw("pe");
  
  double eff02 = heff->GetBinContent(1);
  cout << "eff02 : " << eff02 << endl;
  double err02 = heff->GetBinError(1);
  cout << "err02 : " << err02 << endl;
}
