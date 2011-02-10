/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* orglinks.d - permutation matrix enfilade calls */
/*      xuorglinks.d - link following routines included  */
 
#include "xanadu.h"
#include "enf.h"
#include "players.h"


  bool
appendpm(taskptr, docisaptr, textset)
  typetask *taskptr;
  typeisa *docisaptr;
  typetextset textset;
{
  typeorgl orglptr;
  tumbler vsa;
  bool findorgl(), doinsert();

	if (!findorgl(taskptr, granf, docisaptr, &orglptr, WRITEBERT))
		return(FALSE);
	findvsatoappend((typecorecrum*)orglptr, &vsa);
	return (doinsert(taskptr, docisaptr, &vsa, textset));
}

findvsatoappend (ptr, vsaptr)
  typecorecrum *ptr;
  tumbler *vsaptr;
{
  tumbler linkspacevstart;
  typewid offset, grasp, reach;

	clear (&offset, sizeof(offset));
	tumblerclear (vsaptr);
	tumblerclear (&linkspacevstart);
	tumblerincrement (&linkspacevstart, 0, 2, &linkspacevstart);
	prologuend (ptr, &offset, &grasp, &reach);
	if (
	   iszerolock ((tumbler*)&ptr->cwid, widsize(POOM))
	|| (tumblercmp (&grasp.dsas[V], &linkspacevstart) != LESS)) {
		tumblerincrement (vsaptr, 0, 1, vsaptr); /* no text in doc */
		tumblerincrement (vsaptr, 1, 1, vsaptr);
	} else if (tumblercmp (&reach.dsas[V], &linkspacevstart) == LESS) {
		movetumbler (&reach.dsas[V], vsaptr); /* no links in doc */
	} else {
		findnextaddressinvspace (ptr, &grasp, &linkspacevstart, vsaptr);
	}
}

findnextaddressinvspace (crumptr, offsetptr, nextvspacestartptr, vsaptr)
  typecorecrum *crumptr;
  typewid *offsetptr;
  tumbler *nextvspacestartptr, *vsaptr;
{
  typecorecrum *ptr;
  typewid reach, grasp;
  tumbler maxt;

	if (!(ptr = findleftson((typecuc*)crumptr)))
		return;
	tumblerclear (&maxt);
	for (; ptr; ptr = findrightbro(ptr)) {
		prologuend (ptr, offsetptr, &grasp, &reach);
		if(whereoncrum(ptr,offsetptr,nextvspacestartptr,V) == THRUME) {
			findnextaddressinvspace (ptr, &grasp, nextvspacestartptr, vsaptr);
			return;

		} else if (tumblercmp(&grasp.dsas[V],nextvspacestartptr) != GREATER) {
			tumblermax (&reach.dsas[V], &maxt, &maxt);

		}
	}
	movetumbler (&maxt, vsaptr);
}


  bool
