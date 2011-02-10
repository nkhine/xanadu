/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* Fns.d -  Udanax Back-end Top Level Functions */
/* Created  7/25/80 */

#include "xanadu.h"
#include "players.h"
#include "requests.h"

extern FILE *interfaceinput;

extern int backenddaemon;

void finddocscontaining (taskptr)
  typetask *taskptr;
{
  typespecset specset;
  typelinkset addressset;
  bool getfinddocscontaining(), dofinddocscontaining();

	if (
	   getfinddocscontaining (taskptr, &specset)
	&& dofinddocscontaining (taskptr, specset, &addressset))
		putfinddocscontaining (taskptr, (typeitemset)addressset);
	   else
		putrequestfailed (taskptr);
}


 void copy (taskptr)
  typetask *taskptr;
{
  typeisa docisa, vsa;
  typespecset localspecset;
  bool getcopy(), docopy();
	
	if (
	   getcopy (taskptr, &docisa, &vsa, &localspecset)
	&& docopy (taskptr, &docisa, &vsa, localspecset))
		putcopy (taskptr);
	  else
		putrequestfailed (taskptr);
}

#ifdef UnDEfined
void copy (taskptr) /* kluged unix version for speed */
  typetask *taskptr;
{
  typeisa docisa, vsa;
  typespecset localspecset;
  bool getcopy(), docopy();
 
	(void) getcopy (taskptr, &docisa, &vsa, &localspecset);
	(void) putcopy (taskptr);
	if (!docopy (taskptr, &docisa, &vsa, /*& ECH according to lint*/&localspecset))/* zzz aug 21 1999 reg ? this may cause trouble **/
#ifndef DISTRIBUTION
		fprintf(stderr,"copy failed \n");
#else
	;
#endif
}
#endif


/*void insert (taskptr)
  typetask *taskptr;
{
  typeisa docisa, vsa;
  typetextset textset;
  bool getinsert(), doinsert();

	if (
	   getinsert (taskptr, &docisa, &vsa, &textset)
	&& doinsert (taskptr, &docisa, &vsa, textset))
		putinsert (taskptr);
	  else
		putrequestfailed (taskptr);
}*/

void insert (taskptr)/* cheating version for unix zzz */
  typetask *taskptr;
{
  typeisa docisa, vsa;
  typetextset textset;
  bool getinsert(), doinsert();
 
	   (void) getinsert (taskptr, &docisa, &vsa, &textset);
	   putinsert (taskptr);
	   if (!doinsert (taskptr, &docisa, &vsa, textset))
#ifndef DISTRIBUTION
		fprintf(stderr,"requestfailed in insert\n");
#else
	;
#endif
}

void createlink (taskptr)
  typetask *taskptr;
{
  typeisa docisa, linkisa;
  typespecset fromspecset, tospecset, threespecset;
  bool getcreatelink(), docreatelink();

	if (
	   getcreatelink (taskptr, &docisa, &fromspecset, &tospecset, &threespecset)
	&& docreatelink (taskptr, &docisa, fromspecset, tospecset, threespecset, &linkisa)){
		putcreatelink (taskptr, &linkisa);
	}else
		putrequestfailed (taskptr);
}

void followlink (taskptr)
  typetask *taskptr;
{
  typeisa linkisa;
  typespecset specset;
  INT whichend;
  bool getfollowlink(), dofollowlink();

	if (
	   getfollowlink (taskptr, &linkisa, &whichend)
	&& dofollowlink (taskptr, &linkisa, &specset, whichend)){
		putfollowlink (taskptr, specset);
	  }else
		putrequestfailed (taskptr);
}

void retrievedocvspanset (taskptr)
  typetask *taskptr;
{
  typeisa docisa;
  typevspanset vspanset;
  bool getretrievedocvspanset(), doretrievedocvspanset();

	if (
	   getretrievedocvspanset (taskptr, &docisa)
	&& doretrievedocvspanset (taskptr, &docisa, &vspanset))
		putretrievedocvspanset (taskptr, &vspanset);
	  else
		putrequestfailed (taskptr);
}

/* void rearrange (taskptr)
  typetask *taskptr;
{
  typeisa docisa;
  typecutseq cutseq;
  bool getrearrange(), dorearrange();

	if (
	   getrearrange (taskptr, &docisa, &cutseq)
	&& dorearrange (taskptr, &docisa, &cutseq))
		putrearrange (taskptr);
	  else
		putrequestfailed (taskptr);
}
*/

