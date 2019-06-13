void get_total_sf(string ihcalType="Al", string jetE = "20GeV", string note="04fmR04", string szMeth="truthjet_hi",int initfile=0,int endfile =200){
  
  const int nEBin = 4;
	string szEBin[nEBin] = {"20GeV", "30GeV", "40GeV", "50GeV"}; 
	//double xPnt[nEBin] = {25, 35, 45, 55};

	double jes[nEBin];
	double total[nEBin];
	double cemc[nEBin];
	double ihcal[nEBin];
	double ohcal[nEBin];

  //////////////////// read-in jet files
  string szjes;
  int ijes = 0;
  ifstream infile_jes;
  infile_jes.open(Form("txt_jet/jes_%s_%s_%s.out",szMeth.c_str(),ihcalType.c_str(),note.c_str()));
  ijes=0;
  if (infile_jes.is_open()){
    while( getline (infile_jes,szjes) ) { jes[ijes] = atof(szjes.c_str()); ijes++; }
  }   
  //////////////////// read-in cemc files
  string szcemc;
  int icemc = 0;
  ifstream infile_cemc;
  infile_cemc.open(Form("txt_scale/cemcScale_%s_%s_%s.out",szMeth.c_str(),ihcalType.c_str(),note.c_str()));
  icemc=0;
  if (infile_cemc.is_open()){
    while( getline (infile_cemc,szcemc) ) { cemc[icemc] = atof(szcemc.c_str()); icemc++; }
  }   
  //////////////////// read-in ihcal files
  string szihcal;
  int iihcal = 0;
  ifstream infile_ihcal;
  infile_ihcal.open(Form("txt_scale/ihcalScale_%s_%s_%s.out",szMeth.c_str(),ihcalType.c_str(),note.c_str()));
  iihcal=0;
  if (infile_ihcal.is_open()){
    while( getline (infile_ihcal,szihcal) ) { ihcal[iihcal] = atof(szihcal.c_str()); iihcal++; }
  }   
  //////////////////// read-in ohcal files
  string szohcal;
  int iohcal = 0;
  ifstream infile_ohcal;
  infile_ohcal.open(Form("txt_scale/ohcalScale_%s_%s_%s.out",szMeth.c_str(),ihcalType.c_str(),note.c_str()));
  iohcal=0;
  if (infile_ohcal.is_open()){
    while( getline (infile_ohcal,szohcal) ) { ohcal[iohcal] = atof(szohcal.c_str()); iohcal++; }
  }   

	for (int ie=0; ie<nEBin; ie++){
		total[ie] = 1./jes[ie];
		cout << szEBin[ie].c_str() <<endl;
		cout << " -> jes["<<ie<<"] = " << jes[ie] << endl;
		cout << "total["<<ie<<"] = " << total[ie] << endl;
		cout << "cemc["<<ie<<"] = " << cemc[ie] << endl;
		cout << "ihcal["<<ie<<"] = " << ihcal[ie] << endl;
		cout << "ohcal["<<ie<<"] = " << ohcal[ie] << endl;
    
    string fname = Form("logs_wTot/log_%s_%d_%d_%s_%s_%s",szMeth.c_str(),initfile,endfile,szEBin[ie].c_str(),ihcalType.c_str(),note.c_str());
    ofstream outfile (fname.c_str());
    if (outfile.is_open()){
	    if (szMeth.compare("before")==0) {
        outfile << "cemcScale = 1" << "\n";
	      outfile << "ihcalScale = 1" << "\n";
	      outfile << "ohcalScale = 1" << "\n";
	      outfile << "totalScale = 1" << "\n";
      } else {
        outfile << "cemcScale = "<<cemc[ie] << "\n";
	      outfile << "ihcalScale = "<<ihcal[ie] << "\n";
	      outfile << "ohcalScale = "<<ohcal[ie] << "\n";
	      outfile << "totalScale = "<<total[ie] << "\n";
      }
    }
  }

/*   
  string fname = Form("outvsE/JESvsE_%s_%s.root",ihcalType.c_str(),note.c_str());
  TFile* fin = new TFile( fname.c_str(), "READ");
  //fin->Print(); fin->ls(); 
  const int nMeth = 2;
  //string szMeth;
  //szMeth[0]= "truthjet";
  //szMeth[1]= "truthjet_2par";
  const int nEBin = 4;
  TGraphAsymmErrors* gjesAfter;
  double px[nEBin];
  double py[nEBin];
  for (int im=0; im<nMeth; im++){
    gjesAfter[im] = (TGraphAsymmErrors*)fin->Get(Form("gjesAfter_%d",im));
    if (nEBin != gjesAfter[im]->GetN()) { cout << "KYO:: nEBin does not match to GetN()!"<<endl; return; }
    
    for (int ie=0;ie<nEBin;ie++){
      gjesAfter[im]->GetPoint(ie,px[im][ie],py[im][ie]);  
      cout << "px["<<im<<"]["<<ie<<"] = " << px[im][ie] << endl;
      cout << "py["<<im<<"]["<<ie<<"] = " << py[im][ie] << endl;
    } 
  }
*/
   
}
