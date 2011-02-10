/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* get1.d - Udanax top-level input routines */
/* as of Jul 12 19:00:00 */

#include "xanadu.h"

  bool
getfinddocscontaining (taskptr, specsetptr)
  typetask *taskptr;
  typespecset *specsetptr;
{
	return (getspecset (taskptr, specsetptr));
}


  bool
getcopy (taskptr, docisaptr, vsaptr, localspecsetptr)
  typetask *taskptr;
  typeisa *docisaptr, *vsaptr;
  typespecset *localspecsetptr;
{
	prompt(taskptr, "copy to this document=> ");
	if (!getisa(taskptr, docisaptr))
		return(FALSE);
	prompt(taskptr, "at this address=> ");
	if ( !(getvsa(taskptr, vsaptr)
		&& getspecset(taskptr, localspecsetptr)))
			return(FALSE);
	return(TRUE);
}

  bool
getinsert (taskptr, docisaptr, vsaptr, textsetptr)
  typetask *taskptr;
  tumbler *docisaptr, *vsaptr;
  typetextset *textsetptr;
{
	prompt(taskptr, "text=>\n\n");
	if (!gettextset(taskptr, textsetptr))
		return(FALSE);
	prompt(taskptr, "document=> " );
	if (!getisa(taskptr, docisaptr))
		return(FALSE);
	prompt(taskptr, "address=> ");
	if (!getvsa(taskptr, vsaptr))
		return(FALSE);
	return(TRUE);
}

  bool
getcreatelink (taskptr, docisaptr, fromspecsetptr, tospecsetptr, threespecsetptr)
  typetask *taskptr;
  typeisa *docisaptr;
  typespecset *fromspecsetptr;
  typespecset *tospecsetptr;
  typespecset *threespecsetptr;
{
	prompt (taskptr, "home document=> ");
	if (!getisa (taskptr, docisaptr))
		return (FALSE);
	prompt (taskptr, "fromset\n");
	if (!getspecset (taskptr, fromspecsetptr))
		return (FALSE);
	prompt (taskptr, "toset\n");
	if (!getspecset (taskptr, tospecsetptr))
		return (FALSE);
	prompt (taskptr, "threeset\n");
	if (!getspecset (taskptr, threespecsetptr))
		return (FALSE);
	return (TRUE);
}

  bool
getfollowlink (taskptr, linkisaptr, whichendptr)
  typetask *taskptr;
  typeisa *linkisaptr;
  INT *whichendptr;
{
	prompt (taskptr, "enter link=> ");
	if (!getisa (taskptr, linkisaptr))
		return (FALSE);
	prompt (taskptr, "enter endset=> ");
	if (!(
	   getnumber (taskptr, whichendptr)
	&& (*whichendptr == 1 || *whichendptr == 2 || *whichendptr == 3)))
		return (FALSE);
	return (TRUE);
}

  bool
getcreatenewversion(taskptr,docisaptr)
  typetask *taskptr;
  typeisa *docisaptr;
{
	  prompt(taskptr,"enter document=> ");
	  return (getisa (taskptr, docisaptr));
}

  bool
getretrievedocvspanset (taskptr, docisaptr)
  typetask *taskptr;
  typeisa *docisaptr;
{
	prompt(taskptr, "enter document=> ");
	return (getisa (taskptr, docisaptr));
}

  bool
getretrievedocvspan (taskptr, docisaptr)
  typetask *taskptr;
  typeisa *docisaptr;
{
	prompt(taskptr, "enter document=> ");
	return (getisa (taskptr, docisaptr));
}

  bool
getrearrange (taskptr, docisaptr, cutseqptr)
  typetask *taskptr;
  typeisa *docisaptr;
  typecutseq *cutseqptr;
{
	prompt(taskptr, "enter document=> ");
	if (!getisa(taskptr, docisaptr))
		return(FALSE);
	prompt(taskptr, "enter cutseq=>\n");
	if (!getcutseq(taskptr, cutseqptr))
		return(FALSE);
	return(TRUE);
}

  bool
getretrievev (taskptr, specsetptr)
  typetask *taskptr;
  typespecset *specsetptr;
{
	return getspecset(taskptr,specsetptr);
}

  bool
