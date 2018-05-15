rm -rf build
mkdir -p build
cd build && cmake ..
make -j 6 VERBOSE=1
cd -