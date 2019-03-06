void draw_emfrac(){

  TFile* f1 = new TFile("./2ndSortedRootFiles/2ndSorted_0to300_GammaJet_G4sPHENIX_jet4_allGeV_SS310_doetopcut1_dodphicut1_ver4.root","READ");
  TTree* t1 = (TTree*)f1->Get("out_tree");

  TH1D* h1 = new TH1D("h1",";Truth EM fraction;Counts",101,-0.05,1.05);
  TH1D* h2 = new TH1D("h2",";Truth EM fraction;Counts",101,-0.05,1.05);
  t1->Draw("true_gamma_emfrac>>h1");
  t1->Draw("true_jet_emfrac>>h2");

  TCanvas* c1 = new TCanvas("c1","",500,500);
  gPad->SetLogy(1);
  h1->SetLineColor(kRed);
  h1->Draw();
  h2->SetLineColor(kBlue);
  h2->Draw("same");
}
