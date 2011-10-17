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

LIBS= 		/usr/local/lib/libavcodec.so \
		/usr/local/lib/libavformat.so \
		/usr/local/lib/libavdevice.so \
		/usr/local/lib/libavutil.so \
		/usr/local/lib/libswscale.so \
		/usr/local/lib/libpostproc.so \
		/usr/local/lib/libavfilter.so \
		/usr/local/lib/libpostproc.so

ALL: libffmpeg.so

libffmpeg.so: $(INCLUDES) $(SOURCES)
	$(CC) $(INCLUDES) $(SOURCES) \
	-D__STDC_CONSTANT_MACROS -fpic -shared $(LIBS) -g \
	-o libffmpeg.so
	
clean:
	rm -f *.o libffmpeg.so

install: libffmpeg.so $(INCLUDES)
	install -d $(HEADES_DIR)
	install -t $(HEADES_DIR) include/libffmpeg.h
	install -t $(LIB_DIR) libffmpeg.so
