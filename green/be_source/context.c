/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License,
* which contains precisely the terms of the X11 License.  The full text of
* The Udanax Open-Source License can be found in the distribution in the file
* license.html.  If this file is absent, a copy can be found at
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
#include "xanadu.h"
#include "enf.h"

INT contextnum = 0;
INT c2dontextnum = 0;
INT crumcontextnum = 0;

  typecontext *
createcontext (type)
  SINT type;
{
  typecontext *ret;
  INT *eallocwithtag();

	if (type == GRAN) {
	      ret=(typecontext *)eallocwithtag(sizeof(typecontext),CONTEXTTAG);
		clear (ret, sizeof(typecontext));
		++contextnum;
	} else {
		ret=(typecontext *)eallocwithtag(sizeof(type2dcontext),CONTEXT2DTAG);
		clear (ret, sizeof(type2dcontext));
		++c2dontextnum;
	}
	ret->contexttype = type;
	return (ret);
}

contextfree (context)
  typecontext *context;
{
  typecontext *c;

	for (; c = context; ) {
		if (c->contexttype == GRAN){
			--contextnum;
		}else{
			--c2dontextnum;
		}

		context = context->nextcontext;;
		efree ((char*)c);
	}
}

  typecrumcontext *
createcrumcontext (crumptr, offsetptr)
  typecorecrum *crumptr;
  typedsp *offsetptr;
{
  typecrumcontext *ret;
  INT *eallocwithtag();

	reserve (crumptr);
	ret=(typecrumcontext *)eallocwithtag(sizeof(typecrumcontext),CRUMCONTEXTTAG);
	ret->nextcrumcontext = NULL;
	ret->corecrum = (typecbc *)crumptr;
	movewisp (offsetptr, &ret->totaloffset);
	++crumcontextnum;
	return (ret);
}

crumcontextfree (context)
  typecrumcontext *context;
{
  typecrumcontext *c;

	for (;c=context; context = context->nextcrumcontext,efree((char*)c)) {
		rejuvinate ((typecorecrum*)c->corecrum);
		--crumcontextnum;
	}
}

/* put c on clist in index order */
incontextlistnd (clistptr, c, index)
  typecontext **clistptr;
  typecontext *c;
  INT index;
{
  typecontext *clist, *nextc;
  typedsp grasp;

	prologuecontextnd (c, &grasp, (typedsp*)NULL);
	c->nextcontext = NULL;
	clist = *clistptr;
				/* 1st insertion */
	if (!clist) {
/*if (debug) fprintf (stderr, "first insertion\n");*/
		*clistptr = c;
		return;
	}
				/* on beginning */
	if (whereoncontext (clist, &grasp.dsas[index], index) < THRUME) {
/*if (debug) fprintf (stderr, "beginning\n");  */
		c->nextcontext = clist;
		*clistptr = c;
		return;
	} else {
		for (; nextc = clist->nextcontext; clist = nextc) {
				/* in middle */
			if ((whereoncontext (clist, &grasp.dsas[index], index) > ONMYLEFTBORDER)
			    && (whereoncontext (nextc, &grasp.dsas[index], index) < ONMYLEFTBORDER)) {
/*if (debug) fprintf (stderr, "middle\n"); */
				c->nextcontext = nextc;
				clist->nextcontext = c;
				return;
		       }
		}
	}
	    /* on end */
/*if (debug) fprintf (stderr, "end\n"); */
	c->nextcontext = NULL;
	clist->nextcontext = c;
}

oncontextlistseq (clistptr, c)/* add to list of context */
  typecontext **clistptr;
  typecontext *c;
{
	c->nextcontext = NULL;
	if (!*clistptr) { /* 1st insertion */
		*clistptr = c;
		c->lastcontext = c; 
	} else {		/* on end */
		(*clistptr)->lastcontext->nextcontext = c;
		(*clistptr)->lastcontext = c; 
	}
}
  INT
