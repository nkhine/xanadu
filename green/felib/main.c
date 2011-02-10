/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:        main.d
***************************************************/

#define MAINDECLARATION

#include <signal.h>
#include "fest.h"
#include "feminreq.h"
#include "requests.h"


#define WAITFORBACKEND 
bool issfe = FALSE;
FILE *fromxu, *toxu;

char mypasswd[] = "test trap";
tumbler myaccount =  {0,0,1,1,0,1,0,0,0} /* 1.1.0.1*/;


#define PROTOCOL "0"

main (argc, argv)
  nat argc;
  char *argv[];
{

  med interact();
  med exitflag;
  tumbler docvposition;
  bool exit();
  tasktype task;
  long n;
  void sigtrap();
  char proto[8];

	debug=0;
#ifdef UNIX
        srand (getpid());
#endif

        if (!(fromxu = fdopen (/*3*/atoi(argv[1]),"r"))) {
		fprintf(stderr,"can't open fromxu\n\r");
		curlupanddye();
	}
        if (!(toxu = fdopen (/*6*/atoi(argv[2]),"w"))) {
		fprintf(stderr,"can't open toxu\n\r");
		curlupanddye();
	}
	inittask(&task,stdin, toxu);
        sendprotocol(&task,PROTOCOL);
	inittask(&task,fromxu,stdout);     
        if (!takeprotocol(&task,proto) || strcmp(proto,PROTOCOL) != 0) {
		fprintf(stderr,"FeBe protocol mismatch\n");
		curlupanddye();
	}
        setbuf (stderr, NULL);  
	init (&docvposition);
	for (n=SIGHUP; n<SIGIO; n++)
		signal(n,sigtrap);
	signal(SIGWINCH, SIG_DFL);
        sleep(100);/*loop {
                fillwindow (currentwindow, &docvposition);
                exitflag = interact (&docvposition);
                if (exitflag == EXITNORMALLY) {
                          break;
                }
        }
	*/
        closeupshop();
}
  void
sigtrap(sig)
  int sig;
{
	fprintf(stderr,"Caught signal %d\n",sig);
	/*noraw();*/
	/*endwin();*/
	exit(1);
}
