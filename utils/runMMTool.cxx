#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <ctime>  
#include <numeric>
#include <stdexcept>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TEnv.h"
#include "TH1.h"
#include "TChain.h"
#include "TString.h"
#include "TSystem.h"
#include "TLorentzVector.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"

#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODEventInfo/EventAuxInfo.h"
#include "xAODBase/IParticle.h"
#include "xAODMuon/Muon.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODEgamma/Electron.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODBase/IParticleHelpers.h"

#include "TopEvent/EventTools.h"
#include "AsgTools/AnaToolHandle.h"
#include "AsgAnalysisInterfaces/IFakeBkgTool.h"
#include "AsgAnalysisInterfaces/ILinearFakeBkgTool.h"
#include "FakeBkgTools/FakeBkgInternals.h"
#include "FakeBkgTools/ApplyFakeFactor.h"
#include "FakeBkgTools/AsymptMatrixTool.h"
#include "FakeBkgTools/LhoodMM_tools.h"

static bool debug(0);
static char* name(0);

static unsigned int MMType(0);
static std::string effFile(""), selection(""), process(""), SRNames("");

static const std::string tightKey("tight");
static const std::string treeName("nominal");
static const std::string branchDict("branchMap.conf");

static const double M_Z(91.1876);
static const double M_W(80.3850);
static const double M_EL(5.10998946E-4);

void INFO(std::string msg){ std::cout << name << "\t INFO \t" << msg << std::endl; }
void DEBUG(std::string msg){ if(debug) std::cout << name << "\t DEBUG \t" << msg << std::endl; }
void ERROR(std::string msg){ std::cout << name << "\t ERROR \t" << msg << std::endl; exit(1); }

void printLeptons(xAOD::IParticleContainer& leptons){
  for(auto l : leptons) DEBUG(Form("Lepton[type=%i]: pt=%.1f, eta=%.3f, phi=%.3f, tight=%i", l->type(), l->pt(), l->eta(), l->phi(), (int)l->auxdata<char>(tightKey)));
}

bool ptMatch(double pt1, double pt2){ return TMath::Abs(pt1-pt2)/pt1 < 0.001; }

std::vector<std::string> tokenize(TString in){ 
  std::vector<std::string> out(0);
  TObjArray *arr = in.Tokenize(",");
  for(auto obj : *arr){
    TString str = ((TObjString*)obj)->GetString();
    str.ReplaceAll(" ","");
    out.push_back(str.Data());
  }
  return out;
}

StatusCode setConfigValues(TString conf){
  TEnv env;
  if(env.ReadFile(conf, kEnvAll) != 0){ 
    INFO(Form("Cannot read config file %s", conf.Data()));
    return StatusCode::FAILURE;
  }
  INFO(Form("Reading config file: %s", conf.Data()));

  debug     = env.GetValue("Debug",         0);
  MMType    = env.GetValue("MMType",        1);
  effFile   = env.GetValue("EffFilePath",   "");
  selection = env.GetValue("Selection",     "");
  process   = env.GetValue("Process",       "");
  SRNames   = env.GetValue("SignalRegion",  "");
  env.PrintEnv();
  return StatusCode::SUCCESS;
}

TString getBranch(const char* name){
  TEnv env;
  if(env.ReadFile(branchDict.c_str(), kEnvAll) != 0)
    ERROR(Form("Cannot read file %s", branchDict.c_str()));
  
  TString out = env.GetValue(name, ""); out.ReplaceAll("'","");
  if(!out.Length()) ERROR(Form("Cannot find branch %s in %s",name,branchDict.c_str()));
  DEBUG(Form("--> Found branch %s",out.Data()));
  return out;
}

void sortLeptons(std::vector<double> &pt, std::vector<double> &eta, std::vector<double> &phi, std::vector<double> &charge, std::vector<double> &type, std::vector<double> &tight){

  std::vector<double> pt_sorted(0), eta_sorted(0), phi_sorted(0), charge_sorted(0), type_sorted(0), tight_sorted(0);
  pt_sorted = pt;
  std::sort(pt_sorted.begin(), pt_sorted.end()); std::reverse(pt_sorted.begin(), pt_sorted.end());

  for(unsigned int i(0); i<pt_sorted.size() && i<pt.size(); i++){ 
    for(unsigned int j(0); j<pt.size(); j++){ 
      if( !ptMatch(pt_sorted.at(i),pt.at(j)) ) continue;
      eta_sorted.push_back(eta.at(j));
      phi_sorted.push_back(phi.at(j));
      charge_sorted.push_back(charge.at(j));
      type_sorted.push_back(type.at(j));
      tight_sorted.push_back(tight.at(j));
    } 
  }
  pt     = pt_sorted;
  eta    = eta_sorted;
  phi    = phi_sorted;
  charge = charge_sorted;
  type   = type_sorted;
  tight  = tight_sorted;
  return;
}

