#ifndef LIBFFMPEG_H
#define LIBFFMPEG_H

#ifdef __cplusplus

extern "C" {

void FFMpeg_init();

void FFMpeg_reset();

void FFMpeg_stop();

void FFMpeg_relase();

int FFMpeg_transcode();

void FFMpeg_setFormat (char* arg);

void FFMpeg_setOverrideString (char* arg);

void FFMpeg_setMeThreshold (int arg);

void FFMpeg_setVerbose(int verboseLevel);

void FFMpeg_setFramerate(char* arg);

void FFMpeg_setAudioBitrate(char* arg);

void FFMpeg_setVideoBitrate(char* arg);

void FFMpeg_setFrameCrop(int top, int bottom, int left, int right);

void FFMpeg_setFrameSize(char* arg);

void FFMpeg_setFrameSize1(int width, int height);

int FFMpeg_setPadColor(char* arg);

int FFMpeg_setFramePad(int top, int bottom, int left, int right);

void FFMpeg_setFramePixelFormat(char* arg);

void FFMpeg_setFrameAspectRatio(char* arg);

void FFMpeg_setMetadata(char* arg);

void FFMpeg_setMetadata1(char* key, char* value);

void FFMpeg_setQScale(float qscale);

void FFMpeg_setTopFieldFirst(int topFiledFirst);

void FFMpeg_setThreadCount(int number);

void FFMpeg_setAudioSampleFormat(char* arg);

void FFMpeg_setAudioRate(int arate);

void FFMpeg_setAudioChannels(int achannel);

void FFMpeg_setVideoChannel(int vchannel);

void FFMpeg_setVideoStandard(char* arg);

void FFMpeg_setAudioCodec(char* arg);

void FFMpeg_setAudioTag(char* arg);

void FFMpeg_setVideoTag(char* arg);

void FFMpeg_setVideoCodec(char* arg);

void FFMpeg_setSubtitleCodec(char* arg);

void FFMpeg_setSubtitleTag(char* arg);

void FFMpeg_setMap(char* arg);

void FFMpeg_setMapMetaData(char* arg);

void FFMpeg_setInputTSScale(char* arg);

void FFMpeg_setRecordingTime(char* time);

void FFMpeg_setStartTime(char* time);

void FFMpeg_setRecTimestamp(char* time);

void FFMpeg_setInputTSOffset(char* time);

void FFMpeg_setInputFile(char* filename);

void FFMpeg_newAudioStream();

void FFMpeg_newVideoStream();

void FFMpeg_newSubtitleStream();

void FFMpeg_setOutputFile(char* filename);

void FFMpeg_setPass(int number);

void FFMpeg_setInterMatrix(char* arg);

void FFMpeg_setIntraMatrix(char* arg);

//void FFMpeg_setTarget(char* arg);

void FFMpeg_setVStatsFile(char* arg);

void FFMpeg_setVStats();

void FFMpeg_setVideoBsf(char* arg);

void FFMpeg_setAudioBsf(char* arg);

void FFMpeg_setSubtitleBsf(char* arg);

void FFMpeg_setAudioPreset(char* arg);

void FFMpeg_setVideoPreset(char* arg);

void FFMpeg_setSubtitlePreset(char* arg);

void FFMpeg_setEnableVideo(int enableVideo);

void FFMpeg_setEnableAudio(int enableAudio);

void FFMpeg_setEnableSubtitle(int enableSubtitle);

void FFMpeg_setOther(char* opt, char* arg);

void* FFMpeg_getCurrentFrameFromInput(int input);

void FFMpeg_enableGrabVideoFrame(int boolean);

void FFMPeg_setFileOverwrite(int boolean);

/* arg format is "output-stream-index:streamid-value". */
void FFMpeg_setStreamId(char* arg);

}

#endif

#endif /* LIBFFMPEG_H */