whereoncontext (ptr, address, index)
  register typecontext *ptr;
  tumbler *address;
  INT index;   /* used when enftype == SPAN or POOM */
{
  tumbler left, right;

	switch (ptr->contexttype) {
	  case GRAN:
		movetumbler (&ptr->totaloffset.dsas[WIDTH], &left);
		tumbleradd (&left, &ptr->contextwid.dsas[WIDTH], &right);
		break;
	  case SPAN:
	 /*zzz       if (iszerotumbler (&ptr->contextwid.dsas[SPANRANGE])) {
			qerror ("zero spanrange in whereoncontext\n");
		}  */
	  case POOM:
		movetumbler (&ptr->totaloffset.dsas[index], &left);
		tumbleradd (&left, &ptr->contextwid.dsas[index], &right);
		break;
	  default:
#ifndef DISTRIBUTION
		return(gerror ("whereoncontext: bad enftype\n"));
#else
		return(gerror(""));
#endif
	}
	return (intervalcmp (&left, &right, address));
}

  typecontext *
makecontextfromcbc (crumptr, offsetptr)
  typecbc *crumptr;
  typewid *offsetptr;
{
  typecontext *context;

#ifndef DISTRIBUTION
if(debug){ foo("make contextfromcbc crum is "); dump(crumptr); }
#endif
	reserve((typecorecrum*)crumptr);
	context = createcontext(crumptr->cenftype);
	movewisp(offsetptr, &context->totaloffset);
	movewisp(&crumptr->cwid, &context->contextwid);
	if (is2dcrum((typecorecrum*)crumptr))
		move2dinfo(&((type2dcbc*)crumptr)->c2dinfo, &((type2dcontext*)context)->context2dinfo);
	else
		moveinfo(&crumptr->cinfo, &context->contextinfo);
	if (crumptr->cenftype != GRAN)
		dspadd(&context->totaloffset, &crumptr->cdsp, &context->totaloffset, (INT)crumptr->cenftype);
	context->nextcontext = NULL;
	rejuvinate((typecorecrum*)crumptr);
#ifndef DISTRIBUTION
if (debug) { dumpcontext (context); } 
#endif
	return (context);
}

context2span (context, restrictionspanptr, idx1, foundspanptr, idx2)
  typecontext *context;
  typespan *restrictionspanptr, *foundspanptr;
  INT idx1, idx2;
{
  tumbler upperbound, lowerbound;
  typedsp grasp, reach;
/*  INT i; */

/*foo("entering context2span\n");*/
/*foocontext("context = \n",context);*/
/*foospanset("restrictionspan = \n",restrictionspanptr);*/

	movetumbler (&restrictionspanptr->stream, &lowerbound);
	tumbleradd (&lowerbound, &restrictionspanptr->width, &upperbound);
	prologuecontextnd (context, &grasp, &reach);
/*footumbler("lowerbound = \n",&lowerbound);*/
/*footumbler("upperbound = \n",&upperbound);*/
	if (tumblercmp (&grasp.dsas[idx1], &lowerbound) == LESS) {
/*foo("LESSER\n");*/
/*foodsp("grasp = \n",&grasp,POOM);*/
		tumblerincrement (&grasp.dsas[idx2], 0, (INT) tumblerintdiff(&lowerbound, &grasp.dsas[idx1]), &grasp.dsas[idx2]);
/*foodsp("grasp = \n",&grasp,POOM);*/
	}
	if (tumblercmp (&reach.dsas[idx1], &upperbound)  == GREATER) {
/*foo("GREATER\n");*/
/*foodsp("reach = \n",&reach,POOM);*/
		tumblerincrement (&reach.dsas[idx2], 0,/*i=*/ - tumblerintdiff (&reach.dsas[idx1], &upperbound), &reach.dsas[idx2]);       
/*foodec("tumblerintdiff = ",i);*/
/*foodsp("reach = \n",&reach,POOM);*/
	}
/*foodsp("grasp = \n",&grasp,POOM);*/
/*foodsp("reach = \n",&reach,POOM);*/
	movetumbler (&grasp.dsas[idx2], &foundspanptr->stream);
	tumblersub(&reach.dsas[idx2],&grasp.dsas[idx2],&foundspanptr->width);
/*footumbler("foundspanwidth = \n",&foundspanptr->width);*/
	foundspanptr->itemid = index2itemid (idx2, context);
	foundspanptr->next = NULL;
/*foospanset("leaving context2span with foundspan =  \n",foundspanptr);*/
}

