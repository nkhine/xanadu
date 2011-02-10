/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* Udanax lower-level disk routines */
#include "crum.h"
#include "coredisk.h"
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
  char *findinsideloaf();

extern INT errno;

 INT nolread = 0 /* number of blocks read from disk in session */;
 INT nolwrote = 0 /* same for writes */;

INT enffiledes;	 /* enfilade file descriptor where disk stuff is */
bool enffileread;       /* yeah another external */
void actuallyreadrawloaf();

INT findnumberofdamnsons(diskptr)
  typediskloafptr diskptr;
{
  typeuberrawdiskloaf loaf;
  char *loafp;
  void actuallywriteloaf();
  
  INT numberofsons;
  INT height,enftype;
  INT isapex;
  INT size;

	if(diskptr.diskblocknumber == DISKPTRNULL)
#ifndef DISTRIBUTION
		gerror("null diskptr in changerefcount\n");
#else
		gerror("bad diskptr");
#endif
	actuallyreadrawloaf (&loaf, diskptr.diskblocknumber);
	loafp = findinsideloaf((typeuberdiskloaf*)&loaf,diskptr.insidediskblocknumber);
	hgetfromloaf(&size,loafp);
	hgetfromloaf(&isapex,loafp);
	hgetfromloaf(&height,loafp);
	hgetfromloaf(&enftype,loafp);
	hgetfromloaf(&numberofsons,loafp);
	return(numberofsons);
}

INT changerefcount (diskptr, delta)
  typediskloafptr diskptr;
  INT delta;
{
  typeuberrawdiskloaf loaf;
  char *loafp;
  INT refcount;
  INT numberofunterloafs;
  void actuallywriteloaf();
  char *findinsideloaf();

	if (diskptr.diskblocknumber == DISKPTRNULL)
#ifndef DISTRIBUTION
		gerror("null diskptr in changerefcount\n");
#else
		gerror("bad diskptr");
#endif
	actuallyreadrawloaf(&loaf, diskptr.diskblocknumber);
	loafp = findinsideloaf((typeuberdiskloaf*)&loaf,diskptr.insidediskblocknumber);
	refcount = changeunterrefcount((typediskloaf*)&loaf,loafp,delta);
	numberofunterloafs = numberofliveunterloafs((typeuberdiskloaf*)&loaf);
	if ((INT)refcount > 0 && numberofunterloafs > 0) {
	      actuallywriteloaf (/*sizeof(typediskloaf),*/&loaf, diskptr.diskblocknumber);
	} else{
		diskfree (diskptr.diskblocknumber);
	}
	return(refcount);
}

INT changeunterrefcount (wholeloafp,originalloafp, delta)
  typediskloaf *wholeloafp;
  char *originalloafp;
  INT delta;
{
  INT ret;
  INT numberofsons;
  char *loafp;
  INT height,enftype;
  INT isapex;
  unsigned INT refcount;
  INT foo;
  unsigned INT oldlength, newlength,dummylength, dummy;
  INT lengthdif;
  INT size;
  char *refcountloafp;

	loafp = (char *)originalloafp;
	/*loafp += 3;*/
	hgetfromloaf(&size,loafp);
	hgetfromloaf(&isapex,loafp);
	/*if (isapex //ISAPEX//){
		gerror("attempt to read apex in chengerefcount\n");
	}*/
	hgetfromloaf(&height,loafp);
	hgetfromloaf(&enftype,loafp);
	hgetfromloaf(&numberofsons,loafp);
	refcountloafp = loafp;
	hgetfromloaf(&refcount,loafp);
	foo = loafp-(char *) wholeloafp;

	oldlength = intlengthoflength(refcount);
	refcount += delta;
	ret = refcount;
	newlength = intlengthoflength(refcount);
/*fprintf(stderr,"changeunterrefcount A \n");*/
	if ((INT)refcount > 0) {
/*fprintf(stderr,"changeunterrefcount B \n");*/
		if(oldlength == newlength){
			/*(void)humberput(refcount, loafp-oldlength, &dummylength);*/
		}else{
/*fprintf(stderr,"changeunterrefcount C \n");*/
/*fprintf(stderr,"changeunterrefcoun bt oldlength = %d newlength = %d\n",oldlength,newlength);*/
		      lengthdif = newlength-oldlength;
		      if(newlength >oldlength){
/*fprintf(stderr,"changeunterrefcount D \n");*/
			movmem(loafp,loafp-oldlength+newlength,sizeof(typeuberrawdiskloaf)-foo-newlength);
		      }else if (newlength >oldlength){
/*fprintf(stderr,"changeunterrefcount E \n");*/
			movmem(loafp,loafp-oldlength+newlength,sizeof(typeuberrawdiskloaf)-foo-oldlength);	
		      }
/*fprintf(stderr,"changeunterrefcount F \n");*/
			size = size +oldlength -newlength;
			(void) humber3put(size, (humber) originalloafp, &dummy);
/*fprintf(stderr,"changeunterrefcount G \n");*/
		}
/*fprintf(stderr,"changeunterrefcount H \n");*/
		(void) humberput((INT)refcount, (humber) refcountloafp/*-oldlength*/,&dummylength);
/*fprintf(stderr,"changeunterrefcount I \n");*/
/*dumphexstuff(originalloafp);*/
/*fprintf(stderr,"changeunterrefcount J \n");*/
	} else{
/*fprintf(stderr,"changeunterrefcount K \n");*/
		movmem(originalloafp+size,originalloafp+1,sizeof(typeuberrawdiskloaf)-foo-size+3);
/*fprintf(stderr,"changeunterrefcount L \n");*/
		*originalloafp = 1;
	/*	diskfree (diskptr.diskblocknumber);*/
	}
/*fprintf(stderr,"leaving changeunterrefoount \n");*/
	return(ret);
}


