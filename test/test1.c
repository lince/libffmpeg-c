#include "../include/libffmpeg.h"

int main() {
	FFMpeg_init();

	FFMpeg_setFramerate("25");
	FFMpeg_setFormat("mp4");
	FFMpeg_setInputFile("sintel_trailer-720p.mp4");
	FFMpeg_setVideoCodec("libx264");
	FFMpeg_setVideoPreset("ultrafast");
	FFMpeg_setOther("crf", "24");
	FFMpeg_setFrameSize2(400, 300);
	FFMpeg_setFormat("mp4");
	FFMpeg_setOutputFile("saida1.mp4");
	FFMpeg_transcode();

	/*//while(10) {
		sleep(10);
	//}
	FFMpeg_stop();*/

	FFMpeg_reset();
	FFMpeg_setFrameSize2(800, 800);
	FFMpeg_setFramerate("25");
	FFMpeg_setFormat("x11grab");
	FFMpeg_setInputFile(":0.0");
	FFMpeg_setVideoCodec("libx264");
	FFMpeg_setVideoPreset("ultrafast");
	FFMpeg_setOther("crf", "24");
	FFMpeg_setFormat("mp4");
	FFMpeg_setOutputFile("saida2.mp4");
	FFMpeg_transcode();

	//while(10) {
		sleep(10);
	//}

	return 0;
}
