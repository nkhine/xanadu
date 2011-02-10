/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:       links.d
***************************************************/

#include "fest.h"

/*#define MAKEBOMBPROOF*/

/*tumbler indexdocvposition =     {0,-2, 2,0,1,1,0,0,0,0};*/ /* 0.0.2.0.1.1 */
tumbler indexdocvposition =     {0,0, 1,1,0,1,0,2,0,1,1,0,0,0}; /* 0.0.2.0.1.1 */

/*tumbler linktypedoc       =      {0,-2, 2,0,0,0,0,0,0,0};*/ /* 0.0.2 */
tumbler linktypedoc       =      {0,0, 1,1,0,1,0,2,0,0,0,0,0,0}; /* 0.0.2 */

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

#define NLINKTYPES 4
char *linktypelist[] = {
        "jump",
        "quote",
        "footnote",
        "marginal note"
};


  void
interactlinktype ()
{
  spectype *specspanalloc();
  med linktypenum;
	if(!iswindow){
        linktypenum = selectmenu (currentwindow->tm+currentwindow->rcursl, currentwindow->lm+currentwindow->rcursc, NLINKTYPES, linktypelist);
	maketempthreeset(linktypenum);
	}else{
		makemenu(linktypelist,LINKTYPEMENUCHOICE0,4,"choose Link type");
	}
}

  void
maketempthreeset(linktypenum)
  med linktypenum;
{
        tempthreeset = specspanalloc ();
        movetumbler (&linktypedoc, &tempthreeset->docid);
        movesmalltumbler (linktypeisas[linktypenum], &tempthreeset->specspanptr->vstartaddress);
        tempthreeset->specspanptr->sizeofspan = 1;
}

  static tumbler link;
  void
followlink ()
{
  med whichend;
  med linktype;

	memclear ((char *)&link,sizeof(tumbler));
        findcorrectlink (&link, &whichend);
/* moved to interactlinkset2*/
        if (iszerotumbler (&link)) {
                return;
        }
        findlinktype (&link, &linktype);
        actuallyfollowlink (&link, whichend, linktype);
/**/
}

  void
actuallyfollowlink (linkidptr, whichend, linktype)
  tumbler *linkidptr;
  med whichend;
  med linktype;
{
  tumbler docvsa;
  spectype *endset;

        /*freeupcurrentwindowslinkset ();*/
        switch (linktype) {
          case JUMPLINK:                        
                finddocvsatojumpto (linkidptr, whichend, &docvsa);
                jumptojumplink (&docvsa);
                break;     
          case QUOTELINK:
          case FOOTNOTELINK:
                finddocvsatojumpto (linkidptr, whichend, &docvsa);
		if( whichend == TO){
			jumptofootnotelink (&docvsa);
		}else{
			jumptojumplink (&docvsa);
		}
                break;     
          case MARGINALNOTELINK: 
                finddocvsatojumpto (linkidptr, whichend, &docvsa);
		if( whichend == TO){
			jumptomarginalnotelink (&docvsa);
		}else{
			jumptojumplink (&docvsa);
		}
                break;     
        }
}
  
  void
jumptofootnotelink (docvsaptr)
  tumbler *docvsaptr;
{
        makefootnotewindow (docvsaptr);
        fillwindow/*fromwindowspec*/ (currentwindow, docvsaptr);
}

  void
jumptomarginalnotelink (docvsaptr)
  tumbler *docvsaptr;
{
        makemarginalnotewindow (docvsaptr);
        fillwindow/*fromwindowspec*/ (currentwindow, docvsaptr);
}
   
/*
  void
jumptofootnotelink (endset)
  spectype *endset;
{
        makefootnotewindow (endset);
        fillwindowfromwindowspec (currentwindow);
        specfree (endset);
}
    */
  void
jumptojumplink (docvsaptr)
  tumbler *docvsaptr;
{
        makejumpwindow (docvsaptr);
        fillwindow (currentwindow, docvsaptr);
}

/*
  void
showlinks()
{
}
*/

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

  void
putindexdocumentonstack()
{
        /*freeupcurrentwindowslinkset ();*/
        makejumpwindow (&indexdocvposition);
}
