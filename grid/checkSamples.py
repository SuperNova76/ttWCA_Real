import os,sys,time,subprocess

fnames = sys.argv
fnames.pop(0);

if not len(fnames): 
    print("\033[1;31mERROR: Input does not contain readable lists\033[0m"); sys.exit(1)

if not "RUCIO_ACCOUNT" in os.environ:
    print("\033[1;31mERROR: rucio account not set... please run these commands before: setupATLAS && lsetup rucio\033[0m"); sys.exit(1)

N_found = 0
N_notfound = 0

print("Lists with datasets: {0}\n".format(fnames))
for fname in fnames:
    for l in open(fname):
        if not len(l.split()): continue
    
        sample = l.split()[0]
        if sample[0] == "#": continue
        print("\033[1;34mChecking dataset: {0}\033[0m".format(sample))

        cmd = "rucio list-dids {0} --short --filter type=CONTAINER".format(sample)
        sub = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
        dsname = sub.stdout.read().strip('\n')

        if sample == dsname: 
            print("\033[1;32m--> Dataset found\033[0m")
            N_found = N_found+1
        else: 
            print("\033[1;31m--> Dataset not found\033[0m")
            N_notfound = N_notfound+1
        
        time.sleep(0.5)
        print("")

print("\033[1;34mChecked datasets: {0} (found = {1}, not found = {2})\033[0m".format(N_found + N_notfound, N_found, N_notfound))
