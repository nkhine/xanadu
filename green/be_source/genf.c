/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License,
* which contains precisely the terms of the X11 License.  The full text of
* The Udanax Open-Source License can be found in the distribution in the file
* license.html.  If this file is absent, a copy can be found at
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* genf.d - general enfilade manipulation routines */

#include "crum.h"

#ifndef RESERVED
#define RESERVED -1 /*in this file, this is used to flag calls from coredisk*/
#endif

bool isxumain;

  bool
is2dcrum(ptr)
  typecorecrum *ptr;
{
	return (ptr->cenftype != GRAN);
}

  typecorecrum *
getleftson(ptr)
  typecuc *ptr;
{
	rejuvinateifnotRESERVED((typecorecrum *)ptr);
	ptr = (typecuc *)ptr->leftson;
	if (ptr) {
		rejuvinateifnotRESERVED((typecorecrum *)ptr);
	}
	return ((typecorecrum *)ptr);
}

  typecorecrum *
routinegetrightbro(ptr)
  typecorecrum *ptr;
{
	rejuvinateifnotRESERVED((typecorecrum *)ptr);
	ptr = ptr->rightbro;
	if (ptr) {
		rejuvinateifnotRESERVED((typecorecrum *)ptr);
	}
	return(ptr);
}

  typecorecrum *
getrightmostbro(ptr)
  typecorecrum *ptr;
{
  typecorecrum *p;

	while (p = getrightbro(ptr))
		ptr = p;
	return (ptr);
}

  typecorecrum *
getleftbro(ptr)
  typecorecrum *ptr;
{
	rejuvinateifnotRESERVED(ptr);
	if (ptr->isleftmost)
		return (NULL);
	ptr = ptr->leftbroorfather;
	if (ptr) {
		rejuvinateifnotRESERVED((typecorecrum *)ptr);
	}
	return (ptr);
}

  typecorecrum *
getleftmostbro(ptr)
  typecorecrum *ptr;
{
  typecorecrum *p;

	while (p = getleftbro(ptr))
		ptr = p;
	return (ptr);
}

  typecuc *
getfather(ptr)
  typecorecrum *ptr;
{
	ptr = getleftmostbro(ptr)->leftbroorfather;
	if (ptr)
		rejuvinateifnotRESERVED(ptr);
	return ((typecuc*)ptr);
}

/*  bool
isfullcrum(ptr)
  typecorecrum *ptr;
{
	  return (ptr->isapex);
}
*/
  typecuc *
findfullcrum(descendant)
  typecorecrum *descendant;
{
  typecuc *ptr;

	  for (ptr = (typecuc*)descendant; !isfullcrum((typecorecrum*)ptr); ptr = findfather((typecorecrum*)ptr));
	  return (ptr);
}

  bool
isemptyenfilade(ptr)
  typecuc *ptr;
{
	  if (!isfullcrum((typecorecrum*)ptr))
		  return (FALSE);
	  switch (ptr->cenftype) {
	    case GRAN :
		  return (iszerolock(ptr->cwid.dsas, (unsigned)widsize(ptr->cenftype)));
	    case SPAN :
	    case POOM :
		  return (
		     iszerolock(ptr->cwid.dsas, (unsigned)widsize(ptr->cenftype))
		  && iszerolock(ptr->cdsp.dsas, (unsigned)dspsize(ptr->cenftype)));
	    default :
#ifndef DISTRIBUTION
		  return (gerror("isempytenfilade - bad enftype"));
#else
		return(gerror(""));
#endif
	}
}


/* Just does pointer following */
  typecuc *
functionweakfindfather(ptr)
  typecorecrum *ptr;
{
	if (!ptr){
#ifndef DISTRIBUTION
		gerror("weakfindfater called with NULL\n");
#else
	gerror("");
#endif
	}
	if (isfullcrum(ptr))   /* what about spanf*/
		return (NULL);
	for (; ptr && !ptr->isleftmost; ptr = ptr->leftbroorfather)
		;
	if (ptr) {
		return ((typecuc *)ptr->leftbroorfather);
	} else {
		return (NULL);
	}
}
  typecuc *
