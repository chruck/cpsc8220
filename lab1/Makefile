#
# Makefile for kyouko3
#
# vim: set noexpandtab:

LINUXSRC="/usr/src/linux"

obj-m += kyouko3.o

all: module useKyouko3

useKyouko3:  useKyouko3.o
	${CC} -o $@ $^

module:
	${MAKE} -C ${LINUXSRC} M=${PWD} modules

clean:
	${RM} *.ks *.s *.mod.c tags

tags:
	ctags -R * ${LINUXSRC}/*
