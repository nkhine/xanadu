/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* Udanax Back-end routines to handle sporgls */

#include "xanadu.h"
#include "enf.h"

  bool
specset2sporglset (taskptr, specset, sporglsetptr,type)
  typetask *taskptr;
  typespecset specset;
  typesporglset *sporglsetptr;
  int type;
{
  typesporglset *vspanset2sporglset();

	*sporglsetptr = NULL;
	for (; specset; specset = (typespecset)((typeitemheader*)specset)->next) {
		if (((typeitemheader *)specset)->itemid == ISPANID) {
			*sporglsetptr = (typesporglset)specset;
			sporglsetptr = (typesporglset *)&((typeitemheader *)specset)->next;
		} else if (((typeitemheader *)specset)->itemid == VSPECID) {

			if (!(sporglsetptr = vspanset2sporglset (taskptr, &((typevspec *)specset)->docisa, ((typevspec *)specset)->vspanset, sporglsetptr,type))){
				return (FALSE);
			}
		}
	}
	*sporglsetptr = NULL;

	return (TRUE);
}

  typesporglset *
vspanset2sporglset (taskptr, docisa, vspanset, sporglsetptr,type)
  typetask *taskptr;
  typeisa *docisa;
  typevspanset vspanset;
  typesporglset *sporglsetptr;
  int type;
{
  typeorgl orgl;
  typesporgl *sporglset;
  typeispanset ispanset, *vspanset2ispanset();
  INT *taskalloc();
  bool findorgl();

	ispanset = NULL;
	if (!findorgl (taskptr, granf, docisa, &orgl,type)){
		return (NULL);
	}
	for (; vspanset; vspanset = vspanset->next) {
		(void) vspanset2ispanset (taskptr, orgl, vspanset, &ispanset);
		for (; ispanset; ispanset = ispanset->next) {
		       sporglset = (typesporgl *) taskalloc (taskptr, sizeof(typesporgl));
		       sporglset->itemid = SPORGLID;
		       sporglset->next = NULL;
		       movetumbler (docisa, &sporglset->sporgladdress);
		       movetumbler(&ispanset->stream,&sporglset->sporglorigin);
		       movetumbler (&ispanset->width, &sporglset->sporglwidth);
		       *sporglsetptr = (typesporglset)sporglset;
		       sporglsetptr = (typesporglset *)&sporglset->next;
		}
	}
	/* note that this returns the LAST sporgl alloced this is ok
	   the returned value gets passed back to here to be used
	   for a linked list
	*/
	return (sporglsetptr);
}

  bool
link2sporglset (taskptr, linkisa, sporglsetptr, whichend,type)
  typetask *taskptr;
  typeisa *linkisa;
  typesporglset *sporglsetptr;
  INT whichend;
  int type;
{
  typeorgl orgl;
  tumbler zero;
  typevspan vspan;
  typecontext *context, *c, *retrieverestricted();
  typesporgl *sporglptr;
  INT *taskalloc();
  bool findorgl();

	if (!findorgl (taskptr, granf, linkisa, &orgl,type)){
		return (FALSE);
	}
	tumblerclear (&zero);
	tumblerincrement (&zero, 0, whichend, &vspan.stream);
	tumblerincrement (&zero, 0/*1*/, 1, &vspan.width);
	if (context = retrieverestricted((typecuc*)orgl, &vspan ,V, (typespan*)NULL, I, (typeisa*)NULL)) {
		for (c = context; c; c = c->nextcontext) {
			sporglptr = (typesporgl *)taskalloc(taskptr,sizeof (typesporgl));
			contextintosporgl ((type2dcontext*)c, (tumbler*)NULL, sporglptr, I);
			*sporglsetptr = (typesporglset)sporglptr;
			sporglsetptr = (typesporglset *)&sporglptr->next;
		}
		contextfree (context);
		return (TRUE);
	} else{
		return (FALSE);
	}
}

  bool
