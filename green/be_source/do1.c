/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* xudo1.d - Udanax document handling routines */

#include "xanadu.h"
#include "players.h"


  bool
dofinddocscontaining (taskptr, specset, addresssetptr)
  typetask *taskptr;
  typespecset specset;
  typelinkset *addresssetptr;
{
  typeispanset ispanset;
  bool specset2ispanset(), finddocscontainingsp();

	return (
	   specset2ispanset (taskptr, specset, &ispanset,NOBERTREQUIRED)
	&& finddocscontainingsp (taskptr, ispanset, addresssetptr));
}

  bool
doappend (taskptr, docptr, textset)
  typetask *taskptr;
  typeisa *docptr;
  typetextset textset;
{
  bool appendpm(),insertspanf(); /*zzz dies this put in granf?*/

	return (appendpm (taskptr, docptr, textset)/*&&
       appendpm includes insertspanf!	 insertspanf(taskptr,spanf,docptr,textset,DOCISPAN)*/
	);
}

  bool
dorearrange (taskptr, docisaptr, cutseqptr)
  typetask *taskptr;
  typeisa *docisaptr;
  typecutseq *cutseqptr;
{
  typeorgl docorgl;
  bool findorgl(), rearrangepm();;

	return (
	   findorgl (taskptr, granf, docisaptr, &docorgl, WRITEBERT)
	&& rearrangepm (taskptr, docisaptr, docorgl, cutseqptr)
	/*&& TRUE*/ /* ht stuff */  );
}

  bool
docopy (taskptr, docisaptr, vsaptr, specset)
  typetask *taskptr;
  typeisa *docisaptr;
  tumbler *vsaptr;
  typespecset specset;
{
  typeispanset ispanset;
/*  typeisa htisa;      */
  typeorgl docorgl;
  bool specset2ispanset(), findorgl(), acceptablevsa(), insertpm(), insertspanf();
  bool asserttreeisok();

	return (
	   specset2ispanset (taskptr, specset, &ispanset, NOBERTREQUIRED)
	&& findorgl (taskptr, granf, docisaptr, &docorgl, WRITEBERT)
	&& acceptablevsa (vsaptr, docorgl)
	&& asserttreeisok(docorgl)

	/* the meat of docopy: */
	&& insertpm (taskptr, docisaptr, docorgl, vsaptr, ispanset)

	&&  insertspanf (taskptr, spanf, docisaptr, ispanset, DOCISPAN)
	&& asserttreeisok(docorgl)
/*      &&  ht stuff */ );
}
  bool
docopyinternal (taskptr, docisaptr, vsaptr, specset)
  typetask *taskptr;
  typeisa *docisaptr;
  tumbler *vsaptr;
  typespecset specset;
{
  typeispanset ispanset;
/*  typeisa htisa;      */
  typeorgl docorgl;
  bool specset2ispanset(), findorgl(), acceptablevsa(), insertpm(), insertspanf();
  bool asserttreeisok();

	return (
	   specset2ispanset (taskptr, specset, &ispanset, NOBERTREQUIRED)
	&& findorgl (taskptr, granf, docisaptr, &docorgl, NOBERTREQUIRED)
	&& acceptablevsa (vsaptr, docorgl)
	&& asserttreeisok(docorgl)

	/* the meat of docopy: */
	&& insertpm (taskptr, docisaptr, docorgl, vsaptr, ispanset)

	&&  insertspanf (taskptr, spanf, docisaptr, ispanset, DOCISPAN)
	&& asserttreeisok(docorgl)
/*      &&  ht stuff */ );
}

  typespec spec,spec2,spec3;
  typevstuffset uppervstuffset;
tumbler fivetumbler = {0,0,0,0,500/*100*/,0,0,0,0,0,0,0};
  bool
