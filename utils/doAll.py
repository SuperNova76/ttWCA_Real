import os,sys,time,commands
from glob import glob
from argparse import ArgumentParser

def main():
    parser = ArgumentParser(description="Small script to run on merged data sample")
    parser.add_argument("InDir",        help="Directory with data input files", default="")
    parser.add_argument("--ConfigFile", help="Name of MM config file",          default="Config.conf")
    options = parser.parse_args()

    directory = options.InDir
    if not len(directory):
        print("Please specify a directory"); sys.exit(1)

    if not os.path.exists(directory):
        print("Directory {0} does not exist".format(directory)); sys.exit(1)

    files = sorted(glob(directory+"/*data*.root"))
    if not len(files):
        print("No data files found"); sys.exit(1)

    print("Data files:"); print(files)

    tmpFile = "dataAll.root"
    cmdAdd  = "hadd {0} {1}/*data*.root".format(tmpFile, directory)
    cmdRm   = "rm {0}".format(tmpFile)

    if os.path.isfile(tmpFile):
        print("--> Removing old file")
        os.system(cmdRm)
    time.sleep(1)

    print(cmdAdd)
    os.system(cmdAdd)
    if not os.path.isfile(tmpFile):
        print("--> File {0} cannot be created".format(tmpFile)); sys.exit(1)

    print("--> Created temporary file {0}".format(tmpFile))
    time.sleep(1)

    cmdRun = "runMMTool {0} {1}".format(tmpFile, options.ConfigFile)
    print(cmdRun)
    os.system(cmdRun)

    print("--> Removing temporary file")
    os.system(cmdRm)
    print("...done")
        
if __name__ == "__main__":
    main()



