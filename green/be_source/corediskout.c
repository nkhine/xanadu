/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License,
* which contains precisely the terms of the X11 License.  The full text of
* The Udanax Open-Source License can be found in the distribution in the file
* license.html.  If this file is absent, a copy can be found at
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/*orglwrite and subtree write are called from  reap in credel.d */

#include "crum.h"
#include "coredisk.h"

extern bool isxumain;
 INT nolread;
 INT nolwrote;
 INT noishouldbother;
 INT notakenephewnd;
 INT noeatbrosnd;

typegranf granf;
typespanf spanf;

void diskexit()
{
  static indiskexit();
	indiskexit();
}


/* make sure file is up to date when exitting program */
static indiskexit ()
{
/*  FILE *record;*/bool decrementusers();

	if (decrementusers()) {
		return;
	}
	writeenfilades();

	closediskfile ();
	/*record = fopen ("diskiocount", "a");
	fprintf (record, "%s:  ", (isxumain?"xumain":"backend"));
	fprintf (record, "%5ld reads, %5ld writes, ", nolread, nolwrote);
	fprintf (record, "%5ld isb, %5ld ebnd, %5ld tnnd\n",noishouldbother,
						noeatbrosnd, notakenephewnd);
	fclose (record);*/
	exit (0);
}


/* Update disk copy of all enfilades, and reset core versions for multiuser */
diskflush ()
{
  void initkluge();

	writeenfilades();
	initkluge((typecuc**)&granf, (typecuc**)&spanf);
}


/* Write entire granfilade and spanfilade to disk and flag as unmodified in core */
writeenfilades()
{
  typecbc temporgl;

	temporgl.leftbroorfather = NULL;
	temporgl.modified = TRUE;
	temporgl.cinfo.granstuff.orglstuff.orglincore = TRUE;
	temporgl.cinfo.granstuff.orglstuff.diskorglptr.diskblocknumber = GRANFDISKLOCATION;
	temporgl.cinfo.granstuff.orglstuff.diskorglptr.insidediskblocknumber = 0;
	temporgl.cinfo.granstuff.orglstuff.orglptr = (typecuc *)granf;
	((typecuc *)granf)->leftbroorfather = (typecorecrum *)&temporgl;
	orglwrite (&temporgl);
			      
	temporgl.modified = TRUE;
	temporgl.cinfo.granstuff.orglstuff.orglincore = TRUE;
	temporgl.cinfo.granstuff.orglstuff.diskorglptr.diskblocknumber = SPANFDISKLOCATION;
	temporgl.cinfo.granstuff.orglstuff.diskorglptr.insidediskblocknumber = 0;
	temporgl.cinfo.granstuff.orglstuff.orglptr = (typecuc *)spanf;
	((typecuc *)spanf)->leftbroorfather = (typecorecrum *)&temporgl;
	orglwrite (&temporgl);
}


#define hputinloaf(hp,lp,tp) ((void)humberput((INT)(hp),(humber)(lp),(unsigned INT*)(tp)),(lp)=((char*)lp)+*(tp))
/*#define hputwisp(wp,lp,tp) ((tp)=tumblerfixedtoptr((lp),&(wp.dsas[0])),((char*)lp)+=(tp),(tp)=tumblerfixedtoptr((lp),&(wp.dsas[1])),((char *)lp)+=(tp))*/

  static
hputwiddsp(ptr,loafptrptr)
  typecuc *ptr;
  char **loafptrptr;
{
  int i,nstreams;
  unsigned INT temp;
  typewid *wptr;

	nstreams = widsize(ptr->cenftype);

	wptr = &ptr->cdsp;
	for (i = 0;i < nstreams; ++i) {
	 	temp = tumblerfixedtoptr(&wptr->dsas[i], (humber) *loafptrptr);
		*loafptrptr += temp;	
	}
	wptr = &ptr->cwid;
	for (i = 0;i < nstreams; ++i) {
	 	temp = tumblerfixedtoptr(&wptr->dsas[i], (humber) *loafptrptr);
		*loafptrptr += temp;
	}
}

/*hdump(n,ptr)
  int n;
  char *ptr;
{
#ifndef DISTRIBUTION
  int i;

	for (i = 0; i < n; i++) {
		fprintf(stderr,"%x ", *(ptr+i));
	}
#endif
}
*/

  static int
