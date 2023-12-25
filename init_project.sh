#!/bin/bash

root_path=$(pwd)
root_thirdparty_path=$root_path/thirdparty

function submodule_update {
    git submodule init 
    git submodule update
}

function sample_video_download {
    mkdir build
    cd build
    mkdir bin
    cd bin
    # sample video download
    wget --directory-prefix=./build/test -O sample.mp4 https://sample-videos.com/video123/mp4/720/big_buck_bunny_720p_1mb.mp4
    if [ $? -ne 0 ]; then
        exit -1
    fi

    cd $root_path
}

# submodule update
submodule_update

# sample video download
sample_video_download

