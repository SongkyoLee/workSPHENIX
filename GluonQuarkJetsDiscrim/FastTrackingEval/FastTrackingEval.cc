
#include "FastTrackingEval.h"

#include <phool/phool.h>
#include <phool/getClass.h>

#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/PHTFileServer.h>
#include <fun4all/Fun4AllServer.h>

#include <g4main/PHG4HitContainer.h>
#include <g4main/PHG4TruthInfoContainer.h>
#include <g4main/PHG4Particle.h>
#include <g4main/PHG4Hit.h>
#include <g4main/PHG4VtxPoint.h>
#include <g4main/PHG4VtxPointv1.h>

#include <g4hough/SvtxVertexMap.h>
#include <g4hough/SvtxVertex.h>
#include <g4hough/SvtxTrackMap.h>
#include <g4hough/SvtxTrack.h>
#include <g4eval/SvtxTrackEval.h>
#include <g4hough/SvtxClusterMap.h>
#include <g4hough/SvtxCluster.h>
#include <g4hough/SvtxHitMap.h>
#include <g4hough/SvtxHit.h>
#include <g4hough/SvtxTrackState.h>

#include <g4eval/SvtxEvalStack.h>
#include <g4eval/SvtxTrackEval.h>
#include <g4eval/SvtxClusterEval.h>
#include <g4eval/SvtxTruthEval.h>
#include <g4eval/SvtxVertexEval.h>
#include <g4eval/SvtxHitEval.h>

#include <g4cemc/RawTowerDefs.h>
#include <g4cemc/RawTowerContainer.h>
#include <g4cemc/RawTower.h>
#include <g4cemc/RawTowerGeomContainer.h>
#include <g4cemc/RawTowerGeom.h>
#include <g4cemc/RawClusterContainer.h>
#include <g4cemc/RawCluster.h>

#include <g4detectors/PHG4CylinderCellContainer.h>

#include <phhepmc/PHHepMCGenEvent.h>
#include <HepMC/GenEvent.h>
#include <HepMC/GenParticle.h>

#include <GenFit/AbsMeasurement.h>
#include <GenFit/EventDisplay.h>
#include <GenFit/MeasuredStateOnPlane.h>
#include <GenFit/RKTrackRep.h>
#include <GenFit/StateOnPlane.h>
#include <GenFit/Track.h>
#include <phgenfit/Fitter.h>
#include <phgenfit/PlanarMeasurement.h>
#include <phgenfit/Track.h>
#include <phgenfit/SpacepointMeasurement.h>

#include <phgeom/PHGeomUtility.h>

#include <g4detectors/PHG4CylinderCellGeomContainer.h>
#include <g4detectors/PHG4CylinderCellGeom_Spacalv1.h>
#include <g4detectors/PHG4CylinderGeomContainer.h>
#include <g4detectors/PHG4CylinderGeom_Spacalv3.h>
#include <g4detectors/PHG4CellDefs.h>

// fastjet includes
#include <fastjet/JetDefinition.hh>
#include <fastjet/PseudoJet.hh>
#include <fastjet/ClusterSequence.hh>
#include <fastjet/SISConePlugin.hh>

#include <TTree.h>
#include <TH2D.h>
#include <TVector3.h>
#include <TRandom3.h>

#include <iostream>

// GEANT
#include <Geant4/G4ParticleTable.hh>
#include <Geant4/G4ParticleDefinition.hh>

#define LogDebug(exp)		std::cout<<"DEBUG: "	<<__FILE__<<": "<<__LINE__<<": "<< exp <<"\n"
#define LogError(exp)		std::cout<<"ERROR: "	<<__FILE__<<": "<<__LINE__<<": "<< exp <<"\n"
#define LogWarning(exp)	std::cout<<"WARNING: "	<<__FILE__<<": "<<__LINE__<<": "<< exp <<"\n"

using namespace std;

const float TOWER_E_CUT = 0.100;   // tower energy cutoff for sum

// Jet finding and matching parameters
const float _R = 0.4; 
const float ptmin = 5.0; 
const float p_beam = 200.0; 
const float _theEtaLow = -1.1; 
const float _theEtaHigh = 1.1; 
const float jetMatchDR = 0.4; 

// Progenitor data structures
typedef struct {
  int pid;
  int status; 
  int barcode; 
  float energy;     // constituent energy assoc. with progenitor
  float pg_energy;  // progenitor energy 
} JetProgenitor; 

bool sortfunction( JetProgenitor i, JetProgenitor j){
  return (i.energy > j.energy); 
}

std::vector<JetProgenitor> progenitors; 
std::vector<JetProgenitor> combined_progenitors; 

// Calorimeter incident particles 
std::vector<ParticleTower *> calo_incident; 

// Reconstructed Jets data structures
typedef struct{
  fastjet::PseudoJet *jet; 
  std::vector<JetProgenitor> progenitors;  
  float z_lead; 
  float charge_lead; 
  float emfract; 
  vector<fastjet::PseudoJet> constituents; 
}RecoJet; 

std::vector<RecoJet> prim_jets;
std::vector<RecoJet> trk_jets;
std::vector<RecoJet> twr_jets;

double XYtoPhi(double x, double y)
{
  // -Pi to +Pi
  Double_t phi = atan2(y,x);
  if(phi<-TMath::Pi()) phi += TMath::TwoPi();
  if(phi>=TMath::Pi()) phi -= TMath::TwoPi();
  return phi;
}

double XYtoPhi_02PI(double x, double y)
{
  // 0 to 2pi
  Double_t phi = atan2(y,x);
  if(phi<0.0) phi += TMath::TwoPi();
  if(phi>=TMath::TwoPi()) phi -= TMath::TwoPi();
  return phi;
}

double getEta(double pt, double pz){
  float theta = XYtoPhi(pz,pt);
  float eta = -log(tan(theta/2.0));
  return eta; 
} 

double DeltaPhi(double phi1, double phi2){
  Double_t dphi;
  dphi = phi1 - phi2;
  if(dphi<-TMath::Pi()) dphi += TMath::TwoPi();
  if(dphi>=TMath::Pi()) dphi -= TMath::TwoPi();
  return dphi;
}

//----------------------------------------------------------------------------//
//-- Constructor:
//--  simple initialization
//----------------------------------------------------------------------------//
FastTrackingEval::FastTrackingEval(const string &name) :
  SubsysReco(name), _flags(NONE), pythia6(false), use_ihcal(true), rescale_cemc(false), rescale_hcal(false), 
  cemc_rescale_factor(1.25), hcal_rescale_factor(1.0/0.67), 
  _eval_tree_tracks( NULL), _eval_tree_pairs(NULL),
  _eval_tree_event(NULL), _eval_tree_prim_jets(NULL), _eval_tree_trk_jets(NULL), _eval_tree_twr_jets(NULL), 
  _eval_tree_matched_jets(NULL){
	//initialize
	_event = 0;
	_outfile_name = "FastTrackingEval.root";
	rand = (TRandom *)new TRandom3(0); 
	svtx_eval = NULL;

}

