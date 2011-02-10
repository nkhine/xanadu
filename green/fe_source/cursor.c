/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:      cursor.d
***************************************************/

#include "fest.h"
#include "feminreq.h"
#include "fekeys.h"

#define ESC 0x1b

  bool
iscursorcontrol(input)
  metachar input;
{
        switch (input) {
          case  CUP:
                return(TRUE);
          case  CDOWN:
                return(TRUE);
          case  CLEFT:
                return(TRUE);
          case  CRIGHT:
                return(TRUE);
          case  TAB:
                return(TRUE);
          case  RTAB:
                return(TRUE);
          case  HOMEUP:
                return(TRUE);
          case  HOMEDOWN:
                return(TRUE);
          case  FILETOP:
                return(TRUE);
          case  FILEBOT:
                return(TRUE);
          case  SCREENMID:
                return(TRUE);
          case  NEXTPAGE:
                return(TRUE);
/*
          case  PREVPAGE:
                return(TRUE);
*/
          case  CTABUP:
                return(TRUE);
          case CTABDOWN:
                return(TRUE);
          default:
                return(FALSE);
        }
        return(FALSE);
}

  void
accumulatecursorcontrol (requestptr)
  requesttype *requestptr;
{
  metachar peektypeahead();
  metachar gettypeahead(),c;
  void zerooutcursorcontrol();
  void fixrequeststruct();
  void incrementrequeststruct();

        zerooutcursorcontrol (requestptr);
        for (; istypeahead () && iscursorcontrol (peektypeahead ());) {
                c = gettypeahead ();
                incrementrequeststruct (c, requestptr);
                fixrequeststruct (requestptr);
        }
/*
printf("scroll %d lines",requestptr->requestnumberoflines);
printf("leaving accumulatecursorcontrol\n");
*/
}


  void
zerooutcursorcontrol (requestptr)
  requesttype *requestptr;
{
        tumblerclear (&requestptr->requestvposition);
        requestptr->requestnumberoflines = 0;
        requestptr->requestcursordeltax = 0;
        requestptr->requestcursordeltay = 0;
        requestptr->requestnumberofcharacters = 0;
}

  void
fixrequeststruct (requestptr)  /*wrap cursor around edges & etc*/
  requesttype *requestptr;
{
  long dy, dx;
  long cy, cx;
  med height, width;
  long lines;

        dy = requestptr->requestcursordeltay;
        dx = requestptr->requestcursordeltax;
        cy = currentwindow->rcursl;
        cx = currentwindow->rcursc;
        width = currentwindow->windowwidth;
        height = currentwindow->windowheight;
        lines = requestptr->requestnumberoflines;

        if(dx > 0) {
                dy += (cx + dx) / width;
        } elif (dx < 0) {
                dy -=( (cx + dx) < 0 );
        }

        if (dx > 0) {
                dx = (cx + dx) % width - cx;
        } elif (dx + cx < 0) {
                dx = dx + width;
        }

        if (cy + dy >= height) {
                lines += dy + cy - height +1;
                dy = height - cy-1;		/* i.e. at bottom of screen */
        } elif (cy + dy < 0) {
                lines += dy;
                dy = -cy;			 /* i.e. at top of screen */
        }

        if (cy + dy < 0) {
                lines += dy + cy;
                dy = 1;
        }

        requestptr->requestcursordeltay = dy;
        requestptr->requestcursordeltax = dx;
        currentwindow->rcursl = cy;
        currentwindow->rcursc = cx;
        currentwindow->windowwidth = width;
        currentwindow->windowheight = height;
        requestptr->requestnumberoflines = lines;
}

  void
incrementrequeststruct (c, requestptr)
  metachar c;
  requesttype *requestptr;
{
  metachar peektypeahead();
  metachar gettypeahead();
  tdigit temp;
  spectype *specptr, *specalloc();

        switch (c) {
          case  CUP:
                requestptr->requestcursordeltay --;
                return;
          case  CDOWN:
                requestptr->requestcursordeltay++;
                return;
          case  CLEFT:
                requestptr->requestcursordeltax--;
                return;
/*          case  FILEBOT:  for now to get around ^S */
          case  CRIGHT:
                requestptr->requestcursordeltax++;
                return;
          case  TAB:  /*zzz these tabs arent right cuse cursc isnt up to date*/
                requestptr->requestcursordeltax += TABSIZE
                 -((currentwindow->rcursc /*-1*/
                 + requestptr->requestcursordeltax)%currentwindow->windowwidth)
                 % TABSIZE;
                return;
          case  RTAB:
                requestptr->requestcursordeltax -= ( (currentwindow->rcursc -1
                 +requestptr->requestcursordeltax)
                 % currentwindow->windowwidth)  % TABSIZE +1 ;

                return;

          case  CTABUP:
                requestptr -> requestcursordeltay -= 10;
                return;

          case  CTABDOWN:
                requestptr -> requestcursordeltay += 10;
                return;

          case  HOMEUP:

                requestptr->requestcursordeltay = -currentwindow->rcursl;
                requestptr->requestcursordeltax = -currentwindow->rcursc;
                return;
          case  HOMEDOWN:
                
                requestptr->requestcursordeltay = -currentwindow->rcursl+currentwindow->windowheight-1;
                requestptr->requestcursordeltax = -currentwindow->rcursc;
                return;
          case  FILETOP:
/*          case  PREVPAGE:*/

                zerooutcursorcontrol (requestptr);
                requestptr->requestcursordeltay  = /*1*/-currentwindow->rcursl;
                requestptr->requestcursordeltax = /*1*/-currentwindow->rcursc;
                if (currentwindow->typeofwindow == JUMPLINK) {
                        specptr = specalloc();
                        findspecfromcursorposition (specptr);
                } else {
                   copyspecset(currentwindow->windowview.viewspecptr,&specptr);
                }

                movetumbler (&specptr->docid,&requestptr->requestvposition);
                specfree (specptr);
                temp = findlastplace (&requestptr->requestvposition);
                requestptr->requestvposition.mantissa[temp+2] = 1;
                requestptr->requestvposition.mantissa[temp+3] = 1; 
/*
dumptumbler(&requestptr->requestvposition);
*/
                return;
/*        case  FILEBOT:           
                filebot();
                homedown();
                refstuff(TRUE); 
                return;
*/
          case  SCREENMID:
                requestptr->requestcursordeltay = -currentwindow->rcursl+
                 (currentwindow->windowheight)/2+1;
                return;
          case  NEXTPAGE:
                requestptr->requestnumberoflines += currentwindow->windowheight;
                return;
/*
          case  PREVPAGE:
                requestptr->requestnumberoflines -= currentwindow->windowheight;
                return;
*/
          default:
                return;
        }
}
