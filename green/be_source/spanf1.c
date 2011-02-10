/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* spanf1.d - Udanax spanfilade calls */
 
#include "xanadu.h"
#include "enf.h"
extern bool isxumain;
extern bool specset2sporglset();
  bool
insertspanf (taskptr, spanfptr, isaptr, sporglset, spantype)
  typetask *taskptr;
  typespanf spanfptr;
  typeisa *isaptr;
  typesporglset sporglset;
  INT spantype;
{
  typedsp crumorigin;
  typewid crumwidth;
  tumbler lstream, lwidth;
  type2dbottomcruminfo linfo; /*zzz 2dinfo ? fix ? 5/27/84 -----*/

        prefixtumbler (isaptr, spantype, &crumorigin.dsas[ORGLRANGE]);
        tumblerclear (&crumwidth.dsas[ORGLRANGE]);
        clear (&linfo, sizeof(linfo));
        for (; sporglset; sporglset = (typesporglset)((typeitemheader *)sporglset)->next) {
                if (((typeitemheader *)sporglset)->itemid == ISPANID) {
                        movetumbler (&((typeispan *)sporglset)->stream, &lstream);
                        movetumbler (&((typeispan *)sporglset)->width, &lwidth);       
                        movetumbler (isaptr,&linfo.homedoc);
                } else if (((typeitemheader *)sporglset)->itemid == SPORGLID) {
                        movetumbler (&((typesporgl *)sporglset)->sporglorigin, &lstream);
                        movetumbler (&((typesporgl *)sporglset)->sporglwidth, &lwidth);
                        movetumbler (&((typesporgl *)sporglset)->sporgladdress,&linfo.homedoc);
                } else if (((typeitemheader *)sporglset)->itemid == TEXTID) {
                        movetumbler(isaptr,&lstream);   

                        /*create lwidth out of sporglset->length*/      
                        tumblerclear(&lwidth);
                        lwidth.mantissa[1] = ((typetext *)sporglset) -> length;
                        tumblerjustify(&lwidth);
                        
                        movetumbler(isaptr,&linfo.homedoc);
                }else
#ifndef DISTRIBUTION
                        gerror ("insertspanf - bad itemid\n");
#else
			gerror("");
#endif
                movetumbler (&lstream, &crumorigin.dsas[SPANRANGE]);
                movetumbler (&lwidth, &crumwidth.dsas[SPANRANGE]);
             insertnd(taskptr,(typecuc*)spanfptr,&crumorigin,&crumwidth,&linfo,SPANRANGE);
        }
        return (TRUE);
}

  bool
findlinksfromtothreesp (taskptr, spanfptr, fromvspecset, tovspecset, threevspecset, orglrange, linksetptr)
  typetask *taskptr;
  typespanf spanfptr;
  typespecset fromvspecset;
  typespecset tovspecset;
  typespecset threevspecset;
  typeispan *orglrange;
  typelinkset *linksetptr;
{
  typesporglset fromsporglset;
  typesporglset tosporglset;
  typesporglset threesporglset;
  typelinkset fromlinkset;
  typelinkset tolinkset;
  typelinkset threelinkset;
bool olddebug = debug;
#ifndef DISTRIBUTION
/*debug = isxumain;*/
if(debug){ fprintf(stderr,"findlinksfromtothreesp fromset = %x, toset = %x, threeset = %x\n",fromvspecset,tovspecset,threevspecset); }
#endif
        fromlinkset = tolinkset = threelinkset = NULL;
        if (fromvspecset)
                specset2sporglset (taskptr, fromvspecset, &fromsporglset,NOBERTREQUIRED);
        if (tovspecset)
                specset2sporglset (taskptr, tovspecset, &tosporglset,NOBERTREQUIRED);
        if (threevspecset)
                specset2sporglset (taskptr, threevspecset, &threesporglset,NOBERTREQUIRED);
        if (fromvspecset) {
                sporglset2linkset (taskptr, (typecuc*)spanfptr, fromsporglset, &fromlinkset, orglrange, LINKFROMSPAN);
                if (!fromlinkset) {
                        *linksetptr = NULL;
debug = olddebug;
                        return (TRUE);
                }
        }
        if (tovspecset) {
                sporglset2linkset (taskptr, (typecuc*)spanfptr, tosporglset, &tolinkset, orglrange, LINKTOSPAN);
                if (!tolinkset) {
                        *linksetptr = NULL;
debug = olddebug;
                        return (TRUE);
                }
        }
        if (threevspecset) {
                sporglset2linkset (taskptr, (typecuc*)spanfptr, threesporglset, &threelinkset, orglrange, LINKTHREESPAN);
                if (!threelinkset) {
                        *linksetptr = NULL;
debug = olddebug;
                        return (TRUE);
                }
        }
       intersectlinksets(taskptr,fromlinkset,tolinkset,threelinkset,linksetptr);
debug = olddebug;
        return (TRUE);
}

  bool
