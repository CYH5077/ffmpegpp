#!/bin/bash

mkdir cmake_build
cd ./cmake_build
cmake -DFFMPEG_INCLUDE_DIR="D:\git\ffmpeg_lib\ffmpeg\include" -DFFMPEG_LIBRARY_DIR="D:\git\ffmpeg_lib\ffmpeg\lib" ..
make -j10
make install
cd ..
