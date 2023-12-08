@echo off

cd ./build
cmake -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" ../
make -j10
make install
cd ../
