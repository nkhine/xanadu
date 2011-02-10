/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License,
* which contains precisely the terms of the X11 License.  The full text of
* The Udanax Open-Source License can be found in the distribution in the file
* license.html.  If this file is absent, a copy can be found at
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/

#include <stdio.h>
#include "common.h"
#include "players.h"
#include "port.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>

#define ERROR           -1
#define typerequest INT
#define MAX_PLAYERS     25       /* max # concurrent players     */

/* These two are set in rcfile.c, possibly from the .backendrc file */
extern char hostname[];
extern int  portname;

INT fdtoplayer[32];
tumbler defaultaccount = {0,0,0,0,  1,1,0,14,0};

PLAYER player[MAX_PLAYERS];            /* player information   */
INT n_players = 0;            
INT inputfds = 0;
INT nfds = 0;


INT     main_socket=ERROR;      /* socket to accept connections on */

void  *new_players(player, n_playersp, block ,taskptr)
  PLAYER    player[];       /* array of player info structures      */
  INT      *n_playersp;     /* number of players (incl comp)        */
  INT       block;          /* if nobody wants to join, wait on socket
                               until somebody does. */
  typetask *taskptr;
{
  INT     readfds;                /* for select() call    */
  INT     s;                      /* temp holder for new socket   */
  INT     rc;                     /* # ready selected sockets */
  struct  sockaddr_in     from;   /* connection acceptor */
  INT     fromlen = sizeof(from);
  struct  timeval t;              /* don't let select() run forever */
  INT     i;
  FILE   *temp;
  char    devicename[100];
  int     len;
  typerequest requestinstance;
  INT     open_sock();            /* to open main socket */

    if (main_socket == ERROR) {
	main_socket = open_sock();
    }
/* set up 0 second timeout for use on  select() calls */
/* well it  USED  to be  0 seconds, but that seems too fast now !<reg sep 10 1999>*/
    t.tv_sec = 0L;
    t.tv_usec = 3L;
    for (;;) {
        readfds = 1 << main_socket;
    t.tv_sec = 0L;
    t.tv_usec = 3L;
        if ((rc = select( 32, &readfds, 0, 0, &t )) == -1) {
		perror("select");
		fflush(stdout);
		break;
	}
        if (rc > 0 || block) {
	    if ((s = accept( main_socket, &from, &fromlen)) < 0) {
		perror("accept");
		fflush(stdout);
		break;
	    }
	    if ( *n_playersp >= MAX_PLAYERS ) {
		fprintf(stderr,"TOOMANY frontends: won't log another one\n");
		close(s);
	    } else {
		block = FALSE;
#ifndef DISTRIBUTION
		fprintf(stderr,"accepted connection from %d ",s);
#endif
/*read ttyname and open socket for it;*/
		
		player[*n_playersp].socket = s;
		if ((player[*n_playersp].inp = fdopen(s,"r")) == NULL) {
			perror("fdopen(r)");
			break;
		}
		if ((taskptr->outp = player[*n_playersp].outp = fdopen(s,"w")) == NULL) {
			perror("fdopen(w)");
			break;
		}
		taskptr->inp = player[*n_playersp].inp;


#ifndef DISTRIBUTION
		fprintf(stderr,"%s\n",&(player[*n_playersp].account));
#endif
		if (!establishprotocol(taskptr->inp, taskptr->outp)) {
			break;
		}
		//getrequest(taskptr, &requestinstance);
		//getxaccount(taskptr, &(player[*n_playersp].account));
		//logaccount(&(player[*n_playersp].account));
		player[*n_playersp].wantsout = FALSE;
		
		fdtoplayer[s] = *n_playersp;
		inputfds |= (1<<s);
		nfds = max (s,nfds);
		(*n_playersp)++;
		break;
	    }
        } else
	  break;
    }
}


void *leave(player, xn_players)
  PLAYER  player[];
  INT    *xn_players;
{
  register INT i;

  for (i = 0; i < *xn_players; i++)
    if (player[i].wantsout) {
	  inputfds ^= (1 << (player[i].socket));
	  if (close(player[i].socket) != 0) {
		fprintf(stderr,"user %d: ", i);
		perror("close player.socket");
	  }
	  if(*xn_players >1){
		  player[i] = player[*xn_players - 1];
	  }
	  (*xn_players)--;
	  --i;            /* since this entry is new, test again */
    }
}



INT open_sock()		/* Open the main socket. */
{
  INT     s;
  struct  sockaddr_in     sockaddr;
  struct  hostent         *host;


  if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket()");
    exit(1);
  }
/*
  if ((host = gethostbyname(hostname)) == NULL) {
    perror("gethostbyname()");
    exit(1);
  }
*/
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_port   = htons(portname);
  sockaddr.sin_addr.s_addr = /*inetaddr*/INADDR_ANY;

fprintf(stderr,"calling bind s = %d \n",s);

  if (bind(s, &sockaddr, sizeof(sockaddr)) < 0) {
    perror("bind()");
    exit(1);
  }
  if (listen(s, 0) < 0) {
    perror("listen()");
    exit(1);
  }
  return(s);
}


void *crash()
{
  INT i;

fprintf(stderr, "CRASH while dealing with user %d\n", user);
  for (i = 0; i < 32; i++)
    close(i);	/* BOO HISS: too many closes */
  exit(9);
}


bool isthisusersdocument(tp)
  tumbler *tp;
{
	/* was &(player[n_players].account)  !!!!!! GRRRR ECH */
	return tumbleraccounteq(tp, &(player[user].account));
}

