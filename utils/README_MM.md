The purpose of this executable is to obtain quickly fake lepton predictions from the matrix-method (asymptotic or likelihood) from small Ntuples. They can be derived for any signal or control region, either inclusive or differentially binned in any variable available in the Ntuples.

1. [Running the Executable](#1-running-the-executable)
2. [Configuration Options](#2-configuration-options)
3. [Adding a Region or Variable](#3-adding-a-region-or-variable)

## 1. Running the Executable

The MM tool can be executed with:

```
runMMTool data.root config.conf
```

With `data.root` the Ntuple file for data and `config.conf` the configuration file with the options ( see [here](#2-configuration-options) ). A script executing this command in a specific directory, where all data Ntuples (e.g. separated by year) are located, with the default configuration is provided (`doAll.py`). 

The output is a `.root` file (name can be set in the configuration file) with histograms containing the fake lepton estimates for each region and each variable.


## 2. Configuration Options

+ `Debug` : Setting the tool to debug mode.
+ `MMType` : Type of the matrix-method to be applied. 1) Likelihood matrix-method, 2) asymptotic (linear) matrix-method.
+ `Selection, Process` : The lepton multiplicity and number of fakes to be estimated in a region can be set by the `Selection` and `Process` arguments in the configuration file. They are forwarded to the [FakeBkgTools](https://gitlab.cern.ch/atlas/athena/-/tree/21.2/PhysicsAnalysis/AnalysisCommon/FakeBkgTools) and are equivalent to the arguments described [here](https://gitlab.cern.ch/atlas/athena/-/tree/21.2/PhysicsAnalysis/AnalysisCommon/FakeBkgTools/doc).
+ `SignalRegion` : Names of the signal regions to be processed. These names have to be the same as the definitions added to the executable in the `passCuts()` function ( see [here](#3-adding-region-or-variable) ). For processing several regions, a comma-separated list can be provided.
+ `EffFile` : Input file containing fake and real efficiencies (see [here](https://gitlab.cern.ch/atlas/athena/-/blob/21.2/PhysicsAnalysis/AnalysisCommon/FakeBkgTools/doc/import_root.md) for details). If available to eos, the eos path to the file can be provided, but a CERN token needs to be created before.
+ `OutFileName` : Name (location) of the output file.


## 3. Adding a Region or Variable

For adding a new region, the definition can be added to the `passCuts()` function in the executable. Each region definition needs to have a name which is the same as the name in the configuration file. All requirements for the regions need be added there, except from the lepton multiplicity and thightness (which are given by the `Selection` and `Process` arguments in the configuration file).

New variables for differential estimation can be added by defining a histogram with a specific binning and add it to the `histos` vector (e.g. line [364](https://gitlab.cern.ch/fcardill/ttWCA/-/blob/master/utils/runMMTool.cxx#L364) in `runMMTool.cxx`). The histograms need furthermore to be associated with the a variable available in the Ntuples (line [441](https://gitlab.cern.ch/fcardill/ttWCA/-/blob/master/utils/runMMTool.cxx#L441) in `runMMTool.cxx`). Make sure that the vector positions of the variables match to the positions of the histograms in the `histos` vector. If new variables need to be included, they have to be added via `TTreeReaderValue` (line [318](https://gitlab.cern.ch/fcardill/ttWCA/-/blob/master/utils/runMMTool.cxx#L318) in `runMMTool.cxx`) and their names have to be defined in the [branchMap.conf](https://gitlab.cern.ch/fcardill/ttWCA/-/blob/master/utils/branchMap.conf).
