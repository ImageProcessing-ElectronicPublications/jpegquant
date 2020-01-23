PNAME=jpegquant
PROGS=$(PNAME) jpegcstat jpegcshrink
SRCS=src
CC=gcc
CFLAGS=-Wall
LIBS=-ljpeg
PREFIX?=/usr/local
DATAPREFIX=$(PREFIX)/share
INSTALL=install
RM=rm -fv

all: $(PROGS)

jpegquant: $(SRCS)/jpegquant.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -s

jpegcstat: $(SRCS)/jpegcstat.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -s

jpegcshrink: $(SRCS)/jpegcshrink.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -s

mozjpegquant: $(SRCS)/jpegquant.c
	$(CC) $(CFLAGS) -I/usr/include/mozjpeg -o $@ $^ -lmozjpeg -s

clean:
	$(RM) $(PROGS)

install: $(PROGS)
	$(INSTALL) -d $(PREFIX)/bin
	$(INSTALL) -m 0755 $(PROGS) $(PREFIX)/bin/
	$(INSTALL) -d $(DATAPREFIX)
	$(INSTALL) -d $(DATAPREFIX)/man/man1
	$(INSTALL) -m 0644 man/man1/$(PNAME).1 man/man1/jpegcstat.1 man/man1/jpegcshrink.1 $(DATAPREFIX)/man/man1
	$(INSTALL) -d $(DATAPREFIX)/doc/$(PNAME)
	$(INSTALL) -m 0644 LICENSE README.md $(DATAPREFIX)/doc/$(PNAME)
