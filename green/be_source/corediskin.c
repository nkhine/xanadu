/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
#include "crum.h"
#include "coredisk.h"

extern bool isxumain;
static hgetwiddsp();
static varunpackloaf();
/* Initialize thing from old enf.enf */
void initkluge(granfptr, spanfptr)
  typecuc **granfptr;
  typecuc **spanfptr;
{
  typecbc *tempcbc;
	
	/*kluge up a bottum crum, use it to read in granf, similarly for spanf*/
	tempcbc = (typecbc *)createcrum(0,GRAN);
	tempcbc->cinfo.infotype = GRANORGL;
	tempcbc->cinfo.granstuff.orglstuff.diskorglptr.diskblocknumber = GRANFDISKLOCATION;
	tempcbc->cinfo.granstuff.orglstuff.diskorglptr.insidediskblocknumber = 0 /*1*/;
	inorgl (tempcbc);
	tempcbc->cinfo.granstuff.orglstuff.orglptr->leftbroorfather = NULL;
	*granfptr = tempcbc->cinfo.granstuff.orglstuff.orglptr;
	(*granfptr)->numberofsons = 0; /* hack to make number of sons in granf be correct*/

	/* this is a hack to read in the spanf like an orgl! */
	tempcbc->cinfo.granstuff.orglstuff.diskorglptr.diskblocknumber = SPANFDISKLOCATION;
	tempcbc->cinfo.granstuff.orglstuff.diskorglptr.insidediskblocknumber =0/*1*/;
	inorgl (tempcbc);
	tempcbc->cinfo.granstuff.orglstuff.orglptr->leftbroorfather = NULL;
	*spanfptr = tempcbc->cinfo.granstuff.orglstuff.orglptr; 
	(*spanfptr)->numberofsons = 0;

	freecrum((typecorecrum*)tempcbc);
}

  typediskloaf*
lookinsideloaffor(insidenumber,uloafptr)
  INT insidenumber;
  typediskloaf *uloafptr;
{
	return(uloafptr);
}

  static
unpackloaf (insidediskblocknumber,uloafptr, father)
  INT insidediskblocknumber;
  typediskloaf *uloafptr;
  typecuc *father;
{
	varunpackloaf (insidediskblocknumber,uloafptr, father);
	return;
}
/*#define hgetfromloaf(ip,lp) (*(ip)=intof(lp),fprintf(stderr,"hgetfromloaf gets %d\n",*(ip)),(lp)=((char*)lp)+lengthof(lp))*/


  static
varunpackloaf (insidediskblocknumber,uloafptr, father)
  INT insidediskblocknumber;
  typediskloaf *uloafptr;
  typecuc *father;
{
  typediskloaf *xloafptr;
  typecuc *ptr;
  char *loafp;
  INT numberofsonstoread;
  INT height,enftype;
  INT isapex,refcount;
  INT size;

	if (!uloafptr || !father || father->height <= 0) {
#ifndef DISTRIBUTION
		gerror ("bad varunpackloaf call\n");
#else
		gerror("");
#endif
	}
	xloafptr = lookinsideloaffor(insidediskblocknumber,uloafptr);
	loafp = (char *)xloafptr;
	/*loafp += 3;*/
		hgetfromloaf(&size,loafp);
	hgetfromloaf(&isapex,loafp);
/*fprintf(stderr,"isapex = %d\n",isapex);*/
	if (isapex /*ISAPEX*/){
#ifndef DISTRIBUTION
dumphexstuff(uloafptr);
dumphexstuff(xloafptr);
		gerror("attempt to read apex in varumpackloaf\n");
#else
		gerror("");
#endif
	}
	hgetfromloaf(&height,loafp);
	
	if (height != father->height-1) {
#ifndef DISTRIBUTION
		dump(father);
		dumphexstuff(uloafptr);
		fprintf(stderr,"next thing = %d\n",intof((humber)loafp));
		fprintf(stderr,"father height = %d %d\n",father->height,height);
		gerror ("height mismatch in varunpackloaf\n");
#else
		gerror("");
#endif
	}
#ifdef glurg
	father->modified = FALSE;
#endif glurg
	hgetfromloaf(&enftype,loafp);

	if (enftype != father->cenftype) {
#ifndef DISTRIBUTION
		gerror ("enftype mismatch in varunpackloaf\n");
#else
		gerror("");
#endif
	}
	hgetfromloaf(&numberofsonstoread,loafp);
	hgetfromloaf(&refcount,loafp);
	father->numberofsons = 0;
	while (numberofsonstoread--) {
		ptr = (typecuc *)createcrum (height,enftype);
		adopt ((typecorecrum*)ptr, RIGHTMOSTSON, (typecorecrum*)father);
			hgetwiddsp(ptr,&loafp);
		if (height != 0) {
			ptr->isapex = isapex;
			hgetfromloaf(&ptr->sonorigin.diskblocknumber,loafp);
			hgetfromloaf(&ptr->sonorigin.insidediskblocknumber,loafp);

			if(ptr->sonorigin.diskblocknumber == 0){ 
#ifndef DISTRIBUTION
				dumphexstuff(loafp);
				dump (ptr);
				dumphexstuff(uloafptr);
				gerror("trying to read 0 block\n"); 
#else
		gerror("");
#endif
			} 
			if(ptr->sonorigin.diskblocknumber == DISKPTRNULL){ 
#ifndef DISTRIBUTION
				dump (ptr);
				gerror("trying to read DISKPTRNULL block\n"); 
#else
		gerror("");
#endif
			} 

		} else {
			ptr->isapex = FALSE;
			hgetinfo((typecbc*)ptr,&loafp);
		}
		ptr->modified = FALSE;
	}
}

