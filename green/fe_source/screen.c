/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:      screen.d
***************************************************/

#include "fest.h"
#include "feminreq.h"

  void
getcursor (colptr, lineptr)
  med *colptr, *lineptr;
{
        *colptr = currentwindow->rcursc;
        *lineptr = currentwindow->rcursl;
	if(/*FALSE&&*/iswindow){/*remove cursor*/
		currentwindow->cursorisonscreen = FALSE;
		reversecharacter(currentwindow->lm+currentwindow->rcursc,currentwindow->tm+currentwindow->rcursl);
	}
}

  void
homecursorposition()
{
        placecursor (0, 0);
}

  void
homedowncursorposition()
{
        placecursor (0, currentwindow->windowheight-1);
}

  void
putcursor()
{
	wmove(currentwindow->w,currentwindow->rcursl,currentwindow->rcursc);
}

  void
placecursor (col, line)
  med col, line;
{
	if(currentwindow->cursorisonscreen&&iswindow){
		reversecharacter(currentwindow->lm+currentwindow->rcursc,currentwindow->tm+currentwindow->rcursl);
			currentwindow->cursorisonscreen= FALSE;
	}
        currentwindow->rcursl = line;
        currentwindow->rcursc = col;
	if(iswindow){
		if(!currentwindow->cursorisonscreen){
			reversecharacter(currentwindow->lm+currentwindow->rcursc,currentwindow->tm+currentwindow->rcursl);
			currentwindow->cursorisonscreen= TRUE;
		}
	}else{
		putcursor ();
	}
}

  void
movecursor (deltax, deltay)
  long deltax, deltay;
{
	if(currentwindow->cursorisonscreen&&iswindow){
		reversecharacter(currentwindow->lm+currentwindow->rcursc,currentwindow->tm+currentwindow->rcursl);
	}
        currentwindow->rcursl += deltay;
        currentwindow->rcursc += deltax;
        if (currentwindow->rcursc > currentwindow->windowwidth
         || currentwindow->rcursl > currentwindow->windowheight
         || currentwindow->rcursc < 0
         || currentwindow->rcursl < 0) {
/**/
                fprintf(stderr,"going to bad place line=%d col=%d\n",
			currentwindow->rcursl,currentwindow->rcursc);
/**/
                homecursorposition();
                return;
        }
	if(iswindow){
		reversecharacter(currentwindow->lm+currentwindow->rcursc,currentwindow->tm+currentwindow->rcursl);
	}else{
		putcursor ();
	}
}
