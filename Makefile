CC=gcc
HEADES_DIR= /usr/local/include/libffmpeg
LIB_DIR= /usr/local/lib

INCLUDES=	include/cmdutils.h \
			include/libffmpeg.h 
			
SOURCES=	src/cmdutils.c \
			src/libffmpeg.c
			
LIBS_OLD= 		-lavcodec -lavformat -lavdevice \
			-lavutil -lswscale -lpostproc \
			-lavfilter -Llibpostproc

LIBS= 		/usr/local/lib/libavcodec.so.53 \
		/usr/local/lib/libavformat.so.53 \
		/usr/local/lib/libavdevice.so.53 \
		/usr/local/lib/libavutil.so.51 \
		/usr/local/lib/libswscale.so.2 \
		/usr/local/lib/libpostproc.so.51 \
		/usr/local/lib/libavfilter.so.2

ALL: libffmpeg.so

libffmpeg.so: $(INCLUDES) $(SOURCES)
	$(CC) $(INCLUDES) $(SOURCES) \
	-D__STDC_CONSTANT_MACROS -I./deps/ffmpeg -shared $(LIBS) -g \
	-o libffmpeg.so
	
clean:
	rm -f *.o libffmpeg.so

install: libffmpeg.so $(INCLUDES)
	install -d $(HEADES_DIR)
	install -t $(HEADES_DIR) include/libffmpeg.h
	install -t $(LIB_DIR) libffmpeg.so
