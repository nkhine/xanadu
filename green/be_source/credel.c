/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* credel.d -  enfilade creation and deletion routines */
#define NEWALLOC 
#include "common.h"
#include "enf.h"
#include "reap.h"

typecorecrum *grimreaper;
INT ingrimreaper = FALSE;
INT reaplevel = 0;
INT timesaroundreaper = 0;
INT crumnumber = 0;
INT reapnumber = 0;
INT nreapings = 0;

INT reservnumber = 0;

extern INT *falloc();
void freetoqueue();
char * allocfromqueue();

/*
  void
etag(ptr, tag)
  char * ptr;
  tagtype tag;
{
	*(ptr -sizeof(tagtype)) = tag;
} 
*/

  INT *
eallocwithtag(nbytes, tag)
  unsigned nbytes;
  tagtype tag;
{
  INT * ealloc();		    
  char * ret;
        if(tag == CBCTAG ||tag== CUCTAG){
	   ret = (char *)ealloc(nbytes );
	}else{
	   ret = (char *)malloc(nbytes+sizeof(tagtype));
	   ret += sizeof(tagtype);
	}
/*    etag((char*)ret,tag);*/
     *(ret - sizeof(tagtype)) = tag;
    return((INT *)ret);
}
				   
  INT *
ealloc (nbytes)   /* with tag*/
  unsigned nbytes;
{
  char *ret;
/*fprintf(stderr,"ealloc called with %d\n",nbytes);*/
     for (;;) {
    if(ret=allocfromqueue(nbytes+sizeof(tagtype))){
	ret += sizeof(tagtype);
	return (INT *) ret;
    }
	ret = (char *)falloc (nbytes+sizeof(tagtype));

	if (ret) {
/*	      setmem(ret+sizeof(tagtype),nbytes,0xf9);  */
		*(tagtype *)ret = FALSE;
		return (INT *)(ret+sizeof(tagtype));
	}
	if (grimreaper == NULL){
	        xgrabmorecore();
		/*qerror ("Why am I out of room?\n");*/
	    }
	grimlyreap();
     }
}

void efree (ptr)  /* with tag */
  char *ptr;
{

      /*  ffree(ptr);   */
      if(*(ptr-sizeof(tagtype)) == CBCTAG||*(ptr-sizeof(tagtype)) ==CUCTAG){
#ifdef NEWALLOC
	freetoqueue(ptr-sizeof(tagtype));
#else		
		ffree(ptr - sizeof(tagtype));
#endif		
	}else{
		free(ptr - sizeof(tagtype));
	}

}
/*
void efreewithtag(ptr)
  char * ptr;
{
	ffree(ptr - sizeof(tagtype));
}
*/
void initgrimreaper()
{
	grimreaper = NULL;
}

grimlyreap ()
{
  typecorecrum *ptr;
  bool isreapable();
  int eh;

	ingrimreaper = TRUE;
	if (reaplevel++)
#ifndef DISTRIBUTION
if(reaplevel ==1){
		fprintf(stderr,"Recursive grimreaper call.\n");
}
#endif
	if (!grimreaper)
#ifndef DISTRIBUTION
		gerror ("nothing to reap!\n");
#else
		gerror ("memory fouled\n");
#endif
	reapnumber = 0;
	timesaroundreaper = 0;
	for (ptr = grimreaper; grimreaper; grimreaper = (typecorecrum *)grimreaper->nextcrum) {
		if (grimreaper == ptr){
			++timesaroundreaper;
		}
		if (timesaroundreaper > 10){
			fprintf(stderr,"urk in grimlyreap\n");
			/*lookatalloc();*/
			xgrabmorecore();
			break;
/*
#ifndef DISTRIBUTION
			gerror ("I'm getting bored in grimlyreap.\n");
#else
			gerror ("memory fouled\n");
#endif
*/
		}
		if (grimreaper->age == RESERVED) {
			continue;
		}
		if (isreapable (&eh,grimreaper)) {
			reap (grimreaper);
			reapnumber = 0;
			timesaroundreaper = 0;
			--reaplevel;
			break;
		}else{
			if(eh){
				/*dump(grimreaper);*/
			}
		}
		++reapnumber;
		grimreaper->age++;
	}
	ingrimreaper = FALSE;
}

  bool
