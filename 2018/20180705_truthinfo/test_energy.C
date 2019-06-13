void test_energy(){
  //TFile* f1 = new TFile("testSS310.root","READ"); 
  //TFile* f1 = new TFile("testAl.root","READ"); 
  TFile* f1 = new TFile("testAlframe.root","READ"); 
  TTree* t1 = (TTree*)f1->Get("towerjet4_tree");
  float towerjet4_e=0;
  int towerjet4_cemc_n=0;
  int towerjet4_ihcal_n=0;
  int towerjet4_ohcal_n=0;
  vector<float> *towerjet4_cemc_e=0;
  vector<float> *towerjet4_ihcal_e=0;
  vector<float> *towerjet4_ohcal_e=0;
  t1->SetBranchAddress("towerjet4_e",&towerjet4_e);
  t1->SetBranchAddress("towerjet4_cemc_n",&towerjet4_cemc_n);
  t1->SetBranchAddress("towerjet4_ihcal_n",&towerjet4_ihcal_n);
  t1->SetBranchAddress("towerjet4_ohcal_n",&towerjet4_ohcal_n);
  t1->SetBranchAddress("towerjet4_cemc_e",&towerjet4_cemc_e);
  t1->SetBranchAddress("towerjet4_ihcal_e",&towerjet4_ihcal_e);
  t1->SetBranchAddress("towerjet4_ohcal_e",&towerjet4_ohcal_e);
    
  float cemcEsum=0;
  float ihcalEsum=0;
  float ohcalEsum=0;
  
  cout << "t1->GetEntries() = " << t1->GetEntries()<<endl;
  for (int ij=0; ij<t1->GetEntries(); ij++){
    t1->GetEvent(ij);
    cout << ""<<endl;
    cout << "******* towerjet4_e = " << towerjet4_e << endl;
    cout << "towerjet4_cemc_n = " << towerjet4_cemc_n << endl;
    float cemcEsum=0;
    for (int icemc=0; icemc<towerjet4_cemc_n; icemc++){
      cemcEsum+=towerjet4_cemc_e->at(icemc);
    }
    cout << "cemcEsum = " << cemcEsum << endl;
    
    cout << "towerjet4_ihcal_n = " << towerjet4_ihcal_n << endl;
    float ihcalEsum=0;
    for (int iihcal=0; iihcal<towerjet4_ihcal_n; iihcal++){
      ihcalEsum+=towerjet4_ihcal_e->at(iihcal);
    }
    cout << "ihcalEsum = " << ihcalEsum << endl;

    cout << "towerjet4_ohcal_n = " << towerjet4_ohcal_n << endl;
    float ohcalEsum=0;
    for (int iohcal=0; iohcal<towerjet4_ohcal_n; iohcal++){
      ohcalEsum+=towerjet4_ohcal_e->at(iohcal);
    }
    cout << "ohcalEsum = " << ohcalEsum << endl;
    cout << "****** totalEsum = " << cemcEsum+ihcalEsum+ohcalEsum<<endl;
  }
}
