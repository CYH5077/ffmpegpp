#!/bin/bash

mkdir build
cd ./build
cmake -DCMAKE_BUILD_TYPE=Release . && make -j10
cd