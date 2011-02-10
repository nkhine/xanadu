/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* multiloaf routines */
#include "crum.h"
#include "coredisk.h"
#include <sys/types.h>
#include <netinet/in.h>

typedef struct freediskconscell {
  struct freediskconscell * next;
  freediskentry   * stuff;
} freediskconscell;


#define SIZEOFFREEDISKBUCKET 50
#define NUMBEROFFREEDISKBUCKETS (NUMBYTESINLOAF/SIZEOFFREEDISKBUCKET+1)
static freediskconscell *fdorderedtable[NUMBEROFFREEDISKBUCKETS];
#define FDHASHTABLESIZE 10000
  /* a random prime */
#define FDHASHMULT 723754349

static freediskconscell *fdhashtable[FDHASHTABLESIZE];

INT findandallocateinsidediskblocknumber();

dumpincoretables()
{
#ifndef DISTRIBUTION
	dumpfdorderedtable();
	dumpfdhashtable();
#endif
}

extern void actuallywriteloaf ();

dumpfdhashtable()
{
#ifndef DISTRIBUTION
  INT i;
  freediskconscell *ptr;
fprintf(stderr,"dumping fdhashtable\n");
	for (i = 0; i <FDHASHTABLESIZE; i++) {
		if (fdhashtable[i]) {
			fprintf(stderr,"hashtable[%d]\n ",i);
			for (ptr = fdhashtable[i]; ptr; ptr = ptr->next) {
				dumpfreediskentry(ptr->stuff);
			}
		}
	}
fprintf(stderr,"exiting dumping fdhashtable\n");
#endif
}

dumpfdorderedtable()
{
#ifndef DISTRIBUTION
 INT i;
  freediskconscell *ptr;
fprintf(stderr,"dumping fdorderedtable\n");
	for (i = 0; i <NUMBEROFFREEDISKBUCKETS; i++) {
		if (fdorderedtable[i]) {
			fprintf(stderr,"fdorderedtable[%d] s = %d\n",i,(i+1)*SIZEOFFREEDISKBUCKET);
			for (ptr=fdorderedtable[i]; ptr; ptr = ptr->next) {
				dumpfreediskentry(ptr->stuff);
			}
		}
	}
fprintf(stderr,"exiting dumping fdorderedtable\n");
#endif
}

dumpfreediskentry(ptr)
  freediskentry *ptr;
{
#ifndef DISTRIBUTION
  	fprintf(stderr,"partialdiskblocknumber = %d freespaceinloaf = %d\n",ntohl(ptr->partialdiskblocknumber),ntohs(ptr->freespaceinloaf));
#endif
}

initincorealloctables()/* since these tables are extern i.e. initialized, this
				routine is just a start at restartability */
{
  INT i;
	for (i = 0; i <NUMBEROFFREEDISKBUCKETS; i++) {
		fdorderedtable[i] = NULL;
	}

	for (i = 0; i <FDHASHTABLESIZE; i++) {
		fdhashtable[i] = NULL;
	}
}

  typediskloafptr diskalloc();
savepartialdiskalloctabletodisk()
{
  freediskarray loaf;
  INT blocknumber,i;
  INT bucket;
  freediskconscell *ptr;
  typediskloafptr diskalloc(), dl;

	bucket = 0;
	ptr = fdorderedtable[bucket];
	blocknumber = PARTIALFREEDISKLOCATION;
	for (i = 0 ;; i++, ptr = ptr->next) {
		for (;ptr == 0;) {
			bucket ++;
			if (bucket >=NUMBEROFFREEDISKBUCKETS) {
				/*goto endofbuckets;*/
				loaf.numberofentrysinthisblock = htonl(i);
				loaf.nextdiskblocknumber = 0;
				actuallywriteloaf((typeuberrawdiskloaf*)&loaf,blocknumber);
				return;
			}
			ptr = fdorderedtable[bucket];
		}
		loaf.freeentryarray[i] = *(ptr->stuff);
		if (i>=NFREEENTRYS-1) {
			dl = diskalloc();
			loaf.nextdiskblocknumber = htonl(dl.diskblocknumber);
			loaf.numberofentrysinthisblock = htonl(NFREEENTRYS);
			actuallywriteloaf((typeuberrawdiskloaf*)&loaf,blocknumber);
			i = 0;
			blocknumber = dl.diskblocknumber;
		}
	}
}

