#!/bin/bash

vcpkg install gtest:x64-linux-dynamic ffmpeg[ffmpeg,x264]:x64-linux-dynamic

wget -O sample.mp4 https://sample-videos.com/video123/mp4/720/big_buck_bunny_720p_1mb.mp4 
