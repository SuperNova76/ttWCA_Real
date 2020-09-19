import os
import sys
import time
import commands
import shutil
from glob import glob
from argparse import ArgumentParser

parser = ArgumentParser(description="Script for merging root files in container")
parser.add_argument("InDir",    help="Directory with input files", default ="")
parser.add_argument("--Outdir", help="Directory with output files", default="Merged")
parser.add_argument("--Prefix", help="Container prefixs", default="user.")
parser.add_argument("--Suffix", help="Suffix for merged files", default="_merged")
parser.add_argument("--Merge",  type=int, help="Merge the output (or just print the commands)", default=1)
options = parser.parse_args()

directory = options.InDir
if not len(directory):
    print("\033[31mPlease specify a directory \033[0m"); sys.exit(1)

if not os.path.exists(directory):
    print("\033[31mDirectory {0} does not exist \033[0m".format(directory)); sys.exit(1)

# Make a new outdir or clean the old one
if options.Merge and len(glob(directory+"/"+options.Outdir+"/*")) > 0:
    print("\033[1m-- Cleaning {0}/{1} \033[0m".format(directory,options.Outdir)) 
    shutil.rmtree(directory+"/"+options.Outdir, True)

if options.Merge:
    os.mkdir(directory+"/"+options.Outdir)
    print("\033[1m-- Created {0}/{1} \033[0m".format(directory,options.Outdir)) 

print("\033[1m-- Searching in {0} for containers with {1}* \033[0m".format(directory,options.Prefix))
containers = sorted(glob(directory+"/"+options.Prefix+"*"))

for container in containers:
    print("\033[92m-- Looking in {0} \033[0m".format(container))
    Outname = str(container.replace(".root",""))+options.Suffix+".root"
    filename = str(container)+"/*.root*"
    filelist = glob(filename)
    if not len(filelist):
        print("no .root file found"); continue
    else: 
        print("\033[1m--> Found {0} root files in {1} \033[0m".format(len(filelist),str(container)))

    command = "hadd {0} {1}".format(Outname, filename)
    print(command)
    if options.Merge: os.system(command)
    time.sleep(0.5)

    if options.Merge: shutil.move(Outname, directory+"/"+options.Outdir)
    time.sleep(0.5)
    print

print("..done")