void rearrange (taskptr) /* speed hack for unix */
  typetask *taskptr;
{
  typeisa docisa;
  typecutseq cutseq;
  bool getrearrange(), dorearrange();
 
	(void) getrearrange (taskptr, &docisa, &cutseq);
	putrearrange (taskptr);
	if (!dorearrange (taskptr, &docisa, &cutseq))
#ifndef DISTRIBUTION
		fprintf(stderr,"rearrange failed \n");
#else
		;
#endif
}

void retrievev (taskptr)
  typetask *taskptr;
{
  typespecset specset;
  typevstuffset vstuffset;
  bool getretrievev(), doretrievev();

       if (
	  getretrievev (taskptr, &specset)
       && doretrievev (taskptr, specset, &vstuffset))
		putretrievev (taskptr, &vstuffset);
	 else
		putrequestfailed (taskptr);
}

void findlinksfromtothree (taskptr)
  typetask *taskptr;
{
  typespecset fromvspecset, tovspecset, threevspecset;
  typeispanset homeset;
  typelinkset linkset;
  bool getfindlinksfromtothree(), dofindlinksfromtothree();

	if (
	   getfindlinksfromtothree (taskptr, &fromvspecset, &tovspecset, &threevspecset, &homeset)
	&& dofindlinksfromtothree (taskptr, fromvspecset, tovspecset, threevspecset, (typeispan*)NULL/*homeset*/, &linkset))
		putfindlinksfromtothree (taskptr, linkset);
	  else
		putrequestfailed (taskptr);
}

void findnumoflinksfromtothree (taskptr)
  typetask *taskptr;
{
  typespecset fromvspecset, tovspecset, threevspecset;
  typeispanset homeset;
  INT numberoflinks;
  bool getfindnumoflinksfromtothree(), dofindnumoflinksfromtothree();

	if (
	   getfindnumoflinksfromtothree (taskptr, &fromvspecset, &tovspecset, &threevspecset, &homeset)
	&& dofindnumoflinksfromtothree (taskptr, fromvspecset, tovspecset, threevspecset, homeset, &numberoflinks))
		putfindnumoflinksfromtothree (taskptr, numberoflinks);
	  else
		putrequestfailed (taskptr);
}

void findnextnlinksfromtothree (taskptr)
  typetask *taskptr;
{
  typespecset fromvspecset, tovspecset, threevspecset;
  typeispanset homeset;
  typeisa lastlink;
  typelinkset nextlinkset;
  INT n;
  bool getfindnextnlinksfromtothree(), dofindnextnlinksfromtothree();

	if (
	   getfindnextnlinksfromtothree (taskptr, &fromvspecset, &tovspecset, &threevspecset, &homeset, &lastlink, &n)
	&& dofindnextnlinksfromtothree (taskptr, fromvspecset, tovspecset, threevspecset, homeset, &lastlink, &nextlinkset, &n))
		putfindnextnlinksfromtothree (taskptr, n, nextlinkset);
	  else
		putrequestfailed (taskptr);
}

void navigateonht (taskptr)
  typetask *taskptr;
{
#ifndef DISTRIBUTION
error (taskptr, "GACK !  (historical trace)\n");
#endif
/*
  typeisa docisa, htisa;
  typehtpath turninginstructions;
  bool getnavigateonht();
	if (getnavigateonht(taskptr, &docisa, &htisa, &turninginstructions))
	   donavigateonht(taskptr, &docisa, &htisa, &turninginstructions);
*/
}

void showrelationof2versions (taskptr)
  typetask *taskptr;
{
  typespecset version1, version2;
  typespanpairset relation;
  bool getshowrelationof2versions(), doshowrelationof2versions();

	if (getshowrelationof2versions (taskptr, &version1, &version2)
	 && doshowrelationof2versions (taskptr, version1, version2, &relation))
		putshowrelationof2versions (taskptr, relation);
	else
		putrequestfailed (taskptr);
}

/*
void showrelationof2versions (taskptr)
  typetask *taskptr;
{
  typeisa doc1, doc2;
  INT relation;	// temp //
  bool getshowrelationof2versions(), doshowrelationof2versions();

	if (getshowrelationof2versions(taskptr, &doc1, &doc2))
	   if (doshowrelationof2versions(taskptr, &doc1, &doc2,  &relation))
	       putshowrelationof2versions(taskptr, &relation);
}
*/

