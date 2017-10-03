/*!
 *  \file		FastTrackingEval.h
 *  \brief		Evaluation module for PHG4TrackFastSim output
 *  \details	input: PHG4TruthInfoContainer, SvtxTrackMap with SvtxTrack_FastSim inside
 *  \author		Haiwang Yu <yuhw@nmsu.edu>
 */

#ifndef __FastTrackingEval_H__
#define __FastTrackingEval_H__

#include <fun4all/SubsysReco.h>
#include <string>
#include <vector>

#include <g4cemc/RawTowerDefs.h>

//Forward declerations
class PHCompositeNode;
class PHG4TruthInfoContainer;
class PHG4Particle; 
class SvtxClusterMap;
class SvtxTrackMap;
class SvtxTrack; 
class TFile;
class TTree;
class TH2D;
class TRandom; 
class RawTowerContainer; 
class RawTowerGeomContainer; 
class PHG4CylinderCellContainer; 
class PHG4HitContainer; 
class SvtxTrackEval;

// Structure to hold an incident particle and a 
// list of towers it contributed to. 

typedef struct{
  PHG4Particle *particle; 
  std::vector<RawTowerDefs::keytype> TowerIDs; 
  std::vector<float> px; 
  std::vector<float> py; 
  std::vector<float> pz; 
} ParticleTower; 

namespace PHGenFit {
class Fitter;
} /* namespace PHGenFit */

namespace HepMC {
class GenParticle;
} /* namespace HepMC */

namespace fastjet {
class PseudoJet;
} /* namespace fastjet */

//Brief: basic ntuple and histogram creation for sim evaluation
class FastTrackingEval: public SubsysReco
{
 public: 
  //Default constructor
  FastTrackingEval(const std::string &name="FastTrackingEval");

  //Initialization, called for initialization
  int Init(PHCompositeNode *);

  int InitRun(PHCompositeNode *); 

  //Process Event, called for each event
  int process_event(PHCompositeNode *);

  //End, write and close files
  int End(PHCompositeNode *);

  //Change output filename
  void set_filename(const char* file)
  { if(file) _outfile_name = file; }

  //Flags of different kinds of outputs
  enum Flag
  {
    //all disabled
    NONE = 0,
  };

  //Set the flag
  //Flags should be set like set_flag(FastTrackingEval::TRUTH, true) from macro
  void set_flag(const Flag& flag, const bool& value)
  {
   if(value) _flags |= flag;
   else _flags &= (~flag);
  }

  // set mode
  void SetPythia6(){pythia6 = true;}
  void SetPythia8(){pythia6 = false;}

  // use IHCAL in tower jets
  void UseIHCAL(bool val){use_ihcal = val;}

  // rescale CEMC/HCAL towers
  void RescaleCEMCTowers(bool val){rescale_cemc = val;}
  void RescaleHCALTowers(bool val){rescale_hcal = val;}

  void SetCEMCRescaleFactor(float val){cemc_rescale_factor = val;}
  void SetHCALRescaleFactor(float val){hcal_rescale_factor = val;}

 private:
  //output filename
  std::string _outfile_name;
   
  //Event counter
  int _event;

  //User modules
  void fill_tree(PHCompositeNode*);
  void reset_variables();

  //Get all the nodes
  int GetNodes(PHCompositeNode *);

  //Projections
  void getCluster( PHCompositeNode *topNode, std::string detName, float eta, float phi, int secondFlag=0 ); 
  void getClusterByIndex( PHCompositeNode *topNode, std::string detName, int clIdx, int secondFlag=0 ); 
  void getClusterPCByIndex( PHCompositeNode *topNode, int clIdx, int secondFlag=0 ); 
  float PositionCorrectEMCal3x3( PHCompositeNode *topNode, int clIdx, float e33); 
  void getE33( PHCompositeNode *topNode, std::string detName, float x, float y, int secondFlag=0 ); 
  void getE55( PHCompositeNode *topNode, std::string detName, float x, float y, int secondFlag=0 ); 
  void getEBarrel( PHCompositeNode *topNode, std::string detName, float x, float y, float z, int secondFlag=0 ); 
  bool getHighestEnergyCluster( PHCompositeNode *topNode, std::string detName, float &cl_eta, float &cl_phi ); 
  void getClusterDistanceByIndex(std::string detName, SvtxTrack* track, int secondFlag=0 ); 

  //Sum cluster energy
  //void sumClusterEnergy( PHCompositeNode *topNode, std::string detName ); 
  
  // Di-Electron Pairs
  void GetDiElectronPairs( PHCompositeNode *topNode, HepMC::GenParticle *electron, HepMC::GenParticle *positron); 

