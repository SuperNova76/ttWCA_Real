#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "TROOT.h"
#include "TRint.h"
#include "TSystem.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TObjString.h"
#include "TString.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TLatex.h"
#include "TFile.h"
#include "TLegend.h"
#include "TStyle.h"
#include "THStack.h"
#include "TGraph.h"
#include "RooStats/NumberCountingUtils.h"
#include "RooGlobalFunc.h"
#include "RooStats/RooStatsUtils.h"

class ttWPlotter
{
 public:
  ttWPlotter(std::string name = "ttWPlotter"){
    CNAME = name;
    std::cout << "Initialize " << CNAME << std::endl;
    Debug = 0;
    Print = 0;
    Style = 0;
    Lumi  = 0;
    LogY  = 0;
    OFBin = 0;
    legNo = 0;
    YMin  = -1;
    YMax  = -1;
    figType = "pdf";
    OpenFiles  = 0;
    AtlasLabel = 0;
    Variations = 0;
    ratioYMin  = 0;
    ratioYMax  = 0;
    doMMFakes  = 0;
    PlotLabel.Clear();
    InPath.Clear();
    AtlasStylePath.Clear();
    OutPath.Clear();
    files.clear();
    filesVar.clear();
    fnames.clear();
    fnamesVar.clear();
    bkgList.clear();
    bkgTypes.clear();
    bkgTypesOriginal.clear();
    bkgErrNames.clear();
    bkgErrValues.clear();
    bkgNormNames.clear();
    bkgNormValues.clear();
    MCTypes.clear();
    sortedFiles.clear();
    RatioLine.Clear();
    VarBkgType.Clear();
    sigName.Clear();
    sigNorm = 0;
  };
  ~ttWPlotter(){};


 public:
  void doDebug(bool debug);
  void doPrint(bool print);  
  
  void useData(bool data);
  void useMMFakes(bool useMM);
  void setStyle(bool setAtlas);
  void setInPath(TString path);
  void setAtlasStylePath(TString path){AtlasStylePath = path;}
  void setOutPath(TString path);
  void setBkgList(std::vector<TString> list);
  void setMCTypes(std::map<TString, TString> &m);
  bool getFileList(TString path, int type);
  void getFiles(int type);
  void getVariationFiles();
  void sortByName(std::vector< TH1F* > &v, std::vector<TString> &n);
  void sortBySize(std::vector< TH1F* > &v, std::vector<TString> &n); 
  void sortBkg(std::vector<TFile*> Files, std::vector<TString> Types);
  void setLabel(TString l){PlotLabel = l;}
  void setRatioLine(TString comp){RatioLine = comp;}
  void setYRange(float min, float max){YMin=min; YMax=max;}
  void setYRangeRatio(float min, float max){ratioYMin = min; ratioYMax = max;}
  bool setColor(TH1F* h, TString name);
  void setLumi(float l){Lumi = l;}
  void doLogY(bool logy){LogY = logy;}
  void drawOverflow(bool of){OFBin = of;}
  void drawLegNumbers(bool d){legNo = d;}
  void drawAtlasLabel(bool lab){AtlasLabel = lab;}
  void setFigureFormat(const char* format){figType = format;}
  void setVariations(bool var, TString type=""){ Variations = var; VarBkgType = type;}
  void setBkgNorm(TString name, float value=1.);
  void setBkgError(TString name, float value);
  void setSignal(TString name, float norm=1);
  void printBinErrors(TH1F *h, bool print);
  void getChi2(TH1F *hBkg, TH1F *hDat);
  void applyBkgNorm(std::vector<TString> bkgNames, std::vector<TH1F*> hVec, std::vector<float> norm);
  void applyFlatError(std::vector<TString> bkgNames, std::vector<TH1F*> hVec, std::vector<float> error);
  void applyVariationError(std::vector<TH1F*> bkg, TString bkg_type, TH1F *upValues, TH1F *downValues);

  bool isFilled(TH2F* h, float x, float y);
  bool containsStr(std::vector<TString> v, TString s);

  void addOF(TH1F* h);
  void plot(TString name, int bins, float xMin, float xMax, TString title="");

  TString xLabel(TString name);
  TString mcType(TString filename);
  TString sigLabel(TString id);
  TString getLegendEntry(TH1F* h);

  TH1F* whiteCopy(TH1F* h);
  
  TGraphAsymmErrors* errorGraph(TH1F* h);
  std::vector< TGraphAsymmErrors* > ratioErrorGraph(TH1F *h1, TH1F *h2);
  
  TH1F* ratioPlot(TH1F* h1, TH1F *h2, float yMin, float yMax);
  TH1F* makeSum(std::vector<TH1F*> v, TString name, float xMin, float xMax, bool msg=true);
  THStack* makeStack(std::vector<TH1F*> v, TString name, float xMin, float xMax, bool msg=true);

  float getStatError(TH1F* h, bool msg=false);
  float getTotalError(TH1F* h);

  TH1F* getBkgComponent(TString name, std::vector<TH1F*> hVec);
  TH1F *getComponentRatio(TString name, std::vector<TH1F*> hVec, TH1F *total);
  TH1F *getBkgVarDiff(std::vector<TH1F*> var, std::vector<TH1F*> bkg, TString bkg_type);

  std::vector<float>   getIntegrals(std::vector<TH1F*> v);
  std::vector< TH1F* > getBkgHistos(TString name);
  std::vector< TH1F* > getBkgVariations(TString name, TString var);
  std::vector< std::vector< TFile* > > sortBkg(std::vector<TFile*> Files);
  std::vector< std::vector< TH1F* > > getHisto(TString name);
  std::vector< TString > tokenize(TString s);
  std::vector< TH1F* > getDataHistos(TString name);
  
 private:
  std::string CNAME;
  bool  Debug;
  bool  Print;
  bool  Style;
  bool  OpenFiles;
  bool  LogY;
  bool  OFBin;
  bool  legNo;
  bool  doData;
  bool  doMMFakes;
  bool  Variations;
  bool  AtlasLabel;

  float Lumi;
  float YMin;
  float YMax;
  float ratioYMin;
  float ratioYMax;
  float sigNorm;

  TString PlotLabel;
  TString InPath;
  TString OutPath;
  TString AtlasStylePath;
  TString RatioLine;
  TString VarBkgType;
  TString sigName;

  const char *figType;
  std::map<TString, TString> MCTypes;

  std::vector<TFile*> files;
  std::vector<TFile*> filesVar;
  std::vector<TString> fnames;
  std::vector<TString> fnamesVar;
  std::vector<TString> bkgList;
  std::vector<TString> bkgTypes;
  std::vector<TString> bkgTypesOriginal;
  std::vector<TString> bkgErrNames;
  std::vector<TString> bkgNormNames;
  std::vector<float>   bkgErrValues;
  std::vector<float>   bkgNormValues;
  std::vector< std::vector< TFile* > > sortedFiles;
};

