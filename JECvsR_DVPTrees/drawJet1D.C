void drawJet1D(int inputType = 0, string jetType = "truthjet5", string varType="pt"){

  // select input file
  string szInput;
  if (inputType ==0) { szInput = "DefaultIHCal"; }  
  else if (inputType ==1) { szInput = "AlIHCal"; }  
  else if (inputType ==2) { szInput = "AlIHCalNoReadout"; }  
  else { cout << "Error: choose among 0:Default, 1:Al, or 2: ALNoRO!"; return; }
  
  // select jet and varialbes
  string szVar = jetType + "_" + varType;
  cout << "szVar : " << szVar.c_str() << endl;
 
  // select bin range
  int numbin;
  double binmin, binmax;
  if (varType.compare("pt")==0) {numbin=100; binmin=0; binmax=100;}
  else if (varType.compare("eta")==0) {numbin=30; binmin=-1.5; binmax=1.5;}
  else if (varType.compare("phi")==0) {numbin=33; binmin=-3.3; binmax=3.3;}
  else { cout << "Error: choose among pt, eta, or phi"; return; }
  

  // hist
  TH1D* h01 = new TH1D("h01","",numbin,binmin,binmax);
  
  // open file
  TFile* f01 = new TFile(Form("sPHENIX-Sep17-QCD35-10k-%s.root",szInput.c_str()));
  TTree* ttree = (TTree*)f01->Get("ttree");
  int treeEntry = ttree->GetEntries();
  //int treeEntry = 10;

  // set branch address && read
  int num;
  TBranch *b_num;
  float var[531];
  TBranch *b_var;

  ttree->SetBranchAddress(Form("%s_n",jetType.c_str()),&num,&b_num);
  ttree->SetBranchAddress(Form("%s",szVar.c_str()),&var,&b_var);

  for (int iev=0;iev<treeEntry;iev++){
    ttree->GetEntry(iev);
    cout << iev<<"th number of jet = " << num << endl;
    for (int ij=0; ij<num; ij++){
      cout << "  *" << ij << "th var = " << var[ij] << endl;    
      h01->Fill(var[ij]);
    }
  }
  
  TCanvas *c1 = new TCanvas("c1","c1",600,600);
  c1->cd();
  
//  ttree->Draw(Form("%s>>h01",szVar.c_str()));
  h01->GetXaxis()->SetTitle(szVar.c_str());
  h01->GetXaxis()->CenterTitle(1);
  h01->Draw();
  cout << "numEntry = " << h01->GetEntries() << endl;

  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(32);
  latex->SetTextSize(0.04);
  
  latex->DrawLatex(0.9,0.8,szInput.c_str());

  gStyle->SetOptStat(1);
  c1->SaveAs(Form("OutJet1D/1D_%s_%s.pdf",szInput.c_str(),szVar.c_str()));

  return;
}
