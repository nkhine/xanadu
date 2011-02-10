/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/

/* xudo2.d - miscellaneous bottom-of-the-top routines */

#include "xanadu.h"

  bool
specset2ispanset (taskptr, specset, ispansetptr,type)
  typetask *taskptr;
  typespec *specset;
  typeispanset *ispansetptr;
  int type;
{
  typeorgl docorgl;
  typeispanset *vspanset2ispanset();
  typeispanset *save;
  bool  findorgl();

save = ispansetptr;
	*ispansetptr = NULL;
	for (; specset; specset = (typespec *)((typeitemheader *)specset)->next) {
		if (((typeitemheader *)specset)->itemid == ISPANID) {
			*ispansetptr = (typeispanset)specset;
			ispansetptr = (typeispanset *)&((typeitemheader *)specset)->next;
		} else if (((typeitemheader *)specset)->itemid == VSPECID) {
			if (iszerotumbler (&((typevspec *)specset)->docisa))
#ifndef DISTRIBUTION
				qerror ("retrieve called with docisa 0\n");
#else
		gerror("");
#endif
			if (!(
			  findorgl (taskptr, granf, &((typevspec *)specset)->docisa, &docorgl,type)/*BERT*/
			&& (ispansetptr = vspanset2ispanset (taskptr, docorgl, ((typevspec *)specset)->vspanset, ispansetptr)))){
			       return (FALSE);/*zzzz*/  
			}
		}
	}
/*zzz	*ispansetptr = NULL;  */
#ifndef DISTRIBUTION
fooitemset ("specset2ispanset\nISPANSET\n", *save);
#endif
	return (TRUE);
}

  bool
tumbler2spanset (taskptr, tumblerptr, spansetptr)
  typetask *taskptr;
  typeisa *tumblerptr;
  typespanset *spansetptr;
{
  typespan *spanptr;
  INT *taskalloc();

	spanptr = (typespan *) taskalloc (taskptr, sizeof(typespan));
	spanptr->next = NULL;
	spanptr->itemid = ISPANID;
	movetumbler (tumblerptr, &spanptr->stream);
	tumblerclear (&spanptr->width);
	tumblerincrement (&spanptr->width, tumblerlength (tumblerptr)-1/*zzzzz*/, 1, &spanptr->width);
	*spansetptr = spanptr;
	return (TRUE);
}

/*  bool
spanset2tumbler (taskptr, spanset, tmblptr)
  typetask *taskptr;
  typespanset spanset;
  tumbler *tmblptr;
{
  INT *ealloc(),*eallocwithtag();

	if (spanset->next || !iszerotumbler(&spanset->width))
		return (FALSE);
	tmblptr = (tumbler *) eallocwithtag(sizeof(tumbler),TUMBLERTAG);
	movetumbler (&spanset->stream, tmblptr// SUSPECT //);
	return (TRUE);
}*/

makehint (typeabove, typebelow, typeofatom, isaptr, hintptr)
  INT typeabove, typebelow, typeofatom;
  typeisa *isaptr;
  typehint *hintptr;
{
	hintptr->supertype = typeabove;
	hintptr->subtype = typebelow;
	hintptr->atomtype = typeofatom;
	movetumbler (isaptr, &hintptr->hintisa);
}

validhint (hintptr)
  typehint *hintptr;
{
	if (
	      (hintptr->supertype < NODE)
	   || (hintptr->supertype > DOCUMENT)
	   || (hintptr->subtype < ACCOUNT)
	   || (hintptr->subtype > ATOM)
	   || (hintptr->atomtype < 0)
	   || (hintptr->atomtype > 2)
	   || (hintptr->subtype < hintptr->supertype)
	   || ((hintptr->subtype - hintptr->supertype) > 1)
	   || ((hintptr->subtype == ATOM) == !hintptr->atomtype) ) {
#ifndef DISTRIBUTION
    /*fprintf (stderr,"bad hint: supertype = %d  subtype = %d  atomtype = %d\n",
		      hintptr->supertype, hintptr->subtype, hintptr->atomtype);
		fprintf (stderr,"hintisa = ");
		puttumbler (stderr, &hintptr->hintisa);*/
		gerror ("bad hint");
#else
	gerror("");
#endif
	}
}

  bool  /* vsa 1 if zenf, else w/in vspan of fullcrum */
acceptablevsa (vsaptr, orglptr)
  tumbler *vsaptr;
  typeorgl orglptr;
{
	return (TRUE);
}


  bool
