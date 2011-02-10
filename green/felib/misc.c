/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1988 Xanadu Operating Company
  XU.88.1 Frontend Source Code:        misc.d
***************************************************/

#include "fest.h"

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

  med
spanvsacmp (spana, spanb)
  spantype *spana;
  spantype *spanb;
{
        if (!spana)
                return (LESS);
        if (!spanb)
                return (GREATER);
        if (!spana && !spanb)
                return (EQUAL);
        if (spana->vstartaddress.smallmantissa[1] ==
            spanb->vstartaddress.smallmantissa[1])
                return (EQUAL);
        if (spana->vstartaddress.smallmantissa[1] <
            spanb->vstartaddress.smallmantissa[1])
                return (LESS);
        return (GREATER);
}

 med 
speccmp (speca, specb)
  spectype *speca;
  spectype *specb;
{
  med ret;
  long avsa, bvsa;
  long alen, blen;

        if (!speca || !specb)
                return (TOMYLEFT);
        if ((ret = tumblercmp (&speca->docid, &specb->docid)) != EQUAL)
                return (ret*2);
        else {
                avsa = speca->specspanptr->vstartaddress.smallmantissa[1];
                alen = speca->specspanptr->sizeofspan;
                bvsa = specb->specspanptr->vstartaddress.smallmantissa[1];
                blen = specb->specspanptr->sizeofspan;
                if (avsa+alen < bvsa)
                        return (TOMYRIGHT);
                if (avsa+alen == bvsa)
                        return (ONMYRIGHTBORDER);
                if (bvsa+blen == avsa)
                        return (ONMYLEFTBORDER);
                if (bvsa+blen < avsa)
                        return (TOMYLEFT);
                return (THRUME);
        }
}

  void
findcomonspecset (taskptr, specptr, specset, specptrptr)
  tasktype *taskptr;
  spectype *specptr, *specset;
  spectype **specptrptr;
{
  spectype *tmp, *localspecptr;
  bool findcommonspans();

        *specptrptr = NULL;
        for (tmp = specset; tmp; tmp = tmp->nextspec) {
                if (!tumblereq (&specptr->docid, &tmp->docid))
                        break;
                localspecptr = (spectype*) taskalloc (taskptr, sizeof(spectype));
                movetumbler (&specptr->docid, &localspecptr->docid);
                localspecptr->nextspec = NULL;
                if (!findcommonspans (taskptr, specptr->specspanptr, tmp->specspanptr, &localspecptr->specspanptr))
                        continue;
                *specptrptr = localspecptr;
                specptrptr = &localspecptr->nextspec;
        }
}

  bool
findcommonspans (taskptr, spanptr, spanset, spanptrptr)
  tasktype *taskptr;
  spantype *spanptr, *spanset;
  spantype **spanptrptr;
{
  spantype *tmp, *localspanptr;
  bool ret;
  bool intersectspans();

        ret = FALSE;
        for (tmp = spanset; tmp; tmp = tmp->nextspan) {
                if (intersectspans (taskptr, tmp, spanptr, &localspanptr)) {
                        *spanptrptr = localspanptr;
                        spanptrptr = &localspanptr->nextspan;
                        ret = TRUE;
                }
        }
        return (ret);
}

  bool
intersectspans (taskptr, aspanptr, bspanptr, spanptrptr)
  tasktype *taskptr;
  spantype *aspanptr, *bspanptr;
  spantype **spanptrptr;
{
  long astart, aend, bstart, bend, cstart, cend;

        astart = aspanptr->vstartaddress.smallmantissa[1];
        aend = astart + aspanptr->sizeofspan;
        bstart = bspanptr->vstartaddress.smallmantissa[1];
        bend = bstart + bspanptr->sizeofspan;
        cstart = (astart > bstart ? astart : bstart);
        cend = (aend < bend ? aend : bend);
        if (cstart >= cend)
                return (FALSE);
        *spanptrptr = (spantype *) taskalloc (taskptr, sizeof(spantype));
        smalltumblerclear (&(*spanptrptr)->vstartaddress);
        (*spanptrptr)->vstartaddress.smallmantissa[0] = 1;
        (*spanptrptr)->vstartaddress.smallmantissa[1] = cstart;
        (*spanptrptr)->sizeofspan = cend - cstart;
        (*spanptrptr)->nextspan = NULL;
        return (TRUE);
}


/*
if *specsetptr is null
	allocate new spec
else
	find last spec in set
	if doc is different
		add new spec to specset
	else
		add new span to last spec
*/
 
buildspec(docidptr,start,len,specsetptr)
  tumbler *docidptr;
  med start,len;
  spectype **specsetptr;
{
  spantype *lspan;
  spectype *specptr;
  spantype *spanptr;

	lspan  = spanalloc();
	smalltumblerclear(&lspan->vstartaddress);
	lspan->vstartaddress.smallmantissa[0] = 1;
	lspan->vstartaddress.smallmantissa[1] = start;
	lspan->sizeofspan = len;
	
	if (*specsetptr == NULL) {
		specptr = *specsetptr = specalloc();
		tumblercopy(docidptr,&specptr->docid);
		specptr->specspanptr = lspan;
		return;
	}
       	for (specptr = *specsetptr; specptr->nextspec; specptr = specptr->nextspec);
	if (!tumblereq(&specptr->docid,docidptr)) {
		specptr->nextspec = specalloc();
		specptr = specptr->nextspec;
		tumblercopy(docidptr,&specptr->docid);
		specptr->specspanptr = lspan;
		return;
	}
       	for (spanptr = specptr->specspanptr; spanptr->nextspan; spanptr = spanptr->nextspan);
	spanptr->nextspan = lspan;
}

  med
ncharsinspec(specptr)
  spectype *specptr;
{
  med n=0;
  spantype *spanptr;

    for (; specptr; specptr = specptr->nextspec) {
        for (spanptr=specptr->specspanptr; spanptr; spanptr=spanptr->nextspan) {
            n += spanptr->sizeofspan;
        }
    }
}

#undef max
#ifndef max
  nat 
max (a,b)
  nat a,b;
{
        return (a > b ? a : b);
}
#endif

  nat
diff (a,b)
  nat a,b;
{
        return (max(a,b) - min(a,b));
}

  void
gerror(message)
        char *message;
{
        fprintf(stderr,"Fe Gerror: %s",message);
#ifdef AMIGA
        AmiCleanup();
#endif

  exitasgracefullyaspossible();
}
