#include "config.h"
#include "libavformat/avformat.h"
#include "libavutil/mem.h"
#include <unistd.h>
#include <time.h>

/* meus includes*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdlib.h>
/* meus includes*/

#define SHMKEY 7101
#define SHMINFOKEY 4687
#define SEMKEY 1246

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
#else
  union semun {
    int val;                    /* valor para SETVAL */
     struct semid_ds *buf;       /* buffer para IPC_STAT, IPC_SET */
     unsigned short int *array;  /* vetor para GETALL, SETALL */
     struct seminfo *__buf;      /* buffer para IPC_INFO */
  };
#endif

#ifndef SHM_DEVICE_INFO
#define SHM_DEVICE_INFO
typedef struct {
	int height;
	int width;
	int bitsPerPixel;
	int fps;
	int shmkey;
	int semkey;
} DeviceInfo;
#endif //SHM_DEVICE_INFO

typedef struct shm_grab_s
{
    int frame_size;          /**< Size in bytes of a grabbed frame */
    AVRational time_base;    /**< Time base */
    int64_t time_frame;      /**< Current time */

    int height;              /**< Height of the grab frame */
    int width;               /**< Width of the grab frame */

	int shmid;				/**< Id of shared manager */
	int semid;				/**< Id of the semaphoros */
	int bytes_per_pixel;	/**< Number of bytes utilized per pixel*/

	void* shmdata;			/**< buffer where the image is */
} shm_grab_t;



static int
shmgrab_read_header(AVFormatContext *s1, AVFormatParameters *ap)
{
	shm_grab_t *s = s1->priv_data;
	AVStream *st = NULL;/**/
    int input_pixfmt, height, width, bits_per_pixel;/*atributos da imagem*/
	union semun sem_un;
	int shmsize = 0;
	int shmdif_info;
	DeviceInfo* info;

	/*obtem acesso ao buffer*/
    if(ap->time_base.den <= 0){
        av_log(s1, AV_LOG_ERROR, "AVParameters don't have video rate. Use -r.\n");
        return AVERROR(EIO);
    }
	st = av_new_stream(s1, 0);
    if (!st) {
        return AVERROR(ENOMEM);
    }
    av_set_pts_info(st, 64, 1, 1000000); /* 64 bits pts in us */

	/* Obtém de algum lugar as informações sobre tamanho do imagem */
	/*
	width = (int) fb_varinfo.xres;
    	height = (int) fb_varinfo.yres;
    	bits_per_pixel = (int) fb_varinfo.bits_per_pixel;
	*/

	/*Obtem a descrição do device*/
	if ((shmdif_info = shmget(
			SHMINFOKEY, shmsize, 0777|IPC_CREAT)) < 0) {
		av_log(s1, AV_LOG_ERROR, "Error trying to get the device format information.");
		return AVERROR(ENOMEM);
	}

	info = (DeviceInfo*) shmat(shmdif_info,0,0);

	width = info->width;
	height = info->height;
	bits_per_pixel = info->bitsPerPixel;

	/*cria a memória comaprtihlada*/
	shmsize = width*height*bits_per_pixel/8;
	if ((s->shmid = shmget(
			SHMKEY, shmsize, 0777|IPC_CREAT)) < 0) {
		av_log(s1, AV_LOG_ERROR, "Error trying to alocate the shared memory buffer.");
		return AVERROR(ENOMEM);
	}

	/*cria o e inicializa o semaforo com 1semafaro*/

	if((s->semid=semget(SEMKEY,1,0744|IPC_CREAT)) < 0) {
		av_log(s1, AV_LOG_ERROR, "Error trying to alocate the semaphoros.");
		return AVERROR(ENOMEM);
	}

	/* inicializa o semaforo*/
	sem_un.val = 1;
	semctl(s->semid,0,SETVAL,sem_un);

	av_log(s1, AV_LOG_INFO, "display width: %d height: %d\n", width, height);
	 switch(bits_per_pixel){
    case 8:
        av_log (s1, AV_LOG_DEBUG, "8 bit palette\n");
        input_pixfmt = PIX_FMT_PAL8;
        break;
    case 15:
        av_log (s1, AV_LOG_DEBUG, "16 bit BGR555\n");
        input_pixfmt = PIX_FMT_BGR555;
        break;
    case 16:
        av_log (s1, AV_LOG_DEBUG, "16 bit RGB565\n");
        input_pixfmt = PIX_FMT_RGB565;
        break;
    case 24:
        av_log (s1, AV_LOG_DEBUG, "24 bit RGB24\n");
        input_pixfmt = PIX_FMT_RGB24;
        break;
    case 32:
        av_log (s1, AV_LOG_DEBUG, "32 bit RGB32\n");
        input_pixfmt = PIX_FMT_RGB32;
        break;
    default:
        av_log(s1, AV_LOG_ERROR, "image depth %i not supported ... aborting\n", bits_per_pixel);
        return -1;
    }

    s->frame_size = width * height * bits_per_pixel/8;
    s->width = width;
    s->height = height;
    s->time_base  = ap->time_base;
    s->time_frame = av_gettime() / av_q2d(ap->time_base);

	/* Associa a variavel local com o buffer compartilhado */
    s->shmdata = (unsigned char *) shmat(s->shmid,0,0);

    if(s->shmdata == -1){
        av_log(s1, AV_LOG_ERROR, "Error trying to link the shared memory.\n");
        return AVERROR(ENOMEM);
    }

    st->codec->codec_type = AVMEDIA_TYPE_VIDEO;
    st->codec->codec_id = CODEC_ID_RAWVIDEO;
    st->codec->width = width;
    st->codec->height = height;
    st->codec->pix_fmt = input_pixfmt;
    st->codec->time_base = ap->time_base;
    st->codec->bit_rate = s->frame_size * 1/av_q2d(ap->time_base) * 8;

    return 0;
}

