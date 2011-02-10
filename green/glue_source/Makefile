CC = gcc

all: intf ints intx xlog

intf: intf.o
	gcc intf.o -o intf

ints: ints.o rcfile.o
	gcc ints.o rcfile.o -o ints

ints.o: ints.c rcfile.h port.h

intx: intx.o rcfile.o
	gcc intx.o rcfile.o -o intx

intx.o: intx.c rcfile.h port.h

xlog: xlog.o rcfile.o
	gcc -g  xlog.o rcfile.o -o xlog

xlog.o: xlog.c rcfile.h
	gcc -c -g xlog.c
rcfile.o: rcfile.c rcfile.h port.h

clean:
	rm -f *.o intf intx ints xlog
