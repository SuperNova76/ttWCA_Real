export REL="21.2.143"

if [ ! $ATLAS_LOCAL_ACM_VERSION ]; then 
    echo "--> get voms-proxy"
    voms-proxy-init -voms atlas
    
    echo "--> setup ATLAS"
    export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
    source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh
    
    echo "--> setup extra modules"
    lsetup "rucio -w" "git" "panda" "pyami" "cmake"
fi

if [ ! $ACMSOURCEDIR ]; then
    echo "--> acm Setup"
    if [ ! -d build ]; then mkdir build; fi
    cd build
    acmSetup "--sourcearea=../uctanalysistop/ AnalysisBase,${REL}"
    cd ..
fi

echo "--> acm Compile"
acm compile

echo "--> done"
