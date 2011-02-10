/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:    maintool.d
***************************************************/

#define MAINDECLARATION

#include <signal.h>
#include "feconfig.h" 
#include <stdio.h>    
#include <ctype.h>
#include <suntool/gfx_hs.h>
#include <suntool/tool_hs.h>
#include <suntool/msgsw.h>
#include <suntool/optionsw.h>
#include <suntool/menu.h>
#include "festbot.h"
#include "feminreq.h"

#define WAITFORBACKEND 
unsigned iswindow;
FILE *fromxu, *toxu;

extern  struct menuitem *menu_display();
struct gfxsubwindow *gfx;
 struct menu weirdoptions_body ;
struct menu constructedmenu_body;
/* Menu Definition */
  struct menuitem menu_items[] = {
        {MENU_IMAGESTRING,      "Follow Link",        (caddr_t)FIRSTMENUCHOICE0+0},
        {MENU_IMAGESTRING,      "Parallel textface",  (caddr_t)FIRSTMENUCHOICE0+1},
        {MENU_IMAGESTRING,      "Create New version", (caddr_t)FIRSTMENUCHOICE0+2},
        {MENU_IMAGESTRING,      "Create New Document",(caddr_t)FIRSTMENUCHOICE0+3},
        {MENU_IMAGESTRING,      "Quit",  (caddr_t) FIRSTMENUCHOICE0+4},
};
  struct menu menu_body = {
        MENU_IMAGESTRING, "command",
        sizeof(menu_items) / sizeof(struct menuitem), menu_items,
        (struct menu *)&weirdoptions_body/*NULL*/, (caddr_t)NULL
};
  struct menu *menu_ptry= &constructedmenu_body;
static  struct menu *menu_ptrx= &menu_body;

struct tool *tool;
struct toolsw *msgsw, *optsw, *esw;
/*struct pixfont *font;*/
nat mywindowfd;
caddr_t osw, items[10];
bool issfe = FALSE;
nat menustate = FIRSTMENUSTATE;
nat fontx,fonty;
nat rcursc,rcursl;
main(argc, argv)
        nat argc;
        char **argv;
{
        nat     canvas_selected();
        struct  inputmask im;

        /* Initialization */
        if ((gfx = gfxsw_init(0, argv)) == NULL) {
                fprintf(stderr, "Unable to open graphics subwindow.\n");
                exit(1);
        }
        input_imnull(&im);
        im.im_flags |= IM_ASCII | IM_NEGEVENT;
        win_setinputcodebit(&im, MENU_BUT);
        win_setinputcodebit(&im, SELECT_BUT);
        gfxsw_setinputmask(gfx,
            &im, (struct inputmask *)NULL, WIN_NULLLINK, 1, 1);
        gfxsw_getretained(gfx);
        mumbleinit(argc,argv);
        /* Notification Manager */
        gfxsw_select(gfx, canvas_selected, 0, 0, 0, (struct timeval *)NULL);
 
        /* Cleanup */
        gfxsw_done(gfx);
        mumblequit();
}

/* Notification Handling */
  void
canvas_selected(gfxa, ibits, obits, ebits, timer)
        struct  gfxsubwindow *gfxa;
        nat             *ibits, *obits, *ebits;
        struct  timeval **timer;
{
        struct  menuitem *mi;
        struct  inputevent ie;
	nat wasbutton = FALSE;
	nat x,y;
        
