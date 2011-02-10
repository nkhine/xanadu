/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/*
	rcfile.c - process .backendrc for xlog, backenddaemon and ??
*/
#include "defs.h"
#include <stdio.h>
#include "port.h"
#include "alloc.h"

#define RCFILENAME               ".backendrc"
#define PORTMETANAME		 "port"
#define HOSTMETANAME		 "host"
#define BACKENDDIRECTORYMETANAME "backenddir"
#define BACKENDFILEMETANAME      "backend"
#define ACCOUNTFILEMETANAME      "accountfile"
#define FRONTENDFILEMETANAME     "frontend"
#define BACKENDGLUEFILEMETANAME  "backglue"
#define FRONTENDGLUEFILEMETANAME "frontglue"
#define ALLOCSIZENAME		 "allocsize"
#define INCREMENTALALLOCSIZENAME "incrementalallocsize"


int  portname = PORT;
char hostname[256] = "localhost";
char backenddirectoryname[256] = ".";
char backendfilename[256] =   "backenddaemon";
char accountfilename[256] =   "accountfile";
char frontendfilename[256] =  "fex";
char backendgluefilename[256] =  "intx";
char frontendgluefilename[256] = "ints";

INT incrementalallocsize = INCREMENTALALLOCSIZE;
INT  allocsize = ALLOCSIZE;

/* Read run-time parameters from ./.backendrc    ECH 7-7-88
   This is real dumb, but it should be enough to get the job done for now.
*/
void processrcfile()
{
  FILE *rcfd;
  static char buf[BUFSIZ];
  static char line[256];
  static char name[256];
  INT temp;
  if ((rcfd = fopen(RCFILENAME, "r")) != NULL) {
    while (fgets(buf, BUFSIZ, rcfd)) {
      if (buf[0] != '#' && sscanf(buf, "%s = %s", name, line) == 2) {
        if (!strcmp(name, BACKENDFILEMETANAME))
          strcpy(backendfilename, line);
        else if (!strcmp(name, ACCOUNTFILEMETANAME))
          strcpy(accountfilename, line);
        else if (!strcmp(name, PORTMETANAME)) {
          if (sscanf(line, "%d", &portname) < 1) {
            fprintf(stderr, "Could not use port = %s, using %d\n", line, PORT);
            portname = PORT;
          }
        } else if (!strcmp(name, HOSTMETANAME))
          strcpy(hostname, line);
        else if (!strcmp(name, BACKENDDIRECTORYMETANAME))
          strcpy(backenddirectoryname, line);
        else if (!strcmp(name, BACKENDGLUEFILEMETANAME))
          strcpy(backendgluefilename, line);
        else if (!strcmp(name, FRONTENDGLUEFILEMETANAME))
          strcpy(frontendgluefilename, line);
        else if (!strcmp(name, FRONTENDFILEMETANAME))
          strcpy(frontendfilename, line);
        else if (!strcmp(name, ALLOCSIZENAME)){
	  temp = allocsize;
	  sscanf(line,"%d",allocsize);
	  if(allocsize<100000||allocsize>300000000){/*minimal sanity checking*/
		incrementalallocsize = temp;
	  }
        }else if (!strcmp(name, INCREMENTALALLOCSIZENAME)){
	  temp = incrementalallocsize;
	  sscanf(line,"%d",incrementalallocsize);
	  if(incrementalallocsize<10000||incrementalallocsize>100000000){
		incrementalallocsize = temp;
	  }
        }else
          fprintf(stderr, "Don't know about %s = %s\n", name, line);
      }
    }
    fclose(rcfd);
  }
}