findnumoflinksfromtothreesp (taskptr, spanfptr, fromvspecset, tovspecset, threevspecset, orglrange, numptr)
  typetask *taskptr;
  typespanf spanfptr;
  typespecset fromvspecset;
  typespecset tovspecset;
  typespecset threevspecset;
  typeispan *orglrange;
  INT *numptr;
{
  typelinkset linkset;
  INT n;

        if (!findlinksfromtothreesp (taskptr, spanfptr, fromvspecset, tovspecset, threevspecset, orglrange, &linkset))
                return(FALSE);
        for (n = 0; linkset; linkset = linkset->next, ++n);
        *numptr = n;
        return (TRUE);
}

  bool
findnextnlinksfromtothreesp (taskptr, fromvspecset, tovspecset, threevspecset, orglrangeptr, lastlinkisaptr, nextlinksetptr, nptr)
  typetask *taskptr;
  typespecset fromvspecset;
  typespecset tovspecset;
  typespecset threevspecset;
  typeispan *orglrangeptr;
  typeisa *lastlinkisaptr;
  typelinkset *nextlinksetptr;
  INT *nptr;
{
  INT n;
  typelinkset linkset;

        n = 0;
        *nextlinksetptr = NULL;
        if (!findlinksfromtothreesp (taskptr, spanf, fromvspecset, tovspecset, threevspecset, orglrangeptr, &linkset))
                return (FALSE);
        if (iszerotumbler (lastlinkisaptr)) {
                *nextlinksetptr = linkset;
        } else {
                for (; linkset; linkset = linkset->next) {
                        if (tumblereq (&linkset->address,lastlinkisaptr)) {
                                *nextlinksetptr = linkset->next;
                                break;
                        }
                }
        }
        if (!linkset) {
                *nextlinksetptr = NULL;
                *nptr = 0;
                return (TRUE);
        }
        for (linkset = *nextlinksetptr; linkset; linkset = linkset->next) {
                if (++n >= *nptr) {
                        linkset->next = NULL;
                        break;
                }
        }
        *nptr = n;
        return (TRUE);
}

  bool
finddocscontainingsp (taskptr, ispanset, addresssetptr)
  typetask *taskptr;
  typespanset ispanset;
  typelinkset *addresssetptr;
{
  tumbler docid;
  typecontext *context, *c;
  typecontext *retrieverestricted();
  typelinkset *headptr;
  typelink document;
  typespan docspace;
  typeitem /*typelinkset*/ *onitemlist();
  bool isinlinklist();
#ifndef DISTRIBUTION
if (debug){ fprintf(stderr,"\nFINDDOCSCONTAININGsp\n"); fooitemset ("", ispanset); }
#endif

        headptr = addresssetptr;
        *addresssetptr = NULL;
        clear (&docspace, sizeof(typespan));
        tumblerincrement (&docspace.stream, 0, DOCISPAN, &docspace.stream);
        tumblerincrement (&docspace.width, 0, 1, &docspace.width);
        for (; ispanset; ispanset = ispanset->next) {
                context = retrieverestricted ((typecuc*)spanf, &docspace, ORGLRANGE, ispanset, SPANRANGE, (typeisa*)NULL);
                for (c = context; c; c = c->nextcontext) {
                        movetumbler (&c->totaloffset.dsas[ORGLRANGE], &docid);
                        beheadtumbler (&docid, &document.address);
                        if (isinlinklist(*headptr, &document.address))
                                continue;
                        document.itemid = LINKID;
                        document.next = NULL;
                        addresssetptr = (typelinkset *) onitemlist(taskptr, (typeitem*)&document, (typeitemset*)addresssetptr);
                }
                contextfree (context);
        }
#ifndef DISTRIBUTION          
if(debug) fooitemset("",*headptr);
#endif

        return (TRUE);
}

  bool
