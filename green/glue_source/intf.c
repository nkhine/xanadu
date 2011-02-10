/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/

#include <stdio.h>
#include <signal.h>
#define READ 0
#define WRITE 1
#define tst(a,b) (mode == READ ? (b) : (a))
static int popen_pid;


main (argc, argv)
  int argc;
  char *argv[];
{
        interactive_pipe (argv[1], argv[2]);
}

interactive_pipe(prog1,prog2)
  char *prog1,*prog2;
{
 int p[2],r[2];
 char s1[12],s2[12];
 char *itoas();

        if (pipe(p) < 0) {
                printf("p is null\n");
                return(NULL);
        }
        if (pipe(r) < 0) {
                printf("r is null\n");
                return(NULL);
        }
        if((popen_pid = fork()) == 0) {
                signal(SIGALRM,SIG_IGN);
                close(p[0]);
                close(r[1]);
              /* connect pipes to std i/o */
                close(0);
                dup(r[0]);
                close(1);
                dup(p[1]);
                execl(prog1,"xanadu",0);
        } else{
                close(p[1]);
                close(r[0]);
                execl(prog2,"frontend",itoas(p[0],s1),itoas(r[1],s2),0);
        }
}
  char *
itoas(i,s)
  int i;
  char *s;
{
        sprintf(s,"%d",i);
        return(s);
}

