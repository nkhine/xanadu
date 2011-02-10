/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:   findlinks.d
***************************************************/

#include "fest.h"

   void
findlinktype (linkidptr, linktypeptr)
  tumbler *linkidptr;
  med *linktypeptr;
{
  spectype *specptr;
  tasktype task;

        retrieveendsetfrombackend (&task, linkidptr, 3, &specptr);
        if (!specptr) {
                *linktypeptr = JUMPLINK;
                tfree (&task.tempspace);
                return;
        }
        /* This is at best temporary */
        if(specptr->specspanptr){
           *linktypeptr = (med ) specptr->specspanptr->vstartaddress.smallmantissa[1]-1;
        }else {
           *linktypeptr=0;
        }
        tfree (&task.tempspace);
}

  void
findlinkspecset (linkidptr, whichend, endsetptr)
  tumbler *linkidptr;
  med whichend;
  spectype **endsetptr;
{
  spectype *specptr;
  tasktype task;

        retrieveendsetfrombackend (&task, linkidptr, whichend, &specptr);
        copyspecset (specptr, endsetptr);
        tfree (&task.tempspace);
}

  void
finddocvsatojumpto (linkidptr, whichend, docvsaptr)
  tumbler *linkidptr, *docvsaptr;
  med whichend;
{
  spectype *specptr;
  tasktype task;
/*fprintf(stderr,"entering findocvsatojumpto\n");*/
        retrieveendsetfrombackend (&task, linkidptr, whichend, &specptr);
        interactendset (specptr, docvsaptr);
        tfree (&task.tempspace);
/*fprintf(stderr,"leaveing findocvsatojumpto\n");*/
}

/* links from if any, otherwise links to else nothing*/

  void
findlinkset (taskptr, linksetptr, whichendtojumpto)
  tasktype *taskptr;
  tumbleritemtype **linksetptr;
  med *whichendtojumpto;
{
  spectype *specptr, *specalloc();
/*
 fprintf(stderr,"findlinkset \n");
*/
        specptr = specalloc();
        findspecfromcursorposition (specptr);
        specptr->specspanptr->sizeofspan = 1;
        /*--*//*(specptr->specspanptr->vstartaddress.smallmantissa[1]);*/
/*
 putsmalltumbler(stderr,&(specptr->specspanptr->vstartaddress));
 fprintf(stderr,"\n");
*/
        retrievelinksetfrombackend (taskptr, specptr, FROM, linksetptr);
        if (*linksetptr) {
                *whichendtojumpto = TO;
        } else {
                retrievelinksetfrombackend (taskptr, specptr, TO, linksetptr);
                *whichendtojumpto = FROM;
                /*if(!*linksetptr)gerror ("Null linkset in findlinkset");this is the no link case */
        }
        specfree (specptr);
/*
 fprintf(stderr,"leaveing findlinkset \n");
*/
}

  void
interactendset (specptr, docvsaptr)
  spectype *specptr;
  tumbler *docvsaptr;
{                        /* for now, take first vspan of first spec */
/*  tumbler vhead, vtail;
*/
if(!specptr){
	gerror("interactendset called with null specptr");
}
        gotoxy(0,59);
        /*fprintf(stderr,"%d",specptr->specspanptr->sizeofspan);*/
        makedocvsa (&specptr->docid,&specptr->specspanptr->vstartaddress, docvsaptr);
}
  void
findnextendsetafterspec (taskptr, specptr, endsetptrptr)
  tasktype *taskptr;
  spectype *specptr;
  spectype **endsetptrptr;
{
  long tempvsa;

        tempvsa = specptr->specspanptr->vstartaddress.smallmantissa[1];
        specptr->specspanptr->vstartaddress.smallmantissa[1] += specptr->specspanptr->sizeofspan;
checkalloc("findnextendsetafterspec1");
        findnextendset (taskptr, specptr, endsetptrptr);
checkalloc("findnextendsetafterspec2");
        specptr->specspanptr->vstartaddress.smallmantissa[1] = tempvsa;
}

/* zzzzzzzzz */
  void
findnextendset (taskptr, specptr, endsetptrptr)
  tasktype *taskptr;
  spectype *specptr;
  spectype **endsetptrptr;
{
  spectype *fromsetptr, *tosetptr;
  long tempsize;

        if (*endsetptrptr) {
                specfree (*endsetptrptr);
        }
        tempsize = specptr->specspanptr->sizeofspan;
        specptr->specspanptr->sizeofspan = BIGVIEW;
        retrieveendsetswithinspecsetfrombackend (taskptr, specptr, &fromsetptr, &tosetptr, NULL);
        specptr->specspanptr->sizeofspan = tempsize;
        findspecspaninfromandtosetswithlowestvsa (taskptr, fromsetptr, tosetptr, endsetptrptr);
}

  void
