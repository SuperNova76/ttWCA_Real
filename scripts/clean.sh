echo "--> cleaning"
if [ $ACMSOURCEDIR ]; then
    if [ -a ${ACMSOURCEDIR}/CMakeLists.txt ]; then rm ${ACMSOURCEDIR}/CMakeLists.txt; fi
fi

if [ -d build ]; then
    rm -rf build
fi
echo "--> done"
