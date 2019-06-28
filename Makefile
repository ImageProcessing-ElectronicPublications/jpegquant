PNAME=jpegquant
SRCS=jpegquant.c
CFLAGS=-Wall
LIBS=-ljpeg
CC=gcc

all: jpegquant

jpegquant: $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -s

mozjpegquant: jpegquant.c
	$(CC) $(CFLAGS) -I/usr/include/mozjpeg -o $@ $^ -lmozjpeg -s

clean:
	rm -fv jpegquant mozjpegquant
