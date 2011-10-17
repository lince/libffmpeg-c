/*
 * fbgrab - framebuffer video grab interface
 *
 * (C) Giliard B. de Freitas <giliarde@gmail.com> 2008
 *
 * fbgrab is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * fbgrab is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with fbgrab; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file fbgrab.c
 * framebuffer device demuxer by Giliard B. de Freitas <giliarde@gmail.com>
 */

#include "config.h"
#include "libavformat/avformat.h"
#include "libavutil/mem.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <time.h>
#include <linux/fb.h>
#define _LINUX_TIME_H 1
#define DEFAULT_FB "/dev/fb0"

/**
 * Framebuffer Device Demuxer context
 */
typedef struct fb_grab_s
{
    int frame_size;          /**< Size in bytes of a grabbed frame */
    AVRational time_base;    /**< Time base */
    int64_t time_frame;      /**< Current time */

    int height;              /**< Height of the grab frame */
    int width;               /**< Width of the grab frame */
    
    int fd;                  /**< Framebuffer device file descriptor */

    unsigned char *data;     /**< Image data */
} fb_grab_t;

/**
 * Initializes the fb grab device demuxer (public device demuxer API).
 *
 * @param s1 Context from avformat core
 * @param ap Parameters from avformat core
 * @return <ul>
 *          <li>AVERROR(ENOMEM) no memory left</li>
 *          <li>AVERROR(EIO) other failure case</li>
 *          <li>0 success</li>
 *         </ul>
 */
static int
fbgrab_read_header(AVFormatContext *s1, AVFormatParameters *ap)
{
    fb_grab_t *fbgrab = s1->priv_data;/*Void pointer com algum conteúdo*/
    struct fb_var_screeninfo fb_varinfo;
    AVStream *st = NULL;/**/
    int input_pixfmt, height, width, bits_per_pixel;/*atributos da imagem*/
    char *device;
    
    memset(&fb_varinfo, 0, sizeof(struct fb_var_screeninfo));/*Adiciona um número de zeros equivalentes ao tamanho da estrutra na estrutura recém declarada*/

    device = getenv("FRAMEBUFFER");/*Obtém a localização do framebuffer.*/
    if (device == NULL){
        device = DEFAULT_FB;/*Caso não encontrar, utiliza o padrão do Linux. ( /dev/fb0 )*/
    }

    if((fbgrab->fd=open(device, O_RDONLY)) == -1){
        av_log(s1, AV_LOG_ERROR, "Couldn't open framebuffer device.\n");
        return AVERROR(EIO);
    }

    if (ioctl(fbgrab->fd, FBIOGET_VSCREENINFO, &fb_varinfo) != 0){
        av_log(s1, AV_LOG_ERROR, "ioctl FBIOGET_VSCREENINFO.\n");
        return AVERROR(EIO);
    }

    if(ap->time_base.den <= 0){
        av_log(s1, AV_LOG_ERROR, "AVParameters don't have video rate. Use -r.\n");
        return AVERROR(EIO);
    }

    st = av_new_stream(s1, 0);
    if (!st) {
        return AVERROR(ENOMEM);
    }
    av_set_pts_info(st, 64, 1, 1000000); /* 64 bits pts in us */

    width = (int) fb_varinfo.xres;
    height = (int) fb_varinfo.yres;
    bits_per_pixel = (int) fb_varinfo.bits_per_pixel;
    
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
        av_log (s1, AV_LOG_DEBUG, "16 bit RGB24\n");
        input_pixfmt = PIX_FMT_RGB24;
        break;
    case 32:
        av_log (s1, AV_LOG_DEBUG, "16 bit RGB32\n");
        input_pixfmt = PIX_FMT_RGB32;
        break;
    default:
        av_log(s1, AV_LOG_ERROR, "image depth %i not supported ... aborting\n", bits_per_pixel);
        return -1;
    }

    fbgrab->frame_size = width * height * bits_per_pixel/8;
    fbgrab->width = width;
    fbgrab->height = height;
    fbgrab->time_base  = ap->time_base;
    fbgrab->time_frame = av_gettime() / av_q2d(ap->time_base);
    fbgrab->data = (unsigned char *) mmap(0, fbgrab->frame_size, PROT_READ, MAP_SHARED, fbgrab->fd, 0);
    
    if(fbgrab->data == NULL){
        av_log(s1, AV_LOG_ERROR, "Not enough memory.\n");
        return AVERROR(ENOMEM);
    }

    st->codec->codec_type = CODEC_TYPE_VIDEO;
    st->codec->codec_id = CODEC_ID_RAWVIDEO;
    st->codec->width = width;
    st->codec->height = height;
    st->codec->pix_fmt = input_pixfmt;
    st->codec->time_base = ap->time_base;
    st->codec->bit_rate = fbgrab->frame_size * 1/av_q2d(ap->time_base) * 8;

    return 0;
}

/**
 * Grabs a frame from fb (public device demuxer API).
 *
 * @param s1 Context from avformat core
 * @param pkt Packet holding the brabbed frame
 * @return frame size in bytes
 */
static int
fbgrab_read_packet(AVFormatContext *s1, AVPacket *pkt)
{
    fb_grab_t *s = s1->priv_data;

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

    /* XXX: avoid memcpy */
    memcpy(pkt->data, s->data, s->frame_size);
    return s->frame_size;
}

/**
 * Closes framebuffer grabber (public device demuxer API).
 *
 * @param s1 Context from avformat core
 * @return 0 success, !0 failure
 */
static int
fbgrab_read_close(AVFormatContext *s1)
{
    fb_grab_t *fbgrab = s1->priv_data;

    munmap(fbgrab->data, fbgrab->frame_size);
    
    close(fbgrab->fd);
    
    return 0;
}

/** fb grabber device demuxer declaration */
AVInputFormat fb_grab_device_demuxer =
{
    "fbgrab",
    NULL_IF_CONFIG_SMALL("FBgrab"),
    sizeof(fb_grab_t),
    NULL,
    fbgrab_read_header,
    fbgrab_read_packet,
    fbgrab_read_close,
    .flags = AVFMT_NOFILE,
};