varpackloaf (father, xloafptr, refcount,flag)
  typecuc *father;
  typediskloaf *xloafptr;
  int refcount;
  int flag;
{
  typecorecrum *ptr;
  INT ret;
  char *loafptr; /*,*tempptr;*/
  unsigned INT temp;

	loafptr = (char *)xloafptr;
	if (!flag) {
		if (!father || father->height == 0 || !loafptr) {
#ifndef DISTRIBUTION
			dump(father);
			gerror ("bad packloaf call\n");
#else
	gerror("");
#endif
		}
		if (father->leftson&&toomanysons (father)) {
#ifndef DISTRIBUTION
			fprintf(stderr, "PANIC:  %s(%d), %d sons\n",
				enftypestring (father->cenftype), father->height,
				father->numberofsons);
			qerror ("Too many sons in packloaf\n");
#else
	gerror("");
#endif
		}
/*		tempptr = loafptr;*/
		loafptr += 3;	/*make room for loaflength*/
		hputinloaf(FALSE,loafptr,&temp);
		hputinloaf(father->height-1,loafptr,&temp);
		hputinloaf(father->cenftype,loafptr,&temp);
		hputinloaf(father->numberofsons ,loafptr,&temp);
		hputinloaf(refcount,loafptr,&temp);
	} else {
/*		tempptr = loafptr;*/
		loafptr += 3;	/*make room for loaflength*/
		hputinloaf(TRUE,loafptr,&temp);
		hputinloaf(father->height,loafptr,&temp);
		hputinloaf(father->cenftype,loafptr,&temp);
		hputinloaf(1,loafptr,&temp);
		hputinloaf(refcount,loafptr,&temp);

		hputwiddsp(father,&loafptr);
		hputinloaf(father->sonorigin.diskblocknumber,loafptr,&temp);
		hputinloaf(father->sonorigin.insidediskblocknumber,loafptr,&temp);

		if (father->sonorigin.diskblocknumber == 0) {
#ifndef DISTRIBUTION
			/*dump (ptr); */
			gerror("trying to write 0 block\n");  
#else
	gerror("");
#endif
		} 
		if (father->sonorigin.diskblocknumber == DISKPTRNULL) {
#ifndef DISTRIBUTION
			/*dump (ptr); */
			gerror("trying to write DSKPTRNULL block\n");  
#else
	gerror("");
#endif
		} 

		return((INT)(loafptr)-(INT)xloafptr);
	}
	for (ptr = (typecorecrum *)findleftson(father) ; ptr; ptr = ptr->rightbro) {
		hputwiddsp((typecuc*)ptr,(char **)&loafptr);
		if (ptr->height != 0) {
			hputinloaf(((typecuc *)ptr)->sonorigin.diskblocknumber,loafptr,&temp);
			hputinloaf(((typecuc *)ptr)->sonorigin.insidediskblocknumber,loafptr,&temp);

			if (((typecuc *)ptr)->sonorigin.diskblocknumber == 0) {
#ifndef DISTRIBUTION
				dump (ptr); 
				gerror("trying to write 0 block\n");  
#else
	gerror("");
#endif
			} 
			if (((typecuc *)ptr)->sonorigin.diskblocknumber == DISKPTRNULL) {
#ifndef DISTRIBUTION
				dump (ptr); 
				gerror("trying to write DISKPTRNULL block\n");  
#else
	gerror("");
#endif
			} 
		} else {
			hputinfo ((typecbc*)ptr, &loafptr);
		}
	}

	ret = (INT)loafptr - (INT)xloafptr;
	(void) humber3put(ret, (humber) xloafptr, &temp);
	return (ret);
}

/*
#define hputinloaf(hp,lp,tp) (humberput((hp),(lp),(tp)),(lp)=((char*)lp)+*(tp))
*/

