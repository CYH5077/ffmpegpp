#!/bin/bash

mkdir cmake_build
cd ./cmake_build
cmake -DCMAKE_BUILD_TYPE=Release -DFFMPEG_INCLUDE_DIR="/usr/include/x86_64-linux-gnu/" -DFFMPEG_LIBRARY_DIR="/usr/lib/x86_64-linux-gnu/" ..
make -j10
make install
cd ..
