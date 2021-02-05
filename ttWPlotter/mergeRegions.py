import os,sys,time
from glob import glob
from argparse import ArgumentParser

def main():
    parser = ArgumentParser(description="Script for merging histograms from different regions")
    parser.add_argument("--Dir",    help="Directory prefix",                                        default = "")
    parser.add_argument("--OutDir", help="Directory for merged histograms [default=HISTOS]",        default = "HISTOS")
    parser.add_argument("--Type",   help="File type to be merged          [default =.root]",        default = ".root")
    parser.add_argument("--Merge",  type=int, help="Merge the output (or just print the commands)", default=1)
    options = parser.parse_args()

    pattern = options.Dir
    if not len(pattern): error("Option --Dir empty")

    dirs = sorted(glob(pattern + "*"))
    if not len(dirs): error("Directories with name {0} do not exist".format(pattern))
    print("\nDirectories {0}:".format(dirs))

    if not "ROOTSYS" in os.environ:
        error("No ROOT setup found. Please setup ROOT first")
    else:
        info("Using ROOT version {0}/{1}".format(os.getenv('ROOTSYS').split("/")[-2],os.getenv('ROOTSYS').split("/")[-1]))

    if len(options.OutDir) and not os.path.isdir(options.OutDir):
        info("Directory {0} does not exist, creating".format(options.OutDir))
        os.mkdir(options.OutDir);

    filelist = noPath(sorted(glob(dirs[0] + "/*" + options.Type)))
    checkFiles(dirs,filelist)

    for f in filelist:
        cmd = "hadd {0}/{1}".format(options.OutDir,f)
        for d in dirs: cmd += " {0}/{1}".format(d,f)
        print(cmd)
        if options.Merge: os.system(cmd)

    if options.Merge:
        info("Merged {0} {1} files in {2}".format(len(filelist),options.Type,options.OutDir))

    info("..done")

def checkFiles(dirs,files):
    for f in files:
        for d in dirs:
            name = "{0}/{1}".format(d,f)
            if not os.path.isfile(name): error("File {0} not found".format(name))

def noPath(lIn):
    lOut = []
    for i in lIn: lOut.append(i.split("/")[-1])
    return lOut

def info(msg):
    print("\033[1;34mINFO:\t{0}\033[0m".format(msg))

def error(msg):
    print("\033[1;31mERROR:\t{0}\033[0m".format(msg))
    sys.exit(1)

if __name__ == "__main__":
    main()

