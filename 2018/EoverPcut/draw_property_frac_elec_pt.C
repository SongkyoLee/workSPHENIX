{
 TFile* f1 = new TFile("sortedRootFiles_old/sorted_0to1000_QCDJet_G4sPHENIX_jet4_50GeV_SS310_doetopcut1_dodphicut1_inclusive.root", "READ");
 TH1D* h_pt_tot = (TH1D*)f1->Get("h_pt_tot");
 TH1D* h_pt_pipm = (TH1D*)f1->Get("h_pt_pipm");
 TH1D* h_pt_epm = (TH1D*)f1->Get("h_pt_epm");
 TH1D* h_pt_gamma = (TH1D*)f1->Get("h_pt_gamma");
 TH1D* h_pt_proton = (TH1D*)f1->Get("h_pt_proton");
 TH1D* h_pt_neutron = (TH1D*)f1->Get("h_pt_neutron");
 TH1D* h_pt_kpm = (TH1D*)f1->Get("h_pt_kpm");
 TH1D* h_pt_k0 = (TH1D*)f1->Get("h_pt_k0");

  //// had vs. em
  h_pt_pipm->Add(h_pt_proton);
  h_pt_pipm->Add(h_pt_kpm);
  //h_pt_epm->Add(h_pt_gamma);
  h_pt_neutron->Add(h_pt_k0);
  
  //int rb = 100; 
  //int rb = 10; 
  int rb = 1; 
  h_pt_tot->Rebin(rb);
  h_pt_pipm->Rebin(rb);
  h_pt_epm->Rebin(rb);
  h_pt_gamma->Rebin(rb);
  h_pt_neutron->Rebin(rb);
  //h_pt_tot->Divide(h_pt_tot);
  h_pt_pipm->Divide(h_pt_tot);
  h_pt_epm->Divide(h_pt_tot);
  h_pt_gamma->Divide(h_pt_tot);
  h_pt_neutron->Divide(h_pt_tot);
    
  TCanvas* c1 = new TCanvas("c1","",500,500);
  gPad->SetLogy(0);
  h_pt_tot->GetYaxis()->SetRangeUser(0,1);
  h_pt_tot->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  h_pt_tot->Draw();  
  h_pt_pipm->SetLineColor(kRed+1);
  h_pt_pipm->SetFillColor(kRed+1);
  h_pt_pipm->SetFillStyle(3454);
  h_pt_pipm->Draw("same");  
  h_pt_epm->SetLineColor(kOrange+1);
  h_pt_epm->SetFillColor(kOrange+1);
  h_pt_epm->SetFillStyle(3495);
  h_pt_epm->Draw("same");  
  h_pt_gamma->SetLineColor(kAzure+1);
  h_pt_gamma->SetFillColor(kAzure+1);
  h_pt_gamma->SetFillStyle(3445);
  h_pt_gamma->Draw("same");  
  h_pt_neutron->SetLineColor(kGreen+1);
  h_pt_neutron->SetFillColor(kGreen+1);
  h_pt_neutron->SetFillStyle(3459);
  h_pt_neutron->Draw("same");  

  //TLegend* leg = new TLegend(0.5,0.7,0.9,0.9);
  //leg->SetNColumns(2);
  TLegend* leg = new TLegend(0.45,0.4,0.92,0.65);
  leg->AddEntry(h_pt_pipm,"charged hadron","l");
  leg->AddEntry(h_pt_epm,"e^{#pm}","l");
  leg->AddEntry(h_pt_gamma,"#gamma","l");
  leg->AddEntry(h_pt_neutron,"neutral hadron","l");
  leg->Draw();
  c1->SaveAs("out1DHist/property_fract_pt_lin.pdf");

  TCanvas* c2 = new TCanvas("c2","",500,500);
  //// stack
  THStack *hs = new THStack("hs","");
  h_pt_tot->Draw();  
  hs->Add(h_pt_pipm); 
  hs->Add(h_pt_gamma); 
  hs->Add(h_pt_epm); 
  hs->Add(h_pt_neutron); 
  //hs->GetYaxis()->SetRangeUser(0,1);
  hs->Draw("same");

  TLegend* leg2 = new TLegend(0.45,0.2,0.92,0.45);
  leg2->AddEntry(h_pt_pipm,"charged hadron","l");
  leg2->AddEntry(h_pt_epm,"e^{#pm}","l");
  leg2->AddEntry(h_pt_gamma,"#gamma","l");
  leg2->AddEntry(h_pt_neutron,"neutral hadron","l");
  leg2->Draw();
  
  c2->SaveAs("out1DHist/property_fract_elec_pt_log.pdf");

}
