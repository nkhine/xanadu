CC = cc                         # select gcc if you like

CFLAGS = -pg -g
#CFLAGS = -w
# CFLAGS = -cckr                # use this line for Irix

LFLAGS = -g -pg
# LFLAGS = -lsocket -lnsl       # use this line for SunOS ? solaris?
#LFLAGS = -g -Bstatic -pg -L/usr/local/share

.c.o: 
	$(CC) $(CFLAGS) -c  $*.c

HINCLUDES = common.h players.h defs.h tags.h requests.h enf.h wisp.h crum.h \
	    xanadu.h coredisk.h ndenf.h alloc.h reap.h queues.h

COMMONCSOURCES = alloc.c allocdebug.c bert.c  context.c corediskin.c \
	corediskout.c correspond.c credel.c disk.c diskalloc.c do1.c do2.c \
	edit.c entexit.c fns.c genf.c granf1.c granf2.c init.c insert.c \
	insertnd.c makeroom.c multiloaf.c ndcuts.c ndinters.c orglinks.c \
	recombine.c retrie.c spanf1.c spanf2.c split.c sporgl.c task.c \
	test.c tumble.c tumbleari.c usefull.c wisp.c queues.c rcfile.c

COMMONOBJECTS = alloc.o allocdebug.o bert.o  context.o corediskin.o \
	corediskout.o correspond.o credel.o disk.o diskalloc.o do1.o do2.o \
	edit.o entexit.o fns.o genf.o granf1.o granf2.o init.o insert.o \
	insertnd.o makeroom.o multiloaf.o ndcuts.o ndinters.o orglinks.o \
	recombine.o retrie.o spanf1.o spanf2.o split.o sporgl.o task.o \
	test.o tumble.o tumbleari.o usefull.o wisp.o queues.o rcfile.o

#all: xumain backend backenddaemon intf ints xlog intx mycat
all: xumain backend backenddaemon

clean: 
	rm -f *.o xumain backend backenddaemon backenderror ln[0-9]*

bed: backenddaemon 

be: backend

xumain: $(HINCLUDES) $(COMMONOBJECTS) xumain.o get1.o get2.o put.o
	$(CC) $(COMMONOBJECTS) xumain.o get1.o get2.o put.o $(LFLAGS)  -o m
	rm -f xumain
	mv m xumain
	chmod +w xumain

backend: $(HINCLUDES) $(COMMONOBJECTS) be.o get1fe.o get2fe.o putfe.o
	$(CC) $(COMMONOBJECTS) be.o get1fe.o get2fe.o putfe.o $(LFLAGS) -o be
	rm -f backend
	mv be backend
	chmod +w backend

backenddaemon: $(HINCLUDES) port.h $(COMMONOBJECTS) \
	bed.o socketbe.o get1fe.o get2fe.o putfe.o 
	$(CC) $(COMMONOBJECTS) bed.o socketbe.o get1fe.o get2fe.o putfe.o \
	$(LFLAGS) -o bed
	rm -f backenddaemon
	mv bed backenddaemon
	chmod +w backenddaemon

lint: $(HINCLUDES) be.c get1fe.c get2fe.c putfe.c $(COMMONCSOURCES)
	lint -hbxa be.c get1fe.c get2fe.c putfe.c $(COMMONCSOURCES) >zzlintout
	cp zzlintout zzlintsave

bedlint: $(HINCLUDES) port.h bed.c socketbe.c get1fe.c get2fe.c \
	putfe.c $(COMMONCSOURCES)
	lint bed.c socketbe.c get1fe.c get2fe.c putfe.c \
	$(COMMONCSOURCES) >zzbedlintout
	cp zzbedlintout zzbedlintsave

wc: $(HINCLUDES) be.c get1fe.c get2fe.c putfe.c $(COMMONCSOURCES)
	wc $(HINCLUDES) be.c get1fe.c get2fe.c putfe.c $(COMMONCSOURCES)

c: $(HINCLUDES) port.h $(COMMONCSOURCES) xumain.c be.c bed.c socketbe.c \
	get1fe.c get2fe.c putfe.c get1.c get2.c put.c 
#	mv *.c test2
#	cp *.h test2

tags: $(HINCLUDES) be.c get1fe.c get2fe.c putfe.c $(COMMONCSOURCES) \
	socketbe.c 
	ctags  $(HINCLUDES) be.c get1fe.c get2fe.c putfe.c $(COMMONCSOURCES) \
	socketbe.c 

etags: $(HINCLUDES) be.c get1fe.c get2fe.c putfe.c $(COMMONCSOURCES) \
	socketbe.c 
	etags -t $(HINCLUDES) be.c get1fe.c get2fe.c putfe.c $(COMMONCSOURCES) \
	socketbe.c 

