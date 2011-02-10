/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:    interact.d
***************************************************/

#include "fest.h"
#include "feminreq.h"

med
interact(docvpositionptr)
tumbler *docvpositionptr;
{
	requesttype request;
	void processuserrequest();
	tasktype task;
	/* ? might want to use or set docvpositionptr*/
	if(!docvpositionptr)
		gerror("null pointer in interact");
	loop{

		getuserrequest(&request);
		if(request.requestnumber == EXITNORMALLY){
			return(EXITNORMALLY);
		}
		processuserrequest(&request);
	}
}
/*nat globalrequestptr;*/
void
processuserrequest(requestptr)
requesttype *requestptr;
{
	void createlink();
	void changedocument();
/*
	globalrequestptr = requestptr;
*/

	switch(requestptr->requestnumber){
	case WEIRDOPTIONSCMD:
		weirdoptions();
		break;
	case PARALELTEXTFACE:
	case COMPARE:
		compare(/*requestptr*/);
		break;
	case CURSORPOSITION:
		cursorposition(requestptr);
		break;
	case CREATEFOOTNOTE:
		createfootnote();
		break;
	case XFOLLOWLINK:
		followlink();
		break;
/*
	case WINDOWCONTROL:
                makejumpwindow (&requestptr->requestvposition);
                break;
*/
	case RETURNFROMSTACK:
		returnfromstack(TRUE);
		break;
/*
	case SHOWLINKS:
                showlinks();
                break;
*/
	case DELETECHARACTER:
	case INSERTANDDELETECHAR:
		insertanddeletechar (requestptr);
		break;
	case MAKECUT:
		makecut();
		break;
	case DELETECUTS:
		clearcuts();
		break;
	case MAKELINK2:
		createlink();
		break;
	case XREARRANGE:
		rearrange();
		break;
	case DELETEBLOCK:
		delete();
		break;
	case COPYFUNCTION:
		copy();
		break;
	case CREATEDOCUMENT:
		createdocument();
		break;
	case CREATEVERSION:
		createversion();
		break;
	case CHANGEDOCUMENT:
		changedocument();
		break;
	case CHANGEFROMSET:
		changetempendset (&tempfromset);
/*
if (tempfromset) {
gotoxy (0,screenheight-1);
fprintf (stderr, "fromset   ");
puttumbler (stderr, &tempfromset->docid);
fprintf (stderr, "  ");
putsmalltumbler (stderr, &tempfromset->specspanptr->vstartaddress);
fprintf (stderr, "  %d                 ", tempfromset->specspanptr->sizeofspan);
} else fprintf(stderr, "No from-set");
*/
		break;

	case CHANGETOSET:
		changetempendset (&temptoset);
/*
if (temptoset) {
gotoxy (0,screenheight-1);
fprintf (stderr, "toset     ");
puttumbler (stderr, &temptoset->docid);
fprintf (stderr, "  ");
putsmalltumbler (stderr, &temptoset->specspanptr->vstartaddress);
fprintf (stderr, "  %d                 ", temptoset->specspanptr->sizeofspan);
} else fprintf (stderr, "No to-set");
*/
		break;

	case CLEARENDSETS:
/*
gotoxy (0,screenheight-1);
fprintf (stderr,"clearendsets ");
*/
		cleartempendsets();
		break;
	case REDRAWCMD:
/*
fprintf(stderr,"processing redraw\n");
*/
		paintwindowplain(currentwindow);
		break;
	default:
		dumprequest( requestptr);
		baderror("extremely unfortunate occurence in processuserrequest");
	}
}

