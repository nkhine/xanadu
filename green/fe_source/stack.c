/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:       stack.d
***************************************************/

#include "fest.h"

#define DOCNAMESIZE 64

  med findlastplace();
static med flag;
static  tumbler docvsa;

  void
returnfromstack(paraflag)
  med paraflag;
{
  med count;            
char **list;
  med nitems, temp;
  med i;

	flag = paraflag;

        if (!currentwindow->ispartofparalleltextface &&currentwindow == &theoneandonlyscreen.basewindow)
                return;
        if (currentwindow->ispartofparalleltextface &&currentwindow == &theoneandonlyscreen.basewindow){
	        changecurrentwindowdimensions(0,0,screenheight,screenwidth);
                currentwindow->ispartofparalleltextface = FALSE;
		currentwindow = currentwindow->correspondingwindow;
		returnfromstack2(1);
                return;
	}
	if (currentwindow->ispartofparalleltextface && firstofparalleltextface()){ /* switch to dependent screen and recurse */
		currentwindow = currentwindow->correspondingwindow;
		returnfromstack(paraflag);
		return;
	}

        tumblerclear(&docvsa);
        count = 0;
        if(flag){ 
       		list = (char **) ealloc ((ndocsforstack()+1) * sizeof (char *)); 
		nitems = makelistforstackmenu(list);
		if(iswindow){
			makemenu(list,STACKRETURNCHOICE0,nitems,"choose document");
			return;
		}else{
			count = selectmenu (currentwindow->tm+currentwindow->rcursl, 
				currentwindow->lm+currentwindow->rcursc, nitems, list);
			freestackmenu(nitems,list);
			returnfromstack2(count);
		}
        }else{
                pulllink(&docvsa);
		returnfromstack2(count);
        }

}
freestackmenu(nitems,list)
  med nitems;
  char **list;
{
  med i;
	for(i=0;i<nitems;ffree(list[i++]))
		;
	ffree(list);
}

  void
returnfromstack2(count)
  med count;
{
  med line, col, i;   


        for(i=0; i < count;i++){          
                pulllink (&docvsa);
        }
        line = currentwindow->rcursl;
        col = currentwindow->rcursc;
	if(currentwindow->ispartofparalleltextface &&firstofparalleltextface()){
		changecurrentwindowdimensions(0,0,screenheight,screenwidth);
		for(i=0;i<currentwindow->windowheight;++i){
                     currentwindow->windowline_glorpfed[i]= TRUE;
		}
		currentwindow->ispartofparalleltextface = FALSE;
	}
#ifdef COMMENT
        if (count) {
                /*drawboxaroundwindow (currentwindow);*/
                fillwindow (currentwindow, &docvsa);
        } else if (TRUE || flag) {
                fixglorpfsinwindow (currentwindow);
	}
#endif

	wmove(currentwindow->w,line,col);

	if (currentwindow->border)
		touchwin(currentwindow->border);
	touchwin(currentwindow->w);

	if (currentwindow->border)
		wrefresh(currentwindow->border);
	wrefresh(currentwindow->w);
}
  med
firstofparalleltextface() /*is this the first of the pair */
{
	if(currentwindow->ispartofparalleltextface){
		return currentwindow->correspondingwindow ->superordinatewindow ==
currentwindow;
	}else {
		return (0);
	}
}

ndocsforstack()
{
  windowtype *curr;                       
  med ndocs;
                                      
        for (ndocs=0,curr=currentwindow; curr ;curr=curr->superordinatewindow)
                ++ndocs;
  return(ndocs);
}
 
  med 
makelistforstackmenu(list)
  char /***/**list;
{

/*  windowtype *curr;                       
  med ndocs;
  char doctext[80],*text;
  spectype *ptr , *docspecptr;   
long couple,of,dummys,tobe,padding;
*/
 med ndocs;
  char doctext[80],*text;
  spectype *ptr , *docspecptr;
long couple,of,dummys,tobe,padding;
  windowtype *curr;

        ndocs = ndocsforstack();
        for(curr=currentwindow;curr;curr=curr->superordinatewindow){
                ptr = curr->windowview.viewspecptr;
                text = (char *)ealloc (DOCNAMESIZE);
                *list++ = text;                    
                docspecptr = specspanalloc ();
                movetumbler (&ptr->docid, &docspecptr->docid);   
                docspecptr->specspanptr->vstartaddress.smallmantissa[0] = ptr->specspanptr->vstartaddress.smallmantissa[0];
                docspecptr->specspanptr->vstartaddress.smallmantissa[1] = ptr->specspanptr->vstartaddress.smallmantissa[1]+curr->windowview.characterdisplacementwithinview;     

                docspecptr->specspanptr->sizeofspan = min(screenwidth,80);
                grabspec (docspecptr, doctext);
                specfree (docspecptr);              
                copystringwithoutwhitespace (doctext, text, DOCNAMESIZE); 
        }
           /*zzz when do we free list?*/
        return (ndocs);
}

  void
pushwindow (newwindow)
  windowtype *newwindow;
{
  med i;

        /*freeupcurrentwindowslinkset();*/

/*
	for(i=0;i<currentwindow->windowheight;++i){
                     currentwindow->windowline_glorpfed[i]= TRUE;
*/

        currentwindow->subordinatewindow = newwindow;
        newwindow->superordinatewindow = currentwindow;
        currentwindow = newwindow;
}

  void
pulllink (docvsaptr)
  tumbler *docvsaptr;
{
  windowtype *previouswindow, *this;

        if (currentwindow == &theoneandonlyscreen.basewindow)
                return;
        freeupcurrentwindowslinkset ();
        previouswindow = currentwindow->prevwindow;
        if (previouswindow)
                previouswindow->nextwindow = currentwindow->nextwindow;
        if (currentwindow->nextwindow)
                currentwindow->nextwindow->prevwindow = previouswindow;
        this = currentwindow;
        currentwindow = currentwindow->superordinatewindow;
        windowfree (this);
        currentwindow->subordinatewindow = NULL;
        interactendset/*kluge*/(currentwindow->windowview.viewspecptr,docvsaptr);
        docvsaptr->mantissa[findlastplace (docvsaptr)] += currentwindow->windowview.characterdisplacementwithinview;
}


  void
freeupcurrentwindowslinkset()
{
/* this was commented out */
        currentwindow->isendsetokflag = FALSE;
        specfree (currentwindow->windowview.viewmaskspecptr);
        currentwindow->windowview.viewmaskspecptr = NULL;
        specfree (currentwindow->windowview.viewfromset);
        currentwindow->windowview.viewfromset = NULL;
        specfree (currentwindow->windowview.viewtoset);
        currentwindow->windowview.viewtoset = NULL;
}
 
