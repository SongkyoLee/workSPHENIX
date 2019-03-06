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
  
  TCanvas* c1 = new TCanvas("c1","",500,500);
  gPad->SetLogy(0);
  h_pt_tot->GetYaxis()->SetRangeUser(0.0,250000);
  h_pt_tot->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  h_pt_tot->Draw();  
  h_pt_pipm->SetLineColor(kRed+1);
  h_pt_pipm->Draw("same");  
  h_pt_epm->SetLineColor(kAzure+1);
  h_pt_epm->Draw("same");  
  h_pt_gamma->SetLineColor(kOrange+1);
  h_pt_gamma->Draw("same");  
  h_pt_proton->SetLineColor(kPink+2);
  h_pt_proton->Draw("same");  
  h_pt_neutron->SetLineColor(kGreen+1);
  h_pt_neutron->Draw("same");  
  h_pt_kpm->SetLineColor(kBlue+1);
//  h_pt_kpm->SetLineStyle(2);
  h_pt_kpm->Draw("same");  
  h_pt_k0->SetLineColor(kViolet+1);
//  h_pt_k0->SetLineStyle(2);
  h_pt_k0->Draw("same");  

  TLegend* leg = new TLegend(0.5,0.7,0.9,0.9);
  leg->SetNColumns(2);
  leg->AddEntry(h_pt_tot,"Total","l");
  leg->AddEntry(h_pt_pipm,"#pi^{#pm}","l");
  leg->AddEntry(h_pt_epm,"e^{#pm}","l");
  leg->AddEntry(h_pt_gamma,"#gamma","l");
  leg->AddEntry(h_pt_proton,"proton","l");
  leg->AddEntry(h_pt_neutron,"neutron","l");
  leg->AddEntry(h_pt_kpm,"K^{#pm}","l");
  leg->AddEntry(h_pt_k0,"K^{0}","l");
  leg->Draw();
  c1->SaveAs("out1DHist/consptl_pt_lin.pdf");


  TCanvas* c2 = new TCanvas("c2","",500,500);
  gPad->SetLogy(1);
  TH1D* h_pt_tot2 = (TH1D*)h_pt_tot->Clone();
  h_pt_tot2->GetYaxis()->SetRangeUser(0.1,250000*10);
  h_pt_tot2->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  h_pt_tot2->Draw();  
  h_pt_pipm->SetLineColor(kRed+1);
  h_pt_pipm->Draw("same");  
  h_pt_epm->SetLineColor(kAzure+1);
  h_pt_epm->Draw("same");  
  h_pt_gamma->SetLineColor(kOrange+1);
  h_pt_gamma->Draw("same");  
  h_pt_proton->SetLineColor(kPink+2);
  h_pt_proton->Draw("same");  
  h_pt_neutron->SetLineColor(kGreen+1);
  h_pt_neutron->Draw("same");  
  h_pt_kpm->SetLineColor(kBlue+1);
//  h_pt_kpm->SetLineStyle(2);
  h_pt_kpm->Draw("same");  
  h_pt_k0->SetLineColor(kViolet+1);
//  h_pt_k0->SetLineStyle(2);
  h_pt_k0->Draw("same");  

  leg->Draw();
  
  c2->SaveAs("out1DHist/consptl_pt_log.pdf");
  
}