readpartialdiskalloctablefromdisk()
{
  freediskarray loaf;
  INT blocknumber,i;
  INT first;
/*return;*/
	first = TRUE;
	for (blocknumber = PARTIALFREEDISKLOCATION; blocknumber; blocknumber = ntohl(loaf.nextdiskblocknumber)) {
/*fprintf(stderr,"readpartialdiskalloctablefromdisk reading block# %d\n",blocknumber);*/
		actuallyreadrawloaf((typeuberrawdiskloaf*)&loaf,blocknumber);
		if (!first) {
			diskfree(blocknumber);
		}
		first = FALSE;
if (ntohl(loaf.numberofentrysinthisblock) > NFREEENTRYS)
#ifndef DISTRIBUTION
gerror("numberofentrysinthisblock too big!\n");
#else
gerror("FOO");
#endif
		for (i = 0 ; i < ntohl(loaf.numberofentrysinthisblock); i++) {
			addtofreediskstructures(&(loaf.freeentryarray[i]));
		}
	}
}

addtofreediskstructures(diskentry)
  freediskentry *diskentry;
{
  freediskconscell *newcons;
  freediskentry *newde;
  INT i;

/*fprintf(stderr,"entering addtofreediskstructures\n");*/
/*dumpincoretables();*/
	newde   = (freediskentry *)malloc(sizeof(freediskentry));
	newcons = (freediskconscell *)malloc(sizeof(freediskconscell));
	newcons->stuff = newde;
	*newde = *diskentry;
	i = ntohs(diskentry->freespaceinloaf)/SIZEOFFREEDISKBUCKET;
/*fprintf(stderr,"orderedtable i = %d\n",i);*/
	newcons->next = fdorderedtable[i];
	fdorderedtable[i] = newcons;
	
	newcons = (freediskconscell *)malloc(sizeof(freediskconscell));
	newcons->stuff = newde;
	i = fdhash(ntohl(diskentry->partialdiskblocknumber));
/*fprintf(stderr,"hashtable i = %d\n",i);*/
	newcons->next = fdhashtable[i];
 	fdhashtable[i] = newcons;

/*dumpincoretables();*/
/*fprintf(stderr,"exiting addtofreediskstructures\n");*/
}

addallocatedloaftopartialallocedtables(dp, size)
  typediskloafptr dp;
  INT size;
{
  freediskentry stuff;
/*fprintf(stderr,"addallocatedloaftopartialallocedtables entering\n");*/
	stuff.partialdiskblocknumber = htonl(dp.diskblocknumber);
	stuff.freespaceinloaf = htons(sizeof(typeuberrawdiskloaf) - size-SIZEOFUBERDISKHEADER);
	addtofreediskstructures(&stuff);
/*fprintf(stderr,"addallocatedloaftopartialallocedtables exiting\n");*/
}

changefreediskstructures(diskentry,newsize)
  freediskentry *diskentry;
  INT newsize;
{
  INT temp;
  freediskconscell *ptr,*newptr,*oldptr;
	/* first change hash table*/
/*fprintf(stderr,"entering changefreediskstructures-----------------\n");*/
/*dumpfdhashtable();*/
	temp = fdhash(ntohl(diskentry->partialdiskblocknumber));
	oldptr = 0;
	for (ptr = fdhashtable[temp]; ptr; ptr = ptr->next) {
		if (ptr->stuff->partialdiskblocknumber == diskentry->partialdiskblocknumber) {
			newptr = ptr;
			break;
		}
		oldptr = ptr;
	}
	if (oldptr == newptr) {
#ifndef DISTRIBUTION
		gerror("in hashfromdiskblock not found\n");
#else
		gerror("FOO");
#endif
	}
	if (oldptr == 0) {
		fdhashtable[temp] = newptr->next;
	} else {
		oldptr->next = newptr->next;
	}
	/* next change ordered table */
/*dumpfdhashtable();*/
/*fprintf(stderr," I changefreediskstructures---------------\n");*/
/*dumpfdorderedtable();*/
	temp = ntohs(diskentry->freespaceinloaf)/SIZEOFFREEDISKBUCKET;
	oldptr = 0;
	for (ptr = fdorderedtable[temp]; ptr; ptr = ptr->next) {
		if (ptr->stuff->partialdiskblocknumber == diskentry->partialdiskblocknumber) {
			newptr = ptr;
			break;
		}
		oldptr = ptr;
	}
	if (oldptr == 0) {
		fdorderedtable[temp] = newptr->next;
	} else {
		oldptr->next = newptr->next;
	}
	/* last change size in entry */
	newptr->stuff->freespaceinloaf = htons(newsize);
	addtofreediskstructures(newptr->stuff);/* just call standard routine*/
	/*then delete stuff left lying around  do this more efficiently later */
	free((char *) newptr->stuff);
	free((char *) newptr);
/*dumpfdorderedtable();*/
/*fprintf(stderr," X changefreediskstructures++++++++++++++++++++\n");*/
}