/* sets grasp & reach from ptr */
/*  reach may be NULL so that we won't set it */
prologuecontextnd (ptr, grasp, reach)
  typecontext *ptr;
  typedsp *grasp, *reach;
{
	movewisp (&ptr->totaloffset, grasp);
	if (reach)
		dspadd (grasp, &ptr->contextwid, reach, ptr->contexttype);
}

  INT
index2itemid (index, context)
  INT index;
  typecontext *context;
{
	switch (context->contexttype) {
	  case POOM :
		return (index == I ? ISPANID : VSPANID);
	  case SPAN :
		return (ISPANID);
	  default :
#ifndef DISTRIBUTION
		gerror ("index2itemid bad enftype");
#else
		gerror("");
#endif
		return(0 /* for lint*/ );
	}
}

  bool
context2vstuff (taskptr, context, ispanptr, vstuffsetptr)
  typetask *taskptr;
  typecontext *context;
  typeispan *ispanptr;
  typevstuffset *vstuffsetptr;
{
  typevstuffset vstuffset;
  INT contextinfotype;
  INT *taskalloc();

#ifndef DISTRIBUTION
foo("context2stuff\n");
#endif
	contextinfotype = context->contextinfo.granbottomcruminfo.infotype;
	if (contextinfotype != GRANTEXT && contextinfotype != GRANORGL)
		return (FALSE);
	switch (contextinfotype) {
	  case GRANTEXT:
		vstuffset=(typevstuffset) taskalloc (taskptr, sizeof (typetext));
		((typeitemheader *)vstuffset)->next = NULL;
#ifndef DISTRIBUTION
foocontext ("context after item= ", context);
#endif
		((typeitemheader *)vstuffset)->itemid = TEXTID;
		context2vtext (context, ispanptr, vstuffset);
		if (((typetext *)vstuffset)->length == 0)
			return (FALSE);
		break;
	  case GRANORGL:
	vstuffset = (typevstuffset) taskalloc (taskptr, sizeof (typeaddress));
	((typeitemheader *)vstuffset)->next = NULL;
#ifndef DISTRIBUTION
foocontext ("context after item= ", context);
#endif
		((typeitemheader *)vstuffset)->itemid = ADDRESSID;
		movetumbler (&context->totaloffset.dsas[WIDTH], &((typelink *)vstuffset)->address);
	}
	*vstuffsetptr = vstuffset;
	return (TRUE);
}

context2vtext (context, ispanptr, vstuffset)
  typecontext *context;
  typeispan *ispanptr;
  typevstuffset vstuffset;
{
  typeisa crumistart, crumiend, ispanstart, ispanend;
  INT i, vtlength;

	movetumbler (&context->totaloffset.dsas[WIDTH], &crumistart);
#ifndef DISTRIBUTION
footumbler ("crumistart = ", &crumistart);
#endif
	tumblerincrement (&crumistart, 0, (INT)context->contextinfo.granbottomcruminfo.granstuff.textstuff.textlength, &crumiend);
#ifndef DISTRIBUTION
footumbler ("crumiend = ", &crumiend);
#endif
	movetumbler (&ispanptr->stream, &ispanstart);
	tumbleradd (&ispanstart, &ispanptr->width, &ispanend);
	i = 0;
	vtlength = context->contextinfo.granbottomcruminfo.granstuff.textstuff.textlength;
	if (tumblercmp (&crumistart, &ispanstart) == LESS) {
#ifndef DISTRIBUTION
foo ("first if\n\ti = tumblerintdiff (&ispanstart, &crumistart)\n");
#endif
		i = tumblerintdiff (&ispanstart, &crumistart);
		vtlength -= i;
	}
	if (tumblercmp (&crumiend, &ispanend) == GREATER) {
#ifndef DISTRIBUTION
foo ("second if\n\tvtlength -= tumblerintdiff (&crumiend, &ispanend)\n");
#endif
		vtlength -= tumblerintdiff (&crumiend, &ispanend);
	}
	((typetext *)vstuffset)->length = vtlength > 0 ? vtlength : -vtlength;
	movmem (&context->contextinfo.granbottomcruminfo.granstuff.textstuff.textstring[i], ((typetext *)vstuffset)->string, ((typetext *)vstuffset)->length);
}
