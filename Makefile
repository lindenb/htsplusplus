
CC = gcc
CPPFLAGS =
CFLAGS+= -g -O2
LIBS =
HTSDIR:=$(word 1,$(realpath ../htslib ../../htslib ${HOME}/src/htslib ${HOME}/packages/htslib ))


all:
	$(MAKE) -C src  HTSDIR=$(HTSDIR)

clean:
	$(MAKE) -C src clean

distclean: clean
	-rm -f config.log config.status configure Makefile
	-rm -rf autom4te.cache

.PHONY: all clean distclean
