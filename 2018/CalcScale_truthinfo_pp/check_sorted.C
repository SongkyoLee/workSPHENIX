void check_sorted(){
  TFile* f01 = new TFile("sortedRootFiles/sorted_0to1000_GammaJet_G4sPHENIX_jet4_30GeV_SS310_R04_doetopcut1_dodphicut1.root","READ");
  TFile* f02 = new TFile("sortedRootFiles/sorted_0to1000_GammaJet_G4sPHENIX_jet4_30GeV_Al_R04_doetopcut1_dodphicut1.root","READ");
  TFile* f03 = new TFile("sortedRootFiles/sorted_0to1000_GammaJet_G4sPHENIX_jet4_30GeV_Alframe_R04_doetopcut1_dodphicut1.root","READ");
  //TFile* f02 = new TFile("sortedRootFiles/sorted_0to1000_QCDJet_G4sPHENIX_jet4_50GeV_Al_R02_doetopcut1_dodphicut1.root","READ");
  //TFile* f02 = new TFile("sortedRootFiles/sorted_0to1000_QCDJet_G4sPHENIX_jet4_50GeV_Alframe_R04_doetopcut1_dodphicut1.root","READ");
  //TFile* f02 = new TFile("/Users/songkyolee/Documents/z_20180410_diffE_vtx0_gammaEMCALonly_eta045/sortedRootFiles/sorted_0to1000_QCDJet_G4sPHENIX_jet4_50GeV_Alframe_doetopcut1_dodphicut1.root","READ");
  //TFile* f01 = new TFile("/Users/songkyolee/Documents/z_20180630_diffE_Al/sortedRootFiles/sorted_0to1000_QCDJet_G4sPHENIX_jet4_50GeV_Al_R04_doetopcut1_dodphicut1.root","READ");
  //TFile* f02 = new TFile("/Users/songkyolee/Documents/z_20180630_diffE_Al/sortedRootFiles/sorted_0to1000_QCDJet_G4sPHENIX_jet4_50GeV_Alframe_R04_doetopcut1_dodphicut1.root","READ");
  TTree* t_true_SS310 = (TTree*)f01->Get("true_tree");
  TTree* t_true_Al = (TTree*)f02->Get("true_tree");
  TTree* t_true_Alframe = (TTree*)f03->Get("true_tree");
  TTree* t_reco_SS310 = (TTree*)f01->Get("reco_tree");
  TTree* t_reco_Al = (TTree*)f02->Get("reco_tree");
  TTree* t_reco_Alframe = (TTree*)f03->Get("reco_tree");
  
  //TH1D* h01 = new TH1D("h01","",100,0.9,1.1);
  //TH1D* h02 = new TH1D("h02","",100,0.9,1.1);
  //t_reco_SS310->Draw("(reco_cemcEsum+reco_ihcalEsum+reco_ohcalEsum)/reco_e>>h01");
  //t_reco_Al->Draw("(reco_cemcEsum+reco_ihcalEsum+reco_ohcalEsum)/reco_e>>h02");
  
  TCanvas* c01 = new TCanvas("c01","c01",600,600);
  c01->cd();
  TH1D* h01 = new TH1D("h01","",100,0,60);
  TH1D* h02 = new TH1D("h02","",100,0,60);
  TH1D* h03 = new TH1D("h03","",100,0,60);
  t_reco_SS310->Draw("reco_e>>h01");
  t_reco_Al->Draw("reco_e>>h02");
  t_reco_Alframe->Draw("reco_e>>h03");
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
  t_reco_SS310->Draw("reco_eta>>h04","reco_eta!=0");
  t_reco_Al->Draw("reco_eta>>h05","reco_eta!=0");
  t_reco_Alframe->Draw("reco_eta>>h06","reco_eta!=0");
  h04->Draw("hist");
  h05->SetLineColor(kRed);
  h05->Draw("hist same");
  h06->SetLineColor(kBlue);
  h06->Draw("hist same");

  //TCanvas* c05 = new TCanvas("c05","c05",600,600);
  //c05->cd();
  //TH1D* h05 = new TH1D("h05","",100,-1.,1.);
  //TH1D* h06 = new TH1D("h06","",100,-1.,1.);
  //t_true_SS310->Draw("true_eta>>h05");
  //t_true_Al->Draw("true_eta>>h06");
  //h05->Draw("hist");
  //h06->SetLineColor(kRed);
  //h06->Draw("hist same");

}
