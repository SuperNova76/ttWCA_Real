import os,sys

if len(sys.argv) < 2: 
    print("No search expression found"); sys.exit(1)

expr = sys.argv[1]
if not len(expr) or expr.isspace(): 
    print ("No search expression found"); sys.exit(1)

expr = "rucio list-dids --short --filter type=CONTAINER {0} | sort  &> list.txt".format(expr) 
os.system(expr)

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