isreapable (fuckinap,localreaper)
  int *fuckinap;
  typecorecrum *localreaper;
{
  register typecorecrum *p;
  typecuc *father;
	
	*fuckinap = 0;
	if (!localreaper)
#ifndef DISTRIBUTION
		gerror ("localreaper NULL\n");
#else
		gerror("memory fouled\n");
#endif
	if(localreaper ->age < OLD || localreaper-> age == RESERVED){
		*fuckinap = 1;
		return(FALSE);
	}

	if (localreaper->isapex) {
		if (localreaper->cenftype != POOM){
			return(FALSE);
		}
		if(localreaper->modified){
		    if(!((typecuc *)localreaper)->leftson){
#ifndef DISTRIBUTION
				dump(localreaper);
			    fprintf(stderr,"in isreapable modified and no son in apex");
#endif
				return(FALSE);
		    }
		    for (p = ((typecuc *)localreaper)->leftson; p; p = p->rightbro) {
			if(p->modified){
				return(FALSE);
			}

	if(p ->age < OLD ||p -> age == RESERVED){
				return(FALSE);
			}
			if (p->height > 0 && ((typecuc *)p)->leftson){
				return(FALSE);
			}
			if (p->height == 0 && p->cenftype == GRAN && ((typecbc *)p)->cinfo.infotype == GRANORGL && ((typecbc *)p)->cinfo.granstuff.orglstuff.orglincore){
				return(FALSE);
			}
		    }
		    return(TRUE);
		}else{
		    for (p = ((typecuc *)localreaper)->leftson; p; p = p->rightbro) {
			if(p->modified){
				return(FALSE);
			}
	if(p ->age < OLD ||p -> age == RESERVED){
				return(FALSE);
			}
			if (p->height > 0 && ((typecuc *)p)->leftson){
				return(FALSE);
			}
			if (p->height == 0 && p->cenftype == GRAN && ((typecbc *)p)->cinfo.infotype == GRANORGL && ((typecbc *)p)->cinfo.granstuff.orglstuff.orglincore){
				return(FALSE);
			}
		    }
		    return(TRUE);
		}
	}
/*if height == 0 and not cinfo.orglincore return TRUE */
	if(!localreaper){
#ifndef DISTRIBUTION
		gerror("in isreapable localreaper is NULL\n");
#else
		gerror("memory fouled\n");
#endif
	}
	father = weakfindfather(localreaper);
	if(!father){
#ifndef DISTRIBUTION
		fprintf(stderr,"in isreapable no father !! \n");
#endif
		return(FALSE);
	}
	if (localreaper->height == 0) {
	      if (localreaper->cenftype == GRAN) {
		    if (((typecbc *)localreaper)->cinfo.infotype == GRANORGL) {
			if (((typecbc *)localreaper)->cinfo.granstuff.orglstuff.orglincore){
			       return(FALSE);
			}
		    }
	      }
	      for (p = weakfindleftmostbro(localreaper); p; p = p->rightbro) {
	if(p ->age < OLD ||p -> age == RESERVED){	
			return(FALSE);
			}
			if (p->height == 0 && p->cenftype == GRAN && ((typecbc *)p)->cinfo.infotype == GRANORGL && ((typecbc *)p)->cinfo.granstuff.orglstuff.orglincore){
				return(FALSE);
			}
	       }
	       return (TRUE);

	}else{ /* != 0*/
	    if(father->modified){
		for (p = weakfindleftmostbro(localreaper); p; p = p->rightbro) {
		     if(p->modified){
			   return(FALSE);
		     }
	if(p ->age < OLD ||p -> age == RESERVED){
			   return(FALSE);
		     }
		     if (p->height > 0 && ((typecuc *)p)->leftson){
			   return(FALSE);
		     }
		}
		return(TRUE);
	    } else /* if (!father->modified) */ {
		for(p=weakfindleftmostbro(localreaper);p; p = p->rightbro) {
		     if(p->modified){
			   return(FALSE);
		     }
	if(p ->age < OLD ||p -> age == RESERVED){
			   return(FALSE);
		     }
		     if (p->height > 0 && ((typecuc *)p)->leftson){
			   return(FALSE);
		     }
		}
		return (TRUE);
	    }
	}
}


