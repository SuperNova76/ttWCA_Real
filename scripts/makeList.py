import os,sys
from argparse import ArgumentParser

parser = ArgumentParser(description="Script for rucio download commands")
parser.add_argument("expr",   help="rucio search expression", default="")
parser.add_argument("--dstype", help="dataset type to search",  default="CONTAINER")
opt = parser.parse_args()

if not len(opt.expr) or opt.expr.isspace(): 
    print ("No search expression found"); sys.exit(1)

if not "RUCIO_ACCOUNT" in os.environ: 
    print("rucio account not set... please run these commands before: setupATLAS && lsetup rucio"); sys.exit(1)

rucioCmd = "rucio list-dids --short --filter type={0} {1} | sort  &> list.txt".format(opt.dstype,opt.expr) 
os.system(rucioCmd)

txtFile = open("list.txt","rw")
cmdFile = open("list.sh", "wr")

nDS = 0
for line in txtFile:
    dataset = line.split()[0]
    cmd = "rucio download {0}".format(dataset)
    print(cmd)
    cmd = cmd + "\n"
    cmdFile.write(cmd)
    nDS = nDS+1

os.remove("list.txt")
print "--> Created {0} with {1} datasets".format(cmdFile.name, nDS)
cmdFile.close
