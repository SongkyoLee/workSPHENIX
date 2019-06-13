void check_2ndSortedRootFiles_pt_weight(){
 
  //TFile* f01 = new TFile("2ndSortedRootFilesPtBinningMerged/2ndSorted_0to400_QCDJet_G4sPHENIX_jet4_noGeV_Al_R04_doetopcut1_dodphicut1.root","READ");
  TFile* f01 = new TFile("2ndSortedRootFilesPtBinningMerged/2ndSorted_0to400_QCDJet_G4sPHENIX_jet4_noGeV_Alframe_R04_doetopcut1_dodphicut1.root","READ");
  TTree* t_out_01 = (TTree*)f01->Get("out_tree");
  
  int out_entries01 = t_out_01->GetEntries();
  cout << "out_entries01 = " << out_entries01 << endl;
  
  int xbin = 70;
  float xmin = 0;
  float xmax = 70;
  float totlumi = 45;
  //float ymin = totlumi*1.0e-1; 
  float ymin = 1.0e-3; 
  float ymax = 1.0e7; 
  
  TCanvas* c_true_pt = new TCanvas("c_true_pt","c_true_pt",600,600);
  c_true_pt->cd();
  c_true_pt->SetLogy(1);
  TH1D* h_true_pt_01 = new TH1D("h_true_pt_01","",xbin,xmin,xmax);
  h_true_pt_01->Sumw2();
  //t_out_01->Draw("true_jet_pt>>h_true_pt_01");
  t_out_01->Draw("true_jet_pt>>h_true_pt_01","file_weight");
  SetSoftHistStyle(h_true_pt_01,1,0);
  h_true_pt_01->GetXaxis()->CenterTitle(1);
  h_true_pt_01->GetXaxis()->SetTitle("p_{T,Jet}^{truth} [GeV]");
  h_true_pt_01->SetMinimum(ymin);
  h_true_pt_01->SetMaximum(ymax);
  h_true_pt_01->Draw("hist");
  
  TCanvas* c_reco_pt = new TCanvas("c_reco_pt","c_reco_pt",600,600);
  c_reco_pt->cd();
  c_reco_pt->SetLogy(1);
  TH1D* h_reco_pt_01 = new TH1D("h_reco_pt_01","",xbin,xmin,xmax);
  h_reco_pt_01->Sumw2();
  //t_out_01->Draw("reco_jet_pt>>h_reco_pt_01");
  t_out_01->Draw("reco_jet_pt>>h_reco_pt_01","file_weight");
  SetSoftHistStyle(h_reco_pt_01,1,0);
  h_reco_pt_01->GetXaxis()->CenterTitle(1);
  h_reco_pt_01->GetXaxis()->SetTitle("p_{T,Jet}^{truth} [GeV]");
  h_reco_pt_01->SetMinimum(ymin);
  h_reco_pt_01->SetMaximum(ymax);
  h_reco_pt_01->Draw("hist");
 
  TCanvas* c_true_e = new TCanvas("c_true_e","c_true_e",600,600);
  c_true_e->cd();
  c_true_e->SetLogy(1);
  TH1D* h_true_e_01 = new TH1D("h_true_e_01","",xbin,xmin,xmax);
  h_true_e_01->Sumw2();
  //t_out_01->Draw("true_jet_e>>h_true_e_01");
  t_out_01->Draw("true_jet_e>>h_true_e_01","file_weight");
  SetSoftHistStyle(h_true_e_01,1,0);
  h_true_e_01->GetXaxis()->CenterTitle(1);
  h_true_e_01->GetXaxis()->SetTitle("p_{T,Jet}^{truth} [GeV]");
  h_true_e_01->SetMinimum(ymin);
  h_true_e_01->SetMaximum(ymax);
  h_true_e_01->Draw("hist");
  
  TCanvas* c_reco_e = new TCanvas("c_reco_e","c_reco_e",600,600);
  c_reco_e->cd();
  c_reco_e->SetLogy(1);
  TH1D* h_reco_e_01 = new TH1D("h_reco_e_01","",xbin,xmin,xmax);
  h_reco_e_01->Sumw2();
  //t_out_01->Draw("reco_jet_e>>h_reco_e_01");
  t_out_01->Draw("reco_jet_e>>h_reco_e_01","file_weight");
  SetSoftHistStyle(h_reco_e_01,1,0);
  h_reco_e_01->GetXaxis()->CenterTitle(1);
  h_reco_e_01->GetXaxis()->SetTitle("p_{T,Jet}^{truth} [GeV]");
  h_reco_e_01->SetMinimum(ymin);
  h_reco_e_01->SetMaximum(ymax);
  h_reco_e_01->Draw("hist");


  return;
}