findfather(son)
  typecorecrum *son;
{
  typecuc *ptr;

	  if (ptr = weakfindfather(son))
		rejuvinateifnotRESERVED((typecorecrum *)ptr);
	  return(ptr);
}

  typecorecrum *
findleftbro(ptr)
  typecorecrum *ptr;
{
	if (ptr->isleftmost) {
		rejuvinateifnotRESERVED(ptr);
	       return (NULL);
	}
	ptr = ptr->leftbroorfather;
	rejuvinateifnotRESERVED(ptr);
	return (/*checkenftypes(*/ ptr /*,"")*/);
}

  typecorecrum *
findleftmostbro(ptr)
  typecorecrum *ptr;
{
	  while (!ptr->isleftmost) {
		  ptr = ptr->leftbroorfather;
		rejuvinateifnotRESERVED(ptr);
	}
	return (/*checkenftypes(*/ptr/*,"")*/);
}


  typecorecrum *
weakfindleftmostbro (ptr)
  typecorecrum *ptr;
{
	  while (!ptr->isleftmost)
		  ptr = ptr->leftbroorfather;
	  return (ptr);
}


  typecorecrum *
funcfindrightbro (ptr)
  register typecorecrum *ptr;
{
	 if (!ptr->rightbro) {
		rejuvinateifnotRESERVED (ptr);
		  return (NULL);
	 }
	ptr = ptr->rightbro;
	rejuvinateifnotRESERVED (ptr);
	return (ptr);
}

  typecorecrum *
weakfindrightbro (ptr)
  typecorecrum *ptr;
{
	 if (!ptr->rightbro) {
		  return (NULL);
	 }
	 ptr = ptr->rightbro;
	 return (ptr);
}

  typecorecrum *
findrightmostbro(leftbro)
  typecorecrum *leftbro;
{
  typecorecrum *temp;

	for (; leftbro && (temp = findrightbro (leftbro)); leftbro = temp);
	rejuvinateifnotRESERVED (leftbro);
	return (/*checkenftypes(*/leftbro/*,"")*/);
}

  typecorecrum *
findleftson (ptr)
  register typecuc *ptr;
{
  INT oldage;
	if(ptr == NULL)
		return(NULL);
	oldage = ptr->age;
	if (ptr->leftson == NULL) {
		if (ptr->sonorigin.diskblocknumber == DISKPTRNULL) {
			return (NULL);
		}
		reserve ((typecorecrum*)ptr);
		if(ptr->sonorigin.diskblocknumber == 0){
#ifndef DISTRIBUTION
			dump ((typecorecrum*)ptr);
			gerror("trying to read 0 block\n");
#else
	gerror("");
#endif
		}
		inloaf (ptr);
		/* THIS IS A REAL REJUVINATE FOR A RESERVE*/
		if (oldage != RESERVED) /* zzz experimental zz */
			rejuvinate ((typecorecrum*)ptr);
	}
	rejuvinateifnotRESERVED (ptr->leftson);
	return (ptr->leftson);
}

  typecorecrum *
findrightmostson (ptr)
  typecuc *ptr;
{
	return (findrightmostbro (findleftson (ptr)));
}

  bool
toomanysons(ptr)
  typecuc *ptr;
{
	if (ptr->height) {
		findleftson(ptr);
	}
	return (ptr->numberofsons > (ptr->height > 1 ? MAXUCINLOAF : (is2dcrum((typecorecrum*)ptr)?MAX2DBCINLOAF:MAXBCINLOAF)));
}

  bool
toofewsons(ptr)
  typecuc *ptr;
{
	if(ptr->height && ! ptr->leftson){ /* brings in leftson if not here */
		findleftson(ptr);
	}
	return (ptr->numberofsons < (ptr->height > 1 ? MAXUCINLOAF -1 : (is2dcrum((typecorecrum*)ptr)?MAX2DBCINLOAF:MAXBCINLOAF)));
}

  bool
roomformoresons(ptr)
  typecuc *ptr;
{
	if(ptr->height && ! ptr->leftson){ /* brings in leftson if not here */
		findleftson(ptr);
	}
	return (ptr->numberofsons < (ptr->height > 1 ? MAXUCINLOAF : (is2dcrum((typecorecrum*)ptr)?MAX2DBCINLOAF:MAXBCINLOAF)));
}