reap(localreaper)
  typecorecrum *localreaper;
{
  typecuc *temp;

	if (!localreaper)
#ifndef DISTRIBUTION
		gerror("localreaper NULL0\n");
#else
		gerror("memory fouled\n");
#endif
	++nreapings;
	if (localreaper->isapex) {
		temp = (typecuc *)localreaper->leftbroorfather;
		grimreaper = grimreaper->nextcrum;
		if (!temp) {
			return;
		}
		orglwrite((typecbc*)temp);
		if (!localreaper)
#ifndef DISTRIBUTION
			gerror("localreaper NULL2\n");
#else
			gerror("memory fouled\n");
#endif
		return;
	}
	temp = weakfindfather(localreaper);
	if (!temp)
#ifndef DISTRIBUTION
		gerror("localreaper doesn't have a father\n");
#else
		gerror("memory fouled\n");
#endif
	if (!temp->leftson) {
		grimreaper = grimreaper->nextcrum;
		return;
	}
	subtreewrite(temp);
}

testforrejuvinate(ptr)
   register typecorecrum *ptr;
{
	if (ptr->age == RESERVED) {
		if (!reservnumber)
#ifndef DISTRIBUTION
			gerror("There shouldn't be any more reserved.\n");
#else
			gerror("memory mess\n");
#endif
		--reservnumber;
	}
}


funcrejuvinate(ptr)  /* inner if is testing very useful test */
   register typecorecrum *ptr;
{
	if (ptr->age == RESERVED) {
		if (!reservnumber) {
#ifndef DISTRIBUTION
			dump(ptr);
			gerror ("There shouldn't be any more reserved.\n");
#else
			gerror("memory mess\n");
#endif
		}
		--reservnumber;
	}
	ptr->age = NEW;
}


/* protect a crum and its ancestors from being grimly reaped */
void reserve(ptr)
  typecorecrum *ptr;
{
#ifndef DISTRIBUTION
foohex( "reserve\n",ptr);
#endif
	if (ptr->age != RESERVED) {
		++reservnumber;
	} else {
#ifndef DISTRIBUTION
		gerror("reserve already reserved \n");
#else
		gerror("arg!\n");
#endif
	}
	ptr->age = RESERVED;
}

void testforreservedness(msg)/* test to see if any reserved flags linger in the memory.  if they do is a gross error in crum stuff */
  char *msg;
{
  typecorecrum *ptr;
  INT numreserved = 0;
  bool first;

	for (ptr = grimreaper, first = TRUE; ptr && (first ? TRUE : ptr != grimreaper); ptr = ptr->nextcrum, first = FALSE)
		if (ptr->age == RESERVED) {
			++numreserved;
			if (TRUE||debug)
				dump(ptr);
		}

	if (numreserved) {
#ifndef DISTRIBUTION
		fprintf (stderr, "numreserved = %s:  There are %d reserved crums. (reservnumber = %d)\n", msg, numreserved, reservnumber);
		gerror ("This isn't supposed to be the case.\n");
#else
		gerror("arg!\n");
#endif
	}
	if (reservnumber) {
#ifndef DISTRIBUTION
		fprintf (stderr, "reservnumber = %d\n", reservnumber);
		gerror ("reservnumber is incorrect.\n");
#else
		gerror("arg!\n");
#endif
	}
}

/* assumes crum is disowned */
void subtreefree (ptr)
  typecorecrum *ptr;
{
  typecorecrum *p, *right;

	if (!ptr)
#ifndef DISTRIBUTION
		gerror ("boom in subtreefree called with ptr == NULL");
#else
		gerror("boom\n");
#endif
	if (ptr->height > 0) {
		for (p = ((typecuc *)ptr)->leftson; p; p = right) {
			right = p->rightbro;
			disown (p);
			subtreefree (p);
		}
	} else if (ptr->cenftype == GRAN && ((typecbc *)ptr)->cinfo.infotype == GRANORGL && ((typecbc *)ptr)->cinfo.granstuff.orglstuff.orglincore)
		orglfree (((typecbc *)ptr)->cinfo.granstuff.orglstuff.orglptr);
	freecrum (ptr);
}

void freecrum (ptr)
  typecorecrum *ptr;
{
	if (ptr->age == RESERVED)
#ifndef DISTRIBUTION
		gerror ("freecrum called with RESERVED crum.\n");
#else
		gerror("pow\n");
#endif
	if (grimreaper == ptr){
		grimreaper = grimreaper->nextcrum;
	}
	if (grimreaper == ptr){
		grimreaper = NULL;
	}
	ptr->nextcrum->prevcrum = ptr->prevcrum;
	ptr->prevcrum->nextcrum = ptr->nextcrum;
	/*zzz should it decrement usecount here sometimes?*/
	--crumnumber;
	efree ((char*)ptr);
}