freediskconscell *hashfromdiskblock(diskblocknumber)
  INT diskblocknumber;
{
  INT temp;
  freediskconscell *ptr;
	temp = fdhash(diskblocknumber);
	for (ptr = fdhashtable[temp]; ptr; ptr = ptr->next) {
		if (ntohl(ptr->stuff->partialdiskblocknumber) == diskblocknumber) {
			return (ptr);
		}
	}
#ifndef DISTRIBUTION
	gerror("in hashfromdiskblock not found\n");
#else
	gerror("");
#endif
	return NULL;  /* keep lint quiet */
}

INT fdhash(diskblocknumber)
  INT diskblocknumber;
{
	return (abs(diskblocknumber*FDHASHMULT)%FDHASHTABLESIZE);
}
#define BESTFIT
#ifdef BESTFIT
freediskentry *findfreeenoughloafinbucket(size)
  INT size;
{
  INT i;
  freediskconscell *ptr;

	for (i = size/SIZEOFFREEDISKBUCKET+1; i <NUMBEROFFREEDISKBUCKETS; i++) {
		for (ptr = fdorderedtable[i]; ptr; ptr->next) {
			if (ntohs(ptr->stuff->freespaceinloaf) >= size) {
				return(ptr->stuff);
			}
		}
	}
	return NULL;
}
#else  /*worst fit*/
freediskentry *findfreeenoughloafinbucket(size)
  INT size;
{
  INT i;
  freediskconscell *ptr;
	for (i = NUMBEROFFREEDISKBUCKETS-1; i>=size/SIZEOFFREEDISKBUCKET; i--) {
		for (ptr = fdorderedtable[i]; ptr; ptr = ptr->next) {
			if (ntohs(ptr->stuff->freespaceinloaf) >= size) {
				return(ptr->stuff);
			}
		}
	}
	return NULL;
}
#endif
typediskloafptr partialdiskalloc(size, newloafp)
  INT size;
  INT *newloafp;
{
  freediskentry diskentry, *freeentry;
  typediskloafptr dlp;
  typeuberdiskloaf loaf;

#ifndef DISTRIBUTION
if (size != 1010 && size >990) {
	fprintf(stderr,"partialdiskalloc size = %d\n",size);
}
#endif
	freeentry = findfreeenoughloafinbucket(size);
/*fprintf(stderr,"partialdiskalloc freeentry = %x\n",freeentry);*/
	if (!freeentry) { 
		dlp = diskalloc();
		dlp.insidediskblocknumber = 0/*1*/;
		diskentry.partialdiskblocknumber = htonl(dlp.diskblocknumber);
		diskentry.freespaceinloaf = htons(NUMBYTESINLOAF - size-SIZEOFUBERDISKHEADER) ;
		addtofreediskstructures(&diskentry);
/*fprintf(stderr,"partialdiskblock returning TRUE with diskblocknumber = %d\n",dlp.diskblocknumber);*/
		*newloafp = TRUE;
		return (dlp);
	} else {
		if (size >= ntohs(freeentry->freespaceinloaf)) {
			dumpincoretables();
#ifndef DISTRIBUTION
			fprintf(stderr,"size = %d number = %d\n ",size,ntohl(freeentry->partialdiskblocknumber));
			gerror("partialdiskalloc  found loaf too small\n");
#else
			gerror("");
#endif
		}
		dlp.diskblocknumber = ntohl(freeentry->partialdiskblocknumber);
		actuallyreadrawloaf((typeuberrawdiskloaf*)&loaf,ntohl(freeentry->partialdiskblocknumber));
		dlp.insidediskblocknumber = findandallocateinsidediskblocknumber(dlp.diskblocknumber,size,&loaf);
		if (dlp.insidediskblocknumber >100) {
#ifndef DISTRIBUTION
			gerror("partialdiskalloc insidediskblocknumber >100\n");
#else
			gerror("");
#endif
		}
		changefreediskstructures(freeentry, (INT)(ntohs(freeentry->freespaceinloaf) - size));
		*newloafp = FALSE;
/*fprintf(stderr,"leaving partialdiskalloc o\n");*/
		return (dlp);
	}
}


