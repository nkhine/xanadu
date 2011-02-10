/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:      window.d
***************************************************/

#include "fest.h"
#include "feminreq.h"
                  
#define SPLITSCREEN

  long getcontestsoflastplace();
  long nextl();

char blankline[] = " ";


  void
makejumpwindow (docvsaptr)
  tumbler *docvsaptr;
{
  	med line,column,height,width;      

        if(displaystyle == SPLITVERTICAL){  
                width = screenwidth/2 - 1;
                if (currentwindow->lm == 0)
                        column = screenwidth/2;
                else
                       column = 0;
                line = 0;
                height = screenheight-2;
                jumpwindowcontrol (line, column, height, width, docvsaptr);

        } else if(displaystyle == SPLITHORIZONTAL){  
                width = screenwidth;
                if (currentwindow->tm == 0)
                        line = screenheight/2-1 ;
                else
                        line = 0;
                column = 0;
                height = screenheight/2-1;
                jumpwindowcontrol (line, column, height, width, docvsaptr);

        } else if(displaystyle == QUARTERDISPLAY) {  
   
                width = screenwidth/2 - 1;
                if (currentwindow->tm == 0) {
                        if (currentwindow->lm == 0) {
                                line = 0;
                                column = screenwidth/2;
                        } else {
                                line = screenheight/2;
                                column = screenwidth/2;
                        }
                } else {
                        if (currentwindow->lm == 0) {
                                line = 0;
                                column = 0;
                        } else {
                                line = screenheight/2;
                                column = 0;
                        }       
                }
                height = screenheight/2-1;

                jumpwindowcontrol(line,column,height,width,docvsaptr); 

        }else if (displaystyle == FULLDISPLAY){   
                jumpwindowcontrol (0, 0, screenheight-2,screenwidth,docvsaptr);

        }else if (displaystyle == RANDOMDISPLAY){
                width  = max(4,abs(rand())%(screenwidth-1)); 
                height = max(4,abs(rand())%(screenheight-2));  
                                                   
                column =  abs(rand())%(screenwidth - width);
                line =abs(rand())%(screenheight - height); 
                jumpwindowcontrol (line, column, height,width, docvsaptr);

        }else if (displaystyle == FOOTNOTEDISPLAY){
                width = screenwidth;
                height = 4;
                line = screenheight - height -2;
                column = 0;
                jumpwindowcontrol (line, column, height,width, docvsaptr);
        }else if (displaystyle == MARGINALNOTEDISPLAY){
                width = currentwindow->windowwidth/3;
                height = currentwindow->windowheight;
                line = currentwindow->tm ;
                column = currentwindow->windowwidth -width +currentwindow->lm;
if(!iswindow&&((line+height)==screenheight)){
	height --;
}
                jumpwindowcontrol (line, column, height,width, docvsaptr);
        }else if (displaystyle == EXPERIMENTALDISPLAY){
                line = currentwindow->tm +3;
                column = currentwindow->lm +3;
                width = currentwindow->windowwidth -3;
                height = currentwindow ->windowheight -3;
                jumpwindowcontrol (line, column, height,width, docvsaptr);
                
        }else { /* other and illegal */
                jumpwindowcontrol (0, 0, screenheight-2,screenwidth,docvsaptr);
        }
/*
if(iswindow){
 fprintf(stderr,"\n line %d column %d width %d height %d \n",line,column ,width, height);
}
*/
}

  void
makemarginalnotewindow (docvsaptr)
  tumbler *docvsaptr;
{
  med savedisplay;
/* rogers hack*/                      
        savedisplay = displaystyle;
        displaystyle = MARGINALNOTEDISPLAY;
        makejumpwindow(docvsaptr);
        displaystyle = savedisplay;                   

}

  void
makefootnotewindow (docvsaptr)
  tumbler *docvsaptr;
{
  med savedisplay;
/*  spectype *specset;
{
 erics hack
  med line, column, height, width;

        line=min (screenheight-4, currentwindow->tm+currentwindow->windowheight+1);
        column = currentwindow->lm;
        height = 3;
        width = currentwindow->windowwidth;
        specwindowcontrol (line, column, height, width, specset, FOOTNOTELINK);
*/
/* rogers hack*/                      
        savedisplay = displaystyle;
        displaystyle = FOOTNOTEDISPLAY;
        makejumpwindow(docvsaptr);
        displaystyle = savedisplay;                   

}