void loaffree (father)
  typecuc *father;
{
  typecorecrum *ptr, *next;

	if (father->height <= 0/* || !father->leftson*/){
		gerror ("bad call\n");
	}
	for (ptr = father->leftson; ptr; ptr = next) {
		next = ptr->rightbro;
		disownnomodify (ptr);
		subtreefree (ptr);
	}
	father->modified = FALSE;
}

void orglfree (ptr)
  typecuc *ptr;
{
#ifndef DISTRIBUTION
	if (!ptr)
		qerror ("orglfree called with no orglptr.\n");
	if (!ptr->isapex)
		gerror("Orglfree called with non-fullcrum.\n");
#else
	if (!ptr || !ptr->isapex)
		gerror("bad call\n");
#endif
	if (((typecbc *)ptr->leftbroorfather)->cinfo.granstuff.orglstuff.diskorglptr.diskblocknumber == DISKPTRNULL)
#ifndef DISTRIBUTION
		qerror ("orglfree called with unwritten-out orgl.\n");
#else
		gerror("arg!\n");
#endif
	((typecbc *)ptr->leftbroorfather)->cinfo.granstuff.orglstuff.orglincore = FALSE;
	((typecbc *)ptr->leftbroorfather)->cinfo.granstuff.orglstuff.orglptr = NULL;
	subtreefree ((typecorecrum*)ptr);
}

  typecuc *
createenf (enftype)
  INT enftype;
{
  typecuc *fullcrumptr;
  typecorecrum *ptr;

	fullcrumptr = (typecuc *) createcrum(1,enftype);
	fullcrumptr->cenftype = enftype;
	fullcrumptr->isapex = TRUE;
	fullcrumptr->isleftmost = TRUE;
	adopt(ptr = createcrum(0, enftype), SON, (typecorecrum*)fullcrumptr);
	if (enftype == GRAN) {
		((typecbc *)ptr)->cinfo.infotype = GRANNULL;/*KLUGE will this work?? */
	}
	ivemodified  (ptr);
/*	if(enftype == GRAN){
		levelpush(fullcrumptr);
		adopt (createcrum(0,enftype), RIGHTBRO, fullcrumptr->leftson);
	       adopt(createcrum(0,enftype),SON,fullcrumptr->leftson->rightbro);
		ptr = fullcrumptr->leftson->leftson;
		tumblerincrement(&(ptr->cwid.dsas[WIDTH]),2,1,&(ptr->cwid.dsas[WIDTH]));
		setwispupwards(ptr,1);
	}
*/
	return (fullcrumptr);
}

  typecorecrum *
createcrum(crumheight, enftype)
  INT crumheight;
  INT enftype;
{
  typecorecrum *createcruminternal(),*ptr;

	ptr = createcruminternal(crumheight, enftype, (typecorecrum*)NULL);
	if (grimreaper) {
		ptr->nextcrum = grimreaper;
		ptr->prevcrum = grimreaper->prevcrum;
		grimreaper->prevcrum->nextcrum = ptr;
		grimreaper->prevcrum = ptr;
	} else {
		grimreaper = ptr->nextcrum = ptr->prevcrum = ptr;       
	}
	return(ptr);
}

void initcrum(crumheight, enftype, ptr)
  INT crumheight;
  INT enftype;
  typecorecrum *ptr;
{
  typecorecrum *createcruminternal();

	createcruminternal(crumheight, enftype, ptr);
}

  typecorecrum *
