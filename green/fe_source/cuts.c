/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:        cuts.d
***************************************************/

#include "fest.h"

  void
makecut()  /* assumes window contains only one document */
{
  spectype *specptr, *specalloc();
  void updatecutseq();
  void displaycuts();

        if (cutsequence.numberofcuts > 2) {
                gotoxy (0,screenheight-1);
                fprintf (stderr, "too many cuts ");
                return;
        }
        specptr = specalloc ();
        findspecfromcursorposition (specptr);

        /*specptr->specspanptr->vstartaddress.smallmantissa[1]);*/

        updatecutseq (&specptr->specspanptr->vstartaddress);
        displaycuts (specptr);
        specfree (specptr);
}
  void
clearcuts()
{
  void clearcutsfromview();

        clearcutsfromview();
        memclear ((char *)&cutsequence, sizeof(cutseqtype));
}
  void
clearcutsfromview()
{
  void setglorpfsforcutsinwindow();
        if (!currentwindow->windowview.viewcutset){
		fprintf(stderr,"shfu __ weird return from clearcutsfromview");
                return;
	}
        setglorpfsforcutsinwindow();
        specfree (currentwindow->windowview.viewcutset);
        currentwindow->windowview.viewcutset = NULL;
        fixglorpfsinwindow (currentwindow);
	wrefresh (currentwindow->w);
}
  void
updatecutseq (cut)
  smalltumbler *cut;
{
  med c;
        if (cutsequence.numberofcuts == 0) {
                movesmalltumbler (cut, &cutsequence.cutsarray[0]);
                movesmalltumbler (cut, &cutsequence.cutsarray[1]);
                ++(cutsequence.cutsarray[1].smallmantissa[1]);
        } else if (cutsequence.numberofcuts == 1) {
                c = smalltumblercmp (cut, &cutsequence.cutsarray[0]);
                switch (c) {
                  case EQUAL:
                        return;
                  case LESS:
                        movesmalltumbler (cut, &cutsequence.cutsarray[0]);
                        --(cutsequence.cutsarray[1].smallmantissa[1]);
                        break;
                  case GREATER:
                        movesmalltumbler (cut, &cutsequence.cutsarray[1]);
                        ++(cutsequence.cutsarray[1].smallmantissa[1]);
                        break;
                }
        }

        ++cutsequence.numberofcuts;
}


  bool
completecutseq (cut)
  smalltumbler *cut;
{
  med c;
	if(cutsequence.numberofcuts != 2)
		return(FALSE);
        cutsequence.numberofcuts = 3;
        c = smalltumblercmp (cut, &cutsequence.cutsarray[0]);

        if (c == EQUAL) {
                return (FALSE);
        }
        if (c == LESS) {
                movesmalltumbler (&cutsequence.cutsarray[1], &cutsequence.cutsarray[2]);
                movesmalltumbler (&cutsequence.cutsarray[0], &cutsequence.cutsarray[1]);
                movesmalltumbler (cut, &cutsequence.cutsarray[0]);
                return (TRUE);
        }

        c = smalltumblercmp (cut, &cutsequence.cutsarray[1]);

        if (c == EQUAL) {
                return (FALSE);
        }
        if (c == LESS) {
                movesmalltumbler (&cutsequence.cutsarray[1], &cutsequence.cutsarray[2]);
                movesmalltumbler (cut, &cutsequence.cutsarray[1]);
                return (TRUE);
        }

        movesmalltumbler (cut, &cutsequence.cutsarray[2]);
        /*(cutsequence.cutsarray[2].smallmantissa[1]);*/
        return (TRUE);
}


  void
displaycuts (specptr)
  spectype *specptr;
{

        if (!currentwindow->windowview.viewcutset) {
                copyspecset (specptr, &currentwindow->windowview.viewcutset);
                currentwindow->windowview.viewcutset->specspanptr->sizeofspan = 1;
                currentwindow->windowline_glorpfed[currentwindow->rcursl-1] = TRUE;
        } else {
                currentwindow->windowview.viewcutset->specspanptr->sizeofspan =  diff (
      (nat)( currentwindow->windowview.viewcutset->specspanptr->vstartaddress.smallmantissa[1]), (nat)(specptr->specspanptr->vstartaddress.smallmantissa[1])) + 1;

                currentwindow->windowview.viewcutset->specspanptr->vstartaddress.smallmantissa[0] = 1;

                currentwindow->windowview.viewcutset->specspanptr->vstartaddress.smallmantissa[1] =

 min (
      currentwindow->windowview.viewcutset->specspanptr->vstartaddress.smallmantissa[1], specptr->specspanptr->vstartaddress.smallmantissa[1]);

        }
        setglorpfsforcutsinwindow();
        fixglorpfsinwindow (currentwindow);
	wrefresh (currentwindow->w);
}
  void
setglorpfsforcutsinwindow()
{
  med i, j;

/**/	i = addressisonthisline (currentwindow->windowview.viewcutset->specspanptr->vstartaddress.smallmantissa[1]);

        j = addressisonthisline ((tdigit)(currentwindow->windowview.viewcutset->specspanptr->vstartaddress.smallmantissa[1]
                    + currentwindow->windowview.viewcutset->specspanptr->sizeofspan));
/**/

 /*       i = addressisonthisline (&cutsequence.cutsarray[0].smallmantissa[1]);
        j = addressisonthisline (&cutsequence.cutsarray[2].smallmantissa[1]);
*/
        for (; i <= j; ++i) {
                currentwindow->windowline_glorpfed[i-1] = TRUE;
        }

}
