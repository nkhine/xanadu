/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:   typeahead.d
***************************************************/

#include "fest.h"
#include "feminreq.h"
#include "fekeys.h"

#define OLDCURSES

#define ESC 0x1b
#define MASK8BIT 0xff

static med kount = 0;


  metachar
gettypeahead()
{
  med i,j,k,nread;
  bool istypeahead();
  metachar mchar;
  bool flag = FALSE; /*temporary flag for IGNOREMETACHAR*/
  med getmorechars();

        fflush(stdout);
	if (istypeahead()) {
		kount--;
		return (*firsttypeahead++);
	}
	if (iswindow){
		gerror("should never do this in gettypeahead");
		return(-1) /* for lint */;
	}
	fflush(stdin);
	nread = getmorechars(fetypeahead);
        if (nread > 0) {
                nexttypeahead = firsttypeahead = fetypeahead;
                nexttypeahead += nread;
                return (*firsttypeahead++);
        } else {
                system ("cp core backend.core");
                gerror ("End-of-file in gettypeahead.");
         			/* we want to save the backend core file first */
        }
        return(-1); /* for lint */
}


/*
typeaheadinit() { nexttypeahead = firsttypeahead = fetypeahead; checktypeahead(); return; } 
*/


  metachar
peektypeahead()
{
if(debug>1){
if(debug) fprintf(stderr,"peek.. |%c| %d\n",*firsttypeahead,*firsttypeahead);
}
        checktypeahead();
        if (firsttypeahead == nexttypeahead)
                return (EOF);
        return ((metachar)*firsttypeahead);
}


  void
checktypeahead()
{
      return;
}

  bool
istypeahead()
{
        return (firsttypeahead != nexttypeahead);
}

  bool
besureistypeahead()
{
  metachar waste;
        if(iswindow){
                if(!istypeahead()){
                        fprintf(stderr,"\nI'm surely wrong in besure  kount %d ",kount);
                        return(FALSE);
                }
                return(TRUE);
        }
        if(!istypeahead()){
                waste = gettypeahead();
                firsttypeahead--;
        }
        return(TRUE);
}



#ifdef OLDCURSES	/* Get what old Curses doesn't give us from Termcap */

char PC;
char *BC;
char *UP;
short ospeed;

#define KL	0
#define KR	1
#define KU	2
#define KD	3

#define NCAPS	4

char capnames[NCAPS][3] = {"kl","kr","ku","kd"};
char *caps[NCAPS];
char vchars[NCAPS] = {CLEFT,CRIGHT,CUP,CDOWN};

initcap()
{
  register i,j;
  char *bp,*area,*s,*malloc(),*tgetstr(),*getenv();

	bp = malloc(1024);
	area = malloc(512);
	if (tgetent(bp,getenv("TERM")) != 1) {
		gerror("Termcap error");
	}
	for (i=0; i<NCAPS; i++) {				/* ARROW KEYS */
		if (s = tgetstr(capnames[i],&area)) {
			caps[i] = malloc(strlen(s)+1);
			strcpy(caps[i],s);
		} else {
			caps[i] = (char *)NULL;
		}
	}
	free(bp);
	free(area);
}

keyset(w)
  WINDOW *w;
{
}



#define NOTENOUGH -1

  med
getmorechars(buf)
  metachar buf[];
{
  register i; 
  metachar c;

  char charbuf[NTYPEAHEAD+8];
  metachar virtualchar();
  med next=0,nread;
  metachar *p;

	nread = read(0,charbuf,NTYPEAHEAD);
	for (i=0; i < NTYPEAHEAD && next < nread; ) {			
		while ((c = virtualchar(charbuf,nread,&next)) == NOTENOUGH) {
			read(0,&charbuf[nread],1);
			nread++;
		}
		buf[i++] = c;
	}
	return(i);
}

  metachar
virtualchar(buf,nchars,idx)
  char buf[];
  med nchars,*idx;
{
  register i,j;
  med len,rem,n;
  char *p;

	for (i=0; i<NCAPS; i++) {
		p = &buf[*idx];
		len = strlen(caps[i]);
		rem = nchars - (*idx);
		n = min(rem,len);
		if (strncmp(&buf[*idx],caps[i],n) == 0) {
			if (n==len) {
				(*idx) += len;
				return((metachar)vchars[i]);
			}
			return(NOTENOUGH);
		}
	}
	if (*p == DEL2) *p = DEL;
	(*idx)++;
	return((metachar)*p);
}


#else

initcap()		/* to keep the #ifdef OLDCURSES mess local */
{
}

keyset(w)
  WINDOW *w;
{
	keypad(w,TRUE);
}

  med
getmorechars(buf)
  metachar buf[];
{
  register i; 
  metachar c;

	nodelay(currentwindow->w,FALSE);     /* wait for the first character  */
	for(i=0; i<NTYPEAHEAD; i++) {        /* then read up to a buffer-full */
		c = wgetch(currentwindow->w);
		nodelay(currentwindow->w,TRUE);
		if (c == -1) {                      /* that's all, wait again */
			nodelay(currentwindow->w,FALSE); 
			break;
		}
		if	(c == KEY_UP) 	 buf[i] = CUP;
		else if	(c == KEY_DOWN)	 buf[i] = CDOWN;
		else if	(c == KEY_RIGHT) buf[i] = CRIGHT;
		else if	(c == KEY_LEFT)	 buf[i] = CLEFT;
		else if	(c == DEL2)	 buf[i] = DEL;
		else			 buf[i] = c;
	}
	nodelay(currentwindow->w,FALSE);
	return(i);
}

#endif
