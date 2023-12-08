@echo off

cd ./build
cmake -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" ../
make -j10
make install
cd ../