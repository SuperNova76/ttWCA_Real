#include "ttWPlotter.h"

void ttWPlotter::doDebug(bool debug){
  Debug = debug;
  std::cout << CNAME << "::doDebug() \t" << Debug << std::endl;
  return;
}

void ttWPlotter::doPrint(bool print){
  Print = print;
  std::cout << CNAME << "::doPrint() \t" << Print << std::endl;
}

void ttWPlotter::setInPath(TString path){
  InPath = path;
  std::cout << CNAME << "::setInPath() \t" << InPath << std::endl;
  return;
}

void ttWPlotter::setOutPath(TString path){
  OutPath = path;
  std::cout << CNAME << "::setOutPath() \t" << OutPath << std::endl;
  return;
}

void ttWPlotter::setStyle(bool setAtlas){ 
  Style = true;
  std::cout << CNAME << "::setStyle() \t" << setAtlas << std::endl;
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(000000);
  if(!setAtlas) return;
  gROOT->LoadMacro(AtlasStylePath);
  gROOT->ProcessLine("SetAtlasStyle()");
  return;
}

void ttWPlotter::setBkgNorm(TString name, float value){
  bkgNormNames.push_back(name);
  bkgNormValues.push_back(value);
  std::cout << CNAME << "::setBkgNorm() \t" << Form("Set normalization for bkg %s \t %.2f",name.Data(),value) << std::endl;
  return;
}

void ttWPlotter::setBkgError(TString name, float value){
  bkgErrNames.push_back(name);
  bkgErrValues.push_back(value);
  std::cout << CNAME << "::setBkgError() \t" << Form("Set error for bkg %s \t %.1f%s",name.Data(),value*100,"%") << std::endl;
  return;
}

void ttWPlotter::useData(bool data){
  doData = data;
  std::cout << CNAME << "::useData() \t" << doData << std::endl;
  return;
}

void ttWPlotter::setSignal(TString name,float norm){
  sigName = name; sigNorm = norm;
  std::cout << CNAME << "::setSignal() \t" << Form("Signal process %s (scaled by %.1f)",sigName.Data(),sigNorm) << std::endl;
  return;
}

void ttWPlotter::addOF(TH1F* h){
  if(!h || !OFBin) return;
  int N = h->GetNbinsX();
  
  float UF = h->GetBinContent(0);
  float OF = h->GetBinContent(N+1);

  if(UF > 0.) h->SetBinContent(1, h->GetBinContent(1) + UF);
  if(OF > 0.) h->SetBinContent(N, h->GetBinContent(N) + OF); 

  h->ClearUnderflowAndOverflow();
  return;
}

