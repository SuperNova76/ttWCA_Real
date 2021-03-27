#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <ctime>  
#include <numeric>
#include <utility>
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
static std::string effFile(""), selection(""), process(""), SRNames(""), OutFile("");

static const std::string tightKey("tight");
static const std::string treeName("nominal");
static const std::string branchDict("branchMap.conf");

static const double M_Z(91.1876);
static const double M_W(80.3850);
static const double M_EL(5.10998946E-4);

void INFO(std::string msg){ std::cout << name << "\t INFO \t" << msg << std::endl; }
void DEBUG(std::string msg){ if(debug) std::cout << name << "\t DEBUG \t" << msg << std::endl; }
void ERROR(std::string msg){ std::cout << name << "\t ERROR \t" << msg << std::endl; exit(1); }

void printProcess(int entry, int allEntries, int div=10000, bool print=true){
  if(print && entry%div==0) INFO( Form("--> Processed %i events (%.0f %%)", entry, (float)entry/(float)allEntries * 100) ); 
}

void printLeptons(xAOD::IParticleContainer& leptons){
  for(auto l : leptons) DEBUG( Form("Lepton[type=%i]: pt=%.1f, eta=%.3f, phi=%.3f, tight=%i", l->type(), l->pt(), l->eta(), l->phi(), (int)l->auxdata<char>(tightKey)) );
}

bool ptMatch(double pt1, double pt2){ return TMath::Abs(pt1-pt2)/pt1 < 1E-6; }

bool isFile(std::string name){
  TFile *f = TFile::Open(name.c_str());
  return !(bool)f->IsZombie();
}

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

double absSum(std::vector<double> vec){
  double sum(0); for(auto i : vec) sum += i; 
  return TMath::Abs(sum);
}

StatusCode setConfigValues(TString conf){
  TEnv env;
  if(env.ReadFile(conf, kEnvAll) != 0){ 
    INFO( Form("Cannot read config file %s", conf.Data()) );
    return StatusCode::FAILURE;
  }
  INFO(Form("Reading config file: %s", conf.Data()));

  debug     = env.GetValue("Debug",         0);
  MMType    = env.GetValue("MMType",        1);
  effFile   = env.GetValue("EffFile",      "");
  selection = env.GetValue("Selection",    "");
  process   = env.GetValue("Process",      "");
  SRNames   = env.GetValue("SignalRegion", "");
  OutFile   = env.GetValue("OutFileName",  "");
  env.PrintEnv();
  return StatusCode::SUCCESS;
}

TString getBranch(const char* name){
  TEnv env;
  if(env.ReadFile(branchDict.c_str(), kEnvAll) != 0)
    ERROR( Form("Cannot read file %s", branchDict.c_str()) );
  
  TString out = env.GetValue(name, ""); out.ReplaceAll("'","");
  if(!out.Length()) ERROR(Form("Cannot find branch %s in %s",name,branchDict.c_str()));
  DEBUG( Form("--> Found branch %s",out.Data()) );
  return out;
}