/* Reads whats on disk at place named by loafptr in
*       place pointed to by loaf
*/
void readloaf (loafptr, diskptr)
  typediskloaf *loafptr;
  typediskloafptr diskptr;
{
  typeuberrawdiskloaf uberrawloaf;
  humber temp;
  
/*fprintf(stderr,"entering readloaf diskblocknumber = %d insdiediskblocknumber = %d\n",diskptr.diskblocknumber,diskptr.insidediskblocknumber);*/
 	actuallyreadrawloaf(&uberrawloaf, diskptr.diskblocknumber);
/*fprintf(stderr,"in readloaf dumping loaf\n");*/
/*dumphexstuff(&uberrawloaf);*/
	temp = (humber)findinsideloaf((typeuberdiskloaf*)&uberrawloaf,diskptr.insidediskblocknumber);
	if(!temp){
		temp = (humber) (((char *)&uberrawloaf)+6);
	}
	movmem(temp, loafptr, intof(temp)); /* humber at temp is length of loaf */
}


void actuallyreadrawloaf(loafptr, blocknumber)
  typeuberrawdiskloaf *loafptr;
  INT blocknumber;
{
  bool goodblock();
/*fprintf(stderr,"entering actuallyreadloaf diskblocknumber = %d\n",blocknumber);*/
	if (!loafptr || blocknumber == DISKPTRNULL) {
#ifndef DISTRIBUTION
		fprintf (stderr,"loafptr = %d\n",blocknumber);
		if(loafptr)
			dumpsubtree(loafptr);
		gerror ("bad readloaf call\n");
#else
		gerror("bad call\n");
#endif
	}
	if (!enffileread) {
		if (close (enffiledes) != 0) {
#ifndef DISTRIBUTION
			perror ("close in readloaf");
#endif
			gerror ("close failed\n");
		}
		if ((enffiledes = open ("enf.enf", 2,0)) == -1) {
			perror("open");
			gerror("open");
		}
		enffileread = TRUE;
	}
	if (!goodblock (blocknumber)) {
#ifndef DISTRIBUTION
		fprintf (stderr, "loaf = %x\n", blocknumber);
		fprintf (stderr, "unallocated block in readloaf.\n");
#endif
		abort();
	}
	if (lseek (enffiledes, (long) blocknumber*NUMBYTESINLOAF, 0) < 0) {
#ifndef DISTRIBUTION
		perror ("lseek in readloaf");
#endif
		gerror ("lseek failed\n");
	}
	if (read (enffiledes, (char*)loafptr, sizeof(*loafptr)) <= 0){
		perror ("read");
		qerror ("read");
	}
/*dumphexstuff(loafptr);*/
     /*   if ((INT)loafptr->xdbcloaf.xdbchedr.refcount < 0) {
		qerror ("readloaf read a loaf with refcount < 0\n");
	}*/
	++nolread;
	/*return (TRUE);*/
}