deletefromfreediskstructures(diskentry)
  freediskentry *diskentry;
{
  INT temp;
  freediskconscell *ptr,*newptr,*oldptr;
	/* first change hash table*/
	temp = fdhash(ntohl(diskentry->partialdiskblocknumber));
	oldptr = 0;
	for (ptr = fdhashtable[temp]; ptr; ptr = ptr->next) {
		if (ptr->stuff->partialdiskblocknumber == diskentry->partialdiskblocknumber) {
			newptr = ptr;
			break;
		}
		oldptr = ptr;
	}
	if (oldptr == newptr) {
#ifndef DISTRIBUTION
		gerror("in hashfromdiskblock not found\n");
#else
		gerror("");
#endif
	}
	if (oldptr == 0) {
		fdhashtable[temp] = newptr->next;
	} else {
		oldptr->next = newptr->next;
	}
	/* next change ordered table */
	temp = ntohs(diskentry->freespaceinloaf)/SIZEOFFREEDISKBUCKET;
	oldptr = 0;
	for (ptr = fdorderedtable[temp]; ptr; ptr = ptr->next) {
		if (ptr->stuff->partialdiskblocknumber == diskentry->partialdiskblocknumber) {
			newptr = ptr;
			break;
		}
		oldptr = ptr;
	}
	if (oldptr = 0) {
		fdorderedtable[temp] = newptr->next;
	} else {
		oldptr->next = newptr->next;
	}
	/*then delete stuff left lying around  do this more efficiently later */
	free((char *) newptr->stuff);
	free((char *) newptr);
}

#ifdef UnDEfiNed
newpartialdiskfree(diskloaf)
  typediskloafptr diskloaf;
{
  typeuberdiskloaf loaf;
  freediskconscell *ptr;
  INT number;
  INT temp;

	actuallyreadrawloaf((typeuberrawdiskloaf*)&loaf,diskloaf.diskblocknumber);
	number = numberofliveunterloafs(&loaf);
	if (number == 1) {
		ptr = hashfromdiskblock(diskloaf.diskblocknumber);
		diskfree(diskloaf.diskblocknumber); /* was just diskloaf ECH */
		deletefromfreediskstructures(ptr->stuff);
	} else {
		ptr = hashfromdiskblock(diskloaf.diskblocknumber);
		temp = deallocateinloaf(&loaf,diskloaf.insidediskblocknumber);
		changefreediskstructures(ptr->stuff/*ECH 8-26-88 BAD BUG (no->stuff)*/, (INT)ntohs(ptr->stuff->freespaceinloaf)+temp);
		actuallywriteloaf(/*3377,*/(typeuberrawdiskloaf*)&loaf,diskloaf.diskblocknumber);
	}
}
#endif
  INT
deallocateinloaf(loafp,insidediskblocknumber)
  typeuberdiskloaf *loafp;
  INT insidediskblocknumber;
{
  char *lp;
  unsigned INT number, n;
  INT i;
  unsigned INT temp;

	/*lp = (char *)&loafp->fakepartialuberloaf;*/lp = (char *) loafp +6;

	number = ntohs(loafp->numberofunterloafs);
	for (i = 0; i < number; i++) {
		n = lengthof((humber)lp);
		temp = intof((humber)lp);
		if (i == insidediskblocknumber) {
		    movmem(lp+temp, lp+1, sizeof(typeuberrawdiskloaf)-(lp -(char *)loafp)-1);
		    *lp = 1;
		    return(temp - 1);
		}
		if (n >= temp) {
			lp += n;
		} else {
			lp += temp ;
		}
	}
#ifndef DISTRIBUTION
  gerror("in deallocateinloaf couldnt deallocate\n");
#else
	gerror("");
#endif
  return 0;
}

