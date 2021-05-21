#ifndef ttWSelector_h
#define ttWSelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH1.h>
#include <TMath.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <vector>
#include <string>
#include <map>


class ttWSelector : public TSelector {
 public :
  TTree          *fChain;

  Int_t isMC;

  bool debug;
  bool doWeight;
  bool noMCFakes;

  int   EventsTot;
  float sumWeights;
  float lumi;

  std::unique_ptr<TFile> fOut;
  TString outFileName;
  TString outPathName;

  TString GenName = "";
  TString MCcamp  = "";

  const std::vector<float> lepPtCuts = {30., 20., 15.};

  std::map<std::string, int> lepTypes = {{"Prompt",0}, {"ChMisID",0}, {"HF",0}, {"LF",0}, {"PhConv",0}, {"Tau",0},{"Unknown",0}};

  std::vector<TH1F*> hOut;

  std::vector<int>     cutflowEvents;
  std::vector<float>   cutflowWeights;
  std::vector<TString> cutflowNames;

  static constexpr float MZ = 91.1876;
  static constexpr float MW = 80.3850;

  const std::vector<float> bins_njets   = {1., 2., 3., 4., 5., 6., 7., 10.};
  const std::vector<float> bins_lepPt   = {0.0, 20., 50, 100, 250, 500.};
  const std::vector<float> bins_topPt   = {0.0, 100., 180., 360., 1650.};
  const std::vector<float> bins_dPhiZt  = {0.0, 0.34, 0.82, 1.0};
  const std::vector<float> bins_dRapZt  = {0.0, 0.57, 1.8, 4.5};
  const std::vector<float> bins_dPhill  = {0.0, 0.2, 0.4, 0.6, 0.8, 1.0};
  const std::vector<float> bins_ZRap    = {0.0, 0.10, 0.25, 0.42, 0.64, 0.84, 1.08, 1.33, 2.50};
  const std::vector<float> bins_ZPt     = {0.0, 40., 70., 110., 160., 220., 290., 1000.};
  const std::vector<float> bins_dPhitt  = {0.0, 0.31, 0.59, 0.80, 1.0};
  const std::vector<float> bins_dPhiZtt = {0.0, 0.73, 0.93, 1.0};
  const std::vector<float> bins_ttPt    = {0.0, 67., 140., 236, 500.};

  // Declaration of leaf types
  Double_t        pt_lep1_pt;
  Double_t        m_lep0_pt;
  Double_t        pt_jet0_jvt;
  Double_t        m_lep0_bdtscore;
  Double_t        m_lep1_dPhi_ll;
  Double_t        m_lep2_phi;
  Double_t        nBTags;
  Double_t        tag_jet1_tagweightbin;
  Double_t        pt_lep1_isTight;
  Double_t        m_lep1_iff;
  Double_t        tag_jet2_eta;
  Double_t        pt_lep2_charge;
  Double_t        weight_leptonSF_EL_SF_Trigger_UP;
  Double_t        weight_leptonSF_EL_SF_ID_DOWN;
  Double_t        tag_jet3_eta;
  Double_t        tag_jet2_pt;
  Double_t        tag_jet4_eta;
  Double_t        m_lep2_iff;
  Double_t        tag_jet0_btagWeight;
  Double_t        dEta_bdt;
  Double_t        tag_jet0_pt;
  Double_t        m_lep0_type;
  Double_t        bdt_lep0_phi;
  Double_t        pt_lep1_dRbjet0;
  Double_t        pt_lep1_dRbjet1;
  Double_t        bdt_lep1_mlb0;
  Double_t        bdt_lep1_mlb1;
  Double_t        pt_jet0_phi;
  Double_t        weight_leptonSF_MU_SF_ID_SYST_UP;
  Double_t        NZ_Cands;
  Double_t        pt_jet1_pt;
  Double_t        pt_jet4_btagWeight;
  Double_t        weight_leptonSF_MU_SF_TTVA_STAT_DOWN;
  Double_t        pt_jet3_phi;
  Double_t        weight_leptonSF_EL_SF_Isol_DOWN;
  Double_t        weight_globalLeptonTriggerSF_EL_Trigger_UP;
  Double_t        m_lep0_charge;
  Double_t        tag_jet2_btagWeight;
  Double_t        tag_jet2_jvt;
  Double_t        weight_leptonSF_MU_SF_Isol_STAT_UP;
  Double_t        weight_bTagSF_DL1r_77_eigenvars_C_down;
  Double_t        pt_lep1_mlb1;
  Double_t        pt_lep1_mlb0;
  Double_t        bdt_lep0_eta;
  Double_t        tag_jet5_tagweightbin;
  Double_t        m_lep2_pt;
  Double_t        pt_lep1_mlj;
  Double_t        bdt_lep0_mlb1;
  Double_t        bdt_lep0_mlb0;
  Double_t        m_lep1_mlj;
  Double_t        mumue_2016;
  Double_t        tag_jet3_E;
  Double_t        m_lep1_dRbjet0;
  Double_t        m_lep1_dRbjet1;
  Double_t        m_lep1_eta;
  Double_t        m_lep1_bdtscore;
  Double_t        pt_lep2_pt;
  Double_t        tag_jet5_E;
  Double_t        tag_jet1_phi;
  Double_t        tag_jet1_E;
  Double_t        weight_pileup_UP;
  Double_t        weight_leptonSF_EL_SF_Reco_UP;
  Double_t        pt_jet2_pt;
  Double_t        weight_bTagSF_DL1r_77_eigenvars_Light_down;
  Double_t        pt_jet4_pt;
  Double_t        totC;
  Double_t        dEta_mlb;
  Double_t        pt_lep0_dPhi_ll;
  Double_t        tag_jet4_btagWeight;
  Double_t        m_lep1_mlb1;
  Double_t        m_lep1_mlb0;
  Double_t        pt_lep2_origin;
  Double_t        HT;
  Double_t        eemu_2016;
  Double_t        pt_lep1_type;
  Double_t        pt_lep2_phi;
  Double_t        bdt_lep0_pt;
  Double_t        pt_lep0_bdtscore;
  Double_t        tag_jet5_phi;
  Double_t        pt_lep0_type;
  Double_t        m_lep2_bdtscore;
  Double_t        tag_jet4_E;
  Double_t        pt_lep1_eta;
  Double_t        pt_jet1_tagweightbin;
  Double_t        pt_jet3_E;
  Double_t        weight_leptonSF_EL_SF_Trigger_DOWN;
  Double_t        weight_leptonSF_MU_SF_Isol_SYST_DOWN;
  Double_t        mumumu_2016;
  Double_t        pt_lep0_dRbjet1;
  Double_t        pt_lep0_dRbjet0;
  Double_t        m_lep0_dRbjet1;
  Double_t        m_lep0_dRbjet0;
  Double_t        pt_lep1_charge;
  Double_t        tag_jet4_jvt;
  Double_t        weight_leptonSF_EL_SF_ID_UP;
  Double_t        pt_jet2_jvt;
  Double_t        weight_bTagSF_DL1r_77_extrapolation_up;
  Double_t        tag_jet3_jvt;
  Double_t        normWeight;
  Double_t        dEta_lep_pt;
  Double_t        weight_bTagSF_DL1r_77_extrapolation_from_charm_down;
  Double_t        m_lep2_type;
  Double_t        tag_jet2_E;
  Double_t        weight_jvt_DOWN;
  Double_t        weight_globalLeptonTriggerSF_MU_Trigger_STAT_DOWN;
  Double_t        pt_lep2_eta;
  Double_t        tag_jet4_tagweightbin;
  Double_t        pt_jet3_tagweightbin;
  Double_t        m_lep0_dPhi_ll;
  Double_t        weight_pileup_DOWN;
  Double_t        bdt_lep2_pt;
  Double_t        m_lep0_iff;
  Double_t        weight_bTagSF_DL1r_77_eigenvars_C_up;
  Double_t        pt_lep0_origin;
  Double_t        pt_lep2_dRbjet1;
  Double_t        pt_lep2_dRbjet0;
  Double_t        pt_lep1_bdtscore;
  Double_t        weight_globalLeptonTriggerSF_MU_Trigger_SYST_DOWN;
  Double_t        m_lep2_dRbjet1;
  Double_t        m_lep1_pt;
  Double_t        m_lep1_charge;
  Double_t        pt_jet0_btagWeight;
  Double_t        weight_leptonSF_MU_SF_ID_STAT_UP;
  Double_t        tag_jet4_pt;
  Double_t        mllossf0;
  Double_t        mllossf1;
  Double_t        m_lep2_charge;
  Double_t        pt_jet5_E;
  Double_t        pt_lep0_charge;
  Double_t        tag_jet0_phi;
  Double_t        tag_jet0_tagweightbin;
  Double_t        pt_jet4_tagweightbin;
  Double_t        pt_jet3_eta;
  Double_t        weight_bTagSF_DL1r_77_eigenvars_B_up;
  Double_t        weight_bTagSF_DL1r_77_extrapolation_from_charm_up;
  Double_t        m_lep1_phi;
  Double_t        pt_jet4_jvt;
  Double_t        pt_jet1_btagWeight;
  Double_t        pt_jet2_tagweightbin;
  Double_t        weight_leptonSF_MU_SF_TTVA_SYST_DOWN;
  Double_t        pt_jet5_tagweightbin;
  Double_t        pt_jet1_E;
  Double_t        tag_jet3_tagweightbin;
  Double_t        dEta_true;
  Double_t        m_lep2_dRbjet0;
  Double_t        weight_leptonSF_MU_SF_ID_SYST_DOWN;
  Double_t        pt_jet3_btagWeight;
  Double_t        m_lep0_mlb0;
  Double_t        m_lep0_mlb1;
  Double_t        pt_jet0_tagweightbin;
  Double_t        pt_lep2_isTight;
  Double_t        pt_lep2_type;
  Double_t        weight_leptonSF_MU_SF_ID_STAT_DOWN;
  Double_t        pt_jet0_pt;
  Double_t        pt_jet4_eta;
  Double_t        pt_jet5_eta;
  Double_t        pt_lep0_isTight;
  Double_t        pt_jet1_eta;
  Double_t        tag_jet1_eta;
  Double_t        xsec_new;
  Double_t        bdt_lep1_phi;
  Double_t        pt_jet5_jvt;
  Double_t        pt_lep2_mlj;
  Double_t        bdt_lep2_phi;
  Double_t        m_lep2_dPhi_ll;
  Double_t        tag_jet0_E;
  Double_t        pt_lep2_mlb0;
  Double_t        pt_lep2_mlb1;
  Double_t        pt_jet2_phi;
  Double_t        m_lep0_mlj;
  Double_t        tag_jet3_phi;
  Double_t        m_lep0_phi;
  Double_t        ASM_weight;
  Double_t        pt_jet3_jvt;
  Double_t        weight_leptonSF_EL_SF_Isol_UP;
  Double_t        m_lep1_isTight;
  Double_t        weight_pileup;
  Double_t        ST;
  Double_t        MET;
  Double_t        bdt_lep1_pt;
  Double_t        tag_jet1_jvt;
  Double_t        weight_globalLeptonTriggerSF_EL_Trigger_DOWN;
  Double_t        pt_jet2_btagWeight;
  Double_t        pt_jet2_eta;
  Double_t        pt_lep1_dPhi_ll;
  Double_t        dPhi_bdt;
  Double_t        pt_jet1_jvt;
  Double_t        tag_jet5_jvt;
  Double_t        pt_jet4_E;
  Double_t        m_lep0_isTight;
  Double_t        weight_jvt;
  Double_t        m_lep2_mlb0;
  Double_t        m_lep2_mlb1;
  Double_t        pt_jet5_phi;
  Double_t        weight_globalLeptonTriggerSF;
  Double_t        pt_lep1_iff;
  Double_t        pt_jet3_pt;
  Double_t        weight_leptonSF_MU_SF_Isol_SYST_UP;
  Double_t        pt_jet2_E;
  Double_t        pt_lep0_eta;
  Double_t        tag_jet0_jvt;
  Double_t        pt_jet1_phi;
  Double_t        pt_lep1_phi;
  Double_t        pt_jet0_E;
  Double_t        weight_bTagSF_DL1r_77_eigenvars_B_down;
  Double_t        tag_jet0_eta;
  Double_t        MT;
  Double_t        m_lep2_isTight;
  Double_t        pt_lep1_origin;
  Double_t        pt_jet4_phi;
  Double_t        weight_bTagSF_DL1r_77;
  Double_t        weight_globalLeptonTriggerSF_MU_Trigger_STAT_UP;
  Double_t        pt_jet0_eta;
  Double_t        pt_jet5_btagWeight;
  Double_t        bdt_lep1_eta;
  Double_t        bdt_lep2_mlb1;
  Double_t        tag_jet2_phi;
  Double_t        pt_lep2_iff;
  Double_t        bdt_lep2_mlb0;
  Double_t        tag_jet2_tagweightbin;
  Double_t        weight_leptonSF_MU_SF_TTVA_SYST_UP;
  Double_t        pt_lep0_iff;
  Double_t        weight_leptonSF_MU_SF_TTVA_STAT_UP;
  Double_t        pt_lep0_pt;
  Double_t        weight_leptonSF;
  Double_t        m_lep0_eta;
  Double_t        tag_jet5_btagWeight;
  Double_t        pt_lep0_phi;
  Double_t        m_lep2_mlj;
  Double_t        pt_lep2_dPhi_ll;
  Double_t        pt_lep2_bdtscore;
  Double_t        m_lep2_eta;
  Double_t        weight_leptonSF_EL_SF_Reco_DOWN;
  Double_t        m_lep1_type;
  Double_t        tag_jet3_btagWeight;
  Double_t        tag_jet4_phi;
  Double_t        eee_2016;
  Double_t        weight_bTagSF_DL1r_77_extrapolation_down;
  Double_t        nJets;
  Double_t        weight_jvt_UP;
  Double_t        tag_jet1_pt;
  Double_t        pt_lep0_mlb0;
  Double_t        pt_lep0_mlb1;
  Double_t        pt_jet5_pt;
  Double_t        tag_jet3_pt;
  Double_t        tag_jet5_eta;
  Double_t        tag_jet1_btagWeight;
  Double_t        pt_lep0_mlj;
  Double_t        weight_bTagSF_DL1r_77_eigenvars_Light_up;
  Double_t        bdt_lep2_eta;
  Double_t        weight_globalLeptonTriggerSF_MU_Trigger_SYST_UP;
  Double_t        tag_jet5_pt;
  Double_t        weight_leptonSF_MU_SF_Isol_STAT_DOWN;
  Double_t        pt_lep0_PLVLoose;
  Double_t        pt_lep1_PLVLoose;
  Double_t        pt_lep2_PLVLoose;
  Double_t        pt_lep0_PLVTight;
  Double_t        pt_lep1_PLVTight;
  Double_t        pt_lep2_PLVTight;
  Double_t        pt_lep0_PLImprovedTight;
  Double_t        pt_lep1_PLImprovedTight;
  Double_t        pt_lep2_PLImprovedTight;
  Double_t        pt_lep0_PLImprovedVeryTight;
  Double_t        pt_lep1_PLImprovedVeryTight;
  Double_t        pt_lep2_PLImprovedVeryTight;

