#include "ttWCA/ttWCA.h"
#include "TopEvent/Event.h"
#include "TopEventSelectionTools/TreeManager.h"
#include "TopConfiguration/ConfigurationSettings.h"
#include "TopParticleLevel/TruthTools.h"

namespace top{
  ttWCA::ttWCA() : 
    m_debug(0)
  {}
  
  void ttWCA::initialize(std::shared_ptr<top::TopConfig> config, TFile* file, const std::vector<std::string>& extraBranches){
    top::EventSaverFlatNtuple::initialize(config, file, extraBranches);

    top::ConfigurationSettings* configSettings = top::ConfigurationSettings::get();
    std::string key;
    key = configSettings->value("Debug");
    m_debug = (key=="True");
  }
    
    ///setup reco-level tree///
    //for (auto systematicTree : treeManagers()) {
      //systematicTree->makeOutputVariable(m_var1, "var1");  //example of adding one variable to reco-level tree; m_var1 must be declared in the header
      //if(systematicTree->name()=="nominal") systematicTree->makeOutputVariable(m_var2, "var2");  //example of adding one variable only to the nominal tree, not to the systematic ones
    //}
    
    ///setup particle-level tree///
    //if(topConfig()->doTopParticleLevel()) 
    //{
      //if ( not particleLevelTreeManager() ) EventSaverFlatNtuple::setupParticleLevelTreeManager(); 
      //particleLevelTreeManager()->makeOutputVariable(m_var3,"var3");  //example of adding one variable to the particle-level tree
    //}
    

  
  void ttWCA::saveEvent(const top::Event& event){
    
    top::EventSaverFlatNtuple::saveEvent(event);
    MSG_DEBUG(Form("EventNumer %i RunNumber %i \t Njets: %i, Nmu: %i Nel: %i, EtMiss: %.1f", (int)event.m_info->eventNumber(), (int)event.m_info->runNumber(), (int)event.m_jets.size(), (int)event.m_muons.size(), (int)event.m_electrons.size(), event.m_met->met()));

  }
  
  void ttWCA::saveParticleLevelEvent(const top::ParticleLevelEvent& plEvent){
    
    top::EventSaverFlatNtuple::saveParticleLevelEvent(plEvent);
  }
  
}

