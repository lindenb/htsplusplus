
CC = gcc
CPPFLAGS = 
CFLAGS = -g -O2
LDFLAGS = 
LIBS = 

HTSLIB_CPPFLAGS = -I/home/lindenb/src/htsplusplus/../htslib
HTSLIB_LDFLAGS = -L/home/lindenb/src/htsplusplus/../htslib
HTSDIR = /home/lindenb/src/htsplusplus/../htslib


all:
	$(MAKE) -C src  HTSLIB_CPPFLAGS=$(HTSLIB_CPPFLAGS) HTSLIB_LDFLAGS=$(HTSLIB_LDFLAGS) HTSDIR=$(HTSDIR)

clean:
	$(MAKE) -C src clean

distclean: clean
	-rm -f config.log config.status configure Makefile
	-rm -rf autom4te.cache

.PHONY: all clean distclean
