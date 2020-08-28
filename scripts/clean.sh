echo "--> cleaning"
if [ $ATLAS_LOCAL_ACM_VERSION ]; then 
    acm clean 
fi
if [ -a source/CMakeLists.txt ]; then 
    rm source/CMakeLists.txt
fi

if [ -d build ]; then
    rm -rf build
fi

echo "--> done"