//----------------------------------------------------------------------------//
//-- Init():
//--   Intialize all histograms, trees, and ntuples
//----------------------------------------------------------------------------//
int FastTrackingEval::Init(PHCompositeNode *topNode) {
	cout << PHWHERE << " Opening file " << _outfile_name << endl;
	PHTFileServer::get().open(_outfile_name, "RECREATE");

	// // create TTree
	_eval_tree_tracks = new TTree("tracks", "FastSim Eval => tracks");
	_eval_tree_tracks->Branch("event", &event, "event/I");
	_eval_tree_tracks->Branch("process", &process, "process/I");
	_eval_tree_tracks->Branch("n_mpi", &n_mpi, "n_mpi/I");
	_eval_tree_tracks->Branch("femc_sumE", &femc_sumE, "femc_sumE/F");
	_eval_tree_tracks->Branch("fhcal_sumE", &fhcal_sumE, "fhcal_sumE/F");
	_eval_tree_tracks->Branch("ihcal_sumE", &ihcal_sumE, "ihcal_sumE/F");
	_eval_tree_tracks->Branch("ohcal_sumE", &ohcal_sumE, "ohcal_sumE/F");
	_eval_tree_tracks->Branch("cemc_sumE", &cemc_sumE, "cemc_sumE/F");
	_eval_tree_tracks->Branch("gflavor", &gflavor, "gflavor/I");
	_eval_tree_tracks->Branch("gpx", &gpx, "gpx/F");
	_eval_tree_tracks->Branch("gpy", &gpy, "gpy/F");
	_eval_tree_tracks->Branch("gpz", &gpz, "gpz/F");
	_eval_tree_tracks->Branch("gvx", &gvx, "gvx/F");
	_eval_tree_tracks->Branch("gvy", &gvy, "gvy/F");
	_eval_tree_tracks->Branch("gvz", &gvz, "gvz/F");
	_eval_tree_tracks->Branch("isprim", &isprim, "isprim/F");	
	_eval_tree_tracks->Branch("charge", &charge, "charge/I");
	_eval_tree_tracks->Branch("nhits", &nhits, "nhits/I");
	_eval_tree_tracks->Branch("px", &px, "px/F");
	_eval_tree_tracks->Branch("py", &py, "py/F");
	_eval_tree_tracks->Branch("pz", &pz, "pz/F");
	_eval_tree_tracks->Branch("dca2d", &dca2d, "dca2d/F");
	_eval_tree_tracks->Branch("cemc_cldr", &cemc_cldr, "cemc_cldr1/F");
	_eval_tree_tracks->Branch("cemc_clE", &cemc_clE, "cemc_clE1/F");
	_eval_tree_tracks->Branch("cemc_clE_PC", &cemc_clE_PC, "cemc_clE1_PC/F");
	_eval_tree_tracks->Branch("cemc_E33", &cemc_E33, "cemc_E331/F");
	_eval_tree_tracks->Branch("cemc_E33_PC", &cemc_E33_PC, "cemc_E331_PC/F");
	_eval_tree_tracks->Branch("cemc_E55", &cemc_E55, "cemc_E551/F");
	_eval_tree_tracks->Branch("cemc_clN", &cemc_clN, "cemc_clN1/F");
	_eval_tree_tracks->Branch("hcalin_cldr", &hcalin_cldr, "hcalin_cldr/F");
	_eval_tree_tracks->Branch("hcalin_clE", &hcalin_clE, "hcalin_clE/F");
	_eval_tree_tracks->Branch("hcalin_E33", &hcalin_E33, "hcalin_E33/F");
	_eval_tree_tracks->Branch("hcalin_E55", &hcalin_E55, "hcalin_E55/F");
	_eval_tree_tracks->Branch("hcalin_clN", &hcalin_clN, "hcalin_clN/F");
	_eval_tree_tracks->Branch("hcalout_cldr", &hcalout_cldr, "hcalout_cldr/F");
	_eval_tree_tracks->Branch("hcalout_clE", &hcalout_clE, "hcalout_clE/F");
	_eval_tree_tracks->Branch("hcalout_E33", &hcalout_E33, "hcalout_E33/F");
	_eval_tree_tracks->Branch("hcalout_E55", &hcalout_E55, "hcalout_E55/F");
	_eval_tree_tracks->Branch("hcalout_clN", &hcalout_clN, "hcalout_clN/F");
	_eval_tree_tracks->Branch("vtx_x", &vtx_x, "vtx_x/F");
	_eval_tree_tracks->Branch("vtx_y", &vtx_y, "vtx_y/F");
	_eval_tree_tracks->Branch("vtx_z", &vtx_z, "vtx_z/F");
	_eval_tree_tracks->Branch("vtx_t", &vtx_t, "vtx_t/F");
	_eval_tree_tracks->Branch("x1", &x1, "x1/F");
	_eval_tree_tracks->Branch("x2", &x2, "x2/F");
	_eval_tree_tracks->Branch("Q2", &Q2, "Q2/F");	
	_eval_tree_tracks->Branch("chi2", &chi2, "chi2/F");	
	_eval_tree_tracks->Branch("ndf", &ndf, "ndf/F");

	// Di-electron Pairs

	// _eval_tree_pairs = new TTree("pairs", "Tracking Eval => DY candidate pairs");
	// _eval_tree_pairs->Branch("event", &event, "event1/I");
	// _eval_tree_pairs->Branch("ihcal_sumE", &ihcal_sumE, "ihcal_sumE/F");
	// _eval_tree_pairs->Branch("ohcal_sumE", &ohcal_sumE, "ohcal_sumE/F");
	// _eval_tree_pairs->Branch("cemc_sumE", &cemc_sumE, "cemc_sumE/F");
	// _eval_tree_pairs->Branch("femc_Ntwr", &femc_Ntwr, "femc_Ntwr/F");
	// _eval_tree_pairs->Branch("fhcal_Ntwr", &fhcal_Ntwr, "fhcal_Ntwr/F");
	// _eval_tree_pairs->Branch("ihcal_Ntwr", &ihcal_Ntwr, "ihcal_Ntwr/F");
	// _eval_tree_pairs->Branch("ohcal_Ntwr", &ohcal_Ntwr, "ohcal_Ntwr/F");
	// _eval_tree_pairs->Branch("cemc_Ntwr", &cemc_Ntwr, "cemc_Ntwr/F");
	// _eval_tree_pairs->Branch("fwd_ntrk", &fwd_ntrk, "fwd_ntrk/F");
	// _eval_tree_pairs->Branch("vtx_x", &vtx_x, "vtx_x/F");
	// _eval_tree_pairs->Branch("vtx_y", &vtx_y, "vtx_y/F");
	// _eval_tree_pairs->Branch("vtx_z", &vtx_z, "vtx_z/F");
	// _eval_tree_pairs->Branch("vtx_t", &vtx_t, "vtx_t/F");
	// _eval_tree_pairs->Branch("process", &process, "process/I");
	// _eval_tree_pairs->Branch("x1", &x1, "x1/F");
	// _eval_tree_pairs->Branch("x2", &x2, "x2/F");
	// _eval_tree_pairs->Branch("Q2", &Q2, "Q2/F");	
	// _eval_tree_pairs->Branch("pairAccepted", &pairAccepted, "pairAccepted/I");
	// _eval_tree_pairs->Branch("true_pair", &true_pair, "true_pair/I");

	// _eval_tree_pairs->Branch("gflavor1", &gflavor, "gflavor1/I");
	// _eval_tree_pairs->Branch("gparentid1", &gparentid, "gparentid1/I");
	// _eval_tree_pairs->Branch("gpx1", &gpx, "gpx1/F");
	// _eval_tree_pairs->Branch("gpy1", &gpy, "gpy1/F");
	// _eval_tree_pairs->Branch("gpz1", &gpz, "gpz1/F");
	// _eval_tree_pairs->Branch("gvx1", &gvx, "gvx1/F");
	// _eval_tree_pairs->Branch("gvy1", &gvy, "gvy1/F");
	// _eval_tree_pairs->Branch("gvz1", &gvz, "gvz1/F");
	// _eval_tree_pairs->Branch("isprim1", &isprim, "isprim1/F");	
	// _eval_tree_pairs->Branch("charge1", &charge, "charge1/I");
	// _eval_tree_pairs->Branch("nhits1", &nhits, "nhits1/I");
	// _eval_tree_pairs->Branch("px1", &px, "px1/F");
	// _eval_tree_pairs->Branch("py1", &py, "py1/F");
	// _eval_tree_pairs->Branch("pz1", &pz, "pz1/F");
	// _eval_tree_pairs->Branch("eta1", &eta, "eta1/F");
	// _eval_tree_pairs->Branch("dca2d1", &dca2d, "dca2d1/F");
	// _eval_tree_pairs->Branch("cemc_cldr1", &cemc_cldr, "cemc_cldr1/F");
	// _eval_tree_pairs->Branch("cemc_clE1", &cemc_clE, "cemc_clE1/F");
	// _eval_tree_pairs->Branch("cemc_clE1_PC", &cemc_clE_PC, "cemc_clE1_PC/F");
	// _eval_tree_pairs->Branch("cemc_E331", &cemc_E33, "cemc_E331/F");
	// _eval_tree_pairs->Branch("cemc_E331_PC", &cemc_E33_PC, "cemc_E331_PC/F");
	// _eval_tree_pairs->Branch("cemc_E551", &cemc_E55, "cemc_E551/F");
	// _eval_tree_pairs->Branch("cemc_clN1", &cemc_clN, "cemc_clN1/F");
	// _eval_tree_pairs->Branch("hcalin_cldr1", &hcalin_cldr, "hcalin_cldr1/F");
	// _eval_tree_pairs->Branch("hcalin_clE1", &hcalin_clE, "hcalin_clE1/F");
	// _eval_tree_pairs->Branch("hcalin_E331", &hcalin_E33, "hcalin_E331/F");
	// _eval_tree_pairs->Branch("hcalin_E551", &hcalin_E55, "hcalin_E551/F");
	// _eval_tree_pairs->Branch("hcalin_clN1", &hcalin_clN, "hcalin_clN1/F");
	// _eval_tree_pairs->Branch("hcalout_cldr1", &hcalout_cldr, "hcalout_cldr1/F");
	// _eval_tree_pairs->Branch("hcalout_clE1", &hcalout_clE, "hcalout_clE1/F");
	// _eval_tree_pairs->Branch("hcalout_E331", &hcalout_E33, "hcalout_E331/F");
	// _eval_tree_pairs->Branch("hcalout_E551", &hcalout_E55, "hcalout_E551/F");
	// _eval_tree_pairs->Branch("hcalout_clN1", &hcalout_clN, "hcalout_clN1/F");
	// _eval_tree_pairs->Branch("track_E_over_p1", &track_E_over_p, "track_E_over_p1/F");
	// _eval_tree_pairs->Branch("track_E_over_p1_PC", &track_E_over_p_PC, "track_E_over_p1_PC/F");
	// _eval_tree_pairs->Branch("cemc_plen1", &cemc_plen, "cemc_plen1/F");
	// _eval_tree_pairs->Branch("hcalin_plen1", &hcalin_plen, "hcalin_plen1/F");
	// _eval_tree_pairs->Branch("hcalout_plen1", &hcalout_plen, "hcalout_plen1/F");
	// _eval_tree_pairs->Branch("chi21", &chi2, "chi21/F");	
	// _eval_tree_pairs->Branch("ndf1", &ndf, "ndf1/F");	
	// _eval_tree_pairs->Branch("cemc_clid1", &cemc_clid, "cemc_clid1/F");	
	// _eval_tree_pairs->Branch("DYmatch1", &DYmatch, "DYmatch1/I");

	// _eval_tree_pairs->Branch("gflavor2", &gflavor2, "gflavor2/I");
	// _eval_tree_pairs->Branch("gparentid2", &gparentid2, "gparentid2/I");
	// _eval_tree_pairs->Branch("gpx2", &gpx2, "gpx2/F");
	// _eval_tree_pairs->Branch("gpy2", &gpy2, "gpy2/F");
	// _eval_tree_pairs->Branch("gpz2", &gpz2, "gpz2/F");
	// _eval_tree_pairs->Branch("gvx2", &gvx2, "gvx2/F");
	// _eval_tree_pairs->Branch("gvy2", &gvy2, "gvy2/F");
	// _eval_tree_pairs->Branch("gvz2", &gvz2, "gvz2/F");
	// _eval_tree_pairs->Branch("isprim2", &isprim2, "isprim2/F");	
	// _eval_tree_pairs->Branch("charge2", &charge2, "charge2/I");
	// _eval_tree_pairs->Branch("nhits2", &nhits2, "nhits2/I");
	// _eval_tree_pairs->Branch("px2", &px2, "px2/F");
	// _eval_tree_pairs->Branch("py2", &py2, "py2/F");
	// _eval_tree_pairs->Branch("pz2", &pz2, "pz2/F");
	// _eval_tree_pairs->Branch("eta2", &eta2, "eta2/F");
	// _eval_tree_pairs->Branch("dca2d2", &dca2d2, "dca2d2/F");
	// _eval_tree_pairs->Branch("cemc_cldr2", &cemc_cldr2, "cemc_cldr2/F");
	// _eval_tree_pairs->Branch("cemc_clE2", &cemc_clE2, "cemc_clE2/F");
	// _eval_tree_pairs->Branch("cemc_clE2_PC", &cemc_clE2_PC, "cemc_clE2_PC/F");
	// _eval_tree_pairs->Branch("cemc_E332", &cemc_E332, "cemc_E332/F");
	// _eval_tree_pairs->Branch("cemc_E332_PC", &cemc_E332_PC, "cemc_E332_PC/F");
	// _eval_tree_pairs->Branch("cemc_E552", &cemc_E552, "cemc_E552/F");
	// _eval_tree_pairs->Branch("cemc_clN2", &cemc_clN2, "cemc_clN2/F");
	// _eval_tree_pairs->Branch("hcalin_cldr2", &hcalin_cldr2, "hcalin_cldr2/F");
	// _eval_tree_pairs->Branch("hcalin_clE2", &hcalin_clE2, "hcalin_clE2/F");
	// _eval_tree_pairs->Branch("hcalin_E332", &hcalin_E332, "hcalin_E332/F");
	// _eval_tree_pairs->Branch("hcalin_E552", &hcalin_E552, "hcalin_E552/F");
	// _eval_tree_pairs->Branch("hcalin_clN2", &hcalin_clN2, "hcalin_clN2/F");
	// _eval_tree_pairs->Branch("hcalout_cldr2", &hcalout_cldr2, "hcalout_cldr2/F");
	// _eval_tree_pairs->Branch("hcalout_clE2", &hcalout_clE2, "hcalout_clE2/F");
	// _eval_tree_pairs->Branch("hcalout_E332", &hcalout_E332, "hcalout_E332/F");
	// _eval_tree_pairs->Branch("hcalout_E552", &hcalout_E552, "hcalout_E552/F");
	// _eval_tree_pairs->Branch("hcalout_clN2", &hcalout_clN2, "hcalout_clN2/F");
	// _eval_tree_pairs->Branch("track_E_over_p2", &track_E_over_p2, "track_E_over_p2/F");
	// _eval_tree_pairs->Branch("track_E_over_p2_PC", &track_E_over_p2_PC, "track_E_over_p2_PC/F");
	// _eval_tree_pairs->Branch("cemc_plen2", &cemc_plen2, "cemc_plen2/F");
	// _eval_tree_pairs->Branch("hcalin_plen2", &hcalin_plen2, "hcalin_plen2/F");
	// _eval_tree_pairs->Branch("hcalout_plen2", &hcalout_plen2, "hcalout_plen2/F");
	// _eval_tree_pairs->Branch("chi22", &chi22, "chi22/F");	
	// _eval_tree_pairs->Branch("ndf2", &ndf2, "ndf2/F");	
	// _eval_tree_pairs->Branch("cemc_clid2", &cemc_clid2, "cemc_clid2/F");	
	// _eval_tree_pairs->Branch("DYmatch2", &DYmatch2, "DYmatch2/I");

	// _eval_tree_pairs->Branch("mass", &mass, "mass/F");
	// _eval_tree_pairs->Branch("gmass", &gmass, "gmass/F");
	
	// Event Tree

	_eval_tree_event = new TTree("event", "FastSim Eval => event parameters");
	_eval_tree_event->Branch("event", &event, "event/I");
	_eval_tree_event->Branch("process", &process, "process/I");
	_eval_tree_event->Branch("n_mpi", &n_mpi, "n_mpi/I");
	_eval_tree_event->Branch("femc_sumE", &femc_sumE, "femc_sumE/F");
	_eval_tree_event->Branch("fhcal_sumE", &fhcal_sumE, "fhcal_sumE/F");
	_eval_tree_event->Branch("ihcal_sumE", &ihcal_sumE, "ihcal_sumE/F");
	_eval_tree_event->Branch("ohcal_sumE", &ohcal_sumE, "ohcal_sumE/F");
	_eval_tree_event->Branch("cemc_sumE", &cemc_sumE, "cemc_sumE/F");
	_eval_tree_event->Branch("femc_Ntwr", &femc_Ntwr, "femc_Ntwr/F");
	_eval_tree_event->Branch("fhcal_Ntwr", &fhcal_Ntwr, "fhcal_Ntwr/F");
	_eval_tree_event->Branch("ihcal_Ntwr", &ihcal_Ntwr, "ihcal_Ntwr/F");
	_eval_tree_event->Branch("ohcal_Ntwr", &ohcal_Ntwr, "ohcal_Ntwr/F");
	_eval_tree_event->Branch("cemc_Ntwr", &cemc_Ntwr, "cemc_Ntwr/F");
	_eval_tree_event->Branch("fwd_ntrk", &fwd_ntrk, "fwd_ntrk/F");
	_eval_tree_event->Branch("vtx_x", &vtx_x, "vtx_x/F");
	_eval_tree_event->Branch("vtx_y", &vtx_y, "vtx_y/F");
	_eval_tree_event->Branch("vtx_z", &vtx_z, "vtx_z/F");
	_eval_tree_event->Branch("vtx_t", &vtx_t, "vtx_t/F");	
	_eval_tree_event->Branch("x1", &x1, "x1/F");
	_eval_tree_event->Branch("x2", &x2, "x2/F");
	_eval_tree_event->Branch("Q2", &Q2, "Q2/F");	


	// Primary Jets

	_eval_tree_prim_jets = new TTree("prim_jets", "FastSim Eval => primary jets");
	_eval_tree_prim_jets->Branch("event", &event, "event/I");
	_eval_tree_prim_jets->Branch("process", &process, "process/I");
	_eval_tree_prim_jets->Branch("n_mpi", &n_mpi, "n_mpi/I");
	_eval_tree_prim_jets->Branch("femc_sumE", &femc_sumE, "femc_sumE/F");
	_eval_tree_prim_jets->Branch("fhcal_sumE", &fhcal_sumE, "fhcal_sumE/F");
	_eval_tree_prim_jets->Branch("ihcal_sumE", &ihcal_sumE, "ihcal_sumE/F");
	_eval_tree_prim_jets->Branch("ohcal_sumE", &ohcal_sumE, "ohcal_sumE/F");
	_eval_tree_prim_jets->Branch("cemc_sumE", &cemc_sumE, "cemc_sumE/F");
	_eval_tree_prim_jets->Branch("vtx_x", &vtx_x, "vtx_x/F");
	_eval_tree_prim_jets->Branch("vtx_y", &vtx_y, "vtx_y/F");
	_eval_tree_prim_jets->Branch("vtx_z", &vtx_z, "vtx_z/F");
	_eval_tree_prim_jets->Branch("vtx_t", &vtx_t, "vtx_t/F");
	_eval_tree_prim_jets->Branch("x1", &x1, "x1/F");
	_eval_tree_prim_jets->Branch("x2", &x2, "x2/F");
	_eval_tree_prim_jets->Branch("Q2", &Q2, "Q2/F");	

	_eval_tree_prim_jets->Branch("jet_pT", &jet_pT, "jet_pT/F");
	_eval_tree_prim_jets->Branch("jet_ptot", &jet_ptot, "jet_ptot/F");
	_eval_tree_prim_jets->Branch("jet_E", &jet_E, "jet_E/F");
	_eval_tree_prim_jets->Branch("jet_eta", &jet_eta, "jet_eta/F");
	_eval_tree_prim_jets->Branch("jet_rapidity", &jet_rapidity, "jet_rapidity/F");
	_eval_tree_prim_jets->Branch("jet_mass", &jet_mass, "jet_mass/F");
	_eval_tree_prim_jets->Branch("jet_phi", &jet_phi, "jet_phi/F");
	_eval_tree_prim_jets->Branch("jet_xF", &jet_xF, "jet_xF/F");
	_eval_tree_prim_jets->Branch("jet_constit", &jet_nconstit, "jet_nconstit/I");
	
	_eval_tree_prim_jets->Branch("jet_emfract", &jet_emfract, "jet_emfract/F");
	
	_eval_tree_prim_jets->Branch("pg_prim_id",&pg_prim_id); 
	_eval_tree_prim_jets->Branch("pg_prim_fract",&pg_prim_fract); 
	_eval_tree_prim_jets->Branch("pg_prim_status",&pg_prim_status); 

	_eval_tree_prim_jets->Branch("z_lead", &z_lead_p, "z_lead/F");
	_eval_tree_prim_jets->Branch("charge_lead", &charge_lead_p, "charge_lead/I");

	_eval_tree_prim_jets->Branch("prim_constituent_E",&prim_constituent_E); 
	_eval_tree_prim_jets->Branch("prim_constituent_dR",&prim_constituent_dR); 
	_eval_tree_prim_jets->Branch("prim_constituent_pT",&prim_constituent_pT);  // pT wrt jet axis
	_eval_tree_prim_jets->Branch("prim_constituent_pz",&prim_constituent_pz);  // pz along jet axis
	_eval_tree_prim_jets->Branch("prim_constituent_phi",&prim_constituent_phi); 
	_eval_tree_prim_jets->Branch("prim_constituent_z",&prim_constituent_z);    // z = p||/pjet

	// Track Jets

	_eval_tree_trk_jets = new TTree("trk_jets", "FastSim Eval => track jets");
	_eval_tree_trk_jets->Branch("event", &event, "event/I");
	_eval_tree_trk_jets->Branch("process", &process, "process/I");
	_eval_tree_trk_jets->Branch("n_mpi", &n_mpi, "n_mpi/I");
	_eval_tree_trk_jets->Branch("femc_sumE", &femc_sumE, "femc_sumE/F");
	_eval_tree_trk_jets->Branch("fhcal_sumE", &fhcal_sumE, "fhcal_sumE/F");
	_eval_tree_trk_jets->Branch("ihcal_sumE", &ihcal_sumE, "ihcal_sumE/F");
	_eval_tree_trk_jets->Branch("ohcal_sumE", &ohcal_sumE, "ohcal_sumE/F");
	_eval_tree_trk_jets->Branch("cemc_sumE", &cemc_sumE, "cemc_sumE/F");
	_eval_tree_trk_jets->Branch("vtx_x", &vtx_x, "vtx_x/F");
	_eval_tree_trk_jets->Branch("vtx_y", &vtx_y, "vtx_y/F");
	_eval_tree_trk_jets->Branch("vtx_z", &vtx_z, "vtx_z/F");
	_eval_tree_trk_jets->Branch("vtx_t", &vtx_t, "vtx_t/F");
	_eval_tree_trk_jets->Branch("x1", &x1, "x1/F");
	_eval_tree_trk_jets->Branch("x2", &x2, "x2/F");
	_eval_tree_trk_jets->Branch("Q2", &Q2, "Q2/F");	

	_eval_tree_trk_jets->Branch("jet_pT", &jet_pT, "jet_pT/F");
	_eval_tree_trk_jets->Branch("jet_ptot", &jet_ptot, "jet_ptot/F");
	_eval_tree_trk_jets->Branch("jet_tkE", &jet_tkE, "jet_tkE/F");
	_eval_tree_trk_jets->Branch("jet_E", &jet_E, "jet_E/F");
	_eval_tree_trk_jets->Branch("jet_eta", &jet_eta, "jet_eta/F");
	_eval_tree_trk_jets->Branch("jet_rapidity", &jet_rapidity, "jet_rapidity/F");
	_eval_tree_trk_jets->Branch("jet_mass", &jet_mass, "jet_mass/F");
	_eval_tree_trk_jets->Branch("jet_phi", &jet_phi, "jet_phi/F");
	_eval_tree_trk_jets->Branch("jet_xF", &jet_xF, "jet_xF/F");
	_eval_tree_trk_jets->Branch("jet_constit", &jet_nconstit, "jet_nconstit/I");
	
	_eval_tree_trk_jets->Branch("pg_prim_id",&pg_prim_id); 
	_eval_tree_trk_jets->Branch("pg_prim_fract",&pg_prim_fract); 
	_eval_tree_trk_jets->Branch("pg_prim_status",&pg_prim_status); 
	
	_eval_tree_trk_jets->Branch("pg_trk_id",&pg_trk_id); 
	_eval_tree_trk_jets->Branch("pg_trk_fract",&pg_trk_fract); 
	_eval_tree_trk_jets->Branch("pg_trk_status",&pg_trk_status); 

	_eval_tree_trk_jets->Branch("z_lead", &z_lead, "z_lead/F");
	_eval_tree_trk_jets->Branch("charge_lead", &charge_lead, "charge_lead/I");

	_eval_tree_trk_jets->Branch("mpjet_ptot", &mpjet_ptot, "mpjet_ptot/F");
	_eval_tree_trk_jets->Branch("mpjet_eta", &mpjet_eta, "mpjet_eta/F");
	_eval_tree_trk_jets->Branch("mpjet_pT", &mpjet_pT, "mpjet_pT/F");
	_eval_tree_trk_jets->Branch("mpjet_dR", &mpjet_dR, "mpjet_dR/F");
	_eval_tree_trk_jets->Branch("mpjet_emfract", &mpjet_emfract, "mpjet_emfract/F");
	_eval_tree_trk_jets->Branch("mpjet_z_lead", &mpjet_z_lead, "mpjet_z_lead/F");
	_eval_tree_trk_jets->Branch("mpjet_charge_lead", &mpjet_charge_lead, "mpjet_charge_lead/I");

	_eval_tree_trk_jets->Branch("trk_constituent_E",&trk_constituent_E); 
	_eval_tree_trk_jets->Branch("trk_constituent_dR",&trk_constituent_dR); 
	_eval_tree_trk_jets->Branch("trk_constituent_pT",&trk_constituent_pT);  // pT wrt jet axis
	_eval_tree_trk_jets->Branch("trk_constituent_pz",&trk_constituent_pz);  // pz along jet axis
	_eval_tree_trk_jets->Branch("trk_constituent_phi",&trk_constituent_phi); 
	_eval_tree_trk_jets->Branch("trk_constituent_z",&trk_constituent_z);    // z = p||/pjet

	// Tower Jets

	_eval_tree_twr_jets = new TTree("twr_jets", "FastSim Eval => tower jets");
	_eval_tree_twr_jets->Branch("event", &event, "event/I");
	_eval_tree_twr_jets->Branch("process", &process, "process/I");
	_eval_tree_twr_jets->Branch("n_mpi", &n_mpi, "n_mpi/I");
	_eval_tree_twr_jets->Branch("femc_sumE", &femc_sumE, "femc_sumE/F");
	_eval_tree_twr_jets->Branch("fhcal_sumE", &fhcal_sumE, "fhcal_sumE/F");
	_eval_tree_twr_jets->Branch("ihcal_sumE", &ihcal_sumE, "ihcal_sumE/F");
	_eval_tree_twr_jets->Branch("ohcal_sumE", &ohcal_sumE, "ohcal_sumE/F");
	_eval_tree_twr_jets->Branch("cemc_sumE", &cemc_sumE, "cemc_sumE/F");
	_eval_tree_twr_jets->Branch("vtx_x", &vtx_x, "vtx_x/F");
	_eval_tree_twr_jets->Branch("vtx_y", &vtx_y, "vtx_y/F");
	_eval_tree_twr_jets->Branch("vtx_z", &vtx_z, "vtx_z/F");
	_eval_tree_twr_jets->Branch("vtx_t", &vtx_t, "vtx_t/F");
	_eval_tree_twr_jets->Branch("x1", &x1, "x1/F");
	_eval_tree_twr_jets->Branch("x2", &x2, "x2/F");
	_eval_tree_twr_jets->Branch("Q2", &Q2, "Q2/F");	

	_eval_tree_twr_jets->Branch("jet_pT", &jet_pT, "jet_pT/F");
	_eval_tree_twr_jets->Branch("jet_ptot", &jet_ptot, "jet_ptot/F");
	_eval_tree_twr_jets->Branch("jet_twrE", &jet_twrE, "jet_twrE/F");
	_eval_tree_twr_jets->Branch("jet_E", &jet_E, "jet_E/F");
	_eval_tree_twr_jets->Branch("jet_eta", &jet_eta, "jet_eta/F");
	_eval_tree_twr_jets->Branch("jet_rapidity", &jet_rapidity, "jet_rapidity/F");
	_eval_tree_twr_jets->Branch("jet_mass", &jet_mass, "jet_mass/F");
	_eval_tree_twr_jets->Branch("jet_phi", &jet_phi, "jet_phi/F");
	_eval_tree_twr_jets->Branch("jet_xF", &jet_xF, "jet_xF/F");
	_eval_tree_twr_jets->Branch("jet_constit", &jet_nconstit, "jet_nconstit/I");

	_eval_tree_twr_jets->Branch("pg_prim_id",&pg_prim_id); 
	_eval_tree_twr_jets->Branch("pg_prim_fract",&pg_prim_fract); 
	_eval_tree_twr_jets->Branch("pg_prim_status",&pg_prim_status); 
	
	_eval_tree_twr_jets->Branch("pg_twr_id",&pg_twr_id); 
	_eval_tree_twr_jets->Branch("pg_twr_fract",&pg_twr_fract); 
	_eval_tree_twr_jets->Branch("pg_twr_status",&pg_twr_status); 

	_eval_tree_twr_jets->Branch("z_lead", &z_lead, "z_lead/F");
	_eval_tree_twr_jets->Branch("charge_lead", &charge_lead, "charge_lead/I");

	_eval_tree_twr_jets->Branch("mpjet_ptot", &mpjet_ptot, "mpjet_ptot/F");
	_eval_tree_twr_jets->Branch("mpjet_eta", &mpjet_eta, "mpjet_eta/F");
	_eval_tree_twr_jets->Branch("mpjet_pT", &mpjet_pT, "mpjet_pT/F");
	_eval_tree_twr_jets->Branch("mpjet_dR", &mpjet_dR, "mpjet_dR/F");
	_eval_tree_twr_jets->Branch("mpjet_emfract", &mpjet_emfract, "mpjet_emfract/F");
	_eval_tree_twr_jets->Branch("mpjet_z_lead", &mpjet_z_lead, "mpjet_z_lead/F");
	_eval_tree_twr_jets->Branch("mpjet_charge_lead", &mpjet_charge_lead, "mpjet_charge_lead/I");

	_eval_tree_twr_jets->Branch("cemc_constituent_E",&cemc_constituent_E); 
	_eval_tree_twr_jets->Branch("cemc_constituent_dR",&cemc_constituent_dR); 
	_eval_tree_twr_jets->Branch("cemc_constituent_pT",&cemc_constituent_pT); 
	_eval_tree_twr_jets->Branch("cemc_constituent_pz",&cemc_constituent_pz); 
	_eval_tree_twr_jets->Branch("cemc_constituent_phi",&cemc_constituent_phi); 

	_eval_tree_twr_jets->Branch("ihcal_constituent_E",&ihcal_constituent_E); 
	_eval_tree_twr_jets->Branch("ihcal_constituent_dR",&ihcal_constituent_dR); 
	_eval_tree_twr_jets->Branch("ihcal_constituent_pT",&ihcal_constituent_pT); 
	_eval_tree_twr_jets->Branch("ihcal_constituent_pz",&ihcal_constituent_pz); 
	_eval_tree_twr_jets->Branch("ihcal_constituent_phi",&ihcal_constituent_phi); 

	_eval_tree_twr_jets->Branch("ohcal_constituent_E",&ohcal_constituent_E); 
	_eval_tree_twr_jets->Branch("ohcal_constituent_dR",&ohcal_constituent_dR); 
	_eval_tree_twr_jets->Branch("ohcal_constituent_pT",&ohcal_constituent_pT); 
	_eval_tree_twr_jets->Branch("ohcal_constituent_pz",&ohcal_constituent_pz); 
	_eval_tree_twr_jets->Branch("ohcal_constituent_phi",&ohcal_constituent_phi); 

	// Matched Jets

	_eval_tree_matched_jets = new TTree("matched_jets", "FastSim Eval => matched jets");
	_eval_tree_matched_jets->Branch("event", &event, "event/I");
	_eval_tree_matched_jets->Branch("process", &process, "process/I");
	_eval_tree_matched_jets->Branch("n_mpi", &n_mpi, "n_mpi/I");
	_eval_tree_matched_jets->Branch("femc_sumE", &femc_sumE, "femc_sumE/F");
	_eval_tree_matched_jets->Branch("fhcal_sumE", &fhcal_sumE, "fhcal_sumE/F");
	_eval_tree_matched_jets->Branch("ihcal_sumE", &ihcal_sumE, "ihcal_sumE/F");
	_eval_tree_matched_jets->Branch("ohcal_sumE", &ohcal_sumE, "ohcal_sumE/F");
	_eval_tree_matched_jets->Branch("cemc_sumE", &cemc_sumE, "cemc_sumE/F");
	_eval_tree_matched_jets->Branch("vtx_x", &vtx_x, "vtx_x/F");
	_eval_tree_matched_jets->Branch("vtx_y", &vtx_y, "vtx_y/F");
	_eval_tree_matched_jets->Branch("vtx_z", &vtx_z, "vtx_z/F");
	_eval_tree_matched_jets->Branch("vtx_t", &vtx_t, "vtx_t/F");
	_eval_tree_matched_jets->Branch("x1", &x1, "x1/F");
	_eval_tree_matched_jets->Branch("x2", &x2, "x2/F");
	_eval_tree_matched_jets->Branch("Q2", &Q2, "Q2/F");	

	_eval_tree_matched_jets->Branch("jet_p_ptot", &jet_p_ptot, "jet_p_ptot/F");
	_eval_tree_matched_jets->Branch("jet_trk_ptot", &jet_trk_ptot, "jet_trk_ptot/F");
	_eval_tree_matched_jets->Branch("jet_twr_ptot", &jet_twr_ptot, "jet_twr_ptot/F");
	_eval_tree_matched_jets->Branch("jet_p_pT", &jet_p_pT, "jet_p_pT/F");
	_eval_tree_matched_jets->Branch("jet_trk_pT", &jet_trk_pT, "jet_trk_pT/F");
	_eval_tree_matched_jets->Branch("jet_twr_pT", &jet_twr_pT, "jet_twr_pT/F");
	_eval_tree_matched_jets->Branch("jet_p_eta", &jet_p_eta, "jet_p_eta/F");
	_eval_tree_matched_jets->Branch("jet_trk_eta", &jet_trk_eta, "jet_trk_eta/F");
	_eval_tree_matched_jets->Branch("jet_twr_eta", &jet_twr_eta, "jet_twr_eta/F");
	_eval_tree_matched_jets->Branch("jet_p_phi", &jet_p_phi, "jet_p_phi/F");
	_eval_tree_matched_jets->Branch("jet_trk_phi", &jet_trk_phi, "jet_trk_phi/F");
	_eval_tree_matched_jets->Branch("jet_twr_phi", &jet_twr_phi, "jet_twr_phi/F");

	_eval_tree_matched_jets->Branch("jet_p_emfract", &jet_p_emfract, "jet_p_emfract/F");
       
	_eval_tree_matched_jets->Branch("jet_p_nconstit", &jet_p_nconstit, "jet_p_nconstit/I");
	_eval_tree_matched_jets->Branch("jet_trk_nconstit", &jet_trk_nconstit, "jet_trk_nconstit/I");
	_eval_tree_matched_jets->Branch("jet_twr_nconstit", &jet_twr_nconstit, "jet_twr_nconstit/I");

	_eval_tree_matched_jets->Branch("jet_trk_dR", &jet_trk_dR, "jet_trk_dR/F");
	_eval_tree_matched_jets->Branch("jet_twr_dR", &jet_twr_dR, "jet_twr_dR/F");
	
	_eval_tree_matched_jets->Branch("pg_prim_id",&pg_prim_id); 
	_eval_tree_matched_jets->Branch("pg_prim_fract",&pg_prim_fract); 
	_eval_tree_matched_jets->Branch("pg_prim_status",&pg_prim_status); 

	_eval_tree_matched_jets->Branch("pg_trk_id",&pg_trk_id); 
	_eval_tree_matched_jets->Branch("pg_trk_fract",&pg_trk_fract); 
	_eval_tree_matched_jets->Branch("pg_trk_status",&pg_trk_status); 

	_eval_tree_matched_jets->Branch("pg_twr_id",&pg_twr_id); 
	_eval_tree_matched_jets->Branch("pg_twr_fract",&pg_twr_fract); 
	_eval_tree_matched_jets->Branch("pg_twr_status",&pg_twr_status); 

	_eval_tree_matched_jets->Branch("prim_z_lead", &prim_z_lead, "prim_z_lead/F");
	_eval_tree_matched_jets->Branch("trk_z_lead", &trk_z_lead, "trk_z_lead/F");
	_eval_tree_matched_jets->Branch("twr_z_lead", &twr_z_lead, "twr_z_lead/F");
	_eval_tree_matched_jets->Branch("prim_charge_lead", &prim_charge_lead, "prim_charge_lead/I");
	_eval_tree_matched_jets->Branch("trk_charge_lead", &trk_charge_lead, "trk_charge_lead/I");
	_eval_tree_matched_jets->Branch("twr_charge_lead", &twr_charge_lead, "twr_charge_lead/I");

	_eval_tree_matched_jets->Branch("jet_comb_ptot", &jet_comb_ptot, "jet_comb_ptot/F");
	_eval_tree_matched_jets->Branch("jet_comb_pT", &jet_comb_pT, "jet_comb_pT/F");
	_eval_tree_matched_jets->Branch("jet_comb_eta", &jet_comb_eta, "jet_comb_eta/F");
	_eval_tree_matched_jets->Branch("jet_comb_phi", &jet_comb_phi, "jet_comb_phi/F");
	_eval_tree_matched_jets->Branch("jet_comb_neutral_fract", &jet_comb_neutral_fract, "jet_comb_neutral_fract/F");

	_eval_tree_matched_jets->Branch("jet_had_Z",&hadZ); 
	_eval_tree_matched_jets->Branch("jet_had_Ztrue",&hadZtrue); 

	_eval_tree_matched_jets->Branch("jet_comb_bh_ptot", &jet_comb_bh_ptot, "jet_comb_bh_ptot/F");
	_eval_tree_matched_jets->Branch("jet_comb_bh_pT", &jet_comb_bh_pT, "jet_comb_bh_pT/F");
	_eval_tree_matched_jets->Branch("jet_comb_bh_eta", &jet_comb_bh_eta, "jet_comb_bh_eta/F");
	_eval_tree_matched_jets->Branch("jet_comb_bh_phi", &jet_comb_bh_phi, "jet_comb_bh_phi/F");

	_eval_tree_matched_jets->Branch("prim_constituent_E",&prim_constituent_E); 
	_eval_tree_matched_jets->Branch("prim_constituent_dR",&prim_constituent_dR); 
	_eval_tree_matched_jets->Branch("prim_constituent_pT",&prim_constituent_pT); 
	_eval_tree_matched_jets->Branch("prim_constituent_pz",&prim_constituent_pz); 
	_eval_tree_matched_jets->Branch("prim_constituent_phi",&prim_constituent_phi); 

	_eval_tree_matched_jets->Branch("trk_constituent_E",&trk_constituent_E); 
	_eval_tree_matched_jets->Branch("trk_constituent_dR",&trk_constituent_dR); 
	_eval_tree_matched_jets->Branch("trk_constituent_pT",&trk_constituent_pT); 
	_eval_tree_matched_jets->Branch("trk_constituent_pz",&trk_constituent_pz); 
	_eval_tree_matched_jets->Branch("trk_constituent_phi",&trk_constituent_phi); 

	_eval_tree_matched_jets->Branch("cemc_constituent_E",&cemc_constituent_E); 
	_eval_tree_matched_jets->Branch("cemc_constituent_dR",&cemc_constituent_dR); 
	_eval_tree_matched_jets->Branch("cemc_constituent_pT",&cemc_constituent_pT); 
	_eval_tree_matched_jets->Branch("cemc_constituent_pz",&cemc_constituent_pz); 
	_eval_tree_matched_jets->Branch("cemc_constituent_phi",&cemc_constituent_phi); 

	_eval_tree_matched_jets->Branch("ihcal_constituent_E",&ihcal_constituent_E); 
	_eval_tree_matched_jets->Branch("ihcal_constituent_dR",&ihcal_constituent_dR); 
	_eval_tree_matched_jets->Branch("ihcal_constituent_pT",&ihcal_constituent_pT); 
	_eval_tree_matched_jets->Branch("ihcal_constituent_pz",&ihcal_constituent_pz); 
	_eval_tree_matched_jets->Branch("ihcal_constituent_phi",&ihcal_constituent_phi); 

	_eval_tree_matched_jets->Branch("ohcal_constituent_E",&ohcal_constituent_E); 
	_eval_tree_matched_jets->Branch("ohcal_constituent_dR",&ohcal_constituent_dR); 
	_eval_tree_matched_jets->Branch("ohcal_constituent_pT",&ohcal_constituent_pT); 
	_eval_tree_matched_jets->Branch("ohcal_constituent_pz",&ohcal_constituent_pz); 
	_eval_tree_matched_jets->Branch("ohcal_constituent_phi",&ohcal_constituent_phi); 

	return Fun4AllReturnCodes::EVENT_OK;
}

