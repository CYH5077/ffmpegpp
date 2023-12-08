#!/bin/bash

root_path=$(pwd)
root_thirdparty_path=$root_path/thirdparty

function submodule_update {
    git submodule init 
    git submodule update
}

function thirdparty_build {
    cd $root_thirdparty_path

    ###################################################### ffmpeg build
    cd ./ffmpeg
    ./configure --prefix=./build                  \
                --enable-gpl --enable-nonfree     \
                --enable-shared --disable-static  \
                --enable-pthreads                 \
                --enable-libfdk-aac               \
                --enable-libx264 --enable-libx265 \
                --disable-debug
    if [ $? -ne 0 ]; then
        exit -1
    fi 
    
    make -j$(grep -c processor /proc/cpuinfo)
    make install
    cd $root_thirdparty_path

    ###################################################### gtest build
    cd ./gtest
    mkdir build
    cd build && cmake ..
    make -j$(grep -c processor /proc/cpuinfo)
    cd $root_thirdparty_path

    # thirdparty out
    cd $root_path
}

function sample_video_download {
    # sample video download
    wget --directory-prefix=./build/bin -O sample.mp4 https://sample-videos.com/video123/mp4/720/big_buck_bunny_720p_1mb.mp4 
    if [ $? -ne 0 ]; then
        exit -1
    fi
}

# submodule update
submodule_update

# sample video download
sample_video_download

# thirdparty build
thirdparty_build