getfindlinksfromtothree (taskptr, fromvspecsetptr, tovspecsetptr, threevspecsetptr, homesetptr)
  typetask *taskptr;
  typespecset *fromvspecsetptr;
  typespecset *tovspecsetptr;
  typespecset *threevspecsetptr;
  typeispanset *homesetptr;
{
	prompt (taskptr, "fromset\n");
	if (!getspecset (taskptr, fromvspecsetptr))
		return(FALSE);
	prompt (taskptr, "toset\n");
	if (!getspecset (taskptr, tovspecsetptr))
		return(FALSE);
	prompt (taskptr, "threeset\n");
	if (!getspecset (taskptr, threevspecsetptr))
		return (FALSE);
	prompt (taskptr, "home documents\n");
	if (!getspanset (taskptr, homesetptr, ISPANID))
		return (FALSE);
	return(TRUE);
}

  bool
getfindnumoflinksfromtothree (taskptr, fromvspecsetptr, tovspecsetptr, threevspecsetptr, homesetptr)
  typetask *taskptr;
  typespecset *fromvspecsetptr;
  typespecset *tovspecsetptr;
  typespecset *threevspecsetptr;
  typeispanset *homesetptr;
{
	return (getfindlinksfromtothree (taskptr, fromvspecsetptr, tovspecsetptr, threevspecsetptr, homesetptr));
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
	if(!getfindlinksfromtothree (taskptr, fromvspecsetptr, tovspecsetptr, threevspecsetptr, homesetptr))
		return(FALSE);
	prompt(taskptr, "last link=> ");
	if(!getisa(taskptr, lastlinkptr))
		return(FALSE);
	prompt(taskptr, "number of links => ");
	if(!getnumber(taskptr, nptr))
		return(FALSE);
	return(TRUE);
}


/* getnavigateonht */

  bool
getshowrelationof2versions (taskptr, version1ptr, version2ptr)
  typetask *taskptr;
  typespecset *version1ptr;
  typespecset  *version2ptr;
{
	prompt(taskptr, "version1\n");
	if (!getspecset (taskptr,version1ptr))
		return(FALSE);
	prompt(taskptr, "version2\n");
	if (!getspecset (taskptr,version2ptr))
		return(FALSE);
	return(TRUE);
}
getcreatenewdocument()
{
}

  bool
getdeletevspan (taskptr, docisaptr, vspanptr)
  typetask *taskptr;
  typeisa *docisaptr;
  typevspan *vspanptr;
{
	prompt(taskptr, "document=> ");
	if(!getisa (taskptr,docisaptr))
		return(FALSE);
	prompt(taskptr, "delete this part\n");
	if(!getspan (taskptr,vspanptr,VSPANID/*zzz*/))
		return(FALSE);
	return(TRUE);
}

setdebug (taskptr)
  typetask *taskptr;
{
	prompt (taskptr, "set debug => ");
	getnumber (taskptr, &debug);
}
	
playwithalloc(taskptr)
  typetask * taskptr;
{		     
	prompt(taskptr,"playwithalloc\n");
	lookatalloc();
}   

  bool
getretrieveendsets (taskptr, specsetptr)
  typetask *taskptr;
  typespecset *specsetptr;
{
	return (getspecset (taskptr, specsetptr));
}

bool getxaccount (taskptr, accountptr)
  typetask *taskptr;
  typeisa *accountptr;
{
  bool validaccount();
/*tumblerclear (accountptr);
return (TRUE);
*/

       /* prompt (taskptr, "account? ");*/
	
	   gettumbler (taskptr, accountptr)
	&& validaccount(taskptr, accountptr);
	 taskptr->account = *accountptr;
	fprintf(stderr,"in get xaccount \n");
	return(TRUE);
}



getcreatenode_or_account(taskptr,tp)
  typetask *taskptr;
  tumbler *tp;
{
  gettumbler(taskptr,tp);
	return(TRUE);
}

getopen(taskptr,tp,typep,modep)
  typetask *taskptr;
  tumbler *tp;
  int *typep;
  int *modep;
  
{

gettumbler(taskptr,tp);
getnumber(taskptr,typep);
getnumber(taskptr,modep);
	return(TRUE);
}

getclose(taskptr,tp)
  typetask *taskptr;
  tumbler *tp;
{
  gettumbler(taskptr,tp);
	return(TRUE);
}