void createnewdocument (taskptr)
  typetask *taskptr;
{
  typeisa newdocisa;
  bool docreatenewdocument();

	getcreatenewdocument();
	if (docreatenewdocument (taskptr, &newdocisa))
		putcreatenewdocument (taskptr, &newdocisa);
	  else
		putrequestfailed (taskptr);
}


void createnewversion (taskptr)
  typetask *taskptr;
{
  typeisa originaldocisa, newdocisa;
  bool getcreatenewversion(), docreatenewversion();

	if (
	   getcreatenewversion (taskptr, &originaldocisa)
	&& docreatenewversion (taskptr, &originaldocisa, &originaldocisa, &newdocisa))
		putcreatenewversion (taskptr, &newdocisa);
	  else
		putrequestfailed (taskptr);
}


void retrievedocvspan (taskptr)
  typetask *taskptr;
{
  typeisa docisa;
  typevspan vspan;
  bool getretrievedocvspan(), doretrievedocvspan();

	if (
	   getretrievedocvspan (taskptr, &docisa)
	&& doretrievedocvspan (taskptr, &docisa, &vspan))
		putretrievedocvspan (taskptr, &vspan);
	  else
		putrequestfailed (taskptr);
}

/*
void deletevspan (taskptr)
  typetask *taskptr;
{
  typeisa docisa;
  typespan vspan;
  bool getdeletevspan(), dodeletevspan();

	if (
	   getdeletevspan (taskptr, &docisa, &vspan)
	&& dodeletevspan (taskptr, &docisa, &vspan))
		putdeletevspan (taskptr);
	  else
		putrequestfailed (taskptr);
}
*/
void deletevspan (taskptr) /* kluged unix version for speed */
  typetask *taskptr;
{
  typeisa docisa;
  typespan vspan;
  bool getdeletevspan(), dodeletevspan();
 
	(void) getdeletevspan (taskptr, &docisa, &vspan);
	putdeletevspan (taskptr);
	if (!dodeletevspan (taskptr, &docisa, &vspan))
#ifndef DISTRIBUTION
		fprintf(stderr,"deletevspan failed \n");
#else
	;
#endif
}


void retrieveendsets (taskptr)
  typetask *taskptr;
{
  typespecset specset, fromset, toset, threeset;
  bool getretrieveendsets(), doretrieveendsets();

	if (
	   getretrieveendsets (taskptr, &specset)
	&& doretrieveendsets (taskptr, specset, &fromset, &toset, &threeset)){

		putretrieveendsets (taskptr, fromset, toset, threeset);
	 }else
		putrequestfailed (taskptr);
}

void xaccount(taskptr)
  typetask *taskptr;
{
  bool getxaccount();

	if (getxaccount(taskptr,&(player[user].account))) {
		putxaccount(taskptr);
	} else {
		putrequestfailed(taskptr);
	}
}

void createnode_or_account(taskptr)
  typetask *taskptr;
{
  tumbler t;
  bool getcreatenode_or_account(), docreatenode_or_account();

	if(	getcreatenode_or_account(taskptr,&t)
	     && docreatenode_or_account(taskptr,&t)) {
		putcreatenode_or_account(taskptr,&t);
	} else {
		putrequestfailed(taskptr);
	}
}

void myopen(taskptr)
  typetask  *taskptr;
{
  tumbler t, newt;
  int type, mode;
  bool getopen(), doopen();

	if (
		getopen(taskptr, &t, &type, &mode)
		&&  doopen(taskptr, &t, &newt, type, mode, user)) {
		putopen(taskptr, &newt);
	} else {
		putrequestfailed(taskptr);
	}
}


void myclose(taskptr)
  typetask  *taskptr;
{
  tumbler t;
  bool getclose(), doclose();

	if(
		getclose(taskptr,&t)
		&& doclose(taskptr,&t, user)) {
		putclose(taskptr);
	} else {
		putrequestfailed(taskptr);
	}
}


void quitxanadu(taskptr)
  typetask *taskptr;
{
  void diskexit();

  putquitxanadu(taskptr);
  if (!backenddaemon) {
    if (interfaceinput) {
	fprintf(interfaceinput, "%d~\n", QUIT);
	fclose(interfaceinput);
    }
    diskexit();
  } else
    dobertexit(user);	/* Close all of the user's outstanding opens */
}
