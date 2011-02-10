/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:      select.d
***************************************************/

#include "fest.h"
#include <ctype.h>

#define DOCNAMESIZE 64

  long
makelistfordocumentmenu (docset, listptr)
  tumbleritemtype *docset;
  char ***listptr;
{
  tumbleritemtype *docptr;
  spectype *docspecptr;
  long ndocs;
  char doctext[80];
  char **list;
  char *text;
  nat *ealloc();
 
        for (ndocs = 0,docptr=docset;docptr; docptr = docptr->nexttumbleritem){
                ++ndocs;
	}
        if(ndocs <2){
                return(ndocs); 
        }
        *listptr = list = (char **) ealloc ((ndocs+1) * sizeof (char *));

        docspecptr = specspanalloc ();
        for (docptr = docset; docptr; docptr = docptr->nexttumbleritem) {
                text = (char *)ealloc (DOCNAMESIZE+1);
                *list++ = text;
                movetumbler (&docptr->value, &docspecptr->docid);
                docspecptr->specspanptr->vstartaddress.smallmantissa[0] = 1;
                docspecptr->specspanptr->vstartaddress.smallmantissa[1] = 1;
                docspecptr->specspanptr->sizeofspan = 80; 
                grabspec (docspecptr, doctext);
                copystringwithoutwhitespace (doctext, text, DOCNAMESIZE);
        }
        ffree ((char *)docspecptr); 
        *list = NULL;   /*this terminates list*/
        return (ndocs);
}

  long
makelistforlinkmenu (linkset, listptr, whichendtojumpto)
  tumbleritemtype *linkset;
  char ***listptr;
  med whichendtojumpto;
{
  tumbleritemtype *linkptr;
  spectype *endsetptr;
  long nlinks;
  char doctext[80];
  char **list;
  char *text;
  nat *ealloc();

        for(nlinks=0,linkptr=linkset;linkptr;linkptr=linkptr->nexttumbleritem){
                ++nlinks;
	}
        if(nlinks <2){
                return(nlinks); 
        }
        *listptr = list = (char **) ealloc ((nlinks+1) * sizeof (char *)); 
        for (linkptr = linkset; linkptr; linkptr = linkptr->nexttumbleritem) {
                text = (char *)ealloc (DOCNAMESIZE);
                *list++ = text; 
                findlinkspecset(&linkptr->value, whichendtojumpto, &endsetptr);
                endsetptr->specspanptr->sizeofspan = 80;
                grabspec (endsetptr, doctext);
                ffree ((char *)endsetptr);
                copystringwithoutwhitespace (doctext, text, DOCNAMESIZE);
        }

        *list = NULL;   /* this terminates list*/
        return (nlinks);
}

  void
copystringwithoutwhitespace (src, dst, n)
  char *src;
  char *dst;
  med n;
{
        n--;
        while (*src && n) {
                if (isspace (*src)) {
                        while (isspace (*src))
                                src++; 
                        *dst++ = ' ';
                        *dst = '\0';
                } else {
                        *dst++ = *src++;   
                        *dst = '\0';
                }
                --n;
        }
}

  void
freestringlist (listptr)
  char **listptr;
{
  char **save;

        save = listptr;
        while (*listptr) {
                ffree (*listptr++);
        }
        ffree ((char *)save);
}
