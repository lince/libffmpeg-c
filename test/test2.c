#include <libffmpeg/libffmpeg.h>
#include <stdio.h>


void call_back(int lvl, char* message) {
	printf(message);
}

int main() {
	FFMpeg_init(5);
	FFMpeg_registerLoggerCallBack(call_back);

	FFMpeg_setFormat("mp4");
	FFMpeg_setInputFile("../../libavencoding/test/videosample.mp4");
	FFMpeg_setVideoCodec("libx264");
	FFMpeg_setVideoPreset("baseline");
	FFMpeg_setOther("crf", "24");
	FFMpeg_setFrameSize2(400, 300);
	FFMpeg_setStartTime2(40);
	FFMpeg_setRecordingTime2(10);
	FFMpeg_setFormat("mp4");
	FFMpeg_setOutputFile("saida1.mp4");
	FFMpeg_transcode();
}
