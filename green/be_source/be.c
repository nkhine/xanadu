/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* Udanax main program */
/* backend version */

/*#include <signal.h>*/
#include <time.h>
#include "xanadu.h"
#include "requests.h"
#include "players.h"
#define MAX_PLAYERS 5
int user = 0;
PLAYER player[MAX_PLAYERS];
extern INT errno;

FILE *logfile;
FILE *nulllog;
FILE *reallog;
bool isxumain = FALSE;
char outputbuffer[BUFSIZ];
char inputbuffer[BUFSIZ];
bool logstuff;
FILE *interfaceinput;
extern FILE *febelog;
extern bool firstputforrequest;
typetask *taskptrx;
tumbler defaultaccount = {0,0,0,0, 1,1,0,1,0,0,0,0}; /* 1.1.0.1 */

main ()
{
  typetask task;
  void exit();
  bool establishprotocol();
  char buf[100];
  FILE *fd;
  struct tm *local;
  long clock;

	taskptrx = &task;
	febelog = interfaceinput = reallog = logfile = nulllog = fopen ("/dev/null", "a");

	clock = time (0);
	local = localtime (&clock);
	sprintf(buf,"ln%d.%d.%d:%d",local->tm_mon+1,local->tm_mday,local->tm_hour,local->tm_min);
/*
#ifndef DISTRIBUTION
	sprintf(buf,"febe%d.%d.%d:%d",local->tm_mon+1,local->tm_mday,local->tm_hour,local->tm_min);
	febelog = fopen(buf, "w");
#endif
*/

	freopen ("backenderror", "w", stderr);		/* CHANGE THIS ?? */
	setbuf(stderr,NULL);
	processrcfile();

	setbuf (stdin, inputbuffer);
	setbuf (stdout, outputbuffer);

	if (!establishprotocol(stdin, stdout))
		exit(1);

	debug = FALSE;

	init (1);
	inittask (&task);

	initmagicktricks ();

	movetumbler(&defaultaccount,&task.account);
	
	for (;;) {
ntaskorcommand++;
		xanadu (&task);
		/*testforreservedness ("main");*/
		logfile = nulllog;
	}
/*lookatalloc();*/
}

xanadu(taskptr)
  typetask *taskptr;
{
  typerequest request;

	if (febelog && febelog != nulllog)
		fprintf(febelog, "\nfe:\n");
	firstputforrequest = TRUE;
	logstuff = FALSE;
	if (getrequest (taskptr, &request)) {
		(*requestfns[request]) (taskptr);
		sendresultoutput (taskptr);
	} else {
		taskptr->inp = stdin;
	}
	tfree (taskptr);
	/*lookatalloc();*/
	if (interfaceinput && interfaceinput != nulllog)
		fflush (interfaceinput);
	logstuff = FALSE;
}


bool establishprotocol(inp, outp)
  FILE *inp, *outp;
{
  char ch;
  typetask temptask;

	if (febelog && febelog != nulllog)
		fprintf(febelog, "fe:\n");
	firstputforrequest = TRUE;

	/* This is the metaprotocol for the time being */

	temptask.inp = inp;
	temptask.outp = outp;
	temptask.errp = stderr;

	while ((ch = pullc(&temptask)) != '\n') ;
	while ((ch = pullc(&temptask)) == '\n') ;
	if (ch == 'P' && pullc(&temptask) == '0' && pullc(&temptask) == '~') {
		xuputstring("\nP0~", temptask.outp);
		sendresultoutput(&temptask);
		return TRUE;
	} else {
		xuputstring("\nP?~", temptask.outp);
		sendresultoutput(&temptask);
		return FALSE;
	}
}


frontenddied()
{
	fprintf(stderr, "The frontend apparently has died.\n");
	diskexit();
	gerror("The frontend died");
}


/* for  linker until we get this cleaned up */
bool setmaximumsetupsize (taskptr) typetask *taskptr; {}

bool sourceunixcommand (taskptr) typetask *taskptr; {}


  bool
decrementusers()
{
  return(FALSE);
}

isthisusersdocument(tp)
  tumbler *tp;
{
  bool result = tumbleraccounteq(tp, &taskptrx->account);

	return result;
}