createcruminternal(crumheight, enftype, allocated)
  INT crumheight;
  INT enftype;
  typecorecrum *allocated;
{
  typecorecrum *ptr;
  INT *eallocwithtag();
  unsigned crumsize;

	if (crumheight == 0) {
		switch (enftype) {
		  case GRAN:
			crumsize = sizeof(typecbc);
			break;
		  case SPAN:
		  case POOM:
			crumsize = sizeof(/*typecbc*/type2dcbc);
			break;
		  default:
#ifndef DISTRIBUTION
			fprintf(stderr," enftype = %d\n",enftype);
			gerror("createcrum illegal enftype");
#else
			gerror("bad type\n");
#endif
		}
	} else {
		crumsize = sizeof(typecuc);
	}
	if (!allocated) {
		ptr = (typecorecrum *)eallocwithtag(crumsize, (tagtype)(crumheight > 0 ? CUCTAG : CBCTAG));
	} else {
		ptr = allocated;
	}
	ptr->height = crumheight;
	ptr->isapex = FALSE;
	ptr->cenftype = enftype;
	ptr->modified = TRUE/*FALSE*/;
	ptr->isleftmost = FALSE;
	ptr->age = NEW;
	ptr->leftbroorfather = NULL;
	ptr->rightbro = NULL;
	clear(&ptr->cdsp, sizeof(ptr->cdsp));
	clear(&ptr->cwid, sizeof(ptr->cwid));
	if (crumheight > 0) {
		((typecuc *)ptr)->numberofsons = 0;
		((typecuc *)ptr)->leftson = NULL;
		((typecuc *)ptr)->sonorigin.diskblocknumber = DISKPTRNULL;
	} else {
		if (enftype == GRAN) {
			clear(&((typecbc *)ptr)->cinfo, sizeof(((typecbc *)ptr)->cinfo));
			((typecbc *)ptr)->cinfo.infotype = GRANCLEARLYILLEGALINFO;
		} else {
			clear(&((type2dcbc *)ptr)->c2dinfo, sizeof(((type2dcbc *)ptr)->c2dinfo));
		}
	}
	++crumnumber;
	return(ptr);
}
#include "queues.h"
#include "alloc.h"
#define MAXALLOCQUEUEARRAY 500
struct queue allocqueuearray[MAXALLOCQUEUEARRAY];

initqueues()
{
  INT i;
  INT num;
  INT j;
	for(i=0;i<MAXALLOCQUEUEARRAY;i++){
		qinit(&allocqueuearray[i]);
	}
#ifdef NEWALLOC
	num = allocsize/3;
#else
	num = 0;
#endif
	for(j=0;j<num;j += sizeof(typecuc) + sizeof(typecbc)+ sizeof(type2dcbc)+3*sizeof(tagtype)){
		qpush(&allocqueuearray[(sizeof(typecuc)+sizeof(tagtype)+sizeof(HEADER) -1)/sizeof(HEADER)],falloc(sizeof(typecuc)+sizeof(tagtype)));
		qpush(&allocqueuearray[(sizeof(typecbc)+sizeof(tagtype)+sizeof(HEADER) -1)/sizeof(HEADER)],falloc(sizeof(typecbc)+sizeof(tagtype)));
		qpush(&allocqueuearray[(sizeof(type2dcbc)+sizeof(tagtype)+sizeof(HEADER) -1)/sizeof(HEADER)],falloc(sizeof(type2dcbc)+sizeof(tagtype)));
	}
}
dumptable()
{
  INT i;
  INT tmp;
  fprintf(stderr,"dumptable\n");
    for(i=0;i<MAXALLOCQUEUEARRAY;i++){
	    if(tmp=qlength(&allocqueuearray[i])){
		    fprintf(stderr,"len of %d = %d\n",i,tmp);
	    }
    }
}
char * allocfromqueue(n)
  INT n;
{
/*fprintf(stderr,"allocfromqueue called with %x %d\n",n,((n+sizeof(HEADER) -1)/sizeof(HEADER)));*/
    return((char*)qremove(&allocqueuearray[((n+sizeof(HEADER) -1)/sizeof(HEADER))]));
}

void freetoqueue(ptr)
  char * ptr;
{
  INT n;
/* back before the tag is the sizeof the block in align uints includint the
  header  */
  /*dumptable();*/
  n = ((HEADER *)(ptr-sizeof(HEADER)))->s.size -1;
  /*fprintf(stderr,"freetoqueue called with %x n = %d\n",ptr,n);*/
  qpush(&allocqueuearray[n],ptr);
  
  /*dumptable();*/
}


xgrabmorecore()
{
  char* malloc(), *tmp;
     tmp = (char *)sbrk(incrementalallocsize);
if(!tmp){
	gerror("no more memory in xgrabmorecore\n");
	}
     ((HEADER*)tmp)->s.size = (incrementalallocsize+sizeof(HEADER) -1)/sizeof(HEADER);
     ffree(tmp+sizeof(HEADER));
fprintf(stderr,"xgrabmorecore got another %d\n",incrementalallocsize);
}
