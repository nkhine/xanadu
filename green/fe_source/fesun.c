/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:       fesun.d
***************************************************/

#define GRAPHICS

#include "fest.h"

#ifdef SUNGRAPHICS

  void
clearwindowgraphically (window)
  windowtype *window;
{
  nat h, w, x, y;
        determinegraphicextentofpsudowindow (window, &w, &h, &x, &y);
/*fprintf(stderr,"clear window graphically %d %d %d %d\n",w,h,x,y);*/
	winblockclear(x,y,w,h);
/*fprintf(stderr,"clear window graphically %d %d %d %d exiting\n",w,h,x,y);*/
} 

  void
determinegraphicextentofpsudowindow (window, wptr, hptr, xptr, yptr)
  windowtype *window;
  nat *wptr, *hptr, *xptr, *yptr;
{
  nat w, h, x, y;
        w = window->windowwidth;
        h = window->windowheight;
        x = window->lm;
        y = window->tm;
        w = w * fontx /*+ XSPACING - LINEWIDTH*/;
        h = h * fonty /*+ YSPACING - LINEWIDTH*/;
        x =  x * fontx /*+ LINEWIDTH*/;
        y = /*DISPLAYTOP*/   y * fonty ;
        *wptr = w;
        *hptr = h;
        *xptr = x;
        *yptr = y;
}

  void
sunhline (line, col1, col2)
  nat line, col1, col2;
{
        drawhlinesun (line, col1, col2,PIX_SET/*PIX_SRC^PIX_DST*/);
}

  void
erasehline (line, col1, col2)
  nat line, col1, col2;
{
        drawhlinesun (line, col1, col2,PIX_CLR);
}

  void
drawhlinesun (line, col1, col2,colour)
  nat line, col1, col2,colour;
{
  nat x, y, w;

        getpositiontodrawline (col1+1, line, &x, &y);
        w = (col2 - col1) * fontx;
	pw_vector(gfx->gfx_pixwin,x-3,y,x+w,y,colour,1);
        /*ras_write (w, LINEWIDTH, x+XSPACING*2, DISPLAYTOP-y);*/
}

#ifdef COMMENT
  void
sunvline (line1, line2, col)
  nat line1, line2, col;
{
        drawvlinesun (line1, line2, col,PIX_SET/*PIX_SRC^PIX_DST*/);
}

  void
erasevline (line1, line2, col)
  nat line1, line2, col;
{
        drawvlinesun (line1, line2, col,PIX_CLR);
}
#endif COMMENT

  void
drawvlinesun (line1, line2, col,colour)
  nat line1, line2, col,colour;
{
  nat x, y, h;

        getpositiontodrawline (col, line1, &x, &y);
	x = x+ fontx/2;
        h = (line2 - line1/**/+1/**/) * fonty/* + LINEWIDTH*/ ;
	pw_vector(gfx->gfx_pixwin,x,y,x,y+h,colour,1);
        /*ras_write (LINEWIDTH, h, x+XSPACING*2, DISPLAYTOP-y-h+LINEWIDTH);*/

}

  void
plotlineundercharacter (col, line)
  nat col, line;
{
  nat xpos, ypos;

        getpositiontodrawline (col, line, &xpos, &ypos);
        drawcharacterunderline (xpos, ypos);
}

  void
reversecharacter (col, line)
  nat col, line;
{
  nat xpos, ypos;

	graphbox(col,line+1/*zzz*/,1,1,1);
}

  void
getpositiontodrawline (col, line, xptr, yptr)
  nat col, line;
  nat *xptr, *yptr;
{
        *yptr = (line+1) * fonty-fonty/4;
        *xptr = (col) * fontx;
}  

  void
drawcharacterunderline (x, y)
  nat x, y;
{
	pw_vector(gfx->gfx_pixwin,x,y,x+fontx+fonty/4,y,PIX_SET/*PIX_SRC^PIX_DST*/,1);
}
#else

  void
clearwindowgraphically (window)
  windowtype *window;
{
}
void sunvline(line,col1,col2)nat line,col1,col2;{}
void sunhline(line1,line2,col)nat line1,line2,col;{}
void plotlineundercharacter(col,line)nat col,line;{}
/*
void drw2_abs(){}
void mov2_abs(){}
*/
void reversecharacter(col,line)nat col,line;{}
#endif
