/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:     termlow.d
***************************************************/

#include "fest.h"

#define NORMAL        0
#define BLINK         1
#define REVERSEVIDEO  2
#define HIGHintENSITY 4
#define UNDERLINE     8

#define OLDCURSES

med oldmask = NORMAL; /* now shared with mask.d */

  void
changedisplayattributes (newmask)
  nat newmask;
{
  bool first;
  int foomask;

	if(iswindow) {
		return;
	}
        if(oldmask == newmask){
                return;
        }
        oldmask = newmask;

        if(newmask == NORMAL) {
		wstandend(currentwindow->w);
	} else {
#ifdef OLDCURSES
		wstandout(currentwindow->w);
#else
		foomask = 0;
	        if(newmask&BLINK)	     foomask |= A_BLINK;
	        if(newmask&REVERSEVIDEO)     foomask |= A_STANDOUT;
	        if(newmask&HIGHintENSITY)    foomask |= A_BOLD;
	        if(newmask&UNDERLINE)	     foomask |= A_UNDERLINE;
		wattron(currentwindow->w,foomask);
#endif
	}
}

#ifdef COMMENT
  void
setautoxoff()
{
}

  void
clearautoxoff()
{
}
#endif COMMENT

  void
deleteline(line)
  med line;
{	
  int y,x;
	getyx(currentwindow->w,y,x);
	wmove(currentwindow->w,line,0);
	wdeleteln(currentwindow->w);
	wmove(currentwindow->w,y,x);
}

#ifdef COMMENT
  void
erasescreen ()
{
	wclear(currentwindow->w);
}
#endif COMMENT

  void
erasefromcursor ()
{
	wclrtobot(currentwindow->w);
}

  void
eraseol()
{
if(iswindow){fprintf(stderr,"eraseol in termlow is window");gerror(""); return;}
	wclrtoeol(currentwindow->w);
}

  void
erasebol()
{
if(iswindow){ fprintf(stderr," ereasebol 8 in termlow is window e");return;}

 gerror("erasebol is bogus in fix it");
}

  void
insertline(line)
  med line;
{
  int y,x;
	getyx(currentwindow->w,y,x);
	wmove(currentwindow->w,line,0);
winsertln(currentwindow->w);
	wmove(currentwindow->w,y,x);
}

  void
gotoxy(x,y)
  med x,y;
{
	wmove(currentwindow->w,y,x);
}


#ifdef COMMENT
  void
drawcurs (line,col)
  med line,col;
{
        gotoxy(col+1,line+1);
}

  void
textplot (string, line, col)
  char *string;
  med line,col;
{
        putf("%C%s",(nat)col,(nat)line,string);
        return;
}

  void
beep(){
putf("%C%c",(nat)0,(nat)0,'G' - '@');
}
#endif COMMENT
