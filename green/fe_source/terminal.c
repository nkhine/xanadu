/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:    terminal.d
***************************************************/

#include "fest.h"

  void 
determinescreendimensions ()
{
	screenwidth = COLS;
	screenheight = LINES;
        displaystyle = SPLITVERTICAL;
}
/*
extern med termisambassador;
*/

#ifdef COMMENT
  void
getandsetxy(x, y)
  med *x, *y;
{ char *getenv(), *malloc(), *tgetstr();
  char **isstring,  *holdingtank, *help;
  char *temp;

        help = malloc(512);
        isstring = &help;
        holdingtank = malloc(2049);
	temp = getenv("TERM");
	if(*temp == '\0'){
		temp = "vt100";
	}
        tgetent(holdingtank,temp); 
        *x=tgetnum("co");
        *y=tgetnum("li");
/*
 fprintf(stderr,"x=%d y=%d %s",*x,*y,temp);
/*
/*
	if(comparestrings(temp,"aaa")){
		termisambassador = 1;
	}else{
		termisambassador = 0;
	}
*/
        free(help);
        free(holdingtank);
	
}

bool comparestrings(s1,s2)
  char *s1, *s2;
{
  char *tmp, *tmp2;
 /* return TRUE if s2 is contained in s1 */
	if(!s1 || !s2){
		return FALSE;
	}
	for(tmp = s1;*tmp;tmp++){
		for(tmp2 = s2;*tmp2;tmp2++){
			if(*tmp != *tmp2){
				break;
			}
		}
		if(!*tmp2){
			return(TRUE);
		}
	}
	return(FALSE);
}
#endif COMMENT

#ifndef TERMCAP
tgetent()
{
gerror("tgetent");/* doesn't exist so shouldn't be called */
}
tgetnum()
{
	tgetent();
}
#endif

#ifdef COMMENT
  void
hline (c, lin, col1,col2)
  char c;
  med lin, col1,col2;
{
  med count;

        if (iswindow) {
                sunhline (lin, col1, col2);
                return;
        }
        if (c == ' ') {
                if (currentwindow->windowwidth == (screenwidth)) {
                        gotoxy (col1, lin);
                        eraseol();
                        return;
                } else if (currentwindow->lm == 0){
                        gotoxy (currentwindow->windowwidth - 1, lin);
                        erasebol();
                        return;
                }
        }
        gotoxy (col1, lin);
        for(count = col2-col1+1;count--;putf("%c",c));
}

  void
vline (c, lin1,lin2, col)
  char c;
  med lin1,lin2, col;
{
/*
  med count;
        if (iswindow) {
                sunvline (lin1, lin2-1, col);
                return;
        }
        for(count = lin1;count <= lin2;count++){
                gotoxy (col, count);
                putf("%c",c);
        }
*/
  register i,y;
  med height;

	height = lin2-lin1;
	mvcur(lin1,col,lin1,col-2);

	for (i=0,y=lin1; i < height; i++,y++) {
		putchar('|');
		mvcur(y,col-1,y+1,col-2);
	}
	mvcur(y,col,lin1,col);
}
#endif COMMENT

 void
freecurseswindow(w)
WINDOW *w;
{
	delwin(w);
}