int FastTrackingEval::InitRun(PHCompositeNode *topNode) {

	// Get the CEMC calorimeter radius

	_cemc_radius = -9999.0; 
	RawTowerGeomContainer *geo_cemc = findNode::getClass<RawTowerGeomContainer>(topNode,"TOWERGEOM_CEMC");
	if (geo_cemc) {  
	  _cemc_radius = geo_cemc->get_radius();
	}
	else{
	  cerr << PHWHERE << " ERROR: Can't find node TOWERGEOM_CEMC"  << endl;
	}

	// Get the IHCAL calorimeter radius

	_ihcal_radius = -9999.0;
	if(use_ihcal){
	  RawTowerGeomContainer *geo_ihc = findNode::getClass<RawTowerGeomContainer>(topNode,"TOWERGEOM_HCALIN");
	  if (geo_ihc) {  
	    _ihcal_radius = geo_ihc->get_radius();
	  }
	  else{
	    cerr << PHWHERE << " ERROR: Can't find node TOWERGEOM_HCALIN"  << endl;
	  }
	}

	// Get the OHCAL calorimeter radius

	_ohcal_radius = -9999.0; 
	RawTowerGeomContainer *geo_ohc = findNode::getClass<RawTowerGeomContainer>(topNode,"TOWERGEOM_HCALOUT");
	if (geo_ohc) {  
	  _ohcal_radius = geo_ohc->get_radius();
	}
	else{
	  cerr << PHWHERE << " ERROR: Can't find node TOWERGEOM_HCALOUT"  << endl;
	}
	
	return Fun4AllReturnCodes::EVENT_OK;

}

//----------------------------------------------------------------------------//
//-- process_event():
//--   Call user instructions for every event.
//--   This function contains the analysis structure.
//----------------------------------------------------------------------------//
int FastTrackingEval::process_event(PHCompositeNode *topNode) {
	_event++;
	if (verbosity >= 2 and _event % 1000 == 0)
		cout << PHWHERE << "Events processed: " << _event << endl;

	GetNodes(topNode);

	fill_tree(topNode);

	return Fun4AllReturnCodes::EVENT_OK;
}

//----------------------------------------------------------------------------//
//-- End():
//--   End method, wrap everything up
//----------------------------------------------------------------------------//
int FastTrackingEval::End(PHCompositeNode *topNode) {

	PHTFileServer::get().cd(_outfile_name);

	_eval_tree_tracks->Write();

	//_eval_tree_pairs->Write(); 

	_eval_tree_event->Write();

	_eval_tree_prim_jets->Write();

	_eval_tree_trk_jets->Write();

	_eval_tree_twr_jets->Write();

	_eval_tree_matched_jets->Write();

	//PHTFileServer::get().close();

	delete rand; 

	if(svtx_eval) delete svtx_eval; 

	return Fun4AllReturnCodes::EVENT_OK;
}

