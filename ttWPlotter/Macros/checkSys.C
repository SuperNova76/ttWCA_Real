{
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  TString FileName   = "/lhome/ific/c/cardillo/ttW-CA/Ntuples/run/out_ttW.root";
  TString SampleName = "#it{t#bar{t}W}";
  
  TString VarName    = "el_pt";
  bool    vectorType = true;
  int     vectorPos  = 0;
  TString xAxisName  = "el-pt [GeV]";
  
  int bins = 8;
  
  float xMin = 0,    xMax = 200.;
  float rMin = 0.95, rMax = 1.05;

  bool print = false;
  bool inMeV = true;
  bool debug = true;

  const char* key_nom     = "nominal_Loose";
  const char* key_sysUp   = "EG_SCALE_ALL__1up_Loose";
  const char* key_sysDown = "EG_SCALE_ALL__1down_Loose";

  TFile *f = new TFile(FileName);
  std::cout << Form("Looking at file: %s",FileName.Data()) << std::endl;

  TTree *t_nom     = (TTree*)f->Get(key_nom);
  TTree *t_sysUp   = (TTree*)f->Get(key_sysUp);
  TTree *t_sysDown = (TTree*)f->Get(key_sysDown);

  std::cout << Form("\t TTree(nom)     = %s",t_nom->GetName()) << std::endl;
  std::cout << Form("\t TTree(sysUp)   = %s",t_sysUp->GetName()) << std::endl;
  std::cout << Form("\t TTree(sysDown) = %s",t_sysDown->GetName()) << std::endl;

  TH1F *h_nom     = new TH1F(VarName, VarName, bins, xMin, xMax);
  TH1F *h_sysUp   = new TH1F(VarName+"_up",   VarName+"_up",   bins, xMin, xMax);
  TH1F *h_sysDown = new TH1F(VarName+"_down", VarName+"_down", bins, xMin, xMax);

  float nom(0), sysUp(0), sysDown(0);
  std::vector<float> *nomVec(0), *sysUpVec(0), *sysDownVec(0);

  if(vectorType){
    t_nom->SetBranchAddress(VarName.Data(), &nomVec);
    t_sysUp->SetBranchAddress(VarName.Data(), &sysUpVec);
    t_sysDown->SetBranchAddress(VarName.Data(), &sysDownVec);
  }
  else{
    t_nom->SetBranchAddress(VarName.Data(), &nom);
    t_sysUp->SetBranchAddress(VarName.Data(), &sysUp);
    t_sysDown->SetBranchAddress(VarName.Data(), &sysDown);
  }

  ULong64_t evn_nom(0), evn_up(0), evn_down(0);
  t_nom->SetBranchAddress("eventNumber", &evn_nom);
  t_sysUp->SetBranchAddress("eventNumber", &evn_up);
  t_sysDown->SetBranchAddress("eventNumber", &evn_down);

  for (Long64_t entry(0); entry < t_nom->GetEntries(); entry++){
    float x(0), xUP(0), xDOWN(0);
    t_nom->GetEntry(entry);
    t_sysUp->GetEntry(entry);
    t_sysDown->GetEntry(entry);

    float sf = inMeV ? 1000. : 1.;
    if(vectorType){
      x     = nomVec->size()>vectorPos     ? nomVec->at(vectorPos)/sf     : 0;
      xUP   = sysUpVec->size()>vectorPos   ? sysUpVec->at(vectorPos)/sf   : 0;
      xDOWN = sysDownVec->size()>vectorPos ? sysDownVec->at(vectorPos)/sf : 0;
    }
    else{
      x     = nom/sf;
      xUP   = sysUp/sf;
      xDOWN = sysDown/sf;
    }

    if(debug) std::cout << Form("Entry %i \t evN(nom)=%i, evN(up)=%i, evN(down)=%i", (int)entry, (int)evn_nom, (int)evn_up, (int)evn_down) << std::endl;
    if(debug) std::cout << Form("Entry %i \t Nom=%.3f, Up=%.3f, Down=%.3f", (int)entry, x, xUP, xDOWN) << std::endl;
    h_nom->Fill(x);
    h_sysUp->Fill(xUP);
    h_sysDown->Fill(xDOWN);
  }

  bool logy = false;
  TCanvas *c = new TCanvas(Form("c_%s",VarName.Data()),Form("c_%s",VarName.Data()),1,10,770,560);
  TPad *pad1 = new TPad(Form("%s_p1",VarName.Data()), Form("%s_p1",VarName.Data()), 0., 0.30 , 1.00, 1.00, -1, 0, 0 );
  TPad *pad2 = new TPad(Form("%s_p2",VarName.Data()), Form("%s_p2",VarName.Data()), 0., 0.00,  1.00, 0.30, -1, 0, 0 );
  pad1->SetTicks(1,1);
  pad2->SetTicks(1,1);
  pad1->SetLogy(logy);
  pad1->SetBottomMargin( 0.01 );
  pad2->SetBottomMargin( 0.4 );
  pad1->SetLeftMargin( 0.10 );
  pad2->SetLeftMargin( 0.10 );
  pad1->SetRightMargin( c->GetRightMargin() );
  pad2->SetRightMargin( c->GetRightMargin() );
  pad1->SetTopMargin( c->GetTopMargin() );
  pad2->SetTopMargin( 0.02 );

  pad1->Draw();
  pad2->Draw();
  pad1->cd();

  float maxY(0);
  maxY = TMath::Max( (float)h_nom->GetMaximum(), TMath::Max( (float)h_sysDown->GetMaximum(), (float)h_sysUp->GetMaximum()));
  maxY *= logy ? 100 : 2.2;
  h_nom->GetYaxis()->SetRangeUser(0.01, maxY);

  h_nom->SetLineColor(kBlue-7);
  h_sysUp->SetLineColor(kRed-7);
  h_sysDown->SetLineColor(kGreen-3);
  
  h_nom->SetMarkerColor(h_nom->GetLineColor());
  h_sysUp->SetMarkerColor(h_sysUp->GetLineColor());
  h_sysDown->SetMarkerColor(h_sysDown->GetLineColor());

  h_nom->SetLineWidth(2);
  h_sysUp->SetLineWidth(2);
  h_sysDown->SetLineWidth(2);

  h_nom->GetYaxis()->SetTitleSize(0.05);
  h_nom->GetXaxis()->SetTitleSize(0.05);
  h_nom->GetYaxis()->SetLabelSize(0.05);
  h_nom->GetYaxis()->SetTitleOffset(1.);
  h_nom->GetXaxis()->SetTitleOffset(1.15);
  h_nom->GetYaxis()->SetLabelOffset(0.01);
  h_nom->GetXaxis()->SetLabelOffset(0.01);
  h_nom->GetYaxis()->SetTickLength(0.01);
  h_nom->GetXaxis()->SetTickLength(0.02);
  h_nom->GetYaxis()->SetTitle("Event yields");

  h_nom->Draw("HIST");
  h_sysUp->Draw("HIST SAME");
  h_sysDown->Draw("HIST SAME");

  TLegend *leg = new TLegend(0.32, 0.62, 0.73, 0.86);
  leg->AddEntry(h_nom,     Form("%s: %.1f",     "Nominal",    h_nom->Integral()));
  leg->AddEntry(h_sysUp,   Form("%s: %.1f %%",  key_sysUp,   (h_sysUp->Integral()/h_nom->Integral() - 1.)*100. ));
  leg->AddEntry(h_sysDown, Form("%s: %.1f %%",  key_sysDown, (h_sysDown->Integral()/h_nom->Integral() - 1.)*100. ));
  leg->SetBorderSize(0);
  leg->SetTextSize(0.05);
  leg->Draw("SAME");

  TLatex n;
  n.SetNDC();
  n.SetTextFont(62);
  n.SetTextSize(0.06);
  n.DrawLatex(0.15, 0.75, Form("Sample %s",SampleName.Data()));
  pad1->RedrawAxis();

  pad2->cd();
  pad2->SetGridx();

  TH1F *hRatioUp(0), *hRatioDown(0);
  hRatioUp = (TH1F*)h_nom->Clone(Form("RatioUp_%s",h_nom->GetName()));
  hRatioUp->Reset();

  hRatioDown = (TH1F*)h_nom->Clone(Form("RatioDown_%s",h_nom->GetName()));
  hRatioDown->Reset();

  hRatioUp->GetYaxis()->SetRangeUser(rMin, rMax);
  hRatioUp->GetYaxis()->SetTitle("Ratio");
  hRatioUp->GetYaxis()->SetNdivisions(8);
  hRatioUp->GetYaxis()->SetTitleSize(0.12);
  hRatioUp->GetYaxis()->SetTitleOffset(0.4);
  hRatioUp->GetYaxis()->SetLabelSize(0.12);
  hRatioUp->GetYaxis()->SetLabelOffset(0.011);
  hRatioUp->GetXaxis()->SetTitleSize(0.12);
  hRatioUp->GetXaxis()->SetTitleOffset(1.33);
  hRatioUp->GetXaxis()->SetLabelOffset(0.03);
  hRatioUp->GetXaxis()->SetLabelSize(0.12);
  hRatioUp->GetXaxis()->SetTitle(xAxisName);

  hRatioUp->SetLineColor(h_sysUp->GetLineColor());
  hRatioUp->SetLineStyle(h_sysUp->GetLineStyle());
  hRatioUp->SetMarkerColor(hRatioUp->GetLineColor());

  for(int i(1); i<=hRatioUp->GetNbinsX(); i++){
    float ratio = (h_sysUp->GetBinContent(i)>0 && h_nom->GetBinContent(i)>0) ? h_sysUp->GetBinContent(i)/h_nom->GetBinContent(i) : 0;
    hRatioUp->SetBinContent(i, ratio);
  }
  
  hRatioDown->SetLineColor(h_sysDown->GetLineColor());
  hRatioDown->SetLineStyle(h_sysDown->GetLineStyle());
  hRatioDown->SetMarkerColor(h_sysDown->GetLineColor());

  for(int i(1); i<=hRatioDown->GetNbinsX(); i++){
    float ratio = (h_sysDown->GetBinContent(i)>0 && h_nom->GetBinContent(i)>0) ? h_sysDown->GetBinContent(i)/h_nom->GetBinContent(i) : 0;
    hRatioDown->SetBinContent(i, ratio);
  }
  hRatioUp->Draw("HIST");

  TLine yLine;
  yLine.SetLineWidth(2);
  yLine.SetLineColor(h_nom->GetLineColor());
  yLine.DrawLine(hRatioUp->GetXaxis()->GetXmin(), 1.0, hRatioUp->GetXaxis()->GetXmax(), 1.0);
 
  hRatioUp->Draw("HIST SAME");
  hRatioDown->Draw("HIST SAME");
  gPad->RedrawAxis();

  if(print)
    c->Print(Form("%s_%s_%s.eps",VarName.Data(),key_sysUp,key_sysDown));

}
