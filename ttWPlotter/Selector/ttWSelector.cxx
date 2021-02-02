#define ttWSelector_cxx

#include "ttWSelector.h"
#include <TH2.h>
#include <TStyle.h>

void ttWSelector::Begin(TTree * /*tree*/){

  const char* APP_NAME("Initalize");
  INFO(APP_NAME, Form("Debug mode = %i", (int)debug)   );
  INFO(APP_NAME, Form("isMC       = %i", (int)isMC)    );

  TString option = GetOption();
}

void ttWSelector::SlaveBegin(TTree * /*tree*/){

  TString option = GetOption();
  this->clearAll();
  doWeight = true;

  cutflowNames.clear();
  cutflowEvents.clear();
  cutflowWeights.clear();

  //SR
  makeHisto("Njets_SR",   bins_njets);

  makeHisto("LepPt1_SR",  10, 0,  500.);
  makeHisto("LepPt2_SR",  10, 0,  250.);
  makeHisto("LepPt3_SR",  10, 0,  250.);

  makeHisto("JetPt1_SR",  10, 0,  500.);
  makeHisto("JetPt2_SR",  10, 0,  250.);

  makeHisto("LepEta1_SR",  6, -3.0, 3.0);
  makeHisto("LepPhi1_SR",  8, -4.0, 4.0);

  makeHisto("JetEta1_SR",  6, -3.0, 3.0);
  makeHisto("JetPhi1_SR",  8, -4.0, 4.0);

  makeHisto("LepEta2_SR",  6, -3.0, 3.0);
  makeHisto("LepPhi2_SR",  8, -4.0, 4.0);

  makeHisto("Ht_SR",      20, 0,  1000.);
  makeHisto("EtMiss_SR",  10, 0,   300.);


  //CR-Fake
  makeHisto("LepPt1_CRFake",   10, 0,  500.);
  makeHisto("LepPt2_CRFake",   10, 0,  250.);
  makeHisto("LepPt3_CRFake",   10, 0,  250.);

  makeHisto("JetPt1_CRFake",   10, 0,  500.);
  makeHisto("JetPt2_CRFake",   10, 0,  250.);

  makeHisto("LepEta1_CRFake",  6, -3.0, 3.0);
  makeHisto("LepPhi1_CRFake",  8, -4.0, 4.0);

  makeHisto("JetEta1_CRFake",  6, -3.0, 3.0);
  makeHisto("JetPhi1_CRFake",  8, -4.0, 4.0);
   
  makeHisto("LepEta2_CRFake",  6, -3.0, 3.0);
  makeHisto("LepPhi2_CRFake",  8, -4.0, 4.0);

  makeHisto("Ht_CRFake",      20, 0,  1000.);
  makeHisto("EtMiss_CRFake",  10, 0,   300.);

 
  //ttZ-CR
  makeHisto("Njets_CRttZ",   bins_njets);

  makeHisto("LepPt1_CRttZ",  10, 0,  500.);
  makeHisto("LepPt2_CRttZ",  10, 0,  250.);
  makeHisto("LepPt3_CRttZ",  10, 0,  250.);

  makeHisto("JetPt1_CRttZ",  10, 0,  500.);
  makeHisto("JetPt2_CRttZ",  10, 0,  250.);

  makeHisto("LepEta1_CRttZ",  6, -3.0, 3.0);
  makeHisto("LepPhi1_CRttZ",  8, -4.0, 4.0);

  makeHisto("JetEta1_CRttZ",  6, -3.0, 3.0);
  makeHisto("JetPhi1_CRttZ",  8, -4.0, 4.0);

  makeHisto("LepEta2_CRttZ",  6, -3.0, 3.0);
  makeHisto("LepPhi2_CRttZ",  8, -4.0, 4.0);

  makeHisto("Ht_CRttZ",      20, 0,  1000.);
  makeHisto("EtMiss_CRttZ",  10, 0,   300.);


  //WZ-CR
  makeHisto("LepPt1_CRWZ",  10, 0,  500.);
  makeHisto("LepPt2_CRWZ",  10, 0,  250.);
  makeHisto("LepPt3_CRWZ",  10, 0,  250.);

  makeHisto("JetPt1_CRWZ",  10, 0,  500.);
  makeHisto("JetPt2_CRWZ",  10, 0,  250.);

  makeHisto("LepEta1_CRWZ",  6, -3.0, 3.0);
  makeHisto("LepPhi1_CRWZ",  8, -4.0, 4.0);

  makeHisto("JetEta1_CRWZ",  6, -3.0, 3.0);
  makeHisto("JetPhi1_CRWZ",  8, -4.0, 4.0);

  makeHisto("LepEta2_CRWZ",  6, -3.0, 3.0);
  makeHisto("LepPhi2_CRWZ",  8, -4.0, 4.0);

  makeHisto("Ht_CRWZ",      20, 0,  1000.);
  makeHisto("EtMiss_CRWZ",  10, 0,   300.);
}