//----------------------------------------------------------------------------//
//-- fill_tree():
//--   Fill the various trees...
//----------------------------------------------------------------------------//
void FastTrackingEval::fill_tree(PHCompositeNode *topNode) {
	
  // Fill the calorimeter summed energy variables
  event = _event;
  femc_sumE = fhcal_sumE = ihcal_sumE = ohcal_sumE = cemc_sumE = 0.0; 
  femc_Ntwr = fhcal_Ntwr = ihcal_Ntwr = ohcal_Ntwr = cemc_Ntwr = 0.0; 
  //sumTowerEnergy( topNode, "FEMC" ); 	
  //sumTowerEnergy( topNode, "FHCAL" ); 	
  if(use_ihcal){
    sumTowerEnergy( topNode, "HCALIN" );
  }
  else{
    ihcal_sumE = 0.0;
    ihcal_Ntwr = 0; 
  }
  sumTowerEnergy( topNode, "HCALOUT" ); 	
  sumTowerEnergy( topNode, "CEMC" ); 	
  fwd_ntrk = _trackmap->size(); 

  // Identify the primary vertex 

  PHG4VtxPoint *vtxPoint = _truth_container->GetPrimaryVtx(_truth_container->GetPrimaryVertexIndex());
  vtx_x =  vtxPoint->get_x(); 
  vtx_y =  vtxPoint->get_y(); 
  vtx_z =  vtxPoint->get_z(); 
  vtx_t =  vtxPoint->get_t(); 

  // Get information about the primary event
	
  process = -1;
  //HepMC::GenParticle *electron = NULL; 
  //HepMC::GenParticle *positron = NULL; 

  PHNodeIterator iter(topNode);
  PHHepMCGenEvent *genevent = findNode::getClass<PHHepMCGenEvent>(topNode,"PHHepMCGenEvent");
  if(genevent){

    HepMC::GenEvent *evt = genevent->getEvent();
    process = evt->signal_process_id(); 
    n_mpi = evt->mpi(); 

    const HepMC::PdfInfo *pdfinfo = evt->pdf_info(); 
    if(pdfinfo){
      x1 = pdfinfo->x1();
      x2 = pdfinfo->x2(); 
      Q2 = pdfinfo->scalePDF();
    }
    else{
      x1 = -9999.0; 
      x2 = -9999.0; 
      Q2 = -9999.0; 
    }

    // For DY events identify the e+/e- pair
    // if(process == 1){
    //   // Let's look at the events: 
    //   // Identify the first photon/Z0 with status=2
    //   // The next two particles are the e+/e-
    //   HepMC::GenEvent::particle_iterator pitr = evt->particles_begin();
    //   HepMC::GenEvent::particle_iterator evtStart; 
    //   for( ; pitr!=evt->particles_end(); pitr++){
    // 	HepMC::GenParticle *ptcle = *pitr; 
    // 	//if( ((ptcle->pdg_id()==22) || (ptcle->pdg_id()==23)) && (ptcle->status()==2)){
    // 	// Virtual Z0 when Z0/gamma mixing enabled in pythia (default)
    // 	if( (ptcle->pdg_id()==23) && (ptcle->status()==2)){
	      
    // 	  vpart = ptcle->pdg_id(); 

    // 	  // Save the start of the event (virtual photon/Z0)
    // 	  evtStart = pitr; 

    // 	  // now grab electron/positron
    // 	  int nchk = 0; 
    // 	  do{
    // 	    pitr++; 
    // 	    ptcle = *pitr;
	      
    // 	    if(ptcle->pdg_id()==11)
    // 	      electron = ptcle; 
    // 	    else if(ptcle->pdg_id()==-11)
    // 	      positron = ptcle; 
		 
    // 	    nchk++; 

    // 	  }while(((!positron) || (!electron)) && (nchk<10)); 

    // 	  break; 

    // 	}
    //   }

    //   if( (!electron) || (!positron) ) {

    // 	std::cout << PHWHERE << "ERROR: DY event, but positron and electron not identified! " << std::endl;

    // 	// Dump the event
    // 	HepMC::GenParticle *ptcle = *evtStart; 
    // 	ptcle->print();
    // 	for(int i=0; i<10; i++){
    // 	  evtStart++; 
    // 	  ptcle = *evtStart;
    // 	  ptcle->print();
    // 	}	    
    // 	exit(-1); 

    //   }


    // }

    // If this is a DY event, check to see if the pair is in 
    // the acceptance

    // pairAccepted = 0; 
    // if( (process==1) && electron && positron){
    //   if((electron->momentum().eta()>-1.0) && (electron->momentum().eta()<1.0) &&
    // 	 (positron->momentum().eta()>-1.0) && (positron->momentum().eta()<1.0) )
    // 	pairAccepted = 1;
    //   else
    // 	pairAccepted = 0; 
    // }

	  	
  }

  _eval_tree_event->Fill();
  
  // Di-electron Pairs
  //GetDiElectronPairs(topNode, electron, positron); 

  // Get the PRIMARY Jets

  std::vector<fastjet::PseudoJet> primary_jets;
  GetPrimaryJets(topNode, primary_jets); 

  // Get the TRACK Jets

  std::vector<fastjet::PseudoJet> trk_inclusive_jets;
  GetTrackJets(topNode, trk_inclusive_jets); 

  // Get the CALO Tower Jets

  std::vector<fastjet::PseudoJet> twr_inclusive_jets;
  GetTowerJets(topNode, twr_inclusive_jets); 

  // Combine the jets - look for a triplet match of track, tower and primary jets:

  RawTowerDefs::CalorimeterId cemc_calo_id_ = RawTowerDefs::convert_name_to_caloid( "CEMC" );   
  RawTowerDefs::CalorimeterId ihcal_calo_id_ = RawTowerDefs::convert_name_to_caloid( "HCALIN" );   
  RawTowerDefs::CalorimeterId ohcal_calo_id_ = RawTowerDefs::convert_name_to_caloid( "HCALOUT" );   

  if((prim_jets.size()>0) && (trk_jets.size()>0) && (twr_jets.size()>0)) {

    for (unsigned int iprim = 0; iprim < prim_jets.size(); ++iprim) {

      // Match to a TRACK jet
      float _dR1 = 9999.0;
      int imatch1 = -1; 
      for (unsigned int itrk = 0; itrk < trk_jets.size(); ++itrk) {

  	float dR = sqrt( pow(trk_jets[itrk].jet->eta()-prim_jets[iprim].jet->eta(),2) + 
  			 pow(DeltaPhi(trk_jets[itrk].jet->phi_02pi(), prim_jets[iprim].jet->phi_02pi()),2) ); 
  	if(dR<_dR1){
  	  _dR1 = dR; 
  	  imatch1 = itrk; 
  	}
      }

      if( (imatch1<0) || (_dR1>=jetMatchDR) ) continue; 
      
      // Match to a TOWER jet
      float _dR2 = 9999.0;
      int imatch2 = -1; 
      for (unsigned int itwr = 0; itwr < twr_jets.size(); ++itwr) {

  	float dR = sqrt( pow(twr_jets[itwr].jet->eta()-prim_jets[iprim].jet->eta(),2) + 
  			 pow(DeltaPhi(twr_jets[itwr].jet->phi_02pi(), prim_jets[iprim].jet->phi_02pi()),2) ); 
  	if(dR<_dR2){
  	  _dR2 = dR; 
  	  imatch2 = itwr; 
  	}
      }

      if( (imatch2<0) || (_dR2>=jetMatchDR) ) continue;
      
      // We have a match!!

      jet_p_ptot = sqrt(pow(prim_jets[iprim].jet->px(),2) + 
  		      pow(prim_jets[iprim].jet->py(),2) + 
  		      pow(prim_jets[iprim].jet->pz(),2));
      jet_trk_ptot = sqrt(pow(trk_jets[imatch1].jet->px(),2) + 
  		      pow(trk_jets[imatch1].jet->py(),2) + 
 		      pow(trk_jets[imatch1].jet->pz(),2));
      jet_twr_ptot = sqrt(pow(twr_jets[imatch2].jet->px(),2) + 
  		      pow(twr_jets[imatch2].jet->py(),2) + 
  		      pow(twr_jets[imatch2].jet->pz(),2));
  
      jet_p_pT = sqrt(pow(prim_jets[iprim].jet->px(),2) + 
  		      pow(prim_jets[iprim].jet->py(),2));
      jet_trk_pT = sqrt(pow(trk_jets[imatch1].jet->px(),2) + 
  		      pow(trk_jets[imatch1].jet->py(),2));
      jet_twr_pT = sqrt(pow(twr_jets[imatch2].jet->px(),2) + 
  		      pow(twr_jets[imatch2].jet->py(),2));

      jet_p_eta = prim_jets[iprim].jet->eta(); 
      jet_trk_eta = trk_jets[imatch1].jet->eta(); 
      jet_twr_eta = twr_jets[imatch2].jet->eta(); 

      jet_p_phi = prim_jets[iprim].jet->phi_02pi(); 
      jet_trk_phi = trk_jets[imatch1].jet->phi_02pi(); 
      jet_twr_phi = twr_jets[imatch2].jet->phi_02pi(); 

      jet_p_emfract = prim_jets[iprim].emfract; 

      jet_p_nconstit = prim_jets[iprim].constituents.size();
      jet_trk_nconstit = trk_jets[imatch1].constituents.size();  
      jet_twr_nconstit = twr_jets[imatch2].constituents.size(); 

      jet_trk_dR = _dR1; 
      jet_twr_dR = _dR2; 

      prim_z_lead = prim_jets[iprim].z_lead; 
      trk_z_lead = trk_jets[imatch1].z_lead; 
      twr_z_lead = twr_jets[imatch2].z_lead; 
      prim_charge_lead = prim_jets[iprim].charge_lead;
      trk_charge_lead = trk_jets[imatch1].charge_lead;
      twr_charge_lead = trk_jets[imatch2].charge_lead;

      pg_prim_id.clear();
      pg_prim_fract.clear(); 
      pg_prim_status.clear(); 
      for(unsigned int ip=0; ip<prim_jets[iprim].progenitors.size(); ip++){
	pg_prim_id.push_back((int)prim_jets[iprim].progenitors[ip].pid); 
	pg_prim_fract.push_back((double)(prim_jets[iprim].progenitors[ip].energy/prim_jets[iprim].jet->E())); 
	pg_prim_status.push_back(prim_jets[iprim].progenitors[ip].status); 
      }

      pg_trk_id.clear();
      pg_trk_fract.clear(); 
      pg_trk_status.clear(); 
      for(unsigned int ip=0; ip<trk_jets[imatch1].progenitors.size(); ip++){
	pg_trk_id.push_back((int)trk_jets[imatch1].progenitors[ip].pid); 
	pg_trk_fract.push_back((double)(trk_jets[imatch1].progenitors[ip].energy/trk_jets[imatch1].jet->E())); 
	pg_trk_status.push_back(trk_jets[imatch1].progenitors[ip].status); 
      }

      pg_twr_id.clear();
      pg_twr_fract.clear(); 
      pg_twr_status.clear(); 
      for(unsigned int ip=0; ip<twr_jets[imatch2].progenitors.size(); ip++){
	pg_twr_id.push_back((int)twr_jets[imatch2].progenitors[ip].pid); 
	pg_twr_fract.push_back((double)(twr_jets[imatch2].progenitors[ip].energy/twr_jets[imatch2].jet->E())); 
	pg_twr_status.push_back(twr_jets[imatch2].progenitors[ip].status); 
      }
      
      // Fill the constituents arrays

      prim_constituent_E.clear(); 
      prim_constituent_dR.clear(); 
      prim_constituent_pT.clear(); 
      prim_constituent_pz.clear(); 
      prim_constituent_phi.clear(); 
      for(unsigned int j=0; j<prim_jets[iprim].constituents.size(); j++){
	prim_constituent_E.push_back(prim_jets[iprim].constituents[j].E()); 
	float cdphi = DeltaPhi(prim_jets[iprim].constituents[j].phi_02pi(),jet_p_phi); 
	prim_constituent_dR.push_back(sqrt( pow(prim_jets[iprim].constituents[j].eta()-jet_p_eta,2) + pow(cdphi,2) )); 
	prim_constituent_pT.push_back(sqrt(pow(prim_jets[iprim].constituents[j].px(),2)+pow(prim_jets[iprim].constituents[j].py(),2)));
	prim_constituent_pz.push_back(prim_jets[iprim].constituents[j].pz());
	prim_constituent_phi.push_back(prim_jets[iprim].constituents[j].phi_std());
      }

      trk_constituent_E.clear(); 
      trk_constituent_dR.clear(); 
      trk_constituent_pT.clear(); 
      trk_constituent_pz.clear(); 
      trk_constituent_phi.clear(); 
      for(unsigned int j=0; j<trk_jets[imatch1].constituents.size(); j++){
	trk_constituent_E.push_back(trk_jets[imatch1].constituents[j].E()); 
	float cdphi = DeltaPhi(trk_jets[imatch1].constituents[j].phi_02pi(),jet_trk_phi); 
	trk_constituent_dR.push_back(sqrt( pow(trk_jets[imatch1].constituents[j].eta()-jet_trk_eta,2) + pow(cdphi,2) )); 
	trk_constituent_pT.push_back(sqrt(pow(trk_jets[imatch1].constituents[j].px(),2)+pow(trk_jets[imatch1].constituents[j].py(),2)));
	trk_constituent_pz.push_back(trk_jets[imatch1].constituents[j].pz());
	trk_constituent_phi.push_back(trk_jets[imatch1].constituents[j].phi_std());
      }

      cemc_constituent_E.clear(); 
      cemc_constituent_dR.clear(); 
      cemc_constituent_pT.clear(); 
      cemc_constituent_pz.clear(); 
      cemc_constituent_phi.clear(); 
      ihcal_constituent_E.clear(); 
      ihcal_constituent_dR.clear(); 
      ihcal_constituent_pT.clear(); 
      ihcal_constituent_pz.clear(); 
      ihcal_constituent_phi.clear(); 
      ohcal_constituent_E.clear(); 
      ohcal_constituent_dR.clear(); 
      ohcal_constituent_pT.clear(); 
      ohcal_constituent_pz.clear(); 
      ohcal_constituent_phi.clear(); 
      for(unsigned int j=0; j<twr_jets[imatch2].constituents.size(); j++){
      	std::string cname = 
      	  RawTowerDefs::convert_caloid_to_name( 
	     RawTowerDefs::decode_caloid( (unsigned int)twr_jets[imatch2].constituents[j].user_index() ));
      	float cdphi = DeltaPhi(twr_jets[imatch2].constituents[j].phi_02pi(),jet_twr_phi); 
      	if(cname == "CEMC"){
      	  cemc_constituent_E.push_back(twr_jets[imatch2].constituents[j].E()); 
      	  cemc_constituent_dR.push_back(sqrt( pow(twr_jets[imatch2].constituents[j].eta()-jet_twr_eta,2) + pow(cdphi,2) )); 
	  cemc_constituent_pT.push_back(sqrt(pow(twr_jets[imatch2].constituents[j].px(),2)+pow(twr_jets[imatch2].constituents[j].py(),2)));
	  cemc_constituent_pz.push_back(twr_jets[imatch2].constituents[j].pz());
	  cemc_constituent_phi.push_back(twr_jets[imatch2].constituents[j].phi_std());
      	}
      	else if(cname == "HCALIN"){
      	  ihcal_constituent_E.push_back(twr_jets[imatch2].constituents[j].E()); 
      	  ihcal_constituent_dR.push_back(sqrt( pow(twr_jets[imatch2].constituents[j].eta()-jet_twr_eta,2) + pow(cdphi,2) ));       
	  ihcal_constituent_pT.push_back(sqrt(pow(twr_jets[imatch2].constituents[j].px(),2)+pow(twr_jets[imatch2].constituents[j].py(),2)));
	  ihcal_constituent_pz.push_back(twr_jets[imatch2].constituents[j].pz());
	  ihcal_constituent_phi.push_back(twr_jets[imatch2].constituents[j].phi_std());
      	}
      	else if(cname == "HCALOUT"){
      	  ohcal_constituent_E.push_back(twr_jets[imatch2].constituents[j].E()); 
      	  ohcal_constituent_dR.push_back(sqrt( pow(twr_jets[imatch2].constituents[j].eta()-jet_twr_eta,2) + pow(cdphi,2) ));       
	  ohcal_constituent_pT.push_back(sqrt(pow(twr_jets[imatch2].constituents[j].px(),2)+pow(twr_jets[imatch2].constituents[j].py(),2)));
	  ohcal_constituent_pz.push_back(twr_jets[imatch2].constituents[j].pz());
	  ohcal_constituent_phi.push_back(twr_jets[imatch2].constituents[j].phi_std());
      	}
      }

      // Loop over the track jet constituents, account for that track's energy
      // contribution to the tower jet, and adjust the jet energy and momentum
      // based on the combination. 
      
      float combJetPx = 0.0; 
      float combJetPy = 0.0; 
      float combJetPz = 0.0; 

      float twrJetPx = twr_jets[imatch2].jet->px(); 
      float twrJetPy = twr_jets[imatch2].jet->py(); 
      float twrJetPz = twr_jets[imatch2].jet->pz(); 

      for(unsigned int itrk=0; itrk<trk_jets[imatch1].constituents.size(); itrk++){

        PHG4Particle* con_g4particle = NULL; 
	SvtxTrack* track = NULL; 
        for (SvtxTrackMap::ConstIter track_itr = _trackmap->begin();
	     track_itr != _trackmap->end(); track_itr++) {
	  SvtxTrack* temp = dynamic_cast<SvtxTrack*>(track_itr->second);
	  PHG4Particle* g4particle = svtx_eval->max_truth_particle_by_nclusters(temp);
	  if(!g4particle) continue; 
	  if ( g4particle->get_track_id() == trk_jets[imatch1].constituents[itrk].user_index() ) {
	    track = temp; 
	    con_g4particle = g4particle;
	    break;
	  }
	}
	  
	if(!track) continue; // bail if constituent wasn't found
	if(!con_g4particle) continue; 

	// Now collect the energy from this particle that is contained
	// in the constituent towers that make up the tower jet. 
	
	float cemc_twrJetContrib_px = 0.0; 
	float cemc_twrJetContrib_py = 0.0; 
	float cemc_twrJetContrib_pz = 0.0; 

	float ihcal_twrJetContrib_px = 0.0; 
	float ihcal_twrJetContrib_py = 0.0; 
	float ihcal_twrJetContrib_pz = 0.0; 

	float ohcal_twrJetContrib_px = 0.0; 
	float ohcal_twrJetContrib_py = 0.0; 
	float ohcal_twrJetContrib_pz = 0.0; 
	    
	std::vector<ParticleTower *>::iterator it;
	for (it = calo_incident.begin(); it != calo_incident.end(); ++it){
	
	  PHG4Particle *incident_g4particle = (*it)->particle; 
	  
	  if(con_g4particle->get_track_id()==incident_g4particle->get_track_id()) {

	    // Particle is in incident list - collect the momentum it 
	    // contributed to the tower jet
	    
	    int tsize = (*it)->TowerIDs.size(); 
	    for(int itid = 0; itid < tsize; itid++){
	      for(unsigned int itwr=0; itwr<twr_jets[imatch2].constituents.size(); itwr++){
		if( (unsigned int) twr_jets[imatch2].constituents[itwr].user_index()==(*it)->TowerIDs[itid] ){
		  
		  // cout << "Tower match user_index = " << twr_jets[imatch2].constituents[itwr].user_index() 
		  //      << " ID=" << (*it)->TowerIDs[itid] << endl; 

		  unsigned int calo_id_ = RawTowerDefs::decode_caloid((*it)->TowerIDs[itid]); 
		  
		  if(calo_id_==cemc_calo_id_){
		    cemc_twrJetContrib_px += (*it)->px[itid]; 
		    cemc_twrJetContrib_py += (*it)->py[itid]; 
		    cemc_twrJetContrib_pz += (*it)->pz[itid];
		  }
		  else if(calo_id_==ihcal_calo_id_){
		    ihcal_twrJetContrib_px += (*it)->px[itid]; 
		    ihcal_twrJetContrib_py += (*it)->py[itid]; 
		    ihcal_twrJetContrib_pz += (*it)->pz[itid];
		  }
		  else if(calo_id_==ohcal_calo_id_){
		    ohcal_twrJetContrib_px += (*it)->px[itid]; 
		    ohcal_twrJetContrib_py += (*it)->py[itid]; 
		    ohcal_twrJetContrib_pz += (*it)->pz[itid];
		  }

		}
	      }
	    }

	  }

	}

	// Now - we need to decide whether we will keep the track energy and remove the tower energy, 
	// or vice-versa

	// Is this an EM or HADRONIC shower? 
	
	float cemc_twrJetContrib_p = sqrt(pow(cemc_twrJetContrib_px,2) + 
					  pow(cemc_twrJetContrib_py,2) + 
					  pow(cemc_twrJetContrib_pz,2) ); 

	float ihcal_twrJetContrib_p = sqrt(pow(ihcal_twrJetContrib_px,2) + 
					   pow(ihcal_twrJetContrib_py,2) + 
					   pow(ihcal_twrJetContrib_pz,2) ); 

	float ohcal_twrJetContrib_p = sqrt(pow(ohcal_twrJetContrib_px,2) + 
					   pow(ohcal_twrJetContrib_py,2) + 
					   pow(ohcal_twrJetContrib_pz,2) ); 

	float twr_ptot = sqrt(pow(cemc_twrJetContrib_px+ihcal_twrJetContrib_px+ohcal_twrJetContrib_px,2) + 
			      pow(cemc_twrJetContrib_py+ihcal_twrJetContrib_py+ohcal_twrJetContrib_py,2) + 
			      pow(cemc_twrJetContrib_pz+ihcal_twrJetContrib_pz+ohcal_twrJetContrib_pz,2) ); 
	
	float trk_ptot = sqrt( pow(track->get_px(),2) + pow(track->get_py(),2) + pow(track->get_pz(),2) );  
	float trk_pt = sqrt( pow(track->get_px(),2) + pow(track->get_py(),2) );  
	

	//cout << "trk_ptot = " << trk_ptot << " twr_ptot = " << twr_ptot << " ratio = " << twr_ptot/trk_ptot << endl; 

	// Momentum Resolution

        bool twr_preferred = false; 
	float delta_pT = 0.005 + (0.001*trk_pt); 
	if( (cemc_twrJetContrib_p/(cemc_twrJetContrib_p+ihcal_twrJetContrib_p+ohcal_twrJetContrib_p)) > 0.9 ){
	  // EM shower
	  if(delta_pT>(0.12/sqrt(twr_ptot))) twr_preferred = true; 
	}
	else{
	  // HADRONIC shower 
	  if(delta_pT>sqrt(pow(0.7/sqrt(twr_ptot),2)+pow(0.15,2))) twr_preferred = true; 
	}

	// Eliminate the tower jet contribution from the tower jet total
	twrJetPx -= (cemc_twrJetContrib_px + ihcal_twrJetContrib_px + ohcal_twrJetContrib_px); 
	twrJetPy -= (cemc_twrJetContrib_py + ihcal_twrJetContrib_py + ohcal_twrJetContrib_py); 
	twrJetPz -= (cemc_twrJetContrib_pz + ihcal_twrJetContrib_pz + ohcal_twrJetContrib_pz);
     
	if(twr_preferred){
	  // Rescale the track total momentum to match the tower energy. 
	  // This keeps the better momentum resolution of the calorimetry 
	  // measurement and combines it with the improved eta/phi resolution
	  // of the tracking. 
	  combJetPx += track->get_px()*(twr_ptot/trk_ptot); 
	  combJetPy += track->get_py()*(twr_ptot/trk_ptot); 
	  combJetPz += track->get_pz()*(twr_ptot/trk_ptot); 
	  
	  // reset the constituent momentum - we need the correct 
	  // momentum to get the correct z below. 
	  
	  trk_jets[imatch1].constituents[itrk].reset_momentum(track->get_px()*(twr_ptot/trk_ptot),
							      track->get_py()*(twr_ptot/trk_ptot),
							      track->get_pz()*(twr_ptot/trk_ptot),
							      trk_ptot*(twr_ptot/trk_ptot)); 

	}
	else{
	  // Keep the track contribution as is
	  combJetPx += track->get_px();  
	  combJetPy += track->get_py(); 
	  combJetPz += track->get_pz();
	}

      }

      //cout << " jet combination completed, twrJetPx = " << twrJetPx << " twrJetPy = " 
      //     << twrJetPy << " twrJetPz = " << twrJetPz << endl; 

      // Add back in the remaining neutral energy to the combined jet
      combJetPx += twrJetPx; 
      combJetPy += twrJetPy; 
      combJetPz += twrJetPz; 

      jet_comb_ptot = sqrt( pow(combJetPx,2) + pow(combJetPy,2) + pow(combJetPz,2) ); 
      jet_comb_pT = sqrt( pow(combJetPx,2) + pow(combJetPy,2) ); 
      jet_comb_eta =  getEta(sqrt(pow(combJetPx,2)+pow(combJetPy,2)),combJetPz);  
      jet_comb_neutral_fract = sqrt( pow(twrJetPx,2) + pow(twrJetPy,2) + pow(twrJetPz,2) )/jet_comb_ptot; 
      jet_comb_phi =  XYtoPhi_02PI(combJetPx,combJetPy);  

      // NEXT: We need to loop over all the hadrons in the track jet and get the measured z and 
      // z-resolution

      hadZ.clear(); 
      hadZtrue.clear(); 

      for(unsigned int itrk=0; itrk<trk_jets[imatch1].constituents.size(); itrk++){

        PHG4Particle* con_g4particle = NULL; 
	SvtxTrack* track = NULL;
	for (SvtxTrackMap::ConstIter track_itr = _trackmap->begin();
	     track_itr != _trackmap->end(); track_itr++) {
	  SvtxTrack* temp = dynamic_cast<SvtxTrack*>(track_itr->second);
	  PHG4Particle* g4particle = svtx_eval->max_truth_particle_by_nclusters(temp); 
	  if (g4particle){
	    if ( g4particle->get_track_id() == trk_jets[imatch1].constituents[itrk].user_index() )  {
	      track = temp; 
	      con_g4particle = g4particle;
	      break;
	    }
	  }
	}

	if(!con_g4particle) continue; 
	if(!track) continue; 
	
	// get the measured z and z-resolution

	float meas_z = (track->get_px()*combJetPx + 
			track->get_py()*combJetPy + 
			track->get_pz()*combJetPz)/pow(jet_comb_ptot,2); 

	hadZ.push_back((double) meas_z); 

	float true_z = (con_g4particle->get_px()*prim_jets[iprim].jet->px() + 
			con_g4particle->get_py()*prim_jets[iprim].jet->py() + 
			con_g4particle->get_pz()*prim_jets[iprim].jet->pz())/pow(jet_p_ptot,2); 

	hadZtrue.push_back((double) true_z); 

      }

      // Last step - add in the BH energy

      PHG4HitContainer *_bh_hit_container = findNode::getClass<PHG4HitContainer> (topNode, "G4HIT_BH_1");
      if(_bh_hit_container){
	PHG4HitContainer::ConstRange bh_hit_range = _bh_hit_container->getHits();
	for (PHG4HitContainer::ConstIterator hit_iter = bh_hit_range.first;
	     hit_iter != bh_hit_range.second; hit_iter++){

	  PHG4Hit *this_hit =  hit_iter->second;
	  assert(this_hit); 

	  // Check the BH hit - include it if it is within the jet radius of the 
	  // jet axis

	  float bh_eta = getEta(sqrt(pow(this_hit->get_px(0),2)+pow(this_hit->get_py(0),2)),pow(this_hit->get_pz(0),2)); 
	  float bh_phi = XYtoPhi(this_hit->get_px(0),this_hit->get_py(0)); 
	  float bh_dphi = DeltaPhi(jet_comb_phi,bh_phi); 

	  if( sqrt( pow(jet_comb_eta - bh_eta,2) + pow(bh_dphi,2)) < 2.0*_R){
	    combJetPx += this_hit->get_px(0);  
	    combJetPy += this_hit->get_py(0); 
	    combJetPz += this_hit->get_pz(0);
	  }

	}
      }

      jet_comb_bh_ptot = sqrt( pow(combJetPx,2) + pow(combJetPy,2) + pow(combJetPz,2) ); 
      jet_comb_bh_pT = sqrt( pow(combJetPx,2) + pow(combJetPy,2) ); 
      jet_comb_bh_eta =  getEta(sqrt(pow(combJetPx,2)+pow(combJetPy,2)),combJetPz);  
      jet_comb_bh_phi =  XYtoPhi_02PI(combJetPx,combJetPy);  

      _eval_tree_matched_jets->Fill();

    }

  }

  // Make sure these are clear as the things they point to 
  // will be going out of scope

  prim_jets.clear(); 
  twr_jets.clear(); 
  trk_jets.clear(); 

  // NEED TO DELETE THE CALORIMETER PARTICLE-TOWER MAP

  std::vector<ParticleTower *>::iterator it;
  for (it = calo_incident.begin(); it != calo_incident.end(); ++it){
    ParticleTower *pt = (*it); 
    pt->TowerIDs.clear(); 
    pt->px.clear(); 
    pt->py.clear(); 
    pt->pz.clear(); 
    delete pt; 
  }
  calo_incident.clear();

  return;

}

void FastTrackingEval::GetPrimaryJets(PHCompositeNode *topNode, std::vector<fastjet::PseudoJet> &primary_jets){

  primary_jets.clear(); 

  // get the list of jets from the primary particles

  if (!_truth_container) {
    LogError("_truth_container not found!");
    return;
  }

  std::vector<fastjet::PseudoJet> pseudojets;

  // PRIMARIES ONLY
  PHG4TruthInfoContainer::ConstRange range =
   		_truth_container->GetPrimaryParticleRange();

  for (PHG4TruthInfoContainer::ConstIterator truth_itr = range.first;
       truth_itr != range.second; ++truth_itr) {

    PHG4Particle* g4particle = truth_itr->second;
    if(!g4particle) {
      LogDebug("");
      continue;
    }

    // remove some particles (muons, taus, neutrinos)...
    // 12 == nu_e
    // 13 == muons
    // 14 == nu_mu
    // 15 == taus
    // 16 == nu_tau
    if ((abs(g4particle->get_pid()) >= 12) && (abs( g4particle->get_pid()) <= 16)) continue;
    

    // acceptance... _etamin,_etamax
    if ((g4particle->get_px() == 0.0) && (g4particle->get_px() == 0.0)) continue; // avoid pt=0
    
    TVector3 partMom(g4particle->get_px(),g4particle->get_py(),g4particle->get_pz()); 
    if ( (partMom.Eta() < _theEtaLow) ||
	 (partMom.Eta() > _theEtaHigh)) continue;

    // add this track to the list of tracks for jets

    fastjet::PseudoJet pseudojet (g4particle->get_px(),
				  g4particle->get_py(),
				  g4particle->get_pz(),
				  sqrt(pow(g4particle->get_px(),2)+
				       pow(g4particle->get_py(),2)+
				       pow(g4particle->get_pz(),2)));

    pseudojet.set_user_index(g4particle->get_track_id());
    pseudojets.push_back(pseudojet);

  }

  if(pseudojets.size()>0) { 
    // Call FastJet
    fastjet::JetDefinition *jetdef = new fastjet::JetDefinition(fastjet::antikt_algorithm,_R, fastjet::E_scheme,fastjet::Best);
    fastjet::ClusterSequence jetFinder(pseudojets,*jetdef);
    primary_jets = sorted_by_pt(jetFinder.inclusive_jets(ptmin));
    delete jetdef;

    for (unsigned int ijet = 0; ijet < primary_jets.size(); ++ijet) {

      jet_pT = sqrt(pow(primary_jets[ijet].px(),2) + pow(primary_jets[ijet].py(),2)); 
      jet_ptot = sqrt(pow(primary_jets[ijet].px(),2) + 
		      pow(primary_jets[ijet].py(),2) + 
		      pow(primary_jets[ijet].pz(),2));
      jet_E = primary_jets[ijet].E(); 
      jet_eta = primary_jets[ijet].eta(); 
      jet_rapidity = primary_jets[ijet].rapidity(); 
      jet_mass = primary_jets[ijet].m(); 
      jet_phi = primary_jets[ijet].phi_02pi(); 
      jet_xF = primary_jets[ijet].pz()/p_beam; 
      jet_emfract = 0.0; 

      vector<fastjet::PseudoJet> constituents = primary_jets[ijet].constituents();
      jet_nconstit = constituents.size(); 
      PHG4Particle* con_g4particle = NULL; 

      combined_progenitors.clear();
      z_lead_p = -1.0;
      charge_lead_p = -9999.0; 
      for (unsigned int j=0; j<constituents.size(); j++){
	    
	// Identify it back to the g4particle
	for (PHG4TruthInfoContainer::ConstIterator truth_itr = range.first;
	     truth_itr != range.second; ++truth_itr) {
	  PHG4Particle* g4particle = truth_itr->second;
	  if(!g4particle) continue;
	  if(g4particle->get_track_id() == constituents[j].user_index()) {
	    con_g4particle = g4particle;
	    break; 
	  }
	}

	// Let's see if we can find the constituent's progenitor(s)
	progenitors.clear(); 
	if(con_g4particle) 
	  GetJetPrimaryContributors( topNode, con_g4particle ); 
	else
	  continue; 

	// Sort out the truth EM fraction based on the g4particle type
	// electrons, photons, pizeroes, etas: 
	if( (fabs(con_g4particle->get_pid())==11) || (con_g4particle->get_pid()==22) || 
	    (con_g4particle->get_pid()==111) || (con_g4particle->get_pid()==221) )
	  jet_emfract += sqrt(pow(con_g4particle->get_px(),2)+
			      pow(con_g4particle->get_py(),2)+
			      pow(con_g4particle->get_pz(),2));

	// Add this constituent's progenitors to the list of jet progenitors
	float prog_tot_e = 0.0; 
	for(unsigned int n=0; n<progenitors.size(); n++){
	  prog_tot_e += progenitors[n].pg_energy; 
	}

	for(unsigned int n=0; n<progenitors.size(); n++){
	  // Is this progenitor already in the list? 
	  bool inList = false; 
	  unsigned int match = 0; 
	  for(unsigned int m=0; m<combined_progenitors.size(); m++){
	    if(progenitors[n].barcode == combined_progenitors[m].barcode) {
	      inList = true; 
	      match = m; 
	      break; 
	    }
	  }
	      
	  if(inList){
	    combined_progenitors[match].energy += constituents[j].E()*(progenitors[n].pg_energy/prog_tot_e); 
	  }
	  else{
	    JetProgenitor newProg = progenitors[n]; 
	    newProg.energy = constituents[j].E()*(progenitors[n].pg_energy/prog_tot_e); 
	    combined_progenitors.push_back(newProg); 
	  }
	     
	}

	// Now get the z of the leading constituent. 
	
	float con_ptot = sqrt( pow(con_g4particle->get_px(),2) + 
			       pow(con_g4particle->get_py(),2) + 
			       pow(con_g4particle->get_pz(),2) ); 

	float ctheta = (con_g4particle->get_px()*primary_jets[ijet].px() + 
			con_g4particle->get_py()*primary_jets[ijet].py() + 
			con_g4particle->get_pz()*primary_jets[ijet].pz())/
	               (con_ptot*jet_ptot);

	float z_constit = con_ptot*ctheta/jet_ptot; 

	if(z_constit>z_lead_p) {
	  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	  G4ParticleDefinition* particle = particleTable->FindParticle(con_g4particle->get_name()); 
	  if(particle){
	    charge_lead_p = particle->GetPDGCharge();
	    z_lead_p = z_constit;
	  }
	}

	// Fill the constituents vector
	
	prim_constituent_E.push_back(constituents[j].E()); 
	float cdphi = DeltaPhi(constituents[j].phi_02pi(),jet_phi); 
	prim_constituent_dR.push_back(sqrt( pow(constituents[j].eta()-jet_eta,2) + pow(cdphi,2) )); 
	prim_constituent_pT.push_back(sqrt(pow(constituents[j].px(),2)+pow(constituents[j].py(),2)));
	prim_constituent_pz.push_back(constituents[j].pz());
	prim_constituent_phi.push_back(constituents[j].phi_std());
        prim_constituent_z.push_back(z_constit);

      }

      // Finish the EM fraction

      jet_emfract /= jet_ptot; 
	  
      // Sort from highest energy to lowest energy 
      std::sort(combined_progenitors.begin(), combined_progenitors.end(), sortfunction); 

      pg_prim_id.clear();
      pg_prim_fract.clear(); 
      pg_prim_status.clear(); 
      for(unsigned int ip=0; ip<combined_progenitors.size(); ip++){
	pg_prim_id.push_back((int)combined_progenitors[ip].pid); 
	pg_prim_fract.push_back((double)(combined_progenitors[ip].energy/primary_jets[ijet].E())); 
	pg_prim_status.push_back(combined_progenitors[ip].status); 
      }

      _eval_tree_prim_jets->Fill();

      // Save this jet for later study

      RecoJet newJet; 
      newJet.jet = &primary_jets[ijet]; 
      newJet.progenitors = combined_progenitors;
      newJet.z_lead = z_lead_p; 
      newJet.charge_lead = charge_lead_p; 
      newJet.emfract = jet_emfract; 
      newJet.constituents = primary_jets[ijet].constituents(); 

      prim_jets.push_back(newJet);  
    }

  }


}