  // List of branches
  TBranch        *b_pt_lep1_pt;   //!
  TBranch        *b_m_lep0_pt;   //!
  TBranch        *b_pt_jet0_jvt;   //!
  TBranch        *b_m_lep0_bdtscore;   //!
  TBranch        *b_m_lep1_dPhi_ll;   //!
  TBranch        *b_m_lep2_phi;   //!
  TBranch        *b_nBTags;   //!
  TBranch        *b_tag_jet1_tagweightbin;   //!
  TBranch        *b_pt_lep1_isTight;   //!
  TBranch        *b_m_lep1_iff;   //!
  TBranch        *b_tag_jet2_eta;   //!
  TBranch        *b_pt_lep2_charge;   //!
  TBranch        *b_weight_leptonSF_EL_SF_Trigger_UP;   //!
  TBranch        *b_weight_leptonSF_EL_SF_ID_DOWN;   //!
  TBranch        *b_tag_jet3_eta;   //!
  TBranch        *b_tag_jet2_pt;   //!
  TBranch        *b_tag_jet4_eta;   //!
  TBranch        *b_m_lep2_iff;   //!
  TBranch        *b_tag_jet0_btagWeight;   //!
  TBranch        *b_dEta_bdt;   //!
  TBranch        *b_tag_jet0_pt;   //!
  TBranch        *b_m_lep0_type;   //!
  TBranch        *b_bdt_lep0_phi;   //!
  TBranch        *b_pt_lep1_dRbjet0;   //!
  TBranch        *b_pt_lep1_dRbjet1;   //!
  TBranch        *b_bdt_lep1_mlb0;   //!
  TBranch        *b_bdt_lep1_mlb1;   //!
  TBranch        *b_pt_jet0_phi;   //!
  TBranch        *b_weight_leptonSF_MU_SF_ID_SYST_UP;   //!
  TBranch        *b_NZ_Cands;   //!
  TBranch        *b_pt_jet1_pt;   //!
  TBranch        *b_pt_jet4_btagWeight;   //!
  TBranch        *b_weight_leptonSF_MU_SF_TTVA_STAT_DOWN;   //!
  TBranch        *b_pt_jet3_phi;   //!
  TBranch        *b_weight_leptonSF_EL_SF_Isol_DOWN;   //!
  TBranch        *b_weight_globalLeptonTriggerSF_EL_Trigger_UP;   //!
  TBranch        *b_m_lep0_charge;   //!
  TBranch        *b_tag_jet2_btagWeight;   //!
  TBranch        *b_tag_jet2_jvt;   //!
  TBranch        *b_weight_leptonSF_MU_SF_Isol_STAT_UP;   //!
  TBranch        *b_weight_bTagSF_DL1r_77_eigenvars_C_down;   //!
  TBranch        *b_pt_lep1_mlb1;   //!
  TBranch        *b_pt_lep1_mlb0;   //!
  TBranch        *b_bdt_lep0_eta;   //!
  TBranch        *b_tag_jet5_tagweightbin;   //!
  TBranch        *b_m_lep2_pt;   //!
  TBranch        *b_pt_lep1_mlj;   //!
  TBranch        *b_bdt_lep0_mlb1;   //!
  TBranch        *b_bdt_lep0_mlb0;   //!
  TBranch        *b_m_lep1_mlj;   //!
  TBranch        *b_mumue_2016;   //!
  TBranch        *b_tag_jet3_E;   //!
  TBranch        *b_m_lep1_dRbjet0;   //!
  TBranch        *b_m_lep1_dRbjet1;   //!
  TBranch        *b_m_lep1_eta;   //!
  TBranch        *b_m_lep1_bdtscore;   //!
  TBranch        *b_pt_lep2_pt;   //!
  TBranch        *b_tag_jet5_E;   //!
  TBranch        *b_tag_jet1_phi;   //!
  TBranch        *b_tag_jet1_E;   //!
  TBranch        *b_weight_pileup_UP;   //!
  TBranch        *b_weight_leptonSF_EL_SF_Reco_UP;   //!
  TBranch        *b_pt_jet2_pt;   //!
  TBranch        *b_weight_bTagSF_DL1r_77_eigenvars_Light_down;   //!
  TBranch        *b_pt_jet4_pt;   //!
  TBranch        *b_totC;   //!
  TBranch        *b_dEta_mlb;   //!
  TBranch        *b_pt_lep0_dPhi_ll;   //!
  TBranch        *b_tag_jet4_btagWeight;   //!
  TBranch        *b_m_lep1_mlb1;   //!
  TBranch        *b_m_lep1_mlb0;   //!
  TBranch        *b_pt_lep2_origin;   //!
  TBranch        *b_HT;   //!
  TBranch        *b_eemu_2016;   //!
  TBranch        *b_pt_lep1_type;   //!
  TBranch        *b_pt_lep2_phi;   //!
  TBranch        *b_bdt_lep0_pt;   //!
  TBranch        *b_pt_lep0_bdtscore;   //!
  TBranch        *b_tag_jet5_phi;   //!
  TBranch        *b_pt_lep0_type;   //!
  TBranch        *b_m_lep2_bdtscore;   //!
  TBranch        *b_tag_jet4_E;   //!
  TBranch        *b_pt_lep1_eta;   //!
  TBranch        *b_pt_jet1_tagweightbin;   //!
  TBranch        *b_pt_jet3_E;   //!
  TBranch        *b_weight_leptonSF_EL_SF_Trigger_DOWN;   //!
  TBranch        *b_weight_leptonSF_MU_SF_Isol_SYST_DOWN;   //!
  TBranch        *b_mumumu_2016;   //!
  TBranch        *b_pt_lep0_dRbjet1;   //!
  TBranch        *b_pt_lep0_dRbjet0;   //!
  TBranch        *b_m_lep0_dRbjet1;   //!
  TBranch        *b_m_lep0_dRbjet0;   //!
  TBranch        *b_pt_lep1_charge;   //!
  TBranch        *b_tag_jet4_jvt;   //!
  TBranch        *b_weight_leptonSF_EL_SF_ID_UP;   //!
  TBranch        *b_pt_jet2_jvt;   //!
  TBranch        *b_weight_bTagSF_DL1r_77_extrapolation_up;   //!
  TBranch        *b_tag_jet3_jvt;   //!
  TBranch        *b_normWeight;   //!
  TBranch        *b_dEta_lep_pt;   //!
  TBranch        *b_weight_bTagSF_DL1r_77_extrapolation_from_charm_down;   //!
  TBranch        *b_m_lep2_type;   //!
  TBranch        *b_tag_jet2_E;   //!
  TBranch        *b_weight_jvt_DOWN;   //!
  TBranch        *b_weight_globalLeptonTriggerSF_MU_Trigger_STAT_DOWN;   //!
  TBranch        *b_pt_lep2_eta;   //!
  TBranch        *b_tag_jet4_tagweightbin;   //!
  TBranch        *b_pt_jet3_tagweightbin;   //!
  TBranch        *b_m_lep0_dPhi_ll;   //!
  TBranch        *b_weight_pileup_DOWN;   //!
  TBranch        *b_bdt_lep2_pt;   //!
  TBranch        *b_m_lep0_iff;   //!
  TBranch        *b_weight_bTagSF_DL1r_77_eigenvars_C_up;   //!
  TBranch        *b_pt_lep0_origin;   //!
  TBranch        *b_pt_lep2_dRbjet1;   //!
  TBranch        *b_pt_lep2_dRbjet0;   //!
  TBranch        *b_pt_lep1_bdtscore;   //!
  TBranch        *b_weight_globalLeptonTriggerSF_MU_Trigger_SYST_DOWN;   //!
  TBranch        *b_m_lep2_dRbjet1;   //!
  TBranch        *b_m_lep1_pt;   //!
  TBranch        *b_m_lep1_charge;   //!
  TBranch        *b_pt_jet0_btagWeight;   //!
  TBranch        *b_weight_leptonSF_MU_SF_ID_STAT_UP;   //!
  TBranch        *b_tag_jet4_pt;   //!
  TBranch        *b_mllossf0;   //!
  TBranch        *b_mllossf1;   //!
  TBranch        *b_m_lep2_charge;   //!
  TBranch        *b_pt_jet5_E;   //!
  TBranch        *b_pt_lep0_charge;   //!
  TBranch        *b_tag_jet0_phi;   //!
  TBranch        *b_tag_jet0_tagweightbin;   //!
  TBranch        *b_pt_jet4_tagweightbin;   //!
  TBranch        *b_pt_jet3_eta;   //!
  TBranch        *b_weight_bTagSF_DL1r_77_eigenvars_B_up;   //!
  TBranch        *b_weight_bTagSF_DL1r_77_extrapolation_from_charm_up;   //!
  TBranch        *b_m_lep1_phi;   //!
  TBranch        *b_pt_jet4_jvt;   //!
  TBranch        *b_pt_jet1_btagWeight;   //!
  TBranch        *b_pt_jet2_tagweightbin;   //!
  TBranch        *b_weight_leptonSF_MU_SF_TTVA_SYST_DOWN;   //!
  TBranch        *b_pt_jet5_tagweightbin;   //!
  TBranch        *b_pt_jet1_E;   //!
  TBranch        *b_tag_jet3_tagweightbin;   //!
  TBranch        *b_dEta_true;   //!
  TBranch        *b_m_lep2_dRbjet0;   //!
  TBranch        *b_weight_leptonSF_MU_SF_ID_SYST_DOWN;   //!
  TBranch        *b_pt_jet3_btagWeight;   //!
  TBranch        *b_m_lep0_mlb0;   //!
  TBranch        *b_m_lep0_mlb1;   //!
  TBranch        *b_pt_jet0_tagweightbin;   //!
  TBranch        *b_pt_lep2_isTight;   //!
  TBranch        *b_pt_lep2_type;   //!
  TBranch        *b_weight_leptonSF_MU_SF_ID_STAT_DOWN;   //!
  TBranch        *b_pt_jet0_pt;   //!
  TBranch        *b_pt_jet4_eta;   //!
  TBranch        *b_pt_jet5_eta;   //!
  TBranch        *b_pt_lep0_isTight;   //!
  TBranch        *b_pt_jet1_eta;   //!
  TBranch        *b_tag_jet1_eta;   //!
  TBranch        *b_xsec_new;   //!
  TBranch        *b_bdt_lep1_phi;   //!
  TBranch        *b_pt_jet5_jvt;   //!
  TBranch        *b_pt_lep2_mlj;   //!
  TBranch        *b_bdt_lep2_phi;   //!
  TBranch        *b_m_lep2_dPhi_ll;   //!
  TBranch        *b_tag_jet0_E;   //!
  TBranch        *b_pt_lep2_mlb0;   //!
  TBranch        *b_pt_lep2_mlb1;   //!
  TBranch        *b_pt_jet2_phi;   //!
  TBranch        *b_m_lep0_mlj;   //!
  TBranch        *b_tag_jet3_phi;   //!
  TBranch        *b_m_lep0_phi;   //!
  TBranch        *b_ASM_weight;   //!
  TBranch        *b_pt_jet3_jvt;   //!
  TBranch        *b_weight_leptonSF_EL_SF_Isol_UP;   //!
  TBranch        *b_m_lep1_isTight;   //!
  TBranch        *b_weight_pileup;   //!
  TBranch        *b_ST;   //!
  TBranch        *b_MET;   //!
  TBranch        *b_bdt_lep1_pt;   //!
  TBranch        *b_tag_jet1_jvt;   //!
  TBranch        *b_weight_globalLeptonTriggerSF_EL_Trigger_DOWN;   //!
  TBranch        *b_pt_jet2_btagWeight;   //!
  TBranch        *b_pt_jet2_eta;   //!
  TBranch        *b_pt_lep1_dPhi_ll;   //!
  TBranch        *b_dPhi_bdt;   //!
  TBranch        *b_pt_jet1_jvt;   //!
  TBranch        *b_tag_jet5_jvt;   //!
  TBranch        *b_pt_jet4_E;   //!
  TBranch        *b_m_lep0_isTight;   //!
  TBranch        *b_weight_jvt;   //!
  TBranch        *b_m_lep2_mlb0;   //!
  TBranch        *b_m_lep2_mlb1;   //!
  TBranch        *b_pt_jet5_phi;   //!
  TBranch        *b_weight_globalLeptonTriggerSF;   //!
  TBranch        *b_pt_lep1_iff;   //!
  TBranch        *b_pt_jet3_pt;   //!
  TBranch        *b_weight_leptonSF_MU_SF_Isol_SYST_UP;   //!
  TBranch        *b_pt_jet2_E;   //!
  TBranch        *b_pt_lep0_eta;   //!
  TBranch        *b_tag_jet0_jvt;   //!
  TBranch        *b_pt_jet1_phi;   //!
  TBranch        *b_pt_lep1_phi;   //!
  TBranch        *b_pt_jet0_E;   //!
  TBranch        *b_weight_bTagSF_DL1r_77_eigenvars_B_down;   //!
  TBranch        *b_tag_jet0_eta;   //!
  TBranch        *b_MT;   //!
  TBranch        *b_m_lep2_isTight;   //!
  TBranch        *b_pt_lep1_origin;   //!
  TBranch        *b_pt_jet4_phi;   //!
  TBranch        *b_weight_bTagSF_DL1r_77;   //!
  TBranch        *b_weight_globalLeptonTriggerSF_MU_Trigger_STAT_UP;   //!
  TBranch        *b_pt_jet0_eta;   //!
  TBranch        *b_pt_jet5_btagWeight;   //!
  TBranch        *b_bdt_lep1_eta;   //!
  TBranch        *b_bdt_lep2_mlb1;   //!
  TBranch        *b_tag_jet2_phi;   //!
  TBranch        *b_pt_lep2_iff;   //!
  TBranch        *b_bdt_lep2_mlb0;   //!
  TBranch        *b_tag_jet2_tagweightbin;   //!
  TBranch        *b_weight_leptonSF_MU_SF_TTVA_SYST_UP;   //!
  TBranch        *b_pt_lep0_iff;   //!
  TBranch        *b_weight_leptonSF_MU_SF_TTVA_STAT_UP;   //!
  TBranch        *b_pt_lep0_pt;   //!
  TBranch        *b_weight_leptonSF;   //!
  TBranch        *b_m_lep0_eta;   //!
  TBranch        *b_tag_jet5_btagWeight;   //!
  TBranch        *b_pt_lep0_phi;   //!
  TBranch        *b_m_lep2_mlj;   //!
  TBranch        *b_pt_lep2_dPhi_ll;   //!
  TBranch        *b_pt_lep2_bdtscore;   //!
  TBranch        *b_m_lep2_eta;   //!
  TBranch        *b_weight_leptonSF_EL_SF_Reco_DOWN;   //!
  TBranch        *b_m_lep1_type;   //!
  TBranch        *b_tag_jet3_btagWeight;   //!
  TBranch        *b_tag_jet4_phi;   //!
  TBranch        *b_eee_2016;   //!
  TBranch        *b_weight_bTagSF_DL1r_77_extrapolation_down;   //!
  TBranch        *b_nJets;   //!
  TBranch        *b_weight_jvt_UP;   //!
  TBranch        *b_tag_jet1_pt;   //!
  TBranch        *b_pt_lep0_mlb0;   //!
  TBranch        *b_pt_lep0_mlb1;   //!
  TBranch        *b_pt_jet5_pt;   //!
  TBranch        *b_tag_jet3_pt;   //!
  TBranch        *b_tag_jet5_eta;   //!
  TBranch        *b_tag_jet1_btagWeight;   //!
  TBranch        *b_pt_lep0_mlj;   //!
  TBranch        *b_weight_bTagSF_DL1r_77_eigenvars_Light_up;   //!
  TBranch        *b_bdt_lep2_eta;   //!
  TBranch        *b_weight_globalLeptonTriggerSF_MU_Trigger_SYST_UP;   //!
  TBranch        *b_tag_jet5_pt;   //!
  TBranch        *b_weight_leptonSF_MU_SF_Isol_STAT_DOWN;   //!
  TBranch        *b_pt_lep0_PLVLoose; //!
  TBranch        *b_pt_lep1_PLVLoose; //!
  TBranch        *b_pt_lep2_PLVLoose; //!
  TBranch        *b_pt_lep0_PLVTight; //!
  TBranch        *b_pt_lep1_PLVTight; //!
  TBranch        *b_pt_lep2_PLVTight; //!
  TBranch        *b_pt_lep0_PLImprovedTight; //!
  TBranch        *b_pt_lep1_PLImprovedTight; //!
  TBranch        *b_pt_lep2_PLImprovedTight; //!
  TBranch        *b_pt_lep0_PLImprovedVeryTight; //!
  TBranch        *b_pt_lep1_PLImprovedVeryTight; //!
  TBranch        *b_pt_lep2_PLImprovedVeryTight; //!

