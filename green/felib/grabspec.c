/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1988 Xanadu Operating Company
  XU.88.1 Frontend Source Code:      grabspec.d
***************************************************/

#include "fest.h"
#include "feminreq.h"

 med
grabspec (specptr, textptr)
  spectype *specptr;
  char *textptr;
{
  spectype *localspecset;
  char *localtextptr;
  documenttype *docforspec;
  med textsize;
        if ((!specptr) || (!(specptr->specspanptr)))
		gerror ("No span in grabspec.");

        copyspecset (specptr, &localspecset);
        localtextptr = textptr;
        retrievefromvm (&localspecset, &localtextptr);
        if (localspecset) {
		if (!(docforspec = finddocumentinvmlist(&specptr->docid))) {
			retrievefrombackend (localspecset, localtextptr);
		} else {
			textsize = retrievetextsize(docforspec);
			if (specptr->specspanptr->vstartaddress.smallmantissa[1] <= textsize) {
				retrievefrombackend (localspecset, localtextptr);
			}
		}
                specfree (localspecset);
        }
        return (strlen (textptr));
}
 
  void
grabspecwithouttext (specptr)
  spectype *specptr;
{
  spectype *localspecset;
  med oldlength;

        if ((!specptr) || (!(specptr->specspanptr)))
                gerror ("No span in grabspec.");

        copyspecset (specptr, &localspecset);
        if (localspecset) {
                retrievefrombackend (localspecset,(char *)NULL);
                specfree (localspecset);
        }
        return ;
}
 
  med
retrievetextsize(docptr)
  documenttype *docptr;
{
  spantype *spanset;

	if (docptr->istextsizevalid == FALSE) {
		retrievedocvspanset(&docptr->documentid,&spanset);
		for (;spanset;spanset = spanset->nextspan) {
			if (spanset->vstartaddress.smallmantissa[0] == 1
			  && spanset->vstartaddress.smallmantissa[1] == 1) {
				docptr->textsize = spanset->sizeofspan;
				docptr->istextsizevalid = TRUE;
				break;
			}
		}
	}
	if (docptr->istextsizevalid != TRUE)
		gerror("couldn't find textsize");
	return(docptr->textsize);
}

  bool
retrievedocendsets (taskptr, specptr, fromspecptrptr, tospecptrptr, threespecptrptr)
  tasktype *taskptr;
  spectype *specptr;
  spectype **fromspecptrptr, **tospecptrptr, **threespecptrptr;
{
         retrieveendsetswithinspecsetfrombackend (taskptr, specptr, fromspecptrptr, tospecptrptr, threespecptrptr);
         return (*fromspecptrptr || *tospecptrptr || ((bool)threespecptrptr ? (bool)*threespecptrptr : TRUE ));
}

  void
insertcharactersbeforespecifiedposition (specptr, text, nchars)
  spectype *specptr;
  char *text;
  long nchars;
{
  long start;
  tasktype task;
  smalltumbler vsatoinsertat;
  charspantype charspan;

        charspan.numberofcharactersinspan = nchars;
        charspan.charinspanptr = text;
        charspan.nextcharspan = NULL;
        start = specptr->specspanptr->vstartaddress.smallmantissa[1];
        smalltumblerclear (&vsatoinsertat);
        vsatoinsertat.smallmantissa[0] = 1;
        vsatoinsertat.smallmantissa[1] = start;
        inserttextinbackend(&task,&specptr->docid, &vsatoinsertat, &charspan);
}

  void
deletespecfrombe (specptr)
  spectype *specptr;
{
  tasktype task;
        deletefrombackend (&task, &specptr->docid, specptr->specspanptr);
}
