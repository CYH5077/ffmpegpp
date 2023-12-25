@echo off

for /f %%A in ('cd') do set root_path=%%A
set root_thirdparty_path=%root_path%/thirdparty/windows

call :submodule_update
call :sample_video_download

:submodule_update
    git submodule init
    git submodule update
    exit /b

:sample_video_download
   echo "sample video download"
   mkdir build
   cd build
   mkdir test
   cd test
   wget -O sample.mp4 https://sample-videos.com/video123/mp4/720/big_buck_bunny_720p_1mb.mp4
   if %ERRORLEVEL% neq 0 (
      exit -1 
   )
   cd %root_path%
   exit /b
