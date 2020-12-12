import os,sys,time
from glob import glob
from argparse import ArgumentParser

def main():
    parser = ArgumentParser(description="Script for merging MC campaigns and 15-18 data samples")
    parser.add_argument("InDir",      help="Directory with input files",   default = "")
    parser.add_argument("--Type",     help="File type [default .root]",    default = ".root")
    parser.add_argument("--Label",    help="Label for merged files",       default = "mc16_merged")
    parser.add_argument("--DataName", help="Name for merged data file",    default = "dataAll_13TeV.AllYear.root")
    parser.add_argument("--Execute", type=int, help="Executing commands",  default = 1)
    options = parser.parse_args()

    directory = options.InDir
    if not len(directory): error("Please specify a directory")

    if not os.path.exists(directory): 
        error("Directory {0} does not exist".format(directory))

    if not "ROOTSYS" in os.environ:
        error("No ROOT setup found. Please setup ROOT first")
    else:
        info("Using ROOT version {0}/{1}".format(os.getenv('ROOTSYS').split("/")[-2],os.getenv('ROOTSYS').split("/")[-1]))

    if len(glob(directory+"/*"+options.Label+"*"+options.Type)):
        info("Removing existing files")
        os.system("rm -rf {0}/*{1}*{2}".format(directory,options.Label,options.Type))
        time.sleep(1)

    files = sorted(glob(directory+"/*"+options.Type))
    if not len(files):
        error("No files of type {0} found in {1} Aborting".format(options.Type,directory))

    mc16a = getSamples(files, "mc16a")
    mc16d = getSamples(files, "mc16d")
    mc16e = getSamples(files, "mc16e")
    data  = getSamples(files, "data")

    if not (len(mc16a) == len(mc16d) == len(mc16e)): error("Inconsistent sizes of mc16a/d/e sample lists")
    sampleSize = len(mc16a)

    print("{0} {1}".format("\nMC16a Files:", mc16a))
    print("{0} {1}".format("\nMC16d Files:", mc16d))
    print("{0} {1}".format("\nMC16e Files:", mc16e))
    print("")

    for i in range(sampleSize):
        if not (getMCId(mc16a[i]) == getMCId(mc16d[i]) == getMCId(mc16e[i])): error("Inconsitent MCIds of samples")
        info("MCID {0}:".format(getMCId(mc16a[i])))
        
        commandAdd = "hadd {0} {1} {2} {3}".format(getOutName(mc16a[i],options.Label),mc16a[i],mc16d[i],mc16e[i])
        commandRm  = "rm {0} {1} {2}".format(mc16a[i],mc16d[i],mc16e[i])
        print(commandAdd)
        if options.Execute: 
            os.system(commandAdd)
            time.sleep(1)
            os.system(commandRm)

    if options.Execute:
        info("Created {0} merged MC files".format(sampleSize))

    if not len(data): return
    dataMerged = "{0}/{1}".format(directory,options.DataName)
    if len(data) == 4: 
        info("Data Files:"); print(data)
    else: 
        error("Not all data samples available")

    commandAdd = "hadd {0} {1} {2} {3} {4}".format(dataMerged,data[0],data[1],data[2],data[3])
    commandRm  = "rm -rf {0} {1} {2} {3}".format(data[0],data[1],data[2],data[3])
   
    print(commandAdd)
    if options.Execute:
        os.system(commandAdd)
        time.sleep(1)
        os.system(commandRm)
        info("Created merged data file: {0}".format(dataMerged))

def getSamples(infiles, suf):
    out = []
    for f in infiles: 
        if f.find(suf)>-1: out.append(f)
    info("--> Found {0} samples with label: {1}".format(len(out),suf))
    return out

def getMCId(infile): 
    return infile.split("/")[-1].split(".")[2]

def getOutName(infile,label):
    return infile.replace("mc16a",label)

def info(msg):
    print("\033[1;34mINFO:\t{0}\033[0m".format(msg))

def error(msg):
    print("\033[1;31mERROR:\t{0}\033[0m".format(msg))
    sys.exit(1)

if __name__ == "__main__":
    main()