linksporglset2specset (taskptr, homedoc, sporglset, specsetptr,type)
  typetask *taskptr;
  typeisa *homedoc;
  typesporglset sporglset;
  typespecset *specsetptr;
  int type;
{
  typespecset specset;
  INT *taskalloc();

	*specsetptr = NULL;
	for (; sporglset; sporglset = (typesporglset)((typeitemheader*)sporglset)->next) {
		specset=(typespecset) taskalloc (taskptr, sizeof (typevspec));
		if (iszerotumbler (&((typesporgl *)sporglset)->sporgladdress)) {
			if (iszerotumbler (&((typesporgl *)sporglset)->sporglwidth))
#ifndef DISTRIBUTION
			  gerror("zero wid I span in linksporglset2specset\n");
#else
			  gerror("");
#endif
			((typeitemheader *)specset)->itemid = ISPANID;
			movetumbler(&((typesporgl *)sporglset)->sporglorigin,&((typeispan *)specset)->stream);
			movetumbler(&((typesporgl *)sporglset)->sporglwidth,&((typeispan *)specset)->width);
		} else {
		       linksporglset2vspec(taskptr, homedoc, &sporglset, (typevspec*)specset, type);
		}
		((typeitemheader *)specset)->next = NULL;
		*specsetptr = specset;
		specsetptr = (typespecset *)&((typeitemheader *)specset)->next;
	}
	return (TRUE);
}

/* leaves sporglsetptr on last sporgl processed, NOT next to be processed */

linksporglset2vspec (taskptr, homedoc,  sporglsetptr, specptr,type)
  typetask *taskptr;
  typeisa *homedoc;
  typesporglset *sporglsetptr;
  typevspec *specptr;
  int type;
{
/*  typesporglset sporglset;

	sporglset = *sporglsetptr;*/
	specptr->itemid = VSPECID;
	specptr->next = NULL;
	movetumbler (homedoc/*&sporglset->sporgladdress*/, &specptr->docisa);
	specptr->vspanset = NULL;
	sporglset2vspanset (taskptr,homedoc, sporglsetptr, &specptr->vspanset,type);
}

/* leaves sporglsetptr on last sporgl processed, NOT next to be processed */

sporglset2vspanset (taskptr,homedoc, sporglsetptr, vspansetptr,type)
  typetask *taskptr;
  typeisa *homedoc;
  typesporglset *sporglsetptr;
  typevspanset *vspansetptr;
  int type;
{
  typeorgl orgl;
  typeispan ispan;
/*  tumbler zero; */
  typesporgl *sporglptr;
  typevspanset *ispan2vspanset();
  bool findorgl();

	sporglptr = (typesporgl *)*sporglsetptr;
/*	tumblerclear (&zero); */
	(void) findorgl (taskptr, granf, homedoc/*&sporglptr->sporgladdress*/, &orgl,type);
	ispan.itemid = ISPANID;
	ispan.next = NULL;
	movetumbler (&sporglptr->sporglorigin, &ispan.stream);
	movetumbler (&sporglptr->sporglwidth, &ispan.width);
	vspansetptr = ispan2vspanset (taskptr, orgl, &ispan, vspansetptr);
	for (;;) {
		sporglptr = sporglptr->next;
		if (
		   !sporglptr
		|| !(sporglptr->itemid == SPORGLID)
		|| !tumblereq (&((typesporgl *)sporglptr)->sporgladdress, &((typesporgl *)(*sporglsetptr))->sporgladdress))
			return;
		*sporglsetptr = (typesporglset)sporglptr;
		movetumbler (&sporglptr->sporglorigin, &ispan.stream);
		if (iszerotumbler (&sporglptr->sporglwidth))
#ifndef DISTRIBUTION
		      gerror("2 sporgl ispan width 0 in sporglset2vspanset\n");
#else
			gerror("");
#endif
		movetumbler (&sporglptr->sporglwidth, &ispan.width);
		vspansetptr=ispan2vspanset(taskptr, orgl, &ispan, vspansetptr);
	}
}