void FastTrackingEval::GetTrackJets(PHCompositeNode *topNode, std::vector<fastjet::PseudoJet> &inclusive_jets){

  trk_jets.clear(); 

  // Use the forward track inputs to the jets

  if (!_truth_container) {
    LogError("_truth_container not found!");
    return;
  }

  if (!_trackmap) {
    LogError("_trackmap not found!");
    return;
  }

  std::vector<fastjet::PseudoJet> pseudojets;

  for (SvtxTrackMap::ConstIter track_itr = _trackmap->begin();
       track_itr != _trackmap->end(); track_itr++) {

    SvtxTrack* track = dynamic_cast<SvtxTrack*>(track_itr->second);

    // Run the evaluator to get the origin of this track 
    
    PHG4Particle* g4particle = svtx_eval->max_truth_particle_by_nclusters(track); 

    if(g4particle){
 
      gflavor = g4particle->get_pid();
      gtrackID = g4particle->get_track_id();
      gparentid = GetParticleParentPID(topNode, g4particle); 

      gpx = g4particle->get_px();
      gpy = g4particle->get_py();
      gpz = g4particle->get_pz();

      PHG4VtxPoint *vtx = _truth_container->GetVtx(g4particle->get_vtx_id()); 
      gvx = vtx->get_x(); 
      gvy = vtx->get_y(); 
      gvz = vtx->get_z(); 
	  
      isprim = _truth_container->is_primary(g4particle); 
    
    }
    else{

      gflavor = -9999.0;
      gtrackID = -9999; 
      gparentid = -9999.0; 

      gpx = -9999.0;
      gpy = -9999.0;
      gpz = -9999.0;

      gvx = -9999.0; 
      gvy = -9999.0; 
      gvz = -9999.0; 
	  
      isprim = 0; 

    }

    trackID = track->get_id();
    charge = track->get_charge();
    nhits = track->get_quality();

    px = track->get_px();
    py = track->get_py();
    pz = track->get_pz();
    dca2d = track->get_dca2d();

    chi2 = track->get_chisq(); 
    ndf = track->get_ndf(); 

    // Use the track states to project to the calorimeters and generate 
    // energy sums.
			
    cemc_E33 = track->get_cal_energy_3x3(SvtxTrack::CEMC); 
    cemc_E33_PC = PositionCorrectEMCal3x3(topNode, track->get_cal_cluster_id(SvtxTrack::CEMC), cemc_E33); 
    hcalin_E33 = track->get_cal_energy_3x3(SvtxTrack::HCALIN); 
    hcalout_E33 = track->get_cal_energy_3x3(SvtxTrack::HCALOUT); 

    getClusterByIndex(topNode, "CEMC", track->get_cal_cluster_id(SvtxTrack::CEMC)); 
    getClusterPCByIndex(topNode, track->get_cal_cluster_id(SvtxTrack::CEMC)); 
    if(use_ihcal){
      getClusterByIndex(topNode, "HCALIN", track->get_cal_cluster_id(SvtxTrack::HCALIN));
    }
    else{
      hcalin_cldr = 9999.0;  
      hcalin_clE = 0.0; 
      hcalin_clN = 0.0; 
    }
    getClusterByIndex(topNode, "HCALOUT", track->get_cal_cluster_id(SvtxTrack::HCALOUT)); 

    getClusterDistanceByIndex("CEMC",  track ); 
    getClusterDistanceByIndex("HCALIN",  track ); 
    getClusterDistanceByIndex("HCALOUT",  track ); 

    _eval_tree_tracks->Fill(); 

    // Cuts for "good" tracks
    if((chi2/ndf)>1.5) continue;
    if(ndf<60) continue; // number of hits
    if(fabs(dca2d)>0.1) continue; 
    
    // add this track to the list of tracks for jets

    fastjet::PseudoJet pseudojet (track->get_px(),
				  track->get_py(),
				  track->get_pz(),
				  sqrt(pow(track->get_px(),2)+pow(track->get_py(),2)+pow(track->get_pz(),2)));

    pseudojet.set_user_index(gtrackID);
    pseudojets.push_back(pseudojet);

  }
	
  if(pseudojets.size()>0) { 

    // Call FastJet
    // Sort out the found jets, add in the neutral calorimeter energy

    fastjet::JetDefinition *jetdef = new fastjet::JetDefinition(fastjet::antikt_algorithm,_R, fastjet::E_scheme,fastjet::Best);
    fastjet::ClusterSequence jetFinder(pseudojets,*jetdef);
    //std::vector<fastjet::PseudoJet> inclusive_jets = sorted_by_pt(jetFinder.inclusive_jets(ptmin)); 
    inclusive_jets.clear(); 
    inclusive_jets = sorted_by_pt(jetFinder.inclusive_jets(ptmin)); 
    delete jetdef;

    for (unsigned int ijet = 0; ijet < inclusive_jets.size(); ++ijet) {

      //std::cout << " FOUND A JET! " << std::endl; 

      jet_pT = sqrt(pow(inclusive_jets[ijet].px(),2) + pow(inclusive_jets[ijet].py(),2)); 
      jet_ptot = sqrt(pow(inclusive_jets[ijet].px(),2) + 
		      pow(inclusive_jets[ijet].py(),2) + 
		      pow(inclusive_jets[ijet].pz(),2));
      jet_tkE = inclusive_jets[ijet].E(); 
      jet_eta = inclusive_jets[ijet].eta(); 
      jet_rapidity = inclusive_jets[ijet].rapidity(); 
      jet_mass = inclusive_jets[ijet].m(); 
      jet_phi = inclusive_jets[ijet].phi_02pi(); 
      jet_xF = inclusive_jets[ijet].pz()/p_beam; 

      trk_constituent_E.clear();
      trk_constituent_dR.clear();
      trk_constituent_pT.clear();
      trk_constituent_pz.clear();
      trk_constituent_phi.clear();
      trk_constituent_z.clear();

      vector<fastjet::PseudoJet> constituents = inclusive_jets[ijet].constituents();

      jet_nconstit = constituents.size(); 

      combined_progenitors.clear();
      z_lead = -1.0;
      charge_lead = -9999.0; 
      for (unsigned int j=0; j<constituents.size(); j++){
	    
	// Identify it back to the g4particle
        PHG4Particle* con_g4particle = NULL; 
	PHG4TruthInfoContainer::ConstRange range = _truth_container->GetParticleRange();
	for (PHG4TruthInfoContainer::ConstIterator truth_itr = range.first;
	     truth_itr != range.second; ++truth_itr) {
	  PHG4Particle* g4particle = truth_itr->second;
	  if(!g4particle) continue;
	  if(g4particle->get_track_id() == constituents[j].user_index()) {
	    con_g4particle = g4particle;
	    break; 
	  }
	}

	// Let's see if we can find the constituent's progenitor(s)
	progenitors.clear(); 
	if(con_g4particle) 
	  GetJetPrimaryContributors( topNode, con_g4particle ); 
	else
	  continue; 

	// Add this constituent's progenitors to the list of jet progenitors
	float prog_tot_e = 0.0; 
	for(unsigned int n=0; n<progenitors.size(); n++){
	  prog_tot_e += progenitors[n].pg_energy; 
	}

	for(unsigned int n=0; n<progenitors.size(); n++){
	  // Is this progenitor already in the list? 
	  bool inList = false; 
	  unsigned int match = 0; 
	  for(unsigned int m=0; m<combined_progenitors.size(); m++){
	    if(progenitors[n].barcode == combined_progenitors[m].barcode) {
	      inList = true; 
	      match = m; 
	      break; 
	    }
	  }
	      
	  if(inList){
	    combined_progenitors[match].energy += constituents[j].E()*(progenitors[n].pg_energy/prog_tot_e); 
	  }
	  else{
	    JetProgenitor newProg = progenitors[n]; 
	    newProg.energy = constituents[j].E()*(progenitors[n].pg_energy/prog_tot_e); 
	    combined_progenitors.push_back(newProg); 
	  }
	     
	}

	// Now get the z of the leading constituent. 
	// We need to go back to the measured track properties. 

	SvtxTrack* track = NULL;
	for (SvtxTrackMap::ConstIter track_itr = _trackmap->begin();
	     track_itr != _trackmap->end(); track_itr++) {

	  SvtxTrack* temp = dynamic_cast<SvtxTrack*>(track_itr->second);
	  PHG4Particle* g4particle = svtx_eval->max_truth_particle_by_nclusters(temp); 
	  if(!g4particle) continue; 
	  if ( g4particle->get_track_id() == con_g4particle->get_track_id() ) {
	    track = temp;
	    break; 
	  }
	}
	float z_constit = -1; 
	if(track){
	  float ctheta = (track->get_px()*inclusive_jets[ijet].px() + 
			  track->get_py()*inclusive_jets[ijet].py() + 
			  track->get_pz()*inclusive_jets[ijet].pz())/
	                 (track->get_p()*jet_ptot);
	  z_constit = track->get_p()*ctheta/jet_ptot; 
	  if(z_constit>z_lead) {
	    charge_lead = track->get_charge();
	    z_lead = z_constit; 
	  }
	}

	// Fill the constituents vector
	
	trk_constituent_E.push_back(constituents[j].E()); 
	float cdphi = DeltaPhi(constituents[j].phi_02pi(),jet_phi); 
	trk_constituent_dR.push_back(sqrt( pow(constituents[j].eta()-jet_eta,2) + pow(cdphi,2) )); 
	trk_constituent_pT.push_back(sqrt(pow(constituents[j].px(),2)+pow(constituents[j].py(),2)));
	trk_constituent_pz.push_back(constituents[j].pz());
	trk_constituent_phi.push_back(constituents[j].phi_std());
        trk_constituent_z.push_back(z_constit);

      }
	  
      // Sort from highest energy to lowest energy 
      std::sort(combined_progenitors.begin(), combined_progenitors.end(), sortfunction); 

      pg_trk_id.clear();
      pg_trk_fract.clear(); 
      pg_trk_status.clear(); 
      for(unsigned int ip=0; ip<combined_progenitors.size(); ip++){
	pg_trk_id.push_back((int)combined_progenitors[ip].pid); 
	pg_trk_fract.push_back((double)(combined_progenitors[ip].energy/inclusive_jets[ijet].E())); 
	pg_trk_status.push_back(combined_progenitors[ip].status); 
      }

      // Match to a primary 
      
      mpjet_ptot = -9999.0; 
      mpjet_eta = -9999.0;
      mpjet_pT = -9999.0; 
      mpjet_dR = -9999.0;
      mpjet_emfract = -9999.0; 
      mpjet_z_lead = -9999.0; 
      mpjet_charge_lead = -9999.0; 

      if(prim_jets.size()>0) {

	float _dR = 9999.0;
	int imatch = -1; 
	for (unsigned int iprim = 0; iprim < prim_jets.size(); ++iprim) {

	  float dR = sqrt( pow(inclusive_jets[ijet].eta()-prim_jets[iprim].jet->eta(),2) + 
			   pow(DeltaPhi(inclusive_jets[ijet].phi_02pi(), prim_jets[iprim].jet->phi_02pi()),2) ); 
	  if(dR<_dR){
	    _dR = dR; 
	    imatch = iprim; 
	  }
	}
	if( (imatch>=0) && (_dR<jetMatchDR) ){
	  // We have a match!
	  mpjet_ptot = sqrt(pow(prim_jets[imatch].jet->px(),2) + 
			    pow(prim_jets[imatch].jet->py(),2) + 
			    pow(prim_jets[imatch].jet->pz(),2));
	  mpjet_eta = prim_jets[imatch].jet->eta(); 
	  mpjet_pT = sqrt(pow(prim_jets[imatch].jet->px(),2) + pow(prim_jets[imatch].jet->py(),2)); 
	  mpjet_dR = _dR; 
	  mpjet_emfract = prim_jets[imatch].emfract; 
	  mpjet_z_lead = prim_jets[imatch].z_lead; 
	  mpjet_charge_lead = prim_jets[imatch].charge_lead; 

	  // Fill in the primary progenitor information

	  pg_prim_id.clear();
	  pg_prim_fract.clear(); 
	  pg_prim_status.clear(); 
	  for(unsigned int ip=0; ip<prim_jets[imatch].progenitors.size(); ip++){
	    pg_prim_id.push_back((int) prim_jets[imatch].progenitors[ip].pid); 
	    pg_prim_fract.push_back((double)(prim_jets[imatch].progenitors[ip].energy/prim_jets[imatch].jet->E())); 
	    pg_prim_status.push_back(prim_jets[imatch].progenitors[ip].status); 
	  }

 	}

      }

      _eval_tree_trk_jets->Fill();

      // Save this jet for later study

      RecoJet newJet; 
      newJet.jet = &inclusive_jets[ijet]; 
      newJet.progenitors = combined_progenitors;
      newJet.z_lead = z_lead; 
      newJet.charge_lead = charge_lead; 
      newJet.emfract = mpjet_emfract; 
      newJet.constituents = inclusive_jets[ijet].constituents(); 

      trk_jets.push_back(newJet);  

    }

  }

}