void ttWPlotter::plot(TString name, int bins, float xMin, float xMax, TString title){
  if(!Style) this->setStyle(1);

  //Get files
  if(!OpenFiles){
    this->getFileList(InPath,1);
    if(doData)     this->getFileList(InPath,0); 
    if(Variations) this->getFileList(InPath,2);
    OpenFiles = true;
    std::cout << std::endl;
  }
  if(!OpenFiles){std::cout<< CNAME <<"\t ERROR No files selected" <<std::endl; abort();}
    
  std::cout << CNAME << "::plot() \t " << Form("Plotting %s", name.Data()) << std::endl;
  std::vector<TH1F*> hBkg    = this->getBkgHistos(name);
  std::vector<float> bkgComp = this->getIntegrals(hBkg);
  for(auto h : hBkg){ h->Rebin(bins); this->addOF(h); }

  TH1F *hBkg_Tot_Stat = this->makeSum(hBkg, name, xMin, xMax, false);
  if(bkgNormNames.size()) 
    this->applyBkgNorm(bkgNormNames, hBkg, bkgNormValues);
  if(bkgErrNames.size())
    this->applyFlatError(bkgErrNames, hBkg, bkgErrValues);

  if(Variations && filesVar.size()){
    std::vector<TH1F*> hBkgVarUp   = getBkgVariations(name, "__up");
    std::vector<TH1F*> hBkgVarDown = getBkgVariations(name, "__down");
    for(auto h : hBkgVarUp){   h->Rebin(bins); this->addOF(h); } 
    for(auto h : hBkgVarDown){ h->Rebin(bins); this->addOF(h); }

    TH1F* hBkgVarUpErr   = getBkgVarDiff(hBkgVarUp,   hBkg, VarBkgType);
    TH1F* hBkgVarDownErr = getBkgVarDiff(hBkgVarDown, hBkg, VarBkgType);
    this->applyVariationError(hBkg, VarBkgType, hBkgVarUpErr, hBkgVarDownErr);
  }

  TH1F    *hBkg_Tot    = this->makeSum(hBkg, name, xMin, xMax);
  THStack *hBkg_Stack  = this->makeStack(hBkg, name, xMin, xMax);

  TString xlab(this->xLabel(name)), ylab(Form("Events / %i GeV",(int)(hBkg_Tot->GetXaxis()->GetXmax()-hBkg_Tot->GetXaxis()->GetXmin())/hBkg_Tot->GetNbinsX()));
  if( name.Contains("Njets") || name.Contains("NJets") || name.Contains("Nlep") || name.Contains("NLep") || name.Contains("Eta") || name.Contains("Phi") || hBkg_Tot->GetXaxis()->IsVariableBinSize()) ylab = "Events";

  TCanvas *c = new TCanvas(name,name, 1, 10, 770, 560);
  TPad *pad1 = new TPad(name+"_p1", name+"_p1", 0., 0.30 , 1.00, 1.00, -1, 0, 0 );
  TPad *pad2 = new TPad(name+"_p2", name+"_p2", 0., 0.00,  1.00, 0.30, -1, 0, 0 );
  pad1->SetBottomMargin( 0.01 );
  pad2->SetBottomMargin( 0.4 );
  pad1->SetLeftMargin( 0.10 );
  pad2->SetLeftMargin( 0.10 );
  pad1->SetRightMargin( c->GetRightMargin() );
  pad2->SetRightMargin( c->GetRightMargin() );
  pad1->SetTopMargin( c->GetTopMargin() );
  pad2->SetTopMargin( 0.02 );

  pad2->Draw();
  if(LogY)pad1->SetLogy();
  pad1->Draw();
  
  float yMin(0.01), yMax(hBkg_Tot->GetMaximum()*2.5);
  if(LogY){ yMin=0.2; yMax=yMax*4000; }
  yMin = (YMin>=0) ? YMin : yMin;
  yMax = (YMax>=0) ? YMax : yMax;
  hBkg_Tot->GetYaxis()->SetRangeUser(yMin, yMax);

  hBkg_Tot->GetYaxis()->SetTitle(ylab);
  hBkg_Tot->GetXaxis()->SetTitle(xlab);  

  hBkg_Tot->GetXaxis()->SetTitleSize(0.03);
  hBkg_Tot->GetYaxis()->SetTitleSize(0.05);
  hBkg_Tot->GetYaxis()->SetTitleOffset(1.00);

  hBkg_Tot->GetYaxis()->SetTickLength(0.01);
  hBkg_Tot->GetXaxis()->SetTickLength(0.02);

  hBkg_Tot->GetXaxis()->SetLabelSize(0);
  hBkg_Tot->GetYaxis()->SetLabelSize(0.05);
  hBkg_Tot->GetXaxis()->SetLabelOffset(0.03);
  hBkg_Tot->GetYaxis()->SetLabelOffset(0.011);

  hBkg_Tot->SetLineWidth(0);
  hBkg_Tot->SetLineColor(kBlack);

  pad1->cd();
  hBkg_Tot->Draw("HIST");
  hBkg_Stack->Draw("HIST SAME");
  hBkg_Tot->Draw("HIST SAME");

  TGraphAsymmErrors *Bkg_Err = this->errorGraph(hBkg_Tot);
  Bkg_Err->Draw("E2 SAME");

  TH1F *hSig(0);
  if(sigName.Length()){
    hSig = (TH1F*)(this->getBkgComponent(sigName, hBkg))->Clone(Form("Sig_%s",sigName.Data()));
    hSig->Scale(sigNorm);
    hSig->SetLineColor( hSig->GetFillColor() );
    hSig->SetLineWidth(2);
    hSig->SetFillStyle(0);
    hSig->Draw("HIST SAME");
    if(Debug){std::cout << CNAME <<"::plot()\t " << Form("Draw %s with Integral %.3f", hSig->GetName(), hSig->Integral()) << std::endl;}
  }

  //Draw datas
  std::vector<TH1F*> DataList = this->getDataHistos(name);
  float NData(0);
  for(auto data : DataList){
    this->addOF(data);
    NData += data->Integral(-1,data->GetNbinsX()+1);
    data->Rebin(bins);
    data->GetXaxis()->SetRangeUser(xMin,xMax);
    data->SetMarkerStyle(20);
    data->SetMarkerSize(1.0);
    data->SetLineWidth(1);
    data->SetMarkerColor(kBlack);
    data->SetLineColor(kBlack);
    data->Draw("PEX0 SAME");
  }
  gPad->RedrawAxis();
  
  //Draw legend
  TLegend *leg = new TLegend(legNo ? 0.45 : 0.53, 0.57, 0.93, 0.91);
  leg->SetNColumns(2);
  leg->SetBorderSize(0);
  leg->SetTextFont(42);
  leg->SetTextSize(0.055);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetLineColor(0);
  if(legNo){
    if(!DataList.empty()) leg->AddEntry(DataList[0], Form(" Data (%i)",(int)NData), "pe");
    leg->AddEntry(Bkg_Err, Form(" SM total (%.1f)", hBkg_Tot->Integral(-1,hBkg_Tot->GetNbinsX()+1)), "f");
    for(unsigned int i(1); i<=hBkg.size(); i++){int b(hBkg.size()-i); leg->AddEntry(hBkg[b],Form(" %s (%.1f)",(this->getLegendEntry(hBkg[b])).Data(), bkgComp[b]),"f");}
  }
  else{
    if(!DataList.empty()) leg->AddEntry(DataList[0], " Data", "pe");
    leg->AddEntry(Bkg_Err, " SM total", "f");
    for(unsigned int i(1); i<=hBkg.size(); i++){int b(hBkg.size()-i); leg->AddEntry(hBkg[b],Form(" %s",(this->getLegendEntry(hBkg[b])).Data()), "f");}
  }
  leg->Draw("SAME");

  //Draw plot label
  TLatex n;
  n.SetNDC();
  n.SetTextColor(kBlack);;
  n.SetTextFont(42);
  n.SetTextSize(0.06);
  n.DrawLatex(0.15, AtlasLabel ? 0.75 : 0.84, Form("#sqrt{s} = 13 TeV,  %.0f fb^{-1}", Lumi));
  n.DrawLatex(0.15, AtlasLabel ? 0.66 : 0.75, Form("%s %s", PlotLabel.Data(), title.Data()));
  if(AtlasLabel){
    n.SetTextSize(0.07);
    n.SetTextFont(72); n.DrawLatex(0.15, 0.84, "ATLAS");
    n.SetTextFont(42); n.DrawLatex(0.28, 0.84, "Internal");
  }

  //Draw ratio
  pad2->cd();
  pad2->SetGridy();
  TH1F* hRatio(0);
  if(!DataList.empty()) hRatio = this->ratioPlot(DataList[0], hBkg_Tot, ratioYMin, ratioYMax);
  else hRatio = this->ratioPlot(hBkg_Tot, hBkg_Tot, ratioYMin, ratioYMax);
  hRatio->GetXaxis()->SetRangeUser(xMin,xMax);
 
  //Set data error
  for(int i(1); i<=hRatio->GetNbinsX()+1; i++) hRatio->SetBinError(i, 0.001);
  if(!DataList.empty()){ 
    for(int i(1); i<=hRatio->GetNbinsX()+1; i++) hRatio->SetBinError(i, TMath::Sqrt(1./DataList[0]->GetBinContent(i)) ); 
  }  
  hRatio->Draw("PEX0 AXIS");
  hRatio->Draw("PEX0 AXIG SAME");

  //Set MC error
  std::vector< TGraphAsymmErrors* > ratioGraphs = this->ratioErrorGraph(hBkg_Tot, hBkg_Tot_Stat);
  (ratioGraphs.back())->Draw("E2 SAME");
  (ratioGraphs.front())->Draw("E2 SAME");

  TH1F* compRatio = this->getComponentRatio(RatioLine, hBkg, hBkg_Tot);
  if(compRatio) compRatio->Draw("HIST SAME"); 
  if(!DataList.empty()) hRatio->Draw("PEX0 SAME");

  TLine line;
  line.SetLineColor(kBlack);
  line.SetLineStyle(1);
  line.SetLineWidth(1);
  line.DrawLine(xMin, 1., xMax, 1.);
  gPad->SetTicks();
  gPad->RedrawAxis();

  if(compRatio){
    pad1->cd();
    TLegend *rLeg = new TLegend(0.53, 0.49, 0.78, 0.56);
    rLeg->SetBorderSize(0);
    rLeg->SetTextFont(42);
    rLeg->SetTextSize(0.055);
    rLeg->SetFillColor(0);
    rLeg->SetFillStyle(0);
    rLeg->SetLineColor(0);
    rLeg->AddEntry(compRatio, Form("%s fraction",RatioLine.Data()), "l");
    rLeg->Draw("SAME");
  }

  if(sigName.Length()){
    pad1->cd();
    TLegend *sigLeg = new TLegend(0.53, 0.49, 0.78, 0.56);
    sigLeg->SetBorderSize(0);
    sigLeg->SetTextFont(42);
    sigLeg->SetTextSize(0.055);
    sigLeg->SetFillColor(0);
    sigLeg->SetFillStyle(0);
    sigLeg->SetLineColor(0);
    sigLeg->AddEntry(hSig, Form("%s #times %.0f",getLegendEntry(hSig).Data(),sigNorm), "l");
    sigLeg->Draw("SAME");
  }
  
  //Print plots
  if(Print){
    if(!(bool)OutPath.Length()) OutPath = ".";
    c->Print(Form("%s/%s.%s",OutPath.Data(),name.Data(),figType));
    std::cout << CNAME << "::Print() \t Created " << Form("%s/%s.%s",OutPath.Data(),name.Data(),figType) << std::endl;
  }
  std::cout << std::endl;
  return;
}

