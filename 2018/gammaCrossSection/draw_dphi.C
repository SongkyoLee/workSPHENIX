void draw_dphi(){

  TFile* f1 = new TFile("./2ndSortedRootFiles/2ndSorted_0to310_GammaJet_G4sPHENIX_jet4_allGeV_SS310_doetopcut1_dodphicut1_ver2.root","READ");
  TFile* f2 = new TFile("./2ndSortedRootFiles/2ndSorted_0to310_GammaJet_G4sPHENIX_jet4_allGeV_SS310_doetopcut1_dodphicut1_ver3.root","READ");

  TTree* t1 = (TTree*)f1->Get("out_tree");
  TTree* t2 = (TTree*)f2->Get("out_tree");

  TH1D* h1 = new TH1D("h1",";#Delta#phi(#gamma-Jet);Counts",100,0,3.2);
  TH1D* h2 = new TH1D("h2",";#Delta#phi(#gamma-Jet);Counts",100,0,3.2);
  t1->Draw("reco_dPhi>>h1");
  t2->Draw("reco_dPhi>>h2");

  TCanvas* c1 = new TCanvas("c1","",500,500);
  gPad->SetLogy(1);
  h1->Draw();
  h2->SetLineColor(kRed);
  h2->Draw("same");
}
