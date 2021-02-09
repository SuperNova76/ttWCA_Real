{
  gROOT->LoadMacro("Plotter/ttWPlotter.cxx++");
  gROOT->ProcessLine("ttWPlotter p");
  p.setAtlasStylePath("/lhome/ific/c/cardillo/Atlas/atlasrootstyle/AtlasStyle.C");

  TString path = "HISTOS";
  TString OutPath = "/lhome/ific/c/cardillo/ttW-CA/Plots/Plots-12-02-21/";
  float lumi = 139.;

  p.useData(1);
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

  bool SR1b_low(true), SR1b_high(true), SR2b_low(true), SR2b_high(true);
  bool CRFake(true), CRttZ(true);

  if(SR1b_low){
    p.plot("Njets_SR1b_low",      1,  2,  10,  "SR: 3l-1b,2-3j");
   
    p.plot("LepPt1_SR1b_low",     1,  0,  500, "SR: 3l-1b,2-3j");
    p.plot("LepPt2_SR1b_low",     1,  0,  250, "SR: 3l-1b,2-3j");
    p.plot("LepPt3_SR1b_low",     1,  0,  250, "SR: 3l-1b,2-3j");

    p.plot("JetPt1_SR1b_low",     1,  0,  500, "SR: 3l-1b,2-3j");
    p.plot("JetPt2_SR1b_low",     1,  0,  250, "SR: 3l-1b,2-3j");

    p.plot("LepEta1_SR1b_low",    1, -3.0, 3.0, "SR: 3l-1b,2-3j");
    p.plot("LepPhi1_SR1b_low",    1, -4.0, 4.0, "SR: 3l-1b,2-3j");

    p.plot("JetEta1_SR1b_low",    1, -3.0, 3.0, "SR: 3l-1b,2-3j");
    p.plot("JetPhi1_SR1b_low",    1, -4.0, 4.0, "SR: 3l-1b,2-3j");

    p.plot("LepEta2_SR1b_low",    1, -3.0, 3.0, "SR: 3l-1b,2-3j");
    p.plot("LepPhi2_SR1b_low",    1, -4.0, 4.0, "SR: 3l-1b,2-3j");

    p.plot("Ht_SR1b_low",         1, 300, 1000, "SR: 3l-1b,2-3j");
    p.plot("EtMiss_SR1b_low",     1,  0,   300, "SR: 3l-1b,2-3j");
  }

  if(SR1b_high){
    p.plot("Njets_SR1b_high",      1,  2,  10,  "SR: 3l-1b,#geq4j");
    p.plot("LepPt1_SR1b_high",     1,  0,  500, "SR: 3l-1b,#geq4j");
    p.plot("LepPt2_SR1b_high",     1,  0,  250, "SR: 3l-1b,#geq4j");
    p.plot("LepPt3_SR1b_high",     1,  0,  250, "SR: 3l-1b,#geq4j");

    p.plot("JetPt1_SR1b_high",     1,  0,  500, "SR: 3l-1b,#geq4j");
    p.plot("JetPt2_SR1b_high",     1,  0,  250, "SR: 3l-1b,#geq4j");

    p.plot("LepEta1_SR1b_high",    1, -3.0, 3.0, "SR: 3l-1b,#geq4j");
    p.plot("LepPhi1_SR1b_high",    1, -4.0, 4.0, "SR: 3l-1b,#geq4j");

    p.plot("JetEta1_SR1b_high",    1, -3.0, 3.0, "SR: 3l-1b,#geq4j");
    p.plot("JetPhi1_SR1b_high",    1, -4.0, 4.0, "SR: 3l-1b,#geq4j");

    p.plot("LepEta2_SR1b_high",    1, -3.0, 3.0, "SR: 3l-1b,#geq4j");
    p.plot("LepPhi2_SR1b_high",    1, -4.0, 4.0, "SR: 3l-1b,#geq4j");

    p.plot("Ht_SR1b_high",         1, 300, 1000, "SR: 3l-1b,#geq4j");
    p.plot("EtMiss_SR1b_high",     1,  0,   300, "SR: 3l-1b,#geq4j");
  }

  if(SR2b_low){
    p.plot("Njets_SR2b_low",      1,  2,  10,  "SR: 3l-2b,2-3j");
    p.plot("LepPt1_SR2b_low",     1,  0,  500, "SR: 3l-2b,2-3j");
    p.plot("LepPt2_SR2b_low",     1,  0,  250, "SR: 3l-2b,2-3j");
    p.plot("LepPt3_SR2b_low",     1,  0,  250, "SR: 3l-2b,2-3j");

    p.plot("JetPt1_SR2b_low",     1,  0,  500, "SR: 3l-2b,2-3j");
    p.plot("JetPt2_SR2b_low",     1,  0,  250, "SR: 3l-2b,2-3j");

    p.plot("LepEta1_SR2b_low",    1, -3.0, 3.0, "SR: 3l-2b,2-3j");
    p.plot("LepPhi1_SR2b_low",    1, -4.0, 4.0, "SR: 3l-2b,2-3j");

    p.plot("JetEta1_SR2b_low",    1, -3.0, 3.0, "SR: 3l-2b,2-3j");
    p.plot("JetPhi1_SR2b_low",    1, -4.0, 4.0, "SR: 3l-2b,2-3j");

    p.plot("LepEta2_SR2b_low",    1, -3.0, 3.0, "SR: 3l-2b,2-3j");
    p.plot("LepPhi2_SR2b_low",    1, -4.0, 4.0, "SR: 3l-2b,2-3j");

    p.plot("Ht_SR2b_low",         1, 300, 1000, "SR: 3l-2b,2-3j");
    p.plot("EtMiss_SR2b_low",     1,  0,   300, "SR: 3l-2b,2-3j");
  }

  if(SR2b_high){
    p.plot("Njets_SR2b_high",      1,  2,  10,  "SR: 3l-2b,#geq4j");
    p.plot("LepPt1_SR2b_high",     1,  0,  500, "SR: 3l-2b,#geq4j");
    p.plot("LepPt2_SR2b_high",     1,  0,  250, "SR: 3l-2b,#geq4j");
    p.plot("LepPt3_SR2b_high",     1,  0,  250, "SR: 3l-2b,#geq4j");

    p.plot("JetPt1_SR2b_high",     1,  0,  500, "SR: 3l-2b,#geq4j");
    p.plot("JetPt2_SR2b_high",     1,  0,  250, "SR: 3l-2b,#geq4j");

    p.plot("LepEta1_SR2b_high",    1, -3.0, 3.0, "SR: 3l-2b,#geq4j");
    p.plot("LepPhi1_SR2b_high",    1, -4.0, 4.0, "SR: 3l-2b,#geq4j");

    p.plot("JetEta1_SR2b_high",    1, -3.0, 3.0, "SR: 3l-2b,#geq4j");
    p.plot("JetPhi1_SR2b_high",    1, -4.0, 4.0, "SR: 3l-2b,#geq4j");

    p.plot("LepEta2_SR2b_high",    1, -3.0, 3.0, "SR: 3l-2b,#geq4j");
    p.plot("LepPhi2_SR2b_high",    1, -4.0, 4.0, "SR: 3l-2b,#geq4j");

    p.plot("Ht_SR2b_high",         1, 300, 1000, "SR: 3l-2b,#geq4j");
    p.plot("EtMiss_SR2b_high",     1,  0,   300, "SR: 3l-2b,#geq4j");
  }

  if(CRFake){
    p.plot("LepPt1_CRFake",     1,  0,  500, "CR: 3l-1b3j");
    p.plot("LepPt2_CRFake",     1,  0,  250, "CR: 3l-1b3j");
    p.plot("LepPt3_CRFake",     1,  0,  250, "CR: 3l-1b3j");

    p.plot("JetPt1_CRFake",     1,  0,  500, "CR: 3l-1b3j");
    p.plot("JetPt2_CRFake",     1,  0,  250, "CR: 3l-1b3j");

    p.plot("LepEta1_CRFake",    1, -3.0, 3.0, "CR: 3l-1b3j");
    p.plot("LepPhi1_CRFake",    1, -4.0, 4.0, "CR: 3l-1b3j");
  
    p.plot("JetEta1_CRFake",    1, -3.0, 3.0, "CR: 3l-1b3j");
    p.plot("JetPhi1_CRFake",    1, -4.0, 4.0, "CR: 3l-1b3j");

    p.plot("LepEta2_CRFake",    1, -3.0, 3.0, "CR: 3l-1b3j");
    p.plot("LepPhi2_CRFake",    1, -4.0, 4.0, "CR: 3l-1b3j");

    p.plot("Ht_CRFake",         1, 300, 1000,  "CR: 3l-1b3j");
    p.plot("EtMiss_CRFake",     1,   0,  300,  "CR: 3l-1b3j");
  }

  if(CRttZ){
    p.plot("Njets_CRttZ",      1, 4,  10,   "CR: 3l-ttZ");

    p.plot("LepPt1_CRttZ",     1,  0,  500, "CR: 3l-ttZ");
    p.plot("LepPt2_CRttZ",     1,  0,  250, "CR: 3l-ttZ");
    p.plot("LepPt3_CRttZ",     1,  0,  250, "CR: 3l-ttZ");

    p.plot("JetPt1_CRttZ",     1,  0,  500, "CR: 3l-ttZ");
    p.plot("JetPt2_CRttZ",     1,  0,  250, "CR: 3l-ttZ");

    p.plot("LepEta1_CRttZ",    1, -3.0, 3.0, "CR: 3l-ttZ");
    p.plot("LepPhi1_CRttZ",    1, -4.0, 4.0, "CR: 3l-ttZ");

    p.plot("JetEta1_CRttZ",    1, -3.0, 3.0, "CR: 3l-ttZ");
    p.plot("JetPhi1_CRttZ",    1, -4.0, 4.0, "CR: 3l-ttZ");

    p.plot("LepEta2_CRttZ",    1, -3.0, 3.0, "CR: 3l-ttZ");
    p.plot("LepPhi2_CRttZ",    1, -4.0, 4.0, "CR: 3l-ttZ");
    
    p.plot("Ht_CRttZ",         1, 300, 1000, "CR: 3l-ttZ");
    p.plot("EtMiss_CRttZ",     1,  0,  300, "CR: 3l-ttZ");
  }
}
