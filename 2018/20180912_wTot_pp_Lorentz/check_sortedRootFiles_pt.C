void check_sortedRootFiles_pt(){
  TFile* f01 = new TFile("sortedRootFiles/sorted_0to400_QCDJet_G4sPHENIX_jet4_20GeV_Al_R04_doetopcut1_dodphicut1.root","READ");
  TFile* f02 = new TFile("sortedRootFiles/sorted_0to400_QCDJet_G4sPHENIX_jet4_30GeV_Al_R04_doetopcut1_dodphicut1.root","READ");
  TFile* f03 = new TFile("sortedRootFiles/sorted_0to400_QCDJet_G4sPHENIX_jet4_40GeV_Al_R04_doetopcut1_dodphicut1.root","READ");
  TFile* f04 = new TFile("sortedRootFiles/sorted_0to400_QCDJet_G4sPHENIX_jet4_50GeV_Al_R04_doetopcut1_dodphicut1.root","READ");
  TTree* t_true_01 = (TTree*)f01->Get("true_tree");
  TTree* t_true_02 = (TTree*)f02->Get("true_tree");
  TTree* t_true_03 = (TTree*)f03->Get("true_tree");
  TTree* t_true_04 = (TTree*)f04->Get("true_tree");
  TTree* t_reco_01 = (TTree*)f01->Get("reco_tree");
  TTree* t_reco_02 = (TTree*)f02->Get("reco_tree");
  TTree* t_reco_03 = (TTree*)f03->Get("reco_tree");
  TTree* t_reco_04 = (TTree*)f04->Get("reco_tree");
  
  //TH1D* h01 = new TH1D("h01","",100,0.9,1.1);
  //TH1D* h02 = new TH1D("h02","",100,0.9,1.1);
  //t_reco_01->Draw("(reco_cemcEsum+reco_ihcalEsum+reco_ohcalEsum)/reco_e>>h01");
  //t_reco_02->Draw("(reco_cemcEsum+reco_ihcalEsum+reco_ohcalEsum)/reco_e>>h02");
  
  TCanvas* c_e = new TCanvas("c_e","c_e",600,600);
  c_e->cd();
  TH1D* h_true_e_01 = new TH1D("h_true_e_01","",700,0,70);
  TH1D* h_true_e_02 = new TH1D("h_true_e_02","",700,0,70);
  TH1D* h_true_e_03 = new TH1D("h_true_e_03","",700,0,70);
  TH1D* h_true_e_04 = new TH1D("h_true_e_04","",700,0,70);
  t_true_01->Draw("true_e>>h_true_e_01");
  t_true_02->Draw("true_e>>h_true_e_02");
  t_true_03->Draw("true_e>>h_true_e_03");
  t_true_04->Draw("true_e>>h_true_e_04");
  SetSoftHistStyle(h_true_e_01,0,0);
  h_true_e_01->GetXaxis()->CenterTitle(1);
  h_true_e_01->GetXaxis()->SetTitle("E_{Jet}^{truth} [GeV]");
  h_true_e_01->Draw("hist");
  SetSoftHistStyle(h_true_e_02,1,0);
  //h_true_e_02->SetLineColor(kRed);
  h_true_e_02->Draw("hist same");
  SetSoftHistStyle(h_true_e_03,2,0);
  h_true_e_03->SetLineColor(kBlue);
  h_true_e_03->Draw("hist same");
  SetSoftHistStyle(h_true_e_04,3,0);
  //h_true_e_04->SetLineColor(kBlue);
  h_true_e_04->Draw("hist same");

  TCanvas* c_pt = new TCanvas("c_pt","c_pt",600,600);
  c_pt->cd();
  TH1D* h_true_pt_01 = new TH1D("h_true_pt_01","",700,0,70);
  TH1D* h_true_pt_02 = new TH1D("h_true_pt_02","",700,0,70);
  TH1D* h_true_pt_03 = new TH1D("h_true_pt_03","",700,0,70);
  TH1D* h_true_pt_04 = new TH1D("h_true_pt_04","",700,0,70);
  t_true_01->Draw("true_pt>>h_true_pt_01");
  t_true_02->Draw("true_pt>>h_true_pt_02");
  t_true_03->Draw("true_pt>>h_true_pt_03");
  t_true_04->Draw("true_pt>>h_true_pt_04");
  SetSoftHistStyle(h_true_pt_01,0,0);
  h_true_pt_01->GetXaxis()->CenterTitle(1);
  h_true_pt_01->GetXaxis()->SetTitle("p_{T,Jet}^{truth} [GeV]");
  h_true_pt_01->Draw("hist");
  SetSoftHistStyle(h_true_pt_02,1,0);
  //h_true_pt_02->SetLineColor(kRed);
  h_true_pt_02->Draw("hist same");
  SetSoftHistStyle(h_true_pt_03,2,0);
  h_true_pt_03->SetLineColor(kBlue);
  h_true_pt_03->Draw("hist same");
  SetSoftHistStyle(h_true_pt_04,3,0);
  //h_true_pt_04->SetLineColor(kBlue);
  h_true_pt_04->Draw("hist same");

}
