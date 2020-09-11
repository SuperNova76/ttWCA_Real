import os,sys
import commands,time
import ROOT
from glob import glob
from argparse import ArgumentParser


USERNAME = os.getenv('RUCIO_ACCOUNT')

ROOT_VERSION = "6.18/04"
CMT_CONFIG   = "x86_64-centos7-gcc8-opt"

def main():
    parser = ArgumentParser(description="Script fpr submitting grid jobs from sample list")
    parser.add_argument("Samples",   help="List of input samples (.txt)")
    parser.add_argument("--Config",  help="Name of top-config file ",  default="uctanalysistop/ttWCA/share/Config_ttWCA.txt")
    parser.add_argument("--Version", help="Version of the production", default="v5")
    parser.add_argument("--Submit",  type=int, help="Submit jobs",     default=0)
    options = parser.parse_args()

    if not os.path.isfile(options.Samples): 
        error("List of samples not found, aborting")

    NJobs = 0
    for sample in open(options.Samples): 
        NJobs = NJobs + makeJob(sample,options.Config,options.Version,options.Submit)

    info("Processed {0} jobs".format(NJobs))
    info("Done")
    return


def makeJob(DS,config,version,submit):
    if not len(DS) or DS.isspace() or  DS[0]=='#': return 0
    DS = DS.split()[0].split(':')[-1]
    info("Sample: {0}".format(DS))
 
    cmd = "prun --excludeFile=build,run,uctanalysistop/analyses,uctanalysistop/scripts,/uctanalysistop/tools,uctanalysistop/.git,uctanalysistop/ttWCA/.git --osMatching --useAthenaPackages --cmtConfig={0} --rootVer={1} --writeInputToTxt=IN:in.txt --outputs=output.root --exec=\"top-xaod {2} in.txt\" --maxNFilesPerJob=5 --nGBPerJob=MAX --inDS={3} --extFile=uctanalysistop/ttWCA/share/*.txt,uctanalysistop/ttWCA/share/*.root --mergeOutput --outDS=user.{4}.{5}_{6}/".format(CMT_CONFIG, ROOT_VERSION, config, DS, USERNAME, makeOutputName(DS), version)

    print(cmd)
    if submit: os.system(cmd)
    return 1

def makeOutputName(name):
    dsid = name.split('.')[1]
    MCcamp = getMCcamp(name)
    return "uctatlas_{0}_{1}".format(dsid,MCcamp)

def getMCcamp(n):
    if n.find('r9364')>1:  return "mc16a"
    if n.find('r10201')>1: return "mc16d"
    if n.find('r10724')>1: return "mc16e"
    info("MC campaign not found for sample {0}".format(n))
    return ""

def info(msg):
    print("\033[1mINFO:\t{0}\033[0m".format(msg));

def error(msg):
    print("\033[1;31mERROR:\t{0}\033[0m".format(msg))
    sys.exit(1)

if __name__ == "__main__":
    main()