  // Track Jets
  void GetPrimaryJets( PHCompositeNode *topNode, std::vector<fastjet::PseudoJet> &primary_jets); 

  // Track Jets
  void GetTrackJets( PHCompositeNode *topNode, std::vector<fastjet::PseudoJet> &inclusive_jets); 

  // Sum tower energy
  void sumTowerEnergy( PHCompositeNode *topNode, std::string detName ); 

  // Find parent (secondary or primary)
  int GetParticleParentPID( PHCompositeNode *topNode, PHG4Particle* g4particle ); 

  // Get a list of jet contributors and their energy fraction 
  void GetJetPrimaryContributors( PHCompositeNode *topNode, PHG4Particle* g4particle ); 
  void TraceHepMCParticle( HepMC::GenParticle *ptcle ); 

  // Tower Jets: 
  void FillTowerPseudoJets( RawTowerContainer *towers, RawTowerGeomContainer *towergeom,
			    std::vector<fastjet::PseudoJet> &pseudojets,  const float scale_factor); 
  void GetTowerJets( PHCompositeNode *topNode, std::vector<fastjet::PseudoJet> &inclusive_jets); 

  void GetCalorimeterIncident(PHCompositeNode *topNode, std::string detName, RawTowerContainer* towers, RawTowerGeomContainer *towergeom,
			      PHG4HitContainer *_g4hits, 
			      std::vector<ParticleTower *> &calo_incident); 

  //flags
  unsigned int _flags;
  bool pythia6; 
  bool use_ihcal; 
  bool rescale_cemc; 
  bool rescale_hcal;
  float cemc_rescale_factor;
  float hcal_rescale_factor; 

  //TTrees
  TTree* _eval_tree_tracks;
  TTree* _eval_tree_pairs;
  TTree* _eval_tree_event;
  TTree* _eval_tree_prim_jets;
  TTree* _eval_tree_trk_jets;
  TTree* _eval_tree_twr_jets;
  TTree* _eval_tree_matched_jets;
  int event;
  //-- truth
  int gtrackID;
  int gflavor;
  int gparentid; 
  float gpx;
  float gpy;
  float gpz;
  float gvx;
  float gvy;
  float gvz;
  float isprim; 

  int gflavor2;
  int gparentid2; 
  float gpx2;
  float gpy2;
  float gpz2;
  float gvx2;
  float gvy2;
  float gvz2;
  float isprim2; 

  //-- reco
  int trackID;
  int charge;
  int nhits;
  float px;
  float py;
  float pz;
  float dca2d;
  float femc_cldr, femc_clE, femc_clN; 
  float cemc_cldr, cemc_clE, cemc_clN; 
  float cemc_clE_PC; 
  float femc_E33, fhcal_E33, femc_E55, fhcal_E55; 
  float cemc_E33, hcalin_E33, hcalout_E33, cemc_E55, hcalin_E55, hcalout_E55; 
  float fhcal_cldr, fhcal_clE, fhcal_clN; 
  float hcalin_cldr, hcalin_clE, hcalin_clN; 
  float hcalout_cldr, hcalout_clE, hcalout_clN; 
  float track_E_over_p; 
  float track_E_over_p_PC; 
  float femc_plen, fhcal_plen; 
  float cemc_plen, hcalin_plen, hcalout_plen; 
  float chi2; 
  float ndf; 
  float cemc_E33_PC; 
  float cemc_clid;
  float eta; 

  int trackID2;
  int charge2;
  int nhits2;
  float px2;
  float py2;
  float pz2;
  float dca2d2;
  float femc_cldr2, femc_clE2, femc_clN2; 
  float cemc_cldr2, cemc_clE2, cemc_clN2; 
  float cemc_clE2_PC; 
  float femc_E332, femc_E552, fhcal_E332, fhcal_E552; 
  float cemc_E332, cemc_E552, hcalin_E332, hcalin_E552, hcalout_E332, hcalout_E552; 
  float fhcal_cldr2, fhcal_clE2, fhcal_clN2; 
  float hcalin_cldr2, hcalin_clE2, hcalin_clN2; 
  float hcalout_cldr2, hcalout_clE2, hcalout_clN2; 
  float track_E_over_p2; 
  float track_E_over_p2_PC; 
  float femc_plen2, fhcal_plen2; 
  float cemc_plen2, hcalin_plen2, hcalout_plen2; 
  float chi22; 
  float ndf2; 
  float cemc_E332_PC;
  float cemc_clid2; 
  float eta2;

  float mass; 
  float gmass; 
  float femc_sumE; 
  float fhcal_sumE; 
  float ihcal_sumE; 
  float ohcal_sumE; 
  float cemc_sumE; 
  float femc_Ntwr; 
  float fhcal_Ntwr; 
  float ihcal_Ntwr; 
  float ohcal_Ntwr; 
  float cemc_Ntwr; 
  float fwd_ntrk; 