std::pair<std::string, std::string> getArgs1LX(unsigned int nLep){

  std::string selection(""), process(">=1F[L]");
  switch(nLep){
  case 1:
    selection = "T"; 
    break;
  case 2:
    selection = "T!T+!TT";
    break;
  case 3:
    selection = "T!T!T+!TT!T+!T!TT";
    break;
  case 4:
    selection = "T!T!T!T+!TT!T!T+!T!TT!T+!T!T!TT";
    break;
  default: break;
  }
  return std::make_pair(selection, process);
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

TH1F *makeHisto(TString name, int bins, float xMin, float xMax){
  if(!name.Length() || !bins) return nullptr;

  TH1F *h = new TH1F(name, name, bins, xMin, xMax);
  INFO( Form("Created TH1F %s \t Bins: %i [%.1f|%.1f]", h->GetName(), bins, xMin, xMax) );
  return h;
}

TH1F *makeHisto(TString name, std::vector<float> ranges){
  const unsigned int N = ranges.size();
  if(!name.Length() || !N) return nullptr;

  float *x = new float[N];
  for(unsigned int i(0); i<N; i++) x[i] = ranges.at(i);

  TH1F *h = new TH1F(name, name, N-1, x);
  INFO( Form("Created TH1F %s \t Bins: %i [%.1f|%.1f]", h->GetName(), N-1, ranges.front(), ranges.back()) );
  return h;
}

StatusCode writeHistos(std::vector<TH1F*> hVec, TString outname){
  if(!hVec.size()){
    INFO("No input to write"); return StatusCode::FAILURE;
  }
  std::unique_ptr<TFile> fOut = std::make_unique<TFile>(outname, "UPDATE");
  fOut->cd();
  for(auto h : hVec){
    h->Write();
    INFO( Form("Wrote %s/%s", fOut->GetName(), h->GetName()) );
  }
  INFO( Form("Wrote %i histograms to output \n", (int)hVec.size()) );
  fOut->Close();
  return StatusCode::SUCCESS;
}

void addSuffix(std::vector<TH1F*> hVec, std::string suf){
  for(auto h : hVec) h->SetNameTitle( Form("%s_%s",h->GetName(),suf.c_str()), Form("%s_%s",h->GetTitle(),suf.c_str()) );
}

xAOD::IParticleContainer makeIParticleContainer(std::vector<double> pt, std::vector<double> eta, std::vector<double> phi, std::vector<double> charge, std::vector<double> type, std::vector<double> isTight, bool inGeV=true){
  
  xAOD::IParticleContainer leptons(SG::VIEW_ELEMENTS);
  double ptScale = inGeV ? 1000. : 1.;

  unsigned int nLep(pt.size());
  for(unsigned int i(0); i<nLep; i++){
   
    int lepPDG = type.at(i)==0 ? 11 : (type.at(i)==1 ? 13 : 0); //depends on Ntuple format
    switch( lepPDG ){
    case 11:{
      xAOD::Electron *el = new xAOD::Electron();
      el->makePrivateStore();
      el->setP4(pt.at(i)*ptScale, eta.at(i), phi.at(i), M_EL*ptScale);
      el->setCharge(charge.at(i));
      el->auxdata<char>(tightKey) = isTight.at(i);

      leptons.push_back( static_cast<xAOD::IParticle*>(el) );}
      break;
    case 13:{
      xAOD::Muon *mu = new xAOD::Muon();
      mu->makePrivateStore();
      mu->setP4(pt.at(i)*ptScale, eta.at(i), phi.at(i));
      mu->setCharge(charge.at(i));
      mu->auxdata<char>(tightKey) = isTight.at(i);

      leptons.push_back( static_cast<xAOD::IParticle*>(mu) );}
      break;
    default: break;
    }
  }
  DEBUG( Form("Create xAOD::IParticleContainer with %i entries", (int)leptons.size()) );
  return leptons;
}

template <typename T>
StatusCode initializeMMTool(T &tool){
  if(selection.empty() || process.empty() || effFile.empty() || !isFile(effFile)){
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
  INFO( Form("Initialized tool: %s", tool.name().c_str()) );
  return StatusCode::SUCCESS;
}

bool passCuts(TString name, std::vector<double> lepPt, std::vector<double> lepCharge, double njet, double nbjet, bool isZ){
  bool pass(false);
  const std::vector<double> lepPtCuts = {30., 20., 15.};

  if(name == "SR1b_low")
    pass = lepPt[0]>lepPtCuts[0] && lepPt[1]>lepPtCuts[1] && lepPt[2]>lepPtCuts[2] && absSum(lepCharge)==1 && njet>=2 && njet<4 && nbjet==1 && !isZ;

  if(name == "SR2b_low")
    pass = lepPt[0]>lepPtCuts[0] && lepPt[1]>lepPtCuts[1] && lepPt[2]>lepPtCuts[2] && absSum(lepCharge)==1 && njet>=2 && njet<4 && nbjet>=2 && !isZ;

  if(name == "SR1b_high")
    pass = lepPt[0]>lepPtCuts[0] && lepPt[1]>lepPtCuts[1] && lepPt[2]>lepPtCuts[2] && absSum(lepCharge)==1 && njet>=4 && njet<7 && nbjet==1 && !isZ;

  if(name == "SR2b_high")
    pass = lepPt[0]>lepPtCuts[0] && lepPt[1]>lepPtCuts[1] && lepPt[2]>lepPtCuts[2] && absSum(lepCharge)==1 && njet>=4 && njet<7 && nbjet>=2 && !isZ;

  if(name == "CRttZ")
    pass = lepPt[0]>lepPtCuts[0] && lepPt[1]>lepPtCuts[1] && lepPt[2]>lepPtCuts[2] && absSum(lepCharge)==1 && njet>=2 && njet<7 && nbjet>=2 && isZ;

  return pass;
}

int main(int argc, char* argv[]){

  if(argc < 2) throw std::invalid_argument("Too few arguments. Run runMMTool [input.root] [Config.txt]");
  name = argv[0];

  TString input(argv[1]), config(argv[2]);
  if(!input.Length()) ERROR("No input file found");
  if(!config.Length())  ERROR("No config file found");

  INFO("Initializing");
  auto start = std::time(nullptr);
  INFO(std::ctime(&start));

  TFile *inFile = TFile::Open(input, "READ");
  if(!inFile || inFile->IsZombie()) ERROR("Invalid root file! Exiting");
  INFO( Form("Reading input file %s", inFile->GetName()) );

  TTree* inTree = (TTree*)inFile->Get(treeName.c_str());
  if(!inTree) ERROR("Input tree not found! Exiting");
 
  top::check(setConfigValues(config), "Cannot set config values");
  if(SRNames.empty()) ERROR("No SR specified");
  
  std::vector<std::string> SRs = tokenize(SRNames);
  const unsigned int NSR = SRs.size();
  INFO( Form("Running %i regions [%s]", NSR, SRNames.c_str()) );

  INFO( Form("Reading branches for TTree %s", inTree->GetName()) );
  TTreeReader reader(inTree);

  TTreeReaderValue<Double_t> lep1_pt = {reader, getBranch("lep1_pt")};
  TTreeReaderValue<Double_t> lep2_pt = {reader, getBranch("lep2_pt")};
  TTreeReaderValue<Double_t> lep3_pt = {reader, getBranch("lep3_pt")};
  
  TTreeReaderValue<Double_t> lep1_eta = {reader, getBranch("lep1_eta")};
  TTreeReaderValue<Double_t> lep2_eta = {reader, getBranch("lep2_eta")};
  TTreeReaderValue<Double_t> lep3_eta = {reader, getBranch("lep3_eta")};

  TTreeReaderValue<Double_t> lep1_phi = {reader, getBranch("lep1_phi")};
  TTreeReaderValue<Double_t> lep2_phi = {reader, getBranch("lep2_phi")};
  TTreeReaderValue<Double_t> lep3_phi = {reader, getBranch("lep3_phi")};

  TTreeReaderValue<Double_t> lep1_charge = {reader, getBranch("lep1_charge")};
  TTreeReaderValue<Double_t> lep2_charge = {reader, getBranch("lep2_charge")};
  TTreeReaderValue<Double_t> lep3_charge = {reader, getBranch("lep3_charge")};

  TTreeReaderValue<Double_t> lep1_type = {reader, getBranch("lep1_type")};
  TTreeReaderValue<Double_t> lep2_type = {reader, getBranch("lep2_type")};
  TTreeReaderValue<Double_t> lep3_type = {reader, getBranch("lep3_type")};

  TTreeReaderValue<Double_t> lep1_isTight = {reader, getBranch("lep1_isTight")};
  TTreeReaderValue<Double_t> lep2_isTight = {reader, getBranch("lep2_isTight")};
  TTreeReaderValue<Double_t> lep3_isTight = {reader, getBranch("lep3_isTight")};

  TTreeReaderValue<Double_t> jet1_pt = {reader, getBranch("jet1_pt")};
  TTreeReaderValue<Double_t> jet2_pt = {reader, getBranch("jet2_pt")};
  TTreeReaderValue<Double_t> jet3_pt = {reader, getBranch("jet3_pt")};
  TTreeReaderValue<Double_t> jet4_pt = {reader, getBranch("jet4_pt")};
  TTreeReaderValue<Double_t> jet5_pt = {reader, getBranch("jet5_pt")};

  TTreeReaderValue<Double_t> jet1_eta = {reader, getBranch("jet1_eta")};
  TTreeReaderValue<Double_t> jet2_eta = {reader, getBranch("jet2_eta")}; 

  TTreeReaderValue<Double_t> jet1_phi = {reader, getBranch("jet1_phi")};
  TTreeReaderValue<Double_t> jet2_phi = {reader, getBranch("jet2_phi")};

  TTreeReaderValue<Double_t> EtMiss   = {reader, getBranch("EtMiss")};
  TTreeReaderValue<Double_t> Ht       = {reader, getBranch("Ht")};
  TTreeReaderValue<Double_t> nZ       = {reader, getBranch("nZ")};
  TTreeReaderValue<Double_t> nJet     = {reader, getBranch("nJet")};
  TTreeReaderValue<Double_t> nBjet    = {reader, getBranch("nBjet")};
  TTreeReaderValue<Double_t> dEta_BDT = {reader, getBranch("dEta_BDT")};
 
  for(auto SR : SRs){
    INFO( Form("Running on region: %s", SR.c_str()) );

    std::vector<TH1F*> histos(0);
    histos.emplace_back( makeHisto("Njets",  {1., 2., 3., 4., 5., 6., 7., 10.}) );

    histos.emplace_back( makeHisto("LepPt1", 10, 0, 500) );
    histos.emplace_back( makeHisto("LepPt2", 10, 0, 250) );
    histos.emplace_back( makeHisto("LepPt3", 10, 0, 150) );

    histos.emplace_back( makeHisto("JetPt1", 10, 0, 500) );
    histos.emplace_back( makeHisto("JetPt2", 10, 0, 250) );

    histos.emplace_back( makeHisto("LepEta1", 6, -3.0, 3.0) );
    histos.emplace_back( makeHisto("LepPhi1", 8, -4.0, 4.0) );

    histos.emplace_back( makeHisto("JetEta1", 6, -3.0, 3.0) ); 
    histos.emplace_back( makeHisto("JetPhi1", 8, -4.0, 4.0) );

    histos.emplace_back( makeHisto("LepEta2", 6, -3.0, 3.0) );
    histos.emplace_back( makeHisto("LepPhi2", 8, -4.0, 4.0) );

    histos.emplace_back( makeHisto("Ht",     20,   0, 1000) );
    histos.emplace_back( makeHisto("EtMiss", 10,   0,  300) );
    histos.emplace_back( makeHisto("Deta",   2, -2.5,  2.5) );

    addSuffix(histos, SR);
    std::vector<float> var(histos.size());
  
    //Tool initialization
    asg::AnaToolHandle<CP::IFakeBkgTool>       LHMTool("CP::LhoodMM_tools/LHMTool_"+SR);
    asg::AnaToolHandle<CP::ILinearFakeBkgTool> ASMTool("CP::AsymptMatrixTool/ASMTool_"+SR);
    switch( MMType ){
    case 1:
      top::check(initializeMMTool(LHMTool), "LHMTool cannot be initialized");
      for(unsigned int i(0); i<var.size(); i++)
	top::check( LHMTool->register1DHistogram(histos[i], &var[i]), "Unable to register histogram" );
      break;
    case 2:
      top::check(initializeMMTool(ASMTool), "ASMTool cannot be initialized");
      for(unsigned int i(0); i<var.size(); i++)
	top::check( ASMTool->register1DHistogram(histos[i], &var[i]), "Unable to register histogram" );
      break;
    default: 
      ERROR("No MM type selected");
    }
  
    ULong64_t treeEntries = inTree->GetEntries();
    INFO( Form("Reading TTree %s :: Entries = %ld", inTree->GetName(), (long)treeEntries) );
  
    unsigned int NEvents(0), NLep(0), NLoose(0), NTight(0); float sumWeights(0);
    for(unsigned int entry(0); entry<treeEntries; entry++){

      reader.SetEntry(entry);
      printProcess(entry, (int)treeEntries);
      DEBUG( Form("Processing entry %i", entry) );

      float weightMM(0);   
      std::vector<double> lepPt  = {*lep1_pt,  *lep2_pt,  *lep3_pt};
      std::vector<double> lepEta = {*lep1_eta, *lep2_eta, *lep3_eta};
      std::vector<double> lepPhi = {*lep1_phi, *lep2_phi, *lep3_phi};

      std::vector<double> lepCharge = {*lep1_charge,  *lep2_charge,  *lep3_charge};
      std::vector<double> lepType   = {*lep1_type,    *lep2_type,    *lep3_type};
      std::vector<double> lepTight  = {*lep1_isTight, *lep2_isTight, *lep3_isTight};
      sortLeptons(lepPt, lepEta, lepPhi, lepCharge, lepType, lepTight);

      //Custom varibales
      bool isZ = (*nZ == 1);

      if( !passCuts((TString)SR, lepPt, lepCharge, *nJet, *nBjet, isZ) ) continue;
      DEBUG( Form("Entry %i, N(jets)=%f, N(b-jets)=%f, EtMiss=%.2f, isZ=%i", (int)entry, (float) *nJet, (float) *nBjet, (float) *EtMiss, (int)isZ) );

      //Tight/Loose count
      for(auto T : lepTight){
	(T==1) ? NTight++ : NLoose++;
	NLep++;
      }

      //Variable association
      var[0]  = (float) *nJet;
 
      var[1]  = (float) *lep1_pt;
      var[2]  = (float) *lep2_pt;
      var[3]  = (float) *lep3_pt;

      var[4]  = (float) *jet1_pt;
      var[5]  = (float) *jet2_pt;

      var[6]  = (float) *lep1_eta;
      var[7]  = (float) *lep1_phi;

      var[8]  = (float) *jet1_eta;
      var[9]  = (float) *jet1_phi;

      var[10] = (float) *lep2_eta;
      var[11] = (float) *lep2_phi;

      var[12] = (float) *Ht;
      var[13] = (float) *EtMiss;
      var[14] = (float) *dEta_BDT;

      std::shared_ptr<xAOD::TStore> store = std::make_shared<xAOD::TStore>();
      std::shared_ptr<xAOD::TEvent> event = std::make_shared<xAOD::TEvent>();

      xAOD::IParticleContainer leptons = makeIParticleContainer(lepPt, lepEta, lepPhi, lepCharge, lepType, lepTight);
      printLeptons(leptons);

      switch( MMType ){
      case 1:
	top::check( LHMTool->addEvent(leptons), "Cannot execute LHMTool::addEvent()"); break;
      case 2:
	top::check( ASMTool->addEvent(leptons), "Cannot execute ASMTool::addEvent()");
	top::check( ASMTool->getEventWeight(weightMM, selection, process), "Unable to get ASM weight");
	DEBUG( Form("Entry %i, N(lep) = %i :: weight(ASM) = %.3f", entry, (int)leptons.size(), weightMM) );
	break;
      default: 
	ERROR("No MM type selected");
      }

      for(auto l : leptons) delete l;
      leptons.clear();

      sumWeights += weightMM;
      NEvents++;
    }
    INFO( Form("Processed %i events for region %s :: N(lep)=%i, N(tight)=%i, N(loose)=%i \n", NEvents, SR.c_str(), NLep, NTight, NLoose) );

    float yields(0), yieldsUP(0), yieldsDOWN(0);
    switch( MMType ){
    case 1:
      top::check( LHMTool->getTotalYield(yields, yieldsUP, yieldsDOWN), "Cannot execute LHMTool::getTotalYield()"); break;
    case 2:
      top::check( ASMTool->getTotalYield(yields, yieldsUP, yieldsDOWN), "Cannot execute ASMTool::getTotalYield()"); break;
    default:
      ERROR("No MM type selected");
    }
    INFO( Form("MMTool::totalYields(%s) %.3f (nom) %.3f (up) %.3f (down)", SR.c_str(), yields, yieldsUP, yieldsDOWN) );
    if(MMType==2) INFO( Form("ASMTool::sumWeights(%s) %.3f", SR.c_str(), sumWeights) );

    for(auto h : histos){
      INFO( Form("Getting binned estimations in %s for %s:", SR.c_str(), h->GetName()) ); 
    
      for(int i(1); i<=h->GetNbinsX(); i++) INFO( Form("%s yields: Bin [%i] -> MM yields = %.3f", h->GetName(), i, (float)h->GetBinContent(i)) ); 
      INFO( Form("%s Integral: %.3f \n", h->GetName(), h->Integral()) );
    }

    TString outFileName(OutFile);
    top::check(writeHistos(histos, outFileName), "Cannot write histograms to output file");
  }

  INFO("Finalizing");
  auto end = std::time(nullptr);
  INFO(std::ctime(&end));
  
  return EXIT_SUCCESS;
}
