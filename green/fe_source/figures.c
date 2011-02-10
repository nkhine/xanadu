/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:     figures.d
***************************************************/

#include "fest.h"
#include "feminreq.h"

  void
drawboxaroundwindow(window)
  windowtype *window;
{
  register i;
  med tm,lm,width,height,rm,bm;
  char lchar,rchar,tchar,bchar;

	tm = window->tm;
	lm = window->lm;
	height = window->windowheight;
	width = window->windowwidth;

	if (tm+height < screenheight-1) {
		height++;
	}
	if (lm+width < screenwidth-1) {
		width++;
	}
	if (tm > 0) {
		tm--;
		height++;
	}
	if (lm > 0) {
		lm--;
		width++;
	}
	bm = tm+height;
	rm = lm+width;

	if (!(window->border))
		window->border = newwin(height,width,tm,lm);
	wclear(window->border);

	lchar = (lm > 0) ? '|' : ' ';
	rchar = (rm < screenwidth-1) ? '|' : ' ';
	tchar = (tm > 0) ? '-' : ' ';
	bchar = (bm < screenheight-1) ? '-' : ' ';

	for(i=0; i<height; i++) {
		mvwaddch(window->border,i,0,lchar);
		mvwaddch(window->border,i,width-1,rchar);
	}
	for(i=0; i<width; i++) {
		mvwaddch(window->border,0,i,tchar);
		mvwaddch(window->border,height-1,i,bchar);
	}

	if (lm>0 && tm>0)
		 mvwaddch(window->border,0,0,'+');
	if (lm>0 && bm<screenheight-1)
		 mvwaddch(window->border,height-1,0,'+');
	if (rm<screenwidth-1 && tm>0)
		 mvwaddch(window->border,0,width-1,'+');
	if (rm<screenwidth-1 && bm<screenheight-1)
		 mvwaddch(window->border,height-1,width-1,'+');

	wrefresh(window->border);
	/*
	wclear(window->w);
	wmove(window->w,0,0);
	wrefresh(window->w);
	*/
}
