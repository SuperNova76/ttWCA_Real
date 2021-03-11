{
  bool logy  = false;
  bool print = false;

  float sf = 1.;

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  TString inpath  = "./";
  TString outpath = "/lhome/ific/c/cardillo/ttW-CA/Plots/Plots-10-03-21/comp/";

  TString file1(""),file2(""),file3("");
  file1 = "HISTOS/hist_fakes.root";
  file2 = "HISTOS_MC/hist_tt.root";
  file3 = "";

  TString label1 = " Fakes (MM)";
  TString label2 = " Fakes (#it{t#bar{t}})";
  TString label3 = " ";

  TFile *f1 = file1.Length() ? new TFile(inpath+file1) : 0;
  TFile *f2 = file2.Length() ? new TFile(inpath+file2) : 0;
  TFile *f3 = file3.Length() ? new TFile(inpath+file3) : 0;

  if(!f1 || !f2){ 
    std::cout << Form("ERROR \t No files in %s found", inpath.Data()) << std::endl; exit(1);
  }
  std::cout << Form("File1: %s\nFile2: %s",f1->GetName(),f2->GetName()) << std::endl; 
  if(f3) std::cout << Form("File3: %s",f3->GetName()) << std::endl;

  std::vector<TString> histnames = {"Njets_SR1b_low",  "LepPt2_SR1b_low",  "LepPt3_SR1b_low",  "Ht_SR1b_low",
				    "Njets_SR2b_low",  "LepPt2_SR2b_low",  "LepPt3_SR2b_low",  "Ht_SR2b_low",
				    "Njets_SR1b_high", "LepPt2_SR1b_high", "LepPt3_SR1b_high", "Ht_SR1b_high",
                                    "Njets_SR2b_high", "LepPt2_SR2b_high", "LepPt3_SR2b_high", "Ht_SR2b_high",
  };

  for(auto histname : histnames){

    TH1F* h1 = f1 ? (TH1F*)f1->Get(histname) : 0;
    TH1F* h2 = f2 ? (TH1F*)f2->Get(histname) : 0;
    TH1F* h3 = f3 ? (TH1F*)f3->Get(histname) : 0;

    if(!h1 || !h2){ 
      std::cout << Form("ERROR \t No histogram with the name %s found",histname.Data()) << std::endl; exit(1);
    }
    std::cout << Form("\n--> Plotting %s",histname.Data()) << std::endl;

    if(h2) h2->Scale(sf);
    if(h3) h3->Scale(sf);

    int entries1 = h1 ? h1->GetEntries() : 0;
    int entries2 = h2 ? h2->GetEntries() : 0;
    int entries3 = h3 ? h3->GetEntries() : 0;

    float ev1 = h1 ? h1->Integral(-1,h1->GetNbinsX()+1) : 0;
    float ev2 = h2 ? h2->Integral(-1,h2->GetNbinsX()+1) : 0;
    float ev3 = h3 ? h3->Integral(-1,h3->GetNbinsX()+1) : 0;

    float err1 = entries1 ? ev1/TMath::Sqrt(entries1) : 0;
    float err2 = entries2 ? ev2/TMath::Sqrt(entries2) : 0;
    float err3 = entries3 ? ev3/TMath::Sqrt(entries3) : 0;

    std::cout << Form("Entries  (%s) \t h(1)=%i, h(2)=%i",histname.Data(), entries1, entries2) << std::endl;
    std::cout << Form("Integral (%s) \t h(1)=%.3f +/- %.3f, h(2)=%.3f +/- %.3f \t Ratio=%.3f (%.1f%s)", histname.Data(), ev1, err1, ev2, err2, ev2/ev1, (ev2-ev1)/ev1*100, "%") << std::endl;
    if(entries3){
      std::cout << Form("Entries  (%s) \t h(1)=%i, h(3)=%i",histname.Data(), entries1, entries3) << std::endl;
      std::cout << Form("Integral (%s) \t h(1)=%.3f +/- %.3f, h(3)=%.3f +/- %.3f \t Ratio=%.3f (%.1f%s)", histname.Data(), ev1, err1, ev3, err3, ev3/ev1, (ev3-ev1)/ev1*100, "%") << std::endl;
    }

    TCanvas *c = new TCanvas(Form("c_%s",histname.Data()),Form("c_%s",histname.Data()),1,10,770,560);
    TPad *pad1 = new TPad(Form("%s_p1",histname.Data()), Form("%s_p1",histname.Data()), 0., 0.30 , 1.00, 1.00, -1, 0, 0 );
    TPad *pad2 = new TPad(Form("%s_p2",histname.Data()), Form("%s_p2",histname.Data()), 0., 0.00,  1.00, 0.30, -1, 0, 0 );
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
    maxY = TMath::Max((float)h1->GetMaximum(), (float)h2->GetMaximum());
    if(h3) maxY = TMath::Max((float)h3->GetMaximum(), maxY);

    maxY *= logy ? 100 : 2.5;
    h1->GetYaxis()->SetRangeUser(0.2, maxY);

    h1->SetLineColor(kBlue-7);
    h2->SetLineColor(kRed-7);
    if(h3) h3->SetLineColor(kGreen-3);

    h1->SetMarkerColor(h1->GetLineColor());
    h2->SetMarkerColor(h2->GetLineColor());
    if(h3) h3->SetMarkerColor(h3->GetLineColor());

    h1->SetLineWidth(2);
    h2->SetLineWidth(2);
    if(h3) h3->SetLineWidth(2);

    h1->GetYaxis()->SetTitleSize(0.05);
    h1->GetXaxis()->SetTitleSize(0.05);
    h1->GetYaxis()->SetLabelSize(0.05);
    h1->GetYaxis()->SetTitleOffset(1.);
    h1->GetXaxis()->SetTitleOffset(1.15);
    h1->GetYaxis()->SetLabelOffset(0.01);
    h1->GetXaxis()->SetLabelOffset(0.01);
    h1->GetYaxis()->SetTitle("Event yields");

    TString xAxisName(""), SRlabel("");
    if(histname.Contains("LepPt1"))   xAxisName = "#it{p}_{T}(lep_{1}) [GeV]";
    if(histname.Contains("LepPt2"))   xAxisName = "#it{p}_{T}(lep_{2}) [GeV]";
    if(histname.Contains("LepPt3"))   xAxisName = "#it{p}_{T}(lep_{3}) [GeV]";
    if(histname.Contains("JetPt1"))   xAxisName = "#it{p}_{T}(jet_{1}) [GeV]";
    if(histname.Contains("JetPt2"))   xAxisName = "#it{p}_{T}(jet_{2}) [GeV]";
    if(histname.Contains("LepEta1"))  xAxisName = "#it{#eta}(lep_{1})";
    if(histname.Contains("JetEta1"))  xAxisName = "#it{#eta}(jet_{1})";
    if(histname.Contains("Ht"))       xAxisName = "#it{H}_{T} [GeV]";
    if(histname.Contains("EtMiss"))   xAxisName = "#it{E}_{T}^{miss} [GeV]";
    if(histname.Contains("ZPt"))      xAxisName = "#it{p}_{T}(#it{Z}) [GeV]";
    if(histname.Contains("Njets"))    xAxisName = "#it{N}_{jets}(#it{p}_{T} > 25 GeV)";
    if(histname.Contains("Nbjets"))   xAxisName = "#it{N}_{#it{b}-jets}(#it{p}_{T} > 25 GeV)";
    if(histname.Contains("Deta"))     xAxisName = "#Delta#it{#eta}_{BDT}";
    h1->GetXaxis()->SetTitle(xAxisName);

    if(histname.Contains("_SR1b_low"))  SRlabel = "SR: 3#it{l}, 1#it{b}, 2-3#it{j}";
    if(histname.Contains("_SR2b_low"))  SRlabel = "SR: 3#it{l}, #geq2#it{b}, 2-3#it{j}";
    if(histname.Contains("_SR1b_high")) SRlabel = "SR: 3#it{l}, 1#it{b}, #geq4#it{j}";
    if(histname.Contains("_SR2b_high")) SRlabel = "SR: 3#it{l}, #geq2#it{b}, #geq4#it{j}";
    if(histname.Contains("_CRttZ"))     SRlabel = "CR: #it{t#bar{t}Z}";
   

    TH1F* hErr1 = (TH1F*)h1->Clone(Form("Err_%s",h1->GetName()));
    hErr1->SetFillColor(kBlue-10);
    hErr1->SetFillStyle(1000);
    hErr1->Draw("E2");
    h1->Draw("HIST SAME");

    TH1F* hErr2 = (TH1F*)h2->Clone(Form("Err_%s",h2->GetName()));
    hErr2->SetFillColor(kRed-10);
    hErr2->SetFillStyle(1000);
    hErr2->Draw("E2 SAME");
    h2->Draw("HIST SAME");

    if(h3){
      TH1F* hErr3 = (TH1F*)h3->Clone(Form("Err_%s",h3->GetName()));
      hErr3->SetFillColor(kGreen-10);
      hErr3->SetFillStyle(1000);
      hErr3->Draw("E2 SAME");
      h3->Draw("HIST SAME");
    }  
    hErr1->Draw("E2 SAME");
    h1->Draw("HIST SAME");

    TLegend *leg = new TLegend(0.58, h3 ? 0.53 : 0.55, 0.78, h3 ? 0.73 : 0.71);
    leg->AddEntry(h1, Form("%s: %.1f",label1.Data(), ev1));
    leg->AddEntry(h2, Form("%s: %.1f",label2.Data(), ev2));
    if(h3) leg->AddEntry(h3, Form("%s: %.1f",label3.Data(), ev3));
    leg->SetBorderSize(0);
    leg->SetTextSize(0.05);
    leg->Draw("SAME");

    TLatex n;
    n.SetNDC();
    n.SetTextFont(72);
    n.SetTextSize(0.06);
    n.DrawLatex(0.17, 0.76, "ATLAS");
    n.SetTextFont(42);
    n.DrawLatex(0.28, 0.76, "Internal");
    n.DrawLatex(0.17, 0.66, "#sqrt{s} = 13 TeV, 139 fb^{-1}");

    n.DrawLatex(0.60, 0.45, "3#it{l} non-prompt/fakes");
    n.DrawLatex(0.57, 0.77, SRlabel);
    gPad->RedrawAxis();

    pad2->cd();
    pad2->SetGridx();
  
    TH1F *hRatio12(0), *hRatio13(0), *hRatioErr(0);
    hRatio12 = (TH1F*)h1->Clone(Form("Ratio12_%s",h1->GetName()));
    hRatio12->Reset();
    hRatio12->GetYaxis()->SetRangeUser(0.52, 1.48);
    hRatio12->GetYaxis()->SetTitle("Ratio");
    hRatio12->GetYaxis()->SetNdivisions(8);
    hRatio12->GetYaxis()->SetTitleSize(0.12);
    hRatio12->GetYaxis()->SetTitleOffset(0.4);
    hRatio12->GetYaxis()->SetLabelSize(0.12);
    hRatio12->GetYaxis()->SetLabelOffset(0.011);
    hRatio12->GetXaxis()->SetTitleSize(0.12);
    hRatio12->GetXaxis()->SetTitleOffset(1.30);
    hRatio12->GetXaxis()->SetLabelOffset(0.03);
    hRatio12->GetXaxis()->SetLabelSize(0.12);

    hRatio12->SetLineColor(h2->GetLineColor());
    hRatio12->SetMarkerColor(hRatio12->GetLineColor());

    hRatioErr = (TH1F*)hRatio12->Clone(Form("RatioErr_%s",h1->GetName()));
    hRatioErr->Reset();
    hRatioErr->SetFillStyle(1000);
    hRatioErr->SetFillColor(kBlue-10);

    for(int i(1); i<=hRatio12->GetNbinsX(); i++){
      float ratio = (h2->GetBinContent(i)>0 && h1->GetBinContent(i)>0) ? h2->GetBinContent(i)/h1->GetBinContent(i) : 0;    
   
      float error1 = h1->GetBinContent(i)>0 ? h1->GetBinError(i)/h1->GetBinContent(i) : 0;
      float error2 = h2->GetBinContent(i)>0 ? h2->GetBinError(i)/h2->GetBinContent(i) : 0;
    
      hRatio12->SetBinContent(i, ratio);
      hRatio12->SetBinError(i, error2);

      hRatioErr->SetBinContent(i, 1.);
      hRatioErr->SetBinError(i, error1);
    }

    if(h3){
      hRatio13 = (TH1F*)h1->Clone(Form("Ratio13_%s",h1->GetName()));
      hRatio13->Reset();

      hRatio13->SetLineColor(h3->GetLineColor());
      hRatio13->SetMarkerColor(hRatio13->GetLineColor());

      for(int i(1); i<=hRatio13->GetNbinsX(); i++){
	float ratio = (h3->GetBinContent(i)>0 && h1->GetBinContent(i)>0) ? h3->GetBinContent(i)/h1->GetBinContent(i) : 0;
      
	float error3 = h3->GetBinContent(i)>0 ? h3->GetBinError(i)/h3->GetBinContent(i) : 0;

	hRatio13->SetBinContent(i, ratio);
	hRatio13->SetBinError(i, error3);
      }
    }
    hRatio12->Draw("P0 AXIS");
    hRatioErr->Draw("E2 SAME");
    hRatio12->Draw("P0 AXIG SAME");

    TLine yLine;
    yLine.SetLineWidth(2);
    yLine.SetLineColor(kGray+1);
    yLine.SetLineStyle(9);
    yLine.DrawLine(hRatio12->GetXaxis()->GetXmin(), 1.0, hRatio12->GetXaxis()->GetXmax(), 1.0);
    hRatio12->Draw("P0 SAME");
    if(hRatio13) hRatio13->Draw("P0 SAME"); 
    gPad->RedrawAxis();

    if(print)
      c->Print(Form("%s/%s.eps",outpath.Data(),c->GetName()));
  }
}
