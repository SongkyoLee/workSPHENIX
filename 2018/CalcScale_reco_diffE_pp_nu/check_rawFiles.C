void check_rawFiles(){
  //TFile* f01 = new TFile("DVP_QCD35_10k_G4sPHENIX_File_0_SS310.root","READ");
  //TFile* f02 = new TFile("DVP_QCD35_10k_G4sPHENIX_File_0_Al.root","READ");
  //TFile* f03 = new TFile("DVP_QCD35_10k_G4sPHENIX_File_0_Alframe.root","READ");
  TFile* f01 = new TFile("mergesSS310.root","READ");
  TFile* f02 = new TFile("mergesAl.root","READ");
  TFile* f03 = new TFile("mergesAlframe.root","READ");
  TTree* t_true_SS310 = (TTree*)f01->Get("truthjet4_tree");
  TTree* t_true_Al = (TTree*)f02->Get("truthjet4_tree");
  TTree* t_true_Alframe = (TTree*)f03->Get("truthjet4_tree");
  TTree* t_reco_SS310 = (TTree*)f01->Get("towerjet4_tree");
  TTree* t_reco_Al = (TTree*)f02->Get("towerjet4_tree");
  TTree* t_reco_Alframe = (TTree*)f03->Get("towerjet4_tree");
  
  //TH1D* h01 = new TH1D("h01","",100,0.9,1.1);
  //TH1D* h02 = new TH1D("h02","",100,0.9,1.1);
  //t_reco_SS310->Draw("(reco_cemcEsum+reco_ihcalEsum+reco_ohcalEsum)/reco_e>>h01");
  //t_reco_Al->Draw("(reco_cemcEsum+reco_ihcalEsum+reco_ohcalEsum)/reco_e>>h02");
  
  TCanvas* c01 = new TCanvas("c01","c01",600,600);
  c01->cd();
  TH1D* h01 = new TH1D("h01","",100,0,60);
  TH1D* h02 = new TH1D("h02","",100,0,60);
  TH1D* h03 = new TH1D("h03","",100,0,60);
  t_reco_SS310->Draw("towerjet4_e>>h01");
  t_reco_Al->Draw("towerjet4_e>>h02");
  t_reco_Alframe->Draw("towerjet4_e>>h03");
  h01->Draw("hist");
  h02->SetLineColor(kRed);
  h02->Draw("hist same");
  h03->SetLineColor(kBlue);
  h03->Draw("hist same");

  TCanvas* c04 = new TCanvas("c04","c04",600,600);
  c04->cd();
  TH1D* h04 = new TH1D("h04","",100,-1.,1.);
  TH1D* h05 = new TH1D("h05","",100,-1.,1.);
  TH1D* h06 = new TH1D("h06","",100,-1.,1.);
  t_reco_SS310->Draw("towerjet4_eta>>h04");
  t_reco_Al->Draw("towerjet4_eta>>h05");
  t_reco_Alframe->Draw("towerjet4_eta>>h06");
  h04->Draw("hist");
  h05->SetLineColor(kRed);
  h05->Draw("hist same");
  h06->SetLineColor(kBlue);
  h06->Draw("hist same");

  TCanvas* c07 = new TCanvas("c07","c07",600,600);
  c07->cd();
  TH1D* h07 = new TH1D("h07","",100,-1.,1.);
  TH1D* h08 = new TH1D("h08","",100,-1.,1.);
  TH1D* h09 = new TH1D("h09","",100,-1.,1.);
  t_true_SS310->Draw("truthjet4_eta>>h07");
  t_true_Al->Draw("truthjet4_eta>>h08");
  t_true_Alframe->Draw("truthjet4_eta>>h09");
  h07->Draw("hist");
  h08->SetLineColor(kRed);
  h08->Draw("hist same");
  h09->SetLineColor(kBlue);
  h09->Draw("hist same");

}