hputinfo(ptr,loafptrptr)
  typecbc *ptr;
  char **loafptrptr;
{
  unsigned INT temp;

	if (!is2dcrum((typecorecrum*)ptr)) {
		(void) humberput(ptr->cinfo.infotype,(humber)*loafptrptr,&temp);
		*loafptrptr += temp;
		if (ptr->cinfo.infotype == GRANTEXT) {
			(void) humberput((INT)ptr->cinfo.granstuff.textstuff.textlength,(humber)*loafptrptr,&temp);
			*loafptrptr += temp;
			/*hputinloaf(ptr->cinfo.granstuff.textstuff.textlength,(*loafptrptr),&temp);
*/
			movmem(ptr->cinfo.granstuff.textstuff.textstring,(*loafptrptr),ptr->cinfo.granstuff.textstuff.textlength);
			*loafptrptr += ptr->cinfo.granstuff.textstuff.textlength;
			return;
		} else if (ptr->cinfo.infotype == GRANORGL) {
			(void) humberput(ptr->cinfo.granstuff.orglstuff.diskorglptr.diskblocknumber,(humber)*loafptrptr,&temp);
			*loafptrptr += temp;
			(void) humberput(ptr->cinfo.granstuff.orglstuff.diskorglptr.insidediskblocknumber,(humber)*loafptrptr,&temp);
			*loafptrptr += temp;
			/*hputinloaf(ptr->cinfo.granstuff.orglstuff.diskorglptr,(*loafptrptr),&temp);*/
			return;
		} else if (ptr->cinfo.infotype == GRANNULL) {
			return;
		} else {
#ifndef DISTRIBUTION
			fprintf(stderr,"weird infotype in hputinfo %d \n",ptr->cinfo.infotype);
			gerror("weird infotype in hputinfo\n");
#else
	gerror("");
#endif
			return;
		}
	} else {
		if (ptr ->height) {
			/* looks like we got this all*/
		} else {
			temp = tumblerfixedtoptr(&((type2dcbc *)ptr)->c2dinfo.homedoc,(humber)*loafptrptr);
			(*loafptrptr) += temp;
		}
	}
}

  static int
packloaf (father, loafptr,refcount,flag)
  typecuc *father;
  typediskloaf *loafptr;
  int refcount;
  int flag;
{
	return(varpackloaf (father, loafptr,refcount,flag));
}

orglwrite (orglcbcptr)
  typecbc *orglcbcptr;
{
  typetask task;
  static orglwritepart2();

	if (!orglcbcptr) {
#ifndef DISTRIBUTION
		gerror("in orglwrite passed NULL\n");
#else
	gerror("");
#endif
	}
	inittask (&task);
	orglwritepart2 (&task, orglcbcptr);
	tfree (&task);
}

typediskloaf zzzeroloaf;

  typediskloafptr partialdiskalloc(),diskalloc();
static orglwritepart2 (taskptr, orglcbcptr)
  typetask *taskptr;
  typecbc *orglcbcptr;
{
  typegranbottomcruminfo *infoptr;
  typediskloafptr partialdiskalloc(),diskalloc();
  typediskloafptr  olddiskptr;
  typediskloaf loaf;
  typediskloafptr temploaf;
  INT size;
  unsigned INT dummy;
  INT newloaf;
  typecuc *orglptr;
  static subtreewriterecurs();

  	loaf = zzzeroloaf;
	if (!orglcbcptr)
#ifndef DISTRIBUTION
		gerror ("orglwrite argh!");
#else
	gerror("");
#endif
	infoptr = &orglcbcptr->cinfo;
	orglptr = infoptr->granstuff.orglstuff.orglptr;
	if (!orglcbcptr->modified && orglptr) {
		orglfree (orglptr);
		return;
	}
	if (infoptr->granstuff.orglstuff.orglincore) {
		reserve((typecorecrum*)orglcbcptr);
		olddiskptr = infoptr->granstuff.orglstuff.diskorglptr;
		subtreewriterecurs(taskptr, orglptr);
	/*	writefullcrum (orgl);  */
		size = packloaf(orglptr, &loaf, 1, 1);
		(void) humber3put(size, (humber)&loaf, &dummy);

		if (orglptr->cenftype == POOM) {
			temploaf = partialdiskalloc(size, &newloaf);
			writeloaf(&loaf, temploaf, newloaf);
			changerefcount(temploaf ,1);
			infoptr->granstuff.orglstuff.diskorglptr = temploaf;
			deletefullcrumandgarbageddescendents(olddiskptr,TRUE,(typediskloaf*)NULL,olddiskptr/*olddiskptr is just a place holder*/);
			/*writeloaf(&loaf,infoptr->granstuff.orglstuff.diskorglptr,TRUE);*/
		} else {
		  if (diskheader.hasenftops) {
			deletefullcrumandgarbageddescendents(olddiskptr,FALSE
			,&loaf,infoptr->granstuff.orglstuff.diskorglptr);
			/*writeloaf(&loaf,infoptr->granstuff.orglstuff.diskorglptr,TRUE);*/
		  }else{
			writeloaf(&loaf,infoptr->granstuff.orglstuff.diskorglptr,TRUE);
		  }
		}
	/* NULL EFFECT HERE */
		/*ptr->modified = FALSE;*/
		rejuvinate((typecorecrum*)orglcbcptr);
		orglfree (orglptr);
	} else {
		return;/*zzz*/
	}
}

