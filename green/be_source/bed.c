/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/*
	bed.d	- multi-user backend main
*/

extern int backenddaemon;	/* backend version */

#include <signal.h>
#include <sys/time.h> /* includes <time.h>*/
#ifndef sun
#include <time.h>
#endif sun
#include "xanadu.h"
#include "requests.h"
#include "players.h"
#include <errno.h>
#include <setjmp.h>	/* an easy way to deal with unexpected EOF on frontend input */

jmp_buf frontendeof;

int user = 0;		/* Global current user ID */

extern INT errno;

FILE *logfile;
FILE *nulllog;
FILE *reallog;
bool isxumain = FALSE;
char outputbuffer[BUFSIZ];
char inputbuffer[BUFSIZ];
bool logstuff;
extern FILE *interfaceinput;
extern FILE *febelog;
struct timeval timeout;
INT main_socket;
static tumbler currentaccount;
bool quitafteruser = FALSE;
bool mightbeblocked = FALSE;

main ()  /* inside temporary */
{
  typetask task;
  void exit();
  char buf[100];
  FILE *fd;
  struct tm *local;
  long clock;
 /* extern  void    crash();**/	/* if broken pipe, release port (I hope) */
  extern  INT     strcmp();
  void    flagquitting();

  INT s; /* for socket*/
  INT inputfds2;
  INT i;

    freopen ("backenderror", "w", stderr);
    setbuf(stderr, NULL);

    backenddaemon = 1;

    processrcfile();

    signal(SIGPIPE,crash);
    signal(SIGHUP,crash);
    signal(SIGXCPU,SIG_IGN);
    signal(SIGINT, flagquitting);
    timeout.tv_sec = 2; /* wait 2 seconds on a select, 
				then look for a new user*/

    febelog = interfaceinput = reallog = logfile = nulllog = fopen("/dev/null", "a");

    clock = time(0);
    local = localtime(&clock);
    sprintf (buf, "ln%d.%d.%d:%d", local->tm_mon+1, local->tm_mday, local->tm_hour, local->tm_min);
    interfaceinput=fopen(buf, "w");
#ifndef DISTRIBUTION
    sprintf(buf,"febe%d.%d.%d:%d",local->tm_mon+1,local->tm_mday,local->tm_hour,local->tm_min);
	febelog = fopen(buf, "w");
	setbuf(febelog, NULL);
#endif

    setbuf (stdin, inputbuffer);
    setbuf (stdout, outputbuffer);
    debug = 0;
    init (1);
    inittask (&task);
    errno = 0;
    initmagicktricks ();
    main_socket = open_sock();

    if (n_players < 1) {
      mightbeblocked = TRUE;
      new_players(player, &n_players, TRUE ,&task);/* wait for fe to talk to */
      mightbeblocked = FALSE;
    }
    for (;;) {
	if (n_players < 1) {
	  diskflush();	/* Write out everything when no one around */
          mightbeblocked = TRUE;
	  new_players(player, &n_players, TRUE ,&task);  /* wait for fe to talk to */
          mightbeblocked = FALSE;
	}
	inputfds2 = inputfds;
	if (select(nfds+1, &inputfds2, 0, 0, &timeout) < 0) {
	    if (errno == EINTR) {
		continue;
	    }
	    perror("select");
	    gerror("select in main");
	} else {
	    for (i = 0; i <= nfds; i++) {
		if ((1<<i) & inputfds2) {
		    user = fdtoplayer[i];
		    task.inp = player[user].inp;
		    task.outp = player[user].outp;
		    task.account = player[user].account ;
		    if (!tumblereq(&currentaccount, &task.account)) {
			currentaccount = task.account;
			logaccount(&task.account);
		    }
		    xanadu(&task);
		    if (quitafteruser) {
			if (interfaceinput) {
				fprintf(interfaceinput, "%d~\n", QUIT);
				fclose(interfaceinput);
			}
			writeenfilades();
			closediskfile ();
			exit(0);
		    }
		    /*testforreservedness("main");*/
/*		    logfile = nulllog; */
		    ntaskorcommand++;
		}
	    }
	}
	
	leave(player, &n_players);
	mightbeblocked = TRUE;
	new_players(player, &n_players, FALSE ,&task);
        mightbeblocked = FALSE;
    }
}


xanadu(taskptr)
  typetask *taskptr;
{
  typerequest request;

	logstuff = FALSE;
	if (setjmp(frontendeof)) {
		dobertexit(user);
		player[user].wantsout = TRUE;
	} else if (getrequest(taskptr, &request)) {
		(*requestfns[request])(taskptr);
		sendresultoutput(taskptr);
		if (request == QUIT) {
			player[user].wantsout = TRUE;
		}
	}
	tfree(taskptr);
	if (interfaceinput && interfaceinput != nulllog)
		fflush(interfaceinput);
	logstuff = FALSE;
}


void flagquitting()
{
	signal(SIGINT, SIG_IGN);	/* Don't die too early */
	if (mightbeblocked) {
		if (interfaceinput) {
			fprintf(interfaceinput, "%d~\n", QUIT);
			fclose(interfaceinput);
		}
		writeenfilades();
		closediskfile ();
		exit(0);
	} else
		quitafteruser = TRUE;	/* Flag to stop backend after current request */
}

bool establishprotocol(inp, outp)
  FILE *inp, *outp;
{
  metachar ch;

	/* This is the metaprotocol for the time being */
	while ((ch = getc(inp)) != '\n')
		putc((ch > ' ' ? ch : '.'), stderr);
	while ((ch = getc(inp)) == '\n')
		putc(ch, stderr);
	if (ch == 'P' && getc(inp) == '0' && getc(inp) == '~') {
		if (!feof(inp)) {		/* Don't send to a dead pipe */
			xuputstring("\nP0~", outp);
			fflush(outp);
			return TRUE;
		}
	} else {
		if (!feof(inp)) {		/* Don't send to a dead pipe */
			xuputstring("\nP?~", outp);
			fflush(outp);
		}
	}
	return FALSE;
}


frontenddied()
{
	longjmp(frontendeof, 1);
}


/* for  linker until we get this cleaned up */
  bool
setmaximumsetupsize (taskptr)
  typetask *taskptr;
{
}

  bool
sourceunixcommand (taskptr)
  typetask *taskptr;
{
 }

  bool
decrementusers()
{
	if(n_players >1){
		player[user].wantsout = 1;
	}
	return(n_players>1);
}