TH1F* ttWPlotter::ratioPlot(TH1F* h1, TH1F* h2, float yMin, float yMax){
  TH1F* h_ratio(0);
  if(!h1 || !h2) return h_ratio;

  h_ratio = (TH1F*)h1->Clone(Form("Ratio_%s_%s",h1->GetName(), h2->GetName()) );
  h_ratio->Reset();

  for(int bin(1); bin<=h_ratio->GetNbinsX()+1; bin++){
    float ratio = h2->GetBinContent(bin)>0 ? h1->GetBinContent(bin)/h2->GetBinContent(bin) : 0;
    h_ratio->SetBinContent(bin, ratio);
  }
  h_ratio->SetLineColor(kBlack);
  h_ratio->SetLineWidth(1);
  h_ratio->SetLineStyle(1);
  
  h_ratio->GetYaxis()->SetRangeUser(yMin,yMax);
  h_ratio->GetXaxis()->SetTitle(h2->GetXaxis()->GetTitle());
  h_ratio->GetYaxis()->SetNdivisions(5);
  h_ratio->GetXaxis()->SetTitleSize(0.13);
  h_ratio->GetXaxis()->SetTitleOffset(1.32);
  h_ratio->GetXaxis()->SetLabelSize(0.12);
  h_ratio->GetXaxis()->SetLabelOffset(0.04);
  h_ratio->GetYaxis()->SetTitle("Data / SM");
  h_ratio->GetYaxis()->SetTitleSize(0.12);
  h_ratio->GetYaxis()->SetTitleOffset(0.4);
  h_ratio->GetYaxis()->SetLabelSize(0.12);
  h_ratio->GetYaxis()->SetLabelOffset(0.015);
  h_ratio->GetYaxis()->SetTickLength(0.01);
  h_ratio->SetMarkerSize(1.1);  

  if(Debug){for(int i(1); i<=h_ratio->GetNbinsX()+1; i++) std::cout << CNAME <<"::ratioPlot()\t " << Form("Bin %i: DATA/MC Ratio = %.3f",i, h_ratio->GetBinContent(i)) << std::endl;} 

  return h_ratio;
}