  float x1; 
  float x2; 
  float Q2; 

  int process; 
  int n_mpi; 

  int DYmatch; 
  int DYmatch2; 
  int true_pair; 
  int pairAccepted; 
  int vpart; 

  float cemc_e33; 
  float cemc_e55; 
  float ihcal_e33; 
  float ihcal_e55; 
  float ohcal_e33; 
  float ohcal_e55; 
  
  float vtx_x; 
  float vtx_y; 
  float vtx_z; 
  float vtx_t; 

  float jet_pT; 
  float jet_ptot;
  float jet_E;   
  float jet_tkE;   
  float jet_twrE;   
  float jet_eta; 
  float jet_rapidity; 
  float jet_mass; 
  float jet_phi; 
  float jet_xF; 
  float jet_emfract; 
  int jet_nconstit; 

  float mpjet_ptot;
  float mpjet_eta;
  float mpjet_pT;
  float mpjet_dR; 
  float mpjet_emfract; 
  float mpjet_z_lead; 
  float mpjet_charge_lead; 

  float z_lead; 
  int charge_lead; 

  float z_lead_p; 
  int charge_lead_p; 

  // Matched jet variables

  float jet_p_ptot; 
  float jet_trk_ptot; 
  float jet_twr_ptot; 
  float jet_comb_ptot; 
  float jet_comb_bh_ptot; 
  float jet_p_pT; 
  float jet_trk_pT; 
  float jet_twr_pT; 
  float jet_comb_pT; 
  float jet_comb_bh_pT; 
  float jet_p_eta; 
  float jet_trk_eta; 
  float jet_twr_eta;
  float jet_comb_eta;
  float jet_comb_bh_eta;
  float jet_p_phi; 
  float jet_trk_phi; 
  float jet_twr_phi;
  float jet_comb_phi;
  float jet_comb_bh_phi;
  float jet_p_emfract; 

  float jet_comb_neutral_fract;

  std::vector<double> hadZ; 
  std::vector<double> hadZtrue; 
  
  int jet_p_nconstit; 
  int jet_trk_nconstit; 
  int jet_twr_nconstit; 

  float jet_trk_dR; 
  float jet_twr_dR; 
  
  std::vector<int> pg_prim_id; 
  std::vector<double> pg_prim_fract; 
  std::vector<int> pg_prim_status; 

  std::vector<int> pg_trk_id; 
  std::vector<double> pg_trk_fract; 
  std::vector<int> pg_trk_status; 

  std::vector<int> pg_twr_id; 
  std::vector<double> pg_twr_fract; 
  std::vector<int> pg_twr_status; 

  float prim_z_lead; 
  float trk_z_lead; 
  float twr_z_lead; 
  float prim_charge_lead;
  float trk_charge_lead;
  float twr_charge_lead;

  std::vector<double> prim_constituent_E; 
  std::vector<double> prim_constituent_dR;
  std::vector<double> prim_constituent_pT;
  std::vector<double> prim_constituent_pz;
  std::vector<double> prim_constituent_phi;
  std::vector<double> prim_constituent_z;

  std::vector<double> trk_constituent_E; 
  std::vector<double> trk_constituent_dR;
  std::vector<double> trk_constituent_pT;
  std::vector<double> trk_constituent_pz;
  std::vector<double> trk_constituent_phi;
  std::vector<double> trk_constituent_z;

  std::vector<double> cemc_constituent_E; 
  std::vector<double> cemc_constituent_dR;
  std::vector<double> cemc_constituent_pT;
  std::vector<double> cemc_constituent_pz;
  std::vector<double> cemc_constituent_phi;

  std::vector<double> ihcal_constituent_E; 
  std::vector<double> ihcal_constituent_dR;
  std::vector<double> ihcal_constituent_pT;
  std::vector<double> ihcal_constituent_pz;
  std::vector<double> ihcal_constituent_phi;

  std::vector<double> ohcal_constituent_E; 
  std::vector<double> ohcal_constituent_dR;
  std::vector<double> ohcal_constituent_pT; 
  std::vector<double> ohcal_constituent_pz; 
  std::vector<double> ohcal_constituent_phi; 

  //Node pointers
  PHG4TruthInfoContainer* _truth_container;
  SvtxTrackMap* _trackmap;
  SvtxTrackEval *svtx_eval; 

  //Random number generator
  TRandom *rand; 

  // fitter
  double _cemc_radius; 
  double _ihcal_radius; 
  double _ohcal_radius; 
  PHGenFit::Fitter* _fitter;


};

#endif //* __FastTrackingEval_H__ *//
