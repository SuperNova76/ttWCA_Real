import sys,os,commands
from argparse import ArgumentParser

parser = ArgumentParser(description="Script for deleting DSIDS from specific RSEs")
parser.add_argument("DSIDs", help="List with input datasets",      default="")
parser.add_argument("RSE",   help="Name of rucio storage element", default="")
parser.add_argument("--Delete", type=int, help="Execute delete commands",   default=1)
opt = parser.parse_args()

if not len(opt.DSIDs) or not os.path.isfile(opt.DSIDs): 
    print("Cannot find {0}".format(opt.DSIDs))
    sys.exit(1)

if not len(opt.RSE): 
    print("Did not specify RSE")
    sys.exit(1)

for line in open(opt.DSIDs):
    did     = line.split()[0]
    cmd     = "rucio list-rules {0}".format(did)
    os.system(cmd)

    cmd = "{0} | grep {1}".format(cmd, opt.RSE)
    if len( commands.getoutput(cmd) ): 
        rule_id = commands.getoutput(cmd).split()[0]
        delete  = "rucio delete-rule {0}".format(rule_id)
        print(delete)
        print
        if opt.Delete: os.system(delete)
    
print("...done")
