/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1988 Xanadu Operating Company
  XU.88.1 Frontend Source Code:   felibtop.d
***************************************************/

#include "fest.h"

/* ----------------------------------------------------------------
   takes a pointer to a pre-allocated uninitialized tumbler 
   places the document id of the new document in it 
  ----------------------------------------------------------------- */
  void
createnewdocument(newdocid)
  tumbler *newdocid;
{
  tasktype task;

	createnewdocumentinbackend(&task,newdocid);
	putdocumentinvm(newdocid);
}


/* ----------------------------------------------------------------
   takes a pointer to a document id, a position in that document,
   and a string. The paramenters 'fromset' and 'toset' point to
   endset specs for the document which may need to be edited.
   Either or both of them may be NULL. 
  ----------------------------------------------------------------- */
  void
inserttext(docidptr,position,text,fromset,toset)
  tumbler *docidptr;
  med position;
  char *text;
  spectype *fromset,*toset;
{
  spectype *localspecptr=NULL;
  med len;

	buildspec(docidptr,position,0,&localspecptr);
	len = strlen(text);
	insertcharactersbeforespecifiedposition (localspecptr,text,len);
	fixvm (localspecptr,text,len);
	fixendsetvm (localspecptr,len,fromset,toset);
	specfree(localspecptr);
}


/* ----------------------------------------------------------------
   takes a pointer to a document id, a position in that document,
   and an number of characters to be deleted. 'fromset' and toset
   are as in 'inserttext' 
  ----------------------------------------------------------------- */
  void
deletetext(docidptr,position,nchars,fromset,toset)
  tumbler *docidptr;
  med position,nchars;
  spectype *fromset,*toset;
{
  spectype *localspecptr;

	buildspec(docidptr,position,nchars,&localspecptr);
	deletespecfrombe (localspecptr);
	fixvm (localspecptr,(char *)NULL,-nchars);
	fixendsetvm (localspecptr,-nchars,fromset,toset);
	specfree(localspecptr);
}


/* ----------------------------------------------------------------
   takes a pointer to a document id, a number of cuts 'ncuts' and                 
   four positions in the document 'c1,c2,c3,c4' and performs the
   appropriate rearrange. Ncuts may be 2, 3 or 4. Parameters c(n)
   are ignored where n > ncuts.  'fromset' and toset are as in
   'inserttext'                                     
  ----------------------------------------------------------------- */
  void
rearrangetext(docidptr,ncuts,c1,c2,c3,c4,fromset,toset)
  tumbler *docidptr;
  med ncuts,c1,c2,c3,c4;
  spectype *fromset,*toset;
{
  cutseqtype cuts;
  tasktype task;
  med i;

	if (ncuts < 2 || ncuts > 4) {
		return;
	}
        cuts.numberofcuts = ncuts;
	for (i = 0; i<MAXCUTS; i++) { 
		smalltumblerclear(&cuts.cutsarray[i]);
		cuts.cutsarray[i].smallmantissa[0] = 1;
	}
	cuts.cutsarray[0].smallmantissa[1] = c1;
	cuts.cutsarray[1].smallmantissa[1] = c2;
	cuts.cutsarray[2].smallmantissa[1] = c3;
	cuts.cutsarray[3].smallmantissa[1] = c4;

	rearrangeinbackend (&task, docidptr, &cuts);
        fixvmforrearrange (docidptr,fromset,toset,&cuts);
	tfree(&task.tempspace);
}


/* ----------------------------------------------------------------
   takes a pointer to the specset for the text to be retieved,
   places a pointer to the text found in *stringptr and RETURNS
   THE NUMBER OF CHARACTERS FOUND.

   ** This routine allocates storage for *stringptr which should **
   ** be freed with 'free' when no longer needed.                **
  ----------------------------------------------------------------- */
  med
findtextinspec(specptr,stringptr)
  spectype *specptr;
  char **stringptr;
{
  unsigned len;
  char *p,*realloc();

	p = malloc(ncharsinspec(specptr)+1);
	len = (unsigned)grabspec(specptr,p);
	p = realloc(p,len+1);
	*stringptr = p;
}
  

/* ----------------------------------------------------------------
   takes a pointer to a specset defining the text to be searched
   for endsets, and places pointers to the specsets for the
   from- and to- sets found in *fromsetptr and *tosetptr 

   ** This routine allocates storage for the from- and to- sets  **
   ** which should be freed with 'specfree' (in freealloc.d)     **
   ** when no longer needed.                                     **
  ----------------------------------------------------------------- */
  void