Bool_t ttWSelector::Process(Long64_t entry){
  
  const char* APP_NAME("Process");
  fChain->GetTree()->GetEntry(entry);
  EventsTot++;

  DEBUG(APP_NAME, Form("Entry %ld : NJet=%i, nBjets=%i, EtMiss=%.3f, Ht=%.3f, N(Z)=%i", (long)entry, (int)nJets, (int)nBTags, (float)MET, (float)HT, (int)NZ_Cands));
  
  float w(1.);
  if(isMC && doWeight){ 
    w *= normWeight;
    w *= weight_leptonSF * weight_globalLeptonTriggerSF * weight_bTagSF_DL1r_77 * weight_pileup * weight_jvt;
    DEBUG(APP_NAME, Form("NormWeight = %.6f, lepSF=%.3f, triggerSF=%.3f, bTagSF=%.3f, prwSF=%.3f, jvtSF=%.3f \t total=%.3f", w, weight_leptonSF, weight_globalLeptonTriggerSF, weight_bTagSF_DL1r_77, weight_pileup, weight_jvt, w));
  }
 
  bool is3L_LLL = pt_lep0_pt>0 && pt_lep1_pt>0 && pt_lep2_pt>0 ;
  bool is3L_TTT = is3L_LLL && ( TMath::Abs(pt_lep0_isTight)==1 && TMath::Abs(pt_lep1_isTight)==1 && TMath::Abs(pt_lep2_isTight)==1);

  //bool fakeLep1 = isMC && isFake(GenName, type_lep1, true_type_lep1, true_origin_lep1, TruthIFF_Class_lep1);
  //bool fakeLep2 = isMC && isFake(GenName, type_lep2, true_type_lep2, true_origin_lep2, TruthIFF_Class_lep2);
  //bool fakeLep3 = isMC && isFake(GenName, type_lep3, true_type_lep3, true_origin_lep3, TruthIFF_Class_lep3);
  //countLepTypes(TruthIFF_Class_lep1, TruthIFF_Class_lep2, TruthIFF_Class_lep3);

  //bool removeMCFakes = true;
  //bool isMCFake = removeMCFakes ? (fakeLep1 || fakeLep2 || fakeLep3) : false;

  //bool is3L_PPP = is3L_TTT && !isMCFake;
  bool is3L_PPP = is3L_TTT;

  addCutflow(w, is3L_LLL, "3L_loose");
  addCutflow(w, is3L_TTT, "3L_tight");
  addCutflow(w, is3L_PPP, "3L_tight_prompt");
  
  bool isZ = NZ_Cands > 0;
  int sumQ = TMath::Abs(pt_lep0_charge + pt_lep1_charge + pt_lep2_charge);

  DEBUG(APP_NAME, Form("pt(lep1)=%.1f (T=%i), pt(lep2)=%.1f (T=%i), pt(lep3)=%.1f (T=%i), (isZ=%i), sum(Q)=%i, Ht=%.1f", 
		       (float)pt_lep0_pt, (int)TMath::Abs(pt_lep0_isTight), (float)pt_lep1_pt, (int)TMath::Abs(pt_lep1_isTight), (float)pt_lep2_pt, (int)TMath::Abs(pt_lep2_isTight), (int)isZ, (int)sumQ==1, (float)HT));

  bool pass3L = is3L_PPP && (pt_lep0_pt > lepPtCuts[0] && pt_lep1_pt > lepPtCuts[1] && pt_lep2_pt > lepPtCuts[2]);
  addCutflow(w, pass3L, "pass3L");
  
  bool pass_SR     = pass3L && nJets>=2 && nBTags>=1 && sumQ==1 && !isZ; 
  bool pass_CRFake = pass3L && nJets==3 && nBTags==1;
  bool pass_CRttZ  = pass3L && nJets>=4 && nBTags>=2 && MET>50 && sumQ==1 && isZ;
  bool pass_CRWZ   = pass3L && nJets==1 && nBTags==0 && MET>50 && sumQ==1 && isZ;

  addCutflow(w, pass_SR,     "SR");
  addCutflow(w, pass_CRFake, "CRFake");
  addCutflow(w, pass_CRttZ,  "CRttZ");
  addCutflow(w, pass_CRWZ,   "CRWZ");

  if(pass_SR){  
    fillHisto("Njets_SR",  nJets, w);
    
    fillHisto("LepPt1_SR", pt_lep0_pt, w);
    fillHisto("LepPt2_SR", pt_lep1_pt, w);
    fillHisto("LepPt3_SR", pt_lep2_pt, w);

    fillHisto("JetPt1_SR", pt_jet0_pt, w);
    fillHisto("JetPt2_SR", pt_jet1_pt, w);

    fillHisto("LepEta1_SR", pt_lep0_eta, w);
    fillHisto("LepPhi1_SR", pt_lep0_phi, w);

    fillHisto("JetEta1_SR", pt_jet0_eta, w);
    fillHisto("JetPhi1_SR", pt_jet0_phi, w);

    fillHisto("LepEta2_SR", pt_lep1_eta, w);
    fillHisto("LepPhi2_SR", pt_lep1_phi, w);

    fillHisto("Ht_SR",      HT, w);
    fillHisto("EtMiss_SR", MET, w);
  }

  if(pass_CRFake){
    fillHisto("LepPt1_CRFake", pt_lep0_pt, w);
    fillHisto("LepPt2_CRFake", pt_lep1_pt, w);
    fillHisto("LepPt3_CRFake", pt_lep2_pt, w);

    fillHisto("JetPt1_CRFake", pt_jet0_pt, w);
    fillHisto("JetPt2_CRFake", pt_jet1_pt, w);

    fillHisto("LepEta1_CRFake", pt_lep0_eta, w);
    fillHisto("LepPhi1_CRFake", pt_lep0_phi, w);

    fillHisto("JetEta1_CRFake", pt_jet0_eta, w);
    fillHisto("JetPhi1_CRFake", pt_jet0_phi, w);

    fillHisto("LepEta2_CRFake", pt_lep1_eta, w);
    fillHisto("LepPhi2_CRFake", pt_lep1_phi, w);

    fillHisto("Ht_CRFake",      HT, w);
    fillHisto("EtMiss_CRFake", MET, w);
  }

  if(pass_CRttZ){
    fillHisto("Njets_CRttZ",  nJets, w);

    fillHisto("LepPt1_CRttZ", pt_lep0_pt, w);
    fillHisto("LepPt2_CRttZ", pt_lep1_pt, w);
    fillHisto("LepPt3_CRttZ", pt_lep2_pt, w);

    fillHisto("JetPt1_CRttZ", pt_jet0_pt, w);
    fillHisto("JetPt2_CRttZ", pt_jet1_pt, w);

    fillHisto("LepEta1_CRttZ", pt_lep0_eta, w);
    fillHisto("LepPhi1_CRttZ", pt_lep0_phi, w);

    fillHisto("JetEta1_CRttZ", pt_jet0_eta, w);
    fillHisto("JetPhi1_CRttZ", pt_jet0_phi, w);

    fillHisto("LepEta2_CRttZ", pt_lep1_eta, w);
    fillHisto("LepPhi2_CRttZ", pt_lep1_phi, w);

    fillHisto("Ht_CRttZ",      HT, w);
    fillHisto("EtMiss_CRttZ", MET, w);
  }

  if(pass_CRWZ){
    fillHisto("LepPt1_CRWZ", pt_lep0_pt, w);
    fillHisto("LepPt2_CRWZ", pt_lep1_pt, w);
    fillHisto("LepPt3_CRWZ", pt_lep2_pt, w);

    fillHisto("JetPt1_CRWZ", pt_jet0_pt, w);
    fillHisto("JetPt2_CRWZ", pt_jet1_pt, w);

    fillHisto("LepEta1_CRWZ", pt_lep0_eta, w);
    fillHisto("LepPhi1_CRWZ", pt_lep0_phi, w);

    fillHisto("JetEta1_CRWZ", pt_jet0_eta, w);
    fillHisto("JetPhi1_CRWZ", pt_jet0_phi, w);

    fillHisto("LepEta2_CRWZ", pt_lep1_eta, w);
    fillHisto("LepPhi2_CRWZ", pt_lep1_phi, w);

    fillHisto("Ht_CRWZ",      HT, w);
    fillHisto("EtMiss_CRWZ", MET, w);
  }

  sumWeights += w;
  return kTRUE;
}

