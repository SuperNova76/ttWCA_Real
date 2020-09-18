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
static const std::string branchDict("BranchMap.conf");

static const double M_Z(91.1876);
static const double M_W(80.3850);
static const double M_EL(5.10998946E-4);

void INFO(std::string msg){ std::cout << name << "\t INFO \t" << msg << std::endl; }
void DEBUG(std::string msg){ if(debug) std::cout << name << "\t DEBUG \t" << msg << std::endl; }
void ERROR(std::string msg){ std::cout << name << "\t ERROR \t" << msg << std::endl; exit(1); }

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

void setConfigValues(TString conf){
  TEnv env;
  if(env.ReadFile(conf, kEnvAll) != 0) 
    ERROR(Form("Cannot read config file %s", conf.Data()));
  else
    INFO(Form("Reading config file: %s", conf.Data()));

  debug     = env.GetValue("Debug",         0);
  MMType    = env.GetValue("MMType",        1);
  effFile   = env.GetValue("EffFilePath",   "");
  selection = env.GetValue("Selection",     "");
  process   = env.GetValue("Process",       "");
  SRNames   = env.GetValue("SignalRegion",  "");
  env.PrintEnv();
  return;
}

const char *getBranch(const char* name){
  TEnv env;
  if(env.ReadFile(branchDict.c_str(), kEnvAll) != 0)
    ERROR(Form("Cannot read file %s", branchDict.c_str()));
  
  std::string out = env.GetValue(name, "");
  if(out.empty()) ERROR(Form("Cannot find branch %s in %s",name,branchDict.c_str()));
  DEBUG(Form("--> Found branch %s",out.c_str()));
  return out.c_str();
}

xAOD::IParticleContainer makeIParticleContainer(std::vector<float> pt, std::vector<float> eta, std::vector<float> phi, std::vector<int> charge, std::vector<int> PDG, std::vector<bool> isTight){
  unsigned int nLep(pt.size());
  xAOD::IParticleContainer leptons(SG::VIEW_ELEMENTS);

  for(unsigned int i(0); i<nLep; i++){
    switch( (int)abs(PDG.at(i)) ){
    case 11:{
      xAOD::Electron *el = new xAOD::Electron();
      el->makePrivateStore();
      el->setP4(pt.at(i)*1000, eta.at(i), phi.at(i), M_EL*1000);
      el->setCharge((float)charge.at(i));
      el->auxdata<char>(tightKey) = isTight.at(i);

      leptons.push_back( static_cast<xAOD::IParticle*>(el) );}
      break;
    case 13:{
      xAOD::Muon *mu = new xAOD::Muon();
      mu->makePrivateStore();
      mu->setP4(pt.at(i)*1000, eta.at(i), phi.at(i));
      mu->setCharge((float)charge.at(i));
      mu->auxdata<char>(tightKey) = isTight.at(i);

      leptons.push_back( static_cast<xAOD::IParticle*>(mu) );}
      break;
    default: break;
    }
  }
  DEBUG(Form("Create xAOD::IParticle container with %i entries", (int)leptons.size()));
  return leptons;
}

void initializeMMTool(asg::AnaToolHandle<CP::IFakeBkgTool> tool){

  if(selection.empty() || process.empty() || effFile.empty())
    ERROR("MM Tool needs [Selection] [Process] arguments and efficiency file");
  
  MSG::Level msgLevel = debug ? MSG::DEBUG : MSG::FATAL;
  
  top::check(tool.setProperty("InputFiles",      tokenize(effFile)),  "Cannot set property: InputFiles");
  top::check(tool.setProperty("Selection",       selection),          "Cannot set property: Selection");
  top::check(tool.setProperty("Process",         process),            "Cannot set property: Process");
  top::check(tool.setProperty("EnergyUnit",      "GeV"),              "Cannot set property: EnergyUnit");
  top::check(tool.setProperty("OutputLevel",     msgLevel),           "Cannot set property: OutputLevel");
  top::check(tool.setProperty("TightDecoration", tightKey),           "Cannot set property: TightDecoration"); 

  top::check(tool.initialize(), "MMTool cannot be initialized");
  INFO(Form("Initialized tool: %s", tool.name().c_str()));
}

int main(int argc, char* argv[]){
  if(argc < 2) throw std::invalid_argument("Too few arguments");
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
 
  setConfigValues(config);
  if(SRNames.empty()) ERROR("No SR specified");

  if(!MMType) ERROR("No MM type selected");
  asg::AnaToolHandle<CP::IFakeBkgTool> ASMTool("CP::AsymptMatrixTool/ASMTool");
  asg::AnaToolHandle<CP::IFakeBkgTool> LHMTool("CP::LhoodMM_tools/LHMTool");
  
  asg::AnaToolHandle<CP::IFakeBkgTool> MMTool = MMType==1 ? LHMTool : ASMTool; 
  initializeMMTool(MMTool);

  TTreeReader reader(inTree);  
  ULong64_t treeEntries = inTree->GetEntries();
  INFO( Form("Reading TTree %s :: Entries = %ld", inTree->GetName(), (long)treeEntries) );

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

  TTreeReaderValue<Double_t> lep0_isTight = {reader, getBranch("lep0_isTight")};
  TTreeReaderValue<Double_t> lep1_isTight = {reader, getBranch("lep1_isTight")};
  TTreeReaderValue<Double_t> lep2_isTight = {reader, getBranch("lep2_isTight")};

  INFO("Finalizing");
  auto end = std::time(nullptr);
  INFO(std::ctime(&end));
}