void FastTrackingEval::GetTowerJets(PHCompositeNode *topNode, std::vector<fastjet::PseudoJet> &inclusive_jets)
{
  
  twr_jets.clear(); 

  // Use the CEMC/HCAL tower inputs to the jets

  std::vector<fastjet::PseudoJet> pseudojets;

  // CEMC

  string towernodename = "TOWER_CALIB_CEMC";
  // Grab the towers
  RawTowerContainer* cemc_towers = findNode::getClass<RawTowerContainer>(topNode, towernodename.c_str());
  if (!cemc_towers)
    {
      std::cout << PHWHERE << ": Could not find node " << towernodename.c_str() << std::endl;
      return;
    }
  string towergeomnodename = "TOWERGEOM_CEMC";
  RawTowerGeomContainer *cemc_towergeom = findNode::getClass<RawTowerGeomContainer>(topNode, towergeomnodename.c_str());
  if (! cemc_towergeom)
   {
     cout << PHWHERE << ": Could not find node " << towergeomnodename.c_str() << endl;
     return;
   }
  string hitsnodename = "G4HIT_CEMC";
  PHG4HitContainer * cemc_hits = findNode::getClass<PHG4HitContainer>(topNode, hitsnodename.c_str());
  if (!cemc_hits)
    {
      std::cout << PHWHERE << ": Could not find node " << hitsnodename.c_str() << std::endl;
      return;
    }

  // We generate the calorimeter incident particle list here so that we can resacle the 
  // CEMC tower energy (if requested). 
  calo_incident.clear(); 
  GetCalorimeterIncident(topNode,"CEMC",cemc_towers, cemc_towergeom, cemc_hits, calo_incident);
  FillTowerPseudoJets(cemc_towers, cemc_towergeom, pseudojets, 1.0); 

  // HCALIN
  RawTowerContainer* ihcal_towers = NULL; 
  RawTowerGeomContainer* ihcal_towergeom = NULL; 
  PHG4HitContainer * ihcal_hits = NULL; 
  if(use_ihcal){
    towernodename = "TOWER_CALIB_HCALIN";
    // Grab the towers
    ihcal_towers = findNode::getClass<RawTowerContainer>(topNode, towernodename.c_str());
    if (!ihcal_towers)
      {
	std::cout << PHWHERE << ": Could not find node " << towernodename.c_str() << std::endl;
	return;
      }
    towergeomnodename = "TOWERGEOM_HCALIN";
    ihcal_towergeom = findNode::getClass<RawTowerGeomContainer>(topNode, towergeomnodename.c_str());
    if (! ihcal_towergeom)
      {
	cout << PHWHERE << ": Could not find node " << towergeomnodename.c_str() << endl;
	return;
      }
    hitsnodename = "G4HIT_HCALIN";
    ihcal_hits = findNode::getClass<PHG4HitContainer>(topNode, hitsnodename.c_str());
    if (!ihcal_hits)
      {
	std::cout << PHWHERE << ": Could not find node " << hitsnodename.c_str() << std::endl;
	return;
      }

    GetCalorimeterIncident(topNode,"HCALIN",ihcal_towers, ihcal_towergeom, ihcal_hits, calo_incident); 
    FillTowerPseudoJets(ihcal_towers, ihcal_towergeom, pseudojets, 1.0); 
  }

  // HCALOUT

  towernodename = "TOWER_CALIB_HCALOUT";
  // Grab the towers
  RawTowerContainer* ohcal_towers = findNode::getClass<RawTowerContainer>(topNode, towernodename.c_str());
  if (!ohcal_towers)
    {
      std::cout << PHWHERE << ": Could not find node " << towernodename.c_str() << std::endl;
      return;
    }
  towergeomnodename = "TOWERGEOM_HCALOUT";
  RawTowerGeomContainer* ohcal_towergeom = findNode::getClass<RawTowerGeomContainer>(topNode, towergeomnodename.c_str());
  if (! ohcal_towergeom)
   {
     cout << PHWHERE << ": Could not find node " << towergeomnodename.c_str() << endl;
     return;
   }
  hitsnodename = "G4HIT_HCALOUT";
  PHG4HitContainer * ohcal_hits = findNode::getClass<PHG4HitContainer>(topNode, hitsnodename.c_str());
  if (!ohcal_hits)
    {
      std::cout << PHWHERE << ": Could not find node " << hitsnodename.c_str() << std::endl;
      return;
    }

  GetCalorimeterIncident(topNode,"HCALOUT",ohcal_towers, ohcal_towergeom, ohcal_hits, calo_incident); 
  FillTowerPseudoJets(ohcal_towers, ohcal_towergeom, pseudojets, 1.0); 

  if(pseudojets.size()<=0) return; 

  // Call FastJet
  // Sort out the found jets, see if we can sort out the progenitors
	
  fastjet::JetDefinition *jetdef = new fastjet::JetDefinition(fastjet::antikt_algorithm,_R, fastjet::E_scheme,fastjet::Best);
  fastjet::ClusterSequence jetFinder(pseudojets,*jetdef);
  //std::vector<fastjet::PseudoJet> inclusive_jets = sorted_by_pt(jetFinder.inclusive_jets(ptmin)); 
  inclusive_jets.clear(); 
  inclusive_jets = sorted_by_pt(jetFinder.inclusive_jets(ptmin)); 
  delete jetdef;

  // Loop over the jets that were found, and match the jet constituents (towers) to the 
  // incident particles. Use the incident particles to trace back to the jet progenitors. 

  for (unsigned int ijet = 0; ijet < inclusive_jets.size(); ++ijet) {

    jet_pT = sqrt(pow(inclusive_jets[ijet].px(),2) + pow(inclusive_jets[ijet].py(),2)); 
    jet_ptot = sqrt(pow(inclusive_jets[ijet].px(),2) + 
		    pow(inclusive_jets[ijet].py(),2) + 
		    pow(inclusive_jets[ijet].pz(),2));
    jet_twrE = inclusive_jets[ijet].E(); 
    jet_E = inclusive_jets[ijet].E(); 
    jet_eta = inclusive_jets[ijet].eta(); 
    jet_rapidity = inclusive_jets[ijet].rapidity(); 
    jet_mass = inclusive_jets[ijet].m(); 
    jet_phi = inclusive_jets[ijet].phi_02pi(); 
    jet_xF = inclusive_jets[ijet].pz()/p_beam; 

    cemc_constituent_E.clear();
    cemc_constituent_dR.clear();
    cemc_constituent_pT.clear();
    cemc_constituent_pz.clear();
    cemc_constituent_phi.clear();
    ihcal_constituent_E.clear();
    ihcal_constituent_dR.clear();
    ihcal_constituent_pT.clear();
    ihcal_constituent_pz.clear();
    ihcal_constituent_phi.clear();
    ohcal_constituent_E.clear();
    ohcal_constituent_dR.clear();
    ohcal_constituent_pT.clear();
    ohcal_constituent_pz.clear();
    ohcal_constituent_phi.clear();

    vector<fastjet::PseudoJet> constituents = inclusive_jets[ijet].constituents();
    jet_nconstit = constituents.size(); 

    combined_progenitors.clear();
    z_lead = -1.0;
    charge_lead = -9999.0; 
    for (unsigned int j=0; j<constituents.size(); j++){

      progenitors.clear(); 

      std::vector<ParticleTower *>::iterator it;
      for (it = calo_incident.begin(); it != calo_incident.end(); ++it){
	
	int tsize = (*it)->TowerIDs.size(); 
	for(int itid = 0; itid < tsize; itid++){

	  // Match this towerID to the particles hitting a given tower
	  if( (unsigned int) constituents[j].user_index() == (*it)->TowerIDs[itid]){
	    
	    PHG4Particle *con_g4particle = (*it)->particle; 

	    float con_ptot = sqrt( pow(con_g4particle->get_px(),2) + 
				   pow(con_g4particle->get_py(),2) + 
				   pow(con_g4particle->get_pz(),2) ); 

	    float ctheta = (con_g4particle->get_px()*inclusive_jets[ijet].px() + 
			    con_g4particle->get_py()*inclusive_jets[ijet].py() + 
			    con_g4particle->get_pz()*inclusive_jets[ijet].pz())/
	                   (con_ptot*jet_ptot);

	    float z_constit = con_ptot*ctheta/jet_ptot; 

	    if(z_constit>z_lead) {
	      G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	      G4ParticleDefinition* particle = particleTable->FindParticle(con_g4particle->get_name()); 
	      if(particle){
		charge_lead = particle->GetPDGCharge();
		z_lead = z_constit;
	      }
	    }
	    
	    // Let's see if we can find the constituent's progenitor(s)
	    // This will sum the progenitor list over all particles that 
	    // hit the calorimeter tower. 
	    GetJetPrimaryContributors( topNode, con_g4particle ); 

	  }
	}
      }

      float prog_tot_e = 0.0; 
      for(unsigned int n=0; n<progenitors.size(); n++){
	prog_tot_e += progenitors[n].pg_energy; 
      }

      // Add this constituent's progenitors to the list of jet progenitors
      for(unsigned int n=0; n<progenitors.size(); n++){
	// Is this progenitor already in the list? 
	bool inList = false; 
	unsigned int match = 0; 
	for(unsigned int m=0; m<combined_progenitors.size(); m++){
	  if(progenitors[n].barcode == combined_progenitors[m].barcode) {
	    inList = true; 
	    match = m; 
	    break; 
	  }
	}
	      
	if(inList){
	  combined_progenitors[match].energy += constituents[j].E()*(progenitors[n].pg_energy/prog_tot_e); 
	}
	else{
	  JetProgenitor newProg = progenitors[n]; 
	  newProg.energy = constituents[j].E()*(progenitors[n].pg_energy/prog_tot_e); 
	  combined_progenitors.push_back(newProg); 
	}
	
      }

      // Fill the constituents vector
    
      std::string cname = 
	RawTowerDefs::convert_caloid_to_name(  RawTowerDefs::decode_caloid( (unsigned int)constituents[j].user_index()) );
      float cdphi = DeltaPhi(constituents[j].phi_02pi(),jet_phi); 
      if(cname == "CEMC"){
        cemc_constituent_E.push_back(constituents[j].E()); 
        cemc_constituent_dR.push_back(sqrt( pow(constituents[j].eta()-jet_eta,2) + pow(cdphi,2) )); 
	cemc_constituent_pT.push_back(sqrt(pow(constituents[j].px(),2)+pow(constituents[j].py(),2)));
	cemc_constituent_pz.push_back(constituents[j].pz());
	cemc_constituent_phi.push_back(constituents[j].phi_std());
      }
      else if(cname == "HCALIN"){
        ihcal_constituent_E.push_back(constituents[j].E()); 
        ihcal_constituent_dR.push_back(sqrt( pow(constituents[j].eta()-jet_eta,2) + pow(cdphi,2) ));       
        ihcal_constituent_pT.push_back(sqrt(pow(constituents[j].px(),2)+pow(constituents[j].py(),2)));
	ihcal_constituent_pz.push_back(constituents[j].pz());
	ihcal_constituent_phi.push_back(constituents[j].phi_std());
      }
      else if(cname == "HCALOUT"){
        ohcal_constituent_E.push_back(constituents[j].E()); 
        ohcal_constituent_dR.push_back(sqrt( pow(constituents[j].eta()-jet_eta,2) + pow(cdphi,2) ));       
        ohcal_constituent_pT.push_back(sqrt(pow(constituents[j].px(),2)+pow(constituents[j].py(),2)));
	ohcal_constituent_pz.push_back(constituents[j].pz());
	ohcal_constituent_phi.push_back(constituents[j].phi_std());
      }
      
    }

    // Sort from highest energy to lowest energy 
    std::sort(combined_progenitors.begin(), combined_progenitors.end(), sortfunction); 

    pg_twr_id.clear();
    pg_twr_fract.clear(); 
    pg_twr_status.clear(); 
    for(unsigned int ip=0; ip<combined_progenitors.size(); ip++){
      pg_twr_id.push_back((int)combined_progenitors[ip].pid); 
      pg_twr_fract.push_back((double)(combined_progenitors[ip].energy/inclusive_jets[ijet].E())); 
      pg_twr_status.push_back(combined_progenitors[ip].status); 
    }
	  
    // Match to a primary 
      
    mpjet_ptot = -9999.0; 
    mpjet_eta = -9999.0;
    mpjet_pT = -9999.0; 
    mpjet_dR = -9999.0; 
    mpjet_emfract = -9999.0; 
    mpjet_z_lead = -9999.0; 
    mpjet_charge_lead = -9999.0; 

    if(prim_jets.size()>0) {

      float _dR = 9999.0;
      int imatch = -1; 
      for (unsigned int iprim = 0; iprim < prim_jets.size(); ++iprim) {

	float dR = sqrt( pow(inclusive_jets[ijet].eta()-prim_jets[iprim].jet->eta(),2) + 
			 pow(DeltaPhi(inclusive_jets[ijet].phi_02pi(), prim_jets[iprim].jet->phi_02pi()),2) ); 
	if(dR<_dR){
	  _dR = dR; 
	  imatch = iprim; 
	}
      }
      if( (imatch>=0) && (_dR<jetMatchDR) ){
	// We have a match!
	mpjet_ptot = sqrt(pow(prim_jets[imatch].jet->px(),2) + 
			  pow(prim_jets[imatch].jet->py(),2) + 
			  pow(prim_jets[imatch].jet->pz(),2));
	mpjet_eta = prim_jets[imatch].jet->eta(); 
	mpjet_pT = sqrt(pow(prim_jets[imatch].jet->px(),2) + pow(prim_jets[imatch].jet->py(),2)); 
	mpjet_dR = _dR; 
	mpjet_emfract = prim_jets[imatch].emfract; 
        mpjet_z_lead = prim_jets[imatch].z_lead; 
        mpjet_charge_lead = prim_jets[imatch].charge_lead; 

	// Fill in the primary progenitor information

	pg_prim_id.clear();
	pg_prim_fract.clear(); 
	pg_prim_status.clear(); 
	for(unsigned int ip=0; ip<prim_jets[imatch].progenitors.size(); ip++){
	  pg_prim_id.push_back((int) prim_jets[imatch].progenitors[ip].pid); 
	  pg_prim_fract.push_back((double)(prim_jets[imatch].progenitors[ip].energy/prim_jets[imatch].jet->E())); 
	  pg_prim_status.push_back(prim_jets[imatch].progenitors[ip].status); 
	}

      }

    }

    _eval_tree_twr_jets->Fill();

    // Save this jet for later study

    RecoJet newJet; 
    newJet.jet = &inclusive_jets[ijet]; 
    newJet.progenitors = combined_progenitors;
    newJet.z_lead = z_lead; 
    newJet.charge_lead = charge_lead;
    newJet.emfract = mpjet_emfract; 
    newJet.constituents = inclusive_jets[ijet].constituents(); 

    twr_jets.push_back(newJet);  

  }

  return; 
}

void FastTrackingEval::FillTowerPseudoJets( RawTowerContainer *towers, RawTowerGeomContainer *towergeom,  
					    std::vector<fastjet::PseudoJet> &pseudojets, const float scale_factor){

  RawTowerContainer::ConstRange begin_end  = towers->getTowers();
  RawTowerContainer::ConstIterator itr = begin_end.first;
  for (; itr != begin_end.second; ++itr){

    RawTowerDefs::keytype towerid = itr->first;
    RawTowerGeom *tgeo = towergeom->get_tower_geometry(towerid); 
    RawTower *ctower = towers->getTower(towerid);

    // Tower pedestal energy cut: 
    if(ctower->get_energy()<=TOWER_E_CUT) continue; 

    float x = tgeo->get_center_x(); 
    float y = tgeo->get_center_y(); 
    float z = tgeo->get_center_z(); 

    float eta = getEta(sqrt(pow(x,2)+pow(y,2)),z-vtx_z); 
    float phi = XYtoPhi(x,y); 
 
    double pt = scale_factor*ctower->get_energy() / cosh(eta);
    double px = pt * cos(phi);
    double py = pt * sin(phi);
    double pz = pt * sinh(eta);

    fastjet::PseudoJet pseudojet (px,py,pz,scale_factor*ctower->get_energy()); 
    // this only works because user_index and RawTowerDefs::keytype are int's (signed and unsigned)
    pseudojet.set_user_index((int)ctower->get_id()); 
    pseudojets.push_back(pseudojet);
    
  }

  return; 

}

void FastTrackingEval::GetJetPrimaryContributors( PHCompositeNode *topNode, PHG4Particle* con_g4particle ){


  PHG4Particle* g4particle = con_g4particle; 
  PHNodeIterator iter(topNode);
  PHHepMCGenEvent *genevent = findNode::getClass<PHHepMCGenEvent>(topNode,"PHHepMCGenEvent");
  if(!genevent) {
    std::cout << " PHHepMCGenEvent node missing! " << std::endl; 
    return;
  }
  HepMC::GenEvent *evt = genevent->getEvent();
  HepMC::GenParticle *ptcle = NULL; 

  // Track the g4particle back to its primary

  //std::cout << "Starting Particle ->" << g4particle->get_pid() << std::endl; 

  while( !_truth_container->is_primary(g4particle) ){
    // All particles
    PHG4Particle *prev_g4particle = g4particle; 
    PHG4TruthInfoContainer::ConstRange range = _truth_container->GetParticleRange();
    for (PHG4TruthInfoContainer::ConstIterator truth_itr = range.first;
	 truth_itr != range.second; ++truth_itr) {
      PHG4Particle* mother = truth_itr->second;
      if(g4particle->get_parent_id()==mother->get_track_id()){
	g4particle = mother;  
	break; 
      }
    }
    if(prev_g4particle==g4particle) return; // pure secondary?
    //std::cout << "->" << g4particle->get_pid() << std::endl; 
  }

  // Associate the g4particle with it's HepMC primary 

  HepMC::GenEvent::particle_iterator pitr = evt->particles_begin();
  for( ; pitr!=evt->particles_end(); pitr++){
    ptcle = *pitr; 
    if( ptcle->barcode() == g4particle->get_barcode() ) break; 
  }
  if(!ptcle) {
    std::cout << " No HepMC primary found associated with G4 primary! " << std::endl; 
    return; 
  }

  // Recursively trace out the particle tree
  TraceHepMCParticle(ptcle); 

}

void FastTrackingEval::TraceHepMCParticle( HepMC::GenParticle *ptcle ){
  
  // Important Pythia 8 Status Codes: 
  // 23: outgoing of hardest subprocess
  // 33: outgoing of subsequent subprocesses
  // 43: outgoing from an initial state branching
  // 51: outgoing from a final state branching 
  // 63: outgoing beam remnant

  // Pythia6 Status Codes:
  // 1: Stable final state particle
  // 2/10902: Unstable particle
  // 3: Documentary particle 

  if(ptcle->production_vertex()){
    for ( HepMC::GenVertex::particle_iterator mother 
	    = ptcle->production_vertex()->particles_begin(HepMC::parents);
	  mother != ptcle->production_vertex()->particles_end(HepMC::parents); 
	  ++mother ) {

      //std::cout << "->" << "particle = " << ptcle->pdg_id() << " status = " << ptcle->status() 
      //		<< " parent = " << (*mother)->pdg_id() << " parent status = " << (*mother)->status() << std::endl; 

      if(!pythia6){
	
	// These status codes terminate tracing 

	if( ((*mother)->status()==23) || 
	    ((*mother)->status()==33) ||
	    ((*mother)->status()==43) ||
	    ((*mother)->status()==63) ){ 

	  JetProgenitor jprog; 
	  jprog.pid = (*mother)->pdg_id(); 
	  jprog.status = (*mother)->status(); 
	  jprog.barcode = (*mother)->barcode(); 
	  jprog.energy = 0.0;  
	  jprog.pg_energy = (*mother)->momentum().e(); 
	  progenitors.push_back(jprog); 

	}
	else
	  TraceHepMCParticle(*mother); 
	  
      }
      else{

	if( ((*mother)->status()==3) ){

	  JetProgenitor jprog; 
	  jprog.pid = (*mother)->pdg_id(); 
	  jprog.status = (*mother)->status(); 
	  jprog.barcode = (*mother)->barcode(); 
	  jprog.energy = 0.0;  
	  jprog.pg_energy = (*mother)->momentum().e(); 
	  progenitors.push_back(jprog); 

	}
	else
	  TraceHepMCParticle(*mother); 
	
      }

    }
  }

}


int FastTrackingEval::GetParticleParentPID( PHCompositeNode *topNode, PHG4Particle* g4particle ){

  // If the particle is not a primary, we use the GEANT banks to get 
  // its parent.  If it is a primary, we use the HEPMC primary information 
  // to get the parent of the primary. 

  int prim_PID = -9999.0; 

  if( _truth_container->is_primary(g4particle) ){

    // Find the matching HEPMC primary

    PHNodeIterator iter(topNode);
    PHHepMCGenEvent *genevent = findNode::getClass<PHHepMCGenEvent>(topNode,"PHHepMCGenEvent");
    if(genevent){

      HepMC::GenEvent *evt = genevent->getEvent();
      HepMC::GenParticle *ptcle = NULL; 
      HepMC::GenEvent::particle_iterator pitr = evt->particles_begin();
      for( ; pitr!=evt->particles_end(); pitr++){
	ptcle = *pitr; 
	if( ptcle->barcode() == g4particle->get_barcode() ) break; 
      }
      //std::cout << "PARTICLE:" << std::endl; 
      //ptcle->print();
      if(ptcle){
	
	// This structure allows us to get beyond entries 
	// in the particle record where the same particle is repeated. 

	HepMC::GenParticle *iptcle = ptcle; 
	HepMC::GenParticle *found_mother = NULL; 
	bool match = false; 
	do{
	  if(iptcle->production_vertex()){
	    for ( HepMC::GenVertex::particle_iterator mother 
		    = iptcle->production_vertex()->particles_begin(HepMC::parents);
		  mother != iptcle->production_vertex()->particles_end(HepMC::parents); 
		  ++mother ) {
	      // For now, I just take the first particle in the list
	      // What I'm really interested in here is things like decays, etc.
	      // so this should get me the ID of the decaying particle
	      found_mother = (*mother); 
	      break; 
	    }
	  }
	  // Did we find a particle?  Is it a match to the existing particle?  
	  if(found_mother){
	    if( found_mother->pdg_id() == ptcle->pdg_id() ){
	      HepMC::FourVector p_mom = ptcle->momentum(); 
	      HepMC::FourVector m_mom = found_mother->momentum(); 
	      if( (p_mom.px()==m_mom.px()) && 
		  (p_mom.py()==m_mom.py()) && 
		  (p_mom.pz()==m_mom.pz()) 
		  ){
		match = true; 
		iptcle = found_mother; 
	      }
	      else
		match = false; 
	    }
	    else
	      match = false; 
	  }
	  else
	    match = false; 

	}while(match); 
	       
	if(found_mother){
	  //std::cout << "PARENT:" << std::endl;
	  //found_mother->print(); 
	  prim_PID = found_mother->pdg_id();
	}

      }

    }

  }
  else{
    
    // ALL PARTICLES
    PHG4TruthInfoContainer::ConstRange range = _truth_container->GetParticleRange();
    for (PHG4TruthInfoContainer::ConstIterator truth_itr = range.first;
	 truth_itr != range.second; ++truth_itr) {
      PHG4Particle* mother = truth_itr->second;
      if(g4particle->get_parent_id()==mother->get_track_id()){
	prim_PID = mother->get_pid(); 
	break; 
      }
    }

  }


  return prim_PID; 

}

bool FastTrackingEval:: getHighestEnergyCluster( PHCompositeNode *topNode, string detName, float &cl_eta, float &cl_phi ){

  // pull the clusters
  string clusternodename = "CLUSTER_" + detName;
  RawClusterContainer *clusterList = findNode::getClass<RawClusterContainer>(topNode,clusternodename.c_str());
  if (!clusterList) {
    cerr << PHWHERE << " ERROR: Can't find node " << clusternodename << endl;
    return false;
  }    

  cl_eta = -9999.0;
  cl_phi = -9999.0; 

#define CLUSTER_E_CUTOFF 9.0*TOWER_E_CUT

  float clusEnergy = 0.0;
  for (unsigned int k = 0; k < clusterList->size(); ++k) {
    RawCluster *cluster = clusterList->getCluster(k);
    if((cluster->get_energy()>CLUSTER_E_CUTOFF) && (cluster->get_energy()>clusEnergy)) {
      clusEnergy = cluster->get_energy();
      cl_eta = cluster->get_eta(); 
      cl_phi = cluster->get_phi(); 
    }
  }
  
  if(clusEnergy>0.0) 
    return true;
  else
    return false; 

}


// void FastTrackingEval:: sumClusterEnergy( PHCompositeNode *topNode, string detName ){

//   // pull the clusters
//   string clusternodename = "CLUSTER_" + detName;
//   RawClusterContainer *clusterList = findNode::getClass<RawClusterContainer>(topNode,clusternodename.c_str());
//   if (!clusterList) {
//     cerr << PHWHERE << " ERROR: Can't find node " << clusternodename << endl;
//     return;
//   }    

//   float totEnergy = 0.0; 
//   int nClust = 0; 
//   for (unsigned int k = 0; k < clusterList->size(); ++k) {
//     RawCluster *cluster = clusterList->getCluster(k);
//     if(cluster->get_energy()>CLUSTER_E_CUTOFF) {
//       totEnergy += cluster->get_energy();
//       nClust++; 
//     }
//   }

//   if(detName == "FEMC"){
//     femc_sumEcl = totEnergy;
//     femc_Ncl = nClust;
//   }
//   else if(detName == "FHCAL"){
//     fhcal_sumEcl = totEnergy;
//     fhcal_Ncl = nClust;
//   }
//   else if(detName == "HCALIN"){
//     ihcal_sumEcl = totEnergy;
//     ihcal_Ncl = nClust;
//   }
//   else if(detName == "HCALOUT"){
//     ohcal_sumEcl = totEnergy;
//     ohcal_Ncl = nClust;
//   }
//   else if(detName == "CEMC"){
//     cemc_sumEcl = totEnergy;
//     cemc_Ncl = nClust;
//   }

