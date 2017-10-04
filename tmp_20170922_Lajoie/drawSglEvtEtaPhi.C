void drawSglEvtEtaPhi(){
 TFile* f01 = new TFile("jet_analysis_9.root");
 TTree* event = (TTree*)f01->Get("event");
 TTree* tracks = (TTree*)f01->Get("tracks");
 TTree* prim_jets = (TTree*)f01->Get("prim_jets");
 TTree* trk_jets = (TTree*)f01->Get("trk_jets");
 TTree* twr_jets = (TTree*)f01->Get("twr_jets");
 TTree* matched_jets = (TTree*)f01->Get("matched_jets");
 TH1D* h01 = new TH1D("h01","h01",8001,-4000,4000);
 TH2D* h2D_01 = new TH2D("h2D_01",";#eta;#phi",50,-1.2,1.2,50,0,6.28);
 TH2D* h2D_02 = new TH2D("h2D_02",";#eta;#phi",50,-1.2,1.2,50,0,6.28);
 TH2D* h2D_03 = new TH2D("h2D_03",";#eta;#phi",50,-1.2,1.2,50,0,6.28);
 TH2D* h2D_04 = new TH2D("h2D_04",";#eta;#phi",50,-1.2,1.2,50,0,6.28);
 TH2D* h2D_05 = new TH2D("h2D_05",";#eta;#phi",50,-1.2,1.2,50,0,6.28);
 TH2D* h2D_06 = new TH2D("h2D_06",";#eta;#phi",50,-1.2,1.2,50,0,6.28);
 TH2D* h2D_07 = new TH2D("h2D_07",";#eta;#phi",50,-1.2,1.2,50,0,6.28);
 TH2D* h2D_08 = new TH2D("h2D_08",";#eta;#phi",50,-1.2,1.2,50,0,6.28);
  //h02->GetXaxis()->CenterTitle(1);
  //h02->GetYaxis()->CenterTitle(1);
  
  //TCanvas*c1 = new TCanvas("c1","",500,500);
  TCanvas*c1 = new TCanvas("c1","",1200,600);
  c1->Divide(4,2);
  //tracks->Draw("gflavor>>h01","event==6");
  gPad->SetRightMargin(2.5);
  
  //// Draw
  //prim_jets->Draw("jet_phi:jet_eta>>h02","event==6","colz");
  //prim_jets->Draw("jet_phi:jet_eta>>h02","event==6","lego2");
  
  //// Draw with Weight (ptot)
  c1->cd(1);
  prim_jets->Draw("jet_phi:jet_eta>>h2D_01","jet_ptot*(event==6)","colz");
  c1->cd(5);
  prim_jets->Draw("jet_phi:jet_eta>>h2D_05","jet_ptot*(event==6)","lego2");
  
  ///////////////////////////////////////////

  c1->cd(2);
  trk_jets->Draw("jet_phi:jet_eta>>h2D_02","jet_ptot*(event==6)","colz");
  c1->cd(6);
  trk_jets->Draw("jet_phi:jet_eta>>h2D_06","jet_ptot*(event==6)","lego2");
  
  ///////////////////////////////////////////
  c1->cd(3);
  twr_jets->Draw("jet_phi:jet_eta>>h2D_03","jet_ptot*(event==6)","colz");
  c1->cd(7);
  twr_jets->Draw("jet_phi:jet_eta>>h2D_07","jet_ptot*(event==6)","lego2");
//  c1->Update();
  ///////////////////////////////////////////
  c1->cd(4);
  matched_jets->Draw("jet_p_phi:jet_p_eta>>h2D_04","jet_p_ptot*(event==6)","colz");
  c1->cd(8);
  matched_jets->Draw("jet_p_phi:jet_p_eta>>h2D_08","jet_p_ptot*(event==6)","lego2");
  //matched_jets->Draw("jet_trk_phi:jet_trk_eta>>h02","jet_trk_ptot*(event==6)","colz");
  //matched_jets->Draw("jet_twr_phi:jet_twr_eta>>h02","jet_twr_ptot*(event==6)","colz");
  //twr_jets->Draw("jet_phi:jet_eta>>h02","jet_ptot*(event==6)","lego2");


}
