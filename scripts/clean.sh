echo "--> cleaning"
if [ -a ${TestArea}/CMakeLists.txt ]; then 
    rm ${TestArea}/CMakeLists.txt
fi

if [ -d build ]; then
    rm -rf build
fi

echo "--> done"