insertpm (taskptr, orglisa, orgl, vsaptr, sporglset)
  typetask *taskptr;
  tumbler *orglisa;
  typeorgl orgl;
  tumbler *vsaptr;
  typesporglset sporglset;
{
  tumbler lstream, lwidth;
  type2dbottomcruminfo linfo;
  typewid crumorigin, crumwidth;
  tumbler zero;
  INT shift, inc;

#ifndef DISTRIBUTION
if (debug) {fprintf(stderr,"\nINSERTpm  vsa ");dumptumbler(vsaptr);fprintf(stderr,"\n");dumpitemset(sporglset);}
#endif
	if (iszerotumbler (vsaptr)){
#ifndef DISTRIBUTION
		fprintf(stderr,"insertpm inserting at 0 ---punt zzzz?");
#endif
		return (FALSE);
	}
	tumblerclear (&zero);
	if (tumblercmp (vsaptr, &zero) == LESS)
#ifndef DISTRIBUTION
		gerror ("insertpm called with negative vsa.\n");
#else
		gerror("");
#endif
	logbertmodified(orglisa, user);
	for (; sporglset; sporglset = (typesporglset) sporglset->xxxxsporgl.next) {
		unpacksporgl (sporglset, &lstream, &lwidth, &linfo);
#ifndef DISTRIBUTION
footumbler("lstream = ",&lstream);
#endif
		movetumbler (&lstream, &crumorigin.dsas[I]);
#ifndef DISTRIBUTION
footumbler("lwidth = ",&lwidth);
#endif
		movetumbler (&lwidth, &crumwidth.dsas[I]);
#ifndef DISTRIBUTION
footumbler("vsaptr = ",vsaptr);
#endif
		movetumbler (vsaptr, &crumorigin.dsas[V]);
/*I'm suspissious of this shift <reg> 3/1/85 zzzz*/
		shift = tumblerlength (vsaptr) - 1;
		inc = tumblerintdiff (&lwidth, &zero);
		tumblerincrement (&zero, shift, inc, &crumwidth.dsas[V]);
#ifndef DISTRIBUTION
footumbler("crumwidth = ", &crumwidth.dsas[V]);
#endif
		if (iszerotumbler (&crumwidth.dsas[V]))
#ifndef DISTRIBUTION
			gerror ("crum width 0 in insertpm\n");
#else
			gerror("");
#endif
#ifndef DISTRIBUTION
if (debug) {fprintf(stderr,"    crumorigin: ");dumpwid(&crumorigin,POOM);fprintf(stderr,"    crumwidth: ");dumpwid(&crumwidth,POOM);}
#endif
		insertnd (taskptr, (typecuc*)orgl, &crumorigin, &crumwidth, &linfo, V);
/**/		tumbleradd (vsaptr, &crumwidth.dsas[V], vsaptr);
	}
	return (TRUE);
}


  bool
rearrangepm (taskptr, docisaptr, docorgl, cutseqptr)
  typetask *taskptr;
  tumbler *docisaptr;
  typeorgl docorgl;
  typecutseq *cutseqptr;
{
	rearrangend((typecuc*)docorgl, cutseqptr, V);
	logbertmodified(docisaptr, user);
	return (TRUE);
}


  bool
deletevspanpm(taskptr, docisaptr, docorgl, vspanptr)
  typetask *taskptr;
  tumbler *docisaptr;
  typeorgl docorgl;
  typevspan *vspanptr;
{
	if (iszerotumbler(&vspanptr->width))
		return (FALSE);
	deletend((typecuc*)docorgl, &vspanptr->stream, &vspanptr->width, V);
	logbertmodified(docisaptr, user);
	return (TRUE);
}


  bool
retrievedocumentpartofvspanpm (taskptr, orgl, vspanptr)
  typetask *taskptr;
  typeorgl orgl;
  typevspan *vspanptr;
{ /* this is a kluge*/
	vspanptr->next = NULL;
	vspanptr->itemid = VSPANID;
	movetumbler (&((typecuc *) orgl)->cdsp.dsas[V], &vspanptr->stream);
	movetumbler (&((typecuc *) orgl)->cwid.dsas[V], &vspanptr->width);
	return (TRUE);
}


  bool
retrievevspanpm (taskptr, orgl, vspanptr)
  typetask *taskptr;
  typeorgl orgl;
  typevspan *vspanptr;
{
	vspanptr->next = NULL;
	vspanptr->itemid = VSPANID;
	movetumbler (&((typecuc *) orgl)->cdsp.dsas[V], &vspanptr->stream);
	movetumbler (&((typecuc *) orgl)->cwid.dsas[V], &vspanptr->width);
	return (TRUE);
}
  bool