TH1F* ttWPlotter::makeSum(std::vector< TH1F* > v, TString name, float xMin, float xMax, bool msg){
  TH1F *H(0);
  if(v.empty()){std::cout << CNAME << "\t ERROR No histograms with content to call makeSum()" << std::endl; abort();}
  float yields(0),errorSq(0);

  H = (TH1F*)(v.at(0))->Clone(Form("%s_BkgSum",name.Data())); 
  H->Reset();
  for(auto h : v){
    H->Add(h);
    yields  += h->Integral(-1,h->GetNbinsX()+1);
    errorSq += TMath::Power(this->getTotalError(h), 2);
  }
  if(msg){std::cout << CNAME << "::makeSum() \t " << Form("Returning %s \t Integral = %.4f \t +/- %.4f (stat | %i events) %.4f (total)", H->GetName(), yields, this->getStatError(H), (int)H->GetEntries(), TMath::Sqrt(errorSq)) << std::endl;}
  H->GetXaxis()->SetRangeUser(xMin,xMax);
  return H;
}

THStack* ttWPlotter::makeStack(std::vector< TH1F* > v, TString name, float xMin, float xMax, bool msg){
  THStack *S(0);
  if(v.empty()) return S;

  S = new THStack(name+"_BkgStack",name+"_BkgStack");
  for(auto h : v){
    if(msg){std::cout << CNAME << "::makeStack() \t" << Form("Adding %s \t Integral = %.4f \t +/- %.4f (stat), %.4f (total)", h->GetName(), h->Integral(-1,h->GetNbinsX()+1), this->getStatError(h), this->getTotalError(h)) << std::endl;}

    if(!this->setColor(h, h->GetName())){std::cout << CNAME << "\t ERROR No color selected" << std::endl; abort();}
    h->GetXaxis()->SetRangeUser(xMin,xMax);

    h->SetLineColor( h->GetFillColor() );
    h->SetLineWidth(0);
    S->Add(h);
  }
  return S;  
}

void ttWPlotter::printBinErrors(TH1F *h, bool print){
  if(!print) return;
  for(int i(1); i<=h->GetNbinsX(); i++) std::cout << CNAME << "printBinErrors::() \t" << Form("%s: bin(%i): %.3f +/- %.3f",h->GetName(),i,h->GetBinContent(i),h->GetBinError(i)) << std::endl; 
  return;
}

float ttWPlotter::getStatError(TH1F* h, bool msg){
  float err(0);
  if(!h) return err;
  err = h->GetEntries() ? TMath::Sqrt(h->GetEntries())/h->GetEntries() : 1.;
  if(msg){std::cout << CNAME << "::getStatError \t"<< Form("%s \t relative stat. error = %.1f %s", h->GetName(), err*100, "%") << std::endl;}
  err *= h->Integral(-1,h->GetNbinsX()+1);
  return err;
}

float  ttWPlotter::getTotalError(TH1F* h){
  double err(0);
  if(!h) return err;
  double tot = h->IntegralAndError(-1,h->GetNbinsX()+1, err);
  return (float)TMath::Min(err,tot);
}

std::vector<float> ttWPlotter::getIntegrals(std::vector< TH1F* > v){
  std::vector<float> integrals;
  if(v.empty()) return integrals;
  for(auto h : v) integrals.push_back( h->Integral(-1,h->GetNbinsX()+1) );
  return integrals;
}

TH1F* ttWPlotter::whiteCopy(TH1F* h){
  TH1F* hW(0);
  if(!h) return hW;
  hW = (TH1F*)h->Clone(Form("%s_White",h->GetName()));
  hW->SetLineStyle(1);
  hW->SetLineColor(kWhite);
  return hW;
}

void ttWPlotter::sortBySize(std::vector< TH1F* > &v, std::vector<TString> &n){
  std::vector<TH1F*> hOut; std::vector<TString> sOut;

  std::vector<double> integrals; std::vector<int> index;
  for(unsigned int i(0); i<v.size(); i++) integrals.push_back((v.at(i))->Integral(-1,(v.at(i))->GetNbinsX()+1)); 
  std::sort(integrals.begin(), integrals.end());

  for(unsigned int i(0); i<integrals.size(); i++){
    double integral1 = integrals.at(i);

    for(unsigned int j(0); j<v.size(); j++){     
      double integral2 = (v.at(j))->Integral(-1,(v.at(j))->GetNbinsX()+1);

      if( TMath::Abs( (integral1-integral2)/integral1 ) < 1E-5) index.push_back(j);
    }
  }
  for(unsigned int I(0); I<index.size(); I++){
    hOut.push_back( v.at(index[I]) );
    sOut.push_back( n.at(index[I]) );
  }
  if(Debug){for(unsigned int i (0); i<v.size(); i++) std::cout << CNAME <<"::sort()\t Unsorted: " << Form("%s,\t Integral = %.4f,\t BkgType = %s", (v.at(i))->GetName(), (v.at(i))->Integral(-1,(v.at(i))->GetNbinsX()+1), (n.at(i)).Data()) << std::endl; }
  v.clear(); n.clear();
  v = hOut;
  n = sOut;

  if(Debug){for(unsigned int i (0); i<v.size(); i++) std::cout << CNAME <<"::sort()\t Sorted:   " << Form("%s,\t Integral = %.4f,\t BkgType = %s", (v.at(i))->GetName(), (v.at(i))->Integral(-1,(v.at(i))->GetNbinsX()+1), (n.at(i)).Data()) << std::endl; }
  return;
}

std::vector< TH1F* > ttWPlotter::getBkgHistos(TString name){

  std::vector< TH1F* > hOut; 
  std::vector< std::vector< TH1F* > > sortedHistos = this->getHisto(name);
  bkgTypes = bkgTypesOriginal;

  for(unsigned int i(0); i<sortedHistos.size(); i++){
    TString type = bkgTypes.at(i);
    std::vector< TH1F* > hVec = sortedHistos.at(i);
    if(hVec.empty() || !type.Length()) continue; 
  
    TH1F *H = (TH1F*)(hVec.at(0))->Clone( Form("h_%s_%s", name.Data(), type.Data()) );
    H->Reset();
    for(auto h : hVec) H->Add(h);    
    
    hOut.push_back(H);
  }
  this->sortBySize(hOut, bkgTypes);
  return hOut;
}

