export REL="21.2.136"

if [ ! $ATLAS_LOCAL_ACM_VERSION ]; then 
    echo "--> get voms-proxy"
    voms-proxy-init -voms atlas
    
    echo "--> setup ATLAS"
    setupATLAS
    lsetup "rucio"
    lsetup "git"
    lsetup "panda"
    lsetup "pyami"
fi

if [ ! $ACMSOURCEDIR ]; then
    echo "--> acm Setup"
    if [ ! -d build ]; then mkdir build; fi
    cd build
    acmSetup "--sourcearea=../source/ AnalysisBase,${REL}"
    cd ..
fi

echo "--> acm Compile"
acm compile

echo "--> done"
