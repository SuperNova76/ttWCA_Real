{
  gROOT->LoadMacro("Plotter/ttWPlotter.cxx++");
  gROOT->ProcessLine("ttWPlotter p");
  gErrorIgnoreLevel = kFatal;
  p.setAtlasStylePath("/lhome/ific/c/cardillo/Atlas/atlasrootstyle/AtlasStyle.C");

  TString path = "HISTOS";
  TString OutPath = "/lhome/ific/c/cardillo/ttW-CA/Plots/Plots-12-02-21/DD/";
  float lumi = 139.;

  p.useData(1);
  p.useMMFakes(1);

  p.doLogY(0);
  p.setInPath(path);
  p.setOutPath(OutPath);
  
  p.setLumi(lumi);
  p.drawOverflow(1);
  p.drawAtlasLabel(1);
  p.drawLegNumbers(0);
  //p.setRatioLine("ttW");
  p.setYRangeRatio(0.2,1.8);

  //Signal process and norm
  p.setSignal("ttW", 10);

  p.doDebug(0);
  p.doPrint(0);
  p.setLabel("");
  p.setFigureFormat("eps");

  p.setBkgNorm("Fakes", 1.00);
  bool setFlatErrors(false);
  if(setFlatErrors){
    p.setBkgError("ttZ",   0.1);
    p.setBkgError("ttW",   0.1);
    p.setBkgError("ttH",   0.1);
    p.setBkgError("tZ",    0.1);
    p.setBkgError("Other", 0.1);
    p.setBkgError("VV",    0.1);
  }
  //p.setVariations(true, "Fakes");

  bool fixedYRange(false);
  if(fixedYRange) p.setYRange(0.015,24);

  bool SR1b_low(true), SR1b_high(true), SR2b_low(true), SR2b_high(true), CRttZ(true);

  TString label_SR1b_low  = "SR: 3#it{l}, 1#it{b}, 2-3#it{j}";
  TString label_SR2b_low  = "SR: 3#it{l}, 2#it{b}, 2-3#it{j}";
  TString label_SR1b_high = "SR: 3#it{l}, 1#it{b}, #geq4#it{j}";
  TString label_SR2b_high = "SR: 3#it{l}, 2#it{b}, #geq4#it{j}";
  TString label_CRttZ     = "CR: #it{t#bar{t}Z}";

  if(SR1b_low){
    p.plot("Njets_SR1b_low",      1,  2,  10,  label_SR1b_low);

    p.plot("LepPt1_SR1b_low",     1,  0,  500, label_SR1b_low);
    p.plot("LepPt2_SR1b_low",     1,  0,  250, label_SR1b_low);
    p.plot("LepPt3_SR1b_low",     1,  0,  250, label_SR1b_low);

    p.plot("JetPt1_SR1b_low",     1,  0,  500, label_SR1b_low);
    p.plot("JetPt2_SR1b_low",     1,  0,  250, label_SR1b_low);

    p.plot("LepEta1_SR1b_low",    1, -3.0, 3.0, label_SR1b_low);
    p.plot("LepPhi1_SR1b_low",    1, -4.0, 4.0, label_SR1b_low);

    p.plot("JetEta1_SR1b_low",    1, -3.0, 3.0, label_SR1b_low);
    p.plot("JetPhi1_SR1b_low",    1, -4.0, 4.0, label_SR1b_low);

    p.plot("LepEta2_SR1b_low",    1, -3.0, 3.0, label_SR1b_low);
    p.plot("LepPhi2_SR1b_low",    1, -4.0, 4.0, label_SR1b_low);

    p.plot("Ht_SR1b_low",         1, 300, 1000, label_SR1b_low);
    p.plot("EtMiss_SR1b_low",     1,  0,   300, label_SR1b_low);
    p.plot("Deta_SR1b_low",       1, -2.5, 2.5, label_SR1b_low);
  }

  if(SR1b_high){
    p.plot("Njets_SR1b_high",      1,  4,  10,  label_SR1b_high);

    p.plot("LepPt1_SR1b_high",     1,  0,  500, label_SR1b_high);
    p.plot("LepPt2_SR1b_high",     1,  0,  250, label_SR1b_high);
    p.plot("LepPt3_SR1b_high",     1,  0,  250, label_SR1b_high);

    p.plot("JetPt1_SR1b_high",     1,  0,  500, label_SR1b_high);
    p.plot("JetPt2_SR1b_high",     1,  0,  250, label_SR1b_high);

    p.plot("LepEta1_SR1b_high",    1, -3.0, 3.0, label_SR1b_high);
    p.plot("LepPhi1_SR1b_high",    1, -4.0, 4.0, label_SR1b_high);

    p.plot("JetEta1_SR1b_high",    1, -3.0, 3.0, label_SR1b_high);
    p.plot("JetPhi1_SR1b_high",    1, -4.0, 4.0, label_SR1b_high);

    p.plot("LepEta2_SR1b_high",    1, -3.0, 3.0, label_SR1b_high);
    p.plot("LepPhi2_SR1b_high",    1, -4.0, 4.0, label_SR1b_high);

    p.plot("Ht_SR1b_high",         1, 300, 1000, label_SR1b_high);
    p.plot("EtMiss_SR1b_high",     1,  0,   300, label_SR1b_high);
    p.plot("Deta_SR1b_high",       1, -2.5, 2.5, label_SR1b_high);
  }

  if(SR2b_low){
    p.plot("Njets_SR2b_low",      1,  2,  10,  label_SR2b_low);

    p.plot("LepPt1_SR2b_low",     1,  0,  500, label_SR2b_low);
    p.plot("LepPt2_SR2b_low",     1,  0,  250, label_SR2b_low);
    p.plot("LepPt3_SR2b_low",     1,  0,  250, label_SR2b_low);

    p.plot("JetPt1_SR2b_low",     1,  0,  500, label_SR2b_low);
    p.plot("JetPt2_SR2b_low",     1,  0,  250, label_SR2b_low);

    p.plot("LepEta1_SR2b_low",    1, -3.0, 3.0, label_SR2b_low);
    p.plot("LepPhi1_SR2b_low",    1, -4.0, 4.0, label_SR2b_low);

    p.plot("JetEta1_SR2b_low",    1, -3.0, 3.0, label_SR2b_low);
    p.plot("JetPhi1_SR2b_low",    1, -4.0, 4.0, label_SR2b_low);

    p.plot("LepEta2_SR2b_low",    1, -3.0, 3.0, label_SR2b_low);
    p.plot("LepPhi2_SR2b_low",    1, -4.0, 4.0, label_SR2b_low);

    p.plot("Ht_SR2b_low",         1, 300, 1000, label_SR2b_low);
    p.plot("EtMiss_SR2b_low",     1,  0,   300, label_SR2b_low);
    p.plot("Deta_SR2b_low",       1, -2.5, 2.5, label_SR2b_low);
  }

  if(SR2b_high){
    p.plot("Njets_SR2b_high",      1,  4,  10,  label_SR2b_high);

    p.plot("LepPt1_SR2b_high",     1,  0,  500, label_SR2b_high);
    p.plot("LepPt2_SR2b_high",     1,  0,  250, label_SR2b_high);
    p.plot("LepPt3_SR2b_high",     1,  0,  250, label_SR2b_high);

    p.plot("JetPt1_SR2b_high",     1,  0,  500, label_SR2b_high);
    p.plot("JetPt2_SR2b_high",     1,  0,  250, label_SR2b_high);

    p.plot("LepEta1_SR2b_high",    1, -3.0, 3.0, label_SR2b_high);
    p.plot("LepPhi1_SR2b_high",    1, -4.0, 4.0, label_SR2b_high);

    p.plot("JetEta1_SR2b_high",    1, -3.0, 3.0, label_SR2b_high);
    p.plot("JetPhi1_SR2b_high",    1, -4.0, 4.0, label_SR2b_high);

    p.plot("LepEta2_SR2b_high",    1, -3.0, 3.0, label_SR2b_high);
    p.plot("LepPhi2_SR2b_high",    1, -4.0, 4.0, label_SR2b_high);

    p.plot("Ht_SR2b_high",         1, 300, 1000, label_SR2b_high);
    p.plot("EtMiss_SR2b_high",     1,  0,   300, label_SR2b_high);
    p.plot("Deta_SR2b_high",       1, -2.5, 2.5, label_SR2b_high);
  }

  if(CRttZ){
    p.plot("Njets_CRttZ",      1, 4,  10,   label_CRttZ);

    p.plot("LepPt1_CRttZ",     1,  0,  500, label_CRttZ);
    p.plot("LepPt2_CRttZ",     1,  0,  250, label_CRttZ);
    p.plot("LepPt3_CRttZ",     1,  0,  250, label_CRttZ);

    p.plot("JetPt1_CRttZ",     1,  0,  500, label_CRttZ);
    p.plot("JetPt2_CRttZ",     1,  0,  250, label_CRttZ);

    p.plot("LepEta1_CRttZ",    1, -3.0, 3.0, label_CRttZ);
    p.plot("LepPhi1_CRttZ",    1, -4.0, 4.0, label_CRttZ);

    p.plot("JetEta1_CRttZ",    1, -3.0, 3.0, label_CRttZ);
    p.plot("JetPhi1_CRttZ",    1, -4.0, 4.0, label_CRttZ);

    p.plot("LepEta2_CRttZ",    1, -3.0, 3.0, label_CRttZ);
    p.plot("LepPhi2_CRttZ",    1, -4.0, 4.0, label_CRttZ);
    
    p.plot("Ht_CRttZ",         1, 300, 1000, label_CRttZ);
    p.plot("EtMiss_CRttZ",     1,   0,  300, label_CRttZ);
    p.plot("Deta_CRttZ",      1, -2.5, 2.5,  label_CRttZ);
  }
}
