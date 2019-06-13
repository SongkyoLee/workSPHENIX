void get_wTot(){
  
  ifstream infileJES;  
  infileJES.open("JES_final_qcd.out");
  //infileJES.open(Form("JES_%s.out",inputType.c_str()));
  int ijes=0;
  double tmpjes[531];
  string szjes;
  if (infileJES.is_open()){
    while( getline (infileJES,szjes) ) {
      tmpjes[ijes] = 1./atof(szjes.c_str());
      cout << "tmpjes["<<ijes<<"] = " << tmpjes[ijes] << endl;
      ijes++;
    }
  }
 
  ofstream outfile ("wTot.out"); 
  if (outfile.is_open())
  {
    for (int ie=0; ie<ijes; ie++){
      outfile << tmpjes[ie] << "\n";
    }
  } 

  return; 
}