        if (*ibits & (1 << gfxa->gfx_windowfd)) {
                if (input_readevent(gfxa->gfx_windowfd, &ie)) {
                        perror("canvasinput");
                        exit(1);
                }
		if(ie.ie_code == MENU_BUT){
			wasbutton = TRUE;
		}else if (ie.ie_code == SELECT_BUT&&win_inputposevent(&ie)){
			getcursor(&x,&y);/* clear cursor as side effect*/
			x = ie.ie_locx/fontx-currentwindow->lm;
			y = ie.ie_locy/fonty-currentwindow->tm;
			if (x<0||x>= currentwindow->windowwidth)
				return;
			if (y<0||y >= currentwindow->windowheight)
				return;
			placecursor(x,y);
			return;
		}else if (ie.ie_code == SELECT_BUT){
			return;
		}
                if (ie.ie_code == MENU_BUT && win_inputposevent(&ie) &&
                  (mi=menu_display(((menustate == CONSTRUCTEDMENUSTATE) ?&(menu_ptry) :&(menu_ptrx)),&ie,gfxa->gfx_windowfd))){
                      ie.ie_code = (short) mi->mi_data;
		}
        switch (ie.ie_code) {
		case WEIRDOPTIONCHOICE0+0:
			menustate = FIRSTMENUSTATE;
			(*weirdoptionsarray[0])();
			break;
		case WEIRDOPTIONCHOICE0+1:
			menustate = FIRSTMENUSTATE;
			(*weirdoptionsarray[1])();
			break;
		case WEIRDOPTIONCHOICE0+2:
			menustate = FIRSTMENUSTATE;
			(*weirdoptionsarray[2])();
			break;
		case WEIRDOPTIONCHOICE0+3:
			menustate = FIRSTMENUSTATE;
			(*weirdoptionsarray[3])();
			break;
		case WEIRDOPTIONCHOICE0+4:
			menustate = FIRSTMENUSTATE;
			(*weirdoptionsarray[4])();
			break;
		case WEIRDOPTIONCHOICE0+5:
			menustate = FIRSTMENUSTATE;
			(*weirdoptionsarray[5])();
			break;
		case WEIRDOPTIONCHOICE0+6:
			menustate = FIRSTMENUSTATE;
			(*weirdoptionsarray[6])();
			break;
		case FIRSTMENUCHOICE0+0:
			(*firstmenuarray[0])();
			break;
		case FIRSTMENUCHOICE0+1:
			(*firstmenuarray[1])();
			break;
		case FIRSTMENUCHOICE0+2:
			(*firstmenuarray[2])();
			break;
		case FIRSTMENUCHOICE0+3:
			(*firstmenuarray[3])();
			break;
		case FIRSTMENUCHOICE0+4:
			(*firstmenuarray[4])();
			break;
		case DISPLAYOPTIONSCHOICE0 + 0:
			displaystyle = displaystylearray[0];
			break;
		case DISPLAYOPTIONSCHOICE0 + 1:
			displaystyle = displaystylearray[1];
			break;
		case DISPLAYOPTIONSCHOICE0 + 2:
			displaystyle = displaystylearray[2];
			break;
		case DISPLAYOPTIONSCHOICE0 + 3:
			displaystyle = displaystylearray[3];
			break;
		case DISPLAYOPTIONSCHOICE0 + 4:
			displaystyle = displaystylearray[4];
			break;
		case DISPLAYOPTIONSCHOICE0 + 5:
			displaystyle = displaystylearray[5];
			break;
		case DISPLAYOPTIONSCHOICE0 + 6:
			displaystyle = displaystylearray[6];
			break;
		case DISPLAYOPTIONSCHOICE0 + 7:
			displaystyle = displaystylearray[7];
			break;
		case DOCUMENTMENUCHOICE0 +0:
		case DOCUMENTMENUCHOICE0 +1:
		case DOCUMENTMENUCHOICE0 +2:
		case DOCUMENTMENUCHOICE0 +3:
		case DOCUMENTMENUCHOICE0 +4:
		case DOCUMENTMENUCHOICE0 +5:
		case DOCUMENTMENUCHOICE0 +6:
		case DOCUMENTMENUCHOICE0 +7:
		case DOCUMENTMENUCHOICE0 +8:
		case DOCUMENTMENUCHOICE0 +9:
		case DOCUMENTMENUCHOICE0 +10:
		case DOCUMENTMENUCHOICE0 +11:
		case DOCUMENTMENUCHOICE0 +12:
		case DOCUMENTMENUCHOICE0 +13:
		case DOCUMENTMENUCHOICE0 +14:
		case DOCUMENTMENUCHOICE0 +15:
		case DOCUMENTMENUCHOICE0 +16:
		case DOCUMENTMENUCHOICE0 +17:
		case DOCUMENTMENUCHOICE0 +18:
		case DOCUMENTMENUCHOICE0 +19:
		case DOCUMENTMENUCHOICE0 +20:
			menustate = FIRSTMENUSTATE;
			compare2(ie.ie_code  -DOCUMENTMENUCHOICE0);
			break;
		case LINKTYPEMENUCHOICE0 +0:
		case LINKTYPEMENUCHOICE0 +1:
		case LINKTYPEMENUCHOICE0 +2:
		case LINKTYPEMENUCHOICE0 +3:
			maketempthreeset(ie.ie_code - LINKTYPEMENUCHOICE0);
			menustate = FIRSTMENUSTATE;
			break;
		case LINKMENUCHOICE0 + 0:
		case LINKMENUCHOICE0 + 1:
		case LINKMENUCHOICE0 + 2:
		case LINKMENUCHOICE0 + 3:
		case LINKMENUCHOICE0 + 4:
		case LINKMENUCHOICE0 + 5:
		case LINKMENUCHOICE0 + 6:
		case LINKMENUCHOICE0 + 7:
		case LINKMENUCHOICE0 + 8:
		case LINKMENUCHOICE0 + 9:
		case LINKMENUCHOICE0 + 10:
		case LINKMENUCHOICE0 + 11:
		case LINKMENUCHOICE0 + 12:
		case LINKMENUCHOICE0 + 13:
		case LINKMENUCHOICE0 + 14:
		case LINKMENUCHOICE0 + 15:
		case LINKMENUCHOICE0 + 16:
		case LINKMENUCHOICE0 + 17:
		case LINKMENUCHOICE0 + 18:
		case LINKMENUCHOICE0 + 19:
		case LINKMENUCHOICE0 + 20:
			interactlinkset2(ie.ie_code -LINKMENUCHOICE0);
			menustate = FIRSTMENUSTATE;
			break;

		case JUMPMENUCHOICE0 +0:
		case JUMPMENUCHOICE0 +1:
		case JUMPMENUCHOICE0 +2:
		case JUMPMENUCHOICE0 +3:
		case JUMPMENUCHOICE0 +4:
			changedocument2(ie.ie_code - JUMPMENUCHOICE0);
			menustate = FIRSTMENUSTATE;
			break;
		case STACKRETURNCHOICE0 + 0:
		case STACKRETURNCHOICE0 + 1:
		case STACKRETURNCHOICE0 + 2:
		case STACKRETURNCHOICE0 + 3:
		case STACKRETURNCHOICE0 + 4:
		case STACKRETURNCHOICE0 + 5:
		case STACKRETURNCHOICE0 + 6:
		case STACKRETURNCHOICE0 + 7:
		case STACKRETURNCHOICE0 + 8:
		case STACKRETURNCHOICE0 + 9:
		case STACKRETURNCHOICE0 +10:
		case STACKRETURNCHOICE0 +11:
		case STACKRETURNCHOICE0 +12:
		case STACKRETURNCHOICE0 +13:
		case STACKRETURNCHOICE0 +14:
		case STACKRETURNCHOICE0 +15:
		case STACKRETURNCHOICE0 +16:
		case STACKRETURNCHOICE0 +17:
		case STACKRETURNCHOICE0 +18:
		case STACKRETURNCHOICE0 +19:
		case STACKRETURNCHOICE0 +20:
		case STACKRETURNCHOICE0 +21:
		case STACKRETURNCHOICE0 +22:
		case STACKRETURNCHOICE0 +23:
		case STACKRETURNCHOICE0 +24:
		case STACKRETURNCHOICE0 +25:
		case STACKRETURNCHOICE0 +26:
		case STACKRETURNCHOICE0 +27:
		case STACKRETURNCHOICE0 +28:
		case STACKRETURNCHOICE0 +29:
		case STACKRETURNCHOICE0 +30:
		case STACKRETURNCHOICE0 +31:
		case STACKRETURNCHOICE0 +32:
		case STACKRETURNCHOICE0 +33:
		case STACKRETURNCHOICE0 +34:
		case STACKRETURNCHOICE0 +35:
		case STACKRETURNCHOICE0 +36:
		case STACKRETURNCHOICE0 +37:
		case STACKRETURNCHOICE0 +38:
		case STACKRETURNCHOICE0 +39:
		case STACKRETURNCHOICE0 +40:
			returnfromstack2(ie.ie_code - STACKRETURNCHOICE0);
			menustate = FIRSTMENUSTATE;
			break;
		case 'N'-'@':
			menustate = WEIRDMENUSTATE;
			break;
                /*case 'v':
                        pw_vector(gfx->gfx_pixwin, 5, 5, 5, 100, PIX_SET, 0);
                        break;
                case 's':
                        pw_writebackground(gfx->gfx_pixwin,
                            25, 25, 75, 75, PIX_SET);
                        break;
                case 't':
                        pw_text(gfx->gfx_pixwin, 5, 125, PIX_SRC,
                            (struct pixfont *)NULL,
                            "This is a string written with pw_text.");
                        break;
                case 'c':
                        pw_writebackground(gfx->gfx_pixwin, 0, 0,
                            gfx->gfx_rect.r_width, gfx->gfx_rect.r_height,
                            PIX_CLR);
                        break;
               /* case 'q':
                        gfxsw_selectdone(gfx);
                        break;*/
		*/
                default:
                        gfxsw_inputinterrupts(gfx, &ie);
                }
		if(!wasbutton){
			mumble(0xff&ie.ie_code);
		}
        }
	if(menustate == FIRSTMENUSTATE){
		normal_cursor(TRUE);
	}
        *ibits = *obits = *ebits = 0;
}
  void