/*specwindowcontrol (line, column, height, width, specset, wtype)
  med line;
  med column;
  med height;
  med width;
  spectype *specset;
  med wtype;
{
        initcurrentwindow (line, column, height, width, wtype);
        copyspecset (specset, &currentwindow->windowview.viewspecptr);
}*/

  void
jumpwindowcontrol (line, column, height, width, docvsaptr)
  med line;
  med column;
  med height;
  med width;
  tumbler *docvsaptr;
{
        initcurrentwindow (line, column, height, width, JUMPLINK);
        copyvpositionintospec(docvsaptr,currentwindow->windowview.viewspecptr);
        currentwindow->windowview.viewspecptr->specspanptr->vstartaddress.smallmantissa[1] = 1; /* set view to start of document */
        currentwindow->windowview.viewspecptr->specspanptr->sizeofspan=BIGVIEW;
}

  void
initcurrentwindow (line, column, height, width, wtype)
  med line;
  med column;
  med height;
  med width;
  med wtype;
{
  windowtype *initwindow();

        pushwindow (initwindow (line, column, height, width, wtype));
}

  windowtype *
initwindow (line, column, height, width, wtype)
  med line;
  med column;
  med height;
  med width;
  med wtype;
{                                /* stolen from init */
  windowtype *window, *windowalloc();
  void expand();

        window = windowalloc();
        window->lm = column;
        window->tm = line;
        window->windowheight = height;
        window->windowwidth = width;
        window->rcursl = 0;
        window->rcursc = 0;
        window->isendsetokflag = FALSE;
        window->typeofwindow = wtype;
        window->correspondingwindow = NULL;
        window->ispartofparalleltextface = NULL;
        window->correspondencelist = NULL;
	window->cursorisonscreen = FALSE;
	window->w = newwin(height,width,line,column);
	if (displaystyle == FULLDISPLAY) {
		wclear(window->w);
	}
	window->border = NULL;
	keyset(window->w);
        return (window);
}

  void
fillwindowfromwindowspec (window)
  windowtype *window;
{

        drawboxaroundwindow (window);
        window->windowleftmargins[0] = window->windowview.viewspecptr->specspanptr->vstartaddress.smallmantissa[1];
        window->windowview.characterdisplacementwithinview = window->windowleftmargins[0]-1; /*getcontentoflastplace (docvpositionptr) - 1;*/
        paintwindowplain (window);
}

  void
fillwindow (window, docvpositionptr)
  windowtype *window;
  tumbler *docvpositionptr;
{
#ifdef AMIGA
/* putf("\33[O p"); */ /* UGLY KLUDGE WARNING -- MMFB */
#endif
/*
fprintf(stderr,"fillwindow\n");
*/
        drawboxaroundwindow (window);
        window->windowleftmargins[0] = getcontentoflastplace (docvpositionptr)/* - 1*/;
        window->windowview.characterdisplacementwithinview = window->windowleftmargins[0]-1; /*getcontentoflastplace (docvpositionptr) - 1;*/
	paintwindowplain (window);

#ifdef AMIGA
/* putf("\33[ p"); */ /* UGLY KLUDGE WARNING -- MMFB */
#endif
}

  void
paintwindowplain (window)
  windowtype *window;
{
	register med i;

/*
	if(!iswindow){
		placecursor (0,0); // kluge to get cursor on screen //
	}
*/
	for(i=0;i<window->windowheight;++i){
                     window->windowline_glorpfed[i]= TRUE;
	}
	fixglorpfsinwindow(window);
	wrefresh (window->w);
}

  med
