export REL="21.2.172"
export SOURCEDIR="uctanalysistop"

if [ ! $ATLAS_LOCAL_ROOT ]; then
    echo "--> get voms-proxy"
    voms-proxy-init -voms atlas

    echo "--> setup ATLAS"
    export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
    source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh

    echo "--> setup extra modules"
    lsetup "rucio -w" "git" "panda" "pyami" "cmake"
fi

if [ ! $ACMSOURCEDIR ]; then
    echo "--> acm setup"
    if [ ! -d build ]; then mkdir build; fi
    cd build
    acmSetup "--sourcearea=../${SOURCEDIR} AnalysisBase,${REL}"
    cd ..
fi

echo "--> acm compile"
acm compile

echo "--> done"
