/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:      vmreap.d
***************************************************/
#define LIMITED
#include "fest.h"

#define NEW 0
#define OLD 1

extern vmthingtype *vmbaseptr/* = NULL*/;  /* base of v-stream ordered list */
extern vmthingtype *grimreaper /* = NULL */; /* pointer to circular, unordered vm list */

/*
*
*                       Garbage Collection routines
*                         &  other stuff
*
*/

  void
grimlyreapvm ()
{
  bool isreapable();

        statusofalloc("statalloc");
        for (; grimreaper; grimreaper = (vmthingtype *)grimreaper->vmdocument.nextvm) {

                if (isreapable ()) {
                        reap ();
                        return;
                 }
                ++grimreaper->vmdocument.age;
        }
        gerror ("Nothing to reap.\n");
}

  bool
isreapable ()
{
  bool issomeoneschild();
        if(grimreaper->vmdocument.typeofthing == SPANTYPE){
                return(FALSE);
        }
        if(grimreaper->vmdocument.typeofthing == SPECTYPE){
                return(FALSE);
        }
        if(grimreaper->vmdocument.typeofthing == DOCUMENTTYPE && grimreaper->vmdocument.doccharspanset)
                return (FALSE);

        return (grimreaper->vmdocument.age >= OLD/**/ && !issomeoneschild (grimreaper)/**/);
}

  void
reap ()
{
  vmthingtype *ptr;

        ptr = (vmthingtype *)grimreaper->vmdocument.nextvm;
        removevmtypethingfromlist (grimreaper);
        removefromvmlist (grimreaper);

/**/        grimreaper = ptr;    /* */
}

  void
rejuvinate (ptr)
  vmthingtype *ptr;
{
        if (ptr)
                ptr->vmdocument.age = NEW;
}
