/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code: weirdoptions.d
***************************************************/

#define GRAPHICS

#include "fest.h"
#include <signal.h>

/* weird options */
extern void (*weirdoptionsarray[15])();
/**/
/**/
#ifdef SUNGRAPHICS
static struct menuitem displayoptions_items[] = {
       	{MENU_IMAGESTRING,"do nothing",(caddr_t)DISPLAYOPTIONSCHOICE0+0},
       	{MENU_IMAGESTRING,"quarterdisplay",(caddr_t) DISPLAYOPTIONSCHOICE0+1},
 	{MENU_IMAGESTRING,"splitdisplayvertical ",(caddr_t)DISPLAYOPTIONSCHOICE0+2},
       	{MENU_IMAGESTRING,"splitdisplayhorizontal",(caddr_t)DISPLAYOPTIONSCHOICE0+3},
 	{MENU_IMAGESTRING,"fulldisplay",(caddr_t)DISPLAYOPTIONSCHOICE0+4},
       	{MENU_IMAGESTRING,"randomdisplay",(caddr_t)DISPLAYOPTIONSCHOICE0+5},
 	{MENU_IMAGESTRING,"experimentaldisplay",(caddr_t)DISPLAYOPTIONSCHOICE0+6},
  	{MENU_IMAGESTRING,"illegaldisplay",(caddr_t)DISPLAYOPTIONSCHOICE0+7},
};
static struct menu displayoptions_body = {
	MENU_IMAGESTRING,"Display style",sizeof(displayoptions_items)/sizeof(struct menuitem),displayoptions_items,(struct menu *)NULL,(caddr_t)NULL
};
/**/
/*static*/ struct menuitem weirdoptions_items[] = {
       	{MENU_IMAGESTRING,"do nothing",(caddr_t) WEIRDOPTIONCHOICE0+0},
 	{MENU_IMAGESTRING,"set display style",(caddr_t)WEIRDOPTIONCHOICE0+1},
 	{MENU_IMAGESTRING,"show key definitions",(caddr_t)WEIRDOPTIONCHOICE0+5},
};

/*static*/ struct menu weirdoptions_body = {
	MENU_IMAGESTRING,"random stuff",
	sizeof(weirdoptions_items)/sizeof(struct menuitem),
	weirdoptions_items,(struct menu *)&displayoptions_body,(caddr_t)NULL
};
/*static*/ struct menu *weirdoptions_ptr = &weirdoptions_body;
#endif

#define NWEIRDOPTIONS 15      
char * weirdoptionslist[] = {
        "do nothing:leave menu",
        "set  display style",
        "show key definitions",
	"compare",
	"followlink ^Y",
	"createlink ^L",
	"makecut ^G",
	"deletecuts",
	"deleteblock ^P",
	"rearrange ^C",
	"createdocument",
	"change fromset ^V",
	"change toset ^B",
	"clearendsets",
	"createversion"
};
              
#define NSETOPTIONS 3                             
char * setoptionslist[] = {
        "do nothing just leave this menu",      
        "set display style",
        "set experimental parameters"
};
                     
#define NDISPLAYOPTIONS 8
char * displayoptionslist[] = {
        "do nothing just leave this menu",
        "quarterdisplay",
        "splitdisplayvertical",
        "splitdisplayhorizontal", 
        "fulldisplay",
        "randomdisplay",
        "experimentaldisplay",
        "illegaldisplay"
};

  void
weirdoptions()
{ 
  med choice;
	if(!iswindow){
		choice = selectmenu(currentwindow->tm+currentwindow->rcursl, currentwindow->lm+currentwindow->rcursc,NWEIRDOPTIONS,weirdoptionslist);
		(*weirdoptionsarray[choice])();
	}else{
		;
	}
}

  void
disableinsertx()
{
		disableinsert = !disableinsert;
		if(disableinsert){
			fprintf(stderr,"insert disabled ");
		}else{
			fprintf(stderr,"insert enabled ");
		}

}

setoptions()
{ 
  med choice;
        choice = selectmenu(currentwindow->tm+currentwindow->rcursl, currentwindow->lm+currentwindow->rcursc,NSETOPTIONS,setoptionslist);
        switch( choice ){ 
          case 0:
                fixglorpfsinwindow(currentwindow);
                break;
          case 1:
                setdisplaystyle();
                break;
          case 2:
                /*setexperimentalparameters();*/
                break;
        }
}
  

  void                   
setdisplaystyle()
{   

  med choice;
        choice = selectmenu(currentwindow->tm+currentwindow->rcursl, currentwindow->lm+currentwindow->rcursc,NDISPLAYOPTIONS,displayoptionslist);
        switch( choice ){ 
          case NOCHANGE:
                fixglorpfsinwindow(currentwindow);
                break;
          case QUARTERDISPLAY:                            
                displaystyle = QUARTERDISPLAY;
                break;      
          case SPLITVERTICAL:
                displaystyle = SPLITVERTICAL;
                break;       

          case SPLITHORIZONTAL:
                displaystyle = SPLITHORIZONTAL;
                break;       
          case FULLDISPLAY:
                displaystyle = FULLDISPLAY;
                break;
          case RANDOMDISPLAY:  
                displaystyle = RANDOMDISPLAY;
                break;
          case EXPERIMENTALDISPLAY:
                displaystyle = EXPERIMENTALDISPLAY;
                break;
          case ILLEGALDISPLAY:
                displaystyle = ILLEGALDISPLAY;
                break; 
          default:
                displaystyle = QUARTERDISPLAY;
                break;
        }
	wrefresh (currentwindow->w);
}


  void