levelpush(fullcrumptr)
  typecuc *fullcrumptr;
{
  typecuc *new;
  typecorecrum *createcrum();
  typediskloafptr temploafptr;

#ifndef DISTRIBUTION
fprintf(stderr,"in levelpush");
/*checkwholesubtree(fullcrumptr);*/
#endif
	if (!isfullcrum ((typecorecrum*)fullcrumptr))
#ifndef DISTRIBUTION
		gerror ("Levelpush not called with fullcrum.");
#else
	gerror("");
#endif
	new=(typecuc *)createcrum ((INT)fullcrumptr->height,(INT)fullcrumptr->cenftype);
	new->isleftmost = TRUE;

	transferloaf (fullcrumptr, new);
	temploafptr = fullcrumptr->sonorigin;
	fullcrumptr->sonorigin.diskblocknumber = DISKPTRNULL;
	fullcrumptr->height++;
	adopt ((typecorecrum*)new, SON, (typecorecrum*)fullcrumptr);   /* adopt new under fullcurm*/
	new->sonorigin = temploafptr;
	setwispupwards (new,1);
	ivemodified ((typecorecrum*)new);
	ivemodified ((typecorecrum*)fullcrumptr);/* is this redundant*/
#ifndef DISTRIBUTION
fprintf(stderr,"leaving levelpush");
#endif
}

transferloaf(from, to)
  typecuc *from, *to;
{
  typecuc *ptr;
  INT nsons;

	if (from->height == 0)  /* bottom crums */
		return;
	if(from->cenftype ==SPAN || from->cenftype == POOM){
		    /*the sucker dosen't know what it is yet anywow*/
	}
	ptr = (typecuc *)findleftson (from);
	nsons = from->numberofsons;
	from->numberofsons = 0;
	to->numberofsons = nsons;
	ptr->leftbroorfather = (typecorecrum *)to;
	to->leftson = (typecorecrum *)ptr;
	from->leftson = NULL;
/*	to->sonorigin = from->sonorigin;
	from->sonorigin = DISKPTRNULL;
*/}
   

levelpull(fullcrumptr)
  typecuc *fullcrumptr;
{
/*  typecuc *ptr; */
return;
/*
	if (!isfullcrum (fullcrumptr))
#ifndef DISTRIBUTION
		gerror ("Levelpull not called with fullcrum.");
#else
	gerror("");
#endif
	if (fullcrumptr->numberofsons > 1)
		return;
	if (fullcrumptr->height <= 1)
		return;
	ptr = (typecuc *) findleftson (fullcrumptr);
	dspadd (&fullcrumptr->cdsp, &ptr->cdsp, &fullcrumptr->cdsp, fullcrumptr->cenftype);

	disown (ptr);
	fullcrumptr->height--;
	transferloaf (ptr, fullcrumptr);
	setwispupwards (fullcrumptr,1);
	freecrum (ptr);
*/
}

/*
** Remove crum from its father and siblings.
** It keeps any kids it has
*/

disown(crumptr)
  typecorecrum *crumptr;
{
  typecuc *father;
	
	if (isfullcrum(crumptr)){
#ifndef DISTRIBUTION
		dump(crumptr);
		gerror("can't disownnomodify fullcrum\n");
#else
	gerror("");
#endif
	}
	if (!(father = weakfindfather(crumptr))){
#ifndef DISTRIBUTION
		dump(crumptr);
		gerror("disownnomodify called without a father.\n");
#else
	gerror("");
#endif
	}
	disownnomodify(crumptr);
	ivemodified((typecorecrum*)father);
}

disownnomodify(crumptr)
  typecorecrum *crumptr;
{
  typecuc *father;
  typecorecrum *left, *right;

	if (isfullcrum(crumptr)){
#ifndef DISTRIBUTION
		dump(crumptr);
		gerror("can't disownnomodify fullcrum\n");
#else
	gerror("");
#endif
	}
	if (!(father = weakfindfather((typecorecrum*)crumptr))) {
#ifndef DISTRIBUTION
		dump((typecorecrum*)crumptr);
		gerror("disownnomodify called without a father.\n");
#else
	gerror("");
#endif
	}
	right = findrightbro(crumptr);
	father->numberofsons -= 1;

	if (crumptr->isleftmost) {
		father->leftson = right;
		if (right) {
			right->leftbroorfather = (typecorecrum *)father;
			right->isleftmost = TRUE;
		}
	} else { /* has left brother */
		left = findleftbro(crumptr);
		left->rightbro = right;
		if (right) {
			right->leftbroorfather = left;
		}
	}
	crumptr->leftbroorfather =
		crumptr->rightbro = NULL;
/*	ivemodified(father) ; */ /*for now till we remove this*/
}