xAOD::IParticleContainer makeIParticleContainer(std::vector<double> pt, std::vector<double> eta, std::vector<double> phi, std::vector<double> charge, std::vector<double> type, std::vector<double> isTight){
  unsigned int nLep(pt.size());
  xAOD::IParticleContainer leptons(SG::VIEW_ELEMENTS);

  for(unsigned int i(0); i<nLep; i++){
   
    int lepPDG = type.at(i) == 0 ? 13 : 11; //this has to be adapted according to the way lepton flavors are stored in custom Ntuples
    switch( lepPDG ){
    case 11:{
      xAOD::Electron *el = new xAOD::Electron();
      el->makePrivateStore();
      el->setP4(pt.at(i)*1000, eta.at(i), phi.at(i), M_EL*1000);
      el->setCharge(charge.at(i));
      el->auxdata<char>(tightKey) = isTight.at(i)!=0;

      leptons.push_back( static_cast<xAOD::IParticle*>(el) );}
      break;
    case 13:{
      xAOD::Muon *mu = new xAOD::Muon();
      mu->makePrivateStore();
      mu->setP4(pt.at(i)*1000, eta.at(i), phi.at(i));
      mu->setCharge(charge.at(i));
      mu->auxdata<char>(tightKey) = isTight.at(i)!=0;

      leptons.push_back( static_cast<xAOD::IParticle*>(mu) );}
      break;
    default: break;
    }
  }
  DEBUG(Form("Create xAOD::IParticleContainer with %i entries", (int)leptons.size()));
  return leptons;
}

template <typename T>
StatusCode initializeMMTool(T &tool){
  if(selection.empty() || process.empty() || effFile.empty()){
    INFO("MM Tool needs [Selection] [Process] arguments and efficiency file");
    return StatusCode::FAILURE;
  }
  MSG::Level msgLevel = debug ? MSG::DEBUG : MSG::FATAL;
  
  top::check(tool.setProperty("InputFiles",      tokenize(effFile)),  "Cannot set property: InputFiles");
  top::check(tool.setProperty("Selection",       selection),          "Cannot set property: Selection");
  top::check(tool.setProperty("Process",         process),            "Cannot set property: Process");
  top::check(tool.setProperty("EnergyUnit",      "GeV"),              "Cannot set property: EnergyUnit");
  top::check(tool.setProperty("OutputLevel",     msgLevel),           "Cannot set property: OutputLevel");
  top::check(tool.setProperty("TightDecoration", tightKey+",as_char"),"Cannot set property: TightDecoration"); 

  top::check(tool.initialize(), "MMTool cannot be initialized");
  INFO(Form("Initialized tool: %s", tool.name().c_str()));
  return StatusCode::SUCCESS;
}