eatunixcommand()
{
  char  unixcommand[100];
  char  filename[100];
  med   status;
        clearwindowandglorpfwindow(currentwindow);
        askuserforstring("Enter UNIX command: ", unixcommand);
        sprintf (filename, "xum%d", getpid());
        if (unixcommand[strlen(unixcommand)-1] == '\n')
                unixcommand[strlen (unixcommand) -1] = '\0';
        strcat (unixcommand, ">");
        strcat (unixcommand, filename);
        if (status = system (unixcommand)) {
               putf("UNIX command xit status = %d\n", status);
                perror ("error");
                fixglorpfsinwindow(currentwindow);
                return;
        }
        if (!makenewdocumentwithtextfromfile(filename))
                fixglorpfsinwindow(currentwindow);
        unlink(filename);
}                                 


  void
clearwindowandglorpfwindow(window)
  windowtype *window;
{
  med i;

        clearwindow(window);
        for(i=0;i<window->windowheight;++i){
                     window->windowline_glorpfed[i]= TRUE; 
        }
        gotoxy(0,0);
}


  void
eatunixfile()
{
  char  filename[100];
        clearwindowandglorpfwindow(currentwindow);
        askuserforstring("Enter UNIX file name: ", filename);
        if (!makenewdocumentwithtextfromfile(filename))
                fixglorpfsinwindow(currentwindow);
}

      

  void                
barfunixfile()
{
   char  filename[100];
        clearwindowandglorpfwindow(currentwindow);
        askuserforstring("Enter UNIX file name: ", filename);
        if (!makenewfilewithtextfromdocument(filename))
                fixglorpfsinwindow(currentwindow);
}


/*
  void
destroystartup()
{
        clearscreenflashlightsandringalarmbells();
        if( askifuserreallywantstogoahead()) {     
                acknowlegeandexplainingreatedetailthedestructivenessofthis();
                if(askifuserreallywantstogoahead()) {
                       putf("\nRemoving current enfilade file");
                        system("rm enf.enf");
                       putf("\nRestoring original enfilade file");
                        system("cp enf.save enf.enf");
                       putf("\nRebooting Xanadu");
                        execl("/bin/sh", "sh", "xanadu", 0);
                        perror("\nReboot failed");
                }
        }
}



/*
  void
clearscreenflashlightsandringalarmbells()
{
        clearwindowandglorpfwindow(currentwindow);
       putf("\07This is a very dangerous operation\n");
}

 med
askifuserreallywantstogoahead()
{
  char  yesornostring[100];

        askuserforstring("\nDo you *REALLY* want to do this?", yesornostring);
        return(yesornostring[0]=='y' || yesornostring[0]=='Y');
}
*/

/*
  void
acknowlegeandexplainingreatdetailthedestructivenessofthis()
{
       putf("\n                  ***** WARNING ******\n");
       putf("This operation will cause the state of the System to\n");
       putf("revert to its original state, and all user-made changes\n");
       putf("and links created since this demo was installed will be\n");
       putf("irrevocably lost.\n");
}
*/

  void
askuserforstring(promptstring, stringtoreturn)
  char * promptstring;
  char * stringtoreturn;
{
       putf("%s", promptstring);
        signal(2, SIG_IGN);
        noraw();
        gets (stringtoreturn);
        raw();
        signal(2, SIG_DFL);
}

#define COMMENT
#ifdef COMMENT
 int
makenewdocumentwithtextfromfile(filename)
  char * filename;
{ 
  tasktype task;
  tumbler docid, docvsa;
  smalltumbler temp;
  nat  filedescript;            
  char string[NTYPEAHEAD+1];
  long start,nchars;
  smalltumbler vsatoinsertat;
  charspantype charspan;
                                                          
        start = 1; 
        filedescript = open(filename,0/*read*/);
  /*     putf("*** make new doc from '%s'\n", filename);     */
        putindexdocumentonstack();
        createnewdocumentinbackend (&task, &docid);

/* shovel stuff from file into document*/ 
/* change to removee tabs !*/
        for(;/*EOF*/0!=  (nchars = read (filedescript,string,NTYPEAHEAD));){
                string[nchars] = '\0';
/*putf("nchars = %d, string='%s'\n",nchars,string);  */
                charspan.numberofcharactersinspan = nchars;
                charspan.charinspanptr = string;
                charspan.nextcharspan = NULL;
                smalltumblerclear (&vsatoinsertat);
                vsatoinsertat.smallmantissa[0] = 1;
                vsatoinsertat.smallmantissa[1] = start;
                inserttextinbackend (&task, &docid, &vsatoinsertat, &charspan);       
                start +=nchars;
        }

        smalltumblerclear (&temp);
        temp.smallmantissa[0] = temp.smallmantissa[1] = 1;
        makedocvsa (&docid, &temp, &docvsa);
        makejumpwindow (&docvsa);
        fillwindow (currentwindow, &docvsa);
        return(TRUE);
}

  int
makenewfilewithtextfromdocument(filename)
  char *filename;
{
       putf("not yet making unix file %s",filename);
	return(FALSE);
}
#endif COMMENT
  
  void
showkeydefs()
{
        system("clear;cat /usr/fenew/cheatsheet");
}
