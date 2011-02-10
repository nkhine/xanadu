/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:  menuwindow.d
***************************************************/

#define GRAPHICS

#include <stdio.h>
#include <ctype.h>
#include "feconfig.h"
#include "fetypealias.h"

#ifdef SUNGRAPHICS
#include <suntool/gfx_hs.h>
#include <suntool/tool_hs.h>
#include <suntool/msgsw.h>
#include <suntool/optionsw.h>
#include <suntool/menu.h>
#endif

#include "festbot.h"
#include "feminreq.h"
/*#include  "klugeddcls.h"*/

extern bool iswindow;
#define WAITFORBACKEND
extern FILE *fromxu, *toxu;
#ifdef SUNGRAPHICS
struct menu weirdoptions_body;
struct menu menu_body;
struct menu constructedmenu_body;
struct menuitem constructedmenu_items [30];
extern  struct menuitem *menu_display();
struct gfxsubwindow *gfx;
#endif
void fixglorpfsincurrentwindow();
void setdisplaystyle();
void showkeydefs();
void followlink();
void createlink();
void makecut();
void clearcuts();
void rearrange();
void changefromset();
void changetoset();
void cleartempendsets();
void compare();
void delete();

void followlink();
void createversion();
void createdocument();
void paralleltextface();
void quit();
med menustate;
nat mywindowfd;


  void  
initmenuarrays()
{

#ifdef SUNGRAPHICS
	menu_ptr[0] = &menu_body;
	menu_ptr[1] = &weirdoptions_body;
#endif

        weirdoptionsarray[0] =  fixglorpfsincurrentwindow;
        weirdoptionsarray[1] =  (void (*) ())setdisplaystyle;
        weirdoptionsarray[2] =  (void (*) ())showkeydefs;
        weirdoptionsarray[3] =  (void (*) ())compare;
        weirdoptionsarray[4] =  (void (*) ())followlink;
        weirdoptionsarray[5] =  (void (*) ())createlink;
        weirdoptionsarray[6] =  (void (*) ())makecut;
        weirdoptionsarray[7] =  (void (*) ())clearcuts;
        weirdoptionsarray[8] =  (void (*) ())delete;
        weirdoptionsarray[9] =  (void (*) ())rearrange;
        weirdoptionsarray[10] = (void (*) ())createdocument;
        weirdoptionsarray[11] = (void (*) ())changefromset;
        weirdoptionsarray[12] = (void (*) ())changetoset;
        weirdoptionsarray[13] = (void (*) ())cleartempendsets;
        weirdoptionsarray[14] = (void (*) ())createversion;
	

	firstmenuarray[0] =  (void (*) ()) followlink;
	firstmenuarray[1] =  (void (*) ()) paralleltextface;
	firstmenuarray[2] =  (void (*) ()) createversion;
	firstmenuarray[3] =  (void (*) ()) createdocument;
	firstmenuarray[4] =  (void (*) ()) quit;

	displaystylearray[0] = QUARTERDISPLAY;
	displaystylearray[1] = QUARTERDISPLAY;
	displaystylearray[2] = SPLITVERTICAL;
	displaystylearray[3] = SPLITHORIZONTAL;
	displaystylearray[4] = FULLDISPLAY;
	displaystylearray[5] = RANDOMDISPLAY;
	displaystylearray[6] = EXPERIMENTALDISPLAY;
	displaystylearray[7] = ILLEGALDISPLAY;
}
  void
makemenu(femenuptr,choice0,ndocs,title)
  char ** femenuptr;
  nat choice0;
  med ndocs;
  char title[];
{
#ifdef SUNGRAPHICS
  med i;
	if(iswindow){
		normal_cursor(FALSE);
	}
	menustate = CONSTRUCTEDMENUSTATE;
	if(ndocs >30){
		ndocs = 30;
	}
	constructedmenu_body.m_imagetype = MENU_IMAGESTRING;
	constructedmenu_body.m_imagedata = title;
	constructedmenu_body.m_itemcount = ndocs ;
	constructedmenu_body.m_items = constructedmenu_items ;
	constructedmenu_body.m_next =NULL ;
	constructedmenu_body.m_data = NULL;
	for(i=0;i<ndocs;i++){
		constructedmenu_items[i].mi_imagetype  = MENU_IMAGESTRING;
		constructedmenu_items[i].mi_imagedata  = femenuptr[i];
		constructedmenu_items[i].mi_data  = (caddr_t)choice0+i;
	}
#endif
}
