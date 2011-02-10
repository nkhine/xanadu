/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:        edit.d
***************************************************/

#include "fest.h"

/*#define MAKEBOMBPROOF  disableinsert*/


  void
insertanddeletechar (requestptr)
  requesttype *requestptr;
{
  tasktype task;

/*
fprintf(stderr,"%d %d",currentwindow->rcursl,currentwindow->rcursc);
*/
	if (requestptr->requestnumberofcharacters < 0 && (currentwindow->rcursl > 0 || currentwindow->rcursc > 0) ) {
		requestptr->initialcursorpositionspecptr->specspanptr->sizeofspan = -requestptr->requestnumberofcharacters;
		requestptr->initialcursorpositionspecptr->specspanptr->vstartaddress.smallmantissa[1] += (requestptr->requestnumberofcharacters /*+1zzz*/);
 					/* delete these damn characters */
/*
fprintf(stderr," delete ");
*/
        	deletespecfrombe (requestptr->initialcursorpositionspecptr);

    } elif (requestptr->requestnumberofcharacters > 0) {
    					/* insert these characters */
		insertcharactersbeforespecifiedposition (requestptr->initialcursorpositionspecptr,requestptr->requeststring,requestptr->requestnumberofcharacters);
    } else {
        /* qerror ("INSERTANDDELETECHAR called with 0 characters."); */
        return;
    }
    if(currentwindow->ispartofparalleltextface) {
          compareviewspecs(&task,currentwindow->windowview.viewspecptr,currentwindow->correspondingwindow->windowview.viewspecptr );
    }
    specfree (requestptr->initialcursorpositionspecptr);
}


  void
displayinsertanddeletechar(specptr,string,numberofcharactersptr,numberofblanks)
  spectype *specptr;
  char string[];
  long *numberofcharactersptr,numberofblanks;
{

/*
fprintf(stderr,"displayinsertanddeletechar\n");
dumpspec(specptr);
*/
        if(*numberofcharactersptr == 0)
                return;
        fixvm (specptr, string, *numberofcharactersptr);
        fixendsetvm (specptr, *numberofcharactersptr);
        fixwindowdisplay (string,*numberofcharactersptr,numberofblanks);
        *numberofcharactersptr = 0;
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

/*
fprintf(stderr,"insertcharactersbeforespecifiedposition\n");
dumpspec(specptr);
fprintf(stderr,"|%.*s| %d\n",nchars,text,nchars);
*/
        charspan.numberofcharactersinspan = nchars;
        charspan.charinspanptr = text;
        charspan.nextcharspan = NULL;
        start = specptr->specspanptr->vstartaddress.smallmantissa[1]; /*zzz?-1*/
        smalltumblerclear (&vsatoinsertat);
        vsatoinsertat.smallmantissa[0] = 1;
        vsatoinsertat.smallmantissa[1] = start;
/*
dumptumbler(&vsatoinsertat);
*/
        inserttextinbackend(&task,&specptr->docid, &vsatoinsertat, &charspan);
}
  void
fixwindowdisplay (string,numberofcharacters,numberofblanks)
  char string[];
  long numberofcharacters,numberofblanks;
{
  med line, col;
  long i;
  long deltay;
  long nextl();
  med line2;

if(0){
fprintf(stderr,"fixwindowdisplay %.*s %d %d (%d)\n", numberofcharacters,string,numberofcharacters,numberofblanks,currentwindow->rcursc);
}
        deltay = 0;
        line = currentwindow->rcursl;
        col = currentwindow->rcursc  ;  /* -1 zzzz */
        currentwindow->windowline_glorpfed[line] = TRUE;

						/******* INSERTION **********/
        if (numberofcharacters > 0) {
                for (i = 0; i < numberofcharacters; i++) {
                        if (string[i] == '\n') {
					/* note pre ++ we actually want to insert a line */
					/* after the current one */ 
				insertlineinwindow(currentwindow, "\n",(masktype *) NULL,++line);
				/**/  ++deltay;/**/
                                col = 0;
                        } else {
                                for(; currentwindow->windowleftmargins[line] + currentwindow->windowwidth == nextl(currentwindow->windowleftmargins[line]);) {
                                       line ++;
				       /*deltay--;*/
                                       currentwindow->windowline_glorpfed[line]=TRUE;
                                }
                                incrementrestofleftmargins(line, 1);
                                ++col;
                                if (col > currentwindow->windowwidth) {
                                        col = 0;
                                        ++line;
                                        ++deltay;
                               }
                        }
                        currentwindow->windowline_glorpfed[line] = TRUE;
                }
						/****** DELETION *********/
        } else {
                for (i = numberofcharacters; i < 0; i++) {
                        --col;
			for(line2 = line;currentwindow->windowleftmargins[line2]+currentwindow->windowwidth == nextl(currentwindow->windowleftmargins[line2]);){
			       line2 ++/*,deltay--*/;
			       currentwindow->windowline_glorpfed[line2]=TRUE; 
			}
                        incrementrestofleftmargins (line, -1);
                        if (col < 0) {
                                pullupleftmargins (line);
                                --line;
                                --deltay;
                                col = windowlinelength (line);
                        }
                        currentwindow->windowline_glorpfed[line] = TRUE;
                }
        }
        fixglorpfsinwindow (currentwindow);
        movecursor ((long)(col-(currentwindow->rcursc)-numberofblanks), deltay);
	wrefresh(currentwindow->w);
}

/*---------------------------removed from above ----------------------------*/
/*fprintf(stderr,"rumblemumblefoo  ");*/
/*  				pushdownleftmargins (line);
                                incrementrestofleftmargins (line, col+1);
                                ++line;
                                pushdowntextonscreen (line);
*/
/*---------------------------------------------------------------------------*/


