PNAME=jpegquant
SRCS=jpegquant.c
CC=gcc
CFLAGS=-Wall
LIBS=-ljpeg
PREFIX?=/usr/local
DATAPREFIX=$(PREFIX)/share
INSTALL=install
RM=rm -fv

all: jpegquant

jpegquant: $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -s

mozjpegquant: jpegquant.c
	$(CC) $(CFLAGS) -I/usr/include/mozjpeg -o $@ $^ -lmozjpeg -s

clean:
	$(RM) jpegquant mozjpegquant

install: $(PNAME)
	$(INSTALL) -d $(PREFIX)/bin
	$(INSTALL) -m 0755 $(PNAME) $(PREFIX)/bin/
	$(INSTALL) -d $(DATAPREFIX)
	$(INSTALL) -d $(DATAPREFIX)/man/man1
	$(INSTALL) -m 0644 man/man1/$(PNAME).1 $(DATAPREFIX)/man/man1
	$(INSTALL) -d $(DATAPREFIX)/doc/$(PNAME)
	$(INSTALL) -m 0644 LICENSE README.md $(DATAPREFIX)/doc/$(PNAME)
