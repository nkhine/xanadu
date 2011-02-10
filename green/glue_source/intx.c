/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/*
	The arguments here cry for a comment:

	argv[1]		which program to fork
	argv[2]		string form of user's account number
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

main( argc, argv, envp )
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
   char *ttynamep, *ttyname();
   void processrcfile();

        processrcfile();
	if (chdir(backenddirectoryname) == -1) {
		perror(backenddirectoryname);
		exit(1);
	}
	firsttime = 4;
foo:
	if (argv[2]) {
		strcpy(account,argv[2]);
	} else {
		account[0] = '\0';
	}
        if ((globsock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror( "socket()");
		exitbadly();
	}
	if ((host = gethostbyname(hostname)) == NULL) {
		perror("gethostbyname()");
		exitbadly();
	}

	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(portname);
	sockaddr.sin_addr.s_addr = *(u_long *) host->h_addr;

	fflush(stdout);
	if (signal(SIGINT, closem) == (void *) -1) {
		perror("signal SIGINT");
		exitbadly();
	}
	if (signal(SIGQUIT, closem) == (void *)-1) {
		perror("signal SIGQUIT");
		exitbadly();
	}
	if (connect(globsock, &sockaddr, sizeof(sockaddr)) < 0) {
		if (errno == ETIMEDOUT || errno == ECONNREFUSED) {
			/* timed out try spawning a daemon */
			if (firsttime > 0) {
				firsttime --;
				if (fork()) {
					sleep(15);
					goto foo;
				} else {
					execl(argv[1], argv[1], 0);
					perror("execl(1) in intx");
					exitbadly();
				}
				return;
			} else {
				perror("connect()1");
				exitbadly();
			}
		}
		perror("connect()2");
		exitbadly();
	}
	if ((tempfile = fdopen(globsock,"w")) == NULL) {
		perror("fdopen in intx");
		exitbadly();
	}
	if ((ttynamep = ttyname(0)) == NULL) {
		perror("ttyname in intx");
		exitbadly();
	}
	fprintf(tempfile,"%s\n",ttynamep);
	fprintf(tempfile,"34~%s\n",account);
	fflush(tempfile);
close(0);
close(1);
        signal( SIGINT, SIG_IGN );
        signal( SIGQUIT, SIG_IGN );
wait(0);
}


void    closem()
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
  fprintf(stderr, "intx exiting badly\n");
  exit(1);
}