/* the above replaced by */
/*-----------------------------------------------*/
/*         I suspect these should go somewhere else */
  med 
windowlinelength (line)
  med line;
{                         /* zzzzz */
        return ((med)currentwindow->windowleftmargins[line+1] - currentwindow->windowleftmargins[line]-1);
}

  void
pullupleftmargins (line)
  med line;
{
  med i;

        for (i = line+1; i < currentwindow->windowheight+1; i++)
                currentwindow->windowleftmargins[i] = currentwindow->windowleftmargins[i+1];
}


#ifdef COMMENT
  void
pushdownleftmargins (line)
  med line;
{
  med i;
  long lastl();

        for (i = currentwindow->windowheight+1; i > line; i--)
                currentwindow->windowleftmargins[i] = currentwindow->windowleftmargins[i-1];
        currentwindow->windowleftmargins[line] = lastl(currentwindow->windowleftmargins[line-1]);
}
#endif COMMENT

  void
incrementrestofleftmargins (line, inc)
  med line, inc;
{
        for (++line; line < currentwindow->windowheight+1; ++line)
                currentwindow->windowleftmargins[line] += inc;
}

/*pushdowntextonscreen (line)
  med line;
{
        deleteline (currentwindow->windowheight);
        insertline (currentwindow->tm + line);
}*/

/*                   if ((temp = currentwindow->windowleftmargins[line] - currentwindow->windowleftmargins[line-1]) > currentwindow->windowwidth) {
                        fprintf (stderr,"\33[55;1fline overflow.");
                        ++line;
                        ++deltay;
                        currentwindow->windowleftmargins[line] = currentwindow->windowleftmargins[line-1] + currentwindow->windowwidth;
                        col = 0;
                }
*/

/*----------------------------------------------*/
  void
deletespecfrombe (specptr)
  spectype *specptr;
{
  tasktype task;
/*
fprintf(stderr,"deletespecfrombe\n");
dumpspec(specptr);
*/
        deletefrombackend (&task, &specptr->docid, specptr->specspanptr);
}

/*
createlink()
{
}

*/
  void
rearrange()
{
  spectype *specptr;
  tasktype task;
  documenttype *docptr;
        currentwindow->isendsetokflag = FALSE;
        specptr = specalloc();
        findspecfromcursorposition (specptr);
        if (completecutseq (&specptr->specspanptr->vstartaddress)) {
                rearrangeinbackend (&task, &specptr->docid, &cutsequence);
/* DO SOMETHING ABOUT LINKS! */
                fixvmforrearrange (&specptr->docid);
                tfree (&task.tempspace);
                        if(currentwindow->ispartofparalleltextface){
                        compareviewspecs(&task,currentwindow->windowview.viewspecptr,currentwindow->correspondingwindow->windowview.viewspecptr );
                }
        }
        clearcuts();
        specfree (specptr);
}
  void
delete()
{
/*
  spectype *specptr;
  tasktype task;
  documenttype *docptr;
        currentwindow->isendsetokflag = FALSE;
        specptr = specalloc();
        findspecfromcursorposition (specptr);
        if (completecutseq (&specptr->specspanptr->vstartaddress)) {
                deleteinbackend (&task, &specptr->docid, &cutsequence);
                fixvmforxx(&specptr->docid);
                tfree (&task.tempspace);
                        if(currentwindow->ispartofparalleltextface){
                        compareviewspecs(&task,currentwindow->windowview.viewspecptr,currentwindow->correspondingwindow->windowview.viewspecptr );
                }
        }
        clearcuts();
        specfree (specptr);
*/
}
  void
copy()
{
}
  void
createfootnote()
{
  tasktype task;
  tumbler docid, docvsa;
  smalltumbler temp;
  spectype *toset;
  spectype *fromset;

	toset = specalloc();
	fromset = specalloc();
        createnewdocumentinbackend (&task, &docid);
        smalltumblerclear (&temp);
        temp.smallmantissa[0] = temp.smallmantissa[1] = 1;
        makedocvsa (&docid, &temp, &docvsa);
	maketempthreeset(FOOTNOTELINK);
	findspecfromcursorposition(fromset);
/*create spec pointing into new document kluge ad cheat*/
	copyvpositionintospec(&docvsa,toset);
	toset->specspanptr->sizeofspan = 1;
	insertcharactersbeforespecifiedposition (toset, "\n\n\n\n",(long) 4);
	currentwindow->isendsetokflag = FALSE;
	toset->specspanptr->vstartaddress.smallmantissa[1] = 1;
  createlinkinbackend(&task,&staticlinkhomeposition/*kluge for errorincreatelink see createlink docid*/, fromset,toset,tempthreeset);

        makefootnotewindow (&docvsa);
	putdocumentinvm(&docid);
	currentwindow->newlycreateddocumentp = TRUE;
        fillwindow (currentwindow, &docvsa);
}

  void 
createdocument()
{
  tasktype task;
  tumbler docid, docvsa;
  smalltumbler temp;
  charspantype charspan;

        putindexdocumentonstack();
        createnewdocumentinbackend (&task, &docid);
        smalltumblerclear (&temp);
        temp.smallmantissa[0] = temp.smallmantissa[1] = 1;
        makedocvsa (&docid, &temp, &docvsa);

        charspan.numberofcharactersinspan = 1;
        charspan.charinspanptr = " ";
        charspan.nextcharspan = NULL;
        inserttextinbackend(&task,&docid, &docvsa, &charspan);

        makejumpwindow (&docvsa);
	putdocumentinvm(&docid);
	currentwindow->newlycreateddocumentp = TRUE;
        fillwindow (currentwindow, &docvsa);
}
 