static int
shmgrab_read_packet(AVFormatContext *s1, AVPacket *pkt)
{
	shm_grab_t *s = s1->priv_data;
	struct sembuf ssbuf;

    int64_t curtime, delay;
    struct timespec ts;

    /* Calculate the time of the next frame */
    s->time_frame += INT64_C(1000000);

    /* wait based on the frame rate */
    for(;;) {
        curtime = av_gettime();
        delay = s->time_frame * av_q2d(s->time_base) - curtime;
        if (delay <= 0) {
            if (delay < INT64_C(-1000000) * av_q2d(s->time_base)) {
                s->time_frame += INT64_C(1000000);
            }
            break;
        }
        ts.tv_sec = delay / 1000000;
        ts.tv_nsec = (delay % 1000000) * 1000;
        nanosleep(&ts, NULL);
    }

    if (av_new_packet(pkt, s->frame_size) < 0)
        return AVERROR(EIO);

    pkt->pts = curtime;

    /* lock */
	ssbuf.sem_num=0;
	ssbuf.sem_op=-1;
	ssbuf.sem_flg=1;	// flag == 1 indica acesso bloqueante
	semop(s->semid,&ssbuf,1);

	/* XXX: avoid memcpy */
    	memcpy(pkt->data, s->shmdata, s->frame_size);

	 /* unlock */
	ssbuf.sem_num=0;
	ssbuf.sem_op=1;
	ssbuf.sem_flg=1;	// flag == 1 indica acesso bloqueante
	semop(s->semid,&ssbuf,1);

    return s->frame_size;
}

static int
shmgrab_read_close(AVFormatContext *s1)
{
	union semun sem_un;
	struct shmid_ds shmds;
	shm_grab_t *shmgrab = s1->priv_data;
	//printf("Saindo na shmgrab_read_close.");

	//semctl(shmgrab->semid,0,IPC_RMID,sem_un);
	//shmdt((char *) shmgrab->shmdata);
    //shmctl(shmgrab->shmid,IPC_RMID,&shmds);

    return 0;
}

/** fb grabber device demuxer declaration */
AVInputFormat ff_shm_grab_device_demuxer =
{
    "shmgrab",
    NULL_IF_CONFIG_SMALL("shmgrab"),
    sizeof(shm_grab_t),
    NULL,
    shmgrab_read_header,
    shmgrab_read_packet,
    shmgrab_read_close,
    .flags = AVFMT_NOFILE,
};