inloaf (father)
  typecuc *father;
{
  typediskloaf loaf;
	if (father->height == 0)
#ifndef DISTRIBUTION
		gerror ("inloaf on bottom crum\n");
#else
		gerror("");
#endif
	if (father->leftson)
#ifndef DISTRIBUTION
		gerror ("Its already got sons, and we aren't going to use pseudo-crums!\n");
#else
		gerror("");
#endif
	readloaf (&loaf, father->sonorigin);
	unpackloaf (father->sonorigin.insidediskblocknumber,&loaf, father);
/*nchecknumofsons(father);*/
}
inorgl (granorglptr)
  typecbc *granorglptr;
{
	inorglinternal(granorglptr,(typeuberrawdiskloaf*)NULL);
}
inorglinternal (granorglptr,crumptr)
  typecbc *granorglptr;
  typeuberrawdiskloaf *crumptr;
{
  typediskloaf loaf;
  typecuc *ptr;
  unsigned INT temp,temp2;
  char *loafp;
  int size;

	readloaf (&loaf, granorglptr->cinfo.granstuff.orglstuff.diskorglptr);
	loafp = (char *)&loaf;
	hgetfromloaf(&size,loafp);
	hgetfromloaf(&temp,loafp);/*isapex*/
	hgetfromloaf(&temp,loafp);
	hgetfromloaf(&temp2,loafp);
	if(/*FALSE&&*/crumptr){
		ptr = (typecuc *) crumptr;	/* ECH I DON'T LIKE THIS!!! */
	}else{
		ptr = (typecuc *)createcrum((INT)temp,(INT)temp2);
	}
	ptr->cenftype = temp2;
	ptr->isapex = TRUE;
	ptr->isleftmost = TRUE;
	hgetfromloaf(&temp,loafp);
	ptr->numberofsons = temp;
	hgetfromloaf(&temp,loafp);
	/*ptr->refcount = temp;*/

	hgetwiddsp(ptr,&loafp);
	hgetfromloaf(&temp,loafp);
	ptr->sonorigin.diskblocknumber = temp;
	if(ptr->sonorigin.diskblocknumber == 0){
#ifndef DISTRIBUTION
		dump (ptr);
		gerror("trying to write 0 block\n");
#else
		gerror("");
#endif
	}
	if(ptr->sonorigin.diskblocknumber == DISKPTRNULL){
#ifndef DISTRIBUTION
		dump (ptr);
		gerror("trying to write DSKPTRNULL block\n");
#else
		gerror("");
#endif
	}
	hgetfromloaf(&temp,loafp);
	ptr->sonorigin.insidediskblocknumber = temp;
	ptr->modified = FALSE;
	ptr->leftbroorfather = (typecorecrum *) granorglptr;
	granorglptr->cinfo.granstuff.orglstuff.orglptr = (typecuc *)ptr;
	granorglptr->cinfo.granstuff.orglstuff.orglincore = TRUE;
	rejuvinate ((typecorecrum*)ptr);

/*checknumofsons(ptr);*/
/*dump(ptr);*/
}

  static