/*
** Adopt "new" crum (and his kids) into a family of which "old"
**  is a member.
*/
adopt(new, relative, old)
  typecorecrum *new, *old;
  INT relative;
{
  typecuc *father;
  typecorecrum *left, *right;

	if (!new || !old)
#ifndef DISTRIBUTION
		qerror ("adopt with NULL\n");
#else
	gerror("");
#endif
	if (new == old)
#ifndef DISTRIBUTION
		qerror ("adopt with both crums same.\n");
#else
	gerror("");
#endif
/*	if (isfullcrum(new))
		qerror ("adopt called with fullcrum\n");
*/	new->cenftype = old->cenftype; /* make crum know what kind it is */
	switch (relative) {
	  case LEFTMOSTSON:
		father = (typecuc *) old;
		left = NULL;
		right = findleftson (father);
		break;
	  case RIGHTMOSTSON:
		father = (typecuc *) old;
		if (father->leftson)
			left = findrightmostson (father);
		else
			left = NULL;
		right = NULL;
		break;
	  case RIGHTBRO:
		if (new->height != old->height)
#ifndef DISTRIBUTION
			qerror ("adopt 1\n");
#else
	gerror("");
#endif
		left = old;
		father = findfather (left);
		right = findrightbro (left);
		break;
	  case LEFTBRO:
		if (new->height != old->height)
#ifndef DISTRIBUTION
			qerror ("adopt 2\n");
#else
	gerror("");
#endif
		right = old;
		father = findfather (right);
		left = findleftbro (right);
		break;
	  default:
#ifndef DISTRIBUTION
		qerror ("bad relative\n");
#else
	gerror("");
#endif
	}

	if (!father)
#ifndef DISTRIBUTION
		qerror ("adopt without a father!\n");
#else
	gerror("");
#endif
	if (father->height != new->height +1) {
#ifndef DISTRIBUTION
		fprintf (stderr, "father =\n");
		dump (father);
		fprintf (stderr, "\nnew =\n");
		dump (new);
		fprintf (stderr, "\nold =\n");
		dump (old);
		fprintf (stderr, "relative = %d\n",relative);
		qerror ("height mismatch in adopt\n");
#else
	gerror("");
#endif
	}
	if (left) {
		left->rightbro = new;
		new->leftbroorfather = left;
		new->isleftmost = FALSE;
	} else {
		father->leftson = new;
		new->leftbroorfather = (typecorecrum *) father;
		new->isleftmost = TRUE;
	}

	new->rightbro = right;
	if (right) {
		right->leftbroorfather = new;
		right->isleftmost = FALSE;
	}

	++father->numberofsons;
}

ivemodified (ptr)
  typecorecrum *ptr;
{
  bool fatherflag;

	if(!ptr)
		return;
	rejuvinateifnotRESERVED (ptr);

	fatherflag = TRUE; /* not really, but the incoming ptr wants to get
				modified */
	while (ptr) {
/*		if (ptr->height == 0)*/  /* 10-2-84 what was this for bug*/

	rejuvinateifnotRESERVED (ptr);
		if (fatherflag) {
			/*if (ptr->modified)
				return;*/ /* this optimization commented out because createcrum sets flag TRUE to fix this makesure that all created crums get ivemodified then change createcrum then fix here*/
			ptr->modified = TRUE;
		}
		fatherflag = ptr->isleftmost;
		ptr = ptr->leftbroorfather;
	}
}

  INT
qerror (message)
  char *message;
{
	fprintf (stderr, "Error: %s\n",message);
	abort();
	return(1);
}
  INT /* nonfatalerror*/
nferror(message)
 char *message;
{
	fprintf(stderr," Non Fatal Error %s\n",message);
}

