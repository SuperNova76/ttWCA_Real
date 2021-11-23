#ifndef ttWCA_H
#define ttWCA_H

#include "AsgTools/AnaToolHandle.h"
#include "TopAnalysis/EventSaverFlatNtuple.h"
#include "TopEvent/EventTools.h"
#include "TopConfiguration/TopConfig.h"
#include "TopEvent/Event.h"
#include "TopParticleLevel/ParticleLevelEvent.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODJet/JetContainer.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODBTagging/BTagging.h"
#include "xAODEgamma/EgammaTruthxAODHelpers.h"
#include "MCTruthClassifier/MCTruthClassifier.h"
#include "TruthClassification/TruthClassificationTool.h"
#include "GammaORTools/VGammaORTool.h"

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TString.h"
#include "TLorentzVector.h"
#include <iostream>

using namespace std;

namespace top{
  class ttWCA : public top::EventSaverFlatNtuple {
  public:
    ttWCA();
    virtual ~ttWCA(){}
      
    virtual void initialize(std::shared_ptr<top::TopConfig> config, TFile* file, const std::vector<std::string>& extraBranches) override;
      
    virtual StatusCode initialize() override {return StatusCode::SUCCESS;}      
      
    virtual void saveEvent(const top::Event& event) override;
    virtual void saveParticleLevelEvent(const top::ParticleLevelEvent& plEvent) override;

  private:
    bool m_debug;
    bool m_jetCharge;
    bool m_IFFClass;
    bool m_elID;
    bool m_elConv;
    bool m_PLViso;
    bool m_gammaJetOR;
    bool m_lepSF;

    std::shared_ptr<top::TopConfig> m_config;

    void MSG_INFO(TString msg){ std::cout << Form("top::ttWCA() \t INFO \t %s",msg.Data()) << std::endl; }
    void MSG_DEBUG(TString msg){ if(m_debug) std::cout << Form("top::ttWCA() \t DEBUG \t %s",msg.Data()) << std::endl; }

    void processJets(const top::Event& event);
    void processMuons(const top::Event& event);
    void processElectrons(const top::Event& event); 

    void printTrigger(const top::Event& event);

    void initializeMCTruthClassifier(const std::string& toolName);
    void initializeIFFTool(const std::string& toolName);
    void initializeVGammaORTool(const std::string& toolName);

    void clearOutputVars();

    //Variables for extra branches
    std::vector<float> m_jetcharge;

    std::vector<float> m_mu_IFFtype;
    std::vector<float> m_el_IFFtype;

    //Electron ID WP
    std::vector<int> m_el_ID_LooseAndBLayerLH;
    std::vector<int> m_el_ID_MediumLH;
    std::vector<int> m_el_ID_TightLH;

    //Electron converion-vars
    std::vector<int>   m_el_addAmbiguity;
    std::vector<int>   m_el_ambiguityType;
    std::vector<float> m_el_convRadiusTruth;

    //PLV iso WP
    std::vector<int> m_mu_PLVLoose;
    std::vector<int> m_el_PLVLoose;
    std::vector<int> m_mu_PLVTight;
    std::vector<int> m_el_PLVTight;
    std::vector<int> m_mu_PLImprovedTight;
    std::vector<int> m_el_PLImprovedTight;
    std::vector<int> m_mu_PLImprovedVeryTight;
    std::vector<int> m_el_PLImprovedVeryTight;

    //Individual lepton SF
    std::vector<float> m_el_SF_loose;
    std::vector<float> m_el_SF_tight;

    std::vector<float> m_el_SF_loose_Reco_UP;
    std::vector<float> m_el_SF_tight_Reco_UP;
    std::vector<float> m_el_SF_loose_Reco_DOWN;
    std::vector<float> m_el_SF_tight_Reco_DOWN;

    std::vector<float> m_el_SF_loose_ID_UP;
    std::vector<float> m_el_SF_tight_ID_UP;
    std::vector<float> m_el_SF_loose_ID_DOWN;
    std::vector<float> m_el_SF_tight_ID_DOWN;

    std::vector<float> m_el_SF_loose_Isol_UP;
    std::vector<float> m_el_SF_tight_Isol_UP;
    std::vector<float> m_el_SF_loose_Isol_DOWN;
    std::vector<float> m_el_SF_tight_Isol_DOWN;

    std::vector<float> m_mu_SF_loose;
    std::vector<float> m_mu_SF_tight;

    std::vector<float> m_mu_SF_loose_ID_SYST_UP;
    std::vector<float> m_mu_SF_tight_ID_SYST_UP;
    std::vector<float> m_mu_SF_loose_ID_STAT_UP;
    std::vector<float> m_mu_SF_tight_ID_STAT_UP;
    std::vector<float> m_mu_SF_loose_ID_SYST_DOWN;
    std::vector<float> m_mu_SF_tight_ID_SYST_DOWN;
    std::vector<float> m_mu_SF_loose_ID_STAT_DOWN;
    std::vector<float> m_mu_SF_tight_ID_STAT_DOWN;

    std::vector<float> m_mu_SF_loose_Isol_SYST_UP;
    std::vector<float> m_mu_SF_tight_Isol_SYST_UP;
    std::vector<float> m_mu_SF_loose_Isol_STAT_UP;
    std::vector<float> m_mu_SF_tight_Isol_STAT_UP;
    std::vector<float> m_mu_SF_loose_Isol_SYST_DOWN;
    std::vector<float> m_mu_SF_tight_Isol_SYST_DOWN;
    std::vector<float> m_mu_SF_loose_Isol_STAT_DOWN;
    std::vector<float> m_mu_SF_tight_Isol_STAT_DOWN;

    std::vector<float> m_mu_SF_loose_TTVA_SYST_UP;
    std::vector<float> m_mu_SF_tight_TTVA_SYST_UP;
    std::vector<float> m_mu_SF_loose_TTVA_STAT_UP;
    std::vector<float> m_mu_SF_tight_TTVA_STAT_UP;
    std::vector<float> m_mu_SF_loose_TTVA_SYST_DOWN;
    std::vector<float> m_mu_SF_tight_TTVA_SYST_DOWN;
    std::vector<float> m_mu_SF_loose_TTVA_STAT_DOWN;
    std::vector<float> m_mu_SF_tight_TTVA_STAT_DOWN;

    //GammaJets Overlap
    bool m_isGammaJetEvent;
    bool m_isGammaJetTLV;

    //Tools
    MCTruthClassifier* m_MCTruthClassifier;
    top::ScaleFactorRetriever* m_topSFR;
    asg::AnaToolHandle<CP::IClassificationTool> m_IFFTool;
    asg::AnaToolHandle<IVGammaORTool> m_VGammaORTool;

    ClassDefOverride(top::ttWCA, 0);
  };
}

#endif