flow: $(HINCLUDES) $(COMMONCSOURCES) be.c get1fe.c get2fe.c putfe.c
	cflow  $(HINCLUDES) $(COMMONCSOURCES) be.c get1fe.c get2fe.c \
	putfe.c >flow

intf: intf.o
	$(CC) intf.o -o intf

ints: ints.o rcfile.o
	$(CC) ints.o rcfile.o -o ints

intsr: intsr.o
	$(CC) intsr.o -o intsr

intx: intx.o rcfile.o
	$(CC) intx.o rcfile.o -o intx

xlog: xlog.o rcfile.o
	$(CC) xlog.o rcfile.o -o xlog

xlogl: xlog.o rcfile.o
	$(CC) xlog.o rcfile.o -o xlog

mfe: mfe.o
	$(CC) mfe.o -o mfe

kkp: kkp.o
	$(CC) kkp.o -O -o zkkp
	cp zkkp kkp
	rm zkkp

kkkp: kkkp.o
	$(CC) kkkp.o -O -o zkkkp
	cp zkkkp kkkp
	rm zkkkp
mycat: mycat.o
	$(CC) mycat.o -O -o mycat

fe: fe.h fe2.o fe1.o
	 cc fe1.o fe2.o -O -o fe

fe.h: fe.j

# protos.h: protos.j

#common.h: common.j

#wisp.h: wisp.j

enf.h: wisp.h

#ndenf.h: ndenf.j

crum.h: common.h enf.h

xanadu.h: common.h

#requests.h: requests.j

#reap.h: reap.j

#coredisk.h: coredisk.j



cpmstdio.o: cpmstdio.c

rcfile.o: rcfile.c port.h

entexit.o: entexit.c enf.h xanadu.h coredisk.h

task.o: task.c xanadu.h

dumporgl.o: dumporgl.c common.h  enf.h wisp.h

sporgl.o: sporgl.c xanadu.h common.h enf.h wisp.h

context.o: context.c xanadu.h common.h enf.h wisp.h

corediskin.o: corediskin.c crum.h common.h  enf.h wisp.h coredisk.h

corediskout.o: corediskout.c crum.h common.h  enf.h wisp.h coredisk.h

disk.o: disk.c crum.h common.h  enf.h wisp.h coredisk.h

diskalloc.o: diskalloc.c crum.h common.h  enf.h wisp.h coredisk.h

credel.o: credel.c common.h reap.h  enf.h wisp.h queues.h alloc.h

edit.o: edit.c common.h  enf.h wisp.h ndenf.h

genf.o: genf.c crum.h common.h  enf.h wisp.h

insert.o: insert.c crum.h common.h  enf.h wisp.h ndenf.h

retrie.o: retrie.c crum.h common.h  enf.h wisp.h

split.o: split.c crum.h common.h  enf.h wisp.h

test.o: test.c xanadu.h common.h  enf.h wisp.h ndenf.h coredisk.h

tumble.o: tumble.c common.h

wisp.o: wisp.c common.h  enf.h wisp.h

ndcuts.o: ndcuts.c crum.h ndenf.h players.h

ndinters.o: ndinters.c crum.h common.h  enf.h wisp.h ndenf.h

recombine.o: recombine.c crum.h common.h  enf.h wisp.h

test.o: test.c crum.h common.h  enf.h wisp.h coredisk.h

testtmbl.o: testtmbl.c common.h

usefull.o: usefull.c xanadu.h common.h  enf.h wisp.h

do1.o: do1.c xanadu.h common.h

do2.o: do2.c xanadu.h common.h

fns.o: fns.c xanadu.h common.h

get1.o: get1.c xanadu.h common.h

get2.o: get2.c xanadu.h common.h

granf1.o: granf1.c xanadu.h common.h

granf2.o: granf2.c xanadu.h common.h  enf.h wisp.h

hint.o: hint.c xanadu.h common.h

xumain.o: xumain.c xanadu.h common.h  enf.h wisp.h

orglinks.o: orglinks.c xanadu.h common.h  enf.h wisp.h

put.o: put.c xanadu.h common.h

spanf1.o: spanf1.c xanadu.h common.h  enf.h wisp.h

spanf2.o: spanf2.c xanadu.h common.h  enf.h wisp.h

tf.o: tf.c

be.o: be.c xanadu.h common.h  enf.h wisp.h

init.o: init.c xanadu.h common.h  enf.h wisp.h

get1fe.o: get1fe.c xanadu.h common.h

get2fe.o: get2fe.c xanadu.h common.h

putfe.o: putfe.c xanadu.h common.h



correspond.o: correspond.c xanadu.h

alloc.o: alloc.c alloc.h

allocdebug.o: allocdebug.c alloc.h

ints.o: port.h

bed.o: xanadu.h common.h enf.h wisp.h port.h players.h

socketbe.o: port.h players.h

multiloaf.o: crum.h coredisk.h multiloaf.c

queues.o: queues.h