void ttWSelector::SlaveTerminate(){
  const char* APP_NAME("SlaveTerminate");
  INFO(APP_NAME, "Terminating" );
}

void ttWSelector::Terminate(){
  const char* APP_NAME("Terminate");
  INFO(APP_NAME, Form("Events processed = %i  Sum of weights = %.3f", EventsTot, sumWeights));

  this->sortCutflow();
  this->printCutflow();
  this->printLepTypes();
  this->write();
}

void ttWSelector::makeHisto(TString name, int bins, float xMin, float xMax){
  TH1F *h = new TH1F(name, name, bins, xMin, xMax);
  hOut.push_back(h);

  INFO("makeHisto", Form("Added TH1F %s: \t Bins: %i [%.1f, %.1f]",h->GetName(),bins,xMin,xMax));
}

void ttWSelector::makeHisto(TString name, std::vector<float> ranges){  
  const unsigned int N = ranges.size();

  float *x = new float[N];
  for(unsigned int i(0); i<N; i++) x[i] = ranges.at(i);

  TH1F *h = new TH1F(name, name, N-1, x);
  hOut.push_back(h);

  INFO("makeHisto", Form("Added TH1F %s \t Bins: %i [%.1f, %.1f] (var)",h->GetName(),N-1,ranges.front(),ranges.back()));
}

