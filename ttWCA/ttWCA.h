#ifndef ttWCA_H
#define ttWCA_H

#include "AsgTools/AnaToolHandle.h"
#include "TopAnalysis/EventSaverFlatNtuple.h"
#include "TopEvent/EventTools.h"
#include "TopConfiguration/TopConfig.h"
#include "TopParticleLevel/ParticleLevelEvent.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODJet/JetContainer.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODBTagging/BTagging.h"
//#include "TruthClassification/TruthClassificationTool.h"

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TString.h"

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

      std::shared_ptr<top::TopConfig> m_config;

      void MSG_INFO(TString msg){ std::cout << Form("ttWCA() \t INFO \t %s",msg.Data()) << std::endl; }
      void MSG_DEBUG(TString msg){ if(m_debug) std::cout << Form("ttWCA() \t DEBUG \t %s",msg.Data()) << std::endl; }

      void processJets(const top::Event& event);
      void processMuons(const top::Event& event);
      void processElectrons(const top::Event& event); 

      void printTrigger(const top::Event& event);

      void initializeIFFTool(const std::string& toolName);
      void clearOutputVars();

      std::vector<float> m_jc;
      std::vector<float> m_IFF;

      // asg::AnaToolHandle<CP::IClassificationTool> m_IFFTool;
      
      ClassDefOverride(top::ttWCA, 0);
  };
}

#endif