clearscreen()
{
                pw_writebackground(gfx->gfx_pixwin, 0, 0,
                    gfx->gfx_rect.r_width, gfx->gfx_rect.r_height, PIX_CLR);
}

tumbler docvposition;
requesttype request;
  void
mumbleinit (argc, argv)
  nat argc;
  char *argv[];
{
	normal_cursor(TRUE);
        clearscreen();

        displaystyle = QUARTERDISPLAY;
        initterm();
        testmargins = FALSE;
        srand (getpid());
        toxu = fdopen (/*6*/atoi(argv[2]),"w");
        fromxu = fdopen (/*3*/atoi(argv[1]),"r");
        setbuf (stderr, NULL);  
        raw();
        init (&docvposition);
        fillwindow (currentwindow, &docvposition);
        inputstate = NORMALINPUTSTATE;
        nexttypeahead = firsttypeahead = fetypeahead;
}
  void
mumble(c)
  nat c;
{
  nat interact();
  nat exitflag;

        processtypeahead(c);
        if(inputstate == NORMALINPUTSTATE){
                getuserrequest(&request);
                if (request.requestnumber == EXITNORMALLY) {
                        gfxsw_selectdone(gfx);
                        return;
                }
                processuserrequest(&request);
               /* fillwindow (currentwindow, &docvposition);*/
        }
}
  void
mumblequit()
{
        closeupshop();
#ifdef WAITFORBACKEND
        fprintf(stderr,"Waiting for Backend to terminate\n");
        wait (0);
        /*system ("clear");*/
#endif
}
 
