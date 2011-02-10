/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:     sendtop.d
***************************************************/

#include "fest.h"
#include "requests.h"

/* ----------------- top level send routines -------------------- */

  void
sendinsert (taskptr, docidptr, vsaptr, charspanptr)
  tasktype *taskptr;
  tumbler *docidptr;
  smalltumbler *vsaptr;
  charspantype *charspanptr;
{
        sendnumber (taskptr->outp, (tdigit)INSERT);
        sendtumbler (taskptr->outp, docidptr);
        sendsmalltumbler (taskptr->outp, vsaptr);
        sendtextset (taskptr->outp, charspanptr);
        sendrequestbuffer (taskptr);
}

  void
sendretrievedocvspanset (taskptr, docidptr)
  tasktype *taskptr;
  tumbler *docidptr;
{
        sendnumber (taskptr->outp, (tdigit)RETRIEVEDOCVSPANSET);
        sendtumbler (taskptr->outp, docidptr);
        sendrequestbuffer (taskptr);
}

  void
sendcopy (taskptr, docidptr, vsaptr, specptr)
  tasktype *taskptr;
  tumbler *docidptr;
  smalltumbler *vsaptr;
  spectype *specptr;
{
        sendnumber (taskptr->outp, (tdigit)COPY);
        sendtumbler (taskptr->outp, docidptr);
        sendsmalltumbler (taskptr->outp, vsaptr);
        sendspecset (taskptr->outp, specptr);
        sendrequestbuffer (taskptr);
}

  void
sendrearrange (taskptr, docidptr, cutseqptr)
  tasktype *taskptr;
  tumbler *docidptr;
  cutseqtype *cutseqptr;
{
  med i;
        sendnumber (taskptr->outp,(tdigit) REARRANGE);
        sendtumbler (taskptr->outp, docidptr);
        sendnumber (taskptr->outp, (tdigit)cutseqptr->numberofcuts);
        for (i = 0; i < cutseqptr->numberofcuts; ++i) {
                sendsmalltumbler (taskptr->outp, &cutseqptr->cutsarray[i]);
	}
        sendrequestbuffer (taskptr);
}

  void
sendcreatelink (taskptr, docidptr, fromspecptr, tospecptr, threespecptr)
  tasktype *taskptr;
  tumbler *docidptr;
  spectype *fromspecptr;
  spectype *tospecptr;
  spectype *threespecptr;
{
        sendnumber (taskptr->outp, (tdigit)CREATELINK);
        sendtumbler (taskptr->outp, docidptr);
        sendspecset (taskptr->outp, fromspecptr);
        sendspecset (taskptr->outp, tospecptr);
        if (threespecptr) {	/* MSM 6/23/88 */
	        sendspecset (taskptr->outp, threespecptr);
	} else {
		gerror("threespecptr in sendcreatelink is NULL");
	}
        sendrequestbuffer (taskptr);
}

  void
sendretrievev (taskptr, specptr)
  tasktype *taskptr;
  spectype *specptr;
{
        sendnumber (taskptr->outp,(tdigit) RETRIEVEV);
        sendspecset (taskptr->outp, specptr);
        sendrequestbuffer (taskptr);
}

  void
sendfindlinksfromtothree (taskptr, linkrestrictionptr)
  tasktype *taskptr;
  linkrestrictiontype *linkrestrictionptr;
{
        sendnumber (taskptr->outp,(tdigit) FINDLINKSFROMTOTHREE); /* MSM 6/23/88 */
        sendlinkrestriction (taskptr->outp, linkrestrictionptr);
        sendrequestbuffer (taskptr);
}

  void
sendcreatenewdocument (taskptr)
  tasktype *taskptr;
{
        sendnumber (taskptr->outp, (tdigit)CREATENEWDOCUMENT);
        sendrequestbuffer (taskptr);
}

  void
senddeletevspan (taskptr, docidptr, vspanptr)
  tasktype *taskptr;
  tumbler *docidptr;
  spantype *vspanptr;
{
        sendnumber (taskptr->outp, (tdigit)DELETEVSPAN);
        sendtumbler (taskptr->outp, docidptr);
        sendspan (taskptr->outp, vspanptr);
        sendrequestbuffer (taskptr);
}

  void
sendcreatenewversion (taskptr, docidptr)
  tasktype *taskptr;
  tumbler *docidptr;
{
        sendnumber (taskptr->outp,(tdigit) CREATENEWVERSION);
        sendtumbler (taskptr->outp, docidptr);
        sendrequestbuffer (taskptr);
}

  void
sendretrievedocvspan (taskptr, docidptr)
  tasktype *taskptr;
  tumbler *docidptr;
{
        sendnumber (taskptr->outp,(tdigit) RETRIEVEDOCVSPAN);
        sendtumbler (taskptr->outp, docidptr);
        sendrequestbuffer (taskptr);
}

  void
senddiskexit (taskptr)
  tasktype *taskptr;
{
        sendnumber (taskptr->outp, (tdigit)DISKEXIT);
        sendrequestbuffer (taskptr);
}

  void
sendfollowlink (taskptr, linkptr, whichend)
  tasktype *taskptr;
  tumbler *linkptr;
  med whichend;
{
        sendnumber (taskptr->outp, (tdigit)FOLLOWLINK);
        sendnumber (taskptr->outp, (tdigit)whichend);
        sendtumbler (taskptr->outp, linkptr);
        sendrequestbuffer (taskptr);
}

  void
sendfinddocscontaining (taskptr, specptr)
  tasktype *taskptr;
  spectype *specptr;
{
        sendnumber (taskptr->outp,(tdigit) FINDDOCSCONTAINING);
        sendspecset (taskptr->outp, specptr);
        sendrequestbuffer (taskptr);
}

  void
sendretrieveendsets (taskptr, specset)
  tasktype *taskptr;
  spectype *specset;
{
        sendnumber (taskptr->outp, (tdigit)RETRIEVEENDSETS);	/* MSM 6/23/88 */
        sendspecset (taskptr->outp, specset);
        sendrequestbuffer (taskptr);
}

  void
sendshowrelationof2versions (taskptr, specset1, specset2)
  tasktype *taskptr;
  spectype *specset1, *specset2;
{
        sendnumber (taskptr->outp, (tdigit)SHOWRELATIONOF2VERSIONS);
        sendspecset (taskptr->outp, specset1);
        sendspecset (taskptr->outp, specset2);
        sendrequestbuffer (taskptr);
}

  void
sendaccount(taskptr,acctptr, passwdptr)
  tasktype *taskptr;
  tumbler *acctptr;
  char * passwdptr;
{
	sendnumber(taskptr->outp,(tdigit)XACCOUNT);
        sendtumbler (taskptr->outp, acctptr);
	/*feputstring(passwdptr,taskptr->outp);*/
        sendrequestbuffer (taskptr);
}

  void
sendopen(taskptr,tp,permission,mode)
  tasktype *taskptr;
  tumbler *tp;
  med permission,mode;
{
	sendnumber(taskptr->outp,(tdigit)OPEN);
	sendtumbler(taskptr->outp,tp);
	sendnumber(taskptr->outp,(tdigit)permission);
	sendnumber(taskptr->outp,(tdigit)mode);
        sendrequestbuffer (taskptr);
}

  void
sendclose(taskptr,tp)
  tasktype *taskptr;
  tumbler *tp;
{
	sendnumber(taskptr->outp,(tdigit)CLOSE);
	sendtumbler(taskptr->outp,tp);
        sendrequestbuffer (taskptr);
}