findspecspaninfromandtosetswithlowestvsa (taskptr, fromsetptr, tosetptr, specptrptr)
  tasktype *taskptr;
  spectype *fromsetptr;
  spectype *tosetptr;
  spectype **specptrptr;
{
  spectype *bestone;
  spantype *spanptr;
  spectype *originaltosetptr;
        originaltosetptr = tosetptr;
        for (bestone = tosetptr; tosetptr; tosetptr = tosetptr->nextspec) {
                if (tumblercmp (&bestone->docid, &tosetptr->docid) != LESS) {
                        bestone = tosetptr;
                        for (spanptr = tosetptr->specspanptr; spanptr; spanptr = spanptr->nextspan) {
                                if (spanvsacmp (bestone->specspanptr, spanptr) == GREATER)
                                        bestone->specspanptr = spanptr;
                        }
                }
        }
checkalloc("findspsp1");
        if (!originaltosetptr)
                bestone = fromsetptr;
        for (; fromsetptr; fromsetptr = fromsetptr->nextspec) {
                if (tumblercmp (&bestone->docid, &fromsetptr->docid) != LESS) {
                        /*bestone = fromsetptr;*/
                        for (spanptr = fromsetptr->specspanptr; spanptr; spanptr = spanptr->nextspan) {
                                if (spanvsacmp (bestone->specspanptr, spanptr) == GREATER)
                                        bestone->specspanptr = spanptr;
                        }
                }
        }
checkalloc("findspsp2");
        bestone->nextspec = NULL;
        bestone->specspanptr->nextspan = NULL;
        copyspecset (bestone, specptrptr);
checkalloc("findspsp3");
if ((!*specptrptr&&bestone)||(!bestone&&*specptrptr))
gerror("unhappy in findspecspaninfromandtosetswithlowestvsa");
}
static  tasktype task;


  void
findcorrectlink (linkidptr, whichendtojumpto)
  tumbler *linkidptr;
  med *whichendtojumpto;
{
  tumbleritemtype *linkset;
/*
 fprintf(stderr,"entering findcorrectlink\n");
*/
        findlinkset (&task, &linkset, whichendtojumpto);

        if(!linkidptr) fprintf(stderr,"Null linkidptr in findcorrectlink\n");

        interactlinkset (linkset, linkidptr, *whichendtojumpto);
/*
 fprintf(stderr,"leaveing findcorrectlink\n");
*/
}
static  tumbleritemtype *linkset;
static  tumbler *linkidptr;
static  med listsize;
static  tumbleritemtype *linkitem;
static  char **linklist;
static  med whichendtojumpto;

  void
interactlinkset (paramlinkset, paramlinkidptr, paramwhichendtojumpto)
  tumbleritemtype *paramlinkset;
  tumbler *paramlinkidptr;
  med paramwhichendtojumpto;
{                          
  med linkindex;
	linkset = paramlinkset;
	linkidptr = paramlinkidptr;
	whichendtojumpto = paramwhichendtojumpto;

        if (!linkset)
                return;

        if(!linkidptr)fprintf(stderr," Null linkidptr in interactlinkset");

        listsize = makelistforlinkmenu (linkset, &linklist, whichendtojumpto);
        if(listsize > 1){
		if(iswindow){
			makemenu(linklist,LINKMENUCHOICE0,listsize,"choose link");
			return;
		}
		linkindex = selectmenu (currentwindow->rcursl, currentwindow->rcursc, listsize, linklist);
		interactlinkset2(linkindex);
        }else{
                linkitem = linkset;
		movetumbler (&linkitem->value, linkidptr);
        }
        tfree (&task.tempspace);
}

  void
interactlinkset2(linkindex)
  med linkindex;
{
  med linktype;
	for (linkitem = linkset; /*--*/ linkindex-- && linkitem; linkitem = linkitem->nexttumbleritem);             

	if (!linkitem){
		fprintf(stderr,"Null linkitem in interactlinkset \n");
	}
        movetumbler (&linkitem->value, linkidptr);
	if(listsize>1 && iswindow){
		tfree (&task.tempspace);
		if (iszerotumbler (linkidptr)) {
			return;
		}
		findlinktype(linkidptr, &linktype);
		actuallyfollowlink (linkidptr, whichendtojumpto, linktype);
	}
}





puttumbler(outfile, tumblerptr)
  FILE *outfile;
  tumbler *tumblerptr;
{
  int i, place;

/*
        if (!tumblercheck (tumblerptr) || tumblerptr->exp < MINEXP) {
                dumptumbler (tumblerptr);
                return;
        }
*/
        if (tumblerptr->sign)
                fprintf(outfile, "-");
        for(i = tumblerptr->exp; i < 0; ++i)
                fprintf(outfile, "0.");
        place = NPLACES;
        do {--place;
        } while(place > 0 && tumblerptr->mantissa[place] == 0);
        for(i = 0; i <= place; ++i) {
                putnum(outfile, tumblerptr->mantissa[i]);
                if (i < place)
                        putc('.', outfile);
        }
}


putsmalltumbler(outfile, tumblerptr)
  FILE *outfile;
  tumbler *tumblerptr;
{
  int i, place;

/*
        if (!tumblercheck (tumblerptr) || tumblerptr->exp < MINEXP) {
                dumptumbler (tumblerptr);
                return;
        }
*/
        if (tumblerptr->sign)
                fprintf(outfile, "-");
        for(i = tumblerptr->exp; i < 0; ++i)
                fprintf(outfile, "0.");
        place = SMALLNPLACES;
        do {--place;
        } while(place > 0 && tumblerptr->mantissa[place] == 0);
        for(i = 0; i <= place; ++i) {
                putnum(outfile, tumblerptr->mantissa[i]);
                if (i < place)
                        putc('.', outfile);
        }
}

putnum(outfile, num)
  FILE *outfile;
  int num;
{
        fprintf(outfile, "%d", num);
}

