/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:        putf.d
***************************************************/

#include "feconfig.h"
/*#include <stdio.h>*/
#include "fetypealias.h"
#include "fest.h"

#define NDIG 7
#define NULL 0
/*
** output a formatted message
*/

#ifdef AMIGA
#include "xvarargs.h"
#else
#include <varargs.h>
#endif

  
    void
putf (s,va_alist)
  char *s;
  va_dcl
{
  
  char *cp, **p;
  int c, n, x, y;
  char nbuf[NDIG];
  va_list vargptr;
  char foo;
  WINDOW *win;

	win=currentwindow->w;
        va_start(vargptr);  

/*
 fprintf(stderr,"in putf s = %p \n",s);
 fprintf(stderr,"in putf *s = %s !\n",s);
*/

        for(;;s++){
                if (*s == '\0')
                        return;
                if(*s != '%'){
                        waddch(win,*s);
                        continue;
                }
                switch(c = *++s){
                  case 'C': /* position cursor, eat two arguments */
                        x = va_arg(vargptr,int);
                        y = va_arg(vargptr,int);
/*fprintf(stderr,"in putf case C  = %x y = %x \n",x,y);
*/
                        gotoxy(x,y);
                        break;
                  case 'c':  
                        foo = va_arg(vargptr,int); 
                        waddch(win,foo);
                        break;
                  case 'x':
                        n = va_arg(vargptr,int);
                        if( n < 0) {
                                waddch(win,'-');
                                n = -n;
                        }
                        cp = &nbuf[NDIG];
                        do{
                                *--cp = (n &0xf) + '0';
                                n >>= 4;
                                n &= 017777; /* prevent sign propogation */
                        } while (n != 0);
                        while (cp < &nbuf[NDIG])
                                waddch(win,*cp++);
                        break;
                  case 'o':
                        n = va_arg(vargptr,int);
                        if( n < 0) {
                                waddch(win,'-');
                                n = -n;
                        }
                        cp = &nbuf[NDIG];
                        do{
                                *--cp = (n &07) + '0';
                                n >>= 3;
                                n &= 017777; /* prevent sign propogation */
                        } while (n != 0);
                        while (cp < &nbuf[NDIG])
                                waddch(win,*cp++);
                        break;
                  case 'd':
                        n = va_arg(vargptr,int); 
                  /*      vargptr += sizeof(int);
                        n = ((int *)(vargptr ))[-1]; */
                      /*  n = ((int *)(vargptr += sizeof(int)))[-1];*/
/*fprintf(stderr,"in putf case d n = %d\n",n);
*/
                        if (n<0){
                                waddch(win,'-');
                                n = -n;
                        }
                        cp = &nbuf[NDIG];
                        do{
                                *--cp = (n % 10) + '0';
                                n /= 10;
                        } while (n != 0);
                        while (cp < &nbuf[NDIG])
                                waddch(win,*cp++);
                        break;
                  case 's':
                        cp = va_arg(vargptr,char*);
                        while (cp != NULL && (c = *cp++))
                                if(TRUE||(c >= ' ' && c <= 0177))
                                        waddch(win,c);
                                else
                                        waddch(win,'^');
                        break;
                  default:
                        waddch(win,c);
                        break;
                }
        }
        va_end(vargptr);
}