retrievevspansetpm (taskptr, orgl, vspansetptr)/* return spans  for doc and link part */
  typetask *taskptr;
  typeorgl orgl;
  typevspanset *vspansetptr;
{
  tumbler voffset;
  tumbler maxwid;
  typevspan vspan;
  typevspan linkvspan;
  bool is1story();
  typecorecrum *ccptr;
	ccptr = (typecorecrum *) orgl;
	tumblerclear (&voffset);
	*vspansetptr = NULL;
	if (is1story (&ccptr->cwid.dsas[V])) { /* if it is  just text return that */
		vspan.itemid = VSPANID;
		movetumbler (&ccptr->cdsp.dsas[V], &vspan.stream);
		movetumbler (&ccptr->cwid.dsas[V], &vspan.width);
		vspan.next = NULL;
		putvspaninlist (taskptr, &vspan, vspansetptr);
		return TRUE;
	} else {
		/* the link part is simple, just grab the last  digit off the wid */
/* the text part we get from a max function that delves into the crums*/
/*  in both cases we have to remove the first digit of the tumbler, the 1 and hack it around a bit. */
		linkvspan.itemid = VSPANID;
		//movetumbler (&ccptr->cdsp.dsas[V], &linkvspan.stream);
		movetumbler (&ccptr->cwid.dsas[V], &linkvspan.stream);
		linkvspan.stream.mantissa[1] = 0;
		tumblerjustify(&linkvspan.stream);

		movetumbler (&ccptr->cwid.dsas[V], &linkvspan.width);
		linkvspan.width.mantissa[1] = 0;
		tumblerjustify(&linkvspan.width);
		linkvspan.next = NULL;
	

		 maxtextwid(taskptr, ccptr, &voffset, &maxwid);
		vspan.itemid = VSPANID;
		tumblerclear (&vspan.stream);
		movetumbler (&maxwid, &vspan.width);
		vspan.width.mantissa[0] = 0;
		vspan.next = NULL;
		/*dumptumbler (&vspan.stream);
		dumptumbler (&vspan.width);*/

		putvspaninlist (taskptr, &vspan, vspansetptr);
		putvspaninlist (taskptr, &linkvspan, vspansetptr);

		return (TRUE);
	}
}
bool istextcrum();
bool islinkcrum();
maxtextwid (taskptr, crumptr, voffset, maxwidptr)
  typetask *taskptr;
  typecorecrum *crumptr;
  tumbler *voffset;
  typevspanset *maxwidptr;
{ 
  typecorecrum *ptr, *findleftson();
  typevspan vspan;
  tumbler localvoffset;
  tumbler tmpwid;
  bool is1story();
	if(!crumptr){
		gerror("no crumptr in maxtextwid");
	}
	if (istextcrum (crumptr)) {
		tumbleradd (voffset, &crumptr->cdsp.dsas[V], &tmpwid);
		tumblermax(&tmpwid,maxwidptr,maxwidptr);
	} else {
		tumbleradd (voffset, &crumptr->cdsp.dsas[V], &localvoffset);
		for(ptr = findleftson((typecuc*)crumptr); ptr; ptr = findrightbro (ptr)){
			if(ptr && !islinkcrum(ptr)){/* only look for text or overlapping stuff */
				maxtextwid(taskptr, ptr, &localvoffset,maxwidptr);
			}
		}
	}
}
bool istextcrum(crumptr)
  typecorecrum *crumptr;
{
	if(crumptr->cdsp.dsas[V].mantissa[1] == 0  && is1story(&crumptr->cwid.dsas[V])){ 

		return TRUE;
	}
	return FALSE;
}

bool islinkcrum(crumptr)
  typecorecrum *crumptr;
{
	if(crumptr->cdsp.dsas[V].mantissa[0] == 1 && crumptr->cdsp.dsas[V].mantissa[1] != 0){ /* if the whold crum is displaced into link space it is a link crum this is true if the tumbler is a 1.n tumbler where n!= 0*/ 
		return TRUE;
	}
	return FALSE;
}

#ifdef UnDEFined


  bool