hgetwiddsp(ptr,loafptrptr)
  typecuc *ptr;
  char **loafptrptr;
{
  int i,nstreams;
  unsigned INT temp;
  typewid *wptr;
	nstreams = widsize(ptr->cenftype);
	wptr = &ptr->cdsp;
	for (i = 0;i < nstreams; ++i){
	 	temp = tumblerptrtofixed((humber)*loafptrptr,&wptr->dsas[i]);
		*loafptrptr += temp;	
	}
	wptr = &ptr->cwid;
	for (i = 0;i < nstreams; ++i){
	 	temp = tumblerptrtofixed((humber)*loafptrptr,&wptr->dsas[i]);
		*loafptrptr += temp;	
	}
}

hgetinfo(ptr,loafptrptr)/*this assumes ptr crum is ok except for info*/
  typecbc *ptr;
  char **loafptrptr;
{
  unsigned INT temp;
	if (!is2dcrum((typecorecrum*)ptr)) {
		hgetfromloaf(&ptr->cinfo.infotype,*loafptrptr);
/*	dump(ptr);*/
		if(ptr->cinfo.infotype == GRANTEXT){
			ptr->cinfo.granstuff.textstuff.textlength = intof((humber)*loafptrptr);
			(*loafptrptr) += lengthof((humber)*loafptrptr);
			movmem((*loafptrptr),ptr->cinfo.granstuff.textstuff.textstring,ptr->cinfo.granstuff.textstuff.textlength);
			(*loafptrptr) += ptr->cinfo.granstuff.textstuff.textlength;
			return;
		}else if(ptr->cinfo.infotype == GRANORGL){
			ptr->cinfo.granstuff.orglstuff.diskorglptr.diskblocknumber = intof((humber)*loafptrptr);
/*			if(temp == 0 || temp == -1){
				fprintf(stderr,"bad diskblocknumber = %d \n",temp);
				gerror("boo in hgetinfo");
			}
*/
			(*loafptrptr) += lengthof((humber)*loafptrptr);
			ptr->cinfo.granstuff.orglstuff.diskorglptr.insidediskblocknumber = intof((humber)*loafptrptr);
			(*loafptrptr) += lengthof((humber)*loafptrptr);
			return;
		}else{
			return;
		}
	}else{
		if(ptr ->height){
			/* looks like we got this all*/
		}else{

			temp = tumblerptrtofixed((humber)*loafptrptr,&((type2dcbc *)ptr)->c2dinfo.homedoc);
			(*loafptrptr) += temp;
		}
	}
}

/*finmoveinfo(fromptr,toptr)
  typecbc *fromptr,*toptr;
{
//	moveinfo(fromptr,toptr);
return;//

		toptr->cinfo.infotype = fromptr->cinfo.infotype;
		if(toptr->cinfo.infotype == GRANTEXT){
	//moveinfo(fromptr,toptr);
	return;//
			toptr->cinfo.granstuff.textstuff.textlength  = fromptr->cinfo.granstuff.textstuff.textlength ;
			movmem(fromptr->cinfo.granstuff.textstuff.textstring,toptr->cinfo.granstuff.textstuff.textstring,toptr->cinfo.granstuff.textstuff.textlength);
			return;
		}else if(toptr->cinfo.infotype == GRANORGL){
	//moveinfo(fromptr,toptr);
	return;//
			toptr->cinfo.granstuff.orglstuff.diskorglptr.diskblocknumber  = fromptr->cinfo.granstuff.orglstuff.diskorglptr.diskblocknumber ;
			toptr->cinfo.granstuff.orglstuff.diskorglptr.insidediskblocknumber  = fromptr->cinfo.granstuff.orglstuff.diskorglptr.insidediskblocknumber ;
			return;
		}else if(toptr->cinfo.infotype == GRANNULL){
			return;
		}else{
		fprintf(stderr,"in finmoveinfo bad case  = %d\n",toptr->cinfo.infotype);
#ifndef DISTRIBUTION
		gerror("in finmoveinfo bad case  ");
#else
		gerror("");
#endif
			return;
		}

	//return;
	moveinfo(fromptr,toptr);//
}
*/
/*
finmove2dinfo(fromptr,toptr)
  type2dcbc *fromptr,*toptr;
{
	if(fromptr->height){

	}else{
		toptr->c2dinfo.homedoc = fromptr->c2dinfo.homedoc;
	}
	return;
	//move2dinfo(fromptr,toptr);//
}
*/
