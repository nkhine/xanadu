/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:     display.d
***************************************************/

#include "fest.h"
#include "feminreq.h"

  /*long displayfilledithlineinwindow();*/
  long nextl();

  void
insertlineattop (window, textptr, displaymaskptr)
  windowtype *window;
  char *textptr;
  masktype *displaymaskptr;
{
  void insertlineinwindow();
        insertlineinwindow(window,textptr,displaymaskptr,0);
}
  void
insertlineinwindow (window, textptr, displaymaskptr,line)
  windowtype *window;
  char *textptr;
  masktype *displaymaskptr;
  med line;
{
  med i, tmpc, tmpl,temp,n,len;
  long lastl(); 
  void insertlineatbottom();
  void fixglorpfsinwindow();
  void displayfilledithlineinwindow();

        if(line >= window->windowheight -1 ){
                insertlineatbottom(window,textptr,displaymaskptr);
                return;
        }                     
        len = strlen(textptr);
        for(i = window->windowheight; i > line; i--){
               window->windowleftmargins[i] = window->windowleftmargins[i-1]+len;
               window->windowline_glorpfed[i]=window->windowline_glorpfed[i-1];
        }   
        if(line == 0)
                window->windowleftmargins[line]=lastl(window->windowleftmargins[line]);
        else /* because lastl doesnt yet work right */
                  window->windowleftmargins[line] = window->windowleftmargins[line-1];     
        window->windowline_glorpfed[line] = TRUE;
#ifndef MSDOS
        if(!iswindow&&window->windowwidth == screenwidth){
                deleteline (window->tm + window->windowheight-1);
                insertline (window->tm+line);
                displayfilledithlineinwindow (window, window->tm+line, textptr, displaymaskptr);
        	window->windowline_glorpfed[line] = FALSE;

        }else if (iswindow){
                n = -1;
   		reversecharacter(currentwindow->lm+currentwindow->rcursc,currentwindow->tm+currentwindow->rcursl);
                scrlsubwindow (window->lm, window->tm+line,window->windowwidth, window->windowheight, n);
   		reversecharacter(currentwindow->lm+currentwindow->rcursc,currentwindow->tm+currentwindow->rcursl);
                temp = n;

                for (temp = line;temp<-n;temp++){
                        window->windowleftmargins[temp] = lastl(window->windowleftmargins[temp+1]);
                       /* window->windowline_glorpfed[temp] = TRUE;    */
                }
                fixglorpfsinwindow (window);      
		wrefresh (currentwindow->w);
        }else{
#else
	if(TRUE){
#endif

                getcursor (&tmpc, &tmpl);
                                                 /**/
                for (i = line; i < window->windowheight; i++) {
                        fillithline (window, i);
                }                /*ZZ*/
                fixglorpfsinwindow(window);
                placecursor (tmpc, tmpl);
		wrefresh (currentwindow->w);
        }
}

  void
insertlineatbottom (window, textptr, displaymaskptr)
  windowtype *window;
  char *textptr;
  masktype *displaymaskptr;
{
  med i, tmpc, tmpl,n,temp;
  /*long displayfilledithlineinwindow();*/
  void fixglorpfsinwindow();
  void insertlineinwindow();

        for(i=0;i<window->windowheight;i++){
                window->windowleftmargins[i] = window->windowleftmargins[i+1];
                window->windowline_glorpfed[i] = window->windowline_glorpfed[i+1];
        }
        if (!iswindow&&window->windowwidth == screenwidth) {
                deleteline (window->tm);
                insertline (window->tm + window->windowheight-1);
                displayfilledithlineinwindow (window, window->windowheight-1, textptr, displaymaskptr);
		window->windowline_glorpfed[window->windowheight-1] = FALSE;
        } else if (iswindow){
                n=1;
   		reversecharacter(currentwindow->lm+currentwindow->rcursc,currentwindow->tm+currentwindow->rcursl);
                scrlsubwindow (window->lm, window->tm,window->windowwidth, window->windowheight, n);
   		reversecharacter(currentwindow->lm+currentwindow->rcursc,currentwindow->tm+currentwindow->rcursl);
                temp = n;

                for (temp = window->windowheight-n;temp<window->windowheight;temp++){
                        window->windowleftmargins[temp] = nextl(window->windowleftmargins[temp-1]);
                        window->windowline_glorpfed[temp] = TRUE;
                }
                fixglorpfsinwindow (window);      
		wrefresh (currentwindow->w);
        }else{
                getcursor (&tmpc, &tmpl);
                pullupleftmargins (/*-1*/0);
                for (i = 0; i < window->windowheight; i++) {
                        fillithline (window, i);
                }
                placecursor (tmpc, tmpl);
/*        fprintf(stderr,"random fucking narrow windows motherfucker\n\n\n\n\n");*/
        }
}

  void
fixglorpfsincurrentwindow()
{
	fixglorpfsinwindow(currentwindow);
	wrefresh (currentwindow->w);
}

  void
fixglorpfsinwindow (window)
  windowtype *window;   /*called wither an unglorfed line or ok displacement*/
{
  med i, col, line;
  char text[SCREENWIDTH+1];
  masktype displaymask[SCREENWIDTH+1];
  spectype *specptr, *specalloc();
  long temp;

        getcursor (&col, &line);
						        /*set up line beginings*/
        for (i =0; i <  window->windowheight-1;i++){
               if ((window->windowline_glorpfed[i+1] || window->windowline_glorpfed[i]) &&
                   (window->windowleftmargins[i+1] != nextl(window->windowleftmargins[i]))) {
                        window->windowline_glorpfed[i+1] = TRUE;
                        window->windowleftmargins[i+1] = nextl(window->windowleftmargins[i]);
                }
        }
						        /* display stuff*/
        for (i = 0; i < window->windowheight; i++) {
                if (window->windowline_glorpfed[i]) {
                        specptr = specalloc ();
                        getspecsetfordisplacementinviewinwindow (window, window->windowleftmargins[i], (long)window->windowwidth, specptr);
/*
fprintf(stderr,"line %2d grabbing spec\n");
dumpspec(specptr);
fprintf(stderr,"\n");
*/

                        grabspec (specptr, text);
                        fillupmask/*andcursor*/(window/*,line*/,i,specptr,displaymask);
                        specfree (specptr);
                       			/* getstuff */
                        displayithlineinwindowwithmask(window,i,text,displaymask);
					/*display ith line moves cursor*/
                        window->windowline_glorpfed[i] = FALSE;
					/*for fillupmask to place parallel cursor*/
			window->rcursl = line;
			window->rcursc = col;
		/* note this (nonredundant) stuff fixes nextl not */
		/* ability to feel end of document*/
			temp = nextl(window->windowleftmargins[i]);
			if (i<window->windowheight-1&&temp != window->windowleftmargins[i+1]){
				window->windowline_glorpfed[i+1] = TRUE;
				window->windowleftmargins[i+1] = nextl(window->windowleftmargins[i]);
				/*fprintf(stderr,"fixg t=%d",temp);*/
			}
				if(FALSE&&window->windowleftmargins[i+1] == window->windowleftmargins[i]){
					break;/* don't refresh the blank lines at the end of the window SKIMP */
				}
                }
        }

	if(iswindow)
		placecursor (col,line);
	else
		placecursor (col,line);

}
  void
displayithlineinwindow (window, i, textptr, displaymaskptr)
  windowtype *window;
  med i;
  char *textptr;
  masktype *displaymaskptr;
{

        displayithlineinwindowwithmask (window, i, textptr, displaymaskptr);

}

  void /*long*/
displayfilledithlineinwindow (window, line, textptr, displaymaskptr)
  windowtype *window;
  med line;
  char *textptr;
  masktype *displaymaskptr;
{
  med i;
  med correction;
  bool foo;
                                                 
        if(textptr == NULL)
                return/*(0)*/;
        for(i = 1; textptr[i] && textptr[i] != '\n'&& i <= window->windowwidth/*+1*/; ++i);
        correction = (textptr[i]&&textptr[i] != '\n' ? 1 : 0);
        textptr[i] = '\0';
        foo = textptr[0] == '\n';
        displayithlineinwindow (window, line, textptr/*+foo*/, displaymaskptr);
      /*  return (window->windowleftmargins[line]+i+correction);*/
}
