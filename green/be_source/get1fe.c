/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/

/* get1fe.d - Udanax top-level input routines - front end version */
/* as of Jul 12 19:00:00 */

#include "xanadu.h"
#include "requests.h"
FILE *logfile;
FILE *nulllog;
FILE *reallog;
extern bool logstuff;
FILE *interfaceinput;

  bool
getinsert (taskptr, docisaptr, vsaptr, textsetptr)
  typetask *taskptr;
  tumbler *docisaptr, *vsaptr;
  typetextset *textsetptr;
{
/*logfile = reallog;
fprintf (logfile, "\nINSERT\n");
*/ 
logstuff = TRUE; 
fprintf(interfaceinput, "%d~",INSERT);  
	return (
	   gettumbler (taskptr, docisaptr)
	&& gettumbler (taskptr, vsaptr)
	&& gettextset (taskptr, textsetptr));
/*
	if (! gettumbler (taskptr, docisaptr))
	      return (FALSE);
	fprintf(stderr,"\ndocid  ");
	puttumbler(stderr,docisaptr);
	if (! gettumbler (taskptr, vsaptr))
	      return (FALSE);
	fprintf(stderr,"\nvsa  ");
	puttumbler(stderr,vsaptr);
	fprintf(stderr,"\n");
	if (! gettextset (taskptr, textsetptr))
		return (FALSE);
	return (TRUE);
*/
}

  bool
getretrievedocvspanset (taskptr, docisaptr)
  typetask *taskptr;
  typeisa *docisaptr;
{
/*fprintf (logfile, "\nRETRIEVEDOCVSPANSET\n"); */
	return (gettumbler (taskptr, docisaptr));
}

  bool
getcopy (taskptr, docisaptr, vsaptr, localspecsetptr)
  typetask *taskptr;
  typeisa *docisaptr, *vsaptr;
  typespecset *localspecsetptr;
{
/*logfile = reallog; fprintf (logfile, "\nCOPY\n"); */
logstuff = TRUE; 
fprintf(interfaceinput, "%d~",COPY);  
	 return (
	   gettumbler (taskptr, docisaptr)
	&& gettumbler (taskptr, vsaptr)
	&& getspecset(taskptr, localspecsetptr));
}

  bool
getrearrange (taskptr, docisaptr, cutseqptr)
  typetask *taskptr;
  typeisa *docisaptr;
  typecutseq *cutseqptr;
{
/*logfile = reallog; fprintf (logfile, "\nREARRANGE\n"); */
logstuff = TRUE; 
fprintf(interfaceinput, "%d~",REARRANGE);  
	 return (
	   gettumbler (taskptr, docisaptr)
	&& getcutseq (taskptr, cutseqptr));
}

  bool
getcreatelink (taskptr, docisaptr, fromspecsetptr, tospecsetptr, threespecsetptr)
  typetask *taskptr;
  typeisa *docisaptr;
  typespecset *fromspecsetptr;
  typespecset *tospecsetptr;
  typespecset *threespecsetptr;
{
logstuff = TRUE;
fprintf(interfaceinput, "%d~",CREATELINK);
	return (
	   gettumbler (taskptr, docisaptr)
	&& getspecset (taskptr, fromspecsetptr)
	&& getspecset (taskptr, tospecsetptr)
	&& getspecset (taskptr, threespecsetptr));
}

  bool
getretrievev (taskptr, specsetptr)
  typetask *taskptr;
  typespecset *specsetptr;
{
/*fprintf (logfile, "\nRETRIEVEV\n"); */

	if (getspecset (taskptr, specsetptr)) {
		return (TRUE);
	} else {
		return (FALSE);
	}
}

  bool
getfindnumoflinksfromtothree (taskptr, fromvspecsetptr, tovspecsetptr, threevspecsetptr, homesetptr)
  typetask *taskptr;
  typespecset *fromvspecsetptr;
  typespecset *tovspecsetptr;
  typespecset *threevspecsetptr;
  typeispanset *homesetptr;
{
  bool getfindlinksfromtothree();

	return (getfindlinksfromtothree (taskptr, fromvspecsetptr, tovspecsetptr, threevspecsetptr, homesetptr));
}

  bool
getfindlinksfromtothree (taskptr, fromvspecsetptr, tovspecsetptr, threevspecsetptr, homesetptr)
  typetask *taskptr;
  typespecset *fromvspecsetptr;
  typespecset *tovspecsetptr;
  typespecset *threevspecsetptr;
  typeispanset *homesetptr;
{
  bool kluge();

	return (
	   getspecset (taskptr, fromvspecsetptr)
	&& getspecset (taskptr, tovspecsetptr)
	&& getspecset (taskptr, threevspecsetptr)
	&& getspanset (taskptr, homesetptr, ISPANID) && kluge());
}

  bool
