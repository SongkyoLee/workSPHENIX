void check_frac(){

  //string fname = "out_GENONLY_QCDJet20GeVR04_noTreeCut/merged.root";
  //string fname = "out_GENONLY_QCDJet30GeVR04_noTreeCut/merged.root";
  //string fname = "out_GENONLY_QCDJet40GeVR04_noTreeCut/merged.root";
  string fname = "out_GENONLY_QCDJet50GeVR04_noTreeCut/merged.root";
  
  TFile* f1 = new TFile(fname.c_str(),"READ");
  TTree* t1 = (TTree*)f1->Get("truthjet4_tree");
  float den = t1->GetEntries("truthjet4_pt>0");
  float num = t1->GetEntries("truthjet4_pt>5");
  cout << " t1->GetEntries() = " << t1->GetEntries() << endl;
  cout << " t1->GetEntries(\"truthjet4_pt>5\") = " << t1->GetEntries("truthjet4_pt>5") << endl;
  cout << " fraction = " << num/den <<endl;
}
