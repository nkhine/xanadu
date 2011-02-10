#!/bin/make
# Modern make file by Hugh Daniel 87/4/13
#  This is usefull "alias go 'time  make !* >& zzerrs ; beep &'"
.SUFFIXES:
.SUFFIXES:.o .s .c .h .run
.c.o:
	$(CC)  -c $(CFLAGS) $*.c
.c.sun:
	cc $*.c -o $* $(CFLAGS) $(SUNFLAGS)
.c.core:
	cc $*.c -o $* $(CFLAGS) -lcore $(SUNFALGS)
.s.o:
	as $*.s

CC=  cc
LINT=  lint

SUNFLAGS= -lsuntool -lsunwindow -lpixrect
# CFLAGS=  -g -lm -I 
# LFLAGS= $(CFLAGS) -ltermcap
# LFLAGS= $(CFLAGS) -lcur5.0 -ltermcap
LFLAGS= $(CFLAGS) -lcurses -ltermcap

#Common objects


SOURCES=alloc.c  findlinks.c minxface.c tumble.c vmedit.c bert.c  freealloc.c misc.c  tumjust.c vmlist.c dump.c  grabspec.c sendbot.c tumtest.c vmreap.c felibtop.c inittask.c sendtop.c usefull.c fetake.c list.c  tumadd.c vm.c

OBJECTS=alloc.o  findlinks.o minxface.o tumble.o vmedit.o bert.o  freealloc.o misc.o  tumjust.o vmlist.o dump.o  grabspec.o sendbot.o tumtest.o vmreap.o felibtop.o inittask.o sendtop.o usefull.o fetake.o list.o  tumadd.o vm.o

all: fen

fen: main.o init.o $(OBJECTS)
	$(CC) main.o init.o $(LFLAGS) $(OBJECTS) -o fen
