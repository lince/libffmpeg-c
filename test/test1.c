#include <libffmpeg/libffmpeg.h>

int main() {
	FFMpeg_init();
	FFMpeg_setFrameSize1(800, 800);
	FFMpeg_setFramerate("25");
	FFMpeg_setFormat("x11grab");
	FFMpeg_setInputFile(":0.0");
	FFMpeg_setVideoCodec("libx264");
	FFMpeg_setVideoPreset("ultrafast");
	FFMpeg_setOther("crf", "24");
	FFMpeg_setFormat("mpegts");
	FFMpeg_setOutputFile("udp://127.0.0.1:1234");
	FFMpeg_transcode();
	while(1) {
		sleep(1000);
	}
}
