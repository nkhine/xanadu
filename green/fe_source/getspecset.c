/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:  getspecset.d
***************************************************/

#include "fest.h"
#include "feminreq.h"

  void
getspecsetfordisplacementinviewinwindow(window,displacement,numberofcharacters,specptr)
  windowtype *window;
  long displacement, numberofcharacters;
  spectype *specptr;
{
  long nchars;
/*
 fprintf(stderr,"getspecsetfordisplacementinviewinwindow\n");
*/
        while (numberofcharacters) {
                nchars = getspaninviewinwindow (window, displacement, numberofcharacters, specptr);
                numberofcharacters -= nchars;
                displacement += nchars;
        }
}

 long
getspaninviewinwindow (window, displacement, numberofcharacters, specptr)
  windowtype *window;
  long displacement, numberofcharacters;
  spectype *specptr;
{
  spectype *viewspec;
  spantype *viewspan;
  long i;
  long makespanfromviewspan();

        viewspec = window->windowview.viewspecptr;
        if (viewspec) {
                if (viewspan = viewspec->specspanptr) {
                        /* Go thru view for <displacement> characters */
                       for (i = 0; i < displacement;) {
                                i += viewspan->sizeofspan;
                                if (i >/*=*/ displacement)/*zzz*/
                                        break;
                                if (!(viewspan = viewspan->nextspan)) {
                                        viewspec = viewspec->nextspec;
                                        if (viewspec)
                                                viewspan=viewspec->specspanptr;
                                }
                        }
                } else
                        /* we'll get to this in a bit, but for now... */
                        gerror ("No viewspanset\n");
        } else
                gerror ("No viewspec in window\n");

        return (makespanfromviewspan (viewspec, viewspan, displacement, (long)(i+1), (long)numberofcharacters, specptr));
}

  long
makespanfromviewspan (viewspec, viewspan, displacement, offset, numberofcharacters, specptr)
  spectype *viewspec;
  spantype *viewspan;
  long displacement, offset, numberofcharacters;
  spectype *specptr;
{
  spantype *spanptr;
/*
 fprintf(stderr,"makespanfromviewspan\n");
*/
        /* find end of specset */
        while (specptr->nextspec)
                specptr = specptr->nextspec;
        spanptr = specptr->specspanptr;
        while (spanptr && spanptr->nextspan)
                spanptr = spanptr->nextspan;

/*        if (!tumblercmp (&specptr->docid, &viewspec->docid)) {
                appendvmtypethingtolist (specptr, specalloc ());
                specptr = specptr->nextspec;
        }
*/
        movetumbler (&viewspec->docid, &specptr->docid);
        if (spanptr) {
                 appendvmtypethingtolist ( (vmthingtype*)spanptr,  (vmthingtype*)spanalloc ());
                 spanptr = spanptr->nextspan;
        } else {
                 appendvmtypethingtolist ( (vmthingtype*)specptr,  (vmthingtype*)spanalloc ());
                 spanptr = specptr->specspanptr;
        }
        movesmalltumbler (&viewspan->vstartaddress, &spanptr->vstartaddress);
        if (offset == displacement) {
                 spanptr->sizeofspan = min (numberofcharacters, viewspan->sizeofspan);
        } else {
                spanptr->vstartaddress.smallmantissa[1]+=viewspan->sizeofspan/*0+1*/ - offset + displacement/*-1;*/;
                 spanptr->sizeofspan = min (numberofcharacters, offset - displacement);
        }

        return (spanptr->sizeofspan);
}
