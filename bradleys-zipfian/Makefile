CFLAGS=-std=c99 -Wall -W -Werror -Wwrite-strings -Wmissing-prototypes -g -O2
LDLIBS=-lm
default: ztest
ztest: ztest.o zipf.o
ztest.o zipf.o: zipf.h

tarball: bradleys-zipfian.tar.gz
bradleys-zipfian.tar.gz: Makefile zipf.c zipf.h ztest.c
	ln -s . bradleys-zipfian
	tar czf $@ $(patsubst %, bradleys-zipfian/%, $^)
	rm bradleys-zipfian