// }

void FastTrackingEval:: sumTowerEnergy( PHCompositeNode *topNode, string detName ){

  string towernodename = "TOWER_CALIB_" + detName;
  // Grab the towers
  RawTowerContainer* towers = findNode::getClass<RawTowerContainer>(topNode, towernodename.c_str());
  if (!towers)
    {
      std::cout << PHWHERE << ": Could not find node " << towernodename.c_str() << std::endl;
      return;
    }
 
  float totEnergy = 0.0;
  int Ntwr = 0; 
  RawTowerContainer::ConstRange begin_end  = towers->getTowers();
  RawTowerContainer::ConstIterator itr = begin_end.first;
  for (; itr != begin_end.second; ++itr)
    {
      RawTowerDefs::keytype towerid = itr->first;
      RawTower *rawtower = towers->getTower(towerid);
      if(rawtower) {
	if(rawtower->get_energy()>TOWER_E_CUT) {
	  totEnergy += rawtower->get_energy();
	  Ntwr++; 
	}
      }

    }

  if(detName == "FEMC"){
    femc_sumE = totEnergy;
    femc_Ntwr = Ntwr;
  }
  else if(detName == "FHCAL"){
    fhcal_sumE = totEnergy;
    fhcal_Ntwr = Ntwr;
  }
  else if(detName == "HCALIN"){
    ihcal_sumE = totEnergy;
    ihcal_Ntwr = Ntwr;
  }
  else if(detName == "HCALOUT"){
    ohcal_sumE = totEnergy;
    ohcal_Ntwr = Ntwr;
  }
  else if(detName == "CEMC"){
    cemc_sumE = totEnergy;
    cemc_Ntwr = Ntwr;
  }

}

void FastTrackingEval:: getCluster( PHCompositeNode *topNode, string detName, float eta, float phi, int secondFlag){

  // pull the clusters
  string clusternodename = "CLUSTER_" + detName;
  RawClusterContainer *clusterList = findNode::getClass<RawClusterContainer>(topNode,clusternodename.c_str());
  if (!clusterList) {
    cerr << PHWHERE << " ERROR: Can't find node " << clusternodename << endl;
    return;
  }    

  // loop over all clusters and find nearest
  double min_r = DBL_MAX;
  double min_e = -9999.0;
  int min_n = -1; 

  for (unsigned int k = 0; k < clusterList->size(); ++k) {

    RawCluster *cluster = clusterList->getCluster(k);

    double dphi = atan2(sin(phi-cluster->get_phi()),cos(phi-cluster->get_phi()));
    double deta = eta-cluster->get_eta();
    double r = sqrt(pow(dphi,2)+pow(deta,2));

    if (r < min_r) {
      min_r = r;
      min_e = cluster->get_energy();
      min_n = cluster->getNTowers(); 
    }
  }
  
  if(detName == "FEMC") {
    if(!secondFlag){
      femc_cldr = min_r; 
      femc_clE = min_e;
      femc_clN = min_n; 
    }
    else{
      femc_cldr2 = min_r; 
      femc_clE2 = min_e;
      femc_clN2 = min_n;       
    }
  }
  else if(detName == "FHCAL"){
    if(!secondFlag){
      fhcal_cldr = min_r; 
      fhcal_clE = min_e; 
      fhcal_clN = min_n; 
    }
    else{
      fhcal_cldr2 = min_r; 
      fhcal_clE2 = min_e; 
      fhcal_clN2 = min_n; 
    }
  }
  else if(detName == "CEMC"){
    if(!secondFlag){
      cemc_cldr = min_r; 
      cemc_clE = min_e;
      cemc_clN = min_n; 
    }
    else{
      cemc_cldr2 = min_r; 
      cemc_clE2 = min_e;
      cemc_clN2 = min_n;       
    }
  }
  else if(detName == "HCALIN"){
    if(!secondFlag){
      hcalin_cldr = min_r; 
      hcalin_clE = min_e; 
      hcalin_clN = min_n; 
    }
    else{
      hcalin_cldr2 = min_r; 
      hcalin_clE2 = min_e; 
      hcalin_clN2 = min_n; 
    }
  }
  else if(detName == "HCALOUT"){
    if(!secondFlag){
      hcalout_cldr = min_r; 
      hcalout_clE = min_e; 
      hcalout_clN = min_n; 
    }
    else{
      hcalout_cldr2 = min_r; 
      hcalout_clE2 = min_e; 
      hcalout_clN2 = min_n; 
    }
  }



}

float FastTrackingEval:: PositionCorrectEMCal3x3( PHCompositeNode *topNode, int clIdx, float e33){

  // pull the clusters
  // Assumes a 1-1 correlation between the two lists!
  string clusternodename = "CLUSTER_CEMC";
  RawClusterContainer *clusterList = findNode::getClass<RawClusterContainer>(topNode,clusternodename.c_str());
  if (!clusterList) {
    cerr << PHWHERE << " ERROR: Can't find node " << clusternodename << endl;
    return 0.0;
  }    

  clusternodename = "CLUSTER_POS_COR_CEMC";
  RawClusterContainer *clusterList_posCor = findNode::getClass<RawClusterContainer>(topNode,clusternodename.c_str());
  if (!clusterList_posCor) {
    cerr << PHWHERE << " ERROR: Can't find node " << clusternodename << endl;
    return 0.0;
  }    

  if((clIdx>=0) && (clIdx<(int)clusterList->size())){
    RawCluster *cluster = clusterList->getCluster(clIdx);
    RawCluster *cluster_corr = clusterList_posCor->getCluster(clIdx);
    return (cluster_corr->get_energy()/cluster->get_energy())*e33; 
  }
  else
    return 0.0; 

}

void FastTrackingEval:: getClusterByIndex( PHCompositeNode *topNode, string detName, int clIdx, int secondFlag){

  // pull the clusters
  string clusternodename = "CLUSTER_" + detName;
  RawClusterContainer *clusterList = findNode::getClass<RawClusterContainer>(topNode,clusternodename.c_str());
  if (!clusterList) {
    cerr << PHWHERE << " ERROR: Can't find node " << clusternodename << endl;
    return;
  }    

  double min_r = 9999.0; 
  double min_e = 0.0;
  double min_n = 0.0; 

  if((clIdx>=0) && (clIdx<(int)clusterList->size())){
    RawCluster *cluster = clusterList->getCluster(clIdx);
    min_r = 0.0; 
    min_e = cluster->get_energy();
    min_n = cluster->getNTowers(); 
  }
  
  if(detName == "FEMC") {
    if(!secondFlag){
      femc_cldr = min_r; 
      femc_clE = min_e;
      femc_clN = min_n; 
    }
    else{
      femc_cldr2 = min_r; 
      femc_clE2 = min_e;
      femc_clN2 = min_n;       
    }
  }
  else if(detName == "FHCAL"){
    if(!secondFlag){
      fhcal_cldr = min_r; 
      fhcal_clE = min_e; 
      fhcal_clN = min_n; 
    }
    else{
      fhcal_cldr2 = min_r; 
      fhcal_clE2 = min_e; 
      fhcal_clN2 = min_n; 
    }
  }
  else if(detName == "CEMC"){
    if(!secondFlag){
      cemc_cldr = min_r; 
      cemc_clE = min_e;
      cemc_clN = min_n; 
    }
    else{
      cemc_cldr2 = min_r; 
      cemc_clE2 = min_e;
      cemc_clN2 = min_n;       
    }
  }
  else if(detName == "HCALIN"){
    if(!secondFlag){
      hcalin_cldr = min_r; 
      hcalin_clE = min_e; 
      hcalin_clN = min_n; 
    }
    else{
      hcalin_cldr2 = min_r; 
      hcalin_clE2 = min_e; 
      hcalin_clN2 = min_n; 
    }
  }
  else if(detName == "HCALOUT"){
    if(!secondFlag){
      hcalout_cldr = min_r; 
      hcalout_clE = min_e; 
      hcalout_clN = min_n; 
    }
    else{
      hcalout_cldr2 = min_r; 
      hcalout_clE2 = min_e; 
      hcalout_clN2 = min_n; 
    }
  }



}

void FastTrackingEval:: getClusterPCByIndex( PHCompositeNode *topNode, int clIdx, int secondFlag){

  // only for CEMC

  // pull the clusters
  string clusternodename = "CLUSTER_POS_COR_CEMC";
  RawClusterContainer *clusterList = findNode::getClass<RawClusterContainer>(topNode,clusternodename.c_str());
  if (!clusterList) {
    cerr << PHWHERE << " ERROR: Can't find node " << clusternodename << endl;
    return;
  }    

  double min_e = 0.0;

  if((clIdx>=0) && (clIdx<(int)clusterList->size())){
    RawCluster *cluster = clusterList->getCluster(clIdx); 
    min_e = cluster->get_energy();
  }
  
  if(!secondFlag){
    cemc_clE_PC = min_e;
  }
  else{
    cemc_clE2_PC = min_e;
  }


}


void FastTrackingEval:: getEBarrel( PHCompositeNode *topNode, string detName, float tkx, float tky, float tkz, int secondFlag){

  float phi = XYtoPhi(tkx, tky);  // is this the correct wraparound?
  float eta = getEta(sqrt(tkx*tkx + tky*tky), tkz); 

  string towernodename = "TOWER_CALIB_" + detName;
  // Grab the towers
  RawTowerContainer* towerList = findNode::getClass<RawTowerContainer>(topNode, towernodename.c_str());
  if (!towerList)
    {
      std::cout << PHWHERE << ": Could not find node " << towernodename.c_str() << std::endl;
      return;
    }
  string towergeomnodename = "TOWERGEOM_" + detName;
  RawTowerGeomContainer *towergeo = findNode::getClass<RawTowerGeomContainer>(topNode, towergeomnodename.c_str());
  if (! towergeo)
   {
     cout << PHWHERE << ": Could not find node " << towergeomnodename.c_str() << endl;
     return;
   }

  // calculate 3x3 and 5x5 tower energies
  int binphi = towergeo->get_phibin(phi);
  int bineta = towergeo->get_etabin(eta);

  float energy_3x3 = 0.0;
  float energy_5x5 = 0.0;
  for (int iphi = binphi-2; iphi <= binphi+2; ++iphi) {
    for (int ieta = bineta-2; ieta <= bineta+2; ++ieta) {

      // wrap around
      int wrapphi = iphi;
      if (wrapphi < 0) {
	wrapphi = towergeo->get_phibins() + wrapphi;
      }
      if (wrapphi >= towergeo->get_phibins()) {
	wrapphi = wrapphi - towergeo->get_phibins();
      }

      // edges
      if (ieta < 0) continue;
      if (ieta >= towergeo->get_etabins()) continue;

      RawTower* tower = towerList->getTower(ieta,wrapphi);
      if (tower) {
        energy_5x5 += tower->get_energy();
	if (abs(iphi - binphi)<=1 and abs(ieta - bineta)<=1 )
	  energy_3x3 += tower->get_energy();
      }

    }
  }

  // Set the output
  if(detName == "CEMC") {
    if(!secondFlag){
      cemc_E33 = energy_3x3;
      cemc_E55 = energy_5x5;
    }
    else{
      cemc_E332 = energy_3x3;
      cemc_E552 = energy_5x5;
    }
  }
  else if(detName == "HCALIN") {
    if(!secondFlag){
      hcalin_E33 = energy_3x3; 
      hcalin_E55 = energy_5x5; 
    }
    else{
      hcalin_E332 = energy_3x3; 
      hcalin_E552 = energy_5x5; 
     }
  }
  else if(detName == "HCALOUT") {
    if(!secondFlag){
      hcalout_E33 = energy_3x3; 
      hcalout_E55 = energy_5x5; 
    }
    else{
      hcalout_E332 = energy_3x3; 
      hcalout_E552 = energy_5x5; 
     }
  }


}

void FastTrackingEval:: getE33( PHCompositeNode *topNode, string detName, float tkx, float tky, int secondFlag){

  float twr_sum = 0;

  string towernodename = "TOWER_CALIB_" + detName;
  // Grab the towers
  RawTowerContainer* towers = findNode::getClass<RawTowerContainer>(topNode, towernodename.c_str());
  if (!towers)
    {
      std::cout << PHWHERE << ": Could not find node " << towernodename.c_str() << std::endl;
      return;
    }
  string towergeomnodename = "TOWERGEOM_" + detName;
  RawTowerGeomContainer *towergeom = findNode::getClass<RawTowerGeomContainer>(topNode, towergeomnodename.c_str());
  if (! towergeom)
   {
     cout << PHWHERE << ": Could not find node " << towergeomnodename.c_str() << endl;
     return;
   }
  
  // Locate the central tower
  
  float r_dist = 9999.0; 
  int twr_j = -1; 
  int twr_k = -1; 
  RawTowerDefs::CalorimeterId calo_id_ = RawTowerDefs::convert_name_to_caloid( detName );   

  RawTowerContainer::ConstRange begin_end  = towers->getTowers();
  RawTowerContainer::ConstIterator itr = begin_end.first;
  for (; itr != begin_end.second; ++itr)
    {
      RawTowerDefs::keytype towerid = itr->first;
      RawTowerGeom *tgeo = towergeom->get_tower_geometry(towerid); 
      
      float x = tgeo->get_center_x(); 
      float y = tgeo->get_center_y(); 
      
      float temp_rdist = sqrt(pow(tkx-x,2) + pow(tky-y,2)) ; 
      if(temp_rdist< r_dist){
	r_dist = temp_rdist; 
	twr_j = RawTowerDefs::decode_index1(towerid); 
	twr_k = RawTowerDefs::decode_index2(towerid); 
      }
      
      if( (fabs(tkx-x)<(tgeo->get_size_x()/2.0)) &&
	  (fabs(tky-y)<(tgeo->get_size_y()/2.0)) ) break; 

    }

  // Use the central tower to sum up the 3x3 energy

  if(twr_j>=0 && twr_k>=0){
    for(int ij = -1; ij <=1; ij++){
      for(int ik = -1; ik <=1; ik++){
	RawTowerDefs::keytype temp_towerid = RawTowerDefs::encode_towerid( calo_id_ , twr_j + ij , twr_k + ik ); 
	RawTower *rawtower = towers->getTower(temp_towerid);
	if(rawtower) twr_sum += rawtower->get_energy();
      }
    }
  }

  // Set the output
  if(detName == "FEMC") {
    if(!secondFlag){
      femc_E33 = twr_sum;
    }
    else{
      femc_E332 = twr_sum;
    }
  }
  else{
    if(!secondFlag){
      fhcal_E33 = twr_sum; 
    }
    else{
      fhcal_E332 = twr_sum; 
     }
  }

}

void FastTrackingEval:: getE55( PHCompositeNode *topNode, string detName, float tkx, float tky, int secondFlag){

  float twr_sum = 0;

  string towernodename = "TOWER_CALIB_" + detName;
  // Grab the towers
  RawTowerContainer* towers = findNode::getClass<RawTowerContainer>(topNode, towernodename.c_str());
  if (!towers)
    {
      std::cout << PHWHERE << ": Could not find node " << towernodename.c_str() << std::endl;
      return;
    }
  string towergeomnodename = "TOWERGEOM_" + detName;
  RawTowerGeomContainer *towergeom = findNode::getClass<RawTowerGeomContainer>(topNode, towergeomnodename.c_str());
  if (! towergeom)
   {
     cout << PHWHERE << ": Could not find node " << towergeomnodename.c_str() << endl;
     return;
   }
  
  // Locate the central tower
  
  float r_dist = 9999.0; 
  int twr_j = -1; 
  int twr_k = -1; 
  RawTowerDefs::CalorimeterId calo_id_ = RawTowerDefs::convert_name_to_caloid( detName );   

  RawTowerContainer::ConstRange begin_end  = towers->getTowers();
  RawTowerContainer::ConstIterator itr = begin_end.first;
  for (; itr != begin_end.second; ++itr)
    {
      RawTowerDefs::keytype towerid = itr->first;
      RawTowerGeom *tgeo = towergeom->get_tower_geometry(towerid); 
      
      float x = tgeo->get_center_x(); 
      float y = tgeo->get_center_y(); 
      
      float temp_rdist = sqrt(pow(tkx-x,2) + pow(tky-y,2)) ; 
      if(temp_rdist< r_dist){
	r_dist = temp_rdist; 
	twr_j = RawTowerDefs::decode_index1(towerid); 
	twr_k = RawTowerDefs::decode_index2(towerid); 
      }
      
      if( (fabs(tkx-x)<(tgeo->get_size_x()/2.0)) &&
	  (fabs(tky-y)<(tgeo->get_size_y()/2.0)) ) break; 

    }

  // Use the central tower to sum up the 5x5 energy

  if(twr_j>=0 && twr_k>=0){
    for(int ij = -2; ij <=2; ij++){
      for(int ik = -2; ik <=2; ik++){
	RawTowerDefs::keytype temp_towerid = RawTowerDefs::encode_towerid( calo_id_ , twr_j + ij , twr_k + ik ); 
	RawTower *rawtower = towers->getTower(temp_towerid);
	if(rawtower) twr_sum += rawtower->get_energy();
      }
    }
  }

  // Set the output
  if(detName == "FEMC") {
    if(!secondFlag){
      femc_E55 = twr_sum;
    }
    else{
      femc_E552 = twr_sum;
    }
  }
  else{
    if(!secondFlag){
      fhcal_E55 = twr_sum; 
    }
    else{
      fhcal_E552 = twr_sum; 
     }
  }

}


//----------------------------------------------------------------------------//
//-- reset_variables():
//--   Reset all the tree variables to their default values.
//--   Needs to be called at the start of every event
//----------------------------------------------------------------------------//
void FastTrackingEval::reset_variables() {

	//-- truth
	gtrackID = -9999;
	gflavor = -9999;
	gparentid = -9999; 
	gpx = -9999;
	gpy = -9999;
	gpz = -9999;
	gvx = -9999;
	gvy = -9999;
	gvz = -9999;
	isprim = -9999;

	gflavor2 = -9999;
	gparentid2 = -9999; 
	gpx2 = -9999;
	gpy2 = -9999;
	gpz2 = -9999;
	gvx2 = -9999;
	gvy2 = -9999;
	gvz2 = -9999;
	isprim2 = -9999; 

	//-- reco
	trackID = -9999;
	charge = -9999;
	nhits = -9999;
	px = -9999;
	py = -9999;
	pz = -9999;
	dca2d = -9999;
	femc_cldr = -9999; 
	femc_clE = -9999; 
	femc_E33 = -9999; 
	femc_E55 = -9999; 
	femc_clN = -1; 
	fhcal_cldr = -9999; 
	fhcal_clE = -9999; 
	fhcal_E33 = -9999; 
	fhcal_E55 = -9999; 
	fhcal_clN= -1; 
	track_E_over_p = -9999.0;  
	femc_plen = -9999; 
	fhcal_plen = -9999; 

	charge2 = -9999;
	nhits2 = -9999;
	px2 = -9999;
	py2 = -9999;
	pz2 = -9999;
	dca2d2 = -9999;
	femc_cldr2 = -9999; 
	femc_clE2 = -9999; 
	femc_E332 = -9999; 
	femc_E552 = -9999; 
	femc_clN2 = -1; 
	fhcal_cldr2 = -9999; 
	fhcal_clE2 = -9999; 
	fhcal_E332 = -9999; 
	fhcal_E552 = -9999; 
	fhcal_clN2 = -1; 
	track_E_over_p2 = -9999.0;  
	femc_plen2 = -9999; 
	fhcal_plen2 = -9999; 
	
	mass = -9999; 
	gmass = -9999; 
}

//----------------------------------------------------------------------------//
//-- GetNodes():
//--   Get all the all the required nodes off the node tree
//----------------------------------------------------------------------------//
int FastTrackingEval::GetNodes(PHCompositeNode * topNode) {
	//DST objects
	//Truth container
	_truth_container = findNode::getClass<PHG4TruthInfoContainer>(topNode,
			"G4TruthInfo");
	if (!_truth_container && _event < 2) {
		cout << PHWHERE << " PHG4TruthInfoContainer node not found on node tree"
				<< endl;
		return Fun4AllReturnCodes::ABORTEVENT;
	}

	_trackmap = findNode::getClass<SvtxTrackMap>(topNode,
			"SvtxTrackMap");
	if (!_trackmap && _event < 2) {
		cout << PHWHERE << "SvtxTrackMap node not found on node tree"
				<< endl;
		return Fun4AllReturnCodes::ABORTEVENT;
	}

	// create this object first time through, reset it after that
	if(svtx_eval)
	  svtx_eval->next_event(topNode); 
	else
	  svtx_eval = new SvtxTrackEval(topNode); 

	return Fun4AllReturnCodes::EVENT_OK;
}


