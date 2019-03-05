
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
  
//#define cemcSF 1.07
//#define ihcalSF 3.60
//#define ohcalSF 1.28

///////////
//MyTree_SS310_NotScaled (int pid) 600
//MyTree_Al_NotScaled (float pid) 666
///////////

////// This code is similar to rootAna_emfrac.C, 
////// but scale the jet (after jet finding)
void rootAna_GluonQuarkJet4(string ihcalType = "SS310",string jetE="50GeV",int initfile=0,int endfile =300, float cemc_sf=1.00, float ihcal_sf=1.00, float ohcal_sf=1.00)
{

  //// Suppress error messages from bad files
  gErrorIgnoreLevel = kFatal; 

  int bad_files = 0; 
 
  ////////////////////////////////////////////////////////
  TH1D* h1D_truthjet4_e = new TH1D("truthjet4_e",";Truth Jet E;",100,0,100);
  TH1D* h1D_truthjet4_p = new TH1D("truthjet4_p",";Truth Jet p;",100,0,100);
  TH1D* h1D_truthjet4_pt = new TH1D("truthjet4_pt",";Truth Jet p_{T};",100,0,100);
  TH1D* h1D_truthjet4_eta = new TH1D("truthjet4_eta",";Truth Jet #eta;",100,-1.5,1.5);
  TH1D* h1D_truthjet4_phi = new TH1D("truthjet4_phi",";Truth Jet #phi;",100,-3.5,3.5);
  TH1D* h1D_truthjet4_cons_n = new TH1D("truthjet4_cons_n",";Truth Jet # of constituents;",100,0,100);
  TH1D* h1D_truthjet4_cons_e = new TH1D("truthjet4_cons_e",";Truth Jet constituents E;",80,0,80);
  TH1D* h1D_truthjet4_cons_p = new TH1D("truthjet4_cons_p",";Truth Jet constituents p;",80,0,80);
  TH1D* h1D_truthjet4_cons_pt = new TH1D("truthjet4_cons_pt",";Truth Jet constituents p_{T};",80,0,80);
  TH1D* h1D_truthjet4_cons_eta = new TH1D("truthjet4_cons_eta",";Truth Jet constituents #eta;",100,-1.5,1.5);
  TH1D* h1D_truthjet4_cons_phi = new TH1D("truthjet4_cons_phi",";Truth Jet constituents #phi;",100,-3.5,3.5);
  TH1D* h1D_truthjet4_cons_dR = new TH1D("truthjet4_cons_dR",";Truth Jet constituents dR;",100,0.,1.0);
  TH1D* h1D_truthjet4_cons_pid = new TH1D("truthjet4_cons_pid",";Truth Jet constituents pid;",5000,-2500,2500);
  TH1D* h1D_truthjet4_pg_n = new TH1D("truthjet4_pg_n",";Truth Jet progenitor id;",100,0,50);
  TH1D* h1D_truthjet4_pg_id = new TH1D("truthjet4_pg_id",";Truth Jet progenitor id;",5000,-2500,2500);
  TH1D* h1D_truthjet4_pg_fract = new TH1D("truthjet4_pg_fract",";Truth Jet progenitor fract;",100,0,1);
  TH1D* h1D_truthjet4_pg_status = new TH1D("truthjet4_pg_status",";Truth Jet progenitor status;",100,0,100);
  //////////
  TH1D* h1D_truthjet4_quark_e = new TH1D("truthjet4_quark_e",";Truth Jet E;",100,0,100);
  TH1D* h1D_truthjet4_quark_p = new TH1D("truthjet4_quark_p",";Truth Jet p;",100,0,100);
  TH1D* h1D_truthjet4_quark_pt = new TH1D("truthjet4_quark_pt",";Truth Jet p_{T};",100,0,100);
  TH1D* h1D_truthjet4_quark_eta = new TH1D("truthjet4_quark_eta",";Truth Jet #eta;",100,-1.5,1.5);
  TH1D* h1D_truthjet4_quark_phi = new TH1D("truthjet4_quark_phi",";Truth Jet #phi;",100,-3.5,3.5);
  TH1D* h1D_truthjet4_quark_cons_n = new TH1D("truthjet4_quark_cons_n",";Truth Jet # of constituents;",100,0,100);
  TH1D* h1D_truthjet4_quark_cons_e = new TH1D("truthjet4_quark_cons_e",";Truth Jet constituents E;",80,0,80);
  TH1D* h1D_truthjet4_quark_cons_p = new TH1D("truthjet4_quark_cons_p",";Truth Jet constituents p;",80,0,80);
  TH1D* h1D_truthjet4_quark_cons_pt = new TH1D("truthjet4_quark_cons_pt",";Truth Jet constituents p_{T};",80,0,80);
  TH1D* h1D_truthjet4_quark_cons_eta = new TH1D("truthjet4_quark_cons_eta",";Truth Jet constituents #eta;",100,-1.5,1.5);
  TH1D* h1D_truthjet4_quark_cons_phi = new TH1D("truthjet4_quark_cons_phi",";Truth Jet constituents #phi;",100,-3.5,3.5);
  TH1D* h1D_truthjet4_quark_cons_dR = new TH1D("truthjet4_quark_cons_dR",";Truth Jet constituents dR;",100,0.,1.0);
  TH1D* h1D_truthjet4_quark_cons_pid = new TH1D("truthjet4_quark_cons_pid",";Truth Jet constituents pid;",5000,-2500,2500);
  TH1D* h1D_truthjet4_quark_pg_n = new TH1D("truthjet4_quark_pg_n",";Truth Jet progenitor id;",100,0,50);
  TH1D* h1D_truthjet4_quark_pg_id = new TH1D("truthjet4_quark_pg_id",";Truth Jet progenitor id;",5000,-2500,2500);
  TH1D* h1D_truthjet4_quark_pg_fract = new TH1D("truthjet4_quark_pg_fract",";Truth Jet progenitor fract;",100,0,1);
  TH1D* h1D_truthjet4_quark_pg_status = new TH1D("truthjet4_quark_pg_status",";Truth Jet progenitor status;",100,0,100);
  //////////
  TH1D* h1D_truthjet4_gluon_e = new TH1D("truthjet4_gluon_e",";Truth Jet E;",100,0,100);
  TH1D* h1D_truthjet4_gluon_p = new TH1D("truthjet4_gluon_p",";Truth Jet p;",100,0,100);
  TH1D* h1D_truthjet4_gluon_pt = new TH1D("truthjet4_gluon_pt",";Truth Jet p_{T};",100,0,100);
  TH1D* h1D_truthjet4_gluon_eta = new TH1D("truthjet4_gluon_eta",";Truth Jet #eta;",100,-1.5,1.5);
  TH1D* h1D_truthjet4_gluon_phi = new TH1D("truthjet4_gluon_phi",";Truth Jet #phi;",100,-3.5,3.5);
  TH1D* h1D_truthjet4_gluon_cons_n = new TH1D("truthjet4_gluon_cons_n",";Truth Jet # of constituents;",100,0,100);
  TH1D* h1D_truthjet4_gluon_cons_e = new TH1D("truthjet4_gluon_cons_e",";Truth Jet constituents E;",80,0,80);
  TH1D* h1D_truthjet4_gluon_cons_p = new TH1D("truthjet4_gluon_cons_p",";Truth Jet constituents p;",80,0,80);
  TH1D* h1D_truthjet4_gluon_cons_pt = new TH1D("truthjet4_gluon_cons_pt",";Truth Jet constituents p_{T};",80,0,80);
  TH1D* h1D_truthjet4_gluon_cons_eta = new TH1D("truthjet4_gluon_cons_eta",";Truth Jet constituents #eta;",100,-1.5,1.5);
  TH1D* h1D_truthjet4_gluon_cons_phi = new TH1D("truthjet4_gluon_cons_phi",";Truth Jet constituents #phi;",100,-3.5,3.5);
  TH1D* h1D_truthjet4_gluon_cons_dR = new TH1D("truthjet4_gluon_cons_dR",";Truth Jet constituents dR;",100,0.,1.0);
  TH1D* h1D_truthjet4_gluon_cons_pid = new TH1D("truthjet4_gluon_cons_pid",";Truth Jet constituents pid;",5000,-2500,2500);
  TH1D* h1D_truthjet4_gluon_pg_n = new TH1D("truthjet4_gluon_pg_n",";Truth Jet progenitor id;",100,0,50);
  TH1D* h1D_truthjet4_gluon_pg_id = new TH1D("truthjet4_gluon_pg_id",";Truth Jet progenitor id;",5000,-2500,2500);
  TH1D* h1D_truthjet4_gluon_pg_fract = new TH1D("truthjet4_gluon_pg_fract",";Truth Jet progenitor fract;",100,0,1);
  TH1D* h1D_truthjet4_gluon_pg_status = new TH1D("truthjet4_gluon_pg_status",";Truth Jet progenitor status;",100,0,100);
  ////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////
  TH1D* h1D_towerjet4_e = new TH1D("towerjet4_e",";Tower Jet E;",100,0,100);
  TH1D* h1D_towerjet4_p = new TH1D("towerjet4_p",";Tower Jet p;",100,0,100);
  TH1D* h1D_towerjet4_pt = new TH1D("towerjet4_pt",";Tower Jet p_{T};",100,0,100);
  TH1D* h1D_towerjet4_eta = new TH1D("towerjet4_eta",";Tower Jet #eta;",100,-1.5,1.5);
  TH1D* h1D_towerjet4_phi = new TH1D("towerjet4_phi",";Tower Jet #phi;",100,-3.5,3.5);
  TH1D* h1D_towerjet4_cemc_n = new TH1D("towerjet4_cemc_n",";Tower Jet # of cemc;",300,0,300);
  TH1D* h1D_towerjet4_cemc_e = new TH1D("towerjet4_cemc_e",";Tower Jet cemc E;",100,0,50);
  TH1D* h1D_towerjet4_cemc_p = new TH1D("towerjet4_cemc_p",";Tower Jet cemc p;",100,0,50);
  TH1D* h1D_towerjet4_cemc_pt = new TH1D("towerjet4_cemc_pt",";Tower Jet cemc p_{T};",100,0,50);
  TH1D* h1D_towerjet4_cemc_eta = new TH1D("towerjet4_cemc_eta",";Tower Jet cemc #eta;",100,-1.5,1.5);
  TH1D* h1D_towerjet4_cemc_phi = new TH1D("towerjet4_cemc_phi",";Tower Jet cemc #phi;",100,-3.5,3.5);
  TH1D* h1D_towerjet4_cemc_dR = new TH1D("towerjet4_cemc_dR",";Tower Jet cemc dR;",100,0.,1.0);
  TH1D* h1D_towerjet4_ihcal_n = new TH1D("towerjet4_ihcal_n",";Tower Jet # of ihcal;",100,0,100);
  TH1D* h1D_towerjet4_ihcal_e = new TH1D("towerjet4_ihcal_e",";Tower Jet ihcal E;",100,0,20);
  TH1D* h1D_towerjet4_ihcal_p = new TH1D("towerjet4_ihcal_p",";Tower Jet ihcal p;",100,0,20);
  TH1D* h1D_towerjet4_ihcal_pt = new TH1D("towerjet4_ihcal_pt",";Tower Jet ihcal p_{T};",100,0,20);
  TH1D* h1D_towerjet4_ihcal_eta = new TH1D("towerjet4_ihcal_eta",";Tower Jet ihcal #eta;",100,-1.5,1.5);
  TH1D* h1D_towerjet4_ihcal_phi = new TH1D("towerjet4_ihcal_phi",";Tower Jet ihcal #phi;",100,-3.5,3.5);
  TH1D* h1D_towerjet4_ihcal_dR = new TH1D("towerjet4_ihcal_dR",";Tower Jet ihcal dR;",100,0.,1.0);
  TH1D* h1D_towerjet4_ohcal_n = new TH1D("towerjet4_ohcal_n",";Tower Jet # of ohcal;",100,0,100);
  TH1D* h1D_towerjet4_ohcal_e = new TH1D("towerjet4_ohcal_e",";Tower Jet ohcal E;",100,0,50);
  TH1D* h1D_towerjet4_ohcal_p = new TH1D("towerjet4_ohcal_p",";Tower Jet ohcal p;",100,0,50);
  TH1D* h1D_towerjet4_ohcal_pt = new TH1D("towerjet4_ohcal_pt",";Tower Jet ohcal p_{T};",100,0,50);
  TH1D* h1D_towerjet4_ohcal_eta = new TH1D("towerjet4_ohcal_eta",";Tower Jet ohcal #eta;",100,-1.5,1.5);
  TH1D* h1D_towerjet4_ohcal_phi = new TH1D("towerjet4_ohcal_phi",";Tower Jet ohcal #phi;",100,-3.5,3.5);
  TH1D* h1D_towerjet4_ohcal_dR = new TH1D("towerjet4_ohcal_dR",";Tower Jet ohcal dR;",100,0.,1.0);
  ////////////////////////////////////////////////////////
  TH1D* h1D_towerjet4_quark_e = new TH1D("towerjet4_quark_e",";Tower Jet E;",100,0,100);
  TH1D* h1D_towerjet4_quark_p = new TH1D("towerjet4_quark_p",";Tower Jet p;",100,0,100);
  TH1D* h1D_towerjet4_quark_pt = new TH1D("towerjet4_quark_pt",";Tower Jet p_{T};",100,0,100);
  TH1D* h1D_towerjet4_quark_eta = new TH1D("towerjet4_quark_eta",";Tower Jet #eta;",100,-1.5,1.5);
  TH1D* h1D_towerjet4_quark_phi = new TH1D("towerjet4_quark_phi",";Tower Jet #phi;",100,-3.5,3.5);
  TH1D* h1D_towerjet4_quark_cemc_n = new TH1D("towerjet4_quark_cemc_n",";Tower Jet # of cemc;",300,0,300);
  TH1D* h1D_towerjet4_quark_cemc_e = new TH1D("towerjet4_quark_cemc_e",";Tower Jet cemc E;",100,0,50);
  TH1D* h1D_towerjet4_quark_cemc_p = new TH1D("towerjet4_quark_cemc_p",";Tower Jet cemc p;",100,0,50);
  TH1D* h1D_towerjet4_quark_cemc_pt = new TH1D("towerjet4_quark_cemc_pt",";Tower Jet cemc p_{T};",100,0,50);
  TH1D* h1D_towerjet4_quark_cemc_eta = new TH1D("towerjet4_quark_cemc_eta",";Tower Jet cemc #eta;",100,-1.5,1.5);
  TH1D* h1D_towerjet4_quark_cemc_phi = new TH1D("towerjet4_quark_cemc_phi",";Tower Jet cemc #phi;",100,-3.5,3.5);
  TH1D* h1D_towerjet4_quark_cemc_dR = new TH1D("towerjet4_quark_cemc_dR",";Tower Jet cemc dR;",100,0.,1.0);
  TH1D* h1D_towerjet4_quark_ihcal_n = new TH1D("towerjet4_quark_ihcal_n",";Tower Jet # of ihcal;",100,0,100);
  TH1D* h1D_towerjet4_quark_ihcal_e = new TH1D("towerjet4_quark_ihcal_e",";Tower Jet ihcal E;",100,0,20);
  TH1D* h1D_towerjet4_quark_ihcal_p = new TH1D("towerjet4_quark_ihcal_p",";Tower Jet ihcal p;",100,0,20);
  TH1D* h1D_towerjet4_quark_ihcal_pt = new TH1D("towerjet4_quark_ihcal_pt",";Tower Jet ihcal p_{T};",100,0,20);
  TH1D* h1D_towerjet4_quark_ihcal_eta = new TH1D("towerjet4_quark_ihcal_eta",";Tower Jet ihcal #eta;",100,-1.5,1.5);
  TH1D* h1D_towerjet4_quark_ihcal_phi = new TH1D("towerjet4_quark_ihcal_phi",";Tower Jet ihcal #phi;",100,-3.5,3.5);
  TH1D* h1D_towerjet4_quark_ihcal_dR = new TH1D("towerjet4_quark_ihcal_dR",";Tower Jet ihcal dR;",100,0.,1.0);
  TH1D* h1D_towerjet4_quark_ohcal_n = new TH1D("towerjet4_quark_ohcal_n",";Tower Jet # of ohcal;",100,0,100);
  TH1D* h1D_towerjet4_quark_ohcal_e = new TH1D("towerjet4_quark_ohcal_e",";Tower Jet ohcal E;",100,0,50);
  TH1D* h1D_towerjet4_quark_ohcal_p = new TH1D("towerjet4_quark_ohcal_p",";Tower Jet ohcal p;",100,0,50);
  TH1D* h1D_towerjet4_quark_ohcal_pt = new TH1D("towerjet4_quark_ohcal_pt",";Tower Jet ohcal p_{T};",100,0,50);
  TH1D* h1D_towerjet4_quark_ohcal_eta = new TH1D("towerjet4_quark_ohcal_eta",";Tower Jet ohcal #eta;",100,-1.5,1.5);
  TH1D* h1D_towerjet4_quark_ohcal_phi = new TH1D("towerjet4_quark_ohcal_phi",";Tower Jet ohcal #phi;",100,-3.5,3.5);
  TH1D* h1D_towerjet4_quark_ohcal_dR = new TH1D("towerjet4_quark_ohcal_dR",";Tower Jet ohcal dR;",100,0.,1.0);
  ////////////////////////////////////////////////////////
  TH1D* h1D_towerjet4_gluon_e = new TH1D("towerjet4_gluon_e",";Tower Jet E;",100,0,100);
  TH1D* h1D_towerjet4_gluon_p = new TH1D("towerjet4_gluon_p",";Tower Jet p;",100,0,100);
  TH1D* h1D_towerjet4_gluon_pt = new TH1D("towerjet4_gluon_pt",";Tower Jet p_{T};",100,0,100);
  TH1D* h1D_towerjet4_gluon_eta = new TH1D("towerjet4_gluon_eta",";Tower Jet #eta;",100,-1.5,1.5);
  TH1D* h1D_towerjet4_gluon_phi = new TH1D("towerjet4_gluon_phi",";Tower Jet #phi;",100,-3.5,3.5);
  TH1D* h1D_towerjet4_gluon_cemc_n = new TH1D("towerjet4_gluon_cemc_n",";Tower Jet # of cemc;",300,0,300);
  TH1D* h1D_towerjet4_gluon_cemc_e = new TH1D("towerjet4_gluon_cemc_e",";Tower Jet cemc E;",100,0,50);
  TH1D* h1D_towerjet4_gluon_cemc_p = new TH1D("towerjet4_gluon_cemc_p",";Tower Jet cemc p;",100,0,50);
  TH1D* h1D_towerjet4_gluon_cemc_pt = new TH1D("towerjet4_gluon_cemc_pt",";Tower Jet cemc p_{T};",100,0,50);
  TH1D* h1D_towerjet4_gluon_cemc_eta = new TH1D("towerjet4_gluon_cemc_eta",";Tower Jet cemc #eta;",100,-1.5,1.5);
  TH1D* h1D_towerjet4_gluon_cemc_phi = new TH1D("towerjet4_gluon_cemc_phi",";Tower Jet cemc #phi;",100,-3.5,3.5);
  TH1D* h1D_towerjet4_gluon_cemc_dR = new TH1D("towerjet4_gluon_cemc_dR",";Tower Jet cemc dR;",100,0.,1.0);
  TH1D* h1D_towerjet4_gluon_ihcal_n = new TH1D("towerjet4_gluon_ihcal_n",";Tower Jet # of ihcal;",100,0,100);
  TH1D* h1D_towerjet4_gluon_ihcal_e = new TH1D("towerjet4_gluon_ihcal_e",";Tower Jet ihcal E;",100,0,20);
  TH1D* h1D_towerjet4_gluon_ihcal_p = new TH1D("towerjet4_gluon_ihcal_p",";Tower Jet ihcal p;",100,0,20);
  TH1D* h1D_towerjet4_gluon_ihcal_pt = new TH1D("towerjet4_gluon_ihcal_pt",";Tower Jet ihcal p_{T};",100,0,20);
  TH1D* h1D_towerjet4_gluon_ihcal_eta = new TH1D("towerjet4_gluon_ihcal_eta",";Tower Jet ihcal #eta;",100,-1.5,1.5);
  TH1D* h1D_towerjet4_gluon_ihcal_phi = new TH1D("towerjet4_gluon_ihcal_phi",";Tower Jet ihcal #phi;",100,-3.5,3.5);
  TH1D* h1D_towerjet4_gluon_ihcal_dR = new TH1D("towerjet4_gluon_ihcal_dR",";Tower Jet ihcal dR;",100,0.,1.0);
  TH1D* h1D_towerjet4_gluon_ohcal_n = new TH1D("towerjet4_gluon_ohcal_n",";Tower Jet # of ohcal;",100,0,100);
  TH1D* h1D_towerjet4_gluon_ohcal_e = new TH1D("towerjet4_gluon_ohcal_e",";Tower Jet ohcal E;",100,0,50);
  TH1D* h1D_towerjet4_gluon_ohcal_p = new TH1D("towerjet4_gluon_ohcal_p",";Tower Jet ohcal p;",100,0,50);
  TH1D* h1D_towerjet4_gluon_ohcal_pt = new TH1D("towerjet4_gluon_ohcal_pt",";Tower Jet ohcal p_{T};",100,0,50);
  TH1D* h1D_towerjet4_gluon_ohcal_eta = new TH1D("towerjet4_gluon_ohcal_eta",";Tower Jet ohcal #eta;",100,-1.5,1.5);
  TH1D* h1D_towerjet4_gluon_ohcal_phi = new TH1D("towerjet4_gluon_ohcal_phi",";Tower Jet ohcal #phi;",100,-3.5,3.5);
  TH1D* h1D_towerjet4_gluon_ohcal_dR = new TH1D("towerjet4_gluon_ohcal_dR",";Tower Jet ohcal dR;",100,0.,1.0);
  ////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////
  TH1D* h1D_JES = new TH1D("h1D_JES",";JES (=E_{Tower Jet}/E_{Truth Jet});",100, 0, 2.0);
  TH1D* h1D_JER = new TH1D("h1D_JER",";JER (=(E_{Tower Jet}-E_{Truth Jet})/E_{Tower Jet});",100, -1.0, 1.0);
  TH2D* h2D_JES_eta = new TH2D("h2D_JES_eta",";#eta_{Truth Jet};JES (=E_{Tower Jet}/E_{Truth Jet})",100, -0.6, 0.6, 100, 0, 2.0);
  TH2D* h2D_JER_eta = new TH2D("h2D_JER_eta",";#eta_{Truth Jet};JER (=(E_{Tower Jet}-E_{Truth Jet})/E_{Tower Jet})",100, -0.6, 0.6, 100, -1.0, 1.0);
  TH1D* h1D_modi_JES = new TH1D("h1D_modi_JES",";JES (=E_{Tower Jet}/E_{Truth Jet});",100, 0, 2.0);
  TH1D* h1D_modi_JER = new TH1D("h1D_modi_JER",";JER (=(E_{Tower Jet}-E_{Truth Jet})/E_{Tower Jet});",100, -1.0, 1.0);
  TH2D* h2D_modi_JES_eta = new TH2D("h2D_modi_JES_eta",";#eta_{Truth Jet};JES (=E_{Tower Jet}/E_{Truth Jet})",100, -0.6, 0.6, 100, 0, 2.0);
  TH2D* h2D_modi_JER_eta = new TH2D("h2D_modi_JER_eta",";#eta_{Truth Jet};JER (=(E_{Tower Jet}-E_{Truth Jet})/E_{Tower Jet})",100, -0.6, 0.6, 100, -1.0, 1.0);
  TH2D* h2D_JES_emfrac = new TH2D("h2D_JES_emfrac",";Truth Jet EM fraction;JES (=E_{Tower Jet}/E_{Truth Jet})",100, 0., 1., 100, 0, 2.0);
  TH2D* h2D_JER_emfrac = new TH2D("h2D_JER_emfrac",";Truth Jet EM fraction;JER (=(E_{Tower Jet}-E_{Truth Jet})/E_{Tower Jet})",100, 0., 1., 100, -1.0, 1.0);
  TH2D* h2D_JES_calfrac = new TH2D("h2D_JES_calfrac",";(E_{HCAL}-E_{EMCAL})/(E_{HCAL}+E_{EMCAL});JES (=E_{Tower Jet}/E_{Truth Jet})",100, -1., 1., 100, 0, 2.0);
  TH2D* h2D_modi_JES_emfrac = new TH2D("h2D_modi_JES_emfrac",";Truth Jet EM fraction;JES (=E_{Tower Jet}/E_{Truth Jet})",100, 0., 1., 100, 0, 2.0);
  TH2D* h2D_modi_JER_emfrac = new TH2D("h2D_modi_JER_emfrac",";Truth Jet EM fraction;JER (=(E_{Tower Jet}-E_{Truth Jet})/E_{Tower Jet})",100, 0., 1., 100, -1.0, 1.0);
  TH2D* h2D_modi_JES_calfrac = new TH2D("h2D_modi_JES_calfrac",";(E_{HCAL}-E_{EMCAL})/(E_{HCAL}+E_{EMCAL});JES (=E_{Tower Jet}/E_{Truth Jet})",100, -1., 1., 100, 0, 2.0);
  ////////////////////////////////////////////////////////
  TH1D* h1D_quark_JES = new TH1D("h1D_quark_JES",";JES (=E_{Tower Jet}/E_{Truth Jet});",100, 0, 2.0);
  TH1D* h1D_quark_JER = new TH1D("h1D_quark_JER",";JER (=(E_{Tower Jet}-E_{Truth Jet})/E_{Tower Jet});",100, -1.0, 1.0);
  TH2D* h2D_quark_JES_eta = new TH2D("h2D_quark_JES_eta",";#eta_{Truth Jet};JES (=E_{Tower Jet}/E_{Truth Jet})",100, -0.6, 0.6, 100, 0, 2.0);
  TH2D* h2D_quark_JER_eta = new TH2D("h2D_quark_JER_eta",";#eta_{Truth Jet};JER (=(E_{Tower Jet}-E_{Truth Jet})/E_{Tower Jet})",100, -0.6, 0.6, 100, -1.0, 1.0);
  TH1D* h1D_quark_modi_JES = new TH1D("h1D_quark_modi_JES",";JES (=E_{Tower Jet}/E_{Truth Jet});",100, 0, 2.0);
  TH1D* h1D_quark_modi_JER = new TH1D("h1D_quark_modi_JER",";JER (=(E_{Tower Jet}-E_{Truth Jet})/E_{Tower Jet});",100, -1.0, 1.0);
  TH2D* h2D_quark_modi_JES_eta = new TH2D("h2D_quark_modi_JES_eta",";#eta_{Truth Jet};JES (=E_{Tower Jet}/E_{Truth Jet})",100, -0.6, 0.6, 100, 0, 2.0);
  TH2D* h2D_quark_modi_JER_eta = new TH2D("h2D_quark_modi_JER_eta",";#eta_{Truth Jet};JER (=(E_{Tower Jet}-E_{Truth Jet})/E_{Tower Jet})",100, -0.6, 0.6, 100, -1.0, 1.0);
  TH2D* h2D_quark_JES_emfrac = new TH2D("h2D_quark_JES_emfrac",";Truth Jet EM fraction;JES (=E_{Tower Jet}/E_{Truth Jet})",100, 0., 1., 100, 0, 2.0);
  TH2D* h2D_quark_JER_emfrac = new TH2D("h2D_quark_JER_emfrac",";Truth Jet EM fraction;JER (=(E_{Tower Jet}-E_{Truth Jet})/E_{Tower Jet})",100, 0., 1., 100, -1.0, 1.0);
  TH2D* h2D_quark_JES_calfrac = new TH2D("h2D_quark_JES_calfrac",";(E_{HCAL}-E_{EMCAL})/(E_{HCAL}+E_{EMCAL});JES (=E_{Tower Jet}/E_{Truth Jet})",100, -1., 1., 100, 0, 2.0);
  TH2D* h2D_quark_modi_JES_emfrac = new TH2D("h2D_quark_modi_JES_emfrac",";Truth Jet EM fraction;JES (=E_{Tower Jet}/E_{Truth Jet})",100, 0., 1., 100, 0, 2.0);
  TH2D* h2D_quark_modi_JER_emfrac = new TH2D("h2D_quark_modi_JER_emfrac",";Truth Jet EM fraction;JER (=(E_{Tower Jet}-E_{Truth Jet})/E_{Tower Jet})",100, 0., 1., 100, -1.0, 1.0);
  TH2D* h2D_quark_modi_JES_calfrac = new TH2D("h2D_quark_modi_JES_calfrac",";(E_{HCAL}-E_{EMCAL})/(E_{HCAL}+E_{EMCAL});JES (=E_{Tower Jet}/E_{Truth Jet})",100, -1., 1., 100, 0, 2.0);
  ////////////////////////////////////////////////////////
  TH1D* h1D_gluon_JES = new TH1D("h1D_gluon_JES",";JES (=E_{Tower Jet}/E_{Truth Jet});",100, 0, 2.0);
  TH1D* h1D_gluon_JER = new TH1D("h1D_gluon_JER",";JER (=(E_{Tower Jet}-E_{Truth Jet})/E_{Tower Jet});",100, -1.0, 1.0);
  TH2D* h2D_gluon_JES_eta = new TH2D("h2D_gluon_JES_eta",";#eta_{Truth Jet};JES (=E_{Tower Jet}/E_{Truth Jet})",100, -0.6, 0.6, 100, 0, 2.0);
  TH2D* h2D_gluon_JER_eta = new TH2D("h2D_gluon_JER_eta",";#eta_{Truth Jet};JER (=(E_{Tower Jet}-E_{Truth Jet})/E_{Tower Jet})",100, -0.6, 0.6, 100, -1.0, 1.0);
  TH1D* h1D_gluon_modi_JES = new TH1D("h1D_gluon_modi_JES",";JES (=E_{Tower Jet}/E_{Truth Jet});",100, 0, 2.0);
  TH1D* h1D_gluon_modi_JER = new TH1D("h1D_gluon_modi_JER",";JER (=(E_{Tower Jet}-E_{Truth Jet})/E_{Tower Jet});",100, -1.0, 1.0);
  TH2D* h2D_gluon_modi_JES_eta = new TH2D("h2D_gluon_modi_JES_eta",";#eta_{Truth Jet};JES (=E_{Tower Jet}/E_{Truth Jet})",100, -0.6, 0.6, 100, 0, 2.0);
  TH2D* h2D_gluon_modi_JER_eta = new TH2D("h2D_gluon_modi_JER_eta",";#eta_{Truth Jet};JER (=(E_{Tower Jet}-E_{Truth Jet})/E_{Tower Jet})",100, -0.6, 0.6, 100, -1.0, 1.0);
  TH2D* h2D_gluon_JES_emfrac = new TH2D("h2D_gluon_JES_emfrac",";Truth Jet EM fraction;JES (=E_{Tower Jet}/E_{Truth Jet})",100, 0., 1., 100, 0, 2.0);
  TH2D* h2D_gluon_JER_emfrac = new TH2D("h2D_gluon_JER_emfrac",";Truth Jet EM fraction;JER (=(E_{Tower Jet}-E_{Truth Jet})/E_{Tower Jet})",100, 0., 1., 100, -1.0, 1.0);
  TH2D* h2D_gluon_JES_calfrac = new TH2D("h2D_gluon_JES_calfrac",";(E_{HCAL}-E_{EMCAL})/(E_{HCAL}+E_{EMCAL});JES (=E_{Tower Jet}/E_{Truth Jet})",100, -1., 1., 100, 0, 2.0);
  TH2D* h2D_gluon_modi_JES_emfrac = new TH2D("h2D_gluon_modi_JES_emfrac",";Truth Jet EM fraction;JES (=E_{Tower Jet}/E_{Truth Jet})",100, 0., 1., 100, 0, 2.0);
  TH2D* h2D_gluon_modi_JER_emfrac = new TH2D("h2D_gluon_modi_JER_emfrac",";Truth Jet EM fraction;JER (=(E_{Tower Jet}-E_{Truth Jet})/E_{Tower Jet})",100, 0., 1., 100, -1.0, 1.0);
  TH2D* h2D_gluon_modi_JES_calfrac = new TH2D("h2D_gluon_modi_JES_calfrac",";(E_{HCAL}-E_{EMCAL})/(E_{HCAL}+E_{EMCAL});JES (=E_{Tower Jet}/E_{Truth Jet})",100, -1., 1., 100, 0, 2.0);
  ////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////
  float nprimbin=41, primbinmin=-0.5, primbinmax=40.5;
  //// nconstitut - primary jets 
  TH1D *hjet_p_nconstit = new TH1D("hjet_p_nconstit" , "Number of primary constituents", nprimbin, primbinmin, primbinmax);
  TH1D *hjet_p_nconstit_quark = new TH1D("hjet_p_nconstit_quark" , "Number of primary constituents", nprimbin, primbinmin, primbinmax);
  TH1D *hjet_p_nconstit_gluon = new TH1D("hjet_p_nconstit_gluon" , "Number of primary constituents", nprimbin, primbinmin, primbinmax);
  TH1D *hjet_p_nconstit_else = new TH1D("hjet_p_nconstit_else" , "Number of primary constituents", nprimbin, primbinmin, primbinmax);
  hjet_p_nconstit->Sumw2();
  hjet_p_nconstit_quark->Sumw2();
  hjet_p_nconstit_gluon->Sumw2();
  hjet_p_nconstit_else->Sumw2();
  //float ntwrbin=131, twrbinmin=-0.5, twrbinmax=130.5;
  float ntwrbin=251, twrbinmin=-0.5, twrbinmax=250.5;
  TH1D *hjet_twr_nconstit = new TH1D("hjet_twr_nconstit" , "Number of tower constituents", ntwrbin, twrbinmin, twrbinmax);
  TH1D *hjet_twr_nconstit_quark = new TH1D("hjet_twr_nconstit_quark" , "Number of tower constituents", ntwrbin, twrbinmin, twrbinmax);
  TH1D *hjet_twr_nconstit_gluon = new TH1D("hjet_twr_nconstit_gluon" , "Number of tower constituents", ntwrbin, twrbinmin, twrbinmax);
  TH1D *hjet_twr_nconstit_else = new TH1D("hjet_twr_nconstit_else" , "Number of tower constituents", ntwrbin, twrbinmin, twrbinmax);
  hjet_twr_nconstit->Sumw2();
  hjet_twr_nconstit_quark->Sumw2();
  hjet_twr_nconstit_gluon->Sumw2();
  hjet_twr_nconstit_else->Sumw2();

  ////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////
  //// loop over files 
  for(int j=initfile; j<endfile; j++){
    
    string fname;
    if (jetE.compare("20GeV")==0) { fname= Form("./out_QCDjet_%s_%s/QCD15GeV_R04jet20GeVeta0p6_G4sPHENIX_%s_%d.root",jetE.c_str(),ihcalType.c_str(),ihcalType.c_str(),j);}
    else if (jetE.compare("30GeV")==0) { fname= Form("./out_QCDjet_%s_%s/QCD25GeV_R04jet30GeVeta0p6_G4sPHENIX_%s_%d.root",jetE.c_str(),ihcalType.c_str(),ihcalType.c_str(),j);}
    else if (jetE.compare("40GeV")==0) { fname= Form("./out_QCDjet_%s_%s/QCD30GeV_R04jet40GeVeta0p6_G4sPHENIX_%s_%d.root",jetE.c_str(),ihcalType.c_str(),ihcalType.c_str(),j);}
    else if (jetE.compare("50GeV")==0) { fname= Form("./out_QCDjet_%s_%s/DVP_QCD35_10k_G4sPHENIX_%s_%d.root",jetE.c_str(),ihcalType.c_str(),ihcalType.c_str(),j);}
    if (j%50==0) cout << fname << endl; 

    //// skip bad files
    TFile* fin = new TFile(fname.c_str()); 
    if(fin->GetSize()<10000){fin->Close(); bad_files++; continue;}
    if(fin->IsZombie()) {fin->Close(); bad_files++; continue;}
    
    TTree* maintree = (TTree*)fin->Get("truthjet4_tree");
    TTree* friendtree = (TTree*)fin->Get("towerjet4_tree");

    if (bad_files >0) cout << "Number of bad files = " << bad_files << endl; 
    
    ////////////////////////////////////////////////////////
    int truthevent; 
    float truthjet4_e, truthjet4_p;
    float truthjet4_pt, truthjet4_eta, truthjet4_phi;
    maintree->SetBranchAddress("event",&truthevent); 
    maintree->SetBranchAddress("truthjet4_e",&truthjet4_e); 
    maintree->SetBranchAddress("truthjet4_p",&truthjet4_p); 
    maintree->SetBranchAddress("truthjet4_pt",&truthjet4_pt); 
    maintree->SetBranchAddress("truthjet4_eta",&truthjet4_eta); 
    maintree->SetBranchAddress("truthjet4_phi",&truthjet4_phi); 
    int truthjet4_cons_n;
    float truthjet4_cons_e[999], truthjet4_cons_p[999];
    float truthjet4_cons_pt[999], truthjet4_cons_eta[999], truthjet4_cons_phi[999];
    float truthjet4_cons_dR[999];
//    float truthjet4_cons_pid[999];  // Al,, KYO: to be modified!!!!!
    int truthjet4_cons_pid[999]; //SS310
    maintree->SetBranchAddress("truthjet4_cons_n",&truthjet4_cons_n); 
    maintree->SetBranchAddress("truthjet4_cons_e",&truthjet4_cons_e); 
    maintree->SetBranchAddress("truthjet4_cons_p",&truthjet4_cons_p); 
    maintree->SetBranchAddress("truthjet4_cons_pt",&truthjet4_cons_pt); 
    maintree->SetBranchAddress("truthjet4_cons_eta",&truthjet4_cons_eta); 
    maintree->SetBranchAddress("truthjet4_cons_phi",&truthjet4_cons_phi); 
    maintree->SetBranchAddress("truthjet4_cons_dR",&truthjet4_cons_dR); 
    maintree->SetBranchAddress("truthjet4_cons_pid",&truthjet4_cons_pid); 
    int truthjet4_pg_n;
    int truthjet4_pg_id[999];
    float truthjet4_pg_fract[999];
    int truthjet4_pg_status[999];
    maintree->SetBranchAddress("truthjet4_pg_n",&truthjet4_pg_n); 
    maintree->SetBranchAddress("truthjet4_pg_id",&truthjet4_pg_id); 
    maintree->SetBranchAddress("truthjet4_pg_fract",&truthjet4_pg_fract); 
    maintree->SetBranchAddress("truthjet4_pg_status",&truthjet4_pg_status); 
    
    int towerevent; 
    float towerjet4_e, towerjet4_p;
    float towerjet4_pt, towerjet4_eta, towerjet4_phi;
    friendtree->SetBranchAddress("event",&towerevent); 
    friendtree->SetBranchAddress("towerjet4_e",&towerjet4_e); 
    friendtree->SetBranchAddress("towerjet4_p",&towerjet4_p); 
    friendtree->SetBranchAddress("towerjet4_pt",&towerjet4_pt); 
    friendtree->SetBranchAddress("towerjet4_eta",&towerjet4_eta); 
    friendtree->SetBranchAddress("towerjet4_phi",&towerjet4_phi); 
    
    int towerjet4_cemc_n;
    float towerjet4_cemc_scale;
    float towerjet4_cemc_e[999], towerjet4_cemc_p[999];
    float towerjet4_cemc_pt[999], towerjet4_cemc_eta[999], towerjet4_cemc_phi[999];
    float towerjet4_cemc_dR[999];
    friendtree->SetBranchAddress("towerjet4_cemc_n",&towerjet4_cemc_n); 
    friendtree->SetBranchAddress("towerjet4_cemc_scale",&towerjet4_cemc_scale); 
    friendtree->SetBranchAddress("towerjet4_cemc_e",&towerjet4_cemc_e); 
    friendtree->SetBranchAddress("towerjet4_cemc_p",&towerjet4_cemc_p); 
    friendtree->SetBranchAddress("towerjet4_cemc_pt",&towerjet4_cemc_pt); 
    friendtree->SetBranchAddress("towerjet4_cemc_eta",&towerjet4_cemc_eta); 
    friendtree->SetBranchAddress("towerjet4_cemc_phi",&towerjet4_cemc_phi); 
    friendtree->SetBranchAddress("towerjet4_cemc_dR",&towerjet4_cemc_dR); 
    
    int towerjet4_ihcal_n;
    float towerjet4_ihcal_scale;
    float towerjet4_ihcal_e[999], towerjet4_ihcal_p[999];
    float towerjet4_ihcal_pt[999], towerjet4_ihcal_eta[999], towerjet4_ihcal_phi[999];
    float towerjet4_ihcal_dR[999];
    friendtree->SetBranchAddress("towerjet4_ihcal_n",&towerjet4_ihcal_n); 
    friendtree->SetBranchAddress("towerjet4_ihcal_scale",&towerjet4_ihcal_scale); 
    friendtree->SetBranchAddress("towerjet4_ihcal_e",&towerjet4_ihcal_e); 
    friendtree->SetBranchAddress("towerjet4_ihcal_p",&towerjet4_ihcal_p); 
    friendtree->SetBranchAddress("towerjet4_ihcal_pt",&towerjet4_ihcal_pt); 
    friendtree->SetBranchAddress("towerjet4_ihcal_eta",&towerjet4_ihcal_eta); 
    friendtree->SetBranchAddress("towerjet4_ihcal_phi",&towerjet4_ihcal_phi); 
    friendtree->SetBranchAddress("towerjet4_ihcal_dR",&towerjet4_ihcal_dR); 
    
    int towerjet4_ohcal_n;
    float towerjet4_ohcal_scale;
    float towerjet4_ohcal_e[999], towerjet4_ohcal_p[999];
    float towerjet4_ohcal_pt[999], towerjet4_ohcal_eta[999], towerjet4_ohcal_phi[999];
    float towerjet4_ohcal_dR[999];
    friendtree->SetBranchAddress("towerjet4_ohcal_n",&towerjet4_ohcal_n); 
    friendtree->SetBranchAddress("towerjet4_ohcal_scale",&towerjet4_ohcal_scale); 
    friendtree->SetBranchAddress("towerjet4_ohcal_e",&towerjet4_ohcal_e); 
    friendtree->SetBranchAddress("towerjet4_ohcal_p",&towerjet4_ohcal_p); 
    friendtree->SetBranchAddress("towerjet4_ohcal_pt",&towerjet4_ohcal_pt); 
    friendtree->SetBranchAddress("towerjet4_ohcal_eta",&towerjet4_ohcal_eta); 
    friendtree->SetBranchAddress("towerjet4_ohcal_phi",&towerjet4_ohcal_phi); 
    friendtree->SetBranchAddress("towerjet4_ohcal_dR",&towerjet4_ohcal_dR); 
 
    ////////////////////////////////////////////////////////
    //// 1) Fill the basic truth jet info
    for (int itrue=0; itrue<maintree->GetEntries(); itrue++){
      maintree->GetEntry(itrue);
		  h1D_truthjet4_e->Fill(truthjet4_e);
		  h1D_truthjet4_p->Fill(truthjet4_p);
		  h1D_truthjet4_pt->Fill(truthjet4_pt);
		  h1D_truthjet4_eta->Fill(truthjet4_eta);
		  h1D_truthjet4_phi->Fill(truthjet4_phi);
		  h1D_truthjet4_cons_n->Fill(truthjet4_cons_n);
      for (int icons =0; icons < truthjet4_cons_n; icons ++) {
        h1D_truthjet4_cons_e->Fill(truthjet4_cons_e[icons]);
        h1D_truthjet4_cons_p->Fill(truthjet4_cons_p[icons]);
        h1D_truthjet4_cons_pt->Fill(truthjet4_cons_pt[icons]);
        h1D_truthjet4_cons_eta->Fill(truthjet4_cons_eta[icons]);
        h1D_truthjet4_cons_phi->Fill(truthjet4_cons_phi[icons]);
        h1D_truthjet4_cons_dR->Fill(truthjet4_cons_dR[icons]);
        h1D_truthjet4_cons_pid->Fill(truthjet4_cons_pid[icons]);
		  } 
		  h1D_truthjet4_pg_n->Fill(truthjet4_pg_n);
      for (int ipg =0; ipg < truthjet4_pg_n; ipg ++) {
        h1D_truthjet4_pg_id->Fill(truthjet4_pg_id[ipg]);
        h1D_truthjet4_pg_fract->Fill(truthjet4_pg_fract[ipg]);
        h1D_truthjet4_pg_status->Fill(truthjet4_pg_status[ipg]);
		  } 
    }
    ////////////////////////////////////////////////////////
    //// 2) Fill the basic tower jet info
    for (int itwr=0; itwr<friendtree->GetEntries(); itwr++){
      friendtree->GetEntry(itwr);
		  h1D_towerjet4_e->Fill(towerjet4_e);
		  h1D_towerjet4_p->Fill(towerjet4_p);
		  h1D_towerjet4_pt->Fill(towerjet4_pt);
		  h1D_towerjet4_eta->Fill(towerjet4_eta);
		  h1D_towerjet4_phi->Fill(towerjet4_phi);
		  h1D_towerjet4_cemc_n->Fill(towerjet4_cemc_n);
      for (int icemc =0; icemc < towerjet4_cemc_n; icemc ++) {
        h1D_towerjet4_cemc_e->Fill(towerjet4_cemc_e[icemc]);
        h1D_towerjet4_cemc_p->Fill(towerjet4_cemc_p[icemc]);
        h1D_towerjet4_cemc_pt->Fill(towerjet4_cemc_pt[icemc]);
        h1D_towerjet4_cemc_eta->Fill(towerjet4_cemc_eta[icemc]);
        h1D_towerjet4_cemc_phi->Fill(towerjet4_cemc_phi[icemc]);
        h1D_towerjet4_cemc_dR->Fill(towerjet4_cemc_dR[icemc]);
        //tower_cemcsum+=towerjet4_cemc_e[icemc]; 
		  } 
		  h1D_towerjet4_ihcal_n->Fill(towerjet4_ihcal_n);
      for (int iihcal =0; iihcal < towerjet4_ihcal_n; iihcal ++) {
        h1D_towerjet4_ihcal_e->Fill(towerjet4_ihcal_e[iihcal]);
        h1D_towerjet4_ihcal_p->Fill(towerjet4_ihcal_p[iihcal]);
        h1D_towerjet4_ihcal_pt->Fill(towerjet4_ihcal_pt[iihcal]);
        h1D_towerjet4_ihcal_eta->Fill(towerjet4_ihcal_eta[iihcal]);
        h1D_towerjet4_ihcal_phi->Fill(towerjet4_ihcal_phi[iihcal]);
        h1D_towerjet4_ihcal_dR->Fill(towerjet4_ihcal_dR[iihcal]);
        //tower_ihcalsum+=towerjet4_ihcal_e[iihcal]; 
		  } 
		  h1D_towerjet4_ohcal_n->Fill(towerjet4_ohcal_n);
      for (int iohcal =0; iohcal < towerjet4_ohcal_n; iohcal ++) {
        h1D_towerjet4_ohcal_e->Fill(towerjet4_ohcal_e[iohcal]);
        h1D_towerjet4_ohcal_p->Fill(towerjet4_ohcal_p[iohcal]);
        h1D_towerjet4_ohcal_pt->Fill(towerjet4_ohcal_pt[iohcal]);
        h1D_towerjet4_ohcal_eta->Fill(towerjet4_ohcal_eta[iohcal]);
        h1D_towerjet4_ohcal_phi->Fill(towerjet4_ohcal_phi[iohcal]);
        h1D_towerjet4_ohcal_dR->Fill(towerjet4_ohcal_dR[iohcal]);
        //tower_ohcalsum+=towerjet4_ohcal_e[iohcal]; 
		  } 
    }
    ////////////////////////////////////////////////////////
    //// 3) Fill need-more-calculation info 
    
    //// loop over truth jet entries
    for (int itrue=0; itrue<maintree->GetEntries(); itrue++){
    //for (int itrue=0; itrue<6; itrue++){

      maintree->GetEntry(itrue);
      //if (itrue%1000==0) cout << itrue << endl; 
      
      if (jetE.compare("20GeV")==0) { if (!(truthjet4_e > 20 && truthjet4_e < 30)) continue;}
      else if (jetE.compare("30GeV")==0) { if (!(truthjet4_e > 30 && truthjet4_e < 40)) continue;}
      else if (jetE.compare("40GeV")==0) { if (!(truthjet4_e > 40 && truthjet4_e < 50)) continue;}
      else if (jetE.compare("50GeV")==0) { if (!(truthjet4_e > 50)) continue;}
      if (fabs(truthjet4_eta) > 0.6) continue;
       
      //// matching tower jets (smallest dR) to get JES
      float jes_ratio = 999; //finally needed
      float modi_jes_ratio = 999; //finally needed
      float jer_ratio = 999; //finally needed
      float modi_jer_ratio = 999; //finally needed
      float match_dR = 999;
      float tmp_dR = 999;
      int match_idx = 999;
      for (int itwr=0; itwr<friendtree->GetEntries(); itwr++){
        friendtree->GetEntry(itwr);
        if (truthevent!=towerevent) { continue; } // only in the same event
        tmp_dR = sqrt(pow(truthjet4_eta-towerjet4_eta,2)+ pow(truthjet4_phi-towerjet4_phi,2));
        if (tmp_dR < match_dR){ 
          match_dR = tmp_dR;
          match_idx = itwr;
        }
      }
      //cout << "match_dR = " << match_dR << endl; 
      if (match_dR > 0.4) continue; 
      //if (match_dR > 0.3) continue; 
      
      //// BEST MATCHED tower jet!! 
      friendtree->GetEntry(match_idx);

      jes_ratio = towerjet4_e/truthjet4_e;
      jer_ratio = (towerjet4_e-truthjet4_e)/towerjet4_e;
      //cout << "jes_ratio = " << jes_ratio << endl; 
    
      //// quark vs. gluon jet
      bool is_quark = false; 
      bool is_gluon = false;
    
      //// Progenitors are already ordered in terms of fraction?!
      if(truthjet4_pg_n>0){
        if(truthjet4_pg_fract[0]>0.5){
          if( (truthjet4_pg_id[0]==1) || (truthjet4_pg_id[0]==2) ) 
	          is_quark = true; 
	        else if (truthjet4_pg_id[0]==21) 
	          is_gluon = true; 
          }
      }
      hjet_p_nconstit->Fill(truthjet4_cons_n);
      if(is_quark){ hjet_p_nconstit_quark->Fill(truthjet4_cons_n);}
      else if(is_gluon){ hjet_p_nconstit_gluon->Fill(truthjet4_cons_n);}
      else { hjet_p_nconstit_else->Fill(truthjet4_cons_n); } 
      
      int jet_twr_nconstit=towerjet4_cemc_n+towerjet4_ihcal_n+towerjet4_ohcal_n;
      hjet_twr_nconstit->Fill(jet_twr_nconstit);
      if(is_quark){ hjet_twr_nconstit_quark->Fill(jet_twr_nconstit);}
      else if(is_gluon){ hjet_twr_nconstit_gluon->Fill(jet_twr_nconstit);}
      else { hjet_twr_nconstit_else->Fill(jet_twr_nconstit); } 
      
      //// basic info for quark jet and gluon jet
		  if (is_quark){
        h1D_truthjet4_quark_e->Fill(truthjet4_e);
		    h1D_truthjet4_quark_p->Fill(truthjet4_p);
		    h1D_truthjet4_quark_pt->Fill(truthjet4_pt);
  		  h1D_truthjet4_quark_eta->Fill(truthjet4_eta);
  		  h1D_truthjet4_quark_phi->Fill(truthjet4_phi);
  		  h1D_truthjet4_quark_cons_n->Fill(truthjet4_cons_n);
       for (int icons =0; icons < truthjet4_cons_n; icons ++) {
         h1D_truthjet4_quark_cons_e->Fill(truthjet4_cons_e[icons]);
         h1D_truthjet4_quark_cons_p->Fill(truthjet4_cons_p[icons]);
         h1D_truthjet4_quark_cons_pt->Fill(truthjet4_cons_pt[icons]);
         h1D_truthjet4_quark_cons_eta->Fill(truthjet4_cons_eta[icons]);
         h1D_truthjet4_quark_cons_phi->Fill(truthjet4_cons_phi[icons]);
         h1D_truthjet4_quark_cons_dR->Fill(truthjet4_cons_dR[icons]);
         h1D_truthjet4_quark_cons_pid->Fill(truthjet4_cons_pid[icons]);
  		  } 
  		  h1D_truthjet4_quark_pg_n->Fill(truthjet4_pg_n);
       for (int ipg =0; ipg < truthjet4_pg_n; ipg ++) {
         h1D_truthjet4_quark_pg_id->Fill(truthjet4_pg_id[ipg]);
         h1D_truthjet4_quark_pg_fract->Fill(truthjet4_pg_fract[ipg]);
         h1D_truthjet4_quark_pg_status->Fill(truthjet4_pg_status[ipg]);
  		  } 
  		  h1D_towerjet4_quark_e->Fill(towerjet4_e);
  		  h1D_towerjet4_quark_p->Fill(towerjet4_p);
  		  h1D_towerjet4_quark_pt->Fill(towerjet4_pt);
  		  h1D_towerjet4_quark_eta->Fill(towerjet4_eta);
  		  h1D_towerjet4_quark_phi->Fill(towerjet4_phi);
  		  h1D_towerjet4_quark_cemc_n->Fill(towerjet4_cemc_n);
       for (int icemc =0; icemc < towerjet4_cemc_n; icemc ++) {
         h1D_towerjet4_quark_cemc_e->Fill(towerjet4_cemc_e[icemc]);
         h1D_towerjet4_quark_cemc_p->Fill(towerjet4_cemc_p[icemc]);
         h1D_towerjet4_quark_cemc_pt->Fill(towerjet4_cemc_pt[icemc]);
         h1D_towerjet4_quark_cemc_eta->Fill(towerjet4_cemc_eta[icemc]);
         h1D_towerjet4_quark_cemc_phi->Fill(towerjet4_cemc_phi[icemc]);
         h1D_towerjet4_quark_cemc_dR->Fill(towerjet4_cemc_dR[icemc]);
         //tower_cemcsum+=towerjet4_quark_cemc_e[icemc]; 
  		  } 
  		  h1D_towerjet4_quark_ihcal_n->Fill(towerjet4_ihcal_n);
       for (int iihcal =0; iihcal < towerjet4_ihcal_n; iihcal ++) {
         h1D_towerjet4_quark_ihcal_e->Fill(towerjet4_ihcal_e[iihcal]);
         h1D_towerjet4_quark_ihcal_p->Fill(towerjet4_ihcal_p[iihcal]);
         h1D_towerjet4_quark_ihcal_pt->Fill(towerjet4_ihcal_pt[iihcal]);
         h1D_towerjet4_quark_ihcal_eta->Fill(towerjet4_ihcal_eta[iihcal]);
         h1D_towerjet4_quark_ihcal_phi->Fill(towerjet4_ihcal_phi[iihcal]);
         h1D_towerjet4_quark_ihcal_dR->Fill(towerjet4_ihcal_dR[iihcal]);
         //tower_ihcalsum+=towerjet4_quark_ihcal_e[iihcal]; 
  		  } 
  		  h1D_towerjet4_quark_ohcal_n->Fill(towerjet4_ohcal_n);
       for (int iohcal =0; iohcal < towerjet4_ohcal_n; iohcal ++) {
         h1D_towerjet4_quark_ohcal_e->Fill(towerjet4_ohcal_e[iohcal]);
         h1D_towerjet4_quark_ohcal_p->Fill(towerjet4_ohcal_p[iohcal]);
         h1D_towerjet4_quark_ohcal_pt->Fill(towerjet4_ohcal_pt[iohcal]);
         h1D_towerjet4_quark_ohcal_eta->Fill(towerjet4_ohcal_eta[iohcal]);
         h1D_towerjet4_quark_ohcal_phi->Fill(towerjet4_ohcal_phi[iohcal]);
         h1D_towerjet4_quark_ohcal_dR->Fill(towerjet4_ohcal_dR[iohcal]);
         //tower_ohcalsum+=towerjet4_quark_ohcal_e[iohcal]; 
  		  } 
      }
		  if (is_gluon){
        h1D_truthjet4_gluon_e->Fill(truthjet4_e);
		    h1D_truthjet4_gluon_p->Fill(truthjet4_p);
		    h1D_truthjet4_gluon_pt->Fill(truthjet4_pt);
  		  h1D_truthjet4_gluon_eta->Fill(truthjet4_eta);
  		  h1D_truthjet4_gluon_phi->Fill(truthjet4_phi);
  		  h1D_truthjet4_gluon_cons_n->Fill(truthjet4_cons_n);
       for (int icons =0; icons < truthjet4_cons_n; icons ++) {
         h1D_truthjet4_gluon_cons_e->Fill(truthjet4_cons_e[icons]);
         h1D_truthjet4_gluon_cons_p->Fill(truthjet4_cons_p[icons]);
         h1D_truthjet4_gluon_cons_pt->Fill(truthjet4_cons_pt[icons]);
         h1D_truthjet4_gluon_cons_eta->Fill(truthjet4_cons_eta[icons]);
         h1D_truthjet4_gluon_cons_phi->Fill(truthjet4_cons_phi[icons]);
         h1D_truthjet4_gluon_cons_dR->Fill(truthjet4_cons_dR[icons]);
         h1D_truthjet4_gluon_cons_pid->Fill(truthjet4_cons_pid[icons]);
  		  } 
  		  h1D_truthjet4_gluon_pg_n->Fill(truthjet4_pg_n);
       for (int ipg =0; ipg < truthjet4_pg_n; ipg ++) {
         h1D_truthjet4_gluon_pg_id->Fill(truthjet4_pg_id[ipg]);
         h1D_truthjet4_gluon_pg_fract->Fill(truthjet4_pg_fract[ipg]);
         h1D_truthjet4_gluon_pg_status->Fill(truthjet4_pg_status[ipg]);
  		  } 
  		  h1D_towerjet4_gluon_e->Fill(towerjet4_e);
  		  h1D_towerjet4_gluon_p->Fill(towerjet4_p);
  		  h1D_towerjet4_gluon_pt->Fill(towerjet4_pt);
  		  h1D_towerjet4_gluon_eta->Fill(towerjet4_eta);
  		  h1D_towerjet4_gluon_phi->Fill(towerjet4_phi);
  		  h1D_towerjet4_gluon_cemc_n->Fill(towerjet4_cemc_n);
       for (int icemc =0; icemc < towerjet4_cemc_n; icemc ++) {
         h1D_towerjet4_gluon_cemc_e->Fill(towerjet4_cemc_e[icemc]);
         h1D_towerjet4_gluon_cemc_p->Fill(towerjet4_cemc_p[icemc]);
         h1D_towerjet4_gluon_cemc_pt->Fill(towerjet4_cemc_pt[icemc]);
         h1D_towerjet4_gluon_cemc_eta->Fill(towerjet4_cemc_eta[icemc]);
         h1D_towerjet4_gluon_cemc_phi->Fill(towerjet4_cemc_phi[icemc]);
         h1D_towerjet4_gluon_cemc_dR->Fill(towerjet4_cemc_dR[icemc]);
         //tower_cemcsum+=towerjet4_gluon_cemc_e[icemc]; 
  		  } 
  		  h1D_towerjet4_gluon_ihcal_n->Fill(towerjet4_ihcal_n);
       for (int iihcal =0; iihcal < towerjet4_ihcal_n; iihcal ++) {
         h1D_towerjet4_gluon_ihcal_e->Fill(towerjet4_ihcal_e[iihcal]);
         h1D_towerjet4_gluon_ihcal_p->Fill(towerjet4_ihcal_p[iihcal]);
         h1D_towerjet4_gluon_ihcal_pt->Fill(towerjet4_ihcal_pt[iihcal]);
         h1D_towerjet4_gluon_ihcal_eta->Fill(towerjet4_ihcal_eta[iihcal]);
         h1D_towerjet4_gluon_ihcal_phi->Fill(towerjet4_ihcal_phi[iihcal]);
         h1D_towerjet4_gluon_ihcal_dR->Fill(towerjet4_ihcal_dR[iihcal]);
         //tower_ihcalsum+=towerjet4_gluon_ihcal_e[iihcal]; 
  		  } 
  		  h1D_towerjet4_gluon_ohcal_n->Fill(towerjet4_ohcal_n);
       for (int iohcal =0; iohcal < towerjet4_ohcal_n; iohcal ++) {
         h1D_towerjet4_gluon_ohcal_e->Fill(towerjet4_ohcal_e[iohcal]);
         h1D_towerjet4_gluon_ohcal_p->Fill(towerjet4_ohcal_p[iohcal]);
         h1D_towerjet4_gluon_ohcal_pt->Fill(towerjet4_ohcal_pt[iohcal]);
         h1D_towerjet4_gluon_ohcal_eta->Fill(towerjet4_ohcal_eta[iohcal]);
         h1D_towerjet4_gluon_ohcal_phi->Fill(towerjet4_ohcal_phi[iohcal]);
         h1D_towerjet4_gluon_ohcal_dR->Fill(towerjet4_ohcal_dR[iohcal]);
         //tower_ohcalsum+=towerjet4_gluon_ohcal_e[iohcal]; 
  		  } 
      }
       
      //// loop over tower constituents to get cal fraction 
      float cal_frac = 999; //finally needed
      float cemc_sum = 0;
      float ihcal_sum = 0;
      float ohcal_sum = 0;
      
      float modi_jet_e = 0;
      float modi_cal_frac = 999; //finally needed
      float modi_cemc_sum = 0;
      float modi_ihcal_sum = 0;
      float modi_ohcal_sum = 0;

      for (int icemc =0; icemc < towerjet4_cemc_n; icemc ++) {
        cemc_sum += towerjet4_cemc_e[icemc]; 
        //modi_cemc_sum += towerjet4_cemc_e[icemc]*cemc_sf; 
      } 
      for (int iihcal =0; iihcal < towerjet4_ihcal_n; iihcal ++) {
        ihcal_sum += towerjet4_ihcal_e[iihcal]; 
        //modi_ihcal_sum += towerjet4_ihcal_e[iihcal]*ihcal_sf; 
      } 
      for (int iohcal =0; iohcal < towerjet4_ohcal_n; iohcal ++) {
        ohcal_sum += towerjet4_ohcal_e[iohcal]; 
        //modi_ohcal_sum += towerjet4_ohcal_e[iohcal]*ohcal_sf; 
      } 
      modi_cemc_sum=cemc_sum*cemc_sf;
      modi_ihcal_sum=ihcal_sum*ihcal_sf; 
      modi_ohcal_sum=ohcal_sum*ohcal_sf; 
      cal_frac = (ihcal_sum+ohcal_sum-cemc_sum)/(ihcal_sum+ohcal_sum+cemc_sum);
      modi_cal_frac = (modi_ihcal_sum+modi_ohcal_sum-modi_cemc_sum)/(modi_ihcal_sum+modi_ohcal_sum+modi_cemc_sum);
      modi_jet_e = modi_cemc_sum + modi_ihcal_sum + modi_ohcal_sum; 
      modi_jes_ratio = modi_jet_e/truthjet4_e;
      modi_jer_ratio = (modi_jet_e-truthjet4_e)/modi_jet_e;
      //cout << "modi_jes_ratio = " << modi_jes_ratio << endl; 
      //cout << "cal_frac = " << cal_frac << endl;
      //cout << "modi_cal_frac = " << modi_cal_frac << endl;
       
      //// loop over truth constituents to get em fraction 
      float em_frac = 999; //finally needed 
      float em_den = 0;
      float em_num = 0;
      for (int icons =0; icons < truthjet4_cons_n; icons ++) {
        //// Denominator: All energy
        em_den += truthjet4_cons_e[icons]; 
        //// Numerator: EM only, using truth ptl inf ( e+-, gamma, pi0, eta) 
        if( (fabs(truthjet4_cons_pid[icons])==11) || (truthjet4_cons_pid[icons]==22) || 
        (truthjet4_cons_pid[icons]==111) || (truthjet4_cons_pid[icons]==221) ){
           em_num += truthjet4_cons_e[icons]; 
        } 
      } 
      em_frac = em_num/em_den;
      //cout << "em_frac = " << em_frac << endl;
      
      h1D_JES->Fill(jes_ratio); 
      h1D_JER->Fill(jer_ratio); 
      h2D_JES_eta->Fill(truthjet4_eta,jes_ratio); 
      h2D_JER_eta->Fill(truthjet4_eta,jer_ratio); 
      h1D_modi_JES->Fill(modi_jes_ratio); 
      h1D_modi_JER->Fill(modi_jer_ratio); 
      h2D_modi_JES_eta->Fill(truthjet4_eta,modi_jes_ratio); 
      h2D_modi_JER_eta->Fill(truthjet4_eta,modi_jer_ratio); 
      h2D_JES_calfrac->Fill(cal_frac,jes_ratio); 
      h2D_JES_emfrac->Fill(em_frac,jes_ratio); 
      h2D_JER_emfrac->Fill(em_frac,jer_ratio); 
      h2D_modi_JES_calfrac->Fill(modi_cal_frac,modi_jes_ratio); 
      h2D_modi_JES_emfrac->Fill(em_frac,modi_jes_ratio); 
      h2D_modi_JER_emfrac->Fill(em_frac,modi_jer_ratio); 
      if (is_quark){
        h1D_quark_JES->Fill(jes_ratio); 
        h1D_quark_JER->Fill(jer_ratio); 
        h2D_quark_JES_eta->Fill(truthjet4_eta,jes_ratio); 
        h2D_quark_JER_eta->Fill(truthjet4_eta,jer_ratio); 
        h1D_quark_modi_JES->Fill(modi_jes_ratio); 
        h1D_quark_modi_JER->Fill(modi_jer_ratio); 
        h2D_quark_modi_JES_eta->Fill(truthjet4_eta,modi_jes_ratio); 
        h2D_quark_modi_JER_eta->Fill(truthjet4_eta,modi_jer_ratio); 
        h2D_quark_JES_calfrac->Fill(cal_frac,jes_ratio); 
        h2D_quark_JES_emfrac->Fill(em_frac,jes_ratio); 
        h2D_quark_JER_emfrac->Fill(em_frac,jer_ratio); 
        h2D_quark_modi_JES_calfrac->Fill(modi_cal_frac,modi_jes_ratio); 
        h2D_quark_modi_JES_emfrac->Fill(em_frac,modi_jes_ratio); 
        h2D_quark_modi_JER_emfrac->Fill(em_frac,modi_jer_ratio); 
      }
      if (is_gluon){
        h1D_gluon_JES->Fill(jes_ratio); 
        h1D_gluon_JER->Fill(jer_ratio); 
        h2D_gluon_JES_eta->Fill(truthjet4_eta,jes_ratio); 
        h2D_gluon_JER_eta->Fill(truthjet4_eta,jer_ratio); 
        h1D_gluon_modi_JES->Fill(modi_jes_ratio); 
        h1D_gluon_modi_JER->Fill(modi_jer_ratio); 
        h2D_gluon_modi_JES_eta->Fill(truthjet4_eta,modi_jes_ratio); 
        h2D_gluon_modi_JER_eta->Fill(truthjet4_eta,modi_jer_ratio); 
        h2D_gluon_JES_calfrac->Fill(cal_frac,jes_ratio); 
        h2D_gluon_JES_emfrac->Fill(em_frac,jes_ratio); 
        h2D_gluon_JER_emfrac->Fill(em_frac,jer_ratio); 
        h2D_gluon_modi_JES_calfrac->Fill(modi_cal_frac,modi_jes_ratio); 
        h2D_gluon_modi_JES_emfrac->Fill(em_frac,modi_jes_ratio); 
        h2D_gluon_modi_JER_emfrac->Fill(em_frac,modi_jer_ratio); 
      }
    
    } // end of truth jet loop
  
    fin->Close(); 
  } // end of file loop

  //TCanvas* c1 = new TCanvas("c1","",600,600);
  //h2D_JES_emfrac->Draw();
  
  h2D_JES_eta->SetOption("colz");
  h2D_JER_eta->SetOption("colz");
  h2D_modi_JES_eta->SetOption("colz");
  h2D_modi_JER_eta->SetOption("colz");
  h2D_JES_calfrac->SetOption("colz");
  h2D_JES_emfrac->SetOption("colz");
  h2D_JER_emfrac->SetOption("colz");
  h2D_modi_JES_calfrac->SetOption("colz");
  h2D_modi_JES_emfrac->SetOption("colz");
  h2D_modi_JER_emfrac->SetOption("colz");
  h2D_quark_JES_eta->SetOption("colz");
  h2D_quark_JER_eta->SetOption("colz");
  h2D_quark_modi_JES_eta->SetOption("colz");
  h2D_quark_modi_JER_eta->SetOption("colz");
  h2D_quark_JES_calfrac->SetOption("colz");
  h2D_quark_JES_emfrac->SetOption("colz");
  h2D_quark_JER_emfrac->SetOption("colz");
  h2D_quark_modi_JES_calfrac->SetOption("colz");
  h2D_quark_modi_JES_emfrac->SetOption("colz");
  h2D_quark_modi_JER_emfrac->SetOption("colz");
  h2D_gluon_JES_eta->SetOption("colz");
  h2D_gluon_JER_eta->SetOption("colz");
  h2D_gluon_modi_JES_eta->SetOption("colz");
  h2D_gluon_modi_JER_eta->SetOption("colz");
  h2D_gluon_JES_calfrac->SetOption("colz");
  h2D_gluon_JES_emfrac->SetOption("colz");
  h2D_gluon_JER_emfrac->SetOption("colz");
  h2D_gluon_modi_JES_calfrac->SetOption("colz");
  h2D_gluon_modi_JES_emfrac->SetOption("colz");
  h2D_gluon_modi_JER_emfrac->SetOption("colz");

  TFile* fout = new TFile(Form("outJet4QG/%s_%s_ByGammaJet_%.0f_%.0f_%.0f.root",jetE.c_str(),ihcalType.c_str(),cemc_sf*100,ihcal_sf*100,ohcal_sf*100),"RECREATE");
  fout->cd();
  ////
  h1D_truthjet4_e->Write();
  h1D_truthjet4_p->Write();
  h1D_truthjet4_pt->Write();
  h1D_truthjet4_eta->Write();
  h1D_truthjet4_phi->Write();
  h1D_truthjet4_cons_n->Write();
  h1D_truthjet4_cons_e->Write();
  h1D_truthjet4_cons_p->Write();
  h1D_truthjet4_cons_pt->Write();
  h1D_truthjet4_cons_eta->Write();
  h1D_truthjet4_cons_phi->Write();
  h1D_truthjet4_cons_dR->Write();
  h1D_truthjet4_cons_pid->Write();
  h1D_truthjet4_pg_n->Write();
  h1D_truthjet4_pg_id->Write();
  h1D_truthjet4_pg_fract->Write();
  h1D_truthjet4_pg_status->Write();
  ////
  h1D_truthjet4_quark_e->Write();
  h1D_truthjet4_quark_p->Write();
  h1D_truthjet4_quark_pt->Write();
  h1D_truthjet4_quark_eta->Write();
  h1D_truthjet4_quark_phi->Write();
  h1D_truthjet4_quark_cons_n->Write();
  h1D_truthjet4_quark_cons_e->Write();
  h1D_truthjet4_quark_cons_p->Write();
  h1D_truthjet4_quark_cons_pt->Write();
  h1D_truthjet4_quark_cons_eta->Write();
  h1D_truthjet4_quark_cons_phi->Write();
  h1D_truthjet4_quark_cons_dR->Write();
  h1D_truthjet4_quark_cons_pid->Write();
  h1D_truthjet4_quark_pg_n->Write();
  h1D_truthjet4_quark_pg_id->Write();
  h1D_truthjet4_quark_pg_fract->Write();
  h1D_truthjet4_quark_pg_status->Write();
  ////
  h1D_truthjet4_gluon_e->Write();
  h1D_truthjet4_gluon_p->Write();
  h1D_truthjet4_gluon_pt->Write();
  h1D_truthjet4_gluon_eta->Write();
  h1D_truthjet4_gluon_phi->Write();
  h1D_truthjet4_gluon_cons_n->Write();
  h1D_truthjet4_gluon_cons_e->Write();
  h1D_truthjet4_gluon_cons_p->Write();
  h1D_truthjet4_gluon_cons_pt->Write();
  h1D_truthjet4_gluon_cons_eta->Write();
  h1D_truthjet4_gluon_cons_phi->Write();
  h1D_truthjet4_gluon_cons_dR->Write();
  h1D_truthjet4_gluon_cons_pid->Write();
  h1D_truthjet4_gluon_pg_n->Write();
  h1D_truthjet4_gluon_pg_id->Write();
  h1D_truthjet4_gluon_pg_fract->Write();
  h1D_truthjet4_gluon_pg_status->Write();
  //////// 
  //////// 
  h1D_towerjet4_e->Write();
  h1D_towerjet4_p->Write();
  h1D_towerjet4_pt->Write();
  h1D_towerjet4_eta->Write();
  h1D_towerjet4_phi->Write();
  h1D_towerjet4_cemc_n->Write();
  h1D_towerjet4_cemc_e->Write();
  h1D_towerjet4_cemc_p->Write();
  h1D_towerjet4_cemc_pt->Write();
  h1D_towerjet4_cemc_eta->Write();
  h1D_towerjet4_cemc_phi->Write();
  h1D_towerjet4_cemc_dR->Write();
  h1D_towerjet4_ihcal_n->Write();
  h1D_towerjet4_ihcal_e->Write();
  h1D_towerjet4_ihcal_p->Write();
  h1D_towerjet4_ihcal_pt->Write();
  h1D_towerjet4_ihcal_eta->Write();
  h1D_towerjet4_ihcal_phi->Write();
  h1D_towerjet4_ihcal_dR->Write();
  h1D_towerjet4_ohcal_n->Write();
  h1D_towerjet4_ohcal_e->Write();
  h1D_towerjet4_ohcal_p->Write();
  h1D_towerjet4_ohcal_pt->Write();
  h1D_towerjet4_ohcal_eta->Write();
  h1D_towerjet4_ohcal_phi->Write();
  h1D_towerjet4_ohcal_dR->Write();
  /////
  h1D_towerjet4_quark_e->Write();
  h1D_towerjet4_quark_p->Write();
  h1D_towerjet4_quark_pt->Write();
  h1D_towerjet4_quark_eta->Write();
  h1D_towerjet4_quark_phi->Write();
  h1D_towerjet4_quark_cemc_e->Write();
  h1D_towerjet4_quark_cemc_p->Write();
  h1D_towerjet4_quark_cemc_pt->Write();
  h1D_towerjet4_quark_cemc_eta->Write();
  h1D_towerjet4_quark_cemc_phi->Write();
  h1D_towerjet4_quark_cemc_dR->Write();
  h1D_towerjet4_quark_ihcal_n->Write();
  h1D_towerjet4_quark_ihcal_e->Write();
  h1D_towerjet4_quark_ihcal_p->Write();
  h1D_towerjet4_quark_ihcal_pt->Write();
  h1D_towerjet4_quark_ihcal_eta->Write();
  h1D_towerjet4_quark_ihcal_phi->Write();
  h1D_towerjet4_quark_ihcal_dR->Write();
  h1D_towerjet4_quark_ohcal_n->Write();
  h1D_towerjet4_quark_ohcal_e->Write();
  h1D_towerjet4_quark_ohcal_p->Write();
  h1D_towerjet4_quark_ohcal_pt->Write();
  h1D_towerjet4_quark_ohcal_eta->Write();
  h1D_towerjet4_quark_ohcal_phi->Write();
  h1D_towerjet4_quark_ohcal_dR->Write();
  /////
  h1D_towerjet4_gluon_e->Write();
  h1D_towerjet4_gluon_p->Write();
  h1D_towerjet4_gluon_pt->Write();
  h1D_towerjet4_gluon_eta->Write();
  h1D_towerjet4_gluon_phi->Write();
  h1D_towerjet4_gluon_cemc_e->Write();
  h1D_towerjet4_gluon_cemc_p->Write();
  h1D_towerjet4_gluon_cemc_pt->Write();
  h1D_towerjet4_gluon_cemc_eta->Write();
  h1D_towerjet4_gluon_cemc_phi->Write();
  h1D_towerjet4_gluon_cemc_dR->Write();
  h1D_towerjet4_gluon_ihcal_n->Write();
  h1D_towerjet4_gluon_ihcal_e->Write();
  h1D_towerjet4_gluon_ihcal_p->Write();
  h1D_towerjet4_gluon_ihcal_pt->Write();
  h1D_towerjet4_gluon_ihcal_eta->Write();
  h1D_towerjet4_gluon_ihcal_phi->Write();
  h1D_towerjet4_gluon_ihcal_dR->Write();
  h1D_towerjet4_gluon_ohcal_n->Write();
  h1D_towerjet4_gluon_ohcal_e->Write();
  h1D_towerjet4_gluon_ohcal_p->Write();
  h1D_towerjet4_gluon_ohcal_pt->Write();
  h1D_towerjet4_gluon_ohcal_eta->Write();
  h1D_towerjet4_gluon_ohcal_phi->Write();
  h1D_towerjet4_gluon_ohcal_dR->Write();
  //////// 
  //////// 
  h1D_JES->Write();
  h1D_JER->Write();
  h2D_JES_eta->Write();
  h2D_JER_eta->Write();
  h1D_modi_JES->Write();
  h1D_modi_JER->Write();
  h2D_modi_JES_eta->Write();
  h2D_modi_JER_eta->Write();
  h2D_JES_calfrac->Write();
  h2D_JES_emfrac->Write();
  h2D_JER_emfrac->Write();
  h2D_modi_JES_calfrac->Write();
  h2D_modi_JES_emfrac->Write();
  h2D_modi_JER_emfrac->Write();
  ////
  h1D_quark_JES->Write();
  h1D_quark_JER->Write();
  h2D_quark_JES_eta->Write();
  h2D_quark_JER_eta->Write();
  h1D_quark_modi_JES->Write();
  h1D_quark_modi_JER->Write();
  h2D_quark_modi_JES_eta->Write();
  h2D_quark_modi_JER_eta->Write();
  h2D_quark_JES_calfrac->Write();
  h2D_quark_JES_emfrac->Write();
  h2D_quark_JER_emfrac->Write();
  h2D_quark_modi_JES_calfrac->Write();
  h2D_quark_modi_JES_emfrac->Write();
  h2D_quark_modi_JER_emfrac->Write();
  ////
  h1D_gluon_JES->Write();
  h1D_gluon_JER->Write();
  h2D_gluon_JES_eta->Write();
  h2D_gluon_JER_eta->Write();
  h1D_gluon_modi_JES->Write();
  h1D_gluon_modi_JER->Write();
  h2D_gluon_modi_JES_eta->Write();
  h2D_gluon_modi_JER_eta->Write();
  h2D_gluon_JES_calfrac->Write();
  h2D_gluon_JES_emfrac->Write();
  h2D_gluon_JER_emfrac->Write();
  h2D_gluon_modi_JES_calfrac->Write();
  h2D_gluon_modi_JES_emfrac->Write();
  h2D_gluon_modi_JER_emfrac->Write();
  ////
  hjet_p_nconstit->Write();
  hjet_p_nconstit_quark->Write();
  hjet_p_nconstit_gluon->Write();
  hjet_p_nconstit_else->Write();
  hjet_twr_nconstit->Write();
  hjet_twr_nconstit_quark->Write();
  hjet_twr_nconstit_gluon->Write();
  hjet_twr_nconstit_else->Write();
  fout->Close();

  return; 
}
