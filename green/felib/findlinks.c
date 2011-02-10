/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:   findlinks.d
***************************************************/

#include "fest.h"

tumbler indexdocvposition =     {0,0, 1,1,0,1,0,2,0,1,1,0,0,0}; /* 0.0.2.0.1.1 */
tumbler linktypedoc       =     {0,0, 1,1,0,1,0,2,0,0,0,0,0,0}; /* 0.0.2 */

smalltumbler jumplinktype     =      {0,0, 2,1}; /* 2.1 */
smalltumbler quotelinktype    =      {0,0, 2,2}; /* 2.2 */
smalltumbler footnotelinktype =      {0,0, 2,3}; /* 2.3 */
smalltumbler marginallinktype =      {0,0, 2,4}; /* 2.4 */

smalltumbler *linktypeisas[] = {
        &jumplinktype,
        &quotelinktype,
        &footnotelinktype,
        &marginallinktype
};

char *linktypelist[] = {
        "jump",
        "quote",
        "footnote",
        "marginal note"
};

   void
findlinktype (linkidptr, linktypeptr)
  tumbler *linkidptr;
  med *linktypeptr;
{
  spectype *specptr;
  tasktype task;
  smalltumbler *vsaptr;

        retrieveendsetfrombackend (&task, linkidptr, 3, &specptr);
        if (!specptr || !tumblereq(&specptr->docid,&linktypedoc)) {
                *linktypeptr = DEFAULTLINK;
                tfree (&task.tempspace);
                return;
        }
	vsaptr = &specptr->specspanptr->vstartaddress;

	if (smalltumblereq(vsaptr,&jumplinktype)) {
		*linktypeptr = JUMPLINK;
	} else
	if (smalltumblereq(vsaptr,&quotelinktype)) {
		*linktypeptr = QUOTELINK;
	} else
	if (smalltumblereq(vsaptr,&footnotelinktype)) {
		*linktypeptr = FOOTNOTELINK;
	} else
	if (smalltumblereq(vsaptr,&marginallinktype)) {
		*linktypeptr = MARGINALNOTELINK;
	} else {
                *linktypeptr = DEFAULTLINK;
	}
        tfree (&task.tempspace);
}

  void
findlinkspecset (linkidptr, whichend, endsetptr)
  tumbler *linkidptr;
  med whichend;
  spectype **endsetptr;
{
  spectype *specptr;
  tasktype task;

        retrieveendsetfrombackend (&task, linkidptr, whichend, &specptr);
        copyspecset (specptr, endsetptr);
        tfree (&task.tempspace);
}

  void
maketempthreeset(linktypenum,tempthreesetptr)
  med linktypenum;
  spectype **tempthreesetptr;
{
        *tempthreesetptr = specspanalloc ();
        movetumbler (&linktypedoc, &(*tempthreesetptr)->docid);
        movesmalltumbler (linktypeisas[linktypenum], &(*tempthreesetptr)->specspanptr->vstartaddress);
        (*tempthreesetptr)->specspanptr->sizeofspan = 1;
}

#ifdef COMMENT
  void
findnextendsetafterspec (taskptr, specptr, endsetptrptr)
  tasktype *taskptr;
  spectype *specptr;
  spectype **endsetptrptr;
{
  long tempvsa;

        tempvsa = specptr->specspanptr->vstartaddress.smallmantissa[1];
        specptr->specspanptr->vstartaddress.smallmantissa[1] += specptr->specspanptr->sizeofspan;
        findnextendset (taskptr, specptr, endsetptrptr);
        specptr->specspanptr->vstartaddress.smallmantissa[1] = tempvsa;
}

  void
findnextendset (taskptr, specptr, endsetptrptr)
  tasktype *taskptr;
  spectype *specptr;
  spectype **endsetptrptr;
{
  spectype *fromsetptr, *tosetptr;
  long tempsize;

        if (*endsetptrptr) {
                specfree (*endsetptrptr);
        }
        tempsize = specptr->specspanptr->sizeofspan;
        specptr->specspanptr->sizeofspan = BIGVIEW;
        retrieveendsetswithinspecsetfrombackend (taskptr, specptr, &fromsetptr, &tosetptr, NULL);
        specptr->specspanptr->sizeofspan = tempsize;
        findspecspaninfromandtosetswithlowestvsa (taskptr, fromsetptr, tosetptr, endsetptrptr);
}

  void
findspecspaninfromandtosetswithlowestvsa (taskptr, fromsetptr, tosetptr, specptrptr)
  tasktype *taskptr;
  spectype *fromsetptr;
  spectype *tosetptr;
  spectype **specptrptr;
{
  spectype *bestone;
  spantype *spanptr;
  spectype *originaltosetptr;
        originaltosetptr = tosetptr;
        for (bestone = tosetptr; tosetptr; tosetptr = tosetptr->nextspec) {
                if (tumblercmp (&bestone->docid, &tosetptr->docid) != LESS) {
                        bestone = tosetptr;
                        for (spanptr = tosetptr->specspanptr; spanptr; spanptr = spanptr->nextspan) {
                                if (spanvsacmp (bestone->specspanptr, spanptr) == GREATER)
                                        bestone->specspanptr = spanptr;
                        }
                }
        }
checkalloc("findspsp1");
        if (!originaltosetptr)
                bestone = fromsetptr;
        for (; fromsetptr; fromsetptr = fromsetptr->nextspec) {
                if (tumblercmp (&bestone->docid, &fromsetptr->docid) != LESS) {
                        /*bestone = fromsetptr;*/
                        for (spanptr = fromsetptr->specspanptr; spanptr; spanptr = spanptr->nextspan) {
                                if (spanvsacmp (bestone->specspanptr, spanptr) == GREATER)
                                        bestone->specspanptr = spanptr;
                        }
                }
        }
checkalloc("findspsp2");
        bestone->nextspec = NULL;
        bestone->specspanptr->nextspan = NULL;
        copyspecset (bestone, specptrptr);
checkalloc("findspsp3");
if ((!*specptrptr&&bestone)||(!bestone&&*specptrptr))
gerror("unhappy in findspecspaninfromandtosetswithlowestvsa");
}
#endif