getfindnextnlinksfromtothree (taskptr, fromvspecsetptr, tovspecsetptr, threevspecsetptr, homesetptr, lastlinkptr, nptr)
  typetask *taskptr;
  typespecset *fromvspecsetptr;
  typespecset *tovspecsetptr;
  typespecset *threevspecsetptr;
  typeispanset *homesetptr;
  typeisa *lastlinkptr;
  INT *nptr;
{
	return
	   getfindlinksfromtothree (taskptr, fromvspecsetptr, tovspecsetptr, threevspecsetptr, homesetptr)
	&& gettumbler (taskptr, lastlinkptr)
	&& getnumber (taskptr, nptr);
}


  bool
getshowrelationof2versions (taskptr, version1ptr, version2ptr)
  typetask *taskptr;
  typespecset *version1ptr;
  typespecset  *version2ptr;
{
	return (
	   getspecset (taskptr, version1ptr)
	&& getspecset (taskptr, version2ptr));
}

/* createnewdocument - no get routine */
getcreatenewdocument()
{
logstuff = TRUE; 
fprintf(interfaceinput, "%d~",CREATENEWDOCUMENT);  
}

  bool
getdeletevspan (taskptr, docisaptr, vspanptr)
  typetask *taskptr;
  typeisa *docisaptr;
  typevspan *vspanptr;
{
/*logfile = reallog; fprintf (logfile, "\nDELETEVSPAN\n"); */
logstuff = TRUE; 
fprintf(interfaceinput, "%d~",DELETEVSPAN);  
	 return (
	   gettumbler (taskptr, docisaptr)
	&& getspan (taskptr, vspanptr, VSPANID));
}

  bool
getcreatenewversion (taskptr,docisaptr)
  typetask *taskptr;
  typeisa *docisaptr;
{
/*logfile = reallog; fprintf (logfile, "\nCREATENEWVERSION\n"); */
logstuff = TRUE; 
fprintf(interfaceinput, "%d~",CREATENEWVERSION);  
	   return (gettumbler (taskptr, docisaptr));
}

  bool
getretrievedocvspan (taskptr, docisaptr)
  typetask *taskptr;
  typeisa *docisaptr;
{
/*fprintf (logfile, "\nRETRIEVEDOCVSPAN\n"); */
	return (gettumbler (taskptr, docisaptr));
}

  bool
setdebug (taskptr)
  typetask *taskptr;
{
	return (getnumber (taskptr, &debug));
}


/* disk exit */


/* show enfilades */


  bool
getfollowlink (taskptr, linkisaptr, whichendptr)
  typetask *taskptr;
  typeisa *linkisaptr;
  INT *whichendptr;
{
/*fprintf (logfile, "\nFOLLOWLINK\n"); */
	return (
	   getnumber (taskptr, whichendptr)
	&& gettumbler (taskptr, linkisaptr));
}

/* examine */

/* source unix command */

  bool
getfinddocscontaining (taskptr, specsetptr)
  typetask *taskptr;
  typespecset *specsetptr;
{
/*fprintf (logfile, "\nFINDDOCSCONTAINING\n"); */
	return (getspecset (taskptr, specsetptr));
}

  bool
getretrieveendsets (taskptr, specsetptr)
  typetask *taskptr;
  typespecset *specsetptr;
{
/*fprintf (logfile, "\nRETRIEVEENDSETS\n"); */
	return (getspecset (taskptr, specsetptr));
}

  bool
kluge()
{
/*	fclose (reallog);
	reallog = fopen("xueditlog","a");
*/	return (TRUE);
}

playwithalloc(taskptr)
  typetask * taskptr;
{		     
	prompt(taskptr,"playwithalloc\n");
	lookatalloc();
} 

#include "players.h"

  bool
getxaccount(taskptr,accountptr)
  typetask *taskptr;
  typeisa *accountptr;
{
  bool validaccount();

	logstuff = TRUE;
	if (interfaceinput)
		fprintf(interfaceinput, "%d~",XACCOUNT);  

	   gettumbler (taskptr, accountptr)
	&& validaccount(taskptr, accountptr);
	player[user].account = *accountptr;
	taskptr->account = *accountptr;
	return(TRUE);
}

getcreatenode_or_account(taskptr,tp)
  typetask *taskptr;
  tumbler *tp;
{
	logstuff = TRUE;
	if (interfaceinput)
		fprintf(interfaceinput, "%d~",CREATENODE_OR_ACCOUNT);  
	gettumbler(taskptr,tp);
	return(TRUE);

}

logaccount(tp)
  tumbler *tp;
{
	if (interfaceinput) {
		fprintf(interfaceinput, "%d~",XACCOUNT);  
		puttumbler(interfaceinput,tp);
	}
}

getclose(taskptr,tp)
  typetask *taskptr;
  tumbler *tp;
{
	logstuff = TRUE;
	if (interfaceinput)
		fprintf(interfaceinput, "%d~",CLOSE);  
	gettumbler(taskptr,tp);
	return(TRUE);
}

getopen(taskptr,tp,typep,modep)
  typetask *taskptr;
  tumbler *tp;

  int *typep;
  int *modep;
  
{
	logstuff = TRUE;
	if (interfaceinput)
		fprintf(interfaceinput, "%d~",OPEN);
	gettumbler(taskptr,tp);
	getnumber(taskptr,typep);
	getnumber(taskptr,modep);
	return(*typep != NOBERTREQUIRED);
}


/* historical trace */