deletefullcrumandgarbageddescendents(diskptr,deletefullcrumflag,loafp,newdiskptr)
  typediskloafptr diskptr;
  bool deletefullcrumflag;
  typediskloaf *loafp;
  typediskloafptr newdiskptr;
{
  typecbc *tempcbc;
  typecbc crum;
  typeuberrawdiskloaf crum2;

	if (diskptr.diskblocknumber == DISKPTRNULL) {
		return;
	}
	/*kluge up a bottum crum, use it to read in granf, similarly for spanf*/
	/*tempcbc = (typecbc *)createcrum(0,GRAN);*/
	tempcbc = &crum;
	initcrum(0,GRAN,(typecorecrum*)&crum);

	tempcbc->cinfo.infotype = GRANORGL;
	tempcbc->cinfo.granstuff.orglstuff.diskorglptr =  diskptr;
	if (deletefullcrumflag) {
		inorglinternal (tempcbc,&crum2);
		/*reserve(tempcbc);*/
	} else {
		inorglinternal (tempcbc,&crum2);
		/*reserve(tempcbc);*/
		diskset(newdiskptr.diskblocknumber);
		writeloaf (loafp,newdiskptr,FALSE);
	}
	tempcbc->cinfo.granstuff.orglstuff.orglptr->leftbroorfather = NULL;
	deletewithgarbageddescendents(diskptr, (typecuc*)tempcbc, deletefullcrumflag);
/*kluge SKIMP reserve and rejuvinate added 11-23-86*/
}

deletewithgarbageddescendents(diskptr,father,deletefullcrumflag)
  typediskloafptr diskptr;
  typecuc *  father;
  bool deletefullcrumflag;
{
  typecbc *ptr;
  typediskloafptr ignoreddiskptr;

	if (father->height > 0) {
		ptr = (typecbc *)findleftson(father);
	}
	/* if !flag changerefcount and then possibly recurse*/
	if (!deletefullcrumflag ||!changerefcount(diskptr,-1)) {
	   if (father->height >0) {
		for (; ptr; ptr = (typecbc *)findrightbro((typecorecrum*)ptr)) {
		   if (ptr->height >0) deletewithgarbageddescendents(((typecuc *)ptr)->sonorigin,(typecuc*)ptr,TRUE);
		}
	   } else if (father->cenftype == GRAN
        && ((typecbc *)father)->cinfo.infotype == GRANORGL 
	&& ((typecbc *)father)->cinfo.granstuff.orglstuff.orglincore
        && ((typecbc *)father)->cinfo.granstuff.orglstuff.diskorglptr.diskblocknumber != GRANFDISKLOCATION
	&& ((typecbc *)father)->cinfo.granstuff.orglstuff.diskorglptr.diskblocknumber != SPANFDISKLOCATION) {
		deletefullcrumandgarbageddescendents (((typecbc *)father)->cinfo.granstuff.orglstuff.diskorglptr,TRUE,/*ECH 8-28-88ignoreddiskptr,*/(typediskloaf*)NULL,ignoreddiskptr);
	   }
	}
	/*subtreefree(ptr);*/ /*12/04/86*/
}


subtreewrite (father)
  typecuc *father;
{
  typetask task;
  static subtreewriterecurs();
	inittask (&task);
	subtreewriterecurs (&task, father);
	/*decrementandfreedisk (top);*/
	tfree (&task);
}


