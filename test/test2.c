#include <libffmpeg/libffmpeg.h>

int main() {
	FFMpeg_init();
	FFMpeg_setFormat("avi");
	FFMpeg_setInputFile("novo.avi");
	FFMpeg_setVideoCodec("libx264");
	FFMpeg_setAudioCodec("copy");
	FFMpeg_setVideoPreset("slow");
	FFMpeg_setOther("crf", "24");
	FFMpeg_setFormat("mp4");
	FFMpeg_setOutputFile("saida.mp4");
	FFMpeg_transcode();
	while(1) {
		sleep(1000);
	}
}