retrievevspansetpm (taskptr, orgl, vspansetptr)/* return spans  for doc and link part */
  typetask *taskptr;
  typeorgl orgl;
  typevspanset *vspansetptr;
{
  tumbler voffset;

	tumblerclear (&voffset);
	*vspansetptr = NULL;
	walkorglonvpm (taskptr, (typecorecrum*)orgl, &voffset, vspansetptr);
	cleanupvspanlist (taskptr, vspansetptr);
	return (TRUE);
}

walkorglonvpm (taskptr, crumptr, voffset, vspansetptr)
  typetask *taskptr;
  typecorecrum *crumptr;
  tumbler *voffset;
  typevspanset *vspansetptr;
{ 
  typecorecrum *ptr, *findleftson();
  typevspan vspan;
  tumbler localvoffset;
  bool is1story();

	if (is1story (&crumptr->cwid.dsas[V])) {
		vspan.itemid = VSPANID;
		tumbleradd (voffset, &crumptr->cdsp.dsas[V], &vspan.stream);
		movetumbler (&crumptr->cwid.dsas[V], &vspan.width);
		vspan.next = NULL;
		putvspaninlist (taskptr, &vspan, vspansetptr);
	} else {
		tumbleradd (voffset, &crumptr->cdsp.dsas[V], &localvoffset);
		for(ptr = findleftson((typecuc*)crumptr); ptr; ptr = findrightbro (ptr)){
			walkorglonvpm(taskptr, ptr, &localvoffset,vspansetptr);
		}
	}
}
cleanupvspanlist (taskptr, vspansetptr)
  typetask *taskptr;
  typevspanset *vspansetptr;
{
  typevspan *ptr;
  tumbler spanend;
  bool tumblereq();

	if (!(ptr = *vspansetptr))
		return;
	for (; ptr && ptr->next; ptr = ptr->next) {
		tumbleradd (&ptr->stream, &ptr->width, &spanend);
		if (tumblereq (&spanend, &ptr->next->stream)) {
			tumbleradd(&ptr->width,&ptr->next->width, &ptr->width);
			ptr->next = ptr->next->next;
		}
	}
}
#endif

  typevspan *
makevspan (taskptr, spanptr, nextspan)
  typetask *taskptr;
  typevspan *spanptr, *nextspan;
{
  typevspan *ret;
  INT *taskalloc();

	ret = (typevspan *) taskalloc (taskptr, sizeof (typevspan));
	movetumbler (&spanptr->stream, &ret->stream);
	movetumbler (&spanptr->width, &ret->width);
	ret->itemid = VSPANID;
	ret->next = nextspan;
	return (ret);
}


putvspaninlist (taskptr, spanptr, spansetptr)
  typetask *taskptr;
  typevspan *spanptr;
  typevspanset *spansetptr;
{
  typevspan *ptr, *last, *makevspan();
  tumbler newspanend, oldspanend;
  INT startcmp, endcmp, spancmp;
  INT *taskalloc();

	ptr = *spansetptr;
	last = NULL;
	if (!ptr) {
		*spansetptr = makevspan (taskptr, spanptr, (typevspan*)NULL);
		return;
	}
	for (; ptr; last = ptr, ptr = ptr->next) {
		tumbleradd (&spanptr->stream, &spanptr->width, &newspanend);
		tumbleradd (&ptr->stream, &ptr->width, &oldspanend);
		spancmp = tumblercmp (&spanptr->stream, &oldspanend);
		if (!spancmp) {
			tumbleradd (&ptr->width, &spanptr->width, &ptr->width);
			return;
		} else if (spancmp == GREATER)
			continue;
		spancmp = tumblercmp (&ptr->stream, &newspanend);
		if (!spancmp) {
			movetumbler (&spanptr->stream, &ptr->stream);
			tumbleradd (&spanptr->width, &ptr->width, &ptr->width);
			return;
		} else if (spancmp == GREATER) {
			if (ptr != *spansetptr)
				last->next = makevspan (taskptr, spanptr, ptr);
			else
				*spansetptr = makevspan (taskptr, spanptr, ptr);
			return;
		}
		startcmp = tumblercmp (&spanptr->stream, &ptr->stream);
		endcmp = tumblercmp (&newspanend, &oldspanend);
		if (startcmp > LESS && endcmp < GREATER)
			return;
		switch (startcmp) {
		  case EQUAL:
			if (endcmp == GREATER)
				movetumbler (&spanptr->width, &ptr->width);
			return;
		  case LESS:
			movetumbler (&spanptr->stream, &ptr->stream);
			if (endcmp == GREATER)
				movetumbler (&spanptr->width, &ptr->width);
			else
				tumblersub(&oldspanend,&spanptr->stream,&ptr->width);
			break;
		  case GREATER:
			if (endcmp == GREATER) {
			      tumblersub(&newspanend,&ptr->stream,&ptr->width);
			      return;
			}
		}
	}
	last->next = makevspan (taskptr, spanptr, (typevspan*)NULL);
}

  typevspanset *
