import os,sys,shutil,time
from glob import glob
from argparse import ArgumentParser

def main():
    parser = ArgumentParser(description="Script for renaming ttW-CA grid output files")
    parser.add_argument("Indir",      help="Directory with input files",     default = "")
    parser.add_argument("--Type",     help="File type [default .root]",      default = ".root")
    parser.add_argument("--Outdir",   help="Directory for output files",     default = "ttWCA-10-03-21")
    parser.add_argument("--SkipData", type=int, help="Skip the copying of data files",      default = 0)
    parser.add_argument("--Copy",     type=int, help="Copy files (or just print commands)", default = 1)
    options = parser.parse_args()

    indir  = options.Indir
    outdir = options.Outdir
    if not len(indir): error("Please specify a directory")

    if not os.path.exists(indir): 
        error("Directory {0} does not exist".format(indir))

    info("Create {0}".format(outdir))
    if os.path.exists(outdir):
        warning("Directory {0} already exists: removing existing".format(outdir))
        shutil.rmtree(outdir)
    os.mkdir(outdir)

    info("Creating subdirectories")
    os.mkdir(outdir+"/2016_mc16a")
    os.mkdir(outdir+"/2017_mc16d")
    os.mkdir(outdir+"/2018_mc16e")
    time.sleep(0.5)
    
    if not options.SkipData:
        info("Copying data files")
        data  = sorted(glob(indir+"/*data*"+options.Type))

        if not len(data): error("No data files of type {0} found in {1} Aborting".format(options.Type, indir))
        info("Found {0} data files".format(len(data)))

        for dataname in datalist():
            os.mkdir(outdir+"/"+datalist()[dataname]+"/"+dataname)

        for datafile in data:
            for dataname in datalist():
                if datafile.find(dataname.replace("20","")) > -1: 
                    cmd_data = "cp {0} {1}/{2}/{3}/{3}{4}".format(datafile, outdir, datalist()[dataname], dataname, options.Type)
                    print(cmd_data)
                    if options.Copy: os.system(cmd_data)


    info("Copying MC files")
    mc16a = sorted(glob(indir+"/*mc16a*"+options.Type))
    mc16d = sorted(glob(indir+"/*mc16d*"+options.Type))
    mc16e = sorted(glob(indir+"/*mc16e*"+options.Type))

    if len(mc16a)==0 or len(mc16d)==0 or len(mc16e)==0: 
        error("No files of type {0} found in {1} Aborting".format(options.Type, directory))

    if not (len(mc16a) == len(mc16d) == len(mc16e)): error("Inconsistent sizes of mc16a/d/e sample lists")
    info("Found {0} mc16a/d/e files".format(len(mc16a)))

    DSIDs = 0
    for dsid in processes():
        name = processes()[dsid]
        if not len(dsid) or not len(name): continue
        info("Process {0}: DSID = {1}".format(name, dsid))

        os.mkdir(outdir+"/2016_mc16a/"+name)
        os.mkdir(outdir+"/2017_mc16d/"+name)
        os.mkdir(outdir+"/2018_mc16e/"+name)

        filenames = [findFile(mc16a,dsid), findFile(mc16d,dsid), findFile(mc16e,dsid)]
        if "" in filenames: continue

        cmd_mc16a = "cp {0} {1}/{2}/{3}/{3}{4}".format(filenames[0], outdir, "2016_mc16a", name, options.Type)
        cmd_mc16d = "cp {0} {1}/{2}/{3}/{3}{4}".format(filenames[1], outdir, "2017_mc16d", name, options.Type)
        cmd_mc16e = "cp {0} {1}/{2}/{3}/{3}{4}".format(filenames[2], outdir, "2018_mc16e", name, options.Type)
        cmd_mc16  = [cmd_mc16a, cmd_mc16d, cmd_mc16e]
        DSIDs = DSIDs + 1

        for cmd in cmd_mc16:
            print(cmd)
            if options.Copy: os.system(cmd)

    print("Copied files for {0} DSIDs".format(DSIDs))

def datalist():
    dat = {"data2015": "2016_mc16a", 
           "data2016": "2016_mc16a", 
           "data2017": "2017_mc16d", 
           "data2018": "2018_mc16e"}
    return dat

