/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:          vm.d
***************************************************/

/*         Xanadu Front-end Virtual Memory routines        */


#include "fest.h"


#define NEW 0
#define OLD 1

vmthingtype *vmbaseptr = NULL;  /* base of v-stream ordered list */
vmthingtype *grimreaper = NULL; /* pointer to circular, unordered vm list */

  void
dumpvm()
{
  documenttype *ptr;
	gotoxy(1,40);
        for (ptr = (documenttype *)vmbaseptr; ptr; ptr = ptr->nextdocument)
                dumpdocument (ptr);
}

  void
retrievefromvm (specptrptr, textptrptr)
  spectype **specptrptr;
  char **textptrptr;
{
  charspantype *charspanptr, *findcharspaninvm();
 announce("retrievefromvm");

/*
 fprintf(stderr,"retrievefromvm\n");
*/
        if (vmbaseptr && (charspanptr = findcharspaninvm (*specptrptr))) {
                makecharspanintotext (charspanptr, (*specptrptr)->specspanptr, textptrptr);
                if (!(*specptrptr)->specspanptr) {
/*                           if ((*specptrptr)->specspanptr->sizeofspan == screenwidth){
                                gerror ("Blasted span without getting characters.");
                        }
*/                           removevmtypethingfromlist ((vmthingtype*)*specptrptr);
                        specfree (*specptrptr);
                        *specptrptr = NULL;
                }
          }
 leave();
}

  charspantype *
newcharspaninvm (specptr)
  spectype *specptr;
{
  documenttype *docforspec;
  documenttype *finddocumentinvmlist();
  charspantype *charspanptr, *charspanalloc();
  spantype *spanptr;

        if (!(docforspec = finddocumentinvmlist (&specptr->docid))) {
/*
 fprintf(stderr, "not found 1 in newcharspaninvmvm");
*/
                return (NULL);
        }
	charspanptr = charspanalloc(1);
	appendvmtypethingtolist ( (vmthingtype*)docforspec, (vmthingtype*)charspanptr);
return (charspanptr);

}

  charspantype *
findcharspaninvm (specptr)
  spectype *specptr;
{
  documenttype *docforspec;
  documenttype *finddocumentinvmlist();
  charspantype *charspanptr;
  spantype *spanptr;
  long spanvsa, charspanvsa;
/*
 fprintf(stderr,"findcharspaninvm\n");
*/
        if (!(docforspec = finddocumentinvmlist (&specptr->docid))) {
/*
 fprintf(stderr,"no doc: returning NULL\n");
*/
                return (NULL);
        }
        spanptr = specptr->specspanptr;
        spanvsa = spanptr->vstartaddress.smallmantissa[1];

        for (charspanptr=docforspec->doccharspanset; charspanptr; 
	     charspanptr=charspanptr->nextcharspan) {

                charspanvsa = charspanptr->vstartaddress.smallmantissa[1];
                if (charspanvsa <= spanvsa &&
		    charspanvsa+charspanptr->numberofcharactersinspan >= spanvsa) {        /* zzzz */
                        rejuvinate ((vmthingtype *)charspanptr);
                        return (charspanptr);
                }
                if (spanvsa < charspanvsa){
/*
 fprintf(stderr,"missed it: returning NULL\n");
*/
                        return (NULL);			/* missed it */
		}
        }
/*
 fprintf(stderr,"at bottom: returning NULL\n");
*/
        return (NULL);
}

  void
makecharspanintotext (charspanptr, spanptr, textptrptr)
  charspantype *charspanptr;
  spantype *spanptr;
  char **textptrptr;
{
  charspantype *next;
  long offset;
  long  leftinspan;
  char *text, *spantext;

        text = *textptrptr;
        offset = spanptr->vstartaddress.smallmantissa[1] - charspanptr->vstartaddress.smallmantissa[1];
        if (offset < 0) {
                fprintf (stderr,"text before charspan.\n");
                abort();
        } 

        spantext = (char *) charspanptr->charinspanptr + offset;
        leftinspan = charspanptr->numberofcharactersinspan - offset;
        for (next = charspanptr->nextcharspan; spanptr->sizeofspan && (leftinspan || charspansarecontiguous (charspanptr, next));) {
                if (!leftinspan) {
                        charspanptr = next;
                        rejuvinate ((vmthingtype *)charspanptr);
                        next = next->nextcharspan;
                        leftinspan = charspanptr->numberofcharactersinspan;
                        spantext = charspanptr->charinspanptr;
                }
                *text++ = *spantext++;
                --leftinspan;
                ++spanptr->vstartaddress.smallmantissa[1];
                --spanptr->sizeofspan;
        }
        *text = '\0';
       /**/ *textptrptr = text;/**/ /* SKIMP this was comented out on the pc 10/2/87 reg */
        if (!spanptr->sizeofspan) {
                removevmtypethingfromlist ((vmthingtype *)spanptr);
                spanfree (spanptr);
        }
}

  documenttype *
