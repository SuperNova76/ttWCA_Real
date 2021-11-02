import os,sys,time,subprocess
from glob import glob
from argparse import ArgumentParser

if not "ACMSOURCEDIR" in os.environ:
    print("ACM source-direcory not set... please compile the tool before submitting jobs:\n\n . uctanalysistop/ttWCA/scripts/setup.sh\n"); sys.exit(1)

ACM_SOURCE   = os.getenv('ACMSOURCEDIR')

def main():
    parser = ArgumentParser(description="Script for submitting grid jobs from sample list")
    parser.add_argument("Samples",      help="List of input MC or data samples (.txt)")
    parser.add_argument("--User",       help="Name of user (CERN name)",                                 default="")
    parser.add_argument("--Config",     help="Name of top-config file (path is resovled automatically)", default="Config_ttWCA.txt")
    parser.add_argument("--Version",    help="Version of the production",                                default="uctatlas_v5b1-HIGG")
    parser.add_argument("--FilesPerJob",type=int, help="Number of files per job (prun option)",          default=5)
    parser.add_argument("--Submit",     type=int, help="Submit jobs (or only print the prun-command)",   default=1)
    options = parser.parse_args()

    if not os.path.isfile(options.Samples): 
        error("List of samples not found, aborting")

    username = options.User
    if not len(username):
        info("Username not found. Trying to find it")
        username = findUser()
        if not len(username): error("No username found. Please provide your username (CERN name) or setup a grid proxy")

    if not "{0}/{1}".format(os.getcwd(),ACM_SOURCE.split("/")[-1]) == ACM_SOURCE: 
        error("Incorrect directory. Please submit from {0}/../".format(ACM_SOURCE))

    NJobs = 0
    for sample in open(options.Samples): 
        NJobs = NJobs + makeJob(sample,username,options.Config,options.Version,options.FilesPerJob,options.Submit)

    info("Processed {0} jobs".format(NJobs))
    info("Done")
    return


def makeJob(DS,username,config,version,filesperjob,submit):
    if not len(DS) or DS.isspace() or  DS[0]=='#': return 0
    DS = DS.split()[0].split(':')[-1]
    print
    info("Sample: {0}".format(DS))

    cmd  = "prun \\\n"
    cmd += "--inDS={0} \\\n"                                                                                                                     .format(DS)
    cmd += "--outDS=user.{0}.{1}.{2}/ \\\n"                                                                                                      .format(username,makeOutputName(DS),version)
    cmd += "--excludeFile=build,run,{0}/analyses,{0}/scripts,{0}/tools,{0}/.git,{0}/ATNtupleProduction,{0}/ttWCA/.git,{0}/ttWCA/ttWPlotter \\\n" .format(ACM_SOURCE.split("/")[-1])
    cmd += "--useAthenaPackages --cmtConfig={0} --osMatching  \\\n"                                                                              .format(os.getenv('CMTCONFIG'))
    cmd += "--writeInputToTxt=IN:in.txt --outputs=output.root --exec=\"top-xaod {0} in.txt\" \\\n"                                               .format(findFile(ACM_SOURCE.split("/")[-1],config))
    cmd += "--extFile={0}/ttWCA/share/* \\\n"                                                                                                    .format(ACM_SOURCE.split("/")[-1])
    cmd += "--nFilesPerJob={0} --nGBPerJob=MAX --mergeOutput \\\n"                                                                               .format(filesperjob)

    print(cmd)
    if submit: os.system(cmd)
    time.sleep(1)
    return 1

def makeOutputName(name):
    if name.find('data')>-1:
        year     = name.split('.')[0]
        physname = name.split('.')[1] + "." + name.split('.')[2]
        ptag     = name.split('_p')[-1]
        return "{0}.{1}_p{2}".format(year,physname,ptag)
    else:
        dsid     = name.split('.')[1]
        physname = name.split('.')[2]
        ptag     = name.split('_p')[-1]
        MCcamp    = getMCcamp(name)
        return "{0}.{1}_{2}_p{3}".format(dsid,physname,MCcamp,ptag)
    return

def getMCcamp(n):
    if n.find('r9364')>-1:  return "mc16a"
    if n.find('r10201')>-1: return "mc16d"
    if n.find('r10724')>-1: return "mc16e"
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

def findUser():
    sub = subprocess.Popen("voms-proxy-info", shell=True, stdout=subprocess.PIPE)
    proxyinfo = sub.stdout.read()
    username = proxyinfo.split("/")[5].strip("CN=")
    info("Username from voms-proxy-info: {0}".format(username))
    return username

def info(msg):
    print("\033[1;34mINFO:\t{0}\033[0m".format(msg))

def error(msg):
    print("\033[1;31mERROR:\t{0}\033[0m".format(msg))
    sys.exit(1)

if __name__ == "__main__":
    main()
