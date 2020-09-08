#include "ttWCA/ttWCA.h"
#include "TopEvent/Event.h"
#include "TopEventSelectionTools/TreeManager.h"
#include "TopConfiguration/ConfigurationSettings.h"
#include "TopParticleLevel/TruthTools.h"

namespace top{
  ttWCA::ttWCA() : 
    m_debug(0),
    m_jetCharge(0),
    m_IFFClass(0)
  {}
  
  void ttWCA::initialize(std::shared_ptr<top::TopConfig> config, TFile* file, const std::vector<std::string>& extraBranches){
    top::EventSaverFlatNtuple::initialize(config, file, extraBranches);

    m_config = config;
    top::ConfigurationSettings* configSettings = top::ConfigurationSettings::get();
    
    std::string key;
    key = configSettings->value("Debug");
    m_debug = (key=="True");
    key = configSettings->value("JetCharge");
    m_jetCharge = (key=="True");
    key = configSettings->value("IFFClassification");
    m_IFFClass = (key=="True");

    for(auto sysTree : treeManagers()) {
      if(m_jetCharge)  sysTree->makeOutputVariable(m_jc,  "JetCharge");
      if(m_IFFClass)   sysTree->makeOutputVariable(m_IFF, "IFFClassification");
    }
    MSG_INFO("Initialized");
  }

  void ttWCA::saveEvent(const top::Event& event){
    
    top::EventSaverFlatNtuple::saveEvent(event);
    MSG_DEBUG(Form("EventNumer %i RunNumber %i \t Njets: %i, Nmu: %i Nel: %i, EtMiss: %.1f", (int)event.m_info->eventNumber(), (int)event.m_info->runNumber(), (int)event.m_jets.size(), (int)event.m_muons.size(), (int)event.m_electrons.size(), event.m_met->met()));

    processMuons(event);
    processElectrons(event);
    processJets(event);

    printTrigger(event);
  }
  
  void ttWCA::saveParticleLevelEvent(const top::ParticleLevelEvent& plEvent){ 
    
    top::EventSaverFlatNtuple::saveParticleLevelEvent(plEvent); 
  }
  
  void ttWCA::processJets(const top::Event& event){
    for(const auto jet : event.m_jets){

      std::string btagWP = "isbtagged_" + m_config->bTagWP_available().front();
      double jet_ch = (m_jetCharge && jet->btagging()->isAvailable<double>("JetVertexCharge_discriminant")) ? jet->btagging()->auxdataConst<double>("JetVertexCharge_discriminant") : -99;

      MSG_DEBUG(Form("  Jet: [pt=%.1f | eta=%.3f | phi=%.3f] \t isB(%s)=%i, JC=%.3f", jet->pt(), jet->eta(), jet->phi(), btagWP.c_str(), (int)jet->auxdataConst<char>(btagWP)==1, (float)jet_ch));
      m_jc.push_back(jet_ch);
    }
  }

  void ttWCA::processMuons(const top::Event& event){
    for(const auto mu : event.m_muons){
      MSG_DEBUG(Form("  Mu: [pt=%.1f | eta=%.3f | phi=%.3f] \t isTight=%i", mu->pt(), mu->eta(), mu->phi(), (int)mu->auxdataConst<char>("passPreORSelection")==1));
    }
  }

  void ttWCA::processElectrons(const top::Event& event){
    for(const auto el : event.m_electrons){
      MSG_DEBUG(Form("  El: [pt=%.1f | eta=%.3f | phi=%.3f] \t isTight=%i", el->pt(), el->eta(), el->phi(), (int)el->auxdataConst<char>("passPreORSelection")==1));
    }
  }

  void ttWCA::printTrigger(const top::Event& event){
    for(auto selection : *m_config->allSelectionNames()){
      for(auto trigger : m_config->allTriggers_Tight(selection)){
	MSG_DEBUG(Form("Selection %s \t pass trigger %s : %i", selection.c_str(), trigger.c_str(), (int)event.m_info->auxdataConst<char>("TRIGDEC_"+trigger)));
      }
    }
  }
  
}