doinsert (taskptr, docisaptr, vsaptr, textset)
  typetask *taskptr;
  typeisa *docisaptr;
  tumbler *vsaptr;
  typetextset textset;
{
  typehint hint;
  typespanset ispanset;
/* these defs for debug*/
/*  typespan thisspan;*/
  INT ret;
/*  INT temp;*/
  bool doretrievev(), inserttextingranf(), docopy();

/*if(debug){ 
debug = FALSE;
spec.docisa = *docisaptr;
((typeitemheader)spec).next = NULL;
spec.itemid = VSPECID;
spec.vspanset = &thisspan;
thisspan.itemid = VSPANID;
thisspan.next = NULL;
temp = vsaptr->mantissa[1];
thisspan.stream = *vsaptr;
thisspan.width = fivetumbler;
spec.vspanset->stream.mantissa[1] =1// +=5//;
copyspecset(taskptr,&spec,&spec2);
copyspecset(taskptr,&spec,&spec3);
spec3.vspanset->stream.mantissa[1] += textset->length;
doretrievev(taskptr,&spec2,&uppervstuffset);
vsaptr->mantissa[1] = temp;
debug = TRUE;
}*/

	makehint(DOCUMENT, ATOM, TEXTATOM, docisaptr, &hint);
	ret = (inserttextingranf(taskptr, granf, &hint, textset, &ispanset)
		&& docopy (taskptr, docisaptr, vsaptr, ispanset)
	/* no ht stuff here, 'cause it's taken care of in */
	/*   docopy */ );
	return(ret);
}

checkspecandstringbefore()
{
return;
/*if(debug){ assertspecisstring(&spec2,uppervstuffset->xxtest.string); }*/
}

copyspecset (taskptr,specptr, newptr)
  typetask *taskptr;
  typespec *specptr, *newptr;
{
  typespec  *this;
  INT *talloc();
	if(specptr == NULL)
		return;
	this = newptr;
	for(;specptr;specptr=(typespec *)((typeitemheader *)specptr)->next,this=(typespec *)talloc(taskptr,sizeof(typespec))){
		*this = *specptr;
		copyspanset (taskptr,((typevspec *)specptr)->vspanset, &((typevspec *)this)->vspanset);
	}
	((typeitemheader *)this) -> next = NULL;
}

copyspanset (taskptr,spanptr, newptrptr)
  typetask *taskptr;
  typespan *spanptr, **newptrptr;
{
  typespan  *this;
  INT *talloc();

	this = (typespan *)talloc(taskptr,sizeof(typespan));
	*newptrptr = this;
	for (; spanptr; spanptr = spanptr->next, this->next = (typespan *)talloc(taskptr,sizeof(typespan))) {
		*this = *spanptr;
	}
	this ->next = NULL;
}
 
  bool
dodeletevspan(taskptr, docisaptr, vspanptr)
  typetask *taskptr;
  typeisa *docisaptr;
  typevspan *vspanptr;
{
  typeorgl docorgl;
  bool findorgl(), deletevspanpm();

	return (
	   findorgl (taskptr, granf, docisaptr, &docorgl, WRITEBERT)
	&& deletevspanpm (taskptr, docisaptr, docorgl, vspanptr)
	/*&& TRUE*/ /* ht stuff */ );
}

  bool