std::vector< std::vector< TH1F* > > ttWPlotter::getHisto(TString name){
  std::vector< std::vector< TH1F* > > sortedHistos;
  if(!sortedFiles[0][0]->Get(name)){std::cout << CNAME << "\t ERROR Histogram " << name << " does not exist, aborting" << std::endl; return sortedHistos;}

  for(unsigned int i(0); i<sortedFiles.size(); i++){
    std::vector< TFile* > fVec = sortedFiles.at(i); 
    std::vector< TH1F* > hVec;
    for(auto f : fVec) hVec.push_back( (TH1F*)f->Get(name) );
    sortedHistos.push_back(hVec);
  }
  return sortedHistos;
}

bool ttWPlotter::getFileList(TString path, int type){
  if(!path.Length()) return true;
  //type = 0 Data, 1 MC, 2 MC(Var)
  if(Debug){
    switch( type ){
    case 0: std::cout << CNAME << "::getFileList() \t "<< "Getting Data Files" << std::endl;    break;
    case 1: std::cout << CNAME << "::getFileList() \t "<< "Getting MC Files" << std::endl;      break;
    case 2: std::cout << CNAME << "::getFileList() \t "<< "Getting MC Variations" << std::endl; break;
    default: break;
    }
  }
  gSystem->Exec("ls -1 "+path+"/hist_*.root | sort &> filelist.txt");
  std::ifstream File;
  File.open("filelist.txt");
  if( !File.is_open() ){std::cout << CNAME  << "\t ERROR File not found... aborting" << std::endl; return false;}

  while(true){
    if( File.eof() ) break;
    TString n;
    File >> n;
    bool isRootFile  =  n.Contains(".root");
    bool isDataFile  = (n.Contains("data") || n.Contains("Data"));
    bool isVariation = (n.Contains("__up") || n.Contains("__down"));
    switch( type ){
    case 0:
      if(isRootFile && !isVariation && isDataFile ) fnames.push_back(n);
      break;
    case 1:
      if(isRootFile && !isVariation && !isDataFile) fnames.push_back(n);
      break;
    case 2:
      if(isRootFile &&  isVariation && !isDataFile) fnamesVar.push_back(n);
      break;
    default: break;
    }
  }
  gSystem->Exec("rm filelist.txt");

  switch( type ){
  case 0: this->getFiles(type); break;
  case 1: this->getFiles(type); break;
  case 2: if(fnamesVar.size()) this->getVariationFiles(); break;
  default: break;
  }
  return true;
}

void ttWPlotter::getFiles(int type){

  for(unsigned int i(0); i<fnames.size(); i++){
    TFile *f = new TFile(fnames[i]);
    files.push_back(f);
  }
  if(type!=1) return;
  if(Debug){ for(auto f : files) std::cout << CNAME << "::getFiles() "<< Form("File \t %s", f->GetName()) << std::endl; } 
  
  sortedFiles = this->sortBkg(files);
  for(unsigned int i(0); i<sortedFiles.size(); i++){std::cout << CNAME << "::getFiles() \t "<<Form("Background type %s \t Files found %i",bkgTypes[i].Data(),(int)(sortedFiles[i]).size()) << std::endl;}
  if(Debug){
    for(unsigned int i(0); i<sortedFiles.size(); i++){
      std::cout << "\n Bkg type " << bkgTypes.at(i) << std::endl;
      for(unsigned int j(0); j<(sortedFiles[i]).size(); j++){ std::cout << "\t File " << (sortedFiles[i][j])->GetName() << std::endl; }
    }
  }
  return;
}

void ttWPlotter::getVariationFiles(){
  for(unsigned int i(0); i<fnamesVar.size(); i++){
    TFile *f = new TFile(fnamesVar[i]);
    filesVar.push_back(f);
  }
  if(Debug){ for(auto f : filesVar) std::cout << CNAME << "::getVarFiles() "<< Form("File \t %s", f->GetName()) << std::endl; }
}

std::vector< TH1F* > ttWPlotter::getDataHistos(TString name){
  std::vector<TH1F*> hOut;
  for(auto f : files){
    if(mcType(f->GetName())=="Data"){ 
      hOut.push_back( (TH1F*)f->Get(name) );
      if(Debug){ std::cout << CNAME << "::getFiles() Get DATA file \t" << f->GetName() << std::endl; } 
    }
  }
  for(auto h : hOut) std::cout << CNAME << "::getData() \t" << Form("Data events %.1f",h->Integral(-1,h->GetNbinsX()+1)) << std::endl;
  return hOut;
}

std::vector< TH1F* > ttWPlotter::getBkgVariations(TString name, TString var){
  std::vector<TH1F*> hOut;
  for(auto f : filesVar){
    if( !((TString)f->GetName()).Contains(var)) continue;
    TH1F *h = (TH1F*)f->Get(name);
    hOut.push_back(h);
    if(Debug){ std::cout << CNAME << "::getBkgVar() \t" << Form("Found (%s) histogram %s/%s",var.Data(),f->GetName(),h->GetName()) << std::endl; }
  }
  if(!hOut.size()) std::cout << CNAME << "::getBkgVar() \t" << Form("No variations for %s found",name.Data()) << std::endl; 
  return hOut;
}

