export REL="21.2.160"
export SOURCE_DIR="uctanalysistop"

if [ ! $ATLAS_LOCAL_ROOT_BASE ]; then 
    echo "--> get voms-proxy"
    voms-proxy-init -voms atlas
    
    echo "--> setup ATLAS"
    export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
    source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh
    
    echo "--> setup extra modules"
    lsetup "rucio -w" "git" "panda" "pyami" "cmake"
fi

if [ ! $AnalysisBase_SET_UP ]; then
    echo "--> AnalysisBase Setup"
    cd ${SOURCE_DIR}
    asetup "AnalysisBase,${REL},here"
    cd ..
fi

echo "--> cmake Compile"
if [ ! -d build ]; then mkdir build; fi
cd build
cmake ../${SOURCE_DIR}
cmake --build ./
source */setup.sh
cd ..

echo "--> done"