insertendsetsinspanf (taskptr, spanfptr, linkisaptr, fromsporglset, tosporglset, threesporglset)
  typetask *taskptr;
  typespanf spanfptr;
  typeisa *linkisaptr;
  typesporglset fromsporglset;
  typesporglset tosporglset;
  typesporglset threesporglset;
{  bool insertspanf();
  if (!(
	insertspanf(taskptr,spanfptr,linkisaptr,fromsporglset,LINKFROMSPAN)
	  &&insertspanf(taskptr,spanfptr,linkisaptr,tosporglset,LINKTOSPAN)))
		return (FALSE);
  if (threesporglset) {
    if(!insertspanf(taskptr,spanfptr,linkisaptr,threesporglset,LINKTHREESPAN)){
	    return (FALSE);
     }
  }
  return(TRUE);
}

  bool
insertendsetsinorgl (taskptr, linkisaptr, link, fromvsa, fromsporglset, tovsa, tosporglset, threevsa, threesporglset)
  typetask *taskptr;
  tumbler *linkisaptr;
  typeorgl link;
  typesporglset fromsporglset;
  typesporglset tosporglset;
  typesporglset threesporglset;
  tumbler *fromvsa, *tovsa, *threevsa;
{  bool insertpm();
	if (!( insertpm(taskptr, linkisaptr, link, fromvsa, fromsporglset)
		&& insertpm(taskptr, linkisaptr, link, tovsa, tosporglset))) {
			return (FALSE);
	}
	if (threevsa && threesporglset) {
		if (!insertpm (taskptr, linkisaptr, link, threevsa, threesporglset)){
			return (FALSE);
		}
	}else{
#ifndef DISTRIBUTION
		fprintf(stderr,"leaving INSERTENDSETSINORGL 4\n");      
		fprintf(stderr," threevsa ptr= %x threesporglset ptr = %x threesvsa = \n",threevsa, threesporglset);
		dumptumbler(threevsa);
#endif
	}
	return (TRUE);

}

  bool
findnextlinkvsa (taskptr, docisaptr, vsaptr)
  typetask *taskptr;
  typeisa *docisaptr;
  tumbler *vsaptr;
{
  tumbler vspanreach, firstlink;
  typevspan vspan;
  bool doretrievedocvspan();
	tumblerclear (&firstlink);
	tumblerincrement (&firstlink, 0, 2, &firstlink);
	tumblerincrement (&firstlink, 1, 1, &firstlink);

	(void) doretrievedocvspan (taskptr, docisaptr, &vspan);
	tumbleradd (&vspan.stream, &vspan.width, &vspanreach);
	if (tumblercmp (&vspanreach, &firstlink) == LESS)
		movetumbler (&firstlink, vsaptr);
	else
		movetumbler (&vspanreach, vsaptr);
	return (TRUE);
}

  bool
setlinkvsas (fromvsaptr, tovsaptr, threevsaptr)
  tumbler *fromvsaptr, *tovsaptr, *threevsaptr;
{
	tumblerclear (fromvsaptr);
	tumblerincrement (fromvsaptr, 0, 1, fromvsaptr);
	tumblerincrement (fromvsaptr, 1, 1, fromvsaptr);
	tumblerclear (tovsaptr);
	tumblerincrement (tovsaptr, 0, 2, tovsaptr);
	tumblerincrement (tovsaptr, 1, 1, tovsaptr);
	if (threevsaptr) {
		tumblerclear (threevsaptr);
		tumblerincrement (threevsaptr, 0, 3, threevsaptr);
		tumblerincrement (threevsaptr, 1, 1, threevsaptr);
	}
	return (TRUE);
}

  bool
ispansetandspecsets2spanpairset (taskptr, ispanset, specset1, specset2, pairsetptr)
  typetask *taskptr;
  typeispanset ispanset;
  typespecset specset1;
  typespecset specset2;
  typespanpairset *pairsetptr;
{
#ifndef DISTRIBUTION
fprintf(stderr,"entering ispansetandspecsets2spanpairset\n");
#endif
	if (ispanset == NULL) {
		*pairsetptr = NULL;
#ifndef DISTRIBUTION
fprintf(stderr,"ispanset NULL, no relation\n");
#endif
	} else {
	        restrictspecsetsaccordingtoispans(taskptr, ispanset, &specset1, &specset2);
		makespanpairset (taskptr, ispanset, specset1, specset2, pairsetptr);
#ifndef DISTRIBUTION
foo("after makespanpairset\n");
if(debug)dumpspanpairset(*pairsetptr);
#endif
	}
#ifndef DISTRIBUTION
fprintf(stderr,"leaving ispansetandspecsets2spanpairset\n");
#endif
	return (TRUE);
}