int main(int argc, char* argv[]){
  if(argc < 2) throw std::invalid_argument("Too few arguments. Run runMMTool [input.root] [Config.txt]");
  name = argv[0];

  TString input(argv[1]), config(argv[2]);

  if(!input.Length()) ERROR("No config file found");
  if(!config.Length())  ERROR("No input file found");

  INFO("Initializing");
  auto start = std::time(nullptr);
  INFO(std::ctime(&start));

  TFile *inFile = TFile::Open(input, "READ");
  if(!inFile || inFile->IsZombie()) ERROR("Invalid root file! Exiting");

  TTree* inTree = (TTree*)inFile->Get(treeName.c_str());
  if(!inTree) ERROR("Input tree not found! Exiting");
 
  top::check(setConfigValues(config), "Cannot set config values");
  if(SRNames.empty()) ERROR("No SR specified");
  
  asg::AnaToolHandle<CP::IFakeBkgTool>       LHMTool("CP::LhoodMM_tools/LHMTool");
  asg::AnaToolHandle<CP::ILinearFakeBkgTool> ASMTool("CP::AsymptMatrixTool/ASMTool");

  switch( MMType ){
  case 1:
    top::check(initializeMMTool(LHMTool), "LHMTool cannot be initialized");
    break;
  case 2:
    top::check(initializeMMTool(ASMTool), "ASMTool cannot be initialized");
    break;
  default: 
    ERROR("No MM type selected"); 
  }

  INFO(Form("Reading branches for TTree %s", inTree->GetName()));
  TTreeReader reader(inTree);

  TTreeReaderValue<Double_t> lep0_pt = {reader, getBranch("lep0_pt")};
  TTreeReaderValue<Double_t> lep1_pt = {reader, getBranch("lep1_pt")};
  TTreeReaderValue<Double_t> lep2_pt = {reader, getBranch("lep2_pt")};

  TTreeReaderValue<Double_t> lep0_eta = {reader, getBranch("lep0_eta")};
  TTreeReaderValue<Double_t> lep1_eta = {reader, getBranch("lep1_eta")};
  TTreeReaderValue<Double_t> lep2_eta = {reader, getBranch("lep2_eta")};

  TTreeReaderValue<Double_t> lep0_phi = {reader, getBranch("lep0_phi")};
  TTreeReaderValue<Double_t> lep1_phi = {reader, getBranch("lep1_phi")};
  TTreeReaderValue<Double_t> lep2_phi = {reader, getBranch("lep2_phi")};

  TTreeReaderValue<Double_t> lep0_charge = {reader, getBranch("lep0_charge")};
  TTreeReaderValue<Double_t> lep1_charge = {reader, getBranch("lep1_charge")};
  TTreeReaderValue<Double_t> lep2_charge = {reader, getBranch("lep2_charge")};

  TTreeReaderValue<Double_t> lep0_type = {reader, getBranch("lep0_type")};
  TTreeReaderValue<Double_t> lep1_type = {reader, getBranch("lep1_type")};
  TTreeReaderValue<Double_t> lep2_type = {reader, getBranch("lep2_type")};

  TTreeReaderValue<Double_t> lep0_isTight = {reader, getBranch("lep0_isTight")};
  TTreeReaderValue<Double_t> lep1_isTight = {reader, getBranch("lep1_isTight")};
  TTreeReaderValue<Double_t> lep2_isTight = {reader, getBranch("lep2_isTight")};

  ULong64_t treeEntries = inTree->GetEntries();
  INFO(Form("Reading TTree %s :: Entries = %ld", inTree->GetName(), (long)treeEntries));
  
  unsigned int NEvents(0);
  for(unsigned int entry(0); entry<treeEntries; entry++){

    reader.SetEntry(entry);
    DEBUG(Form("Processing entry %i", entry));

    float weightMM(0);   
    std::vector<double> lepPt  = {*lep0_pt,  *lep1_pt,  *lep2_pt};
    std::vector<double> lepEta = {*lep0_eta, *lep1_eta, *lep2_eta};
    std::vector<double> lepPhi = {*lep0_phi, *lep1_phi, *lep2_phi};

    std::vector<double> lepCharge = {*lep0_charge,  *lep1_charge,  *lep2_charge};
    std::vector<double> lepType   = {*lep0_type,    *lep1_type,    *lep2_type};
    std::vector<double> lepTight  = {*lep0_isTight, *lep1_isTight, *lep2_isTight};

    std::shared_ptr<xAOD::TStore> store = std::make_shared<xAOD::TStore>();
    std::shared_ptr<xAOD::TEvent> event = std::make_shared<xAOD::TEvent>();

    sortLeptons(lepPt, lepEta, lepPhi, lepCharge, lepType, lepTight);
    xAOD::IParticleContainer leptons = makeIParticleContainer(lepPt, lepEta, lepPhi, lepCharge, lepType, lepTight);
    printLeptons(leptons);

    switch( MMType ){
    case 1:
      top::check( LHMTool->addEvent(leptons), "Cannot execute LHMTool::addEvent()");
      break;
    case 2:
      top::check( ASMTool->addEvent(leptons), "Cannot execute ASMTool::addEvent()");
      top::check( ASMTool->getEventWeight(weightMM, selection, process), "Unable to get ASM weight");
      DEBUG(Form("Entry %i, N(lep) = %i :: weight(ASM) = %.3f",entry, (int)leptons.size(), weightMM));
      break;
    default:
      ERROR("No MM type selected");
    }

    NEvents++;
  }
  INFO(Form("Processed %i events", NEvents));

  float yields(0), yieldsUP(0), yieldsDOWN(0);
  switch( MMType ){
  case 1:
    top::check( LHMTool->getTotalYield(yields, yieldsUP, yieldsDOWN), "Cannot execute LHMTool::getTotalYield()");
    break;
  case 2:
    top::check( ASMTool->getTotalYield(yields, yieldsUP, yieldsDOWN), "Cannot execute ASMTool::getTotalYield()");
    break;
  default:
    ERROR("No MM type selected");
  }
  INFO(Form("MMTool::totalYields()\t %.3f (nom) %.3f (up) %.3f (down)", yields, yieldsUP, yieldsDOWN));

  INFO("Finalizing");
  auto end = std::time(nullptr);
  INFO(std::ctime(&end));
}

