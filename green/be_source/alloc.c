/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/

#include "common.h"
#include "alloc.h"
#include <sys/errno.h>

extern INT errno;
INT ingrimreaper;

static HEADER base;
static HEADER *allocp = NULL;
static HEADER baseallocated;
static HEADER * weresurethatthisisourmorecore();


  static INT flag = 0;
  static char *zzalloctop=0,*zzallocbot=0;
  static INT alloccount=0;
 
lookatalloc()
{
	lookatalloc2(&baseallocated);
}

  INT
checkalloc(c)
  char *c;
{
  HEADER *r,*oldr;
  unsigned size,oldsize;


if(!flag){
	return;
}
	if(debug && *c){
		fprintf(stderr,"%s",c);
	}
	oldsize = 0;
	for(oldr=r=base.s.ptr;flag&&(r!=&base)&&(r!=(HEADER *)zzalloctop);r=r+r->s.size){
		size = r->s.size;
		validallocthinge(r);
		if(r->s.size == 0&&r!=(HEADER *)zzalloctop){
		      fprintf(stderr, "checkalloc glorphed with ptr = %x bottom = %x top = %x\n",r,zzallocbot,zzalloctop);
			statusofalloc();
			gerror("found zero size in alloced stuff"); 
			return(1);
		}
		oldr = r;
		oldsize = size;
	}
	return(TRUE);

}

checkallocedptr(ptr,string)
  char * ptr;
  char *string;
{
	if (((HEADER *) (ptr - sizeof(HEADER) -sizeof(tagtype)))->s.size == 0) {
		fprintf(stderr,"checkallocedptr %s \n",string);
		gerror("found null size\n");
	}

}
  INT
validallocthinge(ptr)
  char *ptr;
{
  HEADER *p;
return(1); 
/* 
if(!flag){
	return TRUE;
}
	p = (HEADER *)ptr;
	if (p->s.size == 0 && p != (HEADER *)zzalloctop) {
		fprintf(stderr, "validallocthinge called with ptr = %x bottom = %x top = %x\n",ptr,zzallocbot,zzalloctop);
statusofalloc("");
		gerror("attempt to free zero size thing \n");
	}
	if(flag&&(unsigned)ptr > (unsigned)zzalloctop ||(unsigned)ptr<(unsigned)zzallocbot){
		fprintf(stderr, "validallocthinge called with ptr = %x bottom = %x top = %xn",ptr,zzallocbot,zzalloctop);
		qerror("bad validity check in alloc");
		return(0);
	}
	return(TRUE);
*/
}
  INT *
falloc(nbytes)
  unsigned nbytes;
{
  register HEADER *p, *q;
  register unsigned nunits;
  HEADER *weresurethatthisisourmorecore();

/*	if (errno) {
		if (errno != ENOTTY)
			perror ("xumain/backend/alloc");
		errno = 0;
	} */
if(ingrimreaper){
#ifndef DISTRIBUTION
	fprintf(stderr,"really error in falloc called from under grimreaper\n");
	gerror("falloc called from under grimreaper\n");
#else
	gerror("bad alloc");
#endif

}
if(!nbytes){
	gerror("falloc called with nbytes = 0\n");
	}
	/*fprintf(stderr,"falloc called with %d\n",nbytes);*/
alloccount++;
	nunits = 1 +(nbytes +sizeof(HEADER)-1)/sizeof(HEADER);
	if ((q = allocp) == NULL) {
		base.s.ptr = allocp = q = &base;
		base.s.size = 0;
	}else{
	      /*checkalloc("");*/
	}
	for (p=q->s.ptr;;q=p,p=p->s.ptr){
		if (p->s.size >= nunits){
			if (p-> s.size == nunits)
				q->s.ptr = p->s.ptr;
			else{
				p->s.size -= nunits;
				p += p->s.size;
				p->s.size = nunits;
			} 
			p->s.ptr = NULL;
			allocp = q;
			return((INT *)(p+1));
		}
		if( p == allocp)
			if ((p = weresurethatthisisourmorecore()) == NULL)
				return(NULL);
	}
}

  static HEADER *
weresurethatthisisourmorecore()
{
  char *cp;
  HEADER *up;
  char *sbrk();
  INT temp;

	if (flag)
		return(NULL);
	cp = sbrk(allocsize);
zzalloctop = (char *)cp +allocsize;
zzallocbot = (char *)cp;
	if ((INT)cp == -1){
		flag = 1;
		return(NULL);
	}
	up = (HEADER *)cp;

	temp = up->s.size = ((unsigned)allocsize) / sizeof(HEADER);

if( temp == 0){
#ifndef DISTRIBUTION
  gerror("this dumb fucker doesnt do arithmetic right");
#else
  gerror("temp == 0");
#endif
}   
	baseallocated.s.ptr = (HEADER *)cp;
	baseallocated.s.size = temp;
	ffree((char *)(cp + sizeof(HEADER)/*up +1*/));
	flag = 1;
	return(allocp);
}

ffree(ap)
  char *ap;
{
  register HEADER *p, *q;

/*	if (errno) {
		if (errno != ENOTTY)
			perror ("xumain/backend/free");
		errno = 0;
	}
*/
	p = (HEADER *)ap -1;
if(flag){

       if(!validallocthinge(p))
		gerror("wierd free in alloc\n");

  }

/*	setmem(ap,(p->s.size-1)*(sizeof(HEADER)),0);*/

	for(q=allocp;!(p>q && p<q->s.ptr);q=q->s.ptr){
		if (q>= q-> s.ptr && (p>q || p<q->s.ptr)){
			break;
		}
	}

	if (p+p->s.size == q->s.ptr){
		p->s.size += q->s.ptr->s.size;
		p->s.ptr = q->s.ptr->s.ptr;
	}else{
		p->s.ptr = q->s.ptr;
	}
	if (q+q->s.size == p){
		q->s.size += p->s.size;
		q->s.ptr = p->s.ptr;
	}else{
		q->s.ptr = p;
	}
	allocp = q;
/*checkalloc("");*/
}

 INT
statusofalloc(c)
  char *c;
{
  HEADER *r;
  unsigned number,maxsize,cumsize;

/*      checkalloc (c);*/
	number = maxsize = cumsize = 0;
	for (r=base.s.ptr;flag&&(r!=&base);r=r->s.ptr){
		maxsize = (maxsize > r->s.size ? maxsize : r->s.size);
		cumsize += r->s.size;
		number++;
	}
fprintf(stderr,"%d free hunks %d maxsize %d total free\n",number,maxsize,cumsize);
	return(TRUE);
}