 ttWSelector(std::string name, TTree * /*tree*/ =0) : fChain(0) {
    std::cout << "Initialize selector " << name << std::endl;
    if( name.find("MC") != std::string::npos ){   std::cout << "Running on MC"   << std::endl; isMC = 1; }
    if( name.find("DATA") != std::string::npos ){ std::cout << "Running on DATA" << std::endl; isMC = 0; }
  }
  virtual ~ttWSelector() { }

  virtual Int_t   Version() const { return 2; }
  virtual void    Begin(TTree *tree);
  virtual void    SlaveBegin(TTree *tree);
  virtual void    Init(TTree *tree);
  virtual Bool_t  Notify();
  virtual Bool_t  Process(Long64_t entry);
  virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
  virtual void    SetOption(const char *option) { fOption = option; }
  virtual void    SetObject(TObject *obj) { fObject = obj; }
  virtual void    SetInputList(TList *input) { fInput = input; }
  virtual TList  *GetOutputList() const { return fOutput; }
  virtual void    SlaveTerminate();
  virtual void    Terminate();

  void clearAll();
  void setDebug(bool doDeb){debug = doDeb;}
  void rmMCFakes(bool noFakes){noMCFakes = noFakes;}
  void setOutPathName(std::string path);
  void setOutFileName(std::string file);
  void checkBinContent(TH1F* h);
  void setLumiNorm(float l);
  void setMCcamp(std::string camp);
  void setGenName(std::string genname);
  bool isFake(float type, float IFFClass);
  bool isFakeEvent(float class_lep1, float class_lep2, float class_lep3);
  void countLepTypes(float class_lep1, float class_lep2, float class_lep3);
  void addCutflow(float w, bool pass, TString name);
  void sortCutflow();
  void printCutflow();
  void printLepTypes();
  void write();

