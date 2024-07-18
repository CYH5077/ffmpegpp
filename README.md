# ffmpegpp
ffmpeg 6.1 c++ wrapper<br>
linux, windows support


<h2> Install vcpkg </h2>
<pre> <a href="https://vcpkg.io/en/">https://vcpkg.io/en/</a></pre>

<h2> Install package </h2>
<pre> $ vcpkg install ffmpeg[fdk-aac,fontconfig,freetype,fribidi,gpl,mp3lame,nvcodec,vorbis,vpx,webp,x264,x265] gtest </pre>/

<h1> Build </h1>
<pre>$ cmake -DCMAKE_TOOLCHAIN_FILE=<font color=red>{YOUR VCPKG PATH}</font>/scripts/buildsystems/vcpkg.cmake -DFFMPEGPP_TEST=ON .</pre>
<pre>$ make -j10</pre>
<pre>$ make install</pre>

<br>
<h1> DNN Build <h1>
Install CUDA 11.8 <br>
Install cudnn 8.6.0
<h2> DNN (Usage Super Resolution (EDSR, ESPCN, FSRCNN) </h2>
<pre> $ vcpkg install opencv4[core,contrib,cuda,dnn-cuda] </pre>

<h1> DNN Module </h1>
<pre>$ cmake -DCMAKE_TOOLCHAIN_FILE=<font color=red>{YOUR VCPKG PATH}</font>/scripts/buildsystems/vcpkg.cmake -DFFMPEGPP_SUPPRESS=ON -DFFMPEGPP_TEST=ON .</pre>
<pre>$ make -j10</pre>
<pre>$ make install</pre>