/* Writes stuff at loaf onto piece of disk named by loafptr
*/
void writeloaf (loafptr, diskptr,newloaf)
  typediskloaf *loafptr;
  typediskloafptr diskptr;
  INT newloaf;
{
  void actuallywriteloaf();
  /*typeuberdiskloaf loaf;*/
  typeuberrawdiskloaf loaf;
  char *temp;
  char *last,*end;
  INT s;
  short loaftemp;
/*fprintf(stderr,"entering writeloaf newloaf flag = %d diskblocknumber = %d insdiediskblocknumber = %d\n",newloaf,diskptr.diskblocknumber,diskptr.insidediskblocknumber);*/
	if(diskptr.insidediskblocknumber > 100){
#ifndef DISTRIBUTION
		gerror("in write laof large diskblocknumber\n");
#else
		gerror("bad");
#endif
	}
/*dumphexstuff(loafptr);*/
	(void) intof((humber)loafptr);/* test it to see if it starts with length*/

/* test diskalloc if alloced read in else do as old version */

	if (!newloaf) {
/*fprintf(stderr,"writeloaf 1\n");*/
		actuallyreadrawloaf(&loaf, diskptr.diskblocknumber);
/*dumphexstuff(&loaf);*/
/*fprintf(stderr,"writeloaf 2\n");*/
		temp = findinsideloaf((typeuberdiskloaf*)&loaf, diskptr.insidediskblocknumber);
	/*fprintf(stderr,"numberofunterloafs = %d\n",loaf.xuberdiskloaf.numberofunterloafs);*/
		if(diskptr.insidediskblocknumber == loaf.xuberdiskloaf.numberofunterloafs) {
			end = temp + intof((humber)temp);
		} else {	
			last = findinsideloaf((typeuberdiskloaf*)&loaf, (INT)ntohs(loaf.xuberdiskloaf.numberofunterloafs)-1);
/*dumphexstuff(last);*/
/*fprintf(stderr,"last = %x\n",last);*/
			end  = last +  intof((humber)last);
		}
		s = sizeof(typeuberrawdiskloaf) - (end - (char *)&loaf)-SIZEOFUBERDISKHEADER;
/*fprintf(stderr,"s = %d\n",s);*/
		movmem(loafptr, /*((char*)&loaf)+6*/temp, s);
		loaf.xuberdiskloaf.versiondisknumber = 1;
		loaftemp = ntohs(loaf.xuberdiskloaf.numberofunterloafs) + 1;
		loaf.xuberdiskloaf.numberofunterloafs = htons(loaftemp);
	} else {
/*fprintf(stderr,"writeloaf A\n");*/
		movmem(loafptr, ((char*)&loaf)+6, sizeof(typeuberrawdiskloaf)-SIZEOFUBERDISKHEADER);
/*fprintf(stderr,"writeloaf B\n");*/
		loaf.xuberdiskloaf.versiondisknumber = htonl(1);
		loaf.xuberdiskloaf.numberofunterloafs = htons(1);

	}
/*dumphexstuff(&loaf);*/
  	actuallywriteloaf (/*size,*/&loaf, diskptr.diskblocknumber);
/*fprintf(stderr,"leaving wrietloaf\n");*/
}

void actuallywriteloaf (/*size,*/loafptr, diskblocknumber)
  /*INT size;*/
  typeuberrawdiskloaf *loafptr;
  INT diskblocknumber;
{
  void writethruhash();
/*fprintf(stderr,"entering actuallywriteloaf diskblocknumber = %d\n",diskblocknumber);*/
/*dumphexstuff(loafptr);*/
	/**writethruhash(loafptr,diskblocknumber);*/
	if (!loafptr || diskblocknumber == DISKPTRNULL /*|| (INT)loafptr->xdbcloaf.xdbchedr.refcount < 0*/) {
		gerror ("bad call\n");
	}
	if (!goodblock (diskblocknumber)) {
		qerror ("unallocated block.\n");
	}
	if (lseek (enffiledes, (long) diskblocknumber*NUMBYTESINLOAF, 0) < 0) {
		perror("lseek");
		gerror("lseek failed\n");
	}
	if (write (enffiledes, (char*)loafptr, sizeof(*loafptr)) <= 0) {
		perror ("write");
		qerror ("write\n");
	}
	++nolwrote;
     /*   return (TRUE);*/
}

  bool   /* return FALSE if new file */
initenffile ()
{
  INT fd, creat();
  bool ret, readallocinfo();
  static times = 0;

	if (times)
		qerror ("too many inits\n");
	++times;
	initincorealloctables();
	ret = TRUE;
	fd = open ("enf.enf", 2 /*rw*/,0);
	if (fd == -1) {
		errno = 0;
		if ((fd = creat ("enf.enf", 0666)) == -1) {
			perror ("initenffile");
			gerror ("cant open enf.enf or creatit");
		}
		initheader ();
		enffileread = FALSE;
		ret = FALSE;
	} else {
		ret = readallocinfo (fd);
		enffileread = TRUE;
	}
	if (!ret) {
		writeallocinfo (fd);
	}
	enffiledes = fd;
	return (ret);
}

closediskfile ()
{
	diskallocexit (enffiledes);
	if (close (enffiledes) != 0) {
		perror ("close in closediskfile");
		gerror("close failed");
	}
}

dieandclosefiles ()
{
	abort ();
}

warning (message)
  char *message;
{
	fprintf (stderr, "Warning: %s",message);
}