  void fillHisto(TString name, float val, float w);
  void makeHisto(TString name, int bins, float xMin, float xMax);
  void makeHisto(TString name, std::vector<float> ranges);

  std::string IFFEventType(float class_lep1, float class_lep2, float class_lep3);

  void INFO(const char* app, const char* msg){std::cout << Form("%s::%s() \t INFO \t %s","tHqSelector",app,msg) << std::endl;}
  void DEBUG(const char* app, const char* msg){if(debug) std::cout << Form("%s::%s() \t DEBUG \t %s","tHqSelector",app,msg) << std::endl;}


  ClassDef(ttWSelector,0);
};

#endif

#ifdef ttWSelector_cxx
void ttWSelector::Init(TTree *tree){

  if (!tree) return;
  fChain = tree;
  fChain->SetMakeClass(1);

  fChain->SetBranchAddress("pt_lep1_pt", &pt_lep1_pt, &b_pt_lep1_pt);
  fChain->SetBranchAddress("m_lep0_pt", &m_lep0_pt, &b_m_lep0_pt);
  fChain->SetBranchAddress("pt_jet0_jvt", &pt_jet0_jvt, &b_pt_jet0_jvt);
  fChain->SetBranchAddress("m_lep0_bdtscore", &m_lep0_bdtscore, &b_m_lep0_bdtscore);
  fChain->SetBranchAddress("m_lep1_dPhi_ll", &m_lep1_dPhi_ll, &b_m_lep1_dPhi_ll);
  fChain->SetBranchAddress("m_lep2_phi", &m_lep2_phi, &b_m_lep2_phi);
  fChain->SetBranchAddress("nBTags", &nBTags, &b_nBTags);
  fChain->SetBranchAddress("tag_jet1_tagweightbin", &tag_jet1_tagweightbin, &b_tag_jet1_tagweightbin);
  fChain->SetBranchAddress("pt_lep1_isTight", &pt_lep1_isTight, &b_pt_lep1_isTight);
  fChain->SetBranchAddress("m_lep1_iff", &m_lep1_iff, &b_m_lep1_iff);
  fChain->SetBranchAddress("tag_jet2_eta", &tag_jet2_eta, &b_tag_jet2_eta);
  fChain->SetBranchAddress("pt_lep2_charge", &pt_lep2_charge, &b_pt_lep2_charge);
  fChain->SetBranchAddress("weight_leptonSF_EL_SF_Trigger_UP", &weight_leptonSF_EL_SF_Trigger_UP, &b_weight_leptonSF_EL_SF_Trigger_UP);
  fChain->SetBranchAddress("weight_leptonSF_EL_SF_ID_DOWN", &weight_leptonSF_EL_SF_ID_DOWN, &b_weight_leptonSF_EL_SF_ID_DOWN);
  fChain->SetBranchAddress("tag_jet3_eta", &tag_jet3_eta, &b_tag_jet3_eta);
  fChain->SetBranchAddress("tag_jet2_pt", &tag_jet2_pt, &b_tag_jet2_pt);
  fChain->SetBranchAddress("tag_jet4_eta", &tag_jet4_eta, &b_tag_jet4_eta);
  fChain->SetBranchAddress("m_lep2_iff", &m_lep2_iff, &b_m_lep2_iff);
  fChain->SetBranchAddress("tag_jet0_btagWeight", &tag_jet0_btagWeight, &b_tag_jet0_btagWeight);
  fChain->SetBranchAddress("dEta_bdt", &dEta_bdt, &b_dEta_bdt);
  fChain->SetBranchAddress("tag_jet0_pt", &tag_jet0_pt, &b_tag_jet0_pt);
  fChain->SetBranchAddress("m_lep0_type", &m_lep0_type, &b_m_lep0_type);
  fChain->SetBranchAddress("bdt_lep0_phi", &bdt_lep0_phi, &b_bdt_lep0_phi);
  fChain->SetBranchAddress("pt_lep1_dRbjet0", &pt_lep1_dRbjet0, &b_pt_lep1_dRbjet0);
  fChain->SetBranchAddress("pt_lep1_dRbjet1", &pt_lep1_dRbjet1, &b_pt_lep1_dRbjet1);
  fChain->SetBranchAddress("bdt_lep1_mlb0", &bdt_lep1_mlb0, &b_bdt_lep1_mlb0);
  fChain->SetBranchAddress("bdt_lep1_mlb1", &bdt_lep1_mlb1, &b_bdt_lep1_mlb1);
  fChain->SetBranchAddress("pt_jet0_phi", &pt_jet0_phi, &b_pt_jet0_phi);
  fChain->SetBranchAddress("weight_leptonSF_MU_SF_ID_SYST_UP", &weight_leptonSF_MU_SF_ID_SYST_UP, &b_weight_leptonSF_MU_SF_ID_SYST_UP);
  fChain->SetBranchAddress("NZ_Cands", &NZ_Cands, &b_NZ_Cands);
  fChain->SetBranchAddress("pt_jet1_pt", &pt_jet1_pt, &b_pt_jet1_pt);
  fChain->SetBranchAddress("pt_jet4_btagWeight", &pt_jet4_btagWeight, &b_pt_jet4_btagWeight);
  fChain->SetBranchAddress("weight_leptonSF_MU_SF_TTVA_STAT_DOWN", &weight_leptonSF_MU_SF_TTVA_STAT_DOWN, &b_weight_leptonSF_MU_SF_TTVA_STAT_DOWN);
  fChain->SetBranchAddress("pt_jet3_phi", &pt_jet3_phi, &b_pt_jet3_phi);
  fChain->SetBranchAddress("weight_leptonSF_EL_SF_Isol_DOWN", &weight_leptonSF_EL_SF_Isol_DOWN, &b_weight_leptonSF_EL_SF_Isol_DOWN);
  fChain->SetBranchAddress("weight_globalLeptonTriggerSF_EL_Trigger_UP", &weight_globalLeptonTriggerSF_EL_Trigger_UP, &b_weight_globalLeptonTriggerSF_EL_Trigger_UP);
  fChain->SetBranchAddress("m_lep0_charge", &m_lep0_charge, &b_m_lep0_charge);
  fChain->SetBranchAddress("tag_jet2_btagWeight", &tag_jet2_btagWeight, &b_tag_jet2_btagWeight);
  fChain->SetBranchAddress("tag_jet2_jvt", &tag_jet2_jvt, &b_tag_jet2_jvt);
  fChain->SetBranchAddress("weight_leptonSF_MU_SF_Isol_STAT_UP", &weight_leptonSF_MU_SF_Isol_STAT_UP, &b_weight_leptonSF_MU_SF_Isol_STAT_UP);
  fChain->SetBranchAddress("weight_bTagSF_DL1r_77_eigenvars_C_down", &weight_bTagSF_DL1r_77_eigenvars_C_down, &b_weight_bTagSF_DL1r_77_eigenvars_C_down);
  fChain->SetBranchAddress("pt_lep1_mlb1", &pt_lep1_mlb1, &b_pt_lep1_mlb1);
  fChain->SetBranchAddress("pt_lep1_mlb0", &pt_lep1_mlb0, &b_pt_lep1_mlb0);
  fChain->SetBranchAddress("bdt_lep0_eta", &bdt_lep0_eta, &b_bdt_lep0_eta);
  fChain->SetBranchAddress("tag_jet5_tagweightbin", &tag_jet5_tagweightbin, &b_tag_jet5_tagweightbin);
  fChain->SetBranchAddress("m_lep2_pt", &m_lep2_pt, &b_m_lep2_pt);
  fChain->SetBranchAddress("pt_lep1_mlj", &pt_lep1_mlj, &b_pt_lep1_mlj);
  fChain->SetBranchAddress("bdt_lep0_mlb1", &bdt_lep0_mlb1, &b_bdt_lep0_mlb1);
  fChain->SetBranchAddress("bdt_lep0_mlb0", &bdt_lep0_mlb0, &b_bdt_lep0_mlb0);
  fChain->SetBranchAddress("m_lep1_mlj", &m_lep1_mlj, &b_m_lep1_mlj);
  fChain->SetBranchAddress("mumue_2016", &mumue_2016, &b_mumue_2016);
  fChain->SetBranchAddress("tag_jet3_E", &tag_jet3_E, &b_tag_jet3_E);
  fChain->SetBranchAddress("m_lep1_dRbjet0", &m_lep1_dRbjet0, &b_m_lep1_dRbjet0);
  fChain->SetBranchAddress("m_lep1_dRbjet1", &m_lep1_dRbjet1, &b_m_lep1_dRbjet1);
  fChain->SetBranchAddress("m_lep1_eta", &m_lep1_eta, &b_m_lep1_eta);
  fChain->SetBranchAddress("m_lep1_bdtscore", &m_lep1_bdtscore, &b_m_lep1_bdtscore);
  fChain->SetBranchAddress("pt_lep2_pt", &pt_lep2_pt, &b_pt_lep2_pt);
  fChain->SetBranchAddress("tag_jet5_E", &tag_jet5_E, &b_tag_jet5_E);
  fChain->SetBranchAddress("tag_jet1_phi", &tag_jet1_phi, &b_tag_jet1_phi);
  fChain->SetBranchAddress("tag_jet1_E", &tag_jet1_E, &b_tag_jet1_E);
  fChain->SetBranchAddress("weight_pileup_UP", &weight_pileup_UP, &b_weight_pileup_UP);
  fChain->SetBranchAddress("weight_leptonSF_EL_SF_Reco_UP", &weight_leptonSF_EL_SF_Reco_UP, &b_weight_leptonSF_EL_SF_Reco_UP);
  fChain->SetBranchAddress("pt_jet2_pt", &pt_jet2_pt, &b_pt_jet2_pt);
  fChain->SetBranchAddress("weight_bTagSF_DL1r_77_eigenvars_Light_down", &weight_bTagSF_DL1r_77_eigenvars_Light_down, &b_weight_bTagSF_DL1r_77_eigenvars_Light_down);
  fChain->SetBranchAddress("pt_jet4_pt", &pt_jet4_pt, &b_pt_jet4_pt);
  fChain->SetBranchAddress("totC", &totC, &b_totC);
  fChain->SetBranchAddress("dEta_mlb", &dEta_mlb, &b_dEta_mlb);
  fChain->SetBranchAddress("pt_lep0_dPhi_ll", &pt_lep0_dPhi_ll, &b_pt_lep0_dPhi_ll);
  fChain->SetBranchAddress("tag_jet4_btagWeight", &tag_jet4_btagWeight, &b_tag_jet4_btagWeight);
  fChain->SetBranchAddress("m_lep1_mlb1", &m_lep1_mlb1, &b_m_lep1_mlb1);
  fChain->SetBranchAddress("m_lep1_mlb0", &m_lep1_mlb0, &b_m_lep1_mlb0);
  fChain->SetBranchAddress("pt_lep2_origin", &pt_lep2_origin, &b_pt_lep2_origin);
  fChain->SetBranchAddress("HT", &HT, &b_HT);
  fChain->SetBranchAddress("eemu_2016", &eemu_2016, &b_eemu_2016);
  fChain->SetBranchAddress("pt_lep1_type", &pt_lep1_type, &b_pt_lep1_type);
  fChain->SetBranchAddress("pt_lep2_phi", &pt_lep2_phi, &b_pt_lep2_phi);
  fChain->SetBranchAddress("bdt_lep0_pt", &bdt_lep0_pt, &b_bdt_lep0_pt);
  fChain->SetBranchAddress("pt_lep0_bdtscore", &pt_lep0_bdtscore, &b_pt_lep0_bdtscore);
  fChain->SetBranchAddress("tag_jet5_phi", &tag_jet5_phi, &b_tag_jet5_phi);
  fChain->SetBranchAddress("pt_lep0_type", &pt_lep0_type, &b_pt_lep0_type);
  fChain->SetBranchAddress("m_lep2_bdtscore", &m_lep2_bdtscore, &b_m_lep2_bdtscore);
  fChain->SetBranchAddress("tag_jet4_E", &tag_jet4_E, &b_tag_jet4_E);
  fChain->SetBranchAddress("pt_lep1_eta", &pt_lep1_eta, &b_pt_lep1_eta);
  fChain->SetBranchAddress("pt_jet1_tagweightbin", &pt_jet1_tagweightbin, &b_pt_jet1_tagweightbin);
  fChain->SetBranchAddress("pt_jet3_E", &pt_jet3_E, &b_pt_jet3_E);
  fChain->SetBranchAddress("weight_leptonSF_EL_SF_Trigger_DOWN", &weight_leptonSF_EL_SF_Trigger_DOWN, &b_weight_leptonSF_EL_SF_Trigger_DOWN);
  fChain->SetBranchAddress("weight_leptonSF_MU_SF_Isol_SYST_DOWN", &weight_leptonSF_MU_SF_Isol_SYST_DOWN, &b_weight_leptonSF_MU_SF_Isol_SYST_DOWN);
  fChain->SetBranchAddress("mumumu_2016", &mumumu_2016, &b_mumumu_2016);
  fChain->SetBranchAddress("pt_lep0_dRbjet1", &pt_lep0_dRbjet1, &b_pt_lep0_dRbjet1);
  fChain->SetBranchAddress("pt_lep0_dRbjet0", &pt_lep0_dRbjet0, &b_pt_lep0_dRbjet0);
  fChain->SetBranchAddress("m_lep0_dRbjet1", &m_lep0_dRbjet1, &b_m_lep0_dRbjet1);
  fChain->SetBranchAddress("m_lep0_dRbjet0", &m_lep0_dRbjet0, &b_m_lep0_dRbjet0);
  fChain->SetBranchAddress("pt_lep1_charge", &pt_lep1_charge, &b_pt_lep1_charge);
  fChain->SetBranchAddress("tag_jet4_jvt", &tag_jet4_jvt, &b_tag_jet4_jvt);
  fChain->SetBranchAddress("weight_leptonSF_EL_SF_ID_UP", &weight_leptonSF_EL_SF_ID_UP, &b_weight_leptonSF_EL_SF_ID_UP);
  fChain->SetBranchAddress("pt_jet2_jvt", &pt_jet2_jvt, &b_pt_jet2_jvt);
  fChain->SetBranchAddress("weight_bTagSF_DL1r_77_extrapolation_up", &weight_bTagSF_DL1r_77_extrapolation_up, &b_weight_bTagSF_DL1r_77_extrapolation_up);
  fChain->SetBranchAddress("tag_jet3_jvt", &tag_jet3_jvt, &b_tag_jet3_jvt);
  fChain->SetBranchAddress("normWeight", &normWeight, &b_normWeight);
  fChain->SetBranchAddress("dEta_lep_pt", &dEta_lep_pt, &b_dEta_lep_pt);
  fChain->SetBranchAddress("weight_bTagSF_DL1r_77_extrapolation_from_charm_down", &weight_bTagSF_DL1r_77_extrapolation_from_charm_down, &b_weight_bTagSF_DL1r_77_extrapolation_from_charm_down);
  fChain->SetBranchAddress("m_lep2_type", &m_lep2_type, &b_m_lep2_type);
  fChain->SetBranchAddress("tag_jet2_E", &tag_jet2_E, &b_tag_jet2_E);
  fChain->SetBranchAddress("weight_jvt_DOWN", &weight_jvt_DOWN, &b_weight_jvt_DOWN);
  fChain->SetBranchAddress("weight_globalLeptonTriggerSF_MU_Trigger_STAT_DOWN", &weight_globalLeptonTriggerSF_MU_Trigger_STAT_DOWN, &b_weight_globalLeptonTriggerSF_MU_Trigger_STAT_DOWN);
  fChain->SetBranchAddress("pt_lep2_eta", &pt_lep2_eta, &b_pt_lep2_eta);
  fChain->SetBranchAddress("tag_jet4_tagweightbin", &tag_jet4_tagweightbin, &b_tag_jet4_tagweightbin);
  fChain->SetBranchAddress("pt_jet3_tagweightbin", &pt_jet3_tagweightbin, &b_pt_jet3_tagweightbin);
  fChain->SetBranchAddress("m_lep0_dPhi_ll", &m_lep0_dPhi_ll, &b_m_lep0_dPhi_ll);
  fChain->SetBranchAddress("weight_pileup_DOWN", &weight_pileup_DOWN, &b_weight_pileup_DOWN);
  fChain->SetBranchAddress("bdt_lep2_pt", &bdt_lep2_pt, &b_bdt_lep2_pt);
  fChain->SetBranchAddress("m_lep0_iff", &m_lep0_iff, &b_m_lep0_iff);
  fChain->SetBranchAddress("weight_bTagSF_DL1r_77_eigenvars_C_up", &weight_bTagSF_DL1r_77_eigenvars_C_up, &b_weight_bTagSF_DL1r_77_eigenvars_C_up);
  fChain->SetBranchAddress("pt_lep0_origin", &pt_lep0_origin, &b_pt_lep0_origin);
  fChain->SetBranchAddress("pt_lep2_dRbjet1", &pt_lep2_dRbjet1, &b_pt_lep2_dRbjet1);
  fChain->SetBranchAddress("pt_lep2_dRbjet0", &pt_lep2_dRbjet0, &b_pt_lep2_dRbjet0);
  fChain->SetBranchAddress("pt_lep1_bdtscore", &pt_lep1_bdtscore, &b_pt_lep1_bdtscore);
  fChain->SetBranchAddress("weight_globalLeptonTriggerSF_MU_Trigger_SYST_DOWN", &weight_globalLeptonTriggerSF_MU_Trigger_SYST_DOWN, &b_weight_globalLeptonTriggerSF_MU_Trigger_SYST_DOWN);
  fChain->SetBranchAddress("m_lep2_dRbjet1", &m_lep2_dRbjet1, &b_m_lep2_dRbjet1);
  fChain->SetBranchAddress("m_lep1_pt", &m_lep1_pt, &b_m_lep1_pt);
  fChain->SetBranchAddress("m_lep1_charge", &m_lep1_charge, &b_m_lep1_charge);
  fChain->SetBranchAddress("pt_jet0_btagWeight", &pt_jet0_btagWeight, &b_pt_jet0_btagWeight);
  fChain->SetBranchAddress("weight_leptonSF_MU_SF_ID_STAT_UP", &weight_leptonSF_MU_SF_ID_STAT_UP, &b_weight_leptonSF_MU_SF_ID_STAT_UP);
  fChain->SetBranchAddress("tag_jet4_pt", &tag_jet4_pt, &b_tag_jet4_pt);
  fChain->SetBranchAddress("mllossf0", &mllossf0, &b_mllossf0);
  fChain->SetBranchAddress("mllossf1", &mllossf1, &b_mllossf1);
  fChain->SetBranchAddress("m_lep2_charge", &m_lep2_charge, &b_m_lep2_charge);
  fChain->SetBranchAddress("pt_jet5_E", &pt_jet5_E, &b_pt_jet5_E);
  fChain->SetBranchAddress("pt_lep0_charge", &pt_lep0_charge, &b_pt_lep0_charge);
  fChain->SetBranchAddress("tag_jet0_phi", &tag_jet0_phi, &b_tag_jet0_phi);
  fChain->SetBranchAddress("tag_jet0_tagweightbin", &tag_jet0_tagweightbin, &b_tag_jet0_tagweightbin);
  fChain->SetBranchAddress("pt_jet4_tagweightbin", &pt_jet4_tagweightbin, &b_pt_jet4_tagweightbin);
  fChain->SetBranchAddress("pt_jet3_eta", &pt_jet3_eta, &b_pt_jet3_eta);
  fChain->SetBranchAddress("weight_bTagSF_DL1r_77_eigenvars_B_up", &weight_bTagSF_DL1r_77_eigenvars_B_up, &b_weight_bTagSF_DL1r_77_eigenvars_B_up);
  fChain->SetBranchAddress("weight_bTagSF_DL1r_77_extrapolation_from_charm_up", &weight_bTagSF_DL1r_77_extrapolation_from_charm_up, &b_weight_bTagSF_DL1r_77_extrapolation_from_charm_up);
  fChain->SetBranchAddress("m_lep1_phi", &m_lep1_phi, &b_m_lep1_phi);
  fChain->SetBranchAddress("pt_jet4_jvt", &pt_jet4_jvt, &b_pt_jet4_jvt);
  fChain->SetBranchAddress("pt_jet1_btagWeight", &pt_jet1_btagWeight, &b_pt_jet1_btagWeight);
  fChain->SetBranchAddress("pt_jet2_tagweightbin", &pt_jet2_tagweightbin, &b_pt_jet2_tagweightbin);
  fChain->SetBranchAddress("weight_leptonSF_MU_SF_TTVA_SYST_DOWN", &weight_leptonSF_MU_SF_TTVA_SYST_DOWN, &b_weight_leptonSF_MU_SF_TTVA_SYST_DOWN);
  fChain->SetBranchAddress("pt_jet5_tagweightbin", &pt_jet5_tagweightbin, &b_pt_jet5_tagweightbin);
  fChain->SetBranchAddress("pt_jet1_E", &pt_jet1_E, &b_pt_jet1_E);
  fChain->SetBranchAddress("tag_jet3_tagweightbin", &tag_jet3_tagweightbin, &b_tag_jet3_tagweightbin);
  fChain->SetBranchAddress("dEta_true", &dEta_true, &b_dEta_true);
  fChain->SetBranchAddress("m_lep2_dRbjet0", &m_lep2_dRbjet0, &b_m_lep2_dRbjet0);
  fChain->SetBranchAddress("weight_leptonSF_MU_SF_ID_SYST_DOWN", &weight_leptonSF_MU_SF_ID_SYST_DOWN, &b_weight_leptonSF_MU_SF_ID_SYST_DOWN);
  fChain->SetBranchAddress("pt_jet3_btagWeight", &pt_jet3_btagWeight, &b_pt_jet3_btagWeight);
  fChain->SetBranchAddress("m_lep0_mlb0", &m_lep0_mlb0, &b_m_lep0_mlb0);
  fChain->SetBranchAddress("m_lep0_mlb1", &m_lep0_mlb1, &b_m_lep0_mlb1);
  fChain->SetBranchAddress("pt_jet0_tagweightbin", &pt_jet0_tagweightbin, &b_pt_jet0_tagweightbin);
  fChain->SetBranchAddress("pt_lep2_isTight", &pt_lep2_isTight, &b_pt_lep2_isTight);
  fChain->SetBranchAddress("pt_lep2_type", &pt_lep2_type, &b_pt_lep2_type);
  fChain->SetBranchAddress("weight_leptonSF_MU_SF_ID_STAT_DOWN", &weight_leptonSF_MU_SF_ID_STAT_DOWN, &b_weight_leptonSF_MU_SF_ID_STAT_DOWN);
  fChain->SetBranchAddress("pt_jet0_pt", &pt_jet0_pt, &b_pt_jet0_pt);
  fChain->SetBranchAddress("pt_jet4_eta", &pt_jet4_eta, &b_pt_jet4_eta);
  fChain->SetBranchAddress("pt_jet5_eta", &pt_jet5_eta, &b_pt_jet5_eta);
  fChain->SetBranchAddress("pt_lep0_isTight", &pt_lep0_isTight, &b_pt_lep0_isTight);
  fChain->SetBranchAddress("pt_jet1_eta", &pt_jet1_eta, &b_pt_jet1_eta);
  fChain->SetBranchAddress("tag_jet1_eta", &tag_jet1_eta, &b_tag_jet1_eta);
  fChain->SetBranchAddress("xsec_new", &xsec_new, &b_xsec_new);
  fChain->SetBranchAddress("bdt_lep1_phi", &bdt_lep1_phi, &b_bdt_lep1_phi);
  fChain->SetBranchAddress("pt_jet5_jvt", &pt_jet5_jvt, &b_pt_jet5_jvt);
  fChain->SetBranchAddress("pt_lep2_mlj", &pt_lep2_mlj, &b_pt_lep2_mlj);
  fChain->SetBranchAddress("bdt_lep2_phi", &bdt_lep2_phi, &b_bdt_lep2_phi);
  fChain->SetBranchAddress("m_lep2_dPhi_ll", &m_lep2_dPhi_ll, &b_m_lep2_dPhi_ll);
  fChain->SetBranchAddress("tag_jet0_E", &tag_jet0_E, &b_tag_jet0_E);
  fChain->SetBranchAddress("pt_lep2_mlb0", &pt_lep2_mlb0, &b_pt_lep2_mlb0);
  fChain->SetBranchAddress("pt_lep2_mlb1", &pt_lep2_mlb1, &b_pt_lep2_mlb1);
  fChain->SetBranchAddress("pt_jet2_phi", &pt_jet2_phi, &b_pt_jet2_phi);
  fChain->SetBranchAddress("m_lep0_mlj", &m_lep0_mlj, &b_m_lep0_mlj);
  fChain->SetBranchAddress("tag_jet3_phi", &tag_jet3_phi, &b_tag_jet3_phi);
  fChain->SetBranchAddress("m_lep0_phi", &m_lep0_phi, &b_m_lep0_phi);
  fChain->SetBranchAddress("ASM_weight", &ASM_weight, &b_ASM_weight);
  fChain->SetBranchAddress("pt_jet3_jvt", &pt_jet3_jvt, &b_pt_jet3_jvt);
  fChain->SetBranchAddress("weight_leptonSF_EL_SF_Isol_UP", &weight_leptonSF_EL_SF_Isol_UP, &b_weight_leptonSF_EL_SF_Isol_UP);
  fChain->SetBranchAddress("m_lep1_isTight", &m_lep1_isTight, &b_m_lep1_isTight);
  fChain->SetBranchAddress("weight_pileup", &weight_pileup, &b_weight_pileup);
  fChain->SetBranchAddress("ST", &ST, &b_ST);
  fChain->SetBranchAddress("MET", &MET, &b_MET);
  fChain->SetBranchAddress("bdt_lep1_pt", &bdt_lep1_pt, &b_bdt_lep1_pt);
  fChain->SetBranchAddress("tag_jet1_jvt", &tag_jet1_jvt, &b_tag_jet1_jvt);
  fChain->SetBranchAddress("weight_globalLeptonTriggerSF_EL_Trigger_DOWN", &weight_globalLeptonTriggerSF_EL_Trigger_DOWN, &b_weight_globalLeptonTriggerSF_EL_Trigger_DOWN);
  fChain->SetBranchAddress("pt_jet2_btagWeight", &pt_jet2_btagWeight, &b_pt_jet2_btagWeight);
  fChain->SetBranchAddress("pt_jet2_eta", &pt_jet2_eta, &b_pt_jet2_eta);
  fChain->SetBranchAddress("pt_lep1_dPhi_ll", &pt_lep1_dPhi_ll, &b_pt_lep1_dPhi_ll);
  fChain->SetBranchAddress("dPhi_bdt", &dPhi_bdt, &b_dPhi_bdt);
  fChain->SetBranchAddress("pt_jet1_jvt", &pt_jet1_jvt, &b_pt_jet1_jvt);
  fChain->SetBranchAddress("tag_jet5_jvt", &tag_jet5_jvt, &b_tag_jet5_jvt);
  fChain->SetBranchAddress("pt_jet4_E", &pt_jet4_E, &b_pt_jet4_E);
  fChain->SetBranchAddress("m_lep0_isTight", &m_lep0_isTight, &b_m_lep0_isTight);
  fChain->SetBranchAddress("weight_jvt", &weight_jvt, &b_weight_jvt);
  fChain->SetBranchAddress("m_lep2_mlb0", &m_lep2_mlb0, &b_m_lep2_mlb0);
  fChain->SetBranchAddress("m_lep2_mlb1", &m_lep2_mlb1, &b_m_lep2_mlb1);
  fChain->SetBranchAddress("pt_jet5_phi", &pt_jet5_phi, &b_pt_jet5_phi);
  fChain->SetBranchAddress("weight_globalLeptonTriggerSF", &weight_globalLeptonTriggerSF, &b_weight_globalLeptonTriggerSF);
  fChain->SetBranchAddress("pt_lep1_iff", &pt_lep1_iff, &b_pt_lep1_iff);
  fChain->SetBranchAddress("pt_jet3_pt", &pt_jet3_pt, &b_pt_jet3_pt);
  fChain->SetBranchAddress("weight_leptonSF_MU_SF_Isol_SYST_UP", &weight_leptonSF_MU_SF_Isol_SYST_UP, &b_weight_leptonSF_MU_SF_Isol_SYST_UP);
  fChain->SetBranchAddress("pt_jet2_E", &pt_jet2_E, &b_pt_jet2_E);
  fChain->SetBranchAddress("pt_lep0_eta", &pt_lep0_eta, &b_pt_lep0_eta);
  fChain->SetBranchAddress("tag_jet0_jvt", &tag_jet0_jvt, &b_tag_jet0_jvt);
  fChain->SetBranchAddress("pt_jet1_phi", &pt_jet1_phi, &b_pt_jet1_phi);
  fChain->SetBranchAddress("pt_lep1_phi", &pt_lep1_phi, &b_pt_lep1_phi);
  fChain->SetBranchAddress("pt_jet0_E", &pt_jet0_E, &b_pt_jet0_E);
  fChain->SetBranchAddress("weight_bTagSF_DL1r_77_eigenvars_B_down", &weight_bTagSF_DL1r_77_eigenvars_B_down, &b_weight_bTagSF_DL1r_77_eigenvars_B_down);
  fChain->SetBranchAddress("tag_jet0_eta", &tag_jet0_eta, &b_tag_jet0_eta);
  fChain->SetBranchAddress("MT", &MT, &b_MT);
  fChain->SetBranchAddress("m_lep2_isTight", &m_lep2_isTight, &b_m_lep2_isTight);
  fChain->SetBranchAddress("pt_lep1_origin", &pt_lep1_origin, &b_pt_lep1_origin);
  fChain->SetBranchAddress("pt_jet4_phi", &pt_jet4_phi, &b_pt_jet4_phi);
  fChain->SetBranchAddress("weight_bTagSF_DL1r_77", &weight_bTagSF_DL1r_77, &b_weight_bTagSF_DL1r_77);
  fChain->SetBranchAddress("weight_globalLeptonTriggerSF_MU_Trigger_STAT_UP", &weight_globalLeptonTriggerSF_MU_Trigger_STAT_UP, &b_weight_globalLeptonTriggerSF_MU_Trigger_STAT_UP);
  fChain->SetBranchAddress("pt_jet0_eta", &pt_jet0_eta, &b_pt_jet0_eta);
  fChain->SetBranchAddress("pt_jet5_btagWeight", &pt_jet5_btagWeight, &b_pt_jet5_btagWeight);
  fChain->SetBranchAddress("bdt_lep1_eta", &bdt_lep1_eta, &b_bdt_lep1_eta);
  fChain->SetBranchAddress("bdt_lep2_mlb1", &bdt_lep2_mlb1, &b_bdt_lep2_mlb1);
  fChain->SetBranchAddress("tag_jet2_phi", &tag_jet2_phi, &b_tag_jet2_phi);
  fChain->SetBranchAddress("pt_lep2_iff", &pt_lep2_iff, &b_pt_lep2_iff);
  fChain->SetBranchAddress("bdt_lep2_mlb0", &bdt_lep2_mlb0, &b_bdt_lep2_mlb0);
  fChain->SetBranchAddress("tag_jet2_tagweightbin", &tag_jet2_tagweightbin, &b_tag_jet2_tagweightbin);
  fChain->SetBranchAddress("weight_leptonSF_MU_SF_TTVA_SYST_UP", &weight_leptonSF_MU_SF_TTVA_SYST_UP, &b_weight_leptonSF_MU_SF_TTVA_SYST_UP);
  fChain->SetBranchAddress("pt_lep0_iff", &pt_lep0_iff, &b_pt_lep0_iff);
  fChain->SetBranchAddress("weight_leptonSF_MU_SF_TTVA_STAT_UP", &weight_leptonSF_MU_SF_TTVA_STAT_UP, &b_weight_leptonSF_MU_SF_TTVA_STAT_UP);
  fChain->SetBranchAddress("pt_lep0_pt", &pt_lep0_pt, &b_pt_lep0_pt);
  fChain->SetBranchAddress("weight_leptonSF", &weight_leptonSF, &b_weight_leptonSF);
  fChain->SetBranchAddress("m_lep0_eta", &m_lep0_eta, &b_m_lep0_eta);
  fChain->SetBranchAddress("tag_jet5_btagWeight", &tag_jet5_btagWeight, &b_tag_jet5_btagWeight);
  fChain->SetBranchAddress("pt_lep0_phi", &pt_lep0_phi, &b_pt_lep0_phi);
  fChain->SetBranchAddress("m_lep2_mlj", &m_lep2_mlj, &b_m_lep2_mlj);
  fChain->SetBranchAddress("pt_lep2_dPhi_ll", &pt_lep2_dPhi_ll, &b_pt_lep2_dPhi_ll);
  fChain->SetBranchAddress("pt_lep2_bdtscore", &pt_lep2_bdtscore, &b_pt_lep2_bdtscore);
  fChain->SetBranchAddress("m_lep2_eta", &m_lep2_eta, &b_m_lep2_eta);
  fChain->SetBranchAddress("weight_leptonSF_EL_SF_Reco_DOWN", &weight_leptonSF_EL_SF_Reco_DOWN, &b_weight_leptonSF_EL_SF_Reco_DOWN);
  fChain->SetBranchAddress("m_lep1_type", &m_lep1_type, &b_m_lep1_type);
  fChain->SetBranchAddress("tag_jet3_btagWeight", &tag_jet3_btagWeight, &b_tag_jet3_btagWeight);
  fChain->SetBranchAddress("tag_jet4_phi", &tag_jet4_phi, &b_tag_jet4_phi);
  fChain->SetBranchAddress("eee_2016", &eee_2016, &b_eee_2016);
  fChain->SetBranchAddress("weight_bTagSF_DL1r_77_extrapolation_down", &weight_bTagSF_DL1r_77_extrapolation_down, &b_weight_bTagSF_DL1r_77_extrapolation_down);
  fChain->SetBranchAddress("nJets", &nJets, &b_nJets);
  fChain->SetBranchAddress("weight_jvt_UP", &weight_jvt_UP, &b_weight_jvt_UP);
  fChain->SetBranchAddress("tag_jet1_pt", &tag_jet1_pt, &b_tag_jet1_pt);
  fChain->SetBranchAddress("pt_lep0_mlb0", &pt_lep0_mlb0, &b_pt_lep0_mlb0);
  fChain->SetBranchAddress("pt_lep0_mlb1", &pt_lep0_mlb1, &b_pt_lep0_mlb1);
  fChain->SetBranchAddress("pt_jet5_pt", &pt_jet5_pt, &b_pt_jet5_pt);
  fChain->SetBranchAddress("tag_jet3_pt", &tag_jet3_pt, &b_tag_jet3_pt);
  fChain->SetBranchAddress("tag_jet5_eta", &tag_jet5_eta, &b_tag_jet5_eta);
  fChain->SetBranchAddress("tag_jet1_btagWeight", &tag_jet1_btagWeight, &b_tag_jet1_btagWeight);
  fChain->SetBranchAddress("pt_lep0_mlj", &pt_lep0_mlj, &b_pt_lep0_mlj);
  fChain->SetBranchAddress("weight_bTagSF_DL1r_77_eigenvars_Light_up", &weight_bTagSF_DL1r_77_eigenvars_Light_up, &b_weight_bTagSF_DL1r_77_eigenvars_Light_up);
  fChain->SetBranchAddress("bdt_lep2_eta", &bdt_lep2_eta, &b_bdt_lep2_eta);
  fChain->SetBranchAddress("weight_globalLeptonTriggerSF_MU_Trigger_SYST_UP", &weight_globalLeptonTriggerSF_MU_Trigger_SYST_UP, &b_weight_globalLeptonTriggerSF_MU_Trigger_SYST_UP);
  fChain->SetBranchAddress("tag_jet5_pt", &tag_jet5_pt, &b_tag_jet5_pt);
  fChain->SetBranchAddress("weight_leptonSF_MU_SF_Isol_STAT_DOWN", &weight_leptonSF_MU_SF_Isol_STAT_DOWN, &b_weight_leptonSF_MU_SF_Isol_STAT_DOWN);
  fChain->SetBranchAddress("pt_lep0_PLVLoose", &pt_lep0_PLVLoose, &b_pt_lep0_PLVLoose);
  fChain->SetBranchAddress("pt_lep1_PLVLoose", &pt_lep1_PLVLoose, &b_pt_lep1_PLVLoose);
  fChain->SetBranchAddress("pt_lep2_PLVLoose", &pt_lep2_PLVLoose, &b_pt_lep2_PLVLoose);
  fChain->SetBranchAddress("pt_lep0_PLVTight", &pt_lep0_PLVTight, &b_pt_lep0_PLVTight);
  fChain->SetBranchAddress("pt_lep1_PLVTight", &pt_lep1_PLVTight, &b_pt_lep1_PLVTight);
  fChain->SetBranchAddress("pt_lep2_PLVTight", &pt_lep2_PLVTight, &b_pt_lep2_PLVTight);
  fChain->SetBranchAddress("pt_lep0_PLImprovedTight", &pt_lep0_PLImprovedTight, &b_pt_lep0_PLImprovedTight);
  fChain->SetBranchAddress("pt_lep1_PLImprovedTight", &pt_lep1_PLImprovedTight, &b_pt_lep1_PLImprovedTight);
  fChain->SetBranchAddress("pt_lep2_PLImprovedTight", &pt_lep2_PLImprovedTight, &b_pt_lep2_PLImprovedTight);
  fChain->SetBranchAddress("pt_lep0_PLImprovedVeryTight", &pt_lep0_PLImprovedVeryTight, &b_pt_lep0_PLImprovedVeryTight);
  fChain->SetBranchAddress("pt_lep1_PLImprovedVeryTight", &pt_lep1_PLImprovedVeryTight, &b_pt_lep1_PLImprovedVeryTight);
  fChain->SetBranchAddress("pt_lep2_PLImprovedVeryTight", &pt_lep2_PLImprovedVeryTight, &b_pt_lep2_PLImprovedVeryTight);
}

Bool_t ttWSelector::Notify(){
  return kTRUE;
}

#endif