findandallocateinsidediskblocknumber(diskblocknumber,size,loafp)
  INT diskblocknumber;
  INT size;
  typeuberdiskloaf *loafp;
{
  char *lp;
  INT number;
  INT i;
  INT temp;
  short loaftemp;

/*fprintf(stderr,"entering findandallocateinsidediskblocknumber\n");*/
  lp =  /*(char *)&loafp->fakepartialuberloaf;*/ (char *)loafp+6;/*zzz 1999***/
  number = ntohs(loafp->numberofunterloafs);
  for(i=0;i<number;i++){
      temp = intof((humber)lp);
	  lp += temp ;
  }
/*fprintf(stderr,"2temp = %d lp = %x loafp = %x i = %d n = %d size = %d \n",temp,lp,loafp,i,0,size);
fprintf(stderr,"lp - loafp = %d ",lp-(char*)loafp);
*/
	loaftemp = ntohs(loafp->numberofunterloafs) +1;
	loafp->numberofunterloafs = htons(loaftemp);
	temp =sizeof(typeuberrawdiskloaf)-(lp -(char *)loafp)-size;
#ifndef DISTRIBUTION
  if(temp<0){
      dumphexstuff(lp);
      dumphexstuff(loafp);
      fprintf(stderr,"2temp = %d lp = %x loafp = %x i = %d  size = %d \n",temp,lp,loafp,i,0,size);
      fprintf(stderr,"lp - loafp = %d ",lp-(char*)loafp);
      fprintf(stderr,"expression = %d diskblocknumber = %d number = %d\n",temp,diskblocknumber,number);
      dumpincoretables();
      gerror("expression negative in findandallocateinsidediskblocknumber");
      fprintf(stderr,"expression negative in findandallocateinsidediskblocknumber");
      return(i);
  }
#endif
  movmem(lp,lp+size,sizeof(typeuberrawdiskloaf)-(lp -(char *)loafp)-size);
/*fprintf(stderr," B findandallocateinsidediskblocknumber\n");*/
/*dumphexstuff(loafp);*/
  return (i);
	
}
  INT
numberofliveunterloafs(loafp)
  typeuberdiskloaf *loafp;
{
  char *lp;
  unsigned INT number, n;
  INT i;
  INT ret;
  unsigned INT temp;

	ret = 0;
	lp = /*(char *)&loafp->fakepartialuberloaf;*/ (char *)loafp +6;
	number = ntohs(loafp->numberofunterloafs);
	for (i = 0; i < number; i++) {
		n = lengthof((humber)/*loafp*/lp);/* ECH 8-29-88 */
		temp = intof((humber)lp);
		if (n >= temp) {
			lp += n;
		} else {
			lp += temp ;
		}
		if (n!=temp) {
			ret++;
		}
	}
	return (ret);
}

  char *
findinsideloaf(loafp,ninsideloaf)
  typeuberdiskloaf *loafp;
  INT ninsideloaf;
{
  char *lp;
  unsigned INT number, n;
  INT i;
  unsigned INT temp;

/*fprintf(stderr,"findinsideloaf ninsideloaf = %x \n",ninsideloaf);*/
/*fprintf(stderr,"findinsideloaf loafp = %x \n",loafp);*/
/*fprintf(stderr,"findinsideloaf lp = %x \n",lp);*/
	lp = /*(char *)&loafp->fakepartialuberloaf;*/ (char *)loafp +6;

        number = ntohs(loafp->numberofunterloafs);
/*fprintf(stderr,"findinsideloaf number = %x \n",number);*/
        for (i = 0; i < number; i++) {
                n = lengthof((humber)loafp);
/*fprintf(stderr,"findinsideloaf n = %x \n",n);*/
		if (i == ninsideloaf) {
/*fprintf(stderr,"findinsideloaf returning lp = %x \n",lp);*/
			return (lp);
		}
                temp = intof((humber)lp);
                if (n >= temp) {
                        lp += n;
                } else {
                        lp += temp ;
                }
        }
/*fprintf(stderr,"findinsideloaf returningNULL substitute\n");*/
	return lp;
}
