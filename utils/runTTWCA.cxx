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
static std::string effFile(""), selection(""), process(""), tightKey("tight");

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
    INFO(Form("Reading config file %s", conf.Data()));

  debug     = env.GetValue("Debug",      0);
  MMType    = env.GetValue("MMType",     1);
  effFile   = env.GetValue("EffFile",   "");
  selection = env.GetValue("Selection", "");
  process   = env.GetValue("Process",   "");
  env.PrintEnv();
  return;
}

void initializeMMTool(asg::AnaToolHandle<CP::IFakeBkgTool> tool){
  
  top::check(tool.setProperty("InputFiles",      tokenize(effFile)),  "Cannot declare property: InputFiles");
  top::check(tool.setProperty("Selection",       selection),          "Cannot declare property: Selection");
  top::check(tool.setProperty("Process",         process),            "Cannot declare property: Process");
  top::check(tool.setProperty("TightDecoration", tightKey),           "Cannot declare property: TightDecoration"); 

  top::check(tool.initialize(), "MMTool cannot be initialized");
  INFO(Form("Initialized tool %s", tool.name().c_str()));
}

int main(int argc, char* argv[]){
  if(argc < 2) throw std::invalid_argument("Too few arguments");
  name = argv[0];

  TString config(argv[1]), input(argv[2]);

  if(!config.Length()) ERROR("No config file found");
  if(!input.Length())  ERROR("No input file found");

  INFO("Initializing");
  auto start = std::time(nullptr);
  INFO(std::ctime(&start));

  setConfigValues(config);
 
  asg::AnaToolHandle<CP::IFakeBkgTool> ASMTool("CP::AsymptMatrixTool/ASMTool");
  asg::AnaToolHandle<CP::IFakeBkgTool> LHMTool("CP::LhoodMM_tools/LHMTool");
  if(!MMType) ERROR("No MM type selected");

  asg::AnaToolHandle<CP::IFakeBkgTool> MMTool = MMType==1 ? LHMTool : ASMTool; 
  initializeMMTool(MMTool);

  INFO("Finalizing");
  auto end = std::time(nullptr);
  INFO(std::ctime(&end));
}

