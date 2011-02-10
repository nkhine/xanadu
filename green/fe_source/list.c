/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:        list.d
***************************************************/

#include "fest.h"

extern vmthingtype *vmbaseptr;

  vmthingtype *
nthiteminlist (n, list)
  med n;
  vmthingtype *list;
{
        while (--n) {
                list = (vmthingtype *)list->vmspec.nextspec;
                if (!list)
                        gerror ("n > list size\n");
        }
        return (list);
}

  void
copyspecset (specptr, newptrptr)
  spectype *specptr, **newptrptr;
{
  spectype *last, *this;
  spectype *specalloc();

        if(specptr == NULL)
                return;
        *newptrptr = NULL;
        last = NULL;
        for (; specptr; specptr = specptr->nextspec) {
                this = specalloc ();
                movetumbler (&specptr->docid, &this->docid);
                copyspanset (specptr->specspanptr, &this->specspanptr);
                if (this->specspanptr)
                        this->specspanptr->prevspan = (spantype *)this;
                this->prevspec = last;
                last = this;
                *newptrptr = this;
                newptrptr = &this->nextspec;
        }
}

  void	
copyspanset (spanptr, newptrptr)
  spantype *spanptr, **newptrptr;
{
  spantype *last, *this;
  spantype *spanalloc();

        *newptrptr = NULL;
        last = NULL;
        for (; spanptr; spanptr = spanptr->nextspan) {
                this = spanalloc ();
                movesmalltumbler(&spanptr->vstartaddress,&this->vstartaddress);
                this->sizeofspan = spanptr->sizeofspan;
                this->mask = spanptr->mask;
                this->prevspan = last;
                last = this;
                *newptrptr = this;
                newptrptr = &this->nextspan;
        }
}

  bool
issomeoneschild (vmthing)
  vmthingtype *vmthing;
{
  vmthingtype *last;

        last = (vmthingtype *)vmthing->vmcharspan.prevcharspan;

        switch (vmthing->vmdocument.typeofthing) {

          case CHARSPANTYPE :
          case DOCUMENTTYPE :
                return (FALSE); /* I don't think it matters with charspans */

          case SPECTYPE :
          case SPANTYPE :
                return ((bool)(last != NULL));
        }
        return(FALSE);
}

  void
removevmtypethingfromlist (vmthing)
  vmthingtype *vmthing;
{
  vmthingtype *last, *next;

        last = (vmthingtype *)vmthing->vmcharspan.prevcharspan; /* this works for all working types */
        next = (vmthingtype *)vmthing->vmcharspan.nextcharspan;

        if (vmthing == vmbaseptr)
                vmbaseptr = next;

        switch (vmthing->vmdocument.typeofthing) {

          case CHARSPANTYPE :
                if (next)
                        next->vmcharspan.prevcharspan = (charspantype *)last;
                if (!last)
                        break;
                elif (last->vmdocument.typeofthing == DOCUMENTTYPE)
                        last->vmdocument.doccharspanset = (charspantype *)next;
                else
                        last->vmcharspan.nextcharspan = (charspantype *)next;
                break;

          case DOCUMENTTYPE :
/*
printf ("removing document ");
 dumptumbler (&vmthing->docid);
 fprintf (stderr,"from vm list\n");
*/
          case SPECTYPE:
                if (next)
                        next->vmdocument.prevdocument = (documenttype *)last;
                if (last)
                        last->vmdocument.nextdocument = (documenttype *)next;
                break;

          case SPANTYPE:
                if (next)
                        next->vmspan.prevspan = (spantype *)last;
                if (!last)
                        break;
                elif (last->vmdocument.typeofthing == SPECTYPE)
                        last->vmspec.specspanptr = (spantype *)next;
                else
                        last->vmspan.nextspan = (spantype *)next;
        }

        vmthing->vmcharspan.nextcharspan = NULL;
        vmthing->vmcharspan.prevcharspan = NULL;
/*checkalloc("");*/
}

  void
insertvmtypethinginlist (vmthing, position)
  vmthingtype *vmthing, *position;
{
  vmthingtype *last;

        switch (vmthing->vmdocument.typeofthing) {

          case CHARSPANTYPE:
                if ((last = (vmthingtype *)position->vmcharspan.prevcharspan)->vmdocument.typeofthing == DOCUMENTTYPE){
                        last->vmdocument.doccharspanset = (charspantype *)vmthing;
                }else{
                        last->vmcharspan.nextcharspan = (charspantype *)vmthing;
		}
                vmthing->vmcharspan.prevcharspan = (charspantype *)last;
                vmthing->vmcharspan.nextcharspan = (charspantype *)position;
                position->vmcharspan.prevcharspan = (charspantype *)vmthing;
                break;

          case DOCUMENTTYPE:
          case SPECTYPE:
                vmthing->vmdocument.nextdocument = (documenttype *)position;
                vmthing->vmdocument.prevdocument = position->vmdocument.prevdocument;
		last = (vmthingtype *)position->vmdocument.prevdocument;
                position->vmdocument.prevdocument = (documenttype *)vmthing;
                if (last){
                        last->vmdocument.nextdocument = (documenttype *)vmthing;
		}
                break;

          case SPANTYPE:
                if ((last = (vmthingtype *)position->vmspan.prevspan)->vmdocument.typeofthing == SPECTYPE){
                        last->vmspec.specspanptr = (spantype *)vmthing;
                }else{
                        last->vmspan.nextspan = (spantype *)vmthing;
		}
                vmthing->vmspan.prevspan = (spantype *)last;
                vmthing->vmspan.nextspan = (spantype *)position;
        }
}

  void
appendvmtypethingtolist (position, vmthing)
  vmthingtype *position, *vmthing;
{
  vmthingtype *next;

        switch (vmthing->vmdocument.typeofthing) {

          case CHARSPANTYPE :
                if (position->vmdocument.typeofthing == DOCUMENTTYPE) {
                        next = (vmthingtype *)position->vmdocument.doccharspanset;
                        position->vmdocument.doccharspanset = (charspantype *)vmthing;
                } else {
                        next = (vmthingtype *)position->vmcharspan.nextcharspan;
                        position->vmcharspan.nextcharspan = (charspantype *)vmthing;
                }
                if (next)
                        next->vmcharspan.prevcharspan = (charspantype *)vmthing;
                vmthing->vmcharspan.nextcharspan = (charspantype *)next;
                vmthing->vmcharspan.prevcharspan = (charspantype *)position;
                break;

          case DOCUMENTTYPE:
          case SPECTYPE:
                next = (vmthingtype *)position->vmdocument.nextdocument;
                position->vmdocument.nextdocument = (documenttype *)vmthing;
                vmthing->vmdocument.nextdocument = (documenttype *)next;
                vmthing->vmdocument.prevdocument = (documenttype *)position;
                if (next)
                        next->vmdocument.prevdocument = (documenttype *)vmthing;
                break;

          case SPANTYPE:
                if (position->vmdocument.typeofthing == SPECTYPE) {
                        next = (vmthingtype *)position->vmspec.specspanptr;
                        position->vmspec.specspanptr = (spantype *)vmthing;
                } else {
                        next = (vmthingtype *)position->vmspan.nextspan;
                        position->vmspan.nextspan = (spantype *)vmthing;
                }
                if (next)
                        next->vmspan.prevspan = (spantype *)vmthing;
                vmthing->vmspan.prevspan = (spantype *)position;
                vmthing->vmspan.nextspan = (spantype *)next;
        }
}