static subtreewriterecurs (taskptr, father)
  typetask *taskptr;
  typecuc *father;
{
  typecbc *ptr;
  static uniqueoutloaf();
/*fprintf(stderr,"entering subtreewriterecurs \n");*/

	if (!father || !father->height)
#ifndef DISTRIBUTION
		gerror ("Bad subtreewrite call\n");
#else
	gerror("");
#endif
	/* this, of course, assumes modified is set right */
	if (!father->modified ) {
		if (father->sonorigin.diskblocknumber == DISKPTRNULL) {
#ifndef DISTRIBUTION
	 	      fprintf(stderr,"insubtreewriterecurs sonorigin == -1\n");
		      dumpsubtree(father);
			gerror("in subtreewriterecurs");
#else
	gerror("");
#endif
		}
		loaffree (father);
		return;
	}
/*	if (//father->sonorigin.diskblocknumber == DISKPTRNULL&&//!father->leftson) {
	       fprintf(stderr," case b insubtreewriterecurs sonorigin == -1\n");
		dumpsubtree(father);
		gerror("in subtreewriterecurs");
	}
*/
/* in order to guarantee integrity of disk, it's     */
/*   important to write out or increment sons before */
	/*   father */

	checkmodifiednotthere(father,"A");
	for (ptr =(typecbc *)father->leftson;ptr;ptr=(typecbc *)ptr->rightbro) {
		if (ptr->height == 0 &&ptr->cenftype == GRAN&& ptr->cinfo.infotype != GRANTEXT && ptr->cinfo.infotype != GRANORGL&& ptr->cinfo.infotype != GRANORGL&&ptr->cinfo.infotype != GRANNULL) {
#ifndef DISTRIBUTION
			fprintf(stderr,"bad infotypein subtreewriterecursive  = %d\n",ptr->cinfo.infotype);
			dump(ptr);
			gerror("subtreewriterecurs\n");
#else
	gerror("");
#endif
		}
		if (ptr->height != 0) {
			subtreewriterecurs (taskptr, (typecuc*)ptr);
		} else if (ptr->cenftype == GRAN && ptr->height == 0
		     && ((typecbc *)ptr)->cinfo.infotype == GRANORGL) {
			orglwritepart2 (taskptr, ptr);
		}
		ptr->modified = FALSE;
	}
	if (father->modified) {
		checkmodifiednotthere(father,"B");

		for (ptr = (typecbc *)father->leftson; ptr; ptr = (typecbc *)ptr->rightbro) {
			if (ptr->height >0 && ((typecuc *)ptr)->sonorigin.diskblocknumber != DISKPTRNULL) {
				changerefcount(((typecuc *)ptr)->sonorigin,1);
			}
		}
		checkmodifiednotthere(father,"C");
		uniqueoutloaf (father, 0);

	}
/*rejuvinate(father);*/
	loaffree (father);
}

checkmodifiednotthere(father,string)
  typecuc *father;
  char *string;
{
return;
#ifndef DISTRIBUTION
#ifdef UnDeFineD
    if (father->modified&&(!father->leftson)/*&&father->sonor.diskblocknumber== DISKPTRNULL*/) {
	dump(father);
	fprintf(stderr,"in %s subtreewrite",string);
	gerror("in   subtreewriterecurs bad crum\n");
    }
#endif
#endif
}
typeuberdiskloaf zzzerouberloaf;

static uniqueoutloaf (father,refcount)
  typecuc *father;
  int refcount;
{
  typeuberdiskloaf loaf;
  typediskloafptr diskalloc();
  INT size;
  unsigned INT temp;
  INT newloaf;
/*fprintf(stderr,"entering uniqueoutloaf\n");*/
	if (!father->modified) {
#ifndef DISTRIBUTION
		gerror("uniqueoutloaf called with not modified\n");
#else
	gerror("");
#endif
	}
	loaf = zzzerouberloaf;
	if ((!father->leftson) && father->sonorigin.diskblocknumber != DISKPTRNULL) {
		/*fprintf(stderr,"in uniqueoutloaf calling findleftson\n");
		dump(father);
*/
		(void) findleftson(father); /* get it in so we know numberof sons zzz KLUGE is this right aug 15 86*/
		/*father->numberofsons = findnumberofdamnsons(father);*/
	}
	size = packloaf (father, (typediskloaf*)&loaf,refcount,0);
	father->sonorigin = partialdiskalloc (size, &newloaf);
/*dumphexstuff(&loaf);*/
	(void) humber3put(size, (humber) &loaf, &temp);
/*dumphexstuff(&loaf);*/
/*	loaf.numberofunterloafs = 1;*/
	if (FALSE &&newloaf) {
		addallocatedloaftopartialallocedtables(father->sonorigin,size);
	}
/*dumphexstuff(&loaf);*/
/*fprintf(stderr,"height in uniqueoutloaf = %d\n",father->height);*/
	writeloaf ((typediskloaf*)&loaf, father->sonorigin, newloaf);
	father->modified = FALSE;
/*fprintf(stderr,"leaving uniqueoutloaf\n");*/
}

