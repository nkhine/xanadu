/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:        mask.d
***************************************************/

#include "fest.h"

#define ESC 0x1b

#define NORMAL        0
#define BLINK         1
#define REVERSEVIDEO  2
#define HIGHINTENSITY 4
#define UNDERLINE     8

extern med oldmask;

  void
displayithlineinwindowwithmask (window, line, text, displaymask)
  windowtype *window;
  med line;
  char *text;
  masktype displaymask[];   /* normally called with an array of masks*/
{
  med i;
#ifdef COMMENT
  char *text2;

        if(!displaymask){
                plotntextcharactersinwindow(window,text,strlen(text),line, 0);
        }else{
             text2 = text;
             for (i = 0; i < window->windowwidth; i++) {
                if (!*text2 || *text2++== '\n'){
                        break;
                }
             }
	     cleartoendoflineinwindow((long)i,window,(long)line);
             for (i = 0; i < window->windowwidth; i++) {
                if (!*text || *text == '\n'){
                        break;
                }
		/*
                if(!(iswindow && (displaymask[i].displayattribute&UNDERLINE))) 
		*/
		if (!iswindow)
                    if(!(oldmask == displaymask[i].displayattribute)){
                       changedisplayattributes(displaymask[i].displayattribute);
                    }
                if (displaymask[i].numberwiththisattribute == 0){
                        cleartoendoflineinwindow((long)i,window,(long)line);
                        return;
                }
		mvwaddch(window->w,line,i,*text++);
            }
#endif
	if (!displaymask) {
		plotntextcharactersinwindow(window,text,strlen(text),line, 0);
        	changedisplayattributes (NORMAL);
		return;
	}

	for (i=0; i < window->windowwidth; i++) {
		if ( *(text+i)==NULL  || *(text+i)=='\n')
			break;
		if (oldmask != displaymask[i].displayattribute)
			changedisplayattributes(displaymask[i].displayattribute);
		mvwaddch(window->w,line,i,*(text+i));
	}
	if (i==0) {
		wmove(window->w,line,0);
	}
	wclrtoeol(window->w);
	changedisplayattributes (NORMAL);
}

 med 
speccmp (speca, specb)
  spectype *speca;
  spectype *specb;
{
  med ret;
  long avsa, bvsa;
  long alen, blen;

        if (!speca || !specb)
                return (TOMYLEFT);
        if ((ret = tumblercmp (&speca->docid, &specb->docid)) != EQUAL)
                return (ret*2);
        else {
                avsa = speca->specspanptr->vstartaddress.smallmantissa[1];
                alen = speca->specspanptr->sizeofspan;
                bvsa = specb->specspanptr->vstartaddress.smallmantissa[1];
                blen = specb->specspanptr->sizeofspan;
                if (avsa+alen < bvsa)
                        return (TOMYRIGHT);
                if (avsa+alen == bvsa)
                        return (ONMYRIGHTBORDER);
                if (bvsa+blen == avsa)
                        return (ONMYLEFTBORDER);
                if (bvsa+blen < avsa)
                        return (TOMYLEFT);
                return (THRUME);
        }
}

  void
fillupmask (window, linenumber, specptr, displaymask)
  windowtype *window;
  med linenumber;
  spectype *specptr;
  masktype displaymask[];
{
  tasktype task;
  spectype *fromspecptr, *tospecptr, *cutspecptr,*tempspec;
  bool  retrievedocendsets(), findendsetsinview();
  med temp;

        inittask (&task, stdin, stdout);
        clearmask (displaymask);
      if(currentwindow->dependentscreenp&&(currentwindow->rcursl ==linenumber)){ /* display cursor*/
                tempspec = specalloc();
/*fprintf(stderr,"xxz");*/
                temp = findspecfromcursorposition(tempspec);
                putspansonlineintomask(specptr,tempspec,displaymask,REVERSEVIDEO|BLINK);
                specfree(tempspec);     
        }else if (currentwindow->dependentscreenp){
                /*fprintf(stderr,"rcl=%dlf=%d",currentwindow ->rcursl,linenumber);*/
        }
        if (
           (!window->isendsetokflag)
        || !window->windowview.viewmaskspecptr
        || (speccmp (specptr, window->windowview.viewmaskspecptr) != THRUME)) {

                if (window->windowview.viewmaskspecptr) {
                        specfree (window->windowview.viewmaskspecptr);
                        window->windowview.viewmaskspecptr = NULL;
                }
                copyspecset (specptr, &window->windowview.viewmaskspecptr);
                if (window->typeofwindow == JUMPLINK){
/*zzz LOOK HERE !!! */      window->windowview.viewmaskspecptr->specspanptr->sizeofspan = BIGVIEW;
/*zzz LOOK HERE !!! */      window->windowview.viewmaskspecptr->specspanptr->vstartaddress.smallmantissa[1] = 1;
        }
                if (!retrievedocendsets (&task, window->windowview.viewmaskspecptr, &fromspecptr, &tospecptr, (spectype **)NULL)) {
                        tfree (&task.tempspace);
                        window->isendsetokflag = TRUE;
                        return;
                }
                putendsetsinview (&task, fromspecptr, tospecptr);
        }
        if(findendsetsinview(&task,specptr,&fromspecptr,&tospecptr,&cutspecptr)) {
                putspansonlineintomask(specptr,fromspecptr,displaymask,REVERSEVIDEO);
                putspansonlineintomask(specptr,tospecptr,displaymask,UNDERLINE);
                putspansonlineintomask(specptr, cutspecptr, displaymask, BLINK);
        }

        tfree (&task.tempspace);
        window->isendsetokflag = TRUE;
}

  void
putspansonlineintomask (docspecptr, linkspecptr, displaymask, color)
  spectype *docspecptr, *linkspecptr;
  masktype displaymask[];
  med color;
{
  long displayvsa;
  long endpointvsa, endendvsa;
  spantype *linkspan;
  bool first;

        first = TRUE;
        displayvsa = docspecptr->specspanptr->vstartaddress.smallmantissa[1];
        for (; linkspecptr; linkspecptr = linkspecptr->nextspec) {
                for (linkspan = linkspecptr->specspanptr; linkspan; linkspan = linkspan->nextspan) {

/*
 fprintf(stderr,"  ");
putsmalltumbler(stdout,&linkspan->vstartaddress);
 fprintf(stderr,"  %d",linkspan->sizeofspan);
*/

                        if (linkspan->sizeofspan < 0)
                                gerror ("negative link span size");
                        endpointvsa = linkspan->vstartaddress.smallmantissa[1];
                        endendvsa = endpointvsa + linkspan->sizeofspan;
                        if (first && displayvsa < endpointvsa)
                                maskset (displaymask, (long)0, (long)(endpointvsa - displayvsa), NORMAL);
                        if (displayvsa > endpointvsa){
                                maskset (displaymask, (long)0, linkspan->sizeofspan/* displayvsa-endendvsa*/ , color);/*kluge 11/24/84 zzz */
                        }else{
                                maskset (displaymask, (long)(endpointvsa - displayvsa), linkspan->sizeofspan, color);
                        }
                        first = FALSE;
                }
        }
}

  bool
retrievedocendsets (taskptr, specptr, fromspecptrptr, tospecptrptr, threespecptrptr)
  tasktype *taskptr;
  spectype *specptr;
  spectype **fromspecptrptr, **tospecptrptr, **threespecptrptr;
{
         retrieveendsetswithinspecsetfrombackend (taskptr, specptr, fromspecptrptr, tospecptrptr, threespecptrptr);
         return (*fromspecptrptr || *tospecptrptr || ((bool)threespecptrptr ? (bool)*threespecptrptr : TRUE /*FALSE??zzzz*/ ));
}

  bool
findendsetsinview (taskptr, specptr, fromspecptrptr, tospecptrptr, cutspecptrptr)
  tasktype *taskptr;
  spectype *specptr;
  spectype **fromspecptrptr, **tospecptrptr, **cutspecptrptr;
{

        findcomonspecset (taskptr, specptr, currentwindow->windowview.viewfromset, fromspecptrptr);
        findcomonspecset (taskptr, specptr, currentwindow->windowview.viewtoset, tospecptrptr);
        findcomonspecset (taskptr, specptr, currentwindow->windowview.viewcutset, cutspecptrptr);
        return ((*fromspecptrptr) || (*tospecptrptr) || (*cutspecptrptr));
}

  void
findcomonspecset (taskptr, specptr, specset, specptrptr)
  tasktype *taskptr;
  spectype *specptr, *specset;
  spectype **specptrptr;
{
  spectype *tmp, *localspecptr;
  bool findcommonspans();

        *specptrptr = NULL;
        for (tmp = specset; tmp; tmp = tmp->nextspec) {
                if (!tumblereq (&specptr->docid, &tmp->docid))
                        break;
                localspecptr = (spectype*) taskalloc (taskptr, sizeof(spectype));
                movetumbler (&specptr->docid, &localspecptr->docid);
                localspecptr->nextspec = NULL;
                if (!findcommonspans (taskptr, specptr->specspanptr, tmp->specspanptr, &localspecptr->specspanptr))
                        continue;
                *specptrptr = localspecptr;
                specptrptr = &localspecptr->nextspec;
        }
}

  bool
findcommonspans (taskptr, spanptr, spanset, spanptrptr)
  tasktype *taskptr;
  spantype *spanptr, *spanset;
  spantype **spanptrptr;
{
  spantype *tmp, *localspanptr;
  bool ret;
  bool intersectspans();

        ret = FALSE;
        for (tmp = spanset; tmp; tmp = tmp->nextspan) {
                if (intersectspans (taskptr, tmp, spanptr, &localspanptr)) {
                        *spanptrptr = localspanptr;
                        spanptrptr = &localspanptr->nextspan;
                        ret = TRUE;
                }
        }
        return (ret);
}

  bool
intersectspans (taskptr, aspanptr, bspanptr, spanptrptr)
  tasktype *taskptr;
  spantype *aspanptr, *bspanptr;
  spantype **spanptrptr;
{
  long astart, aend, bstart, bend, cstart, cend;

        astart = aspanptr->vstartaddress.smallmantissa[1];
        aend = astart + aspanptr->sizeofspan;
        bstart = bspanptr->vstartaddress.smallmantissa[1];
        bend = bstart + bspanptr->sizeofspan;
        cstart = (astart > bstart ? astart : bstart);
        cend = (aend < bend ? aend : bend);
        if (cstart >= cend)
                return (FALSE);
        *spanptrptr = (spantype *) taskalloc (taskptr, sizeof(spantype));
        smalltumblerclear (&(*spanptrptr)->vstartaddress);
        (*spanptrptr)->vstartaddress.smallmantissa[0] = 1;
        (*spanptrptr)->vstartaddress.smallmantissa[1] = cstart;
        (*spanptrptr)->sizeofspan = cend - cstart;
        (*spanptrptr)->nextspan = NULL;
        return (TRUE);
}

  void
putendsetsinview (taskptr, fromspecptr, tospecptr)
  tasktype *taskptr;
  spectype *fromspecptr, *tospecptr;
{
        if (currentwindow->windowview.viewfromset) {
                specfree (currentwindow->windowview.viewfromset);
                currentwindow->windowview.viewfromset = NULL;
        }
        if (currentwindow->windowview.viewtoset) {
                specfree (currentwindow->windowview.viewtoset);
                currentwindow->windowview.viewtoset = NULL;
        }
        copyspecset (fromspecptr, &currentwindow->windowview.viewfromset);
        copyspecset (tospecptr, &currentwindow->windowview.viewtoset);
}

  void
clearmask (displaymask)
  masktype displaymask[];
{
  med i;

        for (i = 0; i < currentwindow->windowwidth+1; i++) {
                displaymask[i].numberwiththisattribute = 1;
                displaymask[i].displayattribute = NORMAL;
        }
}

  void
maskset (displaymask, position, count, color)
  masktype displaymask[];
  long position;
  long count;
  med color;
{
/*fprintf(stderr,"      maskset %d %d %d",position,count,color);*/

        if (position < 0 || count < 0) {
                fprintf(stderr,"position = %d\ncount = %d\n",position,count);
                gerror ("maskset called with absurd values.");
        }
if (count == 0){
        count = 1;
}
        while (count-- && position < currentwindow->windowwidth+1) {
                displaymask[position].displayattribute |= color;
                displaymask[position++].numberwiththisattribute = 1;
        }
}
