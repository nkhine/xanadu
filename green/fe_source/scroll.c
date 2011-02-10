/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:      scroll.d
***************************************************/

#include "fest.h"
#include "feminreq.h"

  long nextl();

  void
cursorposition (requestptr)
  requesttype *requestptr;
{
  spectype *specalloc();

        if (!iszerotumbler (&requestptr->requestvposition)) {
                specfree (currentwindow->windowview.viewspecptr); /* zzzz */
                currentwindow->windowview.viewspecptr = specalloc ();
                copyvpositionintospec (&requestptr->requestvposition, currentwindow->windowview.viewspecptr);
                currentwindow->windowview.viewspecptr->specspanptr->sizeofspan = BIGVIEW;
                if (currentwindow->typeofwindow == JUMPLINK) {
                       fillwindow(currentwindow,&requestptr->requestvposition);
                } else {
                       fillwindowfromwindowspec (currentwindow);
                }
                homecursorposition ();  
                if(currentwindow->ispartofparalleltextface){
                        followcursorcontrol();
                }
		wrefresh(currentwindow->w);
                return;
                       /* when we reset screen area deltas will
                        refer to absolute positions to simplify things */
        }
        if (requestptr->requestnumberoflines) {
                fescroll ((med)requestptr->requestnumberoflines);
                requestptr->requestnumberoflines = 0;
        }
        if (TRUE || requestptr->requestcursordeltax || requestptr->requestcursordeltay) {
                movecursor (requestptr->requestcursordeltax, requestptr->requestcursordeltay);
                requestptr->requestcursordeltax = requestptr->requestcursordeltay = 0;
        }
	if(currentwindow->ispartofparalleltextface){
		followcursorcontrol();
	}
	wrefresh(currentwindow->w);
}


  void
fescroll (numberoflines)
  med numberoflines;
{
        if (numberoflines == 0) {
                return;
        }
        extendviewinwindow (numberoflines);
        if (numberoflines > 0) {
                scrolldown (numberoflines);
        } elif (numberoflines < 0) {
                scrollup (-numberoflines);
        }
}

  void
scrolldown (numberoflines)
  med numberoflines;
{
  med i;
  char text[SCREENWIDTH+1];
  masktype displaymaskptr[SCREENWIDTH+1];

        if ((numberoflines > 1) && (iswindow||currentwindow->windowwidth < screenwidth) &!(iswindow&&numberoflines < currentwindow->windowheight/2)) {
                scrolllotsdown (numberoflines);
                return;
        }
        for (i = 0; i < numberoflines; i++) {
                currentwindow->windowview.characterdisplacementwithinview = currentwindow->windowleftmargins[1]-1;
                grabnextlinedown (text, displaymaskptr);
                insertlineatbottom (currentwindow, text, displaymaskptr);
        }
}

  void
scrolllotsdown (numberoflines)
  med numberoflines;
{

        if(numberoflines<0){
                gerror("Numberof lines negative entering scroll lots down");
        }
        if (numberoflines < currentwindow->windowheight) {
                    
                        currentwindow->windowleftmargins[0] = currentwindow->windowleftmargins[numberoflines];
                        paintwindowplain (currentwindow);
                
        } else {
                currentwindow->windowleftmargins[0] = currentwindow->windowleftmargins[currentwindow->windowheight-1];
                numberoflines -= currentwindow->windowheight /*- currentwindow->rcursl*/;
if (numberoflines < 0) gerror("Numberoflines negative in scrolllotsdown.");
                while (numberoflines--) {
                        currentwindow->windowleftmargins[0] = nextl (currentwindow->windowleftmargins[0]);
                }
                paintwindowplain (currentwindow);
        }
}

  void
scrollup (numberoflines)
  med numberoflines;
{
  med i;
  char text[SCREENWIDTH+1];
  masktype displaymaskptr[SCREENWIDTH+1];
  bool attop();

        for (i = 1; i <= numberoflines; i++) {
                if (attop (currentwindow->windowleftmargins[0])) {
                        if (currentwindow->windowview.characterdisplacementwithinview != 0) {
                                currentwindow->windowview.characterdisplacementwithinview = 0;
                                currentwindow->windowleftmargins[0] = 1;
                                currentwindow->windowline_glorpfed[0] = TRUE;
                                fixglorpfsinwindow (currentwindow);
				wrefresh (currentwindow->w);
                                return;
                        } else {
                                fixglorpfsinwindow (currentwindow);
				wrefresh (currentwindow->w);
                                return;
                        }
                }else{
                        i = grabnextlineup (text, displaymaskptr);
                        currentwindow->windowleftmargins[0] = i;
                        setdisplacement (i);

                        insertlineattop (currentwindow, text, displaymaskptr);
                }
        }
}

  long
grabnextlineup (textptr, displaymaskptr)
  char *textptr;
  masktype *displaymaskptr;
{
  spectype *specptr;
  spectype *specalloc();
  long temp;

        specptr = specalloc ();
        temp = determinespecsetfornextlineup (specptr);
        grabspec (specptr, textptr);

        grabmask (specptr, displaymaskptr);
        specfree (specptr);
        return (temp);
}

  long
