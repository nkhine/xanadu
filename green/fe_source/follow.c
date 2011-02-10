/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:      follow.d
***************************************************/

#include "fest.h"
      /*perhaps the correspondences should be ints rather than tumblers*/

  void
followcursorcontrol()
{           
  spectype *curspec,*relatedspec,*specalloc();
  windowtype *oldwindow;
  med temp;
        relatedspec = NULL;
        curspec = specalloc();
        temp = findspecfromcursorposition/*inline*/(curspec);
        convertspectorelatedspec(currentwindow->correspondencelist ,curspec,&relatedspec,temp);
        if(relatedspec&&currentwindow->correspondingwindow){ /*changewindows*/
                currentwindow = currentwindow->correspondingwindow;
                putcursoronspec(relatedspec);
                currentwindow=currentwindow->correspondingwindow;/*changeback*/
                putcursor();
                specfree(relatedspec);
        }else{
                /*printf("huh norelatedspec");*/
                return;
        }
        specfree(curspec);
}                                    

  void
convertspectorelatedspec(relationshiplist,curspec,relatedspec,pasteol)
  correspondencetype *relationshiplist;
  spectype *curspec,**relatedspec;
  med pasteol;
{
  med i,j;
  long foo;
  correspondencetype * temp;
  spectype *specalloc();
/*gotoxy(0,0);
 dumpcorrespondenceset(relationshiplist);
*/
        i = currentwindow->dependentscreenp;
        j = i?0:1;
	/*printf(" d = %d",curspec->specspanptr->vstartaddress.mantissa[1]);*/
        for(temp = relationshiplist;temp;temp = temp->nextcorrespondence){
                foo = curspec->specspanptr->vstartaddress.smallmantissa[1]-lastdigitintumbler(&(temp->stream[i]));
                if(foo >=0 && foo < lastdigitintumbler(&(temp->widthofspan))){
                        if(pasteol){
                                foo--;
                        }
                   *relatedspec = specalloc();
                   copyvpositionintospec(&(temp->stream[j]),*relatedspec);
                   (*relatedspec)->specspanptr->vstartaddress.smallmantissa[1]+=foo;
                        return;
                }
        }
/*printf("returning NULL from convertspectorelatedspec");*/
}

  void
putcursoronspec(specptr)
  spectype * specptr;
{  
  med i;
  long temp, cursorvposition; /*see fillwindow*/
        cursorvposition = specptr->specspanptr->vstartaddress.smallmantissa[1];
        for(i=0;i<currentwindow->windowheight;i++){
                if(cursorvposition >= currentwindow->windowleftmargins[i]&&cursorvposition</**/nextl/**/(currentwindow->windowleftmargins[i/*+1*/])){
                      temp=cursorvposition-currentwindow->windowleftmargins[i];
                      currentwindow->rcursc = temp;
                      currentwindow->rcursl = i;
/*printf("y=%d",i);*/
                      currentwindow->windowline_glorpfed[i] = TRUE;/*fix screennow*/
                      fixglorpfsinwindow(currentwindow); 
                      putcursor();
		      wrefresh(currentwindow->w);
                      currentwindow->windowline_glorpfed[i] = TRUE;/* be sure to fix screen later*/
fflush(stdout);/*sleep(1);*/
                        return;
                }
        }
        currentwindow->windowleftmargins[0] = specptr->specspanptr->vstartaddress.smallmantissa[1];
        paintwindowplain (currentwindow);
}
