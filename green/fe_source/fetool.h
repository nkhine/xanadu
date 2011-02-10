/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:      fetool.h
***************************************************/

#ifdef GRAPHICS
#ifdef SUNGRAPHICS
#include <suntool/gfx_hs.h>
#include <suntool/tool_hs.h>
#include <suntool/msgsw.h>
#include <suntool/optionsw.h>

/*#ifndef major
 #include <sys/types.h> 
 #endif   
*/
/*#include <sys/stat.h>
#include <sys/dir.h>
*/
struct tool *tool;
struct toolsw *msgsw, *optsw, *esw;
/*struct pixfont *font;*/
struct pixwin *pw;
nat  start_proc(), opt_init(), draw_proc(), mesg_proc(), quit_proc(), 
        sigwinched();
#include <suntool/menu.h>
struct pixwin *mypixwin;
struct gfxsubwindow *gfx;
#endif
#endif

#ifdef UNIX_BSD4_2
#include <sgtty.h>
#endif

#define NORMAL        0
#define BLINK         1
#define REVERSEVIDEO  2
#define HIGHINTENSITY 4
#define UNDERLINE     8
#define STANDOUT      32
#define STANDOUTEND   64

char *lastchr(), *namefrom(), *fname(), *galloc();
/*
#ifndef UNIX_BSD_4_2 // this was ifndef5  zzzz 12/28/84//
struct sgttyb ioctl_sgttyb;
struct tchars ioctl_tchars;
struct ltchars ioctl_ltchars;
#endif
  */ 

#ifdef GRAPHICS
#ifdef SUNGRAPHICS
nat fontx,fonty;
nat graphx,graphy;
nat mywindowfd;
/*nat SCROLLAMOUNT;*/
#endif
#endif 
extern  struct menuitem *menu_display();
extern med boldmode,normalmode;
extern char ioctl_exit,ioctl_delarg,ioctl_backspace;   /*exit delarg backspace*/
extern unsigned/*really bool*/ iswindow;
#ifdef MAINDECLARATION
struct menuitem *menu_display();
med boldmode,normalmode;
char ioctl_exit,ioctl_delarg,ioctl_backspace;   /*exit delarg backspace*/
unsigned/*really bool*/ iswindow;
#endif