domakelink (taskptr, docisaptr, fromspecset, tospecset, linkisaptr)
  typetask *taskptr;
  typeisa *docisaptr;
  typespecset fromspecset;
  typespecset tospecset;
  typeisa *linkisaptr;
{
  typehint hint;
  tumbler linkvsa, fromvsa, tovsa;
  typespanset ispanset;
  typesporglset fromsporglset;
  typesporglset tosporglset;
  typeorgl link;
  bool createorglingranf(), insertendsetsinspanf(), insertendsetsinorgl(), docopy();
  bool tumbler2spanset(), findnextlinkvsa(), findorgl(), specset2sporglset(), setlinkvsas();

	makehint (DOCUMENT, ATOM, LINKATOM, docisaptr, &hint);
	return (
	     createorglingranf (taskptr, granf, &hint, linkisaptr)
	  && tumbler2spanset (taskptr, linkisaptr, &ispanset)
	  && findnextlinkvsa (taskptr, docisaptr, &linkvsa)
	  && docopy (taskptr, docisaptr, &linkvsa, ispanset)
	  && findorgl (taskptr, granf, linkisaptr, &link, WRITEBERT)
	  && specset2sporglset (taskptr, fromspecset, &fromsporglset, NOBERTREQUIRED)
	  && specset2sporglset (taskptr, tospecset, &tosporglset, NOBERTREQUIRED)
	  && setlinkvsas (&fromvsa, &tovsa, NULL)
	  && insertendsetsinorgl (taskptr, linkisaptr, link, &fromvsa, fromsporglset, &tovsa, tosporglset, NULL, NULL)
	  && insertendsetsinspanf (taskptr, spanf, linkisaptr, fromsporglset, tosporglset, NULL)
	);
}

  bool
docreatelink (taskptr, docisaptr, fromspecset, tospecset, threespecset, linkisaptr)
  typetask *taskptr;
  typeisa *docisaptr;
  typespecset fromspecset;
  typespecset tospecset;
  typespecset threespecset;
  typeisa *linkisaptr;
{
  typehint hint;
  tumbler linkvsa, fromvsa, tovsa, threevsa;
  typespanset ispanset;
  typesporglset fromsporglset;
  typesporglset tosporglset;
  typesporglset threesporglset;
  typeorgl link;
  bool createorglingranf(), insertendsetsinspanf(), insertendsetsinorgl(), docopy();
  bool tumbler2spanset(), findnextlinkvsa(), findorgl(), specset2sporglset(), setlinkvsas();

	makehint (DOCUMENT, ATOM, LINKATOM, docisaptr, &hint);
	return (
	     createorglingranf (taskptr, granf, &hint, linkisaptr)
	  && tumbler2spanset (taskptr, linkisaptr, &ispanset)
	  && findnextlinkvsa (taskptr, docisaptr, &linkvsa)
	  && docopy (taskptr, docisaptr, &linkvsa, ispanset)
	  && findorgl (taskptr, granf, linkisaptr, &link,/*WRITEBERT ECH 7-1*/NOBERTREQUIRED)
	  && specset2sporglset (taskptr, fromspecset, &fromsporglset,NOBERTREQUIRED)
	  && specset2sporglset (taskptr, tospecset, &tosporglset,NOBERTREQUIRED)
	  && specset2sporglset (taskptr, threespecset, &threesporglset,NOBERTREQUIRED)
	  && setlinkvsas (&fromvsa, &tovsa, &threevsa)
	  && insertendsetsinorgl (taskptr, linkisaptr, link, &fromvsa, fromsporglset, &tovsa, tosporglset, &threevsa, threesporglset)
	  && insertendsetsinspanf (taskptr, spanf, linkisaptr, fromsporglset, tosporglset, threesporglset)
	);
}

  bool
dofollowlink (taskptr, linkisaptr, specsetptr, whichend)
  typetask *taskptr;
  typeisa *linkisaptr;
  typespecset *specsetptr;
  INT whichend;
{
  typesporglset sporglset;
  bool link2sporglset(), linksporglset2specset();

	return (
	   link2sporglset (taskptr, linkisaptr, &sporglset, whichend,NOBERTREQUIRED)
	&& linksporglset2specset (taskptr,&((typesporgl *)sporglset)->sporgladdress, sporglset, specsetptr,/* ECH 6-29 READBERT */NOBERTREQUIRED));

}

  bool
docreatenewdocument (taskptr, isaptr)
  typetask *taskptr;
  typeisa *isaptr;
{
  typehint hint;
  bool createorglingranf();

	makehint (ACCOUNT, DOCUMENT, NULL, &taskptr->account, &hint);
	return (createorglingranf (taskptr, granf, &hint, isaptr));
}

  bool
