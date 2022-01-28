import os,sys,time,ROOT
from glob import glob

debug    = False
plotHist = False
logY     = True

def main():
    filepath = sys.argv[1]
    hkey     = getCFhistName(filepath)

    print("Check: {0}".format(filepath))
    if not os.path.isdir(filepath):
        print("ERROR: {0} does not exist".format(filepath))
        return

    h = getCFhist(filepath,hkey)
    h.SetLineColor(ROOT.kBlue)
    h.SetLineWidth(2)
    printBins(h)

    c = ROOT.TCanvas("c","c")
    c.cd()

    if logY: c.SetLogy()
    c.SetBottomMargin(0.15)
    c.SetGridx()
    ROOT.gStyle.SetOptTitle(0)
    ROOT.gStyle.SetOptStat(000000)
  
    h.Draw("HIST")
    if plotHist:
        c.Print("{0}.eps".format(hkey.split("/")[0]))

    time.sleep(5)
    return

def getCFhist(path, histkey):
    hCF = []
    for fname in sorted(glob("{0}/*.root*".format(path))):
        f = ROOT.TFile(fname)
        h = f.Get(histkey)
        if h.InheritsFrom("TH1"): 
            hist = h.Clone("CF_{0}".format(fname))
            hist.SetDirectory(0)
            if debug: print("--> Found {0}/{1} (N(i)={2})".format(f.GetName(),h.GetName(),hist.GetBinContent(1)))
            hCF.append(hist)

    print("\t--> {0}: found {1} CF histograms".format(histkey,len(hCF)))

    hTemp = hCF[0].Clone("{0}_Sum".format(histkey))
    hTemp.Reset()
    hTemp.SetDirectory(0)
    
    for h in hCF:
        hTemp.Add(h) 
        if debug: print("Add CF histgramm with {0} initial entries".format(h.GetBinContent(1)))
    print
    return hTemp

def printBins(hist):
    for i in range(hist.GetNbinsX()):
        if hist.GetBinContent(i)<=0: continue 
        print("Bin {0:>2}{1:>40} \t :: {2} = {3}".format(i, hist.GetXaxis().GetBinLabel(i), hist.GetName(), hist.GetBinContent(i)))
    print
    return

def getCFhistName(filename):
    if filename.find("_mc16a_")>-1 or filename.find("_r9364")>-1:  return "eemu_2016/cutflow_Loose"
    if filename.find("_mc16d_")>-1 or filename.find("_r10201")>-1: return "eemu_2017/cutflow_Loose"
    if filename.find("_mc16e_")>-1 or filename.find("_r10724")>-1: return "eemu_2018/cutflow_Loose"
    return ""

if __name__ == "__main__": main()
