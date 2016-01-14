#
# Makefile for kyouko3
#
# vim: set noexpandtab:

obj-m += kyouko3.o

all: module useKyouko3

useKyouko3:  useKyouko3.o
	${CC} -o $@ $^

module:
	${MAKE} -C ../linux-4.3.3 M=${PWD} modules

clean:
	rm *.ks *.s *.mod.c

tags:
	ctags -R
