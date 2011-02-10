/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:        init.d
***************************************************/

#include <signal.h>

#include "fest.h"
#include "feminreq.h"
 
  med findlastplace();

/*tumbler staticdocvposition = {0,-2,  1,0,1,1,0,0,0,0};*/ /* 0.0.1.0.1.1 */
tumbler staticdocvposition = {0,0,  1,1,0,1,0,1,0,1,1,0,0}; /* 1,1,0,1,0.1.0.1.1 */

/*tumbler foostaticlinkhomeposition = {0,-2,  2,0,0,0,0,0,0,0}; *//* 0.0.2.0 */
tumbler foostaticlinkhomeposition = {0,0,  1,1,0,1,0,2,0,0,0,0,0,0}; /* 0.0.2.0 */

  void
init (docvpositionptr)
  tumbler *docvpositionptr;
{
  long l;
  windowtype *windowalloc();
  documenttype *initdoc, *documentalloc();
  spectype *specalloc();
  tasktype task;

#ifdef AMIGA
	AmiInit();
#endif
#ifdef MSDOS
extern int ibmmpen();
extern void waitforchars();
extern void waitforchar();

        init_comm();
        set_tty(0,12,3,1); /*port 1,baud rate (12=9600,96=1200) 1 s,no 
        par,on*/
/*
        feputstring("ATDT 249-4020",0);  /*read tel # from file later*/
        feputc(0x0D);                    /*CR*/
        /*     while ((get_lsr() && 0x8000) ==NULL){};*/
/* could wait until carier det comes on, or can use "CONNECT<CR>" which
comes from the modem.*/
/* This input startup sequence depends on the modem setup, fix later*/

        waitforchars('T',0x0d);
        fprintf(stderr,"in init.d anyway\n");
*/
for(l=30000;l;l--); 
        inp_flush();   
        feputc(0x0d);     /* cr to initialize to known state*/

        waitforchars(':',0x20);

/* Note, all this code should cope with getting out of sync due to line noise
and recover to known states in the presents of noise.  Thus, the wait routines
should wait for specific times and then return with error set if the two char 
string is not found. (by changing while loops to for loops)*/

        feputstring("mfe",0);
for(l=30000;l;l--);        /*may be bufffering probs, rets password <cr>*/
        feputc(0x0D,0);
        waitforchars('d',0x0d);
                
/* This command returns 3 lines and ends with "password". Need to eat chars
till we see the "d" <cr>. */
for(l = 30000;l;l--);               
        feputstring("w",0); /*anything term with space or cr.*/
for(l = 30000;l;l--);
        feputc(0x0D,0);
        waitforchars('r',0x0d);  /*same here ends with --ter <cr>*/
     
for(l= 30000;l;l--);                
                               /*send backend a "b"*/
        feputstring("b",0);
    
        fprintf(stderr,"sent the b, waiting for a tilda\n");

        waitforchar('~'); 
    
        fprintf(stderr,"got the tilda, cont startup\n");
     
for(l= 90000;l;l--);  /*this may not be needed*/              
        
        ibmopen();  /*screen open only (I think) and not active with msc*/
#endif
/*        debug = 0;
*/
        disableinsert = FALSE;
        initmenuarrays();

        filllinewithblanks(longblankline,SCREENWIDTH);

        /*system("mkdir feprof");*/
        chdir("feprof");
        staticlinkhomeposition = foostaticlinkhomeposition;
        currentwindow = &(theoneandonlyscreen.basewindow);
        currentwindow->prevwindow = currentwindow->nextwindow = NULL;

        movetumbler(&staticdocvposition,docvpositionptr);

        currentwindow->windowview.viewspecptr = specalloc ();
        copyvpositionintospec (docvpositionptr, currentwindow->windowview.viewspecptr);
        currentwindow->windowview.viewspecptr->specspanptr->sizeofspan=BIGVIEW;
        currentwindow->windowheight = screenheight;
        currentwindow->windowwidth = screenwidth/*screenwidth*/;
        currentwindow->lm = 0;
        currentwindow->tm = 0;
        currentwindow->rcursl = 0;
        currentwindow->rcursc = 0;
        currentwindow->ispartofparalleltextface = FALSE;
        currentwindow->windowview.viewmaskspecptr = NULL;
        currentwindow->typeofwindow = JUMPLINK;
	currentwindow->w = newwin(screenheight,screenwidth,0,0);
	keyset(currentwindow->w);

        memclear ((char *)&cutsequence, sizeof(cutseqtype));
        cleartempendsets();

        currentwindow->windowview.viewfromset
        = currentwindow->windowview.viewtoset
        = currentwindow->windowview.viewcutset
        = NULL;

/*        setup displacement in view to match docvposition
          I don't know if these are seperate anyway
*/
}
 
  void