docreatenode_or_account (taskptr, isaptr)
  typetask *taskptr;
  typeisa *isaptr;
{
  typeisa isa;
  typehint hint;
  bool createorglingranf();

	tumblercopy(isaptr, &isa);
	makehint (NODE, NODE, NULL, /*&taskptr->account*/&isa, &hint);
	return createorglingranf (taskptr, granf, &hint, &isa);
}

  bool
docreatenewversion (taskptr, isaptr, wheretoputit, newisaptr)
  typetask *taskptr;
  typeisa *isaptr, *wheretoputit, *newisaptr;
{
  typehint hint;
  typevspan vspan;
  typevspec vspec;
  tumbler newtp;	/* for internal open */
  bool doretrievedocvspanfoo(), createorglingranf();

	/* ECH 7-13 introduced test for ownership to do right thing for explicit creation
	   of new version of someone else's document */
	if (tumbleraccounteq(isaptr, wheretoputit) && isthisusersdocument(isaptr)) {
		makehint (DOCUMENT, DOCUMENT, NULL, isaptr/*wheretoputit*/, &hint);
	} else {
		/* This does the right thing for new version of someone else's document, as it
		   duplicates the behavior of docreatenewdocument */
		makehint (ACCOUNT, DOCUMENT, NULL, wheretoputit, &hint);
	}
	if (!createorglingranf(taskptr, granf, &hint, newisaptr)) {
		return (FALSE);
	}

	if (!doretrievedocvspanfoo (taskptr, isaptr, &vspan))
		return FALSE;

	vspec.next = NULL;
	vspec.itemid = VSPECID;
	movetumbler(isaptr, &vspec.docisa);
	vspec.vspanset = &vspan;

	/* BERTMODEONLY to prevent recursive createnewversions */
	if (!doopen(taskptr, newisaptr, &newtp, WRITEBERT, BERTMODEONLY, user)) {
#ifndef DISTRIBUTION
fprintf(stderr,"\nisaptr-> ");dumptumbler(isaptr);
fprintf(stderr,"\nwheretoputit-> ");dumptumbler(wheretoputit);
fprintf(stderr,"\nnewisaptr-> ");dumptumbler(newisaptr);
fprintf(stderr,"\nnewtp: ");dumptumbler(&newtp);
		gerror("Couldn't do internal doopen for new doc in docreatenewversion\n");
#else
		gerror("");
#endif
	}
	docopyinternal(taskptr, newisaptr, &vspan.stream, &vspec);
	doclose(taskptr, newisaptr, user);

	return (TRUE);
}

  bool
doretrievedocvspanfoo (taskptr, docisaptr, vspanptr) /* Internal routine, no open required */
  typetask *taskptr;
  typeisa *docisaptr;
  typevspan *vspanptr;
{/* this routine is a kluge not yet kluged*/
  typeorgl docorgl;
  bool findorgl(), retrievedocumentpartofvspanpm();

	return (
	   findorgl (taskptr, granf, docisaptr, &docorgl, NOBERTREQUIRED)
	&& retrievedocumentpartofvspanpm (taskptr, docorgl, vspanptr) );
}


  bool
doretrievedocvspan (taskptr, docisaptr, vspanptr)
  typetask *taskptr;
  typeisa *docisaptr;
  typevspan *vspanptr;
{
  typeorgl docorgl;
  bool findorgl(), retrievevspanpm();

	return
	   findorgl (taskptr, granf, docisaptr, &docorgl, READBERT)
	&& retrievevspanpm (taskptr, docorgl, vspanptr);
}

  bool
