# ttWCA package

**Contact:** fabio.cardillo@cern.ch (or atlas-phys-top-ttw-ca@cern.ch)

[![pipeline status](https://gitlab.cern.ch/fcardill/ttWCA/badges/master/pipeline.svg)](https:://gitlab.cern.ch/fcardill/ttWCA/commits/master)

Package for the production of group Ntuples for the ttW charge-asymmetry analysis. The package is based on the AnalysisTop [CustomSaver](https://gitlab.cern.ch/atlasphys-top/reco/customsaverskeleton/-/tree/master). Most of the output is defined by the [EventSaverFlatNtuple](https://gitlab.cern.ch/atlas/athena/-/tree/21.2/PhysicsAnalysis/TopPhys/xAOD/TopAnalysis/Root) class in AnalysisTop. In addition, some extra branches are added for the defintion of the ttW-CA control and signal regions and for obtaining the data-driven fake lepton estimates via the matrix-method. The current version of the framework is based on AnalysisBase [21.2.202](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/AnalysisBaseReleaseNotes21_2).

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
With `infiles.txt` a list of locally existing [DAOD_TOPQ1](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/TopDerivations#The_TOPQ_derivation_formats) or [DAOD_HIGG8D1](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/HSG8DerivationFramework) files.


+ To submit jobs to the grid:
Should be executed in the directory where run, build and source are:
```
python source/ttWCA/grid/submitJobs.py source/ttWCA/grid/input.txt --Config=ConfigTOP.txt --Version=YourSuffix
```
With `input.txt` as a list of xAOD containers (full name, including scope), `--Config` the top-configuration file with the object defintions, preselection, etc., and `--Version` a custom suffix (version no.) to label the output containers. See `submitJobs.py --help` for further options.
