/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:        menu.d
***************************************************/

#include "fest.h"
#include "fekeys.h"

/* NOTE menus use characterdisplacementwithin view for *line* displacement */ 

  med
selectmenu (menuline, menucol, nitems, items) 
                   /*change internally to make menu be on screen*/
  med menuline;
  med menucol;
  med nitems;
  char *items[];
{
  med item;
  bool scrollmenu();

/*
 fprintf(stderr,"selectmenu\n");
*/
        if (!nitems)
                gerror ("No items in menu call.\n");   
        if (nitems == 1)
                return(1); /*why ask ? */

        initmenu (menuline, menucol, nitems, items);
        while (scrollmenu (nitems, items));
        item = currentwindow->rcursl + (med)currentwindow->windowview.characterdisplacementwithinview;
        returnfromstack (FALSE);
        return (item);
}

  void
initmenu (menuline, menucol, nitems, items)
  med menuline, menucol;
  med nitems;
  char *items[];
{
  windowtype *windowalloc();
  windowtype *menu, *previouswindow;
  med i;
/*
 fprintf(stderr,"initmenu\n");
*/
        previouswindow = currentwindow;
        pushwindow (menu = windowalloc ());

        for (menu->windowwidth = 0, i = 0; i < nitems; i++){   
                menu->windowwidth = max (menu->windowwidth, strlen (items[i]));
        }

        menu->windowwidth = min (menu->windowwidth, screenwidth -3);
        menu->windowheight = min(nitems, screenheight - 3 -2/*2 for prompts*/);
        menu->tm = min(menuline+1,screenheight -menu->windowheight -3);
        menu->lm = min(menucol,screenwidth -menu->windowwidth-3);

	menu->w = newwin(menu->windowheight,menu->windowwidth,menu->tm,menu->lm);
	keyset(menu->w);
	menu->border = NULL;

/*
	menu->windowwidth = min (menu->windowwidth, screenwidth - menucol);
        menu->windowheight = min (nitems, screenheight - menuline - 1);
        menu->tm = menuline;
        menu->lm = menucol;
*/
        for (i = menu->tm-1; i <= menu->tm + menu->windowheight; i++) {
                if (i >= previouswindow->tm
		&& i <= previouswindow->tm + previouswindow->windowheight) {
/*
                        previouswindow->windowline_glorpfed[i-previouswindow->tm] = TRUE;
*/
			touchwin(previouswindow->w);
		}
        }
        drawboxaroundwindow (currentwindow);
        fillmenu (nitems, items);
}

  void
fillmenu (nitems, items)
  med nitems;
  char *items[];
{
/*
 fprintf(stderr,"fillmenu\n");
*/
        currentwindow->windowview.characterdisplacementwithinview = 0;
        fillmenuwindow (nitems, items);
        homecursorposition ();
        selectmenuline (items);
}

  void
fillmenuwindow (nitems, items)
  med nitems;
  char *items[];
{
  long i;
  long d;
/*
 fprintf(stderr,"fillmenuwindow\n");
*/
        d = currentwindow->windowview.characterdisplacementwithinview;
        for (i = d; i < min (nitems, currentwindow->windowheight+d); i++) {
                placecursorinwindow (currentwindow, 0,(med)(i-d));
                clearlineinwindow (currentwindow,0,(med)(i-d));

					     /* change here qqq for sun compatible menus */

                plotntextcharactersinwindow (currentwindow, items[(med)i],
					     currentwindow->windowwidth, (med)(i-d), 0); 
        }
}

  bool
scrollmenu (nitems, items)
  med nitems;
  char *items[];
{
  metachar getmenucontrol();
  metachar c;

        switch (c = getmenucontrol ()) {
	  case FLINK:
		return(FALSE);
          case CUP:
                scrollmenuup (nitems, items);
                break;
          case CDOWN:
                scrollmenudown (nitems, items);
                break;
/*          case CTABDOWN:
                tabmenudown (nitems, items);
                break;
          case CTABUP:
                tabmenuup (nitems, items);
                break;
*/          case HOMEUP:
                homemenuup (nitems, items);
                break;
          case HOMEDOWN:
                homemenudown (nitems, items);
                break;
          case PREVPAGE:
                fillmenu (nitems, items);
                break;
          case FILEBOT:
                menubottom (nitems, items);
        }
        return (c != ' ' &&(c != CR && c != NL));
}

  void
