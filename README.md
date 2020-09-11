# ttWCA package

**Responsible:** fabio.cardillo@cern.ch

[![pipeline status](https://gitlab.cern.ch/fcardill/ttWCA/badges/master/pipeline.svg)](https:://gitlab.cern.ch/fcardill/ttWCA/commits/master)

Proper documentation needs to be added 

1. [Compilation and cleaning ](#1-compilation-and-cleaning)
2. [Running](#2-running)

## 1. Compilation and cleaning:

Please perform the compilation and cleaning from the parent directory of your acm source directory.

+ To compile:
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
```
python source/ttWCA/python/submitJobs.py source/ttWCA/python/samples.txt
```
With `samples.txt` a list of datasets obtained with `rucio list-dids mc16_13TeV:mc16_13TeV.*.deriv.DAOD_TOPQ1.p* --short --filter type=CONTAINER`. Please submit this from the parent directory of your acm source directory.