unpacksporgl (sporglptr, streamptr, widthptr, infoptr)
  typesporglset sporglptr;
  tumbler *streamptr, *widthptr;
  type2dbottomcruminfo *infoptr;
{
	if (((typeitemheader *)sporglptr)->itemid == ISPANID) {
		movetumbler (&((typeispan *)sporglptr)->stream, streamptr);
		movetumbler (&((typeispan *)sporglptr)->width, widthptr);
		tumblerclear (&infoptr->homedoc);
	} else if (((typeitemheader *)sporglptr)->itemid == SPORGLID) {
		movetumbler (&((typesporgl *)sporglptr)->sporglorigin, streamptr);
		movetumbler (&((typesporgl *)sporglptr)->sporglwidth, widthptr);
		movetumbler (&((typesporgl *)sporglptr)->sporgladdress, &infoptr->homedoc /* should be sourcedoc */);
	} else{
#ifndef DISTRIBUTION
		fprintf (stderr,"unpacksporgl - bad itemid\n");
#else
		;
#endif
	}
	if (iszerotumbler (widthptr)) {
#ifndef DISTRIBUTION
		fprintf (stderr, "sporglitemid = %d\n", ((typeitemheader *)sporglptr)->itemid);
		qerror ("zero width in unpacksporgl\n");
#else
		gerror("");
#endif
	}
}

contextintosporgl (context, linkid, sporglptr, index)
  type2dcontext *context;
  tumbler *linkid;
  typesporgl *sporglptr;
  INT index;
{
	sporglptr->itemid = SPORGLID;
	sporglptr->next = NULL;
	movetumbler(/*linkid*/&context->context2dinfo.homedoc,&sporglptr->sporgladdress);
	/* ^^^^^ zzz foo kluge 11/23/84 ^^^^^ */
	movetumbler(&context->totaloffset.dsas[index],&sporglptr->sporglorigin);
	if (iszerotumbler (&context->contextwid.dsas[index])) {
#ifndef DISTRIBUTION
		gerror ("zero wid in contextintosporgl");
#else
		gerror("");
#endif
	}
	movetumbler (&context->contextwid.dsas[index], &sporglptr->sporglwidth);
}

sporglset2linkset (taskptr, spanfptr, sporglset, linksetptr, homeset, spantype)
  typetask *taskptr;
  typecuc *spanfptr;
  typesporglset sporglset;
  typelinkset *linksetptr;
  typeispan *homeset;
  INT spantype;
{
  typeispan nullhomeset;

	*linksetptr = NULL;
	if (TRUE||!homeset) {
		tumblerclear (&nullhomeset.stream);
		tumblerclear (&nullhomeset.width);
		nullhomeset.width.mantissa[0] = 100;
		nullhomeset.next = NULL;
		homeset = &nullhomeset;
	}
	for (; homeset; homeset = homeset->next) {
		sporglset2linksetinrange (taskptr, spanfptr, sporglset, linksetptr, homeset, spantype);
	}
}

sporglset2linksetinrange (taskptr, spanfptr, sporglset, linksetptr, orglrange, spantype)
  typetask *taskptr;
  typecuc *spanfptr;
  typesporglset sporglset;
  typelinkset *linksetptr;
  typeispan *orglrange;
  INT spantype;
{
  typespan range;
  type2dbottomcruminfo linfo;
  type2dbottomcruminfo *infoptr;
  typecontext *context, *c;
  typecontext *retrieverestricted();
  typeisa linksa;
  bool onlinklist();

	infoptr = &linfo;
	for (; sporglset; sporglset = (typesporglset)((typeitemheader *)sporglset)->next) {
		if (FALSE/*trying to kluge links followable thru versions */&&((typeitemheader *)sporglset)->itemid == SPORGLID) {
			infoptr = &linfo;
			movetumbler (&((typesporgl *)sporglset)->sporgladdress, &linfo.homedoc);
		  } else
			infoptr = NULL;
		if (orglrange) {
			prefixtumbler(&orglrange->stream,spantype,&range.stream);
			prefixtumbler (&orglrange->width, 0, &range.width);
			context = retrieverestricted (spanfptr, (typespan*)sporglset, SPANRANGE, &range, ORGLRANGE, (typeisa*)infoptr);
		} else{
			context = retrieverestricted (spanfptr, (typespan*)sporglset, SPANRANGE, (typespan*)NULL, ORGLRANGE, (typeisa*)infoptr);
		}
		for (c = context; c; c = c->nextcontext) {
			beheadtumbler(&c->totaloffset.dsas[ORGLRANGE],&linksa);
			onlinklist (taskptr, linksetptr, &linksa);
		}
		contextfree (context);
	}
}