void
changefromset()
{
	changetempendset (&tempfromset);
}
void
changetoset()
{
	changetempendset (&temptoset);
}
void
createlink ()
{
	tasktype task;

	if (!(tempfromset && temptoset)) {
		return;
	}
	interactlinktype ();
	/*zzz foo this static link homeset is to kluge an error in createlink i.e. if a link is made and then a version boom*/
	createlinkinbackend (&task,&staticlinkhomeposition/* &currentwindow->windowview.viewspecptr->docid*/, tempfromset,
	    temptoset, tempthreeset);
	updateendsetlist (tempfromset, currentwindow->windowview.viewfromset);
	updateendsetlist (temptoset, currentwindow->windowview.viewtoset);
	currentwindow->isendsetokflag = FALSE;
	cleartempendsets ();
	/*clearcuts ();*/  /* redundant*/
	tfree (&task.tempspace);
	fixglorpfsinwindow (currentwindow);
	wrefresh (currentwindow->w);
}

#define NDOCITEMS 5
char *documentjumpitemlist[] = {
	"next document",
	"previous document",
	"first document",
	"next 10nth document",
	"nth document"
};

void
changedocument()
{
	med selection/*,i*/;
	void changedocument2();
	if(currentwindow->ispartofparalleltextface){
		currentwindow->dependentscreenp = TRUE;
		moveandflipcorrespondenceset(currentwindow);
		currentwindow = currentwindow -> correspondingwindow;
		currentwindow->dependentscreenp = FALSE;
		fixglorpfsinwindow(currentwindow);
		wrefresh (currentwindow->w);
	} else{
		if(iswindow){
			makemenu(documentjumpitemlist,JUMPMENUCHOICE0,5,"jump to");
			return;
		}
		selection = selectmenu (currentwindow->tm+currentwindow->rcursl, currentwindow->lm+currentwindow->rcursc,
		    NDOCITEMS,documentjumpitemlist );
		changedocument2(selection);
	}
}

void
changedocument2(selection)
med selection;
{
	metachar gettypeahead();
	tumbler document;
	spectype *specalloc ();
	copyspecintovposition (&document, currentwindow->windowview.viewspecptr);
	/*        i = findbottomfield (&document);   // change back to use this ?//*/

	switch(selection){
	case 0: /*next*/
		document.mantissa[/*i*/0] += 1;
		break;
	case 1: /* previous*/

		if(document.mantissa[/*i*/1] == 0 &&document.mantissa[/*i*/0] == 1){
			return; /* we were there already */
			/*zzz this may be a bad special case if change document*/
			/*is used to version or something*/
		}/*else*/

		document.mantissa[/*i*/0] -= 1;
		if (document.mantissa[/*i*/0] < 1){
			document.mantissa[/*i*/0] = 1;
		}
		break;
	case 2: /* first*/
		document.mantissa[/*i*/0] = 1;
		document.mantissa[1] = 0;
		document.mantissa[2] = 0;
		document.mantissa[3] = 0;
		document.mantissa[4] = 0;
		break;
	case 3: /*nth */
		document.mantissa[/*i*/0] += 10;
		/*document.mantissa[0] = 1;
		document.mantissa[1] = 0;
		document.mantissa[2] = 0;
		document.mantissa[3] = 0;
		document.mantissa[4] = 0;*/
		break;
	case 4: /*nth */
		fprintf(stderr,"enter number = ");
		scanf("%d",document.mantissa);
		/*document.mantissa[0] = 1;
		document.mantissa[1] = 0;
		document.mantissa[2] = 0;
		document.mantissa[3] = 0;
		document.mantissa[4] = 0;*/
		break;
	}
	/*        specfree (currentwindow->windowview.viewspecptr);  // don't free them now wait till we pop back on stack//
	specfree (currentwindow->windowview.viewmaskspecptr);
	specfree (currentwindow->windowview.viewtoset);
	specfree (currentwindow->windowview.viewfromset);
*/
	jumptojumplink (&document);

	/*        currentwindow->windowview.viewspecptr = specalloc ();
	copyvpositionintospec(&document,currentwindow->windowview.viewspecptr);
	fillwindow (currentwindow, &document);         */


}

void
paralleltextface()
{
	compare(/*globalrequestptr*/);
}