findendsetsinspec(specptr,fromsetptr,tosetptr)
  spectype *specptr,**fromsetptr,**tosetptr;
{
  tasktype task;
  spectype *tspec1,*tspec2,*tspec3;
	if(retrievedocendsets (&task,specptr,&tspec1,&tspec2,&tspec3)) {
		copyspecset (tspec1,fromsetptr);
		copyspecset (tspec2,tosetptr);
	}
	tfree(&task.tempspace);
}


/* ----------------------------------------------------------------
   takes a pointer to a specset defining the text to be searched 
   for links, and places a pointer to the a list of the links found
   in *linksetptr.

   ** This routine allocates storage for the linkset which       **
   ** should be freed with 'tumbleritemfree' (in freealloc.d)    **
   ** when no longer needed.                                     **
  ----------------------------------------------------------------- */
  void
findlinks(specptr,whichend,linksetptr)
  spectype *specptr;
  tumbleritemtype **linksetptr;
{
  tasktype task;
  tumbleritemtype *tlinkset;
	void retrievelinksetfrombackend();

	retrievelinksetfrombackend(&task,specptr,whichend,&tlinkset);
	copytumbleritemset(tlinkset,linksetptr);
	tfree(&task.tempspace);
}


/* ----------------------------------------------------------------
   takes a pointer to a link id and FROM, TO, or THREE, and places
   a pointer to the endset in *specsetptr.

   ** This routine allocates storage for the specset which       **
   ** should be freed with 'specfree' (in freealloc.d) when no   **
   **  longer needed.                                            **
  ----------------------------------------------------------------- */
  void
findlinkendset(linkidptr,whichend,specsetptr)
  tumbler *linkidptr;
  med whichend;
  spectype **specsetptr;
{
  tasktype task;
  spectype *tspecset;

	retrieveendsetfrombackend (&task,linkidptr,whichend,&tspecset);
	copyspecset (tspecset,specsetptr);
	tfree(&task.tempspace);
}


/* ----------------------------------------------------------------
   takes a pointer to the home document for the link, pointers to
   from- and to- specsets, and the link type
   (JUMPLINK, QUOTELINK, FOOTNOTELINK or MARGINALNOTELINK)
   places the link id of the link made in *linkptr.
  ----------------------------------------------------------------- */
  void
makenewlink(homedocptr,fromset,toset,type,linkptr)
  tumbler *homedocptr;
  spectype *fromset,*toset;
  med type;
  tumbler *linkptr;
{
  spectype *threeset;
  tasktype task;

	maketempthreeset(type,&threeset);
	createlinkinbackend (&task,homedocptr,fromset,toset,threeset,linkptr);
	specfree(threeset);
	tfree(&task.tempspace);
}


/* ----------------------------------------------------------------
   takes a pointer to a document id 'olddocptr' and places a 
   pointer to the new document in 'newdocptr'
  ----------------------------------------------------------------- */
  void
makenewversion(olddocptr,newdocptr)
  tumbler *olddocptr,*newdocptr;
{
  tasktype task;
	createnewversioninbackend (&task,olddocptr,newdocptr);
}


/* ----------------------------------------------------------------
   takes a pointer to a specset and places a pointer to a list of
   all documents containing anything that is in that specset in
   *docsetptr. 

  ** This routine allocates storage for the docset which should  **
  ** be freed with 'tumbleritemfree' (in freealloc.d) when no    **
  ** longer needed.                                              **
  ----------------------------------------------------------------- */
  void
finddocumentsincluding(specptr,docsetptr)
  spectype *specptr;
  tumbleritemtype **docsetptr;
{
  tasktype task;
  spectype *localspecptr;
  tumbleritemtype *tdocset;

	finddocscontaininginbackend (&task,specptr,&tdocset);
	copytumbleritemset(tdocset,docsetptr);
	tfree(&task.tempspace);
}


/* ----------------------------------------------------------------
   takes pointers to two specsets and places a pointer to a list
   of corresponding portions in *corrsetptr.

   ** This routine allocates storage for the correspondence set  **
   ** which should be freed with 'freecorrespondencelist'        **
   ** (in freealloc.d) when no longer needed.                    **
  ----------------------------------------------------------------- */
  void
showcorrespondence (spec1ptr,spec2ptr,corrsetptr)
  spectype *spec1ptr,*spec2ptr; 
  correspondencetype **corrsetptr;
{
  tasktype task;
	showrelationof2versionsinbackend (&task,spec1ptr,spec2ptr,corrsetptr);
}
