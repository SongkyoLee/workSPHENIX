#include "TMinuit.h"

void check_correlation_SF(string ihcalType="SS310"){
  
  ///////////////////////////////////////
  //// Get Hist for different pT range
  const int nFile = 30;
  cout << "nFile = " << nFile << endl;
  const int nMeth = 5; 
  cout << "nMeth = " << nMeth << endl;

  ///////////////////////////////////////
  ///////////////////////////////////////
  //// Draw JES
  ///////////////////////////////////////
  ///////////////////////////////////////
  double dummy;
  double cemc[nMeth][nFile];
  double ihcal[nMeth][nFile];
  double ohcal[nMeth][nFile];
  
  //////////////////// read-in cemc files
  string szcemc;
  int icemc=0;
  ifstream infile_cemc[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_cemc[im].open(Form("txt_scale/cemcScale_method0%d_%s.out",im+1,ihcalType.c_str()));
    icemc=0;
    if (infile_cemc[im].is_open()){
      while( getline (infile_cemc[im],szcemc) ) { cemc[im][icemc] = atof(szcemc.c_str()); icemc++; }
    }
  }
  string szihcal;
  int iihcal=0;
  ifstream infile_ihcal[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_ihcal[im].open(Form("txt_scale/ihcalScale_method0%d_%s.out",im+1,ihcalType.c_str()));
    iihcal=0;
    if (infile_ihcal[im].is_open()){
      while( getline (infile_ihcal[im],szihcal) ) { ihcal[im][iihcal] = atof(szihcal.c_str()); iihcal++; }
    }
  }
  string szohcal;
  int iohcal=0;
  ifstream infile_ohcal[nMeth];
  for (int im=0; im<nMeth; im++){
    infile_ohcal[im].open(Form("txt_scale/ohcalScale_method0%d_%s.out",im+1,ihcalType.c_str()));
    iohcal=0;
    if (infile_ohcal[im].is_open()){
      while( getline (infile_ohcal[im],szohcal) ) { ohcal[im][iohcal] = atof(szohcal.c_str()); iohcal++; }
    }
  }
  
  //////////////////////////////////////////////////////////////////////
  //// Histogram

  TCanvas* c_1d = new TCanvas("c_1d","",1500,500);
  c_1d->Divide(3,1);
  int finalMeth = 2;
  TH2D* h2d_cemc_ihcal = new TH2D("h2d_cemc_ihcal",";EMCAL SCcale;IHCAL Scale",50,1,2,50,2.0,3.5);
  TH2D* h2d_cemc_ohcal = new TH2D("h2d_cemc_ohcal",";EMCAL SCcale;OHCAL Scale",50,1,2,50,1,2);
  TH2D* h2d_ihcal_ohcal = new TH2D("h2d_ihcal_ohcal",";IHCAL SCcale;OHCAL Scale",50,2,3.5,50,1,2);
  
  for (int ie=0; ie<nFile; ie++){
    h2d_cemc_ihcal->Fill(cemc[finalMeth][ie],ihcal[finalMeth][ie]); 
    h2d_cemc_ohcal->Fill(cemc[finalMeth][ie],ohcal[finalMeth][ie]); 
    h2d_ihcal_ohcal->Fill(ihcal[finalMeth][ie],ohcal[finalMeth][ie]); 
  }
  c_1d->cd(1);
  h2d_cemc_ihcal->Draw("colz");
  c_1d->cd(2);
  h2d_cemc_ohcal->Draw("colz");
  c_1d->cd(3);
  h2d_ihcal_ohcal->Draw("colz");
  return;

}

