#include "ttWCA/ttWCA.h"
#include "TopEvent/Event.h"
#include "TopEventSelectionTools/TreeManager.h"
#include "TopConfiguration/ConfigurationSettings.h"
#include "TopParticleLevel/TruthTools.h"

namespace top{
  ttWCA::ttWCA() : 
    m_debug(0),
    m_jetCharge(0),
    m_IFFClass(0),
    m_elID(0),
    m_PLViso(0)
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
    key = configSettings->value("ElectronPassID");
    m_elID = (key=="True");
    key = configSettings->value("PLVIsolation");
    m_PLViso = (key=="True");

    for(auto sysTree : treeManagers()) {
      if(m_jetCharge) sysTree->makeOutputVariable(m_jetcharge,  "JetCharge");   
      if(m_IFFClass){
	sysTree->makeOutputVariable(m_mu_IFFtype, "mu_IFFclass");
	sysTree->makeOutputVariable(m_el_IFFtype, "el_IFFclass");
      }
      if(m_elID){
	sysTree->makeOutputVariable(m_el_ID_LooseAndBLayerLH, "el_ID_LooseAndBLayerLH");
	sysTree->makeOutputVariable(m_el_ID_MediumLH,         "el_ID_MediumLH");
	sysTree->makeOutputVariable(m_el_ID_TightLH,          "el_ID_TightLH");
      }
      if(m_PLViso){
	sysTree->makeOutputVariable(m_mu_PLVLoose, "mu_PLVLoose");
	sysTree->makeOutputVariable(m_el_PLVLoose, "el_PLVLoose");
	sysTree->makeOutputVariable(m_mu_PLVTight, "mu_PLVTight");
	sysTree->makeOutputVariable(m_el_PLVTight, "el_PLVTight");
	sysTree->makeOutputVariable(m_mu_PLImprovedTight, "mu_PLImprovedTight");
	sysTree->makeOutputVariable(m_el_PLImprovedTight, "el_PLImprovedTight");
	sysTree->makeOutputVariable(m_mu_PLImprovedVeryTight, "mu_PLImprovedVeryTight");
	sysTree->makeOutputVariable(m_el_PLImprovedVeryTight, "el_PLImprovedVeryTight");
      }
    }
    if(m_IFFClass) initializeIFFTool("TruthClassificationTool");

