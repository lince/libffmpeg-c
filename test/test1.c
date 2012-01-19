#include "../include/libffmpeg.h"

#include <stdio.h>

int main() {
	FFMpeg_init(5);

	FFMpeg_setFramerate("25");
	FFMpeg_setFormat("mp4");
	FFMpeg_setInputFile("../../libavencoding/test/videosample.mp4");
	FFMpeg_setVideoCodec("libx264");
	FFMpeg_setVideoPreset("baseline");
	FFMpeg_setOther("crf", "24");
	FFMpeg_setFrameSize2(400, 300);
	FFMpeg_setStartTime2(40);
	//FFMpeg_setDisableAudio(1);
	FFMpeg_setFormat("mp4");
	FFMpeg_setOutputFile("saida1.mp4");
	FFMpeg_transcode();

	FFMpeg_reset(0);
	printf("\n\n\n\n\n\n");

	FFMpeg_setFramerate("25");
	FFMpeg_setFormat("mp4");
	FFMpeg_setInputFile("../../libavencoding/test/videosample.mp4");
	FFMpeg_setVideoCodec("libx264");
	FFMpeg_setVideoPreset("baseline");
	FFMpeg_setOther("crf", "24");
	FFMpeg_setFrameSize2(400, 300);
	FFMpeg_setStartTime2(40);
	//FFMpeg_setDisableAudio(1);
	FFMpeg_setFormat("mp4");
	FFMpeg_setOutputFile("saida2.mp4");
	FFMpeg_transcode();
	return 0;
}