void FastTrackingEval::GetCalorimeterIncident(PHCompositeNode *topNode, 
					      string detName, 
					      RawTowerContainer *towers, 
					      RawTowerGeomContainer *towergeom, 
					      PHG4HitContainer *_g4hits, 
					      std::vector<ParticleTower *> &calo_incident){
  
  RawTowerDefs::CalorimeterId calo_id_ = RawTowerDefs::convert_name_to_caloid( detName );   

  // For CEMC:
  PHG4CylinderCellGeom_Spacalv1 *geo = NULL; 
  const PHG4CylinderGeom_Spacalv3 *layergeom = NULL; 
  bool cemc_first = true; 

  // Generate a map of g4hits associated with each tower
  
  std::map<RawTowerDefs::keytype, std::vector<PHG4Hit*>> TowerHitList; 

  PHG4HitContainer::ConstIterator hiter;
  PHG4HitContainer::ConstRange hit_begin_end = _g4hits->getHits();
  for (hiter = hit_begin_end.first; hiter != hit_begin_end.second; hiter++){

    PHG4Hit* g4hit_i =  hiter->second ;
      
    // Don't include hits with zero energy
    if(g4hit_i->get_edep()<=0) continue; 

    /* encode CaloTowerID from j, k index of tower / hit and calorimeter ID */

    // Forward Calorimeters
    // RawTowerDefs::keytype calotowerid = RawTowerDefs::encode_towerid( calo_id_ ,
    // 								      g4hit_i->get_index_j() ,
    // 								      g4hit_i->get_index_k() );
    
    // Central Barrel

    // HCAL
    RawTowerDefs::keytype calotowerid;

    if( (detName=="HCALIN") || (detName=="HCALOUT")){
    
      short icolumn = hiter->second->get_scint_id();
      int introw = (hiter->second->get_hit_id() >> PHG4HitDefs::hit_idbits);
      
      // Should probably pull this from some constants somewhere
      if( detName=="HCALIN"){
        calotowerid = RawTowerDefs::encode_towerid( calo_id_ ,
						  icolumn ,
						  introw/4 );
      }
      else{
        calotowerid = RawTowerDefs::encode_towerid( calo_id_ ,
						  icolumn ,
						  introw/5 );
      }

    }
    else if(detName=="CEMC"){

      // ONLY VALID FOR Cemc_spacal_configuration == PHG4CylinderGeom_Spacalv1::k2DProjectiveSpacal !!!
      // Go to the cell id, and from there to the tower id

      if(cemc_first){
	PHG4CylinderCellGeomContainer *seggeo = findNode::getClass<PHG4CylinderCellGeomContainer>(topNode, "CYLINDERCELLGEOM_CEMC");
	if (!seggeo)
	  {
	    cout << "FastTrackingEval::GetCalorimeterIncident - Fatal Error - could not locate cell geometry node CYLINDERCELLGEOM_CEMC" << endl;
	    exit(1);
	  }
	PHG4CylinderGeomContainer *layergeo = findNode::getClass<PHG4CylinderGeomContainer>(topNode, "CYLINDERGEOM_CEMC");
	if (!layergeo)
	  {
	    cout << "FastTrackingEval::GetCalorimeterIncident - Fatal Error - Could not locate sim geometry node CYLINDERGEOM_CEMC" << endl;
	    exit(1);
	  }
	PHG4CylinderCellGeom *geo_raw = seggeo->GetFirstLayerCellGeom();
	geo = dynamic_cast<PHG4CylinderCellGeom_Spacalv1 *>(geo_raw);
	assert(geo);
	const PHG4CylinderGeom *layergeom_raw = layergeo->GetFirstLayerGeom();
	assert(layergeom_raw);
	// a special implimentation of PHG4CylinderGeom is required here.
	layergeom = dynamic_cast<const PHG4CylinderGeom_Spacalv3 *>(layergeom_raw);
	assert(layergeom);
	cemc_first = false; 
      }

      int scint_id = hiter->second->get_scint_id();

      // decode scint_id
      PHG4CylinderGeom_Spacalv3::scint_id_coder decoder(scint_id);

      // convert to z_ID, phi_ID
      std::pair<int, int> tower_z_phi_ID = layergeom->get_tower_z_phi_ID(decoder.tower_ID, decoder.sector_ID);
      const int & tower_ID_z = tower_z_phi_ID.first;
      const int & tower_ID_phi = tower_z_phi_ID.second;

      PHG4CylinderGeom_Spacalv3::tower_map_t::const_iterator it_tower =
	layergeom->get_sector_tower_map().find(decoder.tower_ID);
      assert(it_tower != layergeom->get_sector_tower_map().end());

      int etabin = -1;
      try
	{
	  etabin = geo->get_etabin_block(tower_ID_z); // block eta bin
	}
      catch (exception & e)
	{
	  cout << "Print cell geometry:" << endl;
	  geo->identify();
	  cout << "Print scint_id_coder:" << endl;
	  decoder.identify();
	  cout << "Print the hit:" << endl;
	  hiter->second->print();
	  cout << "FastTrackingEval::GetCalorimeterIncident::process_event::"
	       << Name() << " - Fatal Error - " << e.what() << endl;
	  exit(1);
	}

      const int sub_tower_ID_x = it_tower->second.get_sub_tower_ID_x(decoder.fiber_ID);
      const int sub_tower_ID_y = it_tower->second.get_sub_tower_ID_y(decoder.fiber_ID);
      unsigned short fiber_ID = decoder.fiber_ID;
      unsigned short etabinshort  =  etabin * layergeom->get_n_subtower_eta() + sub_tower_ID_y;
      unsigned short phibin = tower_ID_phi * layergeom->get_n_subtower_phi() + sub_tower_ID_x;

      PHG4CellDefs::keytype cellkey = PHG4CellDefs::SpacalBinning::genkey(etabinshort,phibin,fiber_ID);

      short int firstpar = PHG4CellDefs::SpacalBinning::get_etabin(cellkey);
      short int secondpar =  PHG4CellDefs::SpacalBinning::get_phibin(cellkey);

      calotowerid = RawTowerDefs::encode_towerid( calo_id_ ,
						  firstpar ,
						  secondpar );

    }
    
    TowerHitList[calotowerid].push_back(g4hit_i); 
  }

  // Now that we have the map, loop over each tower and search the hits
  // for a particle that originated outside of the front face of the CEMC
  // Why the CEMC? We are interested in generating a list of hits incident on the 
  // full forward calorimeter system. If we just used the calorimeter we are searching
  // we would get a large hit list for showers propagating between the CEMC and IHCAL

  RawTowerContainer::ConstRange begin_end  = towers->getTowers();
  RawTowerContainer::ConstIterator itr = begin_end.first;
  for (; itr != begin_end.second; ++itr){

    RawTowerDefs::keytype towerid = itr->first;    
    RawTower *ctower = towers->getTower(towerid);

    float revised_tower_E = 0.0; 

    std::map<RawTowerDefs::keytype, std::vector<PHG4Hit*>>::iterator fitr; 
    fitr = TowerHitList.find(towerid); 
    if(fitr != TowerHitList.end()){

      // Loop over the hits and look for a candidate particle

      std::vector<PHG4Hit*> truth_hits = (*fitr).second;
      
      // Sum up the G4 hit energy for this tower first
      float sumTwrE = 0.0; 
      std::vector<PHG4Hit*>::iterator vitr = truth_hits.begin(); 
      for(; vitr != truth_hits.end(); vitr++){
	PHG4Hit* g4hit  = *vitr;
	sumTwrE += g4hit->get_light_yield(); 
      }

      vitr = truth_hits.begin(); 
      for(; vitr != truth_hits.end(); vitr++){

	PHG4Hit* g4hit  = *vitr;
	PHG4Particle* particle = _truth_container->GetParticle(g4hit->get_trkid());
	PHG4VtxPoint* vtx = _truth_container->GetVtx( particle->get_vtx_id() );
       
	// Trace back until we get a particle that enters the CEMC 
	// or the chain runs out. 

	while( (sqrt(pow(vtx->get_x(),2)+pow(vtx->get_y(),2))>_cemc_radius) && particle ){
        
	  int parentid = particle->get_parent_id();
	  particle = _truth_container->GetParticle(parentid);
	  if(particle) vtx = _truth_container->GetVtx( particle->get_vtx_id() );

	}

	if(particle){

	  // Tower information we will need

	  RawTowerGeom *tgeo = towergeom->get_tower_geometry(towerid); 

	  float x = tgeo->get_center_x(); 
	  float y = tgeo->get_center_y(); 
	  float z = tgeo->get_center_z(); 

	  float eta = getEta(sqrt(pow(x,2)+pow(y,2)),z-vtx_z); 
	  float phi = XYtoPhi(x,y); 
 
	  // cout << " light yield = " << g4hit->get_light_yield() << " tower sum = " 
	  //      << sumTwrE << " calibrated energy = " << ctower->get_energy() 
	  //      << " energy fraction = " << (g4hit->get_light_yield()/sumTwrE)*ctower->get_energy() << endl; 
	  
	  double pt = 0.0;
	  double px = 0.0;
	  double py = 0.0;
	  double pz = 0.0;
	  float sf = 1.0;

	  if((detName=="CEMC") && rescale_cemc){

	    // Set the scale factor to rescale the CEMC energy based on whether or not this contribution
	    // is EM or hadronic
	    
	    if( !((fabs(particle->get_pid())==11) || (particle->get_pid()==22) || 
		  (particle->get_pid()==111) || (particle->get_pid()==221)) ){
	      // hadronic energy (e/h ratio)
	      sf = cemc_rescale_factor; 
	    }

	  }
	  else if( ((detName=="HCALIN")||(detName=="HCALOUT")) && rescale_hcal){
	    
	    if( !((fabs(particle->get_pid())==11) || (particle->get_pid()==22) || 
		  (particle->get_pid()==111) || (particle->get_pid()==221)) ){
	      // hadronic energy (correct for binding energy losses)
	      sf = hcal_rescale_factor;  
	    }

	  }

	  pt = (g4hit->get_light_yield()/sumTwrE)*sf*ctower->get_energy() / cosh(eta);
	  px = pt * cos(phi);
	  py = pt * sin(phi);
	  pz = pt * sinh(eta);

	  revised_tower_E += (g4hit->get_light_yield()/sumTwrE)*sf*ctower->get_energy(); 

	  // Is this particle already in the list? 
	  bool not_found = true; 
	  std::vector<ParticleTower *>::iterator pt_it;
	  for (pt_it = calo_incident.begin(); pt_it != calo_incident.end(); ++pt_it){
	    if((*pt_it)->particle == particle){
	      (*pt_it)->TowerIDs.push_back(towerid);
	      (*pt_it)->px.push_back(px); 
	      (*pt_it)->py.push_back(py); 
	      (*pt_it)->pz.push_back(pz); 
	      not_found = false;
	      break; 
	    }
	  }
	  if(not_found){
	    // not found - create a new structure and add to the list
	    ParticleTower *my_pt = new ParticleTower(); 
	    my_pt->particle = particle; 
	    my_pt->TowerIDs.clear(); 
	    my_pt->TowerIDs.push_back(towerid);
	    my_pt->px.push_back(px); 
	    my_pt->py.push_back(py); 
	    my_pt->pz.push_back(pz); 
	    calo_incident.push_back(my_pt); 
	  }

	}

      }

      // set the tower to the rescaled energy 
      if( rescale_cemc || rescale_hcal) ctower->set_energy( revised_tower_E );

      // clear the hit list for this tower
      truth_hits.clear(); 
    }

  }
  
  // Clear the tower hit list
  TowerHitList.clear(); 

}

void FastTrackingEval::getClusterDistanceByIndex(string detName, SvtxTrack* track, int secondFlag ){


  if(detName == "CEMC"){
    float min_r = sqrt(pow(track->get_cal_dphi(SvtxTrack::CEMC),2) + pow(track->get_cal_deta(SvtxTrack::CEMC),2) );
    if(!secondFlag){
      cemc_cldr = min_r;   
    }
    else{
      cemc_cldr2 = min_r; 
    }
  }
  else if(detName == "HCALIN"){
    float min_r = sqrt(pow(track->get_cal_dphi(SvtxTrack::HCALIN),2) + pow(track->get_cal_deta(SvtxTrack::HCALIN),2) );
    if(!secondFlag){
      hcalin_cldr = min_r; 
    }
    else{
      hcalin_cldr2 = min_r; 
    }
  }
  else if(detName == "HCALOUT"){
    float min_r = sqrt(pow(track->get_cal_dphi(SvtxTrack::HCALOUT),2) + pow(track->get_cal_deta(SvtxTrack::HCALOUT),2) );
    if(!secondFlag){
      hcalout_cldr = min_r; 
    }
    else{
      hcalout_cldr2 = min_r; 
    }
  }

}

void FastTrackingEval::GetDiElectronPairs(PHCompositeNode *topNode, HepMC::GenParticle *electron, HepMC::GenParticle *positron)
{

  for (SvtxTrackMap::ConstIter track_itr = _trackmap->begin();
       track_itr != _trackmap->end(); track_itr++) {

    SvtxTrack* track = dynamic_cast<SvtxTrack*>(track_itr->second);

    // Run the evaluator to get the origin of this track 
    
    PHG4Particle* g4particle = svtx_eval->max_truth_particle_by_nclusters(track); 

    if(g4particle){
 
      gflavor = g4particle->get_pid();
      gparentid = GetParticleParentPID(topNode, g4particle); 

      gpx = g4particle->get_px();
      gpy = g4particle->get_py();
      gpz = g4particle->get_pz();

      PHG4VtxPoint *vtx = _truth_container->GetVtx(g4particle->get_vtx_id()); 
      gvx = vtx->get_x(); 
      gvy = vtx->get_y(); 
      gvz = vtx->get_z(); 
	  
      isprim = _truth_container->is_primary(g4particle); 
    
    }
    else{

      gflavor = -9999.0;
      gparentid = -9999.0; 

      gpx = -9999.0;
      gpy = -9999.0;
      gpz = -9999.0;

      gvx = -9999.0; 
      gvy = -9999.0; 
      gvz = -9999.0; 
	  
      isprim = 0; 

    }

    trackID = track->get_id();
    charge = track->get_charge();
    nhits = track->get_quality();

    px = track->get_px();
    py = track->get_py();
    pz = track->get_pz();
    dca2d = track->get_dca2d();
    eta = getEta(sqrt(pow(track->get_px(),2)+pow(track->get_py(),2)), track->get_pz()); 

    chi2 = track->get_chisq(); 
    ndf = track->get_ndf(); 
    cemc_clid = track->get_cal_cluster_id(SvtxTrack::CEMC); 

    cemc_E33 = track->get_cal_energy_3x3(SvtxTrack::CEMC); 
    cemc_E33_PC = PositionCorrectEMCal3x3(topNode, track->get_cal_cluster_id(SvtxTrack::CEMC), cemc_E33); 
    hcalin_E33 = track->get_cal_energy_3x3(SvtxTrack::HCALIN); 
    hcalout_E33 = track->get_cal_energy_3x3(SvtxTrack::HCALOUT); 

    getClusterByIndex(topNode, "CEMC", track->get_cal_cluster_id(SvtxTrack::CEMC)); 
    getClusterPCByIndex(topNode, track->get_cal_cluster_id(SvtxTrack::CEMC)); 
    if(use_ihcal){ 
      getClusterByIndex(topNode, "HCALIN", track->get_cal_cluster_id(SvtxTrack::HCALIN)); 
    }
    else{
      hcalin_cldr = 9999.0; 
      hcalin_clE = 0.0; 
      hcalin_clN = 0.0; 
    }
    getClusterByIndex(topNode, "HCALOUT", track->get_cal_cluster_id(SvtxTrack::HCALOUT)); 

    getClusterDistanceByIndex("CEMC",  track ); 
    getClusterDistanceByIndex("HCALIN",  track ); 
    getClusterDistanceByIndex("HCALOUT",  track ); 

    // Does this match a DY primary? 
    DYmatch = 0; 
    if( process==1 && electron && positron && fabs(gflavor)==11 && _truth_container->is_primary(g4particle) ){
      HepMC::GenParticle *match = NULL;
      if(gflavor==11) 
	match = electron; 
      else
	match = positron; 

      if(g4particle->get_barcode()==match->barcode()) 
	DYmatch = 1; 
      else
	DYmatch = 0; 
    }

    // If this is a good track e+/e- candidate, then make pairs with the rest of the tracks
    track_E_over_p = cemc_E33/sqrt(px*px+py*py+pz*pz); 
    track_E_over_p_PC = cemc_E33_PC/sqrt(px*px+py*py+pz*pz); 

    // 3-sigma E/p cut
    if( fabs(track_E_over_p_PC-1.0)>0.350) continue; 
    
    for (SvtxTrackMap::ConstIter track_itr2 = track_itr;
	 track_itr2 != _trackmap->end(); track_itr2++) {

      if(track_itr2==track_itr) continue; 
      SvtxTrack* track2 = dynamic_cast<SvtxTrack*>(track_itr2->second);

      PHG4Particle* g4particle2 = svtx_eval->max_truth_particle_by_nclusters(track2); 

      if(g4particle2){

	gflavor2 = g4particle2->get_pid();
	gparentid2 = GetParticleParentPID(topNode, g4particle2); 

	gpx2 = g4particle2->get_px();
	gpy2 = g4particle2->get_py();
	gpz2 = g4particle2->get_pz();

	PHG4VtxPoint *vtx2 = _truth_container->GetVtx(g4particle2->get_vtx_id()); 
	gvx2 = vtx2->get_x(); 
	gvy2 = vtx2->get_y(); 
	gvz2 = vtx2->get_z(); 
	  
	isprim2 = _truth_container->is_primary(g4particle2); 
	
      }
      else{

	gflavor2 = -9999.0;
	gparentid2 = -9999.0; 

	gpx2 = -9999.0;
	gpy2 = -9999.0;
	gpz2 = -9999.0;

	gvx2 = -9999.0; 
	gvy2 = -9999.0; 
	gvz2 = -9999.0; 
	  
	isprim2 = 0; 

      }

      charge2 = track2->get_charge();
      nhits2 = track2->get_quality();

      px2 = track2->get_px();
      py2 = track2->get_py();
      pz2 = track2->get_pz();
      dca2d2 = track2->get_dca2d();
      eta2 = getEta(sqrt(pow(track2->get_px(),2)+pow(track2->get_py(),2)), track2->get_pz()); 
	
      chi22 = track2->get_chisq(); 
      ndf2 = track2->get_ndf(); 
      cemc_clid2 = track2->get_cal_cluster_id(SvtxTrack::CEMC); 

      cemc_E332 = track2->get_cal_energy_3x3(SvtxTrack::CEMC); 
      cemc_E332_PC = PositionCorrectEMCal3x3(topNode, track2->get_cal_cluster_id(SvtxTrack::CEMC), cemc_E332); 
      hcalin_E332 = track2->get_cal_energy_3x3(SvtxTrack::HCALIN); 
      hcalout_E332 = track2->get_cal_energy_3x3(SvtxTrack::HCALOUT); 

      getClusterByIndex(topNode, "CEMC",  track2->get_cal_cluster_id(SvtxTrack::CEMC),1); 
      getClusterPCByIndex(topNode,  track2->get_cal_cluster_id(SvtxTrack::CEMC),1); 
      if(use_ihcal){
	getClusterByIndex(topNode, "HCALIN",  track2->get_cal_cluster_id(SvtxTrack::HCALIN),1);
      }
      else{
        hcalin_cldr2 = 9999.0; 
        hcalin_clE2 = 0.0; 
        hcalin_clN2 = 0.0; 
      }
      getClusterByIndex(topNode, "HCALOUT",  track2->get_cal_cluster_id(SvtxTrack::HCALOUT),1); 

      getClusterDistanceByIndex("CEMC",  track2, 1); 
      getClusterDistanceByIndex("HCALIN",  track2, 1); 
      getClusterDistanceByIndex("HCALOUT",  track2, 1); 

      // Does this match a DY primary? 
      DYmatch2 = 0; 
      if( process==1 && electron && positron && fabs(gflavor2)==11 && _truth_container->is_primary(g4particle2) ){
	HepMC::GenParticle *match = NULL;
	if(gflavor2==11) 
	  match = electron; 
	else
	  match = positron; 

	if(g4particle2->get_barcode()==match->barcode()) 
	  DYmatch2 = 1; 
	else
	  DYmatch2 = 0; 
      }
	      
      // Is this a true DY pair? 
      if(DYmatch && DYmatch2) 
	true_pair = 1;
      else
	true_pair = 0; 

      track_E_over_p2 = cemc_E332/sqrt(px2*px2+py2*py2+pz2*pz2); 
      track_E_over_p2_PC = cemc_E332_PC/sqrt(px2*px2+py2*py2+pz2*pz2); 
		      
      // 3-sigma E/p cut
      if( fabs(track_E_over_p2_PC-1.0)>0.350) continue; 

      float E1 = sqrt(pow(0.511e-3,2) + px*px + py*py + pz*pz); 
      float E2 = sqrt(pow(0.511e-3,2) + px2*px2 + py2*py2 + pz2*pz2);
 
      mass = sqrt( pow(E1+E2,2) - pow(px+px2,2) - pow(py+py2,2) - pow(pz+pz2,2)); 

      E1 = sqrt(pow(0.511e-3,2) + gpx*gpx + gpy*gpy + gpz*gpz); 
      E2 = sqrt(pow(0.511e-3,2) + gpx2*gpx2 + gpy2*gpy2 + gpz2*gpz2);
 
      gmass = sqrt( pow(E1+E2,2) - pow(gpx+gpx2,2) - pow(gpy+gpy2,2) - pow(gpz+gpz2,2)); 
		
      _eval_tree_pairs->Fill();

    }

  }
		  
  return;

}