TH1F* ttWPlotter::getBkgVarDiff(std::vector<TH1F*> var, std::vector<TH1F*> bkg, TString bkg_type){
  TH1F* hDiff(0);
  if(!var.size()) return hDiff;

  TH1F *nom(0);
  for(auto h : bkg){
    if( ((TString)h->GetName()).Contains(bkg_type) ){nom = h; break;}
  }
  hDiff = (TH1F*)nom->Clone(Form("Var_%s",nom->GetName()));
  hDiff->Reset();
  if(!var.size() || !bkg_type.Length()) return hDiff;

  for(int bin(1); bin<=nom->GetNbinsX()+1; bin++){ 
    float diff(0);
    for(auto hVar : var) diff += TMath::Power(hVar->GetBinContent(bin) - nom->GetBinContent(bin), 2);
    hDiff->SetBinContent(bin, TMath::Sqrt(diff));
  }
  return hDiff;
}

void ttWPlotter::applyVariationError(std::vector<TH1F*> bkg, TString bkg_type, TH1F *upValues, TH1F *downValues){
  TH1F *nom(0);
  for(auto h : bkg){
    if( ((TString)h->GetName()).Contains(bkg_type) ){nom = h; break;}
  }
  for(int bin(1); bin<=nom->GetNbinsX()+1; bin++){
    float err  = TMath::Power(nom->GetBinError(bin), 2);
    float diff = TMath::Power((upValues->GetBinContent(bin) + downValues->GetBinContent(bin))/2., 2);
    nom->SetBinError(bin, TMath::Sqrt(err+diff));
  }
  return;
}

std::vector< std::vector< TFile* > > ttWPlotter::sortBkg(std::vector<TFile*> Files){

  if(Files.empty()){ std::cout << CNAME << "\t ERROR No files or types selected" << std::endl; abort(); }
  bkgTypes.clear();
  this->setMCTypes(MCTypes);

  for (auto f : Files){
    TString type = this->mcType(f->GetName());
    if(type.Length() && !this->containsStr(bkgTypes,type)) bkgTypes.push_back(type);
  }
  bkgTypesOriginal = bkgTypes;
  std::vector< std::vector< TFile* > > sortedFiles((int)bkgTypes.size());

  for(unsigned int i(0); i<bkgTypes.size(); i++){
    for(unsigned int j(0); j<Files.size(); j++){
      TString type = this->mcType(Files[j]->GetName());
      if(type == bkgTypes[i]) (sortedFiles[i]).push_back(Files[j]);
    }
  }
  return sortedFiles;
}

TString ttWPlotter::mcType(TString filename){
  filename.ToLower();
  if(filename.Contains("data") || filename.Contains("Data")) return "Data";

  std::vector<TString> filename_vec = this->tokenize(filename);
  TString fname = filename_vec.back();
  TString id = fname(5,6);

  if(!id.IsDigit()){ std::cout << CNAME << "\t ERROR MCId " << id << " is not a number " << std::endl; abort(); }
  return MCTypes[id];
}

bool ttWPlotter::containsStr(std::vector<TString> v, TString s){

  for (unsigned int i(0); i<v.size(); i++){ if(v.at(i)==s) return true; }
  return false;
}

std::vector< TString > ttWPlotter::tokenize(TString s){
  std::vector<TString> v;
  if( !s.Length() ) return v;

  TObjArray * strings = s.Tokenize("/");
  for(int i(0); i<strings->GetEntries(); i++){
    TString part = ((TObjString*)strings->At(i))->GetString();
    part.ReplaceAll(" ","");
    v.push_back(part);
  }
  return v;
}

bool ttWPlotter::setColor(TH1F* h, TString name){
  if(!h) return true;

  if(name.Contains("_tWZ")){   h->SetLineColor(kBlack);  h->SetFillColor(kYellow-9); return true;}
  if(name.Contains("_VV")){ h->SetLineColor(kBlack);     h->SetFillColor(kAzure+8);  return true;}
  if(name.Contains("_ttX")){   h->SetLineColor(kBlack);  h->SetFillColor(kOrange+1); return true;}
  if(name.Contains("_ttZ")){ h->SetLineColor(kBlack);    h->SetFillColor(kAzure-3);  return true;}
  if(name.Contains("_tZ")){  h->SetLineColor(kBlack);    h->SetFillColor(kRed-5);    return true;}
  if(name.Contains("_Other")){  h->SetLineColor(kBlack); h->SetFillColor(kGreen-6);  return true;}
  if(name.Contains("_tHX")) {  h->SetLineColor(kBlack);  h->SetFillColor(kMagenta-7); return true;}
  if(name.Contains("_Fakes")){  h->SetLineColor(kBlack); h->SetFillColor(kRed+2);    return true;} 
  return true;
}

TString ttWPlotter::getLegendEntry(TH1F* h){
  if(!h) return "";
  TString hname = h->GetName();
  if(hname.Contains("_tWZ"))   return "tWZ";
  if(hname.Contains("_VV"))    return "WZ/ZZ+jets";
  if(hname.Contains("_ttZ"))   return "t#bar{t}Z";
  if(hname.Contains("_ttX"))   return "t#bar{t}+X(W/H)";
  if(hname.Contains("_tZ"))    return "tZq";
  if(hname.Contains("_tHX"))   return "tHq/tHW";
  if(hname.Contains("_Other")) return "Other";
  if(hname.Contains("_Fakes")) return "Fake leptons";
  return "";
}

TGraphAsymmErrors* ttWPlotter::errorGraph(TH1F *h){
  
  TGraphAsymmErrors *g = new TGraphAsymmErrors();
  for(int i(1); i<=h->GetNbinsX()+1; i++){
    g->SetPoint(i, h->GetBinCenter(i), h->GetBinContent(i));

    g->SetPointEXhigh(i, h->GetBinWidth(i)/2.);
    g->SetPointEXlow(i,  h->GetBinWidth(i)/2.);
    g->SetPointEYhigh(i, h->GetBinError(i)/2.);
    g->SetPointEYlow(i,  h->GetBinError(i)/2.);
  }
  g->SetLineWidth(0);
  g->SetLineColor(kGray+1);
  g->SetFillColor( g->GetLineColor() );
  g->SetFillStyle(3254);
  return g;
}