ispan2vspanset (taskptr, orgl, ispanptr, vspansetptr)
  typetask *taskptr;
  typeorgl orgl;
  typeispan *ispanptr;
  typevspanset *vspansetptr;
{
  typespanset *permute();

	return permute(taskptr, orgl, ispanptr, I, vspansetptr, V);
}


 typeispanset *
vspanset2ispanset (taskptr, orgl, vspanptr, ispansetptr)
  typetask *taskptr;
  typeorgl orgl;
  typevspanset vspanptr;
  typeispanset *ispansetptr;
{
  typespanset *permute();

	return permute(taskptr, orgl, vspanptr, V, ispansetptr, I);
}

  typespanset *
permute(taskptr,orgl,restrictionspanset,restrictionindex,targspansetptr,targindex)
  typetask *taskptr;
  typeorgl orgl;
  typespanset restrictionspanset;
  typespanset *targspansetptr;
  INT restrictionindex, targindex;
{
  typespanset *span2spanset();
  typespanset *save;
  	save = targspansetptr;
#ifndef DISTRIBUTION
foo("entering permute\n");foospanset("targspanset ",*targspansetptr);foospanset("restrictionset is ",restrictionspanset);
#endif
	/*consolidatespans(restrictionspanset);
foospanset("restrictionset after consolidation is ",restrictionspanset);	*/
       for (; restrictionspanset; restrictionspanset = restrictionspanset->next) {
		targspansetptr = span2spanset(taskptr, orgl, restrictionspanset, restrictionindex, targspansetptr, targindex);
	}
#ifndef DISTRIBUTION
foospanset("leaving permute\n",*save);
/*checkpointer ("targspansetptr(permute): ", targspansetptr);*/
#endif
	return (save);
}


  typespanset *
span2spanset (taskptr, orgl, restrictionspanptr, restrictionindex, targspansetptr, targindex)
  typetask *taskptr;
  typeorgl orgl;
  typespanset restrictionspanptr;
  typespanset *targspansetptr;
  INT restrictionindex, targindex;
{
  typecontext *context, *c, *retrieverestricted();
  typespan foundspan;
  typespan *nextptr;
  typeitem *onitemlist();
#ifndef DISTRIBUTION
foo("entering span2spanset\n");
#endif

	context = retrieverestricted((typecuc*)orgl, restrictionspanptr, restrictionindex, (typespan*)NULL, targindex, (typeisa*)NULL);
#ifndef DISTRIBUTION
foocontextlist("in sapan2spanset after retrieverestrictec context =",context);
#endif
	for (c = context; c; c = c->nextcontext) {
#ifndef DISTRIBUTION
foocontext("in span2spanset loop  ",c);
#endif
		context2span (c, restrictionspanptr, restrictionindex, &foundspan, targindex);
		nextptr = (typespan *)onitemlist (taskptr, (typeitem*)&foundspan, (typeitemset*)targspansetptr);
	}
	if(!context){
		return(targspansetptr);
	}
#ifndef DISTRIBUTION
foocontext("leaving span2spanset returning context = \n",context);
#endif
		contextfree (context);
	return (&nextptr->next);
}
/*
consolidatespanset(spanset);
  typespan *spanset;
{
	for(;spanset->next;spanset = spanset->next){
		if(
	}
}*/

  typeitem *
