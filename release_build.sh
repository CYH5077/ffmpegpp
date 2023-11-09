#!/bin/bash

cmake -DCMAKE_TOOLCHAIN_FILE=/home/choi/git/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Release . && make -j10