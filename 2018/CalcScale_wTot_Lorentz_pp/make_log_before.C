void make_log_before(string sampleType = "QCDJet",
                              	string ihcalType = "Al",
                              	string jetE = "20GeV",
                              	string note = "R04",
                              	int initfile=0,
                              	int endfile =1000,
                              	bool doetopcut=true,
                              	bool dodphicut=true) 
{
  // Outputfile 
  ofstream outfile;
  string filename = Form("logs/log_before_%d_%d_%s_%s_%s",initfile,endfile,jetE.c_str(),ihcalType.c_str(),note.c_str()); 
  cout << "filename = " << filename << endl;
  //outfile.open(filename,ios::out | ios::app);
  outfile.open(filename,ios::out);
  outfile<< "cemcScale = 1"<<"\n"; 
  outfile<< "ihcalScale = 1"<<"\n"; 
  outfile<< "ohcalScale = 1"<<"\n"; 
  //outfile<< "cemcEmScale = 1"<<"\n"; 
  outfile.close();
}
