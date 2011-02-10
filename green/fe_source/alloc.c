/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:       alloc.d
***************************************************/

#include <stdio.h>
#include "feconfig.h"
#include "fetypealias.h"

#define TRUE 1
#define FALSE 0

#ifndef MSDOS
#define ALLOCSIZE /*0xD000*/  0xD000
#else
#include <malloc.h>
#define ALLOCSIZE 0x8000
#endif
typedef nat ALIGN;

/*unsigned extern _endstk;*/

union header {
        struct{
                union header *ptr;
                unsigned size;
        }s;
        ALIGN x;
};

typedef union header HEADER;


static HEADER base;
static HEADER *allocp = NULL;

  static bool flag = 0,flag1 = 0;
  static char *zzalloctop=0,*zzallocbot=0;

  static nat alloccount=0;

 med
checkalloc(c)
  char *c;
{
  HEADER *r,*oldr;
  nat size,oldsize;
return(0);
/* for lint*/
/*        checkstack();*/
/*
        oldsize = 0;
        for (oldr=r=base.s.ptr;flag&&(r!=&base)&&(r!=zzalloctop);r=r+r->s.size){      size = r->s.size;
                validallocthinge(r);
                if(r->s.size == 0){
                        fprintf(stderr, "checkalloc glorphed with ptr = %x bottom = %x top = %xn",r,zzallocbot,zzalloctop);
                        gerror("found zero size in alloced stuff");
                }
        }
        return(TRUE);
*/
}
  void
checkstack()
{
/*   nat temp;
        if((unsigned)&temp<= (unsigned)_endstk)
                gerror("in checkstack");
*/}

  med
validallocthinge(ptr)
  char *ptr;
{
/*        if(flag&&(char *)ptr > zzalloctop ||(char *)ptr<zzallocbot){
                fprintf(stderr, "validallocthinge called with ptr = %x bottom = %x top = %xn",ptr,zzallocbot,zzalloctop);
                gerror("bad validity check in alloc");
        }
*/
        return(TRUE);
}

  bool
validallocthing(ptr)
  char *ptr;
{
 /*       if(flag&&(char *)ptr > zzalloctop ||(char *)ptr<zzallocbot)
                return(FALSE);
        else
*/
                return(TRUE);
}

  nat *
falloc(nbytes)
  unsigned nbytes;
{
/*  register*/ HEADER *p, *q;
/*  register*/ unsigned nunits;
  HEADER *weresurethisisourmorecore();
alloccount++;

/* fprintf(stderr,"entered falloc %d",alloccount); */
/*checkalloc("in falloc");*/

        nunits = 1 +(nbytes +sizeof(HEADER)-1)/sizeof(HEADER);
        if ((q = allocp) == NULL) {
                base.s.ptr = allocp = q = &base;
                base.s.size = 0;
        }else{
                checkalloc("");
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
                        allocp = q;
                        return((nat *)(p+1));
                }
                if( p == allocp)
                        if ((p = weresurethisisourmorecore()) == NULL)
                                return(NULL);
        }
}


  static HEADER *
weresurethisisourmorecore()
{
  register char *cp;
  register HEADER *up;
  char *sbrk();
  void ffree();

        if (flag)
                return(NULL);
        flag1 = 1;
#ifndef MSDOS
        cp = sbrk(ALLOCSIZE);
#else
	cp = malloc(ALLOCSIZE);
#endif
zzalloctop = (char *)cp +ALLOCSIZE;
zzallocbot = (char *)cp;
        if ((nat)cp == -1){
                flag = 1;
                return(NULL);
        }
        flag = 1;
        up = (HEADER *)cp;
        up->s.size = (ALLOCSIZE )/sizeof(HEADER);
        ffree((char *)(up +1));
        return(allocp);
}
 void
ffree(ap)
  char *ap;
{
  register HEADER *p, *q;
          if(flag){
               if(!validallocthinge(ap))
                        gerror("wierd free in alloc\n");

        }else if(flag1){
                flag=1;
        }

        p = (HEADER *)ap -1;
        memclear(ap,(p->s.size-1)*(sizeof(HEADER)));
        /*setmem(ap,(p->s.size-1)*(sizeof(HEADER)),0);*/
        for(q=allocp;!(p>q && p<q->s.ptr);q=q->s.ptr)
                if (q>= q-> s.ptr && (p>q || p<q->s.ptr))
                        break;

        if (p+p->s.size == q->s.ptr){
                p->s.size += q->s.ptr->s.size;
                p->s.ptr = q->s.ptr->s.ptr;
        }else
                p->s.ptr = q->s.ptr;
        if (q+q->s.size == p){
                q->s.size += p->s.size;
                q->s.ptr = p->s.ptr;
        }else
                q->s.ptr = p;
        allocp = q;
/*checkalloc("");*/
}


 med
statusofalloc(c)
  char *c;
{
  HEADER *r;
  nat number,maxsize,cumsize;
        checkalloc(c);
        number = maxsize = cumsize = 0;
        for (r=base.s.ptr;flag&&(r!=&base);r=r->s.ptr){
                maxsize = max(maxsize,(nat)r->s.size);
                cumsize += r->s.size;
                number++;
        }
/*printf("%d free hunks %d maxsize %d total free",number,maxsize,cumsize);
*/           return(TRUE);
}
