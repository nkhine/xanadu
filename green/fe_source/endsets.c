/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:     endsets.d
***************************************************/

#include "fest.h"
#include "feminreq.h"

  long nextl();
  void
updateendsetlist (newspec, specset)
  spectype *newspec, *specset;
{
  spantype *tmp, *next;
  long i, j;

        if (tumblereq (&currentwindow->windowview.viewspecptr->docid, &newspec->docid)) {
                i = addressisonthisline (newspec->specspanptr->vstartaddress.smallmantissa[1]);
                j = addressisonthisline ((tdigit)(newspec->specspanptr->vstartaddress.smallmantissa[1] + newspec->specspanptr->sizeofspan));


                for (; i <= j; ++i) {
                        currentwindow->windowline_glorpfed[i-1] = TRUE;
                }

                if (!specset) {
                        copyspecset (newspec, &specset);
                        return;
                }
                for (tmp = specset->specspanptr; tmp; tmp = tmp->nextspan) {
                        if (smalltumblercmp (&tmp->vstartaddress, &newspec->specspanptr->vstartaddress) != GREATER) {
                              next = tmp->nextspan;
                              copyspanset(newspec->specspanptr,&tmp->nextspan);
                              newspec->specspanptr->nextspan = next;
                              return;
                        }
                }
        }
}

  void
changetempendset (endsetptr)
  spectype **endsetptr;
{
 
currentwindow ->isendsetokflag = FALSE;
       if (currentwindow->windowview.viewcutset) {
                copyspecset (currentwindow->windowview.viewcutset, endsetptr);
                clearcuts();  /* this stuff shoulden't use current window zzz*/
        } else {
		*endsetptr = specalloc();
                findspecfromcursorposition(*endsetptr)/*NULL*/;
        }
}

  void
cleartempendsets()
{

        if (tempfromset) {
                specfree (tempfromset);
                tempfromset = NULL;
        }
        if (temptoset) {
                specfree (temptoset);
                temptoset = NULL;
        }
        if (tempthreeset) {
                specfree (tempthreeset);
                tempthreeset = NULL;
        }
}

  med
findspecfromcursorposition (specptr)
  spectype *specptr;
{
  long disp;
  long beyondlogicaleol;

        beyondlogicaleol = 0;
        disp = currentwindow->windowleftmargins[currentwindow->rcursl] + currentwindow->rcursc /*zzz+ 1*/;
       if(disp>nextl(currentwindow->windowleftmargins[currentwindow->rcursl])){
           beyondlogicaleol=disp-nextl(currentwindow->windowleftmargins[currentwindow->rcursl]) + 1 /* zzz+1*/;
/*printf("beyondlogicaleol %d",beyondlogicaleol);*/
        }
        getspecsetfordisplacementinviewinwindow (currentwindow,(long)( disp-beyondlogicaleol), (long)1, specptr);
        return ((med)beyondlogicaleol);
}

  void
findspecfromcursorpositionoffsetandlength (specptr, offset, length)
  spectype *specptr;
  long offset, length;
{
  long disp;
  long beyondlogicaleol;

        beyondlogicaleol = 0;
        disp = /*max (*/currentwindow->windowleftmargins[currentwindow->rcursl  ] + currentwindow->rcursc /*+ 1*/ + offset/*, 0)*/;
        if (disp > nextl (currentwindow->windowleftmargins[currentwindow->rcursl])){
           beyondlogicaleol = disp - nextl (currentwindow->windowleftmargins[currentwindow->rcursl]);
        }

        getspecsetfordisplacementinviewinwindow (currentwindow, (long)(disp-beyondlogicaleol), length, specptr);
}
  void
makedocvsa (tumblerptr, smalltumblerptr, fulltumblerptr)
  tumbler *tumblerptr, *fulltumblerptr;
  smalltumbler *smalltumblerptr;
{
  med findlastplace();
  med i;
  med j;

        movetumbler (tumblerptr, fulltumblerptr);
        i = findlastplace (tumblerptr) + 1;
        for (j = 0; j < SMALLNPLACES; ++j) {
               fulltumblerptr->mantissa[++i]=smalltumblerptr->smallmantissa[j];
        }

}
  tdigit
addressisonthisline (address)
  tdigit address;
{
  med i;
        for (i = 0; i < currentwindow->windowheight; ++i) {
                if (/*currentwindow->windowview.characterdisplacementwithinview + */currentwindow->windowleftmargins[i] > address) {
                        return (i);
                }
        }
        return(currentwindow->windowheight);/*this is an ERROR ZZZ*/
}

  void
baderror(s)
  char *s;
{
        fprintf(stderr,"%s",s);
        fprintf(stderr,"bad error\n\n\n");
}