fillithline (window, line)
  windowtype *window;
  med line;
{
  spectype *specptr, *specalloc();
  med i, correction;
  masktype displaymask[SCREENWIDTH+1];
  char text[SCREENWIDTH+1];
  bool isnullsomethingflag;
  bool foo;

        isnullsomethingflag = FALSE;
        specptr = specalloc ();
        getspecsetfordisplacementinviewinwindow (window, window->windowleftmargins[line], (long)window->windowwidth, specptr);
        window->windowline_glorpfed[line] = FALSE;
        grabspec (specptr, text);
        if (text[0] == 0) {
                isnullsomethingflag = TRUE;
                i = correction = 0;
        } else {
                for(i=1;text[i]&&text[i]!='\n'&&i<= screenwidth/*+1*/; ++i); 
                correction = ((text[i]&&text[i] != '\n') ? 1 : 0);
                text[i] = '\0';    

                specptr->specspanptr->sizeofspan = i/*+correction*/;
                fillupmask (window, line, specptr, displaymask);
        }
        specfree (specptr);
        foo =  (text[0] == '\n');      
      window->windowleftmargins[line+1]=nextl(window->windowleftmargins[line]);
       if (isnullsomethingflag == FALSE){
              displayithlineinwindowwithmask(window,line,text/*+foo*/,displaymask);
              return(1);
        } else {
                displayithlineinwindow (window, line, blankline,  (masktype *)NULL);
                return (0);
        }
/*
        return(1);
*/
}

  void
clearwindow (window)
  windowtype *window;
{
  med i, j;
	if(iswindow){
		clearwindowgraphically(window);
		return;
	}
        if (window->windowwidth == screenwidth) {
                placecursor (0,0);
                erasefromcursor ();
                return;
        }
        for (i = 0; i < window->windowheight; i++) {
                placecursor (0, i);
                printblanks(window->windowwidth);
        }
}

  void
plotntextcharactersinwindow (window, string, n, line, col)
  windowtype *window;
  char *string;
  med n;
  med line, col;
{
  med i;

	window->rcursc = col;
        window->rcursl = line;
	if(strlen(string) > n) {
		string[n] = '0'; /*zzzz*/
	}
        putf("%C%s",(nat)(col),(nat)(line),string);
/*
	mvwaddstr(window->w,line,col,string);
*/
wrefresh(window->w);
}

#ifdef COMMENT
  void
cleartoendoflineinwindow (n,window,l)
  long n;
  windowtype *window;
  long l;
{
#ifndef MSDOS
/*
        if (iswindow || window->lm + window->windowwidth < screenwidth) {
		gotoxy ( window->lm+n, l + window->tm);
                printblanks(window->windowwidth-n);
        } else{
*/
		gotoxy ( window->lm, l + window->tm);
                eraseol ();
/*
	}
*/
#else
		gotoxy ( window->lm+n, l + window->tm);
                printblanks(window->windowwidth-n);

#endif

}
#endif

  void
clearlineinwindow (window,c,l)
  windowtype *window;
  med c, l;
{

        /*getcursor (&c, &l);*/
	
	if(iswindow){
		winblockclear(c+window ->lm,l+window->tm,window->windowwidth -c,1);
		return;
	}
        gotoxy (c + window->lm, l + window->tm);
#ifndef MSDOS
	if (window->lm + window->windowwidth < screenwidth) {
                printblanks(window->windowwidth);
        } else{
                eraseol ();
	}
#else
                printblanks(window->windowwidth);

#endif
        gotoxy (c + window->lm, l + window->tm);
}

#ifdef COMMENT
  void
plotlineundercharacterinwindow (window, col, line)
  windowtype *window;
  med col, line;
{
        col += window->lm;
        line += window->tm;
        plotlineundercharacter (col, line);
}

  void
reversecharacterinwindow (window, col, line)
  windowtype *window;
  med col, line;
{
        col += window->lm;
        line += window->tm;
        reversecharacter (col, line);
}
#endif COMMENT

  void
placecursorinwindow (window, col, line)
  windowtype *window;
  med col, line;
{ 
        
        window->rcursc = col;
        window->rcursl = line;  
	/*
        gotoxy (window->lm + col, window->tm + line);
	*/
        gotoxy (col,line);
}
