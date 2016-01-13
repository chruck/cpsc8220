#
# Makefile for kyouko3
#

obj-m += kyouko3.o

default:
	${MAKE} -C /usr/src/linux M=${PWD} modules

clean:
	rm *.ks *.s *.mod.c