determinespecsetfornextlineup (specptr)
  spectype *specptr;
{
  long lastl(),temp;

        getspecsetfordisplacementinviewinwindow (currentwindow, 
						 (temp=lastl(currentwindow->windowleftmargins[0])),
					         (long)currentwindow->windowwidth, specptr);
        return(temp);
}

 med
grabnextlinedown (textptr, displaymaskptr)
  char *textptr;
  masktype *displaymaskptr;
{
  spectype *specptr;
  spectype *specalloc();
  med temp;

        specptr = specalloc ();
        determinespecsetfornextlinedown (specptr);

if (!specptr->specspanptr || !specptr->specspanptr->sizeofspan)
gerror ("Gross out in grabnextlinedown.");

        temp = grabspec (specptr, textptr);
        grabmask (specptr, displaymaskptr);
        specfree (specptr);
        return (temp);
}

  void
determinespecsetfornextlinedown (specptr)
  spectype *specptr;
{
  long temp;

        getspecsetfordisplacementinviewinwindow (currentwindow, 
          (temp =nextl(currentwindow->windowleftmargins[currentwindow->windowheight-1])),
          (long)currentwindow->windowwidth, specptr);
        currentwindow->windowleftmargins[currentwindow->windowheight] = temp;
}

  void
grabmask (specptr, displaymask)
  spectype *specptr;
  masktype *displaymask;
{
        clearmask (displaymask);
        fillupmask (currentwindow, -1, specptr, displaymask);/* mask for place where cursor isn't thus -1 */
}

 med
grabspec (specptr, textptr)
  spectype *specptr;
  char *textptr;
{
  spectype *localspecset;
  char *localtextptr;
  documenttype *docforspec;
  med textsize;
        if ((!specptr) || (!(specptr->specspanptr)))
		gerror ("No span in grabspec.");

        copyspecset (specptr, &localspecset);
        localtextptr = textptr;
        retrievefromvm (&localspecset, &localtextptr);
        if (localspecset) {
		if (!(docforspec = finddocumentinvmlist(&specptr->docid))) {
			retrievefrombackend (localspecset, localtextptr);
		} else {
			textsize = retrievetextsize(docforspec);
			if (specptr->specspanptr->vstartaddress.smallmantissa[1] <= textsize) {
				retrievefrombackend (localspecset, localtextptr);
			}
		}
                specfree (localspecset);
        }
        return (strlen (textptr));
}
 
  med
retrievetextsize(docptr)
  documenttype *docptr;
{
  spantype *spanset;

/*
fprintf(stderr,"\n>---------------------\n");
dumptumbler(&docptr->documentid);
*/

	if (docptr->istextsizevalid == FALSE) {
		retrievedocvspanset(&docptr->documentid,&spanset);
		for (;spanset;spanset = spanset->nextspan) {
/*
fprintf(stderr,"{");
dumpsmalltumbler(&spanset->vstartaddress);
fprintf(stderr,"}\n");
fprintf(stderr,"%d ",spanset->vstartaddress.smallmantissa[0]);
fprintf(stderr,"%d ",spanset->vstartaddress.smallmantissa[1]);
fprintf(stderr,"\n");
*/

			if (spanset->vstartaddress.smallmantissa[0] == 1
			  && spanset->vstartaddress.smallmantissa[1] == 1) {
				docptr->textsize = spanset->sizeofspan;
				docptr->istextsizevalid = TRUE;
				break;
			}
		}
	}
	if (docptr->istextsizevalid != TRUE)
		gerror("couldn't find textsize");
	return(docptr->textsize);
}

  void
grabspecwithouttext (specptr)
  spectype *specptr;
{
  spectype *localspecset;
  med oldlength;
/*
fprintf(stderr,"grabspecwithouttext %x\n",specptr);
*/
        if ((!specptr) || (!(specptr->specspanptr)))
                gerror ("No span in grabspec.");

        copyspecset (specptr, &localspecset);
        if (localspecset) {
                retrievefrombackend (localspecset,(char *)NULL);
                specfree (localspecset);
        }
        return ;
}

#ifdef COMMENT
  void
changedisplacement (i)   /* this assumes line starts are view relative??*/
  med i;
{
        currentwindow->windowview.characterdisplacementwithinview += i;
        currentwindow->windowleftmargins[0] = currentwindow->windowview.characterdisplacementwithinview+1;
}
#endif COMMENT

  void
setdisplacement (i)   /* this assumes line starts are view relative??*/
  med i;
{
        currentwindow->windowview.characterdisplacementwithinview = i;
        currentwindow->windowleftmargins[0] = i+1;
}

  void
extendviewinwindow (numberoflines)
  med numberoflines;
{
        /*construct extension to view*/
 /*doquotelinks()*/
 /*dolinkspans() to get maskinformation*/
}
