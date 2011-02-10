.SUFFIXES:.o .s .c .h .run
.c.o:
	$(CC) -c $(CFLAGS) $*.c

CC = gcc
LINT = lint
CFLAGS = -g -w

SUNFLAGS= -lsuntool -lsunwindow -lpixrect
LFLAGS= $(CFLAGS) -lcurses -ltermcap

#Common objects
OBJECTS= alloc.o command.o compare.o cursor.o cuts.o display.o\
	dump.o edit.o endsets.o fesun.o fetake.o figures.o find.o\
	findlinks.o follow.o freealloc.o getspecset.o getusrreq.o\
	init.o inittask.o initterm.o insertdel.o interact.o links.o\
	list.o mask.o menu.o  menuwindow.o minxface.o\
	nextl.o parseesc.o screen.o scroll.o\
	select.o sendbot.o sendtop.o stack.o sunscroll.o\
	terminal.o termlow.o tumadd.o tumble.o tumjust.o tumtest.o\
	typeahead.o usefull.o uxf.o version.o vm.o vmedit.o vmlist.o\
	vmreap.o weirdoptions.o window.o
# Common includes
INCLUDES= defs.h feconfig.h fest.h fetool.h festbot.h requests.h\
	feminreq.h fekeys.h

SOURCES= alloc.c command.c compare.c cursor.c cuts.c display.c\
	dump.c edit.c endsets.c fesun.c fetake.c figures.c find.c\
	findlinks.c follow.c freealloc.c getspecset.c getusrreq.c\
	init.c inittask.c initterm.c insertdel.c interact.c links.c\
	list.c mask.c menu.c  menuwindow.c minxface.c\
	nextl.c parseesc.c screen.c scroll.c\
	select.c sendbot.c sendtop.c stack.c sunscroll.c\
	terminal.c termlow.c tumadd.c tumble.c tumjust.c tumtest.c\
	typeahead.c usefull.c uxf.c version.c vm.c vmedit.c vmlist.c\
	vmreap.c weirdoptions.c window.c

all: fex 


LIB = 		/usr/lib
LIB_DEPEND =	$(LIB)/maccrt0.o $(LIB)/low.o $(LIB)/libmac.a /lib/libc.a $(LIB)/low.ld

LIB1 =		$(LIB)/maccrt0.o
LIB2 =		$(LIB)/low.o -lmac -lc $(LIB)/low.ld
MACOBJECTS= mainmac.o macglue.o bert.o  nowindowcur.o putf.o sun.o $(OBJECTS)

fet: $(INCLUDES) $(FETOBJECTS)
	$(CC) $(FETOBJECTS) $(LFLAGS) -o fet

femac: $(INCLUDES) macinclude.h $(MACOBJECTS)
	ld -o femac  $(LIB1)  $(MACOBJECTS) -lm -ltermcap $(LIB2) 

FEXINCLUDES= klugeddcls.h fetypealias.h $(INCLUDES)
FEXOBJECTS= main.o bert.o nowindowcur.o putf.o sun.o $(OBJECTS)
FEXSOURCES= main.c bert.c nowindowcur.c putf.c sun.c $(SOURCES)
FEXDSOURCES= main.d bert.d nowindowcur.d putf.d sun.d $(DSOURCES)

fex: $(FEXINCLUDES) $(FEXOBJECTS)
	$(CC) $(FEXOBJECTS) $(LFLAGS) -o fex

lint: fexlint

fexlint: $(FEXINCLUDES) $(FEXSOURCES)
	$(LINT)  $(FEXSOURCES) > zzlintout
	cp zzlintout zzlintout.saved

SFEOBJECTS= mainsfe.o bert.o putf.o sun.o nowindowcur.o $(OBJECTS)
sfe: $(INCLUDES) $(SFEOBJECTS)
	$(CC) $(SFEOBJECTS) $(LFLAGS) -o sfe

tags: $(FEXINCLUDES) $(FEXSOURCES)
	ctags $(FEXINCLUDES) $(FEXSOURCES)

fetlint: $(FETINCLUDES) $(FETOBJECTS)
	$(LINT) -hxba $(FETINCLUDES) $(FETOBJECTS) > zzlintout


c: feconfig.h fest.h fetool.h festbot.h requests.h feminreq.h\
	fekeys.h init.c interact.c links.c findlinks.c scroll.c\
	getspecset.c getusrreq.c typeahead.c screen.c display.c\
	inittask.c tumble.c sendbot.c fetake.c alloc.c freealloc.c\
	vm.c vmedit.c list.c minxface.c usefull.c terminal.c\
	termlow.c dump.c uxf.c mask.c edit.c window.c initterm.c\
	cuts.c cursor.c command.c insertdel.c parseesc.c tumadd.c\
	tumtest.c tumjust.c nextl.c vmlist.c vmreap.c sendtop.c\
	endsets.c select.c menuwindow.c menu.c \
	figures.c fesun.c sunscroll.c windowputf.c version.c\
	compare.c stack.c weirdoptions.c find.c follow.c
	mv *.c test
	cp *.h test


sfeinit: sfeinit.o
	$(CC) sfeinit.o -o sfeinit
tipsfe:	tipsfe.o
	$(CC) tipsfe.o -o tipsfe
clean:
	rm -f *.o core symtab zz* fet fex sfe
love:
	@echo "  Not war?"
war:
	@echo "  Not Love?"
out:
	@echo "  With whom?"


