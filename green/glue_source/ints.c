/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
# include       "port.h"
# include       <sys/types.h>
# include       <sys/socket.h>
# include         <netinet/in.h>
# include       <netdb.h>
# include       <stdio.h>
# include       <ctype.h>
# include       <signal.h>
#include <errno.h>
#include "rcfile.h"

int     globsock;      /* the socket */

main(argc, argv, envp)
  int     argc;
  char    **argv;
  char    **envp;
{
  struct  sockaddr_in     sockaddr;
  struct  hostent         *host;
  u_long  inetaddr;
  char    *getenv();
  extern  int getuid();
  void    closem();
  char account[200];
  char s1[12],s2[12];
  char *itoas();
  int firsttime;
  FILE * tempfile,*fdopen();
/* char password[100],xandle[100];*/

	processrcfile();
	if (chdir(backenddirectoryname) == -1) {
		perror(backenddirectoryname);
		exit(1);
	}
	firsttime = 1;
foo:
	if (argv[2]) {
		strcpy(account, argv[2]);
	} else {
		account[0] = '\0';
	}
        if ((globsock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket()");
		exitbadly();
	}
	if ((host = gethostbyname(hostname)) == NULL) {
		perror( "gethostbyname" );
		exitbadly();
	}

	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(portname);
	sockaddr.sin_addr.s_addr = *(u_long *) host->h_addr;

	fflush(stdout);
	if (signal(SIGINT, closem) == (void *)-1) {
		perror("signal SIGINT");
		exitbadly();
	}
	if (signal(SIGQUIT, closem) == (void *)-1) {
		perror("signal SIGQUIT");
		exitbadly();
	}
	if (connect(globsock, &sockaddr, sizeof(sockaddr)) < 0) {
		if (errno == ETIMEDOUT || errno == ECONNREFUSED) {
			/* timed out try spawning a deamon */
			if (firsttime) {
				firsttime = 0;
				if (fork()) {
					sleep(15);
					goto foo;
				} else {
					execl(backendfilename, "xubackend", 0);
					perror("exec backend");
					exitbadly();
				}
				return;
			} else {
				perror("connect1");
				exitbadly();
			}
		}
		perror("connect2");
		exitbadly();
	}

	if ((tempfile = fdopen(globsock, "w")) == NULL) {
		perror("fdopen in ints");
		exitbadly();
	}
	fprintf(tempfile,"%s\n","SOCKET");
	fprintf(tempfile,"34~%s~",account);/* send account to be */
	fflush(tempfile);

        if (signal(SIGINT, SIG_IGN) == (void *)-1) {
		perror("signal SIGINT");
		exitbadly();
	}
        if (signal(SIGQUIT, SIG_IGN) == (void *)-1) {
		perror("signal SIGQUIT");
		exitbadly();
	}

        execl(argv[1], "frontend", itoas(globsock,s1), itoas(globsock,s2), 0);
	perror("execl frontend");
	exitbadly();
}


void closem()
{
  close(globsock);
  exitbadly();
}


  char *
itoas(i,s)
  int i;
  char *s;
{
        sprintf(s,"%d",i);
        return(s);
}


exitbadly()
{
	fprintf(stderr, "ints exiting badly\n");
	exit(1);
}
