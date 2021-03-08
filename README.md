# ttWCA package

**Responsible:** fabio.cardillo@cern.ch

[![pipeline status](https://gitlab.cern.ch/fcardill/ttWCA/badges/master/pipeline.svg)](https:://gitlab.cern.ch/fcardill/ttWCA/commits/master)

Proper documentation needs to be added 

1. [Compilation and cleaning ](#1-compilation-and-cleaning)
2. [Running](#2-running)

## 1. Compilation and cleaning:

Please perform the compilation and cleaning from the parent directory of your acm source directory.

+ To setup [ATLAS environment](https://atlassoftwaredocs.web.cern.ch/ABtutorial/release_setup) and compile:
```
source/ttWCA/scripts/setup.sh
```

+ To clean:
```
source/ttWCA/scripts/clean.sh
```

## 2. Running

+ To run locally:
```
cd run
top-xaod ../source/ttWCA/share/Config_ttWCA.txt infiles.txt
```
With `infiles.txt` a list of locally existing DAOD_TOPQ1 files.


+ To submit jobs to the grid:
Should be executed in the directory where run, build and source are:
```
python source/ttWCA/grid/submitJobs.py source/ttWCA/grid/input.txt --Config=ConfigTOP.txt --Version=YourSuffix
```
With `input.txt` as a list of xAOD containers (full name, including scope), `--Config` the top-configuration file with the object defintions, preselection, etc., and `--Version` a custom suffix (version no.) to label the output containers. See `submitJobs.py --help` for further options.
