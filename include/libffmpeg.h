/*
 * libffmpeg.h
 *
 *  Created on: Sep 29, 2011
 *      Author: caioviel
 */

#define FFMpeg_SUCCESS 		0
#define FFMpeg_ERROR 	   -1

#ifndef LIBFFMPEG_H
#define LIBFFMPEG_H

#ifdef __cplusplus

extern "C" {

/* Main methods */
int FFMpeg_init(int verboseLvl);

int FFMpeg_transcode();

void FFMpeg_reset();

int FFMpeg_getErrorNumber();

void FFMpeg_getErrorStr(char* retValue);

void FFMpeg_stop();

/* General Options Methods */
int FFMpeg_setFormat (char* arg);

int FFMpeg_setInputFile(const char* filename);

int FFMpeg_setOverwriteFile(int boolean);

//TODO: Revisar essas chamadas de map
int FFMpeg_setMap(char* arg);

int FFMpeg_setMapMedata(char* arg);

int FFMpeg_setMapChapters(char* arg);

int FFMpeg_setRecordingTime1(const char* arg);

int FFMpeg_setRecordingTime2(long int time);

int FFMpeg_setLimiteSize(unsigned long int maxBytesSize);

int FFMpeg_setStartTime1(char* time);

int FFMpeg_setStartTime2(unsigned long int time);

int FFMpeg_setInputTSOffset1(char* time);

int FFMpeg_setInputTSOffset2(unsigned long int time);

int FFMpeg_setInputTSScale(const char* arg);

int FFMpeg_setRecordingTimestamp1(char* time);

int FFMpeg_setRecordingTimestamp2(unsigned long int time);

int FFMpeg_setMetadata1(char* arg);

int FFMpeg_setMetadata2(char* key, char* value);

int FFMpeg_setDataFramesToRecord(int value);

int FFMpeg_setDoBenchMark(int boolean);

int FFMpeg_setTimeLimit(long int time);

int FFMpeg_setDoPacketDump(int boolean);

int FFMpeg_setDoHexDump(int boolean);

int FFMpeg_setReadAtNativeRate(int boolean);

int FFMpeg_setLoopInput(int boolean);

int FFMpeg_setLoopOutputNumber(int value);

int FFMpeg_setVerbose(int verboseLevel);

int FFMpeg_setTarget(char* arg);

int FFMpeg_setThreadCount(int number);

int FFMpeg_setVideoSyncMethod(int number);

int FFMpeg_setAudioSyncMethod(int number);

int FFMpeg_setAudioDriftThreshould(float number);

int FFMpeg_setCopyTimestamp(int boolean);

int FFMpeg_setCopyTimeBase(int boolean);

int FFMpeg_setShortestInput(int boolean);

int FFMpeg_setDTSDeltaThreshould(float boolean);

int FFMpeg_setProgramId(int number);

int FFMpeg_setCopyInitialNonKeyframes(int boolean);

/* Video Options Methods */

int FFMpeg_setVideoBitrate(char* arg);

int FFMPeg_setVideoMaxFrames(int number);

int FFMpeg_setFramerate(char* arg);

//TODO: by a integer number.
//int FFMpeg_setFramerate2(int framerate);

int FFMpeg_setFrameSize1(char* arg);

int FFMpeg_setFrameSize2(int width, int height);

int FFMpeg_setAspectRatio(char* arg);

int FFMpeg_setPixelFormat(char* arg);

int FFMPeg_setBitsPerRawSample(int number);

//TODO: verificar a possibilidade de voltar utilizando os filtros
//int FFMpeg_setFrameCrop(int top, int bottom, int left, int right);
//int FFMpeg_setFramePad(int top, int bottom, int left, int right);

int FFMpeg_setIntraOnly(int boolean);

int FFMpeg_setDisableVideo(int boolean);

int FFMpeg_setVideoDiscard(int boolean);

int FFMpeg_setVideoQScale(float qscale);

int FFMpeg_setVideoRCOverrideString (char* arg);

int FFMpeg_setVideoCodec(char* arg);

int FFMpeg_setMeThreshold (int number);

int FFMpeg_setVideoSameQuality(int boolean);

int FFMpeg_setPassNumber(int number);

int FFMPeg_setPassLogFile(char* arg);

int FFMpeg_setDoDeinterlace(int boolean);

int FFMpeg_setDoPSNR(int boolean);

int FFMpeg_setVideoCodingStatusFile1 ();

int FFMpeg_setVideoCodingStatusFile2 (char* arg);

int FFMpeg_setVideoFilters(char* arg);

int FFMpeg_setInterMatrix(char* arg);

int FFMpeg_setIntraMatrix(char* arg);

int FFMpeg_setTopFieldFirst(char* arg);

int FFMpeg_setIntraDCPrecision(int number);

int FFMpeg_setVideoTag(char* arg);

int FFMpeg_newVideoStream();

int FFMpeg_setVideoLanguage(char* arg);

int FFMpeg_setQPHistogram(int boolean);

int FFMpeg_setForceFPS(int boolean);

// streamIndex:value
int FFMpeg_setStreamId1(char* arg);

int FFMpeg_setStreamId2(int streamIndex, int value);

int FFMpeg_setForceKeyFrame(char* arg);

/* Audio Options Methods */

int FFMpeg_setAudioBitrate(char* arg);

int FFMPeg_setAudioMaxFrames(int number);

int FFMpeg_setAudioQScale(float number);

int FFMpeg_setAudioRate(int arate);

int FFMpeg_setAudioChannels(int achannel);

int FFMpeg_setDisableAudio(int boolean);

int FFMpeg_setAudioCodec(char* arg);

int FFMpeg_setAudioTag(char* arg);

//change audio volume (256=normal)
int FFMpeg_setAudioVolume(int volume);

int FFMpeg_newAudioStream();

int FFMpeg_setAudioLanguage(char* arg);

int FFMpeg_setAudioSampleFormat(char* arg);

/* Subtitle Options Methods */

int FFMpeg_setDisableSubtitle(int boolean);

int FFMpeg_setSubtitleCodec(char* arg);

int FFMpeg_newSubtitleStream();

int FFMpeg_setSubtitleLanguage(char* arg);

int FFMpeg_setSubtitleTag(char* arg);

/* Grab Options Methods */

int FFMpeg_setVideoChannel(int vchannel);

int FFMpeg_setVideoStandard(char* arg);

int FFMpeg_setInputSync(int boolean);

/* Muxer Options Methods */

int FFMpeg_setMuxerMaxDelay(float number);

int FFMpeg_setMuxerMaxPreload(float number);

int FFMpeg_setVideoBitstreamFilter(char* arg);

int FFMpeg_setAudioBitstreamFilter(char* arg);

int FFMpeg_setSubtitleBitstreamFilter(char* arg);

int FFMpeg_setAudioPreset(char* arg);

int FFMpeg_setVideoPreset(char* arg);

int FFMpeg_setSubtitlePreset(char* arg);

int FFMpeg_setFilePreset(char* arg);

/* Data Options Methods */

int FFMpeg_setDataCodec(char* arg);

int FFMpeg_setPadColor(char* arg);

/* Others Options Methods */

int FFMpeg_setOther(char* opt, char* arg);

int FFMpeg_setOutputFile(char* filename);


//void* FFMpeg_getCurrentFrameFromInput(int input);

//void FFMpeg_enableGrabVideoFrame(int bool);



}

#endif

#endif /* LIBFFMPEG_H */