doretrievedocvspanset (taskptr, docisaptr, vspansetptr)
  typetask *taskptr;
  typeisa *docisaptr;
  typevspanset *vspansetptr;
{
  typeorgl docorgl;
  bool findorgl(), isemptyorgl(), retrievevspansetpm();

	return
	   findorgl (taskptr, granf, docisaptr, &docorgl, READBERT)
	&& !isemptyorgl (docorgl)
	&& retrievevspansetpm (taskptr, docorgl, vspansetptr);
}

  bool
doretrievev (taskptr, specset, vstuffsetptr)
  typetask *taskptr;
  typespecset specset;
  typevstuffset *vstuffsetptr;
{
  typeispanset ispanset;
  bool specset2ispanset(), ispanset2vstuffset();

	return
	   specset2ispanset (taskptr, specset, &ispanset,READBERT)
	&& ispanset2vstuffset (taskptr, granf, ispanset, vstuffsetptr);
}

  bool
dofindlinksfromtothree (taskptr, fromvspecset, tovspecset, threevspecset, orglrangeptr, linksetptr)
  typetask *taskptr;
  typespecset fromvspecset;
  typespecset tovspecset;
  typespecset threevspecset;
  typeispan *orglrangeptr;
  typelinkset *linksetptr;
{
  bool findlinksfromtothreesp();

	return findlinksfromtothreesp(taskptr, spanf, fromvspecset, tovspecset, threevspecset, orglrangeptr, linksetptr);
}

  bool
dofindnumoflinksfromtothree (taskptr, fromvspecset, tovspecset, threevspecset, orglrangeptr, numptr)
  typetask *taskptr;
  typespecset *fromvspecset;
  typespecset *tovspecset;
  typespecset *threevspecset;
  typeispan *orglrangeptr;
  INT *numptr;
{
  bool findnumoflinksfromtothreesp();

	return findnumoflinksfromtothreesp (taskptr, spanf, fromvspecset, tovspecset, threevspecset, orglrangeptr, numptr);
}

  bool
dofindnextnlinksfromtothree (taskptr, fromvspecptr, tovspecptr, threevspecptr, orglrangeptr, lastlinkisaptr, nextlinksetptr, nptr)
  typetask *taskptr;
  typevspec *fromvspecptr;
  typevspec *tovspecptr;
  typevspec *threevspecptr;
  typeispan *orglrangeptr;
  typeisa *lastlinkisaptr;
  typelinkset *nextlinksetptr;
  INT *nptr;
{
  bool findnextnlinksfromtothreesp();

	return findnextnlinksfromtothreesp (taskptr, fromvspecptr, tovspecptr, threevspecptr, orglrangeptr, lastlinkisaptr, nextlinksetptr, nptr);
}

  bool
doretrieveendsets(taskptr, specset, fromsetptr, tosetptr, threesetptr)
  typetask *taskptr;
  typespecset specset;
  typespecset *fromsetptr;
  typespecset *tosetptr;
  typespecset *threesetptr;
{
  bool retrieveendsetsfromspanf();

	 return retrieveendsetsfromspanf(taskptr, specset, fromsetptr, tosetptr, threesetptr);
}


  bool
doshowrelationof2versions(taskptr, version1, version2, relation)
  typetask *taskptr;
  typespecset version1;
  typespecset version2;
  typespanpairset *relation;
{
  typeispanset version1ispans = NULL;
  typeispanset version2ispans = NULL;
  typeispanset commonispans = NULL;
  bool specset2ispanset();
  bool intersectspansets();
  bool ispansetandspecsets2spanpairset();

	return
		specset2ispanset(taskptr, version1, &version1ispans, READBERT)
	  &&    specset2ispanset(taskptr, version2, &version2ispans, READBERT)
	  &&    intersectspansets(taskptr, version1ispans, version2ispans, &commonispans, ISPANID)
	  &&    ispansetandspecsets2spanpairset(taskptr, commonispans, version1, version2, relation)
	;
}

 
/* --------------- punt line ----------------- */

/* donavigateonht */
