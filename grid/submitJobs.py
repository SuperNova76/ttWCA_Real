import os,sys,time
import ROOT
from glob import glob
from argparse import ArgumentParser

USERNAME   = os.getenv('RUCIO_ACCOUNT')
ACM_SOURCE = os.getenv('ACMSOURCEDIR')

ROOT_VERSION = "6.18/04"
CMT_CONFIG   = "x86_64-centos7-gcc8-opt"
FILESPERJOB  = 5

def main():
    parser = ArgumentParser(description="Script for submitting grid jobs from sample list")
    parser.add_argument("Samples",   help="List of input MC or data samples (.txt)")
    parser.add_argument("--Config",  help="Name of top-config file (path is resovled automatically)", default="Config_ttWCA.txt")
    parser.add_argument("--Version", help="Version of the production",                                default="uctatlas_v7")
    parser.add_argument("--Submit",  type=int, help="Submit jobs (or only print the prun-command)",   default=0)
    options = parser.parse_args()

    if not os.path.isfile(options.Samples): 
        error("List of samples not found, aborting")

    if not "{0}/{1}".format(os.getcwd(),ACM_SOURCE.split("/")[-1]) == ACM_SOURCE: 
        error("Incorrect directory. Please submit from {0}/../".format(ACM_SOURCE))

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
 
    cmd = "prun --excludeFile=build,run,{0}/analyses,{0}/scripts,{0}/tools,{0}/.git,{0}/ATNtupleProduction,{0}/ttWCA/.git --osMatching --useAthenaPackages --cmtConfig={1} --rootVer={2} --writeInputToTxt=IN:in.txt --outputs=output.root --exec=\"top-xaod {3} in.txt\" --nFilesPerJob={4} --nGBPerJob=MAX --inDS={5} --extFile={0}/ttWCA/share/* --mergeOutput --outDS=user.{6}.{7}.{8}/".format(ACM_SOURCE.split("/")[-1], CMT_CONFIG, ROOT_VERSION, findFile(ACM_SOURCE.split("/")[-1],config), FILESPERJOB, DS, USERNAME, makeOutputName(DS), version)

    print(cmd)
    if submit: os.system(cmd)
    print("")
    time.sleep(1)
    return 1

def makeOutputName(name):
    dsid     = name.split('.')[1]
    physname = name.split('.')[2]
    ptag     = name.split('_p')[-1]
    MCcamp    = getMCcamp(name)
    return "{0}.{1}_{2}_p{3}".format(dsid,physname,MCcamp,ptag)

def getMCcamp(n):
    if n.find('r9364')>1:  return "mc16a"
    if n.find('r10201')>1: return "mc16d"
    if n.find('r10724')>1: return "mc16e"
    info("MC campaign not found for sample {0}".format(n))
    return ""

def findFile(path,key):
    d = [] 
    for root, dirs, files in os.walk(path):
        for f in files: 
            if f.find(key)>-1: d.append("{0}/{1}".format(root,f))

    if len(d)==0: error("No files with key {0} found".format(key))
    if len(d)> 1: error("More than one file with key {0} found: {1}".format(key,d))
    return d[0]

def info(msg):
    print("\033[1mINFO:\t{0}\033[0m".format(msg));

def error(msg):
    print("\033[1;31mERROR:\t{0}\033[0m".format(msg))
    sys.exit(1)

if __name__ == "__main__":
    main()