std::vector<TGraphAsymmErrors*> ttWPlotter::ratioErrorGraph(TH1F *h1, TH1F *h2){
  std::vector<TGraphAsymmErrors*> v(0);
  
  TGraphAsymmErrors *ge = new TGraphAsymmErrors();
  for(int i(1); i<=h1->GetNbinsX()+1; i++){
    ge->SetPoint(i, h1->GetBinCenter(i), 1.);

    ge->SetPointEXhigh(i, h1->GetBinWidth(i)/2.);
    ge->SetPointEXlow(i,  h1->GetBinWidth(i)/2.);

    float rErr = h1->GetBinContent(i)>0. ? h1->GetBinError(i)/h1->GetBinContent(i) : 0.;
    ge->SetPointEYhigh(i, rErr);
    ge->SetPointEYlow(i,  rErr);
  }
  ge->SetLineColor(kGray+1);
  ge->SetFillColor( ge->GetLineColor() );
  ge->SetFillStyle(3254);


  TGraphAsymmErrors *ge2 = new TGraphAsymmErrors();
  for(int i(1); i<=h2->GetNbinsX()+1; i++){
    ge2->SetPoint(i, h2->GetBinCenter(i), 1.);

    ge2->SetPointEXhigh(i, h2->GetBinWidth(i)/2.);
    ge2->SetPointEXlow(i,  h2->GetBinWidth(i)/2.);

    float rErr = h2->GetBinContent(i)>0. ? h2->GetBinError(i)/h2->GetBinContent(i) : 0.;

    float rErrSysUp  = TMath::Power(ge->GetErrorYhigh(i),2) - TMath::Power(rErr,2);
    float rErrSysLow = TMath::Power(ge->GetErrorYlow(i), 2) - TMath::Power(rErr,2);
    if(rErrSysUp<0.)  rErrSysUp  = 0.0001;
    if(rErrSysLow<0.) rErrSysLow = 0.0001;

    ge2->SetPointEYhigh(i, TMath::Sqrt(rErrSysUp)  );
    ge2->SetPointEYlow(i,  TMath::Sqrt(rErrSysLow) );
  }
  ge2->SetLineColor(kYellow-9);
  ge2->SetFillColor(kYellow-9);
  ge2->SetFillStyle(1111);

  v.push_back(ge);
  v.push_back(ge2);
  return v;
}

TH1F* ttWPlotter::getBkgComponent(TString name, std::vector<TH1F*> hVec){
  for(auto h : hVec){
    if( ((TString)h->GetName()).Contains(name) ) return h;
  }
  return nullptr;
}

TH1F* ttWPlotter::getComponentRatio(TString name, std::vector<TH1F*> hVec, TH1F *total){
  if(!name.Length()) return nullptr;

  TH1F *hComp = this->getBkgComponent(name, hVec);
  TH1F* hCompR = (TH1F*)total->Clone(Form("Ratio_%s", name.Data())); 
  hCompR->Reset();

  float tot(0), comp(0);
  for(int i(1); i<=total->GetNbinsX()+1; i++){
    tot  += total->GetBinContent(i);
    comp += hComp->GetBinContent(i);

    float ratio = (tot>0) ? comp/tot : 0;
    hCompR->SetBinContent(i,ratio);
  }
  hCompR->SetLineWidth(2);
  hCompR->SetLineStyle(9);
  hCompR->SetLineColor(hComp->GetLineColor());
  return hCompR;
}

void ttWPlotter::applyBkgNorm(std::vector<TString> bkgNames, std::vector<TH1F*> hVec, std::vector<float> norm){
  for(unsigned int i(0); i<bkgNames.size(); i++){
    float scale = norm.at(i);
    for(auto h : hVec){
      if( ((TString)h->GetName()).Contains(bkgNames.at(i)) ) h->Scale(scale);
    }
  }
  return;
}

void ttWPlotter::applyFlatError(std::vector<TString> bkgNames, std::vector<TH1F*> hVec, std::vector<float> error){
  
  for(unsigned int i(0); i<bkgNames.size(); i++){
    float err = error.at(i);
    for(auto h : hVec){
      if( !((TString)h->GetName()).Contains(bkgNames.at(i)) ) continue;

      for(int bin(1); bin<=h->GetNbinsX()+1; bin++){
	float newErr = TMath::Sqrt(TMath::Power(h->GetBinError(bin),2) + TMath::Power(h->GetBinContent(bin)*err,2));
	newErr = TMath::Min(newErr, (float)h->GetBinContent(bin));
	h->SetBinError(bin, newErr);
      }
    }
  }
  return;
}
  
