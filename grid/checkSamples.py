import os,sys

fnames = sys.argv
fnames.pop(0)
print(fnames)

for fname in fnames:
    for l in open(fname):
        if not len(l.split()): continue
    
        sname = l.split()[0]
        if sname[0] == "#": continue
        cmd = "rucio list-dids {0} --filter type=CONTAINER".format(sname)
        print(cmd)

        os.system(cmd)
        print("")
