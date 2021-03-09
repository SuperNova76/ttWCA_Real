import os
import sys
import time
import ROOT
from glob import glob
from argparse import ArgumentParser

def main():

    parser = ArgumentParser(description="Script for TH1F creation from Ntuple input")
    parser.add_argument("--File",    help="Name of input file", default="")
    parser.add_argument("--InPath",  help="Name of input path", default="")
    parser.add_argument("--OutPath", help="Location of output file [default=HISTOS]", default="HISTOS")
    parser.add_argument("--NoMCFakes", type=int,   help="Remove truth-level fakes from MC",  default=1)
    parser.add_argument("--Lumi",      type=float, help="Luminosity in fb^-1",               default=1)
    parser.add_argument("--Debug",     type=int,   help="Debug messages for selector class", default=0)
    options = parser.parse_args()

    if ( not len(options.File) or not os.path.isfile(options.File) ) and not len(options.InPath): 
        error("No input file selected, aborting")

    if len(options.OutPath) and not os.path.isdir(options.OutPath):
        info("Directory {0} does not exist, creating".format(options.OutPath)) 
        os.mkdir(options.OutPath);

    if not "ROOTSYS" in os.environ:
        error("No ROOT setup found. Please setup ROOT first")

    info("Loading selector")
    if ROOT.gROOT.LoadMacro("Selector/ttWSelector.cxx++") == -1:
        error("Selector could not be loaded")

    if not options.Debug:
        ROOT.gROOT.ProcessLine("gErrorIgnoreLevel = kFatal")

    if len(options.File):
        readFile(options.File, options.OutPath, options.NoMCFakes, options.Lumi, options.Debug)
        return

    infiles = sorted(glob(options.InPath+"/*.root"))
    if not len(infiles):
        info("No .root files found in {0} -> looking in potential subdirectories".format(options.InPath)) 
        infiles = sorted(glob(options.InPath+"/*/*.root"))
    if not len(infiles): 
        error("No input .root files found, aborting")

    print; info("Infiles:"); print(infiles)
    for infile in infiles:
        print; info("Reading {0}".format(infile))
        readFile(infile, options.OutPath, options.NoMCFakes, options.Lumi, options.Debug)
        time.sleep(0.5)
    return

def readFile(filename, path, nofakes, lumi, debug, treename="nominal"):

    infile = ROOT.TFile(filename)
    if not infile.GetListOfKeys().Contains(treename):
        info("File {0} does not contain tree with name {1}. Skipping file".format(filename,treename)); return

    intree = infile.Get(treename)
    if not intree.InheritsFrom("TTree"):
        info("Object {0} in file {1} is no TTree type. Skipping file".format(treename,filename)); return

    info("--> Input tree({0}) has {1} entries".format(intree.GetName(),intree.GetEntries()))
 
    if isData(filename): 
        Selector = ROOT.ttWSelector("ttWSelector_DATA")
    else: 
        Selector = ROOT.ttWSelector("ttWSelector_MC")

    Selector.Init(intree)
    Selector.setDebug(debug)
    Selector.rmMCFakes(nofakes)
    Selector.Begin(intree)
    Selector.SlaveBegin(intree)
    Selector.setOutPathName(path)
    Selector.setOutFileName("hist_{0}".format(filename.split("/")[-1]))
    Selector.setLumiNorm(lumi)
    #Selector.setGenName(getGenName(filename))
    #Selector.setMCcamp(getMCcamp(filename))
    for i in range(intree.GetEntries()): Selector.Process(i)
  
    Selector.SlaveTerminate()
    Selector.Terminate()
    return

def isData(filename):
    filename = filename.split("/")[-1]
    if filename.lower().find("data")>-1 or filename.lower().find("allyear")>-1: return True
    return False

def getGenName(filename):
    filename = filename.split("/")[-1]
    if filename.find("Sherpa")>-1:   return "Sherpa"
    if filename.find("Sh22")>-1:     return "Sherpa"
    if filename.find("Ph")>-1:       return "Powheg"
    if filename.find("aMC")>-1:      return "MadGraph"
    if filename.find("MadGraph")>-1: return "MadGraph"
    if filename.find("Py")>-1:       return "Pythia"
    return ""

def getMCcamp(filename):
    if filename.lower().find("r9364") >-1 or filename.lower().find("mc16a")>-1: return "MC16a"
    if filename.lower().find("r10201")>-1 or filename.lower().find("mc16d")>-1: return "MC16d"
    if filename.lower().find("r10724")>-1 or filename.lower().find("mc16e")>-1: return "MC16e"
    return ""

def info(msg):  
    print("\033[1;34mINFO:\t{0}\033[0m".format(msg))

def error(msg):
    print("\033[1;31mERROR:\t{0}\033[0m".format(msg))
    sys.exit(1)

if __name__ == "__main__":
    main()


