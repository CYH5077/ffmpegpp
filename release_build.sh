#!/bin/bash

cd ./build
cmake -DCMAKE_BUILD_TYPE=Release .
make -j10
make install
cd