TString ttWPlotter::xLabel(TString name){

  if(name.Contains("Meff"))    return "#it{m}_{eff} [GeV]";
  if(name.Contains("Mj"))      return "#it{m}_{j} [GeV]";
  if(name.Contains("EtMiss"))  return "#it{E}_{T}^{miss} [GeV]";
  if(name.Contains("Met"))     return "#it{E}_{T}^{miss} [GeV]";
  if(name.Contains("Ht"))      return "#it{H}_{T} [GeV]";
  if(name.Contains("LepPt1"))   return "#it{p}_{T}(lep_{1}) [GeV]";
  if(name.Contains("JetPt1"))   return "#it{p}_{T}(jet_{1}) [GeV]";
  if(name.Contains("LepPtNonZ"))return "#it{p}_{T}(lep^{non-#it{Z}}) [GeV]";
  if(name.Contains("TopPtNonZ"))return "#it{p}_{T}(#it{t}^{lep}) [GeV]";
  if(name.Contains("LepPhi1")) return "#phi (lep_{1})";
  if(name.Contains("LepEta1")) return "#eta (lep_{1})";
  if(name.Contains("JetPhi1")) return "#phi (jet_{1})"; 
  if(name.Contains("JetEta1")) return "#eta (jet_{1})"; 
  if(name.Contains("LepPt2"))  return "#it{p}_{T}(lep_{2}) [GeV]";
  if(name.Contains("JetPt2"))  return "#it{p}_{T}(jet_{2}) [GeV]";
  if(name.Contains("LepPhi2")) return "#phi (lep_{2})";
  if(name.Contains("LepEta2")) return "#eta (lep_{2})";
  if(name.Contains("JetPhi2")) return "#phi (jet_{2})";
  if(name.Contains("JetEta2")) return "#eta (jet_{2})";
  if(name.Contains("LepPt3"))  return "#it{p}_{T}(lep_{3}) [GeV]";
  if(name.Contains("JetPt3"))  return "#it{p}_{T}(jet_{3}) [GeV]";
  if(name.Contains("mT"))     return  "#it{m}_{T}^{ #it{W}} [GeV]";
  if(name.Contains("Njets"))  return "N_{jets} (#it{p}_{T} > 25 GeV)";
  if(name.Contains("Nbjets")) return "N_{b-jets} (85%)";
  if(name.Contains("Nlep"))   return "N_{lep} (#it{e/#mu})";
  if(name.Contains("ZPt"))    return "#it{p}_{T}^{ #it{Z}} [GeV]";
  if(name.Contains("Zm"))     return "#it{m}_{#it{ll}} [GeV]";
  if(name.Contains("ZRap"))   return "|y^{ #it{Z}}|";
  if(name.Contains("dPhiZtt"))return "#Delta#phi(#it{t#bar{t}},#it{Z})/#pi";
  if(name.Contains("dPhill")) return "#Delta#phi(#it{l}^{+},#it{l}^{-})/#pi";
  if(name.Contains("dPhiZt")) return "#Delta#phi(#it{Z},#it{t}^{lep})";
  if(name.Contains("dRapZt")) return "#Deltay(#it{Z},#it{t}^{lep})";
  if(name.Contains("dPhitt")) return "#Delta#phi(#it{l_{t}}^{+},#it{l_{t}}^{-})/#pi";
  if(name.Contains("ttPt"))   return "p_{T}^{ #it{t#bar{t}}} [GeV]";
  return "";
}

void ttWPlotter::setMCTypes(std::map<TString, TString> &m){

  m["364250"] = "VV"; //ZZ->llll
  m["364253"] = "VV";  //WZ->lllv
  
  m["364283"] = "VV"; //ZZ->llll (EW)
  m["364284"] = "VV"; //WZ->lllv (EW)

  m["364288"] = "VV"; //ZZ->llll (low pt)
  m["364289"] = "VV"; //WZ->lllv (low pt)

  m["410155"] = "ttX"; //ttW

  m["346443"] = "ttX"; //ttH(1l)
  m["346444"] = "ttX"; //ttH(2l)
  m["346445"] = "ttX"; //ttH(0l)

  m["410218"] = "ttZ"; //Zee
  m["410219"] = "ttZ"; //Zmumu
  m["410220"] = "ttZ"; //Ztautau
  
  m["412118"] = "tWZ"; 
  m["412063"] = "tZ";
 
  m["346799"] = "tHX";
  m["346678"] = "tHX";
  
  m["364242"] = "Other"; //VVV
  m["364243"] = "Other";
  m["364244"] = "Other";
  m["364245"] = "Other";
  m["364246"] = "Other";
  m["364247"] = "Other";
  m["364248"] = "Other";
  m["364249"] = "Other";

  m["304014"] = "Other"; //3-top  
  m["412043"] = "Other"; //4-top

  m["342282"] = "Other"; //gg->H
  m["342283"] = "Other"; //VBF->H
  m["342284"] = "Other"; //WH
  m["342285"] = "Other"; //ZH

  bool DDFakes = true;
  if(DDFakes){
    m["999999"] = "Fakes";
  }
  else{
    m["410472"] = "Fakes"; //ttbar

    m["410646"] = "Fakes"; //tW (t)
    m["410647"] = "Fakes"; //tW (tbar) 

    m["410156"] = "Fakes"; //ttZ, Z->vv
    m["410157"] = "Fakes"; //ttZ, Z->qq
  
    m["364100"] = "Fakes"; //Z+jets
    m["364101"] = "Fakes";
    m["364102"] = "Fakes";
    m["364103"] = "Fakes";
    m["364104"] = "Fakes";
    m["364105"] = "Fakes";
    m["364106"] = "Fakes";
    m["364107"] = "Fakes";
    m["364108"] = "Fakes";
    m["364109"] = "Fakes";
    m["364110"] = "Fakes";
    m["364100"] = "Fakes";
    m["364111"] = "Fakes";
    m["364112"] = "Fakes";
    m["364113"] = "Fakes";
    m["364114"] = "Fakes";
    m["364115"] = "Fakes";
    m["364116"] = "Fakes";
    m["364117"] = "Fakes";
    m["364118"] = "Fakes";
    m["364119"] = "Fakes";
    m["364120"] = "Fakes";
    m["364121"] = "Fakes";
    m["364122"] = "Fakes";
    m["364123"] = "Fakes";
    m["364124"] = "Fakes";
    m["364125"] = "Fakes";
    m["364126"] = "Fakes";
    m["364127"] = "Fakes";
    m["364128"] = "Fakes";
    m["364129"] = "Fakes";
    m["364130"] = "Fakes";
  }

  return;
}

