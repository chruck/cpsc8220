#
# Makefile for kyouko3
#
# vim: set noexpandtab:

obj-m += kyouko3.o

all: module kyouko3Ramsize

kyouko3Ramsize:  kyouko3Ramsize.o
	${CC} -o $@ $^

module:
	${MAKE} -C /usr/src/linux M=${PWD} modules

clean:
	${RM} *.ks *.s *.mod.c tags

tags:
	ctags -R * ../../linux/*