    MSG_INFO("Initialized");
    return;
  }

  void ttWCA::saveEvent(const top::Event& event){
    clearOutputVars();
    if(!event.m_saveEvent) return;

    MSG_DEBUG(Form("EventNumer %i RunNumber %i \t Njets: %i, Nmu: %i Nel: %i, EtMiss: %.1f", 
		   (int)event.m_info->eventNumber(), (int)event.m_info->runNumber(), (int)event.m_jets.size(), (int)event.m_muons.size(), (int)event.m_electrons.size(), event.m_met->met()));

    processMuons(event);
    processElectrons(event);
    processJets(event);

    printTrigger(event);

    top::EventSaverFlatNtuple::saveEvent(event);
    return;
  }
  
  void ttWCA::saveParticleLevelEvent(const top::ParticleLevelEvent& plEvent){     
    top::EventSaverFlatNtuple::saveParticleLevelEvent(plEvent); 
    return;
  }

  void ttWCA::initializeIFFTool(const std::string& toolName){
    MSG_INFO(Form("Initializing %s",toolName.c_str()));
    m_IFFTool.setTypeAndName("TruthClassificationTool/"+toolName);

    top::check(m_IFFTool.setProperty("separateChargeFlipElectrons", true),               "Unable to set property: separateChargeFlipElectrons");  
    top::check(m_IFFTool.setProperty("OutputLevel",  m_debug ? MSG::DEBUG : MSG::FATAL), "Unable to set property: OutputLevel");
    top::check(m_IFFTool.initialize(),                                                   "Unable to initialize TruthClassificationTool");
    MSG_INFO(Form("Intialized %s", m_IFFTool.name().c_str()));
    return;
  }
  
  void ttWCA::processJets(const top::Event& event){
    for(const auto jet : event.m_jets){

      std::string btagWP = "isbtagged_" + m_config->bTagWP_available().front();
      double jet_ch = (m_jetCharge && jet->btagging()->isAvailable<double>("JetVertexCharge_discriminant")) ? jet->btagging()->auxdataConst<double>("JetVertexCharge_discriminant") : -99;

      MSG_DEBUG(Form("  Jet: [pt=%.1f | eta=%.3f | phi=%.3f] \t isB(%s)=%i, JC=%.3f", jet->pt(), jet->eta(), jet->phi(), btagWP.c_str(), (int)jet->auxdataConst<char>(btagWP)==1, (float)jet_ch));

      m_jetcharge.push_back(jet_ch);
    }
  }

  void ttWCA::processMuons(const top::Event& event){
    for(const auto mu : event.m_muons){

      int passPLVLoose  = (m_PLViso && mu->isAvailable<char>("AnalysisTop_Isol_PLVLoose")) ? mu->auxdataConst<char>("AnalysisTop_Isol_PLVLoose")==1 : -99;
      int passPLVTight  = (m_PLViso && mu->isAvailable<char>("AnalysisTop_Isol_PLVTight")) ? mu->auxdataConst<char>("AnalysisTop_Isol_PLVTight")==1 : -99;
      int passPLIVTight     = (m_PLViso && mu->isAvailable<char>("AnalysisTop_PLImprovedTight")) ? mu->auxdataConst<char>("AnalysisTop_PLImprovedTight")==1 : -99;
      int passPLIVVeryTight = (m_PLViso && mu->isAvailable<char>("AnalysisTop_PLImprovedVeryTight")) ? mu->auxdataConst<char>("AnalysisTop_PLImprovedVeryTight")==1 : -99;

      unsigned int IFFType(99);
      if(m_IFFClass && top::isSimulation(event)) top::check(m_IFFTool->classify(*mu, IFFType), "Unable the classifiy muon");

      MSG_DEBUG(Form("  Mu: [pt=%.1f | eta=%.3f | phi=%.3f] \t isTight=%i, type=%i, origin=%i, IFFType=%i", mu->pt(), mu->eta(), mu->phi(), (int)mu->auxdataConst<char>("passPreORSelection")==1, int(top::isSimulation(event) ? mu->auxdataConst<int>("truthType") : 0), int(top::isSimulation(event) ? mu->auxdataConst<int>("truthOrigin") : 0), (int)IFFType));

      m_mu_IFFtype.push_back(IFFType);

      m_mu_PLVLoose.push_back(passPLVLoose);
      m_mu_PLVTight.push_back(passPLVTight);
      m_mu_PLImprovedTight.push_back(passPLIVTight);
      m_mu_PLImprovedVeryTight.push_back(passPLIVVeryTight);
    }
  }

  void ttWCA::processElectrons(const top::Event& event){
    for(const auto el : event.m_electrons){

      int passLoose =  (m_elID && el->isAvailable<char>("DFCommonElectronsLHLooseBL")) ? el->auxdataConst<char>("DFCommonElectronsLHLooseBL")==1 : -99;
      int passMedium = (m_elID && el->isAvailable<char>("DFCommonElectronsLHMedium"))  ? el->auxdataConst<char>("DFCommonElectronsLHMedium")==1 : -99;
      int passTight =  (m_elID && el->isAvailable<char>("DFCommonElectronsLHTight"))   ? el->auxdataConst<char>("DFCommonElectronsLHTight")==1 : -99;

      int passPLVLoose  = (m_PLViso && el->isAvailable<char>("AnalysisTop_Isol_PLVLoose")) ? el->auxdataConst<char>("AnalysisTop_Isol_PLVLoose")==1 : -99;
      int passPLVTight  = (m_PLViso && el->isAvailable<char>("AnalysisTop_Isol_PLVTight")) ? el->auxdataConst<char>("AnalysisTop_Isol_PLVTight")==1 : -99;
      int passPLIVTight     = (m_PLViso && el->isAvailable<char>("AnalysisTop_Isol_PLImprovedTight")) ? el->auxdataConst<char>("AnalysisTop_Isol_PLImprovedTight")==1 : -99;
      int passPLIVVeryTight = (m_PLViso && el->isAvailable<char>("AnalysisTop_Isol_PLImprovedVeryTight")) ? el->auxdataConst<char>("AnalysisTop_Isol_PLImprovedVeryTight")==1 : -99;

      unsigned int IFFType(99);
      if(m_IFFClass && top::isSimulation(event)) top::check(m_IFFTool->classify(*el, IFFType), "Unable the classifiy electron");

      MSG_DEBUG(Form("  El: [pt=%.1f | eta=%.3f | phi=%.3f] \t isTight=%i, type=%i, origin=%i, IFFType=%i", el->pt(), el->eta(), el->phi(), (int)el->auxdataConst<char>("passPreORSelection")==1, int(top::isSimulation(event) ? el->auxdataConst<int>("truthType") : 0), int(top::isSimulation(event) ? el->auxdataConst<int>("truthOrigin") : 0), (int)IFFType));

      m_el_IFFtype.push_back(IFFType);

      m_el_ID_LooseAndBLayerLH.push_back(passLoose);
      m_el_ID_MediumLH.push_back(passMedium);
      m_el_ID_TightLH.push_back(passTight);

      m_el_PLVLoose.push_back(passPLVLoose);
      m_el_PLVTight.push_back(passPLVTight);
      m_el_PLImprovedTight.push_back(passPLIVTight);
      m_el_PLImprovedVeryTight.push_back(passPLIVVeryTight);
    }
  }

  void ttWCA::printTrigger(const top::Event& event){
    for(auto selection : *m_config->allSelectionNames()){
      for(auto trigger : m_config->allTriggers_Tight(selection)){
	MSG_DEBUG(Form("Selection %s \t pass trigger %s : %i", selection.c_str(), trigger.first.c_str(), (int)event.m_info->auxdataConst<char>("TRIGDEC_"+trigger.first)));
      }
    }
  }
  
  void ttWCA::clearOutputVars(){
    m_jetcharge.clear();
    m_mu_IFFtype.clear(); m_el_IFFtype.clear();

    m_el_ID_LooseAndBLayerLH.clear();
    m_el_ID_MediumLH.clear();
    m_el_ID_TightLH.clear();

    m_mu_PLVLoose.clear(); m_el_PLVLoose.clear();
    m_mu_PLVTight.clear(); m_el_PLVTight.clear();
    m_mu_PLImprovedTight.clear();     m_el_PLImprovedTight.clear();
    m_mu_PLImprovedVeryTight.clear(); m_el_PLImprovedVeryTight.clear();

    return;
  }
}
