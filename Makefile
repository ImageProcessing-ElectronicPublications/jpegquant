PNAME=jpegquant
PROGS=$(PNAME) jpegquarel jpegcstat jpegcshrink
SRCS=src
CC=gcc
CFLAGS=-Wall -Isrc
ifeq ($(OS),Windows_NT)
  LIBS=libjpeg-8.dll
else
  LIBS= -ljpeg
endif
PREFIX?=/usr/local
DATAPREFIX=$(PREFIX)/share
INSTALL=install
RM=rm -fv

all: $(PROGS)

jpegquant: $(SRCS)/jpegquant.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -s

jpegquarel: $(SRCS)/jpegquarel.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -s

jpegcstat: $(SRCS)/jpegcstat.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -s

jpegcshrink: $(SRCS)/jpegcshrink.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -s

clean:
	$(RM) $(PROGS)

install: $(PROGS)
	$(INSTALL) -d $(PREFIX)/bin
	$(INSTALL) -m 0755 $(PROGS) $(PREFIX)/bin/
	$(INSTALL) -d $(DATAPREFIX)
	$(INSTALL) -d $(DATAPREFIX)/man/man1
	$(INSTALL) -m 0644 man/man1/$(PNAME).1 man/man1/jpegquarel.1 man/man1/jpegcstat.1 man/man1/jpegcshrink.1 $(DATAPREFIX)/man/man1
	$(INSTALL) -d $(DATAPREFIX)/doc/$(PNAME)
	$(INSTALL) -m 0644 LICENSE README.md $(DATAPREFIX)/doc/$(PNAME)
