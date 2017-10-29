
#include <iostream>
#include <sstream>
#include <string> 
#include "TROOT.h"
#include "TChain.h"
#include "TMath.h"
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TNtuple.h"
#include "TRandom3.h"
#include "TVector.h"
#include "TRotation.h"
#include "TF1.h"
#include "TFile.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TSpectrum.h"
#include "TVector3.h"

#include <string.h>
#include "TChain.h"
#include "TTree.h"
#include "TKey.h"
#include "Riostream.h"

#include "TError.h"

#include <vector>

void rootAna_scale(string inType = "SS310_PiPl_25GeV",
                  int initfile=0,
                  int endfile =1,
                  bool do_cemc_scale=false, 
                  bool do_ihcal_scale=false, 
                  bool do_ohcal_scale=false, 
                  float cemc_mip_cut=0.5, 
                  float ihcal_mip_cut=0.3, 
                  float ohcal_mip_cut = 5.0)
{
  bool do_E3x3 = true;
  bool do_E5x5 = true;
  bool do_clE = true;
 
  float true_p;
  TString ihcalType;
  TString ptlType;
  
  float cemc_sf=1.;
  float ihcal_sf=1.;
  float ohcal_sf=1.;
 
  int nbin = 400; 
  if (inType.compare("SS310_PiPl_25GeV")==0){ 
    true_p  = 25;
    ihcalType = "SS310";
    ptlType = "#pi^{+}";
    if (do_cemc_scale) cemc_sf=1.;
    if (do_ihcal_scale) ihcal_sf=1.;
    if (do_ohcal_scale) ohcal_sf=1./0.662;
  }
  else if (inType.compare("SS310_PiMi_25GeV")==0){ 
    true_p  = 25;
    ihcalType = "SS310";
    ptlType = "#pi^{-}";
    if (do_cemc_scale) cemc_sf=1.;
    if (do_ihcal_scale) ihcal_sf=1.;
    if (do_ohcal_scale) ohcal_sf=1./0.662;
  }
  else if (inType.compare("Al_PiPl_25GeV")==0){ 
    true_p  = 25;
    ihcalType = "Al";
    ptlType = "#pi^{+}";
    if (do_cemc_scale) cemc_sf=1.;
    if (do_ihcal_scale) ihcal_sf=1.;
    if (do_ohcal_scale) ohcal_sf=1./0.662;
  }
  else if (inType.compare("Al_PiMi_25GeV")==0){ 
    true_p  = 25;
    ihcalType = "Al";
    ptlType = "#pi^{-}";
    if (do_cemc_scale) cemc_sf=1.;
    if (do_ihcal_scale) ihcal_sf=1.;
    if (do_ohcal_scale) ohcal_sf=1./0.662;
  }
  else if (inType.compare("SS310_PiPl_10GeV")==0){ 
    true_p  = 10;
    ihcalType = "SS310";
    ptlType = "#pi^{+}";
    if (do_cemc_scale) cemc_sf=1.;
    if (do_ihcal_scale) ihcal_sf=1.;
    if (do_ohcal_scale) ohcal_sf=1./0.604;
  }
  else if (inType.compare("SS310_PiMi_10GeV")==0){ 
    true_p  = 10;
    ihcalType = "SS310";
    ptlType = "#pi^{-}";
    if (do_cemc_scale) cemc_sf=1.;
    if (do_ihcal_scale) ihcal_sf=1.;
    if (do_ohcal_scale) ohcal_sf=1./0.604;
  }
  else if (inType.compare("Al_PiPl_10GeV")==0){ 
    true_p  = 10;
    ihcalType = "Al";
    ptlType = "#pi^{+}";
    if (do_cemc_scale) cemc_sf=1.;
    if (do_ihcal_scale) ihcal_sf=1.;
    if (do_ohcal_scale) ohcal_sf=1./0.601;
  }
  else if (inType.compare("Al_PiMi_10GeV")==0){ 
    true_p  = 10;
    ihcalType = "Al";
    ptlType = "#pi^{-}";
    if (do_cemc_scale) cemc_sf=1.;
    if (do_ihcal_scale) ihcal_sf=1.;
    if (do_ohcal_scale) ohcal_sf=1./0.601;
  }
  else {
    cout<<"WARNING! select correct inType" <<endl; return;
  } 
  
  //// Suppress error messages from bad files
  gErrorIgnoreLevel = kFatal; 

  TChain* track_trees = new TChain("track_tree");
  
  int bad_files = 0; 

  for(int j=initfile; j<=endfile; j++){
    
    TString fname = Form("./out_%s/ParticleGun_G4sPHENIX_%s_%d.root",inType.c_str(),inType.c_str(),j);
    if (j%100==0) cout << fname << endl; 

    //// skip bad files
    TFile fin(fname); 
    if(fin.GetSize()<10000){fin.Close(); bad_files++; continue;}
    if(fin.IsZombie()) {fin.Close(); bad_files++; continue;}
    fin.Close(); 
 
    track_trees->AddFile(fname);
  }
  cout << "Number of bad files = " << bad_files << endl; 
  
  ////////////////////////////////////////////////////////
  //TH1D* h1D_g4particle_pt = new TH1D("g4particle_pt",";g4particle_pt;",50,0,50);
  //TH1D* h1D_g4particle_eta = new TH1D("g4particle_eta",";g4particle_eta;",100,-1.5,1.5);
  ////////////////////////////////////////////////////////
  TH1D* h1D_cemc_E3x3 = new TH1D("cemc_E3x3",";cemc_E3x3;",400,0,true_p*1.4);
  TH1D* h1D_ihcal_E3x3 = new TH1D("ihcal_E3x3",";ihcal_E3x3;",400,0,true_p*1.4);
  TH1D* h1D_ohcal_E3x3 = new TH1D("ohcal_E3x3",";ohcal_E3x3;",400,0,true_p*1.4);
  TH1D* h1D_allThree_E3x3 = new TH1D("allThree_E3x3",";allThree_E3x3;",400,0,true_p*1.4);
  TH1D* h1D_ohcal_incident_E3x3 = new TH1D("ohcal_incident_E3x3",";Truth E - cemc_E - ihcal_E;",400,0,true_p*1.4);
  TH1D* h1D_ohcal_frac_E3x3 = new TH1D("ohcal_frac_E3x3",";ohcal_E / (Truth E - cemc_E - ihcal_E) ;",400,0,2.);
  TH1D* h1D_outerTwo_incident_E3x3 = new TH1D("outerTwo_incident_E3x3",";Truth E - cemc_E;",400,0,true_p*1.4);
  TH1D* h1D_outerTwo_frac_E3x3 = new TH1D("outerTwo_frac_E3x3",";(ihcal_E + ohcal_E) / (Truth E - cemc_E);",400,0,2.);
  TH1D* h1D_allThree_frac_E3x3 = new TH1D("allThree_frac_E3x3",";(cemc_E + ihcal_E + ohcal_E) / Truth E;",400,0,2.);
  ////
  TH1D* h1D_cemc_E5x5 = new TH1D("cemc_E5x5",";cemc_E5x5;",400,0,true_p*1.4);
  TH1D* h1D_ihcal_E5x5 = new TH1D("ihcal_E5x5",";ihcal_E5x5;",400,0,true_p*1.4);
  TH1D* h1D_ohcal_E5x5 = new TH1D("ohcal_E5x5",";ohcal_E5x5;",400,0,true_p*1.4);
  TH1D* h1D_allThree_E5x5 = new TH1D("allThree_E5x5",";allThree_E5x5;",400,0,true_p*1.4);
  TH1D* h1D_ohcal_incident_E5x5 = new TH1D("ohcal_incident_E5x5",";Truth E - cemc_E - ihcal_E;",400,0,true_p*1.4);
  TH1D* h1D_ohcal_frac_E5x5 = new TH1D("ohcal_frac_E5x5",";ohcal_E / (Truth E - cemc_E - ihcal_E) ;",400,0,2.);
  TH1D* h1D_outerTwo_incident_E5x5 = new TH1D("outerTwo_incident_E5x5",";Truth E - cemc_E;",400,0,true_p*1.4);
  TH1D* h1D_outerTwo_frac_E5x5 = new TH1D("outerTwo_frac_E5x5",";(ihcal_E + ohcal_E) / (Truth E - cemc_E);",400,0,2.);
  TH1D* h1D_allThree_frac_E5x5 = new TH1D("allThree_frac_E5x5",";(cemc_E + ihcal_E + ohcal_E) / Truth E;",400,0,2.);
  ////
  TH1D* h1D_cemc_clE = new TH1D("cemc_clE",";cemc_clE;",400,0,true_p*1.4);
  TH1D* h1D_ihcal_clE = new TH1D("ihcal_clE",";ihcal_clE;",400,0,true_p*1.4);
  TH1D* h1D_ohcal_clE = new TH1D("ohcal_clE",";ohcal_clE;",400,0,true_p*1.4);
  TH1D* h1D_allThree_clE = new TH1D("allThree_clE",";allThree_clE;",400,0,true_p*1.4);
  TH1D* h1D_ohcal_incident_clE = new TH1D("ohcal_incident_clE",";Truth E - cemc_E - ihcal_E;",400,0,true_p*1.4);
  TH1D* h1D_ohcal_frac_clE = new TH1D("ohcal_frac_clE",";ohcal_E / (Truth E - cemc_E - ihcal_E) ;",400,0,2.);
  TH1D* h1D_outerTwo_incident_clE = new TH1D("outerTwo_incident_clE",";Truth E - cemc_E;",400,0,true_p*1.4);
  TH1D* h1D_outerTwo_frac_clE = new TH1D("outerTwo_frac_clE",";(ihcal_E + ohcal_E) / (Truth E - cemc_E);",400,0,2.);
  TH1D* h1D_allThree_frac_clE = new TH1D("allThree_frac_clE",";(cemc_E + ihcal_E + ohcal_E) / Truth E;",400,0,2.);
  ////////
  //// cut
  ////////
  TH1D* h1D_cemc_E3x3_cut = new TH1D("cemc_E3x3_cut",";cemc_E3x3;",400,0,true_p*1.4);
  TH1D* h1D_ihcal_E3x3_cut = new TH1D("ihcal_E3x3_cut",";ihcal_E3x3;",400,0,true_p*1.4);
  TH1D* h1D_ohcal_E3x3_cut = new TH1D("ohcal_E3x3_cut",";ohcal_E3x3;",400,0,true_p*1.4);
  TH1D* h1D_allThree_E3x3_cut = new TH1D("allThree_E3x3_cut",";allThree_E3x3_cut;",400,0,true_p*1.4);
  TH1D* h1D_ohcal_incident_E3x3_cut = new TH1D("ohcal_incident_E3x3_cut",";Truth E - cemc_E - ihcal_E;",400,0,true_p*1.4);
  TH1D* h1D_ohcal_frac_E3x3_cut = new TH1D("ohcal_frac_E3x3_cut",";ohcal_E / (Truth E - cemc_E - ihcal_E) ;",400,0,2.);
  TH1D* h1D_outerTwo_incident_E3x3_cut = new TH1D("outerTwo_incident_E3x3_cut",";Truth E - cemc_E;",400,0,true_p*1.4);
  TH1D* h1D_outerTwo_frac_E3x3_cut = new TH1D("outerTwo_frac_E3x3_cut",";(ihcal_E + ohcal_E) / (Truth E - cemc_E);",400,0,2.);
  TH1D* h1D_allThree_frac_E3x3_cut = new TH1D("allThree_frac_E3x3_cut",";(cemc_E + ihcal_E + ohcal_E) / Truth E;",400,0,2.);
  ////
  TH1D* h1D_cemc_E5x5_cut = new TH1D("cemc_E5x5_cut",";cemc_E5x5;",400,0,true_p*1.4);
  TH1D* h1D_ihcal_E5x5_cut = new TH1D("ihcal_E5x5_cut",";ihcal_E5x5;",400,0,true_p*1.4);
  TH1D* h1D_ohcal_E5x5_cut = new TH1D("ohcal_E5x5_cut",";ohcal_E5x5;",400,0,true_p*1.4);
  TH1D* h1D_allThree_E5x5_cut = new TH1D("allThree_E5x5_cut",";allThree_E5x5_cut;",400,0,true_p*1.4);
  TH1D* h1D_ohcal_incident_E5x5_cut = new TH1D("ohcal_incident_E5x5_cut",";Truth E - cemc_E - ihcal_E;",400,0,true_p*1.4);
  TH1D* h1D_ohcal_frac_E5x5_cut = new TH1D("ohcal_frac_E5x5_cut",";ohcal_E / (Truth E - cemc_E - ihcal_E) ;",400,0,2.);
  TH1D* h1D_outerTwo_incident_E5x5_cut = new TH1D("outerTwo_incident_E5x5_cut",";Truth E - cemc_E;",400,0,true_p*1.4);
  TH1D* h1D_outerTwo_frac_E5x5_cut = new TH1D("outerTwo_frac_E5x5_cut",";(ihcal_E + ohcal_E) / (Truth E - cemc_E);",400,0,2.);
  TH1D* h1D_allThree_frac_E5x5_cut = new TH1D("allThree_frac_E5x5_cut",";(cemc_E + ihcal_E + ohcal_E) / Truth E;",400,0,2.);
  ////
  TH1D* h1D_cemc_clE_cut = new TH1D("cemc_clE_cut",";cemc_clE;",400,0,true_p*1.4);
  TH1D* h1D_ihcal_clE_cut = new TH1D("ihcal_clE_cut",";ihcal_clE;",400,0,true_p*1.4);
  TH1D* h1D_ohcal_clE_cut = new TH1D("ohcal_clE_cut",";ohcal_clE;",400,0,true_p*1.4);
  TH1D* h1D_allThree_clE_cut = new TH1D("allThree_clE_cut",";allThree_clE_cut;",400,0,true_p*1.4);
  TH1D* h1D_ohcal_incident_clE_cut = new TH1D("ohcal_incident_clE_cut",";Truth E - cemc_E - ihcal_E;",400,0,true_p*1.4);
  TH1D* h1D_ohcal_frac_clE_cut = new TH1D("ohcal_frac_clE_cut",";ohcal_E / (Truth E - cemc_E - ihcal_E) ;",400,0,2.);
  TH1D* h1D_outerTwo_incident_clE_cut = new TH1D("outerTwo_incident_clE_cut",";Truth E - cemc_E;",400,0,true_p*1.4);
  TH1D* h1D_outerTwo_frac_clE_cut = new TH1D("outerTwo_frac_clE_cut",";(ihcal_E + ohcal_E) / (Truth E - cemc_E);",400,0,2.);
  TH1D* h1D_allThree_frac_clE_cut = new TH1D("allThree_frac_clE_cut",";(cemc_E + ihcal_E + ohcal_E) / Truth E;",400,0,2.);

  ////////////////////////////////////////////////////////
  //////// TH2D
  ////////////////////////////////////////////////////////

  TH2D* h2D_cemc_vs_ihcal_E3x3 = new TH2D("h2D_cemc_vs_ihcal_E3x3",";cemc E3x3;ihcal E3x3",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_cemc_vs_ohcal_E3x3 = new TH2D("h2D_cemc_vs_ohcal_E3x3",";cemc E3x3;ohcal E3x3",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_ihcal_vs_ohcal_E3x3 = new TH2D("h2D_ihcal_vs_ohcal_E3x3",";ihcal E3x3;ohcal E3x3",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_innerTwo_vs_ohcal_E3x3 = new TH2D("h2D_innerTwo_vs_ohcal_E3x3",";cemc E3x3 + ihcal E3x3;ohcal E3x3",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_cemc_vs_outerTwo_E3x3 = new TH2D("h2D_cemc_vs_outerTwo_E3x3",";cemc E3x3 + ihcal E3x3;ohcal E3x3",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_outerTwo_frac_vs_diffIO_frac_E3x3 = new TH2D("h2D_outerTwo_frac_vs_diffIO_frac_E3x3","(ihcal_E + ohcal_E) / (Truth E - cemc_E);(ihcal_E - ohcal_E) / (ihcal_E + ohcal_E)",400,-1.,1.,400,0,2.);
  TH2D* h2D_allThree_frac_vs_diffHE_frac_E3x3 = new TH2D("h2D_allThree_frac_vs_diffHE_frac_E3x3","(cemc_E + ihcal_E + ohcal_E) / Truth E;((ihcal_E + ohcal_E) - cemc_E) / ((ihcal_E + ohcal_E) + cemc_E)",400,-1.,1.,400,0,2.);
  TH2D* h2D_outerTwo_frac_vs_diffIO_frac_E3x3_nomip = new TH2D("h2D_outerTwo_frac_vs_diffIO_frac_E3x3_nomip","(ihcal_E + ohcal_E) / (Truth E - cemc_E);(ihcal_E - ohcal_E) / (ihcal_E + ohcal_E)",400,-1.,1.,400,0,2.);
  TH2D* h2D_allThree_frac_vs_diffHE_frac_E3x3_nomip = new TH2D("h2D_allThree_frac_vs_diffHE_frac_E3x3_nomip","(cemc_E + ihcal_E + ohcal_E) / Truth E;((ihcal_E + ohcal_E) - cemc_E) / ((ihcal_E + ohcal_E) + cemc_E)",400,-1.,1.,400,0,2.);
  ////
  TH2D* h2D_cemc_vs_ihcal_E5x5 = new TH2D("h2D_cemc_vs_ihcal_E5x5",";cemc E5x5;ihcal E5x5",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_cemc_vs_ohcal_E5x5 = new TH2D("h2D_cemc_vs_ohcal_E5x5",";cemc E5x5;ohcal E5x5",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_ihcal_vs_ohcal_E5x5 = new TH2D("h2D_ihcal_vs_ohcal_E5x5",";ihcal E5x5;ohcal E5x5",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_innerTwo_vs_ohcal_E5x5 = new TH2D("h2D_innerTwo_vs_ohcal_E5x5",";cemc E5x5 + ihcal E5x5;ohcal E5x5",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_cemc_vs_outerTwo_E5x5 = new TH2D("h2D_cemc_vs_outerTwo_E5x5",";cemc E5x5 + ihcal E5x5;ohcal E5x5",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_outerTwo_frac_vs_diffIO_frac_E5x5 = new TH2D("h2D_outerTwo_frac_vs_diffIO_frac_E5x5","(ihcal_E + ohcal_E) / (Truth E - cemc_E);(ihcal_E - ohcal_E) / (ihcal_E + ohcal_E)",400,-1.,1.,400,0,2.);
  TH2D* h2D_allThree_frac_vs_diffHE_frac_E5x5 = new TH2D("h2D_allThree_frac_vs_diffHE_frac_E5x5","(cemc_E + ihcal_E + ohcal_E) / Truth E;((ihcal_E + ohcal_E) - cemc_E) / ((ihcal_E + ohcal_E) + cemc_E)",400,-1.,1.,400,0,2.);
  TH2D* h2D_outerTwo_frac_vs_diffIO_frac_E5x5_nomip = new TH2D("h2D_outerTwo_frac_vs_diffIO_frac_E5x5_nomip","(ihcal_E + ohcal_E) / (Truth E - cemc_E);(ihcal_E - ohcal_E) / (ihcal_E + ohcal_E)",400,-1.,1.,400,0,2.);
  TH2D* h2D_allThree_frac_vs_diffHE_frac_E5x5_nomip = new TH2D("h2D_allThree_frac_vs_diffHE_frac_E5x5_nomip","(cemc_E + ihcal_E + ohcal_E) / Truth E;((ihcal_E + ohcal_E) - cemc_E) / ((ihcal_E + ohcal_E) + cemc_E)",400,-1.,1.,400,0,2.);
  ////
  TH2D* h2D_cemc_vs_ihcal_clE = new TH2D("h2D_cemc_vs_ihcal_clE",";cemc clE;ihcal clE",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_cemc_vs_ohcal_clE = new TH2D("h2D_cemc_vs_ohcal_clE",";cemc clE;ohcal clE",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_ihcal_vs_ohcal_clE = new TH2D("h2D_ihcal_vs_ohcal_clE",";ihcal clE;ohcal clE",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_innerTwo_vs_ohcal_clE = new TH2D("h2D_innerTwo_vs_ohcal_clE",";cemc clE + ihcal clE;ohcal clE",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_cemc_vs_outerTwo_clE = new TH2D("h2D_cemc_vs_outerTwo_clE",";cemc clE + ihcal clE;ohcal clE",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_outerTwo_frac_vs_diffIO_frac_clE = new TH2D("h2D_outerTwo_frac_vs_diffIO_frac_clE","(ihcal_E + ohcal_E) / (Truth E - cemc_E);(ihcal_E - ohcal_E) / (ihcal_E + ohcal_E)",400,-1.,1.,400,0,2.);
  TH2D* h2D_allThree_frac_vs_diffHE_frac_clE = new TH2D("h2D_allThree_frac_vs_diffHE_frac_clE","(cemc_E + ihcal_E + ohcal_E) / Truth E;((ihcal_E + ohcal_E) - cemc_E) / ((ihcal_E + ohcal_E) + cemc_E)",400,-1.,1.,400,0,2.);
  TH2D* h2D_outerTwo_frac_vs_diffIO_frac_clE_nomip = new TH2D("h2D_outerTwo_frac_vs_diffIO_frac_clE_nomip","(ihcal_E + ohcal_E) / (Truth E - cemc_E);(ihcal_E - ohcal_E) / (ihcal_E + ohcal_E)",400,-1.,1.,400,0,2.);
  TH2D* h2D_allThree_frac_vs_diffHE_frac_clE_nomip = new TH2D("h2D_allThree_frac_vs_diffHE_frac_clE_nomip","(cemc_E + ihcal_E + ohcal_E) / Truth E;((ihcal_E + ohcal_E) - cemc_E) / ((ihcal_E + ohcal_E) + cemc_E)",400,-1.,1.,400,0,2.);
  ////////
  //// cut
  ////////
  TH2D* h2D_cemc_vs_ihcal_E3x3_cut = new TH2D("h2D_cemc_vs_ihcal_E3x3_cut",";cemc E3x3;ihcal E3x3",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_cemc_vs_ohcal_E3x3_cut = new TH2D("h2D_cemc_vs_ohcal_E3x3_cut",";cemc E3x3;ohcal E3x3",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_ihcal_vs_ohcal_E3x3_cut = new TH2D("h2D_ihcal_vs_ohcal_E3x3_cut",";ihcal E3x3;ohcal E3x3",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_innerTwo_vs_ohcal_E3x3_cut = new TH2D("h2D_innerTwo_vs_ohcal_E3x3_cut",";cemc E3x3 + ihcal E3x3;ohcal E3x3",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_cemc_vs_outerTwo_E3x3_cut = new TH2D("h2D_cemc_vs_outerTwo_E3x3_cut",";cemc E3x3 + ihcal E3x3;ohcal E3x3",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_outerTwo_frac_vs_diffIO_frac_E3x3_cut = new TH2D("h2D_outerTwo_frac_vs_diffIO_frac_E3x3_cut","(ihcal_E + ohcal_E) / (Truth E - cemc_E);(ihcal_E - ohcal_E) / (ihcal_E + ohcal_E)",400,-1.,1.,400,0,2.);
  TH2D* h2D_allThree_frac_vs_diffHE_frac_E3x3_cut = new TH2D("h2D_allThree_frac_vs_diffHE_frac_E3x3_cut","(cemc_E + ihcal_E + ohcal_E) / Truth E;((ihcal_E + ohcal_E) - cemc_E) / ((ihcal_E + ohcal_E) + cemc_E)",400,-1.,1.,400,0,2.);
  TH2D* h2D_outerTwo_frac_vs_diffIO_frac_E3x3_cut_nomip = new TH2D("h2D_outerTwo_frac_vs_diffIO_frac_E3x3_cut_nomip","(ihcal_E + ohcal_E) / (Truth E - cemc_E);(ihcal_E - ohcal_E) / (ihcal_E + ohcal_E)",400,-1.,1.,400,0,2.);
  TH2D* h2D_allThree_frac_vs_diffHE_frac_E3x3_cut_nomip = new TH2D("h2D_allThree_frac_vs_diffHE_frac_E3x3_cut_nomip","(cemc_E + ihcal_E + ohcal_E) / Truth E;((ihcal_E + ohcal_E) - cemc_E) / ((ihcal_E + ohcal_E) + cemc_E)",400,-1.,1.,400,0,2.);
  ////
  TH2D* h2D_cemc_vs_ihcal_E5x5_cut = new TH2D("h2D_cemc_vs_ihcal_E5x5_cut",";cemc E5x5;ihcal E5x5",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_cemc_vs_ohcal_E5x5_cut = new TH2D("h2D_cemc_vs_ohcal_E5x5_cut",";cemc E5x5;ohcal E5x5",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_ihcal_vs_ohcal_E5x5_cut = new TH2D("h2D_ihcal_vs_ohcal_E5x5_cut",";ihcal E5x5;ohcal E5x5",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_innerTwo_vs_ohcal_E5x5_cut = new TH2D("h2D_innerTwo_vs_ohcal_E5x5_cut",";cemc E5x5 + ihcal E5x5;ohcal E5x5",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_cemc_vs_outerTwo_E5x5_cut = new TH2D("h2D_cemc_vs_outerTwo_E5x5_cut",";cemc E5x5 + ihcal E5x5;ohcal E5x5",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_outerTwo_frac_vs_diffIO_frac_E5x5_cut = new TH2D("h2D_outerTwo_frac_vs_diffIO_frac_E5x5_cut","(ihcal_E + ohcal_E) / (Truth E - cemc_E);(ihcal_E - ohcal_E) / (ihcal_E + ohcal_E)",400,-1.,1.,400,0,2.);
  TH2D* h2D_allThree_frac_vs_diffHE_frac_E5x5_cut = new TH2D("h2D_allThree_frac_vs_diffHE_frac_E5x5_cut","(cemc_E + ihcal_E + ohcal_E) / Truth E;((ihcal_E + ohcal_E) - cemc_E) / ((ihcal_E + ohcal_E) + cemc_E)",400,-1.,1.,400,0,2.);
  TH2D* h2D_outerTwo_frac_vs_diffIO_frac_E5x5_cut_nomip = new TH2D("h2D_outerTwo_frac_vs_diffIO_frac_E5x5_cut_nomip","(ihcal_E + ohcal_E) / (Truth E - cemc_E);(ihcal_E - ohcal_E) / (ihcal_E + ohcal_E)",400,-1.,1.,400,0,2.);
  TH2D* h2D_allThree_frac_vs_diffHE_frac_E5x5_cut_nomip = new TH2D("h2D_allThree_frac_vs_diffHE_frac_E5x5_cut_nomip","(cemc_E + ihcal_E + ohcal_E) / Truth E;((ihcal_E + ohcal_E) - cemc_E) / ((ihcal_E + ohcal_E) + cemc_E)",400,-1.,1.,400,0,2.);
  ////
  TH2D* h2D_cemc_vs_ihcal_clE_cut = new TH2D("h2D_cemc_vs_ihcal_clE_cut",";cemc clE;ihcal clE",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_cemc_vs_ohcal_clE_cut = new TH2D("h2D_cemc_vs_ohcal_clE_cut",";cemc clE;ohcal clE",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_ihcal_vs_ohcal_clE_cut = new TH2D("h2D_ihcal_vs_ohcal_clE_cut",";ihcal clE;ohcal clE",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_innerTwo_vs_ohcal_clE_cut = new TH2D("h2D_innerTwo_vs_ohcal_clE_cut",";cemc clE + ihcal clE;ohcal clE",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_cemc_vs_outerTwo_clE_cut = new TH2D("h2D_cemc_vs_outerTwo_clE_cut",";cemc clE + ihcal clE;ohcal clE",400, 0., true_p*1.4., 400, 0, true_p*1.4.);
  TH2D* h2D_outerTwo_frac_vs_diffIO_frac_clE_cut = new TH2D("h2D_outerTwo_frac_vs_diffIO_frac_clE_cut","(ihcal_E + ohcal_E) / (Truth E - cemc_E);(ihcal_E - ohcal_E) / (ihcal_E + ohcal_E)",400,-1.,1.,400,0,2.);
  TH2D* h2D_allThree_frac_vs_diffHE_frac_clE_cut = new TH2D("h2D_allThree_frac_vs_diffHE_frac_clE_cut","(cemc_E + ihcal_E + ohcal_E) / Truth E;((ihcal_E + ohcal_E) - cemc_E) / ((ihcal_E + ohcal_E) + cemc_E)",400,-1.,1.,400,0,2.);
  TH2D* h2D_outerTwo_frac_vs_diffIO_frac_clE_cut_nomip = new TH2D("h2D_outerTwo_frac_vs_diffIO_frac_clE_cut_nomip","(ihcal_E + ohcal_E) / (Truth E - cemc_E);(ihcal_E - ohcal_E) / (ihcal_E + ohcal_E)",400,-1.,1.,400,0,2.);
  TH2D* h2D_allThree_frac_vs_diffHE_frac_clE_cut_nomip = new TH2D("h2D_allThree_frac_vs_diffHE_frac_clE_cut_nomip","(cemc_E + ihcal_E + ohcal_E) / Truth E;((ihcal_E + ohcal_E) - cemc_E) / ((ihcal_E + ohcal_E) + cemc_E)",400,-1.,1.,400,0,2.);
  ////////////////////////////////////////////////////////
  
  int track_n;  
  track_trees->SetBranchAddress("track_n",&track_n); 
  float cemc_E3x3[999], ihcal_E3x3[999], ohcal_E3x3[999];
  track_trees->SetBranchAddress("cemc_E3x3",&cemc_E3x3); 
  track_trees->SetBranchAddress("ihcal_E3x3",&ihcal_E3x3); 
  track_trees->SetBranchAddress("ohcal_E3x3",&ohcal_E3x3); 
  float cemc_E5x5[999], ihcal_E5x5[999], ohcal_E5x5[999];
  track_trees->SetBranchAddress("cemc_E5x5",&cemc_E5x5); 
  track_trees->SetBranchAddress("ihcal_E5x5",&ihcal_E5x5); 
  track_trees->SetBranchAddress("ohcal_E5x5",&ohcal_E5x5); 
  float cemc_clE[999], ihcal_clE[999], ohcal_clE[999];
  track_trees->SetBranchAddress("cemc_clE",&cemc_clE); 
  track_trees->SetBranchAddress("ihcal_clE",&ihcal_clE); 
  track_trees->SetBranchAddress("ohcal_clE",&ohcal_clE); 

  int nsim = track_trees->GetEntries();
  float allThree =0;
  float ohcal_incident =0;
  float ohcal_frac =0;
  float outerTwo_incident =0;
  float outerTwo_frac =0;
  float allThree_frac =0;
  float diffIO_frac =0;
  float diffHE_frac =0;

  float cemc_e;
  float ihcal_e;
  float ohcal_e;

  for(int isim=0; isim<nsim; isim++){
    track_trees->GetEvent(isim);
    if (isim%1000==0) { cout << isim<<"th events" << endl; }
    for(int itrk=0; itrk<track_n; itrk++){
      if (do_E3x3){    
        cemc_e=cemc_E3x3[itrk]*cemc_sf;
        ihcal_e=ihcal_E3x3[itrk]*ihcal_sf;
        ohcal_e=ohcal_E3x3[itrk]*ohcal_sf;
        allThree = cemc_e+ihcal_e+ohcal_e;
        ohcal_incident = true_p - (cemc_e+ihcal_e);
        ohcal_frac = ohcal_e/ohcal_incident;
        outerTwo_incident = true_p - cemc_e;
        outerTwo_frac = (ihcal_e+ohcal_e)/outerTwo_incident;
        allThree_frac = allThree/true_p;
        diffIO_frac = (ihcal_e-ohcal_e) / (ihcal_e+ohcal_e);
        diffHE_frac = ((ihcal_e+ohcal_e)-cemc_e) / ((ihcal_e+ohcal_e)+cemc_e);
        //
        h1D_cemc_E3x3->Fill(cemc_e); 
        h1D_ihcal_E3x3->Fill(ihcal_e); 
        h1D_ohcal_E3x3->Fill(ohcal_e); 
        h1D_allThree_E3x3->Fill(allThree); 
        h1D_ohcal_incident_E3x3->Fill(ohcal_incident); 
        h1D_ohcal_frac_E3x3->Fill(ohcal_frac); 
        h1D_allThree_frac_E3x3->Fill(allThree_frac); 
        h1D_outerTwo_incident_E3x3->Fill(outerTwo_incident); 
        h1D_outerTwo_frac_E3x3->Fill(outerTwo_frac); 
        //
        h2D_cemc_vs_ihcal_E3x3->Fill(cemc_e,ihcal_e);
        h2D_cemc_vs_ohcal_E3x3->Fill(cemc_e,ohcal_e);
        h2D_ihcal_vs_ohcal_E3x3->Fill(ihcal_e,ohcal_e);
        h2D_innerTwo_vs_ohcal_E3x3->Fill(cemc_e+ihcal_e, ohcal_e);
        h2D_cemc_vs_outerTwo_E3x3->Fill(cemc_e, ihcal_e+ohcal_e);
        h2D_outerTwo_frac_vs_diffIO_frac_E3x3->Fill(diffIO_frac,outerTwo_frac);
        h2D_allThree_frac_vs_diffHE_frac_E3x3->Fill(diffHE_frac,allThree_frac);
        //// skip if cemc && ihcal && ohcal are all mip (black hole)
        if ( !(cemc_E3x3[itrk] < cemc_mip_cut && ihcal_E3x3[itrk]< ihcal_mip_cut && ohcal_E3x3[itrk] < ohcal_mip_cut) ){
          h2D_outerTwo_frac_vs_diffIO_frac_E3x3_nomip->Fill(diffIO_frac,outerTwo_frac);
          h2D_allThree_frac_vs_diffHE_frac_E3x3_nomip->Fill(diffHE_frac,allThree_frac);
        }
      }
      if (do_E5x5){    
        cemc_e=cemc_E5x5[itrk]*cemc_sf;
        ihcal_e=ihcal_E5x5[itrk]*ihcal_sf;
        ohcal_e=ohcal_E5x5[itrk]*ohcal_sf;
        allThree = cemc_e+ihcal_e+ohcal_e;
        ohcal_incident = true_p - (cemc_e+ihcal_e);
        ohcal_frac = ohcal_e/ohcal_incident;
        outerTwo_incident = true_p - cemc_e;
        outerTwo_frac = (ihcal_e+ohcal_e)/outerTwo_incident;
        allThree_frac = allThree/true_p;
        diffIO_frac = (ihcal_e-ohcal_e) / (ihcal_e+ohcal_e);
        diffHE_frac = ((ihcal_e+ohcal_e)-cemc_e) / ((ihcal_e+ohcal_e)+cemc_e);
        //
        h1D_cemc_E5x5->Fill(cemc_e); 
        h1D_ihcal_E5x5->Fill(ihcal_e); 
        h1D_ohcal_E5x5->Fill(ohcal_e); 
        h1D_allThree_E5x5->Fill(allThree); 
        h1D_ohcal_incident_E5x5->Fill(ohcal_incident); 
        h1D_ohcal_frac_E5x5->Fill(ohcal_frac); 
        h1D_allThree_frac_E5x5->Fill(allThree_frac); 
        h1D_outerTwo_incident_E5x5->Fill(outerTwo_incident); 
        h1D_outerTwo_frac_E5x5->Fill(outerTwo_frac); 
        //
        h2D_cemc_vs_ihcal_E5x5->Fill(cemc_e,ihcal_e);
        h2D_cemc_vs_ohcal_E5x5->Fill(cemc_e,ohcal_e);
        h2D_ihcal_vs_ohcal_E5x5->Fill(ihcal_e,ohcal_e);
        h2D_innerTwo_vs_ohcal_E5x5->Fill(cemc_e+ihcal_e, ohcal_e);
        h2D_cemc_vs_outerTwo_E5x5->Fill(cemc_e, ihcal_e+ohcal_e);
        h2D_outerTwo_frac_vs_diffIO_frac_E5x5->Fill(diffIO_frac,outerTwo_frac);
        h2D_allThree_frac_vs_diffHE_frac_E5x5->Fill(diffHE_frac,allThree_frac);
        if ( !(cemc_E5x5[itrk] < cemc_mip_cut && ihcal_E5x5[itrk]< ihcal_mip_cut && ohcal_E5x5[itrk] < ohcal_mip_cut) ){
          h2D_outerTwo_frac_vs_diffIO_frac_E5x5_nomip->Fill(diffIO_frac,outerTwo_frac);
          h2D_allThree_frac_vs_diffHE_frac_E5x5_nomip->Fill(diffHE_frac,allThree_frac);
        }
      }
      if (do_clE){    
        cemc_e=cemc_clE[itrk]*cemc_sf;
        ihcal_e=ihcal_clE[itrk]*ihcal_sf;
        ohcal_e=ohcal_clE[itrk]*ohcal_sf;
        allThree = cemc_e+ihcal_e+ohcal_e;
        ohcal_incident = true_p - (cemc_e+ihcal_e);
        ohcal_frac = ohcal_e/ohcal_incident;
        outerTwo_incident = true_p - cemc_e;
        outerTwo_frac = (ihcal_e+ohcal_e)/outerTwo_incident;
        allThree_frac = allThree/true_p;
        diffIO_frac = (ihcal_e-ohcal_e) / (ihcal_e+ohcal_e);
        diffHE_frac = ((ihcal_e+ohcal_e)-cemc_e) / ((ihcal_e+ohcal_e)+cemc_e);
        //
        h1D_cemc_clE->Fill(cemc_e); 
        h1D_ihcal_clE->Fill(ihcal_e); 
        h1D_ohcal_clE->Fill(ohcal_e); 
        h1D_allThree_clE->Fill(allThree); 
        h1D_ohcal_incident_clE->Fill(ohcal_incident); 
        h1D_ohcal_frac_clE->Fill(ohcal_frac); 
        h1D_allThree_frac_clE->Fill(allThree_frac); 
        h1D_outerTwo_incident_clE->Fill(outerTwo_incident); 
        h1D_outerTwo_frac_clE->Fill(outerTwo_frac); 
        //
        h2D_cemc_vs_ihcal_clE->Fill(cemc_e,ihcal_e);
        h2D_cemc_vs_ohcal_clE->Fill(cemc_e,ohcal_e);
        h2D_ihcal_vs_ohcal_clE->Fill(ihcal_e,ohcal_e);
        h2D_innerTwo_vs_ohcal_clE->Fill(cemc_e+ihcal_e, ohcal_e);
        h2D_cemc_vs_outerTwo_clE->Fill(cemc_e, ihcal_e+ohcal_e);
        h2D_outerTwo_frac_vs_diffIO_frac_clE->Fill(diffIO_frac,outerTwo_frac);
        h2D_allThree_frac_vs_diffHE_frac_clE->Fill(diffHE_frac,allThree_frac);
        if ( !(cemc_clE[itrk] < cemc_mip_cut && ihcal_clE[itrk]< ihcal_mip_cut && ohcal_clE[itrk] < ohcal_mip_cut) ){
          h2D_outerTwo_frac_vs_diffIO_frac_clE_nomip->Fill(diffIO_frac,outerTwo_frac);
          h2D_allThree_frac_vs_diffHE_frac_clE_nomip->Fill(diffHE_frac,allThree_frac);
        }
      }
      
      ////////////// cut (energy before scaling) ///////////////

      if (cemc_E3x3[itrk] < cemc_mip_cut && ihcal_E3x3[itrk]< ihcal_mip_cut){
        if (do_E3x3){    
          cemc_e=cemc_E3x3[itrk]*cemc_sf;
          ihcal_e=ihcal_E3x3[itrk]*ihcal_sf;
          ohcal_e=ohcal_E3x3[itrk]*ohcal_sf;
          allThree = cemc_e+ihcal_e+ohcal_e;
          ohcal_incident = true_p - (cemc_e+ihcal_e);
          ohcal_frac = ohcal_e/ohcal_incident;
          outerTwo_incident = true_p - cemc_e;
          outerTwo_frac = (ihcal_e+ohcal_e)/outerTwo_incident;
          allThree_frac = allThree/true_p;
          diffIO_frac = (ihcal_e-ohcal_e) / (ihcal_e+ohcal_e);
          diffHE_frac = ((ihcal_e+ohcal_e)-cemc_e) / ((ihcal_e+ohcal_e)+cemc_e);
          ////
          h1D_cemc_E3x3_cut->Fill(cemc_e); 
          h1D_ihcal_E3x3_cut->Fill(ihcal_e); 
          h1D_ohcal_E3x3_cut->Fill(ohcal_e); 
          h1D_allThree_E3x3_cut->Fill(allThree); 
          h1D_ohcal_incident_E3x3_cut->Fill(ohcal_incident); 
          h1D_ohcal_frac_E3x3_cut->Fill(ohcal_frac); 
          h1D_allThree_frac_E3x3_cut->Fill(allThree_frac); 
          h1D_outerTwo_incident_E3x3_cut->Fill(outerTwo_incident); 
          h1D_outerTwo_frac_E3x3_cut->Fill(outerTwo_frac); 
          //
          h2D_cemc_vs_ihcal_E3x3_cut->Fill(cemc_e,ihcal_e);
          h2D_cemc_vs_ohcal_E3x3_cut->Fill(cemc_e,ohcal_e);
          h2D_ihcal_vs_ohcal_E3x3_cut->Fill(ihcal_e,ohcal_e);
          h2D_innerTwo_vs_ohcal_E3x3_cut->Fill(cemc_e+ihcal_e, ohcal_e);
          h2D_cemc_vs_outerTwo_E3x3_cut->Fill(cemc_e, ihcal_e+ohcal_e);
          h2D_outerTwo_frac_vs_diffIO_frac_E3x3_cut->Fill(diffIO_frac,outerTwo_frac);
          h2D_allThree_frac_vs_diffHE_frac_E3x3_cut->Fill(diffHE_frac,allThree_frac);
          if ( !(cemc_E3x3[itrk] < cemc_mip_cut && ihcal_E3x3[itrk]< ihcal_mip_cut && ohcal_E3x3[itrk] < ohcal_mip_cut) ){
            h2D_outerTwo_frac_vs_diffIO_frac_E3x3_cut_nomip->Fill(diffIO_frac,outerTwo_frac);
            h2D_allThree_frac_vs_diffHE_frac_E3x3_cut_nomip->Fill(diffHE_frac,allThree_frac);
          }
        }
        if (do_E5x5){    
          cemc_e=cemc_E5x5[itrk]*cemc_sf;
          ihcal_e=ihcal_E5x5[itrk]*ihcal_sf;
          ohcal_e=ohcal_E5x5[itrk]*ohcal_sf;
          allThree = cemc_e+ihcal_e+ohcal_e;
          ohcal_incident = true_p - (cemc_e+ihcal_e);
          ohcal_frac = ohcal_e/ohcal_incident;
          outerTwo_incident = true_p - cemc_e;
          outerTwo_frac = (ihcal_e+ohcal_e)/outerTwo_incident;
          allThree_frac = allThree/true_p;
          diffIO_frac = (ihcal_e-ohcal_e) / (ihcal_e+ohcal_e);
          diffHE_frac = ((ihcal_e+ohcal_e)-cemc_e) / ((ihcal_e+ohcal_e)+cemc_e);
          ////
          h1D_cemc_E5x5_cut->Fill(cemc_e); 
          h1D_ihcal_E5x5_cut->Fill(ihcal_e); 
          h1D_ohcal_E5x5_cut->Fill(ohcal_e); 
          h1D_allThree_E5x5_cut->Fill(allThree); 
          h1D_ohcal_incident_E5x5_cut->Fill(ohcal_incident); 
          h1D_ohcal_frac_E5x5_cut->Fill(ohcal_frac); 
          h1D_allThree_frac_E5x5_cut->Fill(allThree_frac); 
          h1D_outerTwo_incident_E5x5_cut->Fill(outerTwo_incident); 
          h1D_outerTwo_frac_E5x5_cut->Fill(outerTwo_frac); 
          //
          h2D_cemc_vs_ihcal_E5x5_cut->Fill(cemc_e,ihcal_e);
          h2D_cemc_vs_ohcal_E5x5_cut->Fill(cemc_e,ohcal_e);
          h2D_ihcal_vs_ohcal_E5x5_cut->Fill(ihcal_e,ohcal_e);
          h2D_innerTwo_vs_ohcal_E5x5_cut->Fill(cemc_e+ihcal_e, ohcal_e);
          h2D_cemc_vs_outerTwo_E5x5_cut->Fill(cemc_e, ihcal_e+ohcal_e);
          h2D_outerTwo_frac_vs_diffIO_frac_E5x5_cut->Fill(diffIO_frac,outerTwo_frac);
          h2D_allThree_frac_vs_diffHE_frac_E5x5_cut->Fill(diffHE_frac,allThree_frac);
          if ( !(cemc_E5x5[itrk] < cemc_mip_cut && ihcal_E5x5[itrk]< ihcal_mip_cut && ohcal_E5x5[itrk] < ohcal_mip_cut) ){
            h2D_outerTwo_frac_vs_diffIO_frac_E5x5_cut_nomip->Fill(diffIO_frac,outerTwo_frac);
            h2D_allThree_frac_vs_diffHE_frac_E5x5_cut_nomip->Fill(diffHE_frac,allThree_frac);
          }
        }
        if (do_clE){    
          cemc_e=cemc_clE[itrk]*cemc_sf;
          ihcal_e=ihcal_clE[itrk]*ihcal_sf;
          ohcal_e=ohcal_clE[itrk]*ohcal_sf;
          allThree = cemc_e+ihcal_e+ohcal_e;
          ohcal_incident = true_p - (cemc_e+ihcal_e);
          ohcal_frac = ohcal_e/ohcal_incident;
          outerTwo_incident = true_p - cemc_e;
          outerTwo_frac = (ihcal_e+ohcal_e)/outerTwo_incident;
          allThree_frac = allThree/true_p;
          diffIO_frac = (ihcal_e-ohcal_e) / (ihcal_e+ohcal_e);
          diffHE_frac = ((ihcal_e+ohcal_e)-cemc_e) / ((ihcal_e+ohcal_e)+cemc_e);
          ////
          h1D_cemc_clE_cut->Fill(cemc_e); 
          h1D_ihcal_clE_cut->Fill(ihcal_e); 
          h1D_ohcal_clE_cut->Fill(ohcal_e); 
          h1D_allThree_clE_cut->Fill(allThree); 
          h1D_ohcal_incident_clE_cut->Fill(ohcal_incident); 
          h1D_ohcal_frac_clE_cut->Fill(ohcal_frac); 
          h1D_allThree_frac_clE_cut->Fill(allThree_frac); 
          h1D_outerTwo_incident_clE_cut->Fill(outerTwo_incident); 
          h1D_outerTwo_frac_clE_cut->Fill(outerTwo_frac); 
          //
          h2D_cemc_vs_ihcal_clE_cut->Fill(cemc_e,ihcal_e);
          h2D_cemc_vs_ohcal_clE_cut->Fill(cemc_e,ohcal_e);
          h2D_ihcal_vs_ohcal_clE_cut->Fill(ihcal_e,ohcal_e);
          h2D_innerTwo_vs_ohcal_clE_cut->Fill(cemc_e+ihcal_e, ohcal_e);
          h2D_cemc_vs_outerTwo_clE_cut->Fill(cemc_e, ihcal_e+ohcal_e);
          h2D_outerTwo_frac_vs_diffIO_frac_clE_cut->Fill(diffIO_frac,outerTwo_frac);
          h2D_allThree_frac_vs_diffHE_frac_clE_cut->Fill(diffHE_frac,allThree_frac);
          if ( !(cemc_clE[itrk] < cemc_mip_cut && ihcal_clE[itrk]< ihcal_mip_cut && ohcal_clE[itrk] < ohcal_mip_cut) ){
            h2D_outerTwo_frac_vs_diffIO_frac_clE_cut_nomip->Fill(diffIO_frac,outerTwo_frac);
            h2D_allThree_frac_vs_diffHE_frac_clE_cut_nomip->Fill(diffHE_frac,allThree_frac);
          }
        }
      } //end of cut
    } //end of ntrk loop
  }  //end of nsim loop
      
  h2D_cemc_vs_ihcal_E3x3->SetOption("colz");
  h2D_cemc_vs_ohcal_E3x3->SetOption("colz");
  h2D_ihcal_vs_ohcal_E3x3->SetOption("colz");
  h2D_innerTwo_vs_ohcal_E3x3->SetOption("colz");
  h2D_cemc_vs_outerTwo_E3x3->SetOption("colz");
  h2D_outerTwo_frac_vs_diffIO_frac_E3x3->SetOption("colz");
  h2D_allThree_frac_vs_diffHE_frac_E3x3->SetOption("colz");
  h2D_outerTwo_frac_vs_diffIO_frac_E3x3_nomip->SetOption("colz");
  h2D_allThree_frac_vs_diffHE_frac_E3x3_nomip->SetOption("colz");
  //
  h2D_cemc_vs_ihcal_E5x5->SetOption("colz");
  h2D_cemc_vs_ohcal_E5x5->SetOption("colz");
  h2D_ihcal_vs_ohcal_E5x5->SetOption("colz");
  h2D_innerTwo_vs_ohcal_E5x5->SetOption("colz");
  h2D_cemc_vs_outerTwo_E5x5->SetOption("colz");
  h2D_outerTwo_frac_vs_diffIO_frac_E5x5->SetOption("colz");
  h2D_allThree_frac_vs_diffHE_frac_E5x5->SetOption("colz");
  h2D_outerTwo_frac_vs_diffIO_frac_E5x5_nomip->SetOption("colz");
  h2D_allThree_frac_vs_diffHE_frac_E5x5_nomip->SetOption("colz");
  //
  h2D_cemc_vs_ihcal_clE->SetOption("colz");
  h2D_cemc_vs_ohcal_clE->SetOption("colz");
  h2D_ihcal_vs_ohcal_clE->SetOption("colz");
  h2D_innerTwo_vs_ohcal_clE->SetOption("colz");
  h2D_cemc_vs_outerTwo_clE->SetOption("colz");
  h2D_outerTwo_frac_vs_diffIO_frac_clE->SetOption("colz");
  h2D_allThree_frac_vs_diffHE_frac_clE->SetOption("colz");
  h2D_outerTwo_frac_vs_diffIO_frac_clE_nomip->SetOption("colz");
  h2D_allThree_frac_vs_diffHE_frac_clE_nomip->SetOption("colz");
  ////
  h2D_cemc_vs_ihcal_E3x3_cut->SetOption("colz");
  h2D_cemc_vs_ohcal_E3x3_cut->SetOption("colz");
  h2D_ihcal_vs_ohcal_E3x3_cut->SetOption("colz");
  h2D_innerTwo_vs_ohcal_E3x3_cut->SetOption("colz");
  h2D_cemc_vs_outerTwo_E3x3_cut->SetOption("colz");
  h2D_outerTwo_frac_vs_diffIO_frac_E3x3_cut->SetOption("colz");
  h2D_allThree_frac_vs_diffHE_frac_E3x3_cut->SetOption("colz");
  h2D_outerTwo_frac_vs_diffIO_frac_E3x3_cut_nomip->SetOption("colz");
  h2D_allThree_frac_vs_diffHE_frac_E3x3_cut_nomip->SetOption("colz");
  //
  h2D_cemc_vs_ihcal_E5x5_cut->SetOption("colz");
  h2D_cemc_vs_ohcal_E5x5_cut->SetOption("colz");
  h2D_ihcal_vs_ohcal_E5x5_cut->SetOption("colz");
  h2D_innerTwo_vs_ohcal_E5x5_cut->SetOption("colz");
  h2D_cemc_vs_outerTwo_E5x5_cut->SetOption("colz");
  h2D_outerTwo_frac_vs_diffIO_frac_E5x5_cut->SetOption("colz");
  h2D_allThree_frac_vs_diffHE_frac_E5x5_cut->SetOption("colz");
  h2D_outerTwo_frac_vs_diffIO_frac_E5x5_cut_nomip->SetOption("colz");
  h2D_allThree_frac_vs_diffHE_frac_E5x5_cut_nomip->SetOption("colz");
  //
  h2D_cemc_vs_ihcal_clE_cut->SetOption("colz");
  h2D_cemc_vs_ohcal_clE_cut->SetOption("colz");
  h2D_ihcal_vs_ohcal_clE_cut->SetOption("colz");
  h2D_innerTwo_vs_ohcal_clE_cut->SetOption("colz");
  h2D_cemc_vs_outerTwo_clE_cut->SetOption("colz");
  h2D_outerTwo_frac_vs_diffIO_frac_clE_cut->SetOption("colz");
  h2D_allThree_frac_vs_diffHE_frac_clE_cut->SetOption("colz");
  h2D_outerTwo_frac_vs_diffIO_frac_clE_cut_nomip->SetOption("colz");
  h2D_allThree_frac_vs_diffHE_frac_clE_cut_nomip->SetOption("colz");
  
  //TCanvas* c1 = new TCanvas("c1","",600,600);
  //h2D_JES_ohcal_scale->Draw();

  TFile* fout = new TFile(Form("out_scale/%s_scale_%d_%d_%d.root",inType.c_str(),(int)do_cemc_scale,(int)do_ihcal_scale,(int)do_ohcal_scale),"RECREATE");
  fout->cd();
  
  TDirectory* beforeCut = fout->mkdir("beforeCut");
  beforeCut->cd();
  if (do_E3x3){    
    h1D_cemc_E3x3->Write();
    h1D_ihcal_E3x3->Write();
    h1D_ohcal_E3x3->Write();
    h1D_allThree_E3x3->Write();
    h1D_ohcal_incident_E3x3->Write();
    h1D_ohcal_frac_E3x3->Write();
    h1D_allThree_frac_E3x3->Write();
    h1D_outerTwo_incident_E3x3->Write();
    h1D_outerTwo_frac_E3x3->Write();
    //
    h2D_cemc_vs_ihcal_E3x3->Write();
    h2D_cemc_vs_ohcal_E3x3->Write();
    h2D_ihcal_vs_ohcal_E3x3->Write();
    h2D_innerTwo_vs_ohcal_E3x3->Write();
    h2D_cemc_vs_outerTwo_E3x3->Write();
    h2D_outerTwo_frac_vs_diffIO_frac_E3x3->Write();
    h2D_allThree_frac_vs_diffHE_frac_E3x3->Write();
    h2D_outerTwo_frac_vs_diffIO_frac_E3x3_nomip->Write();
    h2D_allThree_frac_vs_diffHE_frac_E3x3_nomip->Write();
  }
  if (do_E5x5){    
    h1D_cemc_E5x5->Write();
    h1D_ihcal_E5x5->Write();
    h1D_ohcal_E5x5->Write();
    h1D_allThree_E5x5->Write();
    h1D_ohcal_incident_E5x5->Write();
    h1D_ohcal_frac_E5x5->Write();
    h1D_allThree_frac_E5x5->Write();
    h1D_outerTwo_incident_E5x5->Write();
    h1D_outerTwo_frac_E5x5->Write();
    //
    h2D_cemc_vs_ihcal_E5x5->Write();
    h2D_cemc_vs_ohcal_E5x5->Write();
    h2D_ihcal_vs_ohcal_E5x5->Write();
    h2D_innerTwo_vs_ohcal_E5x5->Write();
    h2D_cemc_vs_outerTwo_E5x5->Write();
    h2D_outerTwo_frac_vs_diffIO_frac_E5x5->Write();
    h2D_allThree_frac_vs_diffHE_frac_E5x5->Write();
    h2D_outerTwo_frac_vs_diffIO_frac_E5x5_nomip->Write();
    h2D_allThree_frac_vs_diffHE_frac_E5x5_nomip->Write();
  }
  if (do_clE){    
    h1D_cemc_clE->Write();
    h1D_ihcal_clE->Write();
    h1D_ohcal_clE->Write();
    h1D_allThree_clE->Write();
    h1D_ohcal_incident_clE->Write();
    h1D_ohcal_frac_clE->Write();
    h1D_allThree_frac_clE->Write();
    h1D_outerTwo_incident_clE->Write();
    h1D_outerTwo_frac_clE->Write();
    //
    h2D_cemc_vs_ihcal_clE->Write();
    h2D_cemc_vs_ohcal_clE->Write();
    h2D_ihcal_vs_ohcal_clE->Write();
    h2D_innerTwo_vs_ohcal_clE->Write();
    h2D_cemc_vs_outerTwo_clE->Write();
    h2D_outerTwo_frac_vs_diffIO_frac_clE->Write();
    h2D_allThree_frac_vs_diffHE_frac_clE->Write();
    h2D_outerTwo_frac_vs_diffIO_frac_clE_nomip->Write();
    h2D_allThree_frac_vs_diffHE_frac_clE_nomip->Write();
  }
  
  TDirectory* afterCut = fout->mkdir("afterCut");
  afterCut->cd();
  if (do_E3x3){    
    h1D_cemc_E3x3_cut->Write();
    h1D_ihcal_E3x3_cut->Write();
    h1D_ohcal_E3x3_cut->Write();
    h1D_allThree_E3x3_cut->Write();
    h1D_ohcal_incident_E3x3_cut->Write();
    h1D_ohcal_frac_E3x3_cut->Write();
    h1D_allThree_frac_E3x3_cut->Write();
    h1D_outerTwo_incident_E3x3_cut->Write();
    h1D_outerTwo_frac_E3x3_cut->Write();
    //
    h2D_cemc_vs_ihcal_E3x3_cut->Write();
    h2D_cemc_vs_ohcal_E3x3_cut->Write();
    h2D_ihcal_vs_ohcal_E3x3_cut->Write();
    h2D_innerTwo_vs_ohcal_E3x3_cut->Write();
    h2D_cemc_vs_outerTwo_E3x3_cut->Write();
    h2D_outerTwo_frac_vs_diffIO_frac_E3x3_cut->Write();
    h2D_allThree_frac_vs_diffHE_frac_E3x3_cut->Write();
    h2D_outerTwo_frac_vs_diffIO_frac_E3x3_cut_nomip->Write();
    h2D_allThree_frac_vs_diffHE_frac_E3x3_cut_nomip->Write();
  }
  if (do_E5x5){    
    h1D_cemc_E5x5_cut->Write();
    h1D_ihcal_E5x5_cut->Write();
    h1D_ohcal_E5x5_cut->Write();
    h1D_allThree_E5x5_cut->Write();
    h1D_ohcal_incident_E5x5_cut->Write();
    h1D_ohcal_frac_E5x5_cut->Write();
    h1D_allThree_frac_E5x5_cut->Write();
    h1D_outerTwo_incident_E5x5_cut->Write();
    h1D_outerTwo_frac_E5x5_cut->Write();
    //
    h2D_cemc_vs_ihcal_E5x5_cut->Write();
    h2D_cemc_vs_ohcal_E5x5_cut->Write();
    h2D_ihcal_vs_ohcal_E5x5_cut->Write();
    h2D_innerTwo_vs_ohcal_E5x5_cut->Write();
    h2D_cemc_vs_outerTwo_E5x5_cut->Write();
    h2D_outerTwo_frac_vs_diffIO_frac_E5x5_cut->Write();
    h2D_allThree_frac_vs_diffHE_frac_E5x5_cut->Write();
    h2D_outerTwo_frac_vs_diffIO_frac_E5x5_cut_nomip->Write();
    h2D_allThree_frac_vs_diffHE_frac_E5x5_cut_nomip->Write();
  }
  if (do_clE){    
    h1D_cemc_clE_cut->Write();
    h1D_ihcal_clE_cut->Write();
    h1D_ohcal_clE_cut->Write();
    h1D_allThree_clE_cut->Write();
    h1D_ohcal_incident_clE_cut->Write();
    h1D_ohcal_frac_clE_cut->Write();
    h1D_allThree_frac_clE_cut->Write();
    h1D_outerTwo_incident_clE_cut->Write();
    h1D_outerTwo_frac_clE_cut->Write();
    //
    h2D_cemc_vs_ihcal_clE_cut->Write();
    h2D_cemc_vs_ohcal_clE_cut->Write();
    h2D_ihcal_vs_ohcal_clE_cut->Write();
    h2D_innerTwo_vs_ohcal_clE_cut->Write();
    h2D_cemc_vs_outerTwo_clE_cut->Write();
    h2D_outerTwo_frac_vs_diffIO_frac_clE_cut->Write();
    h2D_allThree_frac_vs_diffHE_frac_clE_cut->Write();
    h2D_outerTwo_frac_vs_diffIO_frac_clE_cut_nomip->Write();
    h2D_allThree_frac_vs_diffHE_frac_clE_cut_nomip->Write();
  } 
  
  fout->Close();
  
  return; 
}
