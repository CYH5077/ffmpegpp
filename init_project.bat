@echo off

for /f %%A in ('cd') do set root_path=%%A
set root_thirdparty_path=%root_path%/thirdparty/windows

call :submodule_update
call :sample_video_download
call :thirdparty_install

:submodule_update
echo "submodule update"
git lfs update 
git lfs pull
exit /b 

:thirdparty_install
echo "install gtest"
cd %root_thirdparty_path%/gtest
tar xf gtest.tar
echo "install ffmpeg"
cd %root_thirdparty_path%/ffmpeg
tar xf ffmpeg.tar
cd %root_path%
exit /b

:sample_video_download
echo "sample video download"
mkdir build
cd build
mkdir bin
cd bin
wget -O sample.mp4 https://sample-videos.com/video123/mp4/720/big_buck_bunny_720p_1mb.mp4 
if %ERRORLEVEL% neq 0 (
   exit -1 
)
cd %root_path%
exit /b
