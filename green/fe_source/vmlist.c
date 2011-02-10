/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:      vmlist.d
***************************************************/

#include "fest.h"


#define NEW 0
#define OLD 1

extern vmthingtype *vmbaseptr/* = NULL*/;  /* base of v-stream ordered list */
extern vmthingtype *grimreaper /* = NULL */; /* pointer to circular, unordered vm list */
/*
*
*                  Special purpose vm list handling routines
*
*/


  void
removefromvmlist (vmthing)
  vmthingtype *vmthing;
{
  vmthingtype *last, *next;
  void dumpreaper();

/*
 fprintf(stderr,"removefromvmlist\n");
 dumpreaper();
*/
        validallocthinge((char *)vmthing);

        if (next = (vmthingtype *)vmthing->vmdocument.nextvm)
                next->vmdocument.prevvm = vmthing->vmdocument.prevvm;
        if (last = (vmthingtype *)vmthing->vmdocument.prevvm)
                last->vmdocument.nextvm = (char *)next;
        vmfree(vmthing);
        if (grimreaper == vmthing) {

                grimreaper = next;
    }

        if (next == vmthing && last == vmthing ){
                grimreaper = NULL;
     fprintf(stderr,"GRIMREAPER NULL.\n");
    }
checkalloc("");

}

  void
dumpreaper ()
{
  vmthingtype *ptr;
  bool flag;

        for (ptr = grimreaper, flag = FALSE; ptr && (!flag || ((vmthingtype *)ptr->vmdocument.nextvm) != grimreaper); ptr = (vmthingtype *)ptr->vmdocument.nextvm) {
                dumpvmthing (ptr);
                flag = TRUE;
        }
}

  void
vmfree(vmthing)
  vmthingtype *vmthing;
{
#ifdef UNDEFINED
/*
       if(vmthing == vmbaseptr){
                if (vmthing->typeofthing == DOCUMENTTYPE) {
                        vmbaseptr = vmthing->nextdocument/*zzz or next spec*/;
                        if (vmbaseptr)
                                vmbaseptr->prevdocument = NULL;
                        else {
                                dumpreaper ();
                                fprintf (stderr, "Document id = ");
                                dumptumbler (&vmthing->docid);
                                fprintf (stderr,"\n");
                                if (vmthing->doccharspanset)
                                        fprintf (stderr, "This document that is getting reaped still has charspans.\n");
                                fprintf (stderr, "Why is the only document header in VM getting freed?\n");
                                abort ();
                        }
                } else {
                        fprintf (stderr, "This code doesn't know what to do if the first thing in VM isn't of type DOCUMENTTYPE.\n");
                        abort ();
                }
        }
*/
#endif UNDEFINED
	switch(vmthing->vmdocument.typeofthing){

         case CHARSPANTYPE:
                charspanfree((charspantype *)vmthing);
                break;

         case DOCUMENTTYPE:
                documentfree((documenttype *)vmthing);
                break;

         case     SPANTYPE:
                spanfree((spantype *)vmthing);
                break;

         case     SPECTYPE:
                specfree((spectype *)vmthing);
                break;

         case DOCRANGETYPE:

         case LINKRESTRICTIONTYPE:

         case LINKGROUPTYPE:

         case LINKCONTENTTYPE:

         default:
                ffree((char *)vmthing);
        }


}

 void
putinvmlist (vmthing)
  vmthingtype *vmthing;
{
        if (!grimreaper) {
                grimreaper = vmthing;
                vmthing->vmdocument.prevvm = vmthing->vmdocument.nextvm = (char *)vmthing;

        } else {
                vmthing->vmdocument.nextvm = (char *)grimreaper;
                vmthing->vmdocument.prevvm = grimreaper->vmdocument.prevvm;
                ((vmthingtype *)(grimreaper->vmdocument.prevvm))->vmdocument.nextvm = (char *)vmthing;
                grimreaper->vmdocument.prevvm = (char *)vmthing;
        }
        rejuvinate (vmthing);
}

/*
                Some Span Comparison Stuff
*/

  bool
charspansarecontiguous (aptr, bptr)
  charspantype *aptr, *bptr;
{
        if (!aptr || !bptr || aptr->typeofthing != CHARSPANTYPE || bptr->typeofthing != CHARSPANTYPE)
                return (FALSE);
        return (aptr->vstartaddress.smallmantissa[1] + aptr->numberofcharactersinspan == bptr->vstartaddress.smallmantissa[1]);
}

/*
  bool
spansarecontiguous (aptr, bptr)
  spantype *aptr, *bptr;
{
}
*/