putdocumentinvm (newdocid)
  tumbler *newdocid;
{
  documenttype *ptr, *documentalloc();
  documenttype *rightplace, *findwheretoputdocumentinvmlist();

        ptr = documentalloc ();
        movetumbler (newdocid, &ptr->documentid);
        if (!vmbaseptr) {
                vmbaseptr = (vmthingtype *)ptr;
        } else {
                rightplace = findwheretoputdocumentinvmlist (ptr);
                if (rightplace == (documenttype *)&vmbaseptr) {
                        insertvmtypethinginlist ((vmthingtype *)ptr, (vmthingtype *)vmbaseptr);
                        vmbaseptr = (vmthingtype *)ptr;
                } elif (rightplace)
                        appendvmtypethingtolist ((vmthingtype *)rightplace, (vmthingtype *)ptr);
                else
                        documentfree (ptr);
        }
        putinvmlist ((vmthingtype *)ptr);
        return (ptr);
}

  documenttype *
findwheretoputdocumentinvmlist (document)
  documenttype *document;
{
  documenttype *ptr, *last;
  med cmp;
/* spurious &  not commented out 2-21-1986 zzz kluge reg*/
        for (ptr = (documenttype *)vmbaseptr, last = (documenttype *)/**/&/**/vmbaseptr; ptr; last = ptr, ptr = ptr->nextdocument) {
                cmp = tumblercmp (&document->documentid, &ptr->documentid);
                /* I cheat here by making docid and linkid in the same place */
                if (cmp == EQUAL)
                        return (NULL); /* its already there */
                if (cmp != GREATER)
                        return (last);
        }
        return (last);
}

  void
puttextinvm (specptr, charspanptr)
  spectype *specptr;
  charspantype *charspanptr;
{
  documenttype *docptr, *finddocumenttoputtextin();
  charspantype *vmspanptr, *charspanalloc(), *last, *next, *ptr, *qtr;
  long newvsa, newlen, spanvsa, spanlen;
        docptr = finddocumenttoputtextin (&specptr->docid);
if (!docptr)
gerror ("Null docptr\n");
        newlen = charspanptr->numberofcharactersinspan;
        vmspanptr = charspanalloc ((med)newlen);
        movmem (charspanptr->charinspanptr, vmspanptr->charinspanptr, (unsigned)newlen);
        movesmalltumbler (&specptr->specspanptr->vstartaddress, &vmspanptr->vstartaddress);
        newvsa = vmspanptr->vstartaddress.smallmantissa[1];
        putinvmlist ((vmthingtype *)vmspanptr);
        for (last = (charspantype *)docptr, ptr = docptr->doccharspanset; ptr;
             last = ptr, ptr = next) {
                next = ptr->nextcharspan;
                spanvsa = ptr->vstartaddress.smallmantissa[1];
                spanlen = ptr->numberofcharactersinspan;
                if (spanvsa > newvsa) {
                        insertvmtypethinginlist ((vmthingtype *)vmspanptr, (vmthingtype *)ptr);
                        if (newvsa+newlen > spanvsa) {
                                removevmtypethingfromlist ((vmthingtype*)ptr);
                                removefromvmlist ((vmthingtype *)ptr);
                               /* charspanfree (ptr);         */
                        }
                        return;
                } elif (spanvsa+spanlen > newvsa) {
                        appendvmtypethingtolist ( (vmthingtype*)ptr,  (vmthingtype*)vmspanptr);
                        if (next && (newvsa+newlen > next->vstartaddress.smallmantissa[1])) {
                                qtr = next->nextcharspan;
                                removevmtypethingfromlist ((vmthingtype*)next);
                                removefromvmlist ((vmthingtype *)next);
                                /*charspanfree (next);      */
                                next = qtr;
                        }
                        if ((ptr->numberofcharactersinspan -= spanvsa+spanlen - newvsa) <= 0)
                                removevmtypethingfromlist ((vmthingtype*)ptr);
                        return;
                }
        }
        appendvmtypethingtolist ( (vmthingtype*)last, (vmthingtype*) vmspanptr);
}

  documenttype *
finddocumenttoputtextin (newdocid)
  tumbler *newdocid;
{
  documenttype *ptr, *finddocumentinvmlist();
  documenttype *putdocumentinvm();

        if (!(ptr = finddocumentinvmlist (newdocid)))
                ptr = putdocumentinvm (newdocid);
        return (ptr);
}

  documenttype *
finddocumentinvmlist (docidtofind)
  tumbler *docidtofind;
{
  documenttype *p;

        for (p = (documenttype *)vmbaseptr; p; p = p->nextdocument) {
                if (p->typeofthing == DOCUMENTTYPE) {
                        if (tumblercmp (docidtofind, &p->documentid) == EQUAL)
                                break;
                }
        }
        rejuvinate ((vmthingtype *)p);
        return (p);
}
