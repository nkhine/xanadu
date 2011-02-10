/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
 
/*#include <signal.h>*/
/*#include <errno.h>*/
#include <time.h>
#include "xanadu.h"
#include "requests.h"
extern FILE *febelog;
extern bool isxumain;
void (*requestfns[NREQUESTS])();

void nullfun(taskptr)
  typetask *taskptr;
{
	putrequestfailed(taskptr);
}


init (safe)
  bool safe;
{
       INT i;
       void copy(), insert(), makelink(), retrievedocvspanset(), rearrange(), retrievev(),
	    navigateonht(), showrelationof2versions(), createnewdocument(), deletevspan(),
	    createnewversion(), retrievedocvspan(), sourceunixcommand(), followlink(),
	    finddocscontaining(), exit(), retrieveendsets(), createlink(),
	    findnumoflinksfromtothree(), findlinksfromtothree(), findnextnlinksfromtothree(),
	    xaccount(), createnode_or_account(), myopen(), myclose(), quitxanadu();
	/* Debugging stuff */
	void setdebug(), showenfilades(), examine(), dumpgranfwids(), ioinfo(), playwithalloc(),
	     setmaximumsetupsize();
	long start_time;
	struct tm *tm;

	start_time = time((long *) 0);
	tm = gmtime(&start_time);
	for (i = 0; i < NREQUESTS; ++i)
		requestfns[i] = nullfun;

	requestfns[COPY] = copy;
	requestfns[INSERT] = insert;
	requestfns[RETRIEVEDOCVSPANSET] = retrievedocvspanset;
	requestfns[REARRANGE] = rearrange;
	requestfns[RETRIEVEV] = retrievev;
	requestfns[NAVIGATEONHT] = navigateonht;
	requestfns[SHOWRELATIONOF2VERSIONS] = showrelationof2versions;
	requestfns[CREATENEWDOCUMENT] = createnewdocument;
	requestfns[DELETEVSPAN] = deletevspan;
	requestfns[CREATENEWVERSION] = createnewversion;
	requestfns[RETRIEVEDOCVSPAN] = retrievedocvspan;
	requestfns[QUIT] = quitxanadu;
	requestfns[SOURCEUNIXCOMMAND] = sourceunixcommand;
	requestfns[FOLLOWLINK] = followlink;
	requestfns[FINDDOCSCONTAINING] = finddocscontaining;
	requestfns[CREATELINK] = createlink;
	requestfns[RETRIEVEENDSETS] = retrieveendsets;
	requestfns[FINDNUMOFLINKSFROMTOTHREE] = findnumoflinksfromtothree;
	requestfns[FINDLINKSFROMTOTHREE] = findlinksfromtothree;
	requestfns[FINDNEXTNLINKSFROMTOTHREE] = findnextnlinksfromtothree;
	requestfns[CREATENODE_OR_ACCOUNT]= createnode_or_account;
	requestfns[OPEN] = myopen;
	requestfns[CLOSE] = myclose;
	requestfns[XACCOUNT] = xaccount;
	
	if (safe) {
		requestfns[SOURCEUNIXCOMMAND] = nullfun;
		requestfns[NAVIGATEONHT] = nullfun;
		requestfns[FINDNUMOFLINKSFROMTOTHREE] = nullfun;
		requestfns[FINDNEXTNLINKSFROMTOTHREE] = nullfun;
	}

	ntaskorcommand = 0;
}

