# sPHENIX working area

## Table of contents
* [2017/JESvsR_DVPTrees](#JESvsR_DVPTrees)
* [2017/LajoieFirstTrees](#LajoieFirstTrees)
* [2017/GluonQuarkJetsDiscrim](#GluonQuarkJetsDiscrim)
* [2017/TowerScalePionSample](#TowerScalePionSample)
* [2017/JetScaleGammaJetSample_171215collaboMeeting](#JetScaleGammaJetSample_171215collaboMeeting)
* [2017/JetScaleJetSample_171215collaboMeeting](#JetScaleJetSample_171215collaboMeeting)
* [2018/forCDR_Jan](#forCDR_Jan)
* [2018/EoverPcut](#EoverPcut)
* [2018/variousMethodsForScales](#variousMethodsForScales)
* [2018/gammaCrossSection](#gammaCrossSection)
* [2018/forCDR_May](#forCDR_May)
* [2018/CalcScale_diffE_vtx0](#CalcScale_diffE_vtx0)
* [2018/CalcScale_py8jet_vtx10gaus](#CalcScale_py8jet_vtx10gaus)
* [2018/CalcScale_diffE_ETvsPT_fitting](#CalcScale_diffE_ETvsPT_fitting)
* [2018/punchThroughStudy](#punchThroughStudy)
* [2018/KPPcheck](#KPPcheck)
* [2018/CalcScale_truthinfor_pp](#CalcScale_truthinfor_pp)
* [2018/CalcScale_truthinfor_HIJet](#CalcScale_truthinfor_HIJet)
* [2018/forHallman](#forHallman)
* [2018/CalcScale_wTot_Lorentz_pp](#CalcScale_wTot_Lorentz_pp)
* [2018/CalcScale_wTot_Lorentz_HIJet](#CalcScale_wTot_Lorentz_HIJet)
* [2018/CalcScale_wTot_Lorentz_HIJet_frompp](#CalcScale_wTot_Lorentz_HIJet_frompp)
* [2018/TreeProduction](#TreeProduction)
* [2018/CalcScale_reco_py8jet_6set_pp](#CalcScale_reco_py8jet_6set_pp)
* [2018/CalcScale_reco_diffE_pp_nu](#CalcScale_reco_diffE_pp_nu)
* [2018/forTDR_March](#forTDR_March)

<a name="JESvsR_DVPTrees"/></a>
## 2017/JESvsR_DVPTrees
* (201709) JES and JER vs. R from DVP's trees

<a name="LajoieFirstTrees"/></a>
## 2017/LajoieFirstTrees
* (201710) First check on Lajoie's trees

<a name="GluonQuarkJetsDiscrim"/></a>
## 2017/GluonQuarkJetsDiscrim
* (201710) check the discrimination of gluon jet vs. quark jet using Lajoie'sTree

<a name="TowerScalePionSample"/></a>
## 2017/TowerScalePionSample
* (201710-11) extract the calorimeter scaling constants using single pion simulation
* not used: later decided to use jet itself instead of pion sample

<a name="TowerScaleJetSample"/></a>
## 2017/TowerScaleJetSample
* (201710-11) extract the calorimeter scaling constants using dijet simulation
* not used: later decided to scale "Jet E in each Cal Layer" instead of tower by tower (dir name: TowerScale vs. JetScale)

<a name="JetScaleGammaJetSample_171215collaboMeeting"/></a>
## 2017/JetScaleGammaJetSample_171215collaboMeeting
* (201712) extract the calorimeter scaling constants using gamma-jet simulation

<a name="JetScaleJetSample_171215collaboMeeting"/></a>
## 2017/JetScaleJetSample_171215collaboMeeting
* (201712) Apply the scaling constants obtained from Gamma-jet samples to Di-jet samples

<a name="forCDR_Jan"/></a>
## 2018/forCDR_Jan
* (201801) plotting for CDR (not actually used)

<a name="EoverPcut"/></a>
## 2018/EoverPcut
* (201802) some macros to check EoverP cuts and constituents in a jet (input files are wrong?)

<a name="variousMethodsForScales"/></a>
## 2018/variousMethodsForScales
* Until 201803, jet |eta|<0.6 with larger EMCal acceptanc<0.6 with larger EMCal acceptance
* (201803) various methods are tested to determine scaling factors (e.g., asymmetry method, Minuit, hybrid, etc)

<a name="gammaCrossSection"/></a>
## 2018/gammaCrossSection
* From 201804: new |eta| < 0.45 with reduced EMCal acceptance
* (201804) calculate gamma cross-section for realisitc pp data taking 

<a name="punchThroughStudy_May"/></a>
## 2018/punchThroughStudy_May
* (201805) not finalized.. punch through study by Gaussian tail

<a name="forCDR_May"/></a>
## 2018/forCDR_May
* (201805) Actual and final plots for CDR 2018 May. input txt_jes and txt_scale are from XXX
 
<a name="CalcScale_diffE_vtx0"/></a>
## 2018/CalcScale_diffE_vtx0
* (20180410) |eta| < 0.45, vtx = 0 cm, 4 samples with differenc E slice [20, 30, 40, 50, 60] GeV

<a name="CalcScale_py8jet_vtx10gaus"/></a>
## 2018/CalcScale_py8jet_vtx10gaus
* (20180410) |eta| < 0.45, vtx = Gaussian with sigma = 10 cm, one samples from pythia8

<a name="CalcScale_diffE_ETvsPT_fitting"/></a>
## 2018/CalcScale_diffE_ETvsPT_fitting
* (20180410) |eta| < 0.45, 4 samples with differenc E slice [20, 30, 40, 50, 60] GeV
* ET vs PT instead of E, and fit the resolution (for comparison with Dennis results)

<a name="punchThroughStudy"/></a>
## 2018/punchThroughStudy
* (201805) punchThroughStudy (not finalized)

<a name="KPPcheck"/></a>
## 2018/KPPcheck
* (201805) for KPP study (to Jin Huang)

<a name="CalcScale_truthinfor_pp"/></a>
## 2018/CalcScale_truthinfor_pp
* (20180705) use truth jet energy for calibration instead of gamma energy

<a name="CalcScale_truthinfor_HIJet"/></a>
## 2018/CalcScale_truthinfor_HIJet
* (20180705) use truth jet energy for calibration instead of gamma energy

<a name="forHallman"/></a>
## 2018/forHallman
* (201807) supplementary plot to Hallman

<a name="CalcScale_wTot_Lorentz_pp"/></a>
## 2018/CalcScale_wTot_Lorentz_pp
* (201809) Introduce Scaling Constant for total Energy, use Lorentz vector to add pT info into tree
* most recent result!!!

<a name="CalcScale_wTot_Lorentz_HIJet"/></a>
## 2018/CalcScale_wTot_Lorentz_HIJet
* (201809) Introduce Scaling Constant for total Energy, use Lorentz vector to add pT info into tree
* most recent result!!!

<a name="CalcScale_wTot_Lorentz_HIJet_frompp"/></a>
## 2018/CalcScale_wTot_Lorentz_HIJet_frompp
* (201809) use pp scaling constants to HI
* not used

<a name="TreeProduction"/></a>
## 2018/TreeProduction
* (201807) Tree production (sortedRootFiles, 2ndSortedRootFiles)
* most recent result!!!

<a name="CalcScale_reco_py8jet_6set_pp"/></a>
## 2018/CalcScale_reco_py8jet_6set_pp
* (201810) try to use reco jet Pt binning instead of truth jet Pt as real data to apply scaling constant.

<a name="CalcScale_reco_diffE_pp_nu"/></a>
## 2018/CalcScale_reco_diffE_pp_nu
* (201810) try to use reco jet Pt binning instead of truth jet Pt as real data to apply scaling constant.
* Not working yet

<a name="forTDR_March"/></a>
## 2018/forTDR_March
* (201903) Plots for TDR (simply re-arrange 2018 CDR plots)


