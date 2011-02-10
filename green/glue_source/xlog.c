/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
# include       <stdio.h>
#ifdef aux
#include <string.h>
#else
#include <strings.h>
#endif
# include       <ctype.h>
# include       <signal.h>
#include <errno.h>
#include <sys/time.h>
#include "rcfile.h"

int errno;
char  **environ;

#define NULL 0
#define NOACCOUNT "0.1.1.0.1"
#define bool int
#define TRUE 1
#define FALSE 0

static int static0 = 0;

main( argc, argv, envp )
  int     argc;
  char    **argv;
  char    **envp;
{
  char    *getenv();
  char s1[12],s2[12];
  char *itoas();
  void closem();
  char c;
  char pw[1001];
  FILE *fp;
  struct timeval t;
  char storedaccountnumber[200];
  void processrcfile();

	processrcfile();
	if (chdir(backenddirectoryname) == -1) {
		perror(backenddirectoryname);
		exitbadly();
	}
	fp = fopen("account","a");
	fprintf(fp,"account = %s ",argv[1]);
	fflush(stdout);
	signal( SIGINT, closem );
	signal( SIGQUIT, closem );
	printf("password\n");
	fscanf(stdin,"%s",pw);
	fprintf(fp,"pw = %s ",pw);
	gettimeofday(&t,NULL);
	fprintf(fp,"time = %s\n",ctime(&t));
	fclose(fp);
        getchar();
	getaccountinformation(argv[1], pw, storedaccountnumber);
	printf("THIS LOGIN SEQUENCE WILL CHANGE SOON DON'T HARD CODE IT IN YOUR FE\n\r");
	printf("if you have your own frontend (on your computer) type b else type any other character\n\r");
	c = getchar();
	if (c == 'b') {
	  raw();
	  printf("entering backend \n");
	  /* backend file will inherit working directory set above */
 	  execl(backendgluefilename, backendgluefilename,
		backendfilename, storedaccountnumber, 0);
	} else {
	  printf("welcome to Xanadu\n\n\r");
	  printf(" as of today, July 28, the status is:\n\r");
	  printf("there are some bugs and the system is fragile, \n\n\r");
	  printf("IMPORTANT NOTICE:  changes you make may be permanent\n\r");
	  printf("but they will not be in your account, that will be soon\n\n\n\r");
	  printf("here goes....\n");
	  c = getchar();
	  raw();
	  execl(frontendgluefilename, frontendgluefilename,
			frontendfilename, storedaccountnumber, 0);
          perror("execl()");
	}
}


void closem()
{
	exitbadly();
}


exitbadly()
{
	exit(1);
}


  char *
itoas(i,s)
  int i;
  char *s;
{
        sprintf(s,"%d",i);
        return(s);
}


#ifdef aux
#include <sys/ttold.h>
#define RAW O_RAW
#define ECHO O_ECHO
#endif

#ifdef _POSIX_SOURCE

#include <termios.h>

raw()
{
  struct termios arg;

  tcgetattr(0, &arg);
  cfmakeraw(&arg);
  tcsetattr(0, TCSADRAIN, &arg);
  /* Change to TCSANOW if you don't want to wait for output to drain */
}

/* cbreak() and halfbaked() don't seem to be used anyway */

#else /* Not POSIX */

#include <sys/ioctl.h>

raw()
{
  struct sgttyb arg;
      ioctl(0,TIOCGETP,&arg);
      arg.sg_flags = (arg.sg_flags |/*CBREAK */ RAW) & ~ECHO;
/*      arg.sg_flags = arg.sg_flags & ~CBREAK; */
      ioctl(0,TIOCSETP,&arg);
}

cbreak()
{
  struct sgttyb arg;
      ioctl(0,TIOCGETP,&arg);
      arg.sg_flags = (arg.sg_flags |CBREAK) & ~ECHO;
      ioctl(0,TIOCSETP,&arg);
}

halfbaked()
{
  struct sgttyb arg;
      ioctl(0,TIOCGETP,&arg);
      arg.sg_flags = (arg.sg_flags |/*CBREAK*/RAW) & ~ECHO;
      ioctl(0,TIOCSETP,&arg);
}

#endif /* POSIX */

  
getaccountinformation(login, password, storedaccountnumber)
  char *login, *password, *storedaccountnumber;
{
  bool accountfound, findaccount();
  char storedpassword[20];
 
	accountfound = findaccount(login, storedpassword, storedaccountnumber);
	if(accountfound) {
		if(strcmp(password, storedpassword)) {
			fprintf(stderr, "Sorry, incorrect password.\n");
			*storedaccountnumber = NULL;
		}
	} else {
		strcpy(storedaccountnumber,NOACCOUNT);	
	}
}

  bool
findaccount(login, storedpassword, storedaccountnumber)
  char *login, *storedpassword, *storedaccountnumber;
{
  FILE *fopen(), *accountfile;
  char storedlogin[100];
  int c;
  bool endofline, getnextfield();

	if(!login){
		return (FALSE);
	}
	if ((accountfile = fopen(accountfilename, "r")) == NULL)
		return(FALSE);

	for(c = 0;c != EOF;) {
		endofline = getnextfield(accountfile, storedlogin);
		if (!strcmp(login, storedlogin)) {
			if(!endofline) {
				endofline = getnextfield(accountfile, storedpassword);
			} else {
				fprintf(stderr, "premature end-of-line in account file.  No password field.\n");
				break;
			}
			if(!endofline) {
				endofline = getnextfield(accountfile, storedaccountnumber);
			} else {
				fprintf(stderr, "premature end-of-line in account file.  No account number field.\n");
				break;
			}
			fclose(accountfile);
			return(TRUE);
		} else {
			for(c = getc(accountfile);c != '\n' && c != EOF; c = getc(accountfile))
				;
		}	
	}
	fclose(accountfile);
	return(FALSE);
}

  bool
getnextfield(accountfile, field)
  FILE *accountfile;
  char *field;
{
  int i, c;

	for(i=0;(c = getc(accountfile)) != ':' && c != '\n' && c != EOF;i++) {
		field[i] = c;
	}
	field[i] = '\0';
	return(c == '\n' || c == EOF);
}