onitemlist (taskptr, itemptr, itemsetptr)
  typetask *taskptr;
  typeitem *itemptr;
  typeitemset *itemsetptr;
{
  typeitem *temp, *newitem;
/*foo("entering onitemlist\n");*/
#ifndef DISTRIBUTION
	if (!itemsetptr)
		gerror ("Null itemsetptr\n");
	if (!itemptr)
		gerror ("Null itemptr\n");
#else
	if (!itemsetptr || !itemptr)
		gerror("");
#endif
	switch(((typeitemheader *)itemptr)->itemid){  /* allocate and copy to proper sized item*/
	  case TEXTID:
		newitem = (typeitem *) taskalloc(taskptr, sizeof(typetext));
		movmem (itemptr, newitem, sizeof(typetext));
		break;
	  case ISPANID:
		newitem = (typeitem *) taskalloc(taskptr, sizeof(typeispan));
		movmem (itemptr, newitem, sizeof(typeispan));
		break;

	  case VSPANID:
		newitem = (typeitem *) taskalloc(taskptr, sizeof(typevspan));
		movmem (itemptr, newitem, sizeof(typevspan));
		break;

	  case VSPECID:
		newitem = (typeitem *) taskalloc(taskptr, sizeof(typevspec));
		movmem (itemptr, newitem, sizeof(typevspec));
		break;

	  case NODEID:
		newitem = (typeitem *) taskalloc(taskptr, sizeof(typeboolsetnode));
		movmem (itemptr, newitem, sizeof(typeboolsetnode));
		break;

	  case ADDRESSID: /* also LINKID*/
		newitem = (typeitem *) taskalloc(taskptr, sizeof(typeaddress));
		movmem (itemptr, newitem, sizeof(typeaddress));
		break;

	  case SPORGLID: /* zzz kluge sporglitem is union sporgl is struct which should i use here i.e. how is this used and set  in fact sporgl happens to be big member in union but this is troubles me  REG 1/5/86*/
		newitem = (typeitem *) taskalloc(taskptr, sizeof(typesporglitem));
		movmem (itemptr, newitem, sizeof(typesporglitem));
		break;

	 default:
		gerror("improper item");


	}
	((typeitemheader *)newitem)->next = NULL;
	if (*itemsetptr == NULL){
		*itemsetptr = newitem;
	  } else {
		/* this loop advances the ptr ot the end of the list */
		for (temp = *itemsetptr;
#ifndef DISTRIBUTION
				checkitem ("onitem2.5: ", temp),
				checkitem ("onitem2.75: ", ((typeitemheader *)temp)->next),
#endif
				temp && ((typeitemheader *)temp)->next;
				temp = (typeitem *)((typeitemheader *)temp)->next)
#ifndef DISTRIBUTION
			if(debug) {checkitem ("onitem3: ", temp);}
#else
			;
#endif
		((typeitemheader *)temp)->next = (typeitemheader *)newitem;
	}
	return (newitem);
}

  bool
isemptyorgl (fullcrumptr)
  typeorgl fullcrumptr;
{
	return (
	   iszerolock ((tumbler*)&((typecuc*)fullcrumptr)->cwid, widsize(POOM))
	&& iszerolock ((tumbler*)&((typecuc*)fullcrumptr)->cdsp, dspsize(POOM)));
}

