#./configure --enable-gpl --enable-nonfree --enable-static --enable-postproc --enable-pthreads --enable-shared --enable-libfaac --enable-libopencore-amrnb --enable-libopencore-amrwb --enable-libtheora --enable-libx264 --enable-libxvid --enable-x11grab --enable-muxer=alsa --enable-demuxer=alsa --enable-avfilter --enable-libmp3lame


#./configure --enable-gpl --enable-version3 --enable-nonfree --enable-static --enable-postproc --enable-pthreads --enable-shared --enable-libfaac --enable-#libopencore-amrnb --enable-libopencore-amrwb --enable-libtheora --enable-libx264 --enable-libxvid --enable-x11grab

sudo apt-get install libfaac-dev libfreetype6-dev libmp3lame-dev libxvidcore-dev libopenjpeg-dev libtheora-dev libasound2-dev yasm

git clone git://git.videolan.org/x264.git libx264
cd libx264
./configure --enable-shared
make
sudo make install
cd ..


./configure --enable-shared --enable-gpl --enable-version3 --enable-x11grab --enable-libfaac --enable-libfreetype --enable-libmp3lame --enable-libtheora --enable-libx264 --enable-libxvid  --enable-libopenjpeg --enable-muxer=alsa --enable-demuxer=alsa --enable-avfilter --enable-nonfree

make

sudo make install




#INSTALL	libavdevice/libavdevice.so
#INSTALL	libavfilter/libavfilter.so
#INSTALL	libavformat/libavformat.so
#INSTALL	libavcodec/libavcodec.so
#INSTALL	libpostproc/libpostproc.so
#INSTALL	libswscale/libswscale.so
#INSTALL	libavutil/libavutil.so