def processes():
    proc = {"700000": "ttW_Sherpa228",
            "410155": "ttW_Pythia",
            "700168": "ttW_Sherpa2210",
            "700205": "ttW_Sherpa2210_EWK",
            "410218": "ttZee",
            "410219": "ttZmumu",
            "410220": "ttZtautau",
            "410276": "ttZee_mll_1_5",
            "410277": "ttZmumu_mll_1_5",
            "410278": "ttZtautau_mll_1_5",
            "364250": "llll",
            "364253": "lllv",
            "364283": "lllljj_EWK",
            "364284": "lllvjj_EWK",
            "364288": "llll_lowMll",
            "364289": "lllv_lowMll",
            "345705": "ggllll_0M4l130",
            "345706": "ggllll_130M4L",
            "346443": "ttH_dilep",
            "346444": "ttH_semilep",
            "346445": "ttH_allhad",
            "412118": "tWZ_DR1",
            "412119": "tWZ_DR2",
            "412063": "tZq",
            "410081": "ttWW",
            "342284": "WH",
            "342285": "ZH",
            "364242": "WWW_3l3v",
            "364243": "WWZ_4l2v",
            "364244": "WWZ_2l4v",
            "364245": "WZZ_5l1v",
            "364246": "WZZ_3l3v",
            "364247": "ZZZ_6l0v",
            "364248": "ZZZ_4l2v",
            "364249": "ZZZ_2l4v",
            "410470": "tt_PhPy8",
            "410557": "tt_PhHw7",
            "410389": "tty",
            "304014": "ttt",
            "412043": "tttt",
            "364100": "Zmumu_MAXHTPTV0_70_CVetoBVeto",
            "364101": "Zmumu_MAXHTPTV0_70_CFilterBVeto",
            "364102": "Zmumu_MAXHTPTV0_70_BFilter",
            "364103": "Zmumu_MAXHTPTV70_140_CVetoBVeto",
            "364104": "Zmumu_MAXHTPTV70_140_CFilterBVeto",
            "364105": "Zmumu_MAXHTPTV70_140_BFilter",
            "364106": "Zmumu_MAXHTPTV140_280_CVetoBVeto",
            "364107": "Zmumu_MAXHTPTV140_280_CFilterBVeto",
            "364108": "Zmumu_MAXHTPTV140_280_BFilter",
            "364109": "Zmumu_MAXHTPTV280_500_CVetoBVeto",
            "364110": "Zmumu_MAXHTPTV280_500_CFilterBVeto",
            "364111": "Zmumu_MAXHTPTV280_500_BFilter",
            "364112": "Zmumu_MAXHTPTV500_1000",
            "364113": "Zmumu_MAXHTPTV1000_E_CMS",
            "364114": "Zee_MAXHTPTV0_70_CVetoBVeto",
            "364115": "Zee_MAXHTPTV0_70_CFilterBVeto",
            "364116": "Zee_MAXHTPTV0_70_BFilter",
            "364117": "Zee_MAXHTPTV70_140_CVetoBVeto",
            "364118": "Zee_MAXHTPTV70_140_CFilterBVeto",
            "364119": "Zee_MAXHTPTV70_140_BFilter",
            "364120": "Zee_MAXHTPTV140_280_CVetoBVeto",
            "364121": "Zee_MAXHTPTV140_280_CFilterBVeto",
            "364122": "Zee_MAXHTPTV140_280_BFilter",
            "364123": "Zee_MAXHTPTV280_500_CVetoBVeto",
            "364124": "Zee_MAXHTPTV280_500_CFilterBVeto",
            "364125": "Zee_MAXHTPTV280_500_BFilter",
            "364126": "Zee_MAXHTPTV500_1000",
            "364127": "Zee_MAXHTPTV1000_E_CMS",
            "364198": "Zmm_Mll10_40_MAXHTPTV0_70_BVeto",
            "364199": "Zmm_Mll10_40_MAXHTPTV0_70_BFilter",
            "364200": "Zmm_Mll10_40_MAXHTPTV70_280_BVeto",
            "364201": "Zmm_Mll10_40_MAXHTPTV70_280_BFilter",
            "364202": "Zmm_Mll10_40_MAXHTPTV280_E_CMS_BVeto",
            "364203": "Zmm_Mll10_40_MAXHTPTV280_E_CMS_BFilter",
            "364204": "Zmm_Mee10_40_MAXHTPTV0_70_BVeto",
            "364205": "Zmm_Mee10_40_MAXHTPTV0_70_BFilter",
            "364206": "Zmm_Mee10_40_MAXHTPTV70_280_BVeto",
            "364207": "Zmm_Mee10_40_MAXHTPTV70_280_BFilter",
            "364208": "Zmm_Mee10_40_MAXHTPTV280_E_CMS_BVeto",
            "364209": "Zmm_Mee10_40_MAXHTPTV280_E_CMS_BFilter",
            "364210": "Ztt_Mll10_40_MAXHTPTV0_70_BVeto",
            "364211": "Ztt_Mll10_40_MAXHTPTV0_70_BFilter",
            "364212": "Ztt_Mll10_40_MAXHTPTV70_280_BVeto",
            "364213": "Ztt_Mll10_40_MAXHTPTV70_280_BFilter",
            "364214": "Ztt_Mll10_40_MAXHTPTV280_E_CMS_BVeto",
            "364215": "Ztt_Mll10_40_MAXHTPTV280_E_CMS_BFilter",
            "366140": "eegamma_LO_pty_7_15",
            "366141": "eegamma_LO_pty_15_35",
            "366142": "eegamma_LO_pty_35_70",
            "366143": "eegamma_LO_pty_70_140",
            "366144": "eegamma_LO_pty_140_E_CMS",
            "366145": "mumugamma_LO_pty_7_15",
            "366146": "mumugamma_LO_pty_15_35",
            "366147": "mumugamma_LO_pty_35_70",
            "366148": "mumugamma_LO_pty_70_140",
            "366149": "mumugamma_LO_pty_140_E_CMS",
            "366150": "tautaugamma_LO_pty_7_15",
            "366151": "tautaugamma_LO_pty_15_35",
            "366152": "tautaugamma_LO_pty_35_70",
            "366153": "tautaugamma_LO_pty_70_140",
            "366154": "tautaugamma_LO_pty_140_E_CMS",
            "361106": "Zee_PhPy8",
            "361107": "Zmm_PhPy8",
            "361108": "Ztt_PhPy8"}
    return proc

def findFile(files, key):
    for f in files:
        if f.find(key) > -1: return f
    warning("File {0} not found in list".format(key))
    return ""

def info(msg):
    print("\033[1;34mINFO:\t {0}\033[0m".format(msg))

def warning(msg):
    print("\033[1;33mWARNING: {0}\033[0m".format(msg))

def error(msg):
    print("\033[1;31mERROR:\t {0}\033[0m".format(msg))
    sys.exit(1)

if __name__ == "__main__":
    main()