retrieveendsetsfromspanf (taskptr, specset, fromsetptr, tosetptr, threesetptr)
  typetask *taskptr;
  typespecset specset;
  typespecset *fromsetptr;
  typespecset *tosetptr;
  typespecset *threesetptr;
{
  typespan fromspace, tospace, threespace;
  typesporglset sporglset;
  typesporglset fromsporglset;
  typesporglset tosporglset;
  typesporglset threesporglset;
  bool bang(),retrievesporglsetinrange(),linksporglset2specset();
  bool temp;
#ifndef DISTRIBUTION
if (debug) {
fprintf(stderr,"retrieveendsetsfromspanf\n");
fooitemset ("", specset);
}
#endif

        clear (&fromspace, sizeof(typespan));
        clear (&tospace, sizeof(typespan));
        clear (&threespace, sizeof(typespan));

        fromspace.stream.mantissa[0] = LINKFROMSPAN;
        fromspace.width.mantissa[0] = 1;

        tospace.stream.mantissa[0] = LINKTOSPAN;
        tospace.width.mantissa[0] = 1;

        threespace.stream.mantissa[0] = LINKTHREESPAN;
        threespace.width.mantissa[0] = 1;

        fromsporglset = tosporglset = threesporglset = NULL;


        if (!(specset2sporglset (taskptr, specset, &sporglset, NOBERTREQUIRED)
        && retrievesporglsetinrange(taskptr,sporglset,&fromspace,&fromsporglset)
        && linksporglset2specset (taskptr,&((typevspec *)specset)->docisa, fromsporglset, fromsetptr, NOBERTREQUIRED)
        && retrievesporglsetinrange (taskptr, sporglset, &tospace, &tosporglset)
        && linksporglset2specset (taskptr, &((typevspec*)specset)->docisa, tosporglset, tosetptr, NOBERTREQUIRED))){
                return (FALSE);
	}
        if (threesetptr) {
              temp = (retrievesporglsetinrange(taskptr, sporglset, &threespace, &threesporglset)
                  && linksporglset2specset(taskptr,&((typevspec *)specset)->docisa, threesporglset,threesetptr, NOBERTREQUIRED));
		return(temp);
	}
	return(TRUE);
}

  bool
retrievesporglsetinrange (taskptr, sporglptr, whichspace, sporglsetptr)
  typetask *taskptr;
  typesporglset sporglptr;
  typespan *whichspace;
  typesporglset *sporglsetptr;
{
  typecontext *context, *c, *tmp;
  typecontext *retrieverestricted();
  typesporgl *sporglset;
  INT *taskalloc();

        for (; sporglptr; sporglptr = (typesporglset)sporglptr->xxxxsporgl.next) {
               context = retrieverestricted((typecuc*)spanf, (typespan*)sporglptr, SPANRANGE, whichspace, ORGLRANGE, (typeisa*)NULL/*kluge to make links show thru to version &sporglptr->sporgladdress*/);

/*dumpcontextlist(context);*/
                for (c = context; c;) {
                        sporglset = (typesporgl*) taskalloc(taskptr, sizeof(typesporgl));
                        contextintosporgl((type2dcontext*)c, (tumbler*)NULL, sporglset, SPANRANGE);
                        *sporglsetptr = (typesporglset)sporglset;
                        sporglsetptr = (typesporglset*)&sporglset->next;

                        tmp = c->nextcontext;
			/*if (c->contexttype == GRAN)
			    --contextnum;
			 else
			    --c2dontextnum;

                        efree((char*)c);*/

                        c = tmp;
                }
			contextfree(context);
        }
        return (TRUE);
}
