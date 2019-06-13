void check_sortedRootFiles_pt_weight(){
 
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
  
  int true_entries01 = t_true_01->GetEntries();
  int true_entries02 = t_true_02->GetEntries();
  int true_entries03 = t_true_03->GetEntries();
  int true_entries04 = t_true_04->GetEntries();
  cout << "true_entries01 = " << true_entries01 << endl;
  cout << "true_entries02 = " << true_entries02 << endl;
  cout << "true_entries03 = " << true_entries03 << endl;
  cout << "true_entries04 = " << true_entries04 << endl;
 
  int reco_entries01 = t_reco_01->GetEntries();
  int reco_entries02 = t_reco_02->GetEntries();
  int reco_entries03 = t_reco_03->GetEntries();
  int reco_entries04 = t_reco_04->GetEntries();
  cout << "reco_entries01 = " << reco_entries01 << endl;
  cout << "reco_entries02 = " << reco_entries02 << endl;
  cout << "reco_entries03 = " << reco_entries03 << endl;
  cout << "reco_entries04 = " << reco_entries04 << endl;
  
  //// weight 
  float totlumi = 45; //45 /pb
  
  //// LumiCal 15GeV (/pb)
  //float lumi01 = 1.237e-03;
  //float lumi02 = 2.342e-03;
  //float lumi03 = 3.834e-02;
  //float lumi04 = 6.684e-01;
 
  //// LumiCal 0GeV (/pb) 
  //float lumi01 = 7.175e-05;
  //float lumi02 = 7.460e-04;
  //float lumi03 = 2.789e-02;
  //float lumi04 = 5.849e-01;
 
  //// LumiCal 20GeV, 30GeV, 40GeV, 50GeV (/pb)
  //float lumi01 = 6.326e-03;
  //float lumi02 = 1.578e-01;
  //float lumi03 = 2.234e+00;
  //float lumi04 = 4.811e+01;
 
  //// GENOnly noTreeCut (/pb)
  float lumi01 = 0.904758;
  float lumi02 = 19.0015;
  float lumi03 = 339.592;
  float lumi04 = 5903.88;
  
  float weight01 = totlumi/lumi01; 
  float weight02 = totlumi/lumi02; 
  float weight03 = totlumi/lumi03; 
  float weight04 = totlumi/lumi04; 
  
  int xbin = 70;
  float xmin = 0;
  float xmax = 70;
  float ymin = totlumi*1.0e-3; 
  
  TCanvas* c_true_pt = new TCanvas("c_true_pt","c_true_pt",600,600);
  c_true_pt->cd();
  c_true_pt->SetLogy(1);
  TH1D* h_true_pt_01 = new TH1D("h_true_pt_01","",xbin,xmin,xmax);
  TH1D* h_true_pt_02 = new TH1D("h_true_pt_02","",xbin,xmin,xmax);
  TH1D* h_true_pt_03 = new TH1D("h_true_pt_03","",xbin,xmin,xmax);
  TH1D* h_true_pt_04 = new TH1D("h_true_pt_04","",xbin,xmin,xmax);
  h_true_pt_01->Sumw2();
  h_true_pt_02->Sumw2();
  h_true_pt_03->Sumw2();
  h_true_pt_04->Sumw2();
  t_true_01->Draw("true_pt>>h_true_pt_01",Form("%f",weight01));
  t_true_02->Draw("true_pt>>h_true_pt_02",Form("%f",weight02));
  t_true_03->Draw("true_pt>>h_true_pt_03",Form("%f",weight03));
  t_true_04->Draw("true_pt>>h_true_pt_04",Form("%f",weight04));
  SetSoftHistStyle(h_true_pt_01,1,0);
  h_true_pt_01->GetXaxis()->CenterTitle(1);
  h_true_pt_01->GetXaxis()->SetTitle("p_{T,Jet}^{truth} [GeV]");
  h_true_pt_01->SetMinimum(ymin);
  h_true_pt_01->Draw("hist");
  SetSoftHistStyle(h_true_pt_02,2,0);
  //h_true_pt_02->SetLineColor(kRed);
  h_true_pt_02->Draw("hist same");
  SetSoftHistStyle(h_true_pt_03,3,0);
  h_true_pt_03->SetLineColor(kBlue);
  h_true_pt_03->Draw("hist same");
  SetSoftHistStyle(h_true_pt_04,4,0);
  //h_true_pt_04->SetLineColor(kBlue);
  h_true_pt_04->Draw("hist same");
  
  TCanvas* c_reco_pt = new TCanvas("c_reco_pt","c_reco_pt",600,600);
  c_reco_pt->cd();
  c_reco_pt->SetLogy(1);
  TH1D* h_reco_pt_01 = new TH1D("h_reco_pt_01","",xbin,xmin,xmax);
  TH1D* h_reco_pt_02 = new TH1D("h_reco_pt_02","",xbin,xmin,xmax);
  TH1D* h_reco_pt_03 = new TH1D("h_reco_pt_03","",xbin,xmin,xmax);
  TH1D* h_reco_pt_04 = new TH1D("h_reco_pt_04","",xbin,xmin,xmax);
  h_reco_pt_01->Sumw2();
  h_reco_pt_02->Sumw2();
  h_reco_pt_03->Sumw2();
  h_reco_pt_04->Sumw2();
  t_reco_01->Draw("reco_pt>>h_reco_pt_01",Form("%f",weight01));
  t_reco_02->Draw("reco_pt>>h_reco_pt_02",Form("%f",weight02));
  t_reco_03->Draw("reco_pt>>h_reco_pt_03",Form("%f",weight03));
  t_reco_04->Draw("reco_pt>>h_reco_pt_04",Form("%f",weight04));
  SetSoftHistStyle(h_reco_pt_01,1,0);
  h_reco_pt_01->GetXaxis()->CenterTitle(1);
  h_reco_pt_01->GetXaxis()->SetTitle("p_{T,Jet}^{truth} [GeV]");
  h_reco_pt_01->SetMinimum(ymin);
  h_reco_pt_01->Draw("hist");
  SetSoftHistStyle(h_reco_pt_02,2,0);
  //h_reco_pt_02->SetLineColor(kRed);
  h_reco_pt_02->Draw("hist same");
  SetSoftHistStyle(h_reco_pt_03,3,0);
  h_reco_pt_03->SetLineColor(kBlue);
  h_reco_pt_03->Draw("hist same");
  SetSoftHistStyle(h_reco_pt_04,4,0);
  //h_reco_pt_04->SetLineColor(kBlue);
  h_reco_pt_04->Draw("hist same");

  TCanvas* c_true_pt_sliced = new TCanvas("c_true_pt_sliced","c_true_pt_sliced",600,600);
  c_true_pt_sliced->cd();
  c_true_pt_sliced->SetLogy(1);
  TH1D* h_true_pt_sliced_01 = new TH1D("h_true_pt_sliced_01","",xbin,xmin,xmax);
  TH1D* h_true_pt_sliced_02 = new TH1D("h_true_pt_sliced_02","",xbin,xmin,xmax);
  TH1D* h_true_pt_sliced_03 = new TH1D("h_true_pt_sliced_03","",xbin,xmin,xmax);
  TH1D* h_true_pt_sliced_04 = new TH1D("h_true_pt_sliced_04","",xbin,xmin,xmax);
  h_true_pt_sliced_01->Sumw2();
  h_true_pt_sliced_02->Sumw2();
  h_true_pt_sliced_03->Sumw2();
  h_true_pt_sliced_04->Sumw2();
  t_true_01->Draw("true_pt>>h_true_pt_sliced_01",Form("%f*(20<true_pt&&true_pt<30)",weight01));
  t_true_02->Draw("true_pt>>h_true_pt_sliced_02",Form("%f*(30<true_pt&&true_pt<40)",weight02));
  t_true_03->Draw("true_pt>>h_true_pt_sliced_03",Form("%f*(40<true_pt&&true_pt<50)",weight03));
  t_true_04->Draw("true_pt>>h_true_pt_sliced_04",Form("%f*(50<true_pt&&true_pt<60)",weight04));
  SetSoftHistStyle(h_true_pt_sliced_01,1,0);
  h_true_pt_sliced_01->GetXaxis()->CenterTitle(1);
  h_true_pt_sliced_01->GetXaxis()->SetTitle("p_{T,Jet}^{truth} [GeV]");
  h_true_pt_sliced_01->SetMinimum(ymin);
  h_true_pt_sliced_01->Draw("hist");
  SetSoftHistStyle(h_true_pt_sliced_02,2,0);
  //h_true_pt_sliced_02->SetLineColor(kRed);
  h_true_pt_sliced_02->Draw("hist same");
  SetSoftHistStyle(h_true_pt_sliced_03,3,0);
  h_true_pt_sliced_03->SetLineColor(kBlue);
  h_true_pt_sliced_03->Draw("hist same");
  SetSoftHistStyle(h_true_pt_sliced_04,4,0);
  //h_true_pt_sliced_04->SetLineColor(kBlue);
  h_true_pt_sliced_04->Draw("hist same");
  

}