void ttWSelector::fillHisto(TString name, float val, float w){
  for (auto h : hOut){
    if( h->GetName()==name ) h->Fill(val, w);
  }
}

void ttWSelector::checkBinContent(TH1F* h){
  if(!h) return;
  for(int i(0); i<=h->GetNbinsX(); i++){ if(h->GetBinContent(i)<0.){ h->SetBinContent(i,0); h->SetBinError(i,0); } }
}

bool ttWSelector::isFake(TString mcname, float type, int truthType, int truthOrigin, float IFFClass){
  TString lepFlavor = type==1. ? "Electron" : (type==2. ? "Muon" : (type==3 ? "Tau" : "Unknown"));
  DEBUG("makeHisto", Form("Generator: %s, type=%.0f(%s), truthType=%i truthOrigin=%i, IFFClass=%.0f", mcname.Data(), type, lepFlavor.Data(), truthType, truthOrigin, IFFClass));

  if(mcname.Contains("Sherpa")){
    if(type==1. &&  IFFClass==2)  return false;
    if(type==2. && (IFFClass==4 || (truthType==5 && truthOrigin==0))) return false;
  }
  else{
    if(type==1. && IFFClass==2) return false;
    if(type==2. && IFFClass==4) return false;
  }
  return true;
}

void ttWSelector::countLepTypes(float class_lep1, float class_lep2, float class_lep3){
  for(std::map<std::string,int>::iterator i=lepTypes.begin(); i!=lepTypes.end(); ++i){
    if( i->first == IFFEventType(class_lep1, class_lep2, class_lep3) ) i->second ++;
  }
}

std::string ttWSelector::IFFEventType(float class_lep1, float class_lep2, float class_lep3){
  if( (class_lep1==2 || class_lep1==4) && (class_lep2==2 || class_lep2==4) && (class_lep3==2 || class_lep3==4) ) return "Prompt";
  if( class_lep1==3 || class_lep2==3 || class_lep3==3 ) return "ChMisID";
  if( class_lep1==8 || class_lep1==9 || class_lep2==8 || class_lep2==9 || class_lep3==8 || class_lep3==9 ) return "HF";
  if( class_lep1==10 || class_lep2==10 || class_lep3==10 ) return "LF";
  if( class_lep1==5 || class_lep2==5 || class_lep3==5 ) return "PhConv";
  if( class_lep1==7 || class_lep2==7 || class_lep3==7 ) return "Tau";
  return "Unknown";
}