scrollmenuup (nitems, items)
  med nitems;
  char *items[];
{
        if (currentwindow->rcursl > 0) {
                deselectmenuline (items);
                --currentwindow->rcursl;
        } elif (currentwindow->windowview.characterdisplacementwithinview) {
                --currentwindow->windowview.characterdisplacementwithinview;
                fillmenuwindow (nitems, items);
                homecursorposition ();
        }
        selectmenuline (items);
        putcursor ();
}

  void
scrollmenudown (nitems, items)
  med nitems;
  char *items[];
{
        deselectmenuline (items);
        if (currentwindow->rcursl < currentwindow->windowheight - 1) {
                ++currentwindow->rcursl;
        } elif (currentwindow->rcursl + currentwindow->windowview.characterdisplacementwithinview < nitems - 1)  {
                ++currentwindow->windowview.characterdisplacementwithinview;
                fillmenuwindow (nitems, items);
                homedowncursorposition ();
        }
        selectmenuline (items);
        putcursor ();
}

  void
homemenuup (nitems, items)
  med nitems;
  char *items[];
{
        deselectmenuline (items);
        homecursorposition ();
        selectmenuline (items);
        putcursor ();
}

  void
homemenudown (nitems, items)
  med nitems;
  char *items[];
{
        deselectmenuline (items);
        homedowncursorposition ();
        selectmenuline (items);
        putcursor ();
}

  void
menubottom (nitems, items)
  med nitems;
  char *items[];
{
        currentwindow->windowview.characterdisplacementwithinview = nitems - currentwindow->windowheight;
        fillmenuwindow (nitems, items);
        homedowncursorposition ();
        selectmenuline (items);
        putcursor ();
}

  metachar
getmenucontrol ()
{
  metachar gettypeahead();
  metachar menuerror();
  metachar c;

        c = gettypeahead ();
        loop {
                if (c == CUP || c == CDOWN || c == HOMEUP|| c == FLINK/* zzz*/
                 || c == HOMEDOWN || c == FILETOP || c == PREVPAGE
                 || c == FILEBOT || c == ' ' || c == NL || c == CR
                 /*|| c == CTABDOWN || c == CTABUP*/)
                        break;
                c = menuerror ("Not menu control character.");
        }
        return (c);
}

  void
selectmenuline (items)
  char *items[];

{
  med item;
/*
 fprintf(stderr,"selectmenuline\n");
*/
        item = (med)currentwindow->windowview.characterdisplacementwithinview
             + currentwindow->rcursl;
        changedisplayattributes (REVERSEVIDEO);
        plotntextcharactersinwindow (currentwindow, items[item], currentwindow->windowwidth, currentwindow->rcursl, 0);
        changedisplayattributes (NORMAL);
	gotoxy(currentwindow->windowwidth-1,currentwindow->rcursl);
	wrefresh(currentwindow->w);
}

/*makemenulineblink (selection, items)
  med selection;
  char *items[];

{
        changedisplayattributes (BLINK|REVERSEVIDEO);
        plotntextcharactersinwindow (currentwindow, items[selection], currentwindow->windowwidth, currentwindow->rcursl, 0);
        changedisplayattributes (NORMAL);
}*/

  void
deselectmenuline (items)
  char *items[];
{
  med item;

        item = (med)currentwindow->windowview.characterdisplacementwithinview
             + currentwindow->rcursl;
        changedisplayattributes (NORMAL);
        plotntextcharactersinwindow (currentwindow, items[item], currentwindow->windowwidth, currentwindow->rcursl, 0);
}

  metachar
menuerror (msg)
  char *msg;
{
  metachar gettypeahead();
  metachar c;

        gotoxy (0, screenheight-1);
        changedisplayattributes (NORMAL);
        putf ("%C%s",(nat)0,(nat)(screenheight-1), msg);
        c = gettypeahead ();
        gotoxy (0, screenheight-1);
        eraseol ();
        return (c);
}
