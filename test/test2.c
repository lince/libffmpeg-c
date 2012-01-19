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
	FFMpeg_setStartTime1("10");
	FFMpeg_setRecordingTime1("10");
	FFMpeg_setFormat("mp4");
	FFMpeg_setOverwriteFile(1);
	FFMpeg_setOutputFile("saida1.mp4");
	FFMpeg_transcode();
}