copyvpositionintospec (docpositionptr, specptr)
                          /* breaks a v address into span format */
  tumbler *docpositionptr;
  spectype *specptr;

{
  med i;
  spantype *spanalloc();

        i = findbottomfield (docpositionptr);
        movetumbler (docpositionptr, &specptr->docid);
        specptr->docid.mantissa[i] = 0;
        specptr->docid.mantissa[i+1] = 0;
        appendvmtypethingtolist ( (vmthingtype*)specptr, (vmthingtype*) spanalloc ());
        specptr->specspanptr->vstartaddress.exp = 0;
        specptr->specspanptr->vstartaddress.sign = 0;
        specptr->specspanptr->vstartaddress.smallmantissa[0] = docpositionptr->mantissa[i];
        specptr->specspanptr->vstartaddress.smallmantissa[1] = docpositionptr->mantissa[i+1];
}


  void
copyspecintovposition (docpositionptr, specptr)
                          /* span format breaks a v address into */
  tumbler *docpositionptr;
  spectype *specptr;

{
  med i;

        movetumbler ( &specptr->docid,docpositionptr);
        i = findlastplace (docpositionptr);
        docpositionptr->mantissa[i+2] = 1/*specptr->specspanptr->vstartaddress.smallmantissa[0]*/;
        docpositionptr->mantissa[i+3] = 1/*specptr->specspanptr->vstartaddress.smallmantissa[1]*/;
/*dumptumbler (docpositionptr); */
}


  med
findbottomfield (tumblerptr)
  tumbler *tumblerptr;
{
  med i;
/* this looks questionable*/
        i = findlastplace (tumblerptr);
        for (; tumblerptr->mantissa[i] != 0 && i >= 0; i--) {
        }
        i++;
        return (i);
}

  med
findlastplace (tumblerptr)
  tumbler *tumblerptr;
{
  med i;

        for (i = NPLACES-1; tumblerptr->mantissa[i] == 0 && i >= 0; i--);
        return (i);
}

  tdigit
getcontentoflastplace (tumblerptr)
  tumbler *tumblerptr;
{
        return (tumblerptr->mantissa[findlastplace (tumblerptr)]);
}

 tdigit
getcontentofsmallbottomfield (smalltumblerptr)
  smalltumbler *smalltumblerptr;
{
        return (smalltumblerptr->smallmantissa[1]);
}     

  void
quit()
{
        closeupshop();
        exit(0);
}
      
  void
closeupshop()
{
#ifdef AMIGA
	AmiCleanup();
#endif
					/* CURSES STUFF */
	wclear(currentwindow->w);
	wrefresh(currentwindow->w);
	noraw();
	endwin();

        tellbackendtoquit();
        fprintf(stderr,"Waiting 2 for Backend to terminate\n\r");
}

  void
clearvdisplacement (displacementptr)
  smalltumbler *displacementptr;
{
        smalltumblerclear (displacementptr);
}
 
  void
cleardocid (docidptr)
  tumbler *docidptr;
{
        tumblerclear (docidptr);
}     

  void
filllinewithblanks(line,number)
  char *line;
  med number;
{
  med i;
        line[number] = '\0';
        for(i = number-1;i>=0;i--){
                line[i] = ' ';
        }
}

  void
printblanks(n)
  med n;
{
  med i;
        waddstr(currentwindow->w,&longblankline[SCREENWIDTH -n]);
}