void ttWSelector::clearAll(){
  INFO("clearAll", "Resetting all values");
  hOut.clear();

  doWeight   = false;
  EventsTot  = 0;
  sumWeights = 0;

  outPathName.Clear();
  outFileName.Clear();
}

void ttWSelector::addCutflow(float w, bool pass, TString name){
  if(!pass) return;

  int cutPos(-1);
  for(unsigned int i(0); i<cutflowNames.size(); i++){if(name == cutflowNames.at(i)) cutPos = i;}
  if(cutPos < 0){ 
    cutflowNames.push_back(name);
    cutflowEvents.push_back(1);
    cutflowWeights.push_back(w);
  }
  else{ 
    cutflowEvents.at(cutPos)  += 1;
    cutflowWeights.at(cutPos) += w;
  }
  return;
}

void ttWSelector::sortCutflow(){
  std::vector<int> events = cutflowEvents;
  std::sort(events.rbegin(),events.rend());

  unsigned int N = events.size();
  std::vector<float> weights(N);
  std::vector<TString> names(N);

  std::vector<int> index(N);
  for(unsigned int i(0); i<N; i++){
    for(unsigned int j(0); j<N; j++){
      if( (events[i] == cutflowEvents[j]) && std::find(index.begin(),index.end(),j)==index.end() ) index[i] = j;
    }
  }
  for(unsigned int i(0); i<index.size(); i++){
    weights[i] = cutflowWeights[index[i]];
    names[i]   = cutflowNames[index[i]];
  }
  cutflowEvents.clear();
  cutflowNames.clear();
  cutflowWeights.clear();

  cutflowEvents  = events;
  cutflowWeights = weights;
  cutflowNames   = names;
  return;
}

void ttWSelector::printCutflow(){
  INFO("printCutflow", "Summary");
  for(unsigned int i(0); i<cutflowNames.size(); i++)
    INFO("printCutflow", Form("Cut %i :: %s \t\t Ev: %i (%.1f %%) \t Weights: %.1f  (%.1f %%)", i,
			      cutflowNames[i].Data(), cutflowEvents[i], float(cutflowEvents[i])/float(EventsTot)*100, cutflowWeights[i], float(cutflowWeights[i])/float(sumWeights)*100)); 
  return;
}

void ttWSelector::printLepTypes(){
  INFO("printLepTypes", "Summary");
  for(std::map<std::string,int>::iterator i=lepTypes.begin(); i!=lepTypes.end(); ++i){
    INFO("printLepTypes", Form("LepType :: %s \t\t Ev: %i (%.1f %%)",(i->first).c_str(), int(i->second), float(i->second)/float(EventsTot)*100));
  }
  return;
}

void ttWSelector::setLumiNorm(float l){
  INFO("setLumiNorm", Form("Set total luminosity to %.1f fb^-1", l));
  lumi = l;
}

void ttWSelector::setMCcamp(std::string camp){
  if(!isMC) return;
  MCcamp = camp;
  INFO("setMCcamp", Form("Filename: %s -> MC campaign: %s",outFileName.Data(),MCcamp.Data()));
  outFileName.ReplaceAll(".root", Form("_%s.root",MCcamp.Data()));
}

void ttWSelector::setGenName(std::string genname){
  if(!isMC) return;
  GenName = genname;
  INFO("setGenName", Form("Filename: %s -> Generator name: %s",outFileName.Data(),GenName.Data()));
}

void ttWSelector::setOutPathName(std::string path){
  outPathName = path;
  if(!outPathName.Length()) outPathName = ".";
  INFO("setOutPathName", Form("Set outpath name to %s", outPathName.Data()));
}

void ttWSelector::setOutFileName(std::string file){
  outFileName = file;
  INFO("setOutFileName", Form("Set outfile name to %s", outFileName.Data()));
}

void ttWSelector::write(){
  
  fOut = std::make_unique<TFile>(outPathName+"/"+outFileName, "RECREATE");
  INFO("write", Form("Created output file %s", fOut->GetName()));
  INFO("write", Form("Writing %i histograms", (int)hOut.size()));

  fOut->cd();
  for (auto h : hOut){ 
    checkBinContent(h);
    h->Write(); 
  }
}
