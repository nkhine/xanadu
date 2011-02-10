/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:     compare.d
***************************************************/

#include "fest.h"

static  tasktype task;
static  spectype *specset,*specset2;
static  tumbleritemtype *docsetptr;

  void
compare()			/*compare versions or documents*/
{
  char **menuptr;
  med ndocs;                               
  med selectedmenu,selectmenu() ;
  void compare2();

        copyspecset(currentwindow->windowview.viewspecptr,&specset);
        copyspecset(currentwindow->windowview.viewspecptr,&specset2);

        finddocscontaininginbackend (&task, specset, &docsetptr); 
        ndocs = makelistfordocumentmenu (docsetptr, &menuptr);
        if (ndocs > 1) {
		if (!iswindow) {
	                selectedmenu = 
				selectmenu(currentwindow->rcursl,
					   currentwindow->rcursc,
					    ndocs,menuptr) 
                                + 1;
		} else {
			makemenu(menuptr,DOCUMENTMENUCHOICE0,ndocs,
			         "document menu");
		}
        } else {
		return;                                      
        }                                            

	if (iswindow) {
		return;
	} else{
		compare2(selectedmenu);
	}
}

  void
compare2(selectedmenu)
  med selectedmenu;                        
{    /*compare versions or documents*/
  tumbler docid, docvsa, otherdocid;
  smalltumbler temp;
  med findlastplace();
  vmthingtype * nthiteminlist();     
  windowtype *oldwindow,*newwindow;
  void changecurrentwindowdimensions();
  void compareviewspecs();

        if(selectedmenu >1){
	    movetumbler(&(nthiteminlist(selectedmenu,(vmthingtype *)docsetptr)->vmtumbleritem.value),&otherdocid);
        }else{
                movetumbler(&docsetptr->value,&otherdocid);   
        }
        tumblercopy(&otherdocid,&specset2->docid);
        /*dumptumbler(&otherdocid); */

        oldwindow = currentwindow;
        changecurrentwindowdimensions(0,0,screenheight-2,screenwidth/2-1);
        smalltumblerclear (&temp);
        temp.smallmantissa[0] = temp.smallmantissa[1] = 1;
        makedocvsa (&(specset->docid), &temp, &docvsa);           
        fillwindow (currentwindow, &docvsa);    

        smalltumblerclear (&temp);
        temp.smallmantissa[0] = temp.smallmantissa[1] = 1;
        makedocvsa (&specset2->docid, &temp, &docvsa);          
     jumpwindowcontrol(0,screenwidth/2,screenheight-2,screenwidth/2-1,&docvsa);
        fillwindow (currentwindow, &docvsa);     
        newwindow = currentwindow;
        currentwindow = oldwindow;      
        oldwindow->correspondingwindow = newwindow;
        newwindow->correspondingwindow = oldwindow;
	newwindow->dependentscreenp = TRUE;
        compareviewspecs(&task,specset,specset2/*,currentwindow->windowview.viewspecptr,newwindow->windowview.viewspecptr*/);
        gotoxy(currentwindow->rcursc+currentwindow->lm,currentwindow->rcursl+currentwindow->tm);
              
} 

  void
changecurrentwindowdimensions(tm,lm,height,width)
  med tm,lm,height,width;
{
        currentwindow->tm = tm;
        currentwindow->lm = lm;
        currentwindow->windowheight = height;
        currentwindow->windowwidth = width;
	if(iswindow){
		clearwindowgraphically(currentwindow);
	}
}
  void
compareviewspecs(taskptr,specset1,specset2a  )
 
  tasktype *taskptr;                                             
  spectype *specset1, *specset2a;
{  
  windowtype *oldwindow,*newwindow;
        if(currentwindow->correspondencelist){
                freecorrespondencelist(currentwindow->correspondencelist);
                currentwindow->correspondencelist = NULL; /* fix someday to update rather tahn empty and recalculate where possible*/
        }
/*fprintf(stderr,"\ncalling showrelationship ");*/
        showrelationof2versionsinbackend(taskptr,specset1,specset2a,&currentwindow->correspondencelist);
/*fprintf(stderr," corespondenceset  = %x",currentwindow->correspondencelist); 
        if(currentwindow->correspondencelist) 
                dumpcorrespondenceset(currentwindow->correspondencelist);
  */               
        oldwindow = currentwindow;
        oldwindow->ispartofparalleltextface = TRUE;
        newwindow = currentwindow ->correspondingwindow;
        newwindow->ispartofparalleltextface = TRUE;
        newwindow->correspondencelist = oldwindow->correspondencelist;
        newwindow->correspondingwindow = oldwindow;

}

  void
moveandflipcorrespondenceset(oldwindow)
  windowtype *oldwindow;
{
  tumbler temptumbler;
  correspondencetype *corptr;
  windowtype *newwindow;

	newwindow = oldwindow->correspondingwindow;
	newwindow->correspondencelist = oldwindow->correspondencelist;
	oldwindow->correspondencelist = NULL;
	for(corptr=newwindow->correspondencelist;corptr;corptr=corptr->nextcorrespondence){/* zzz kluge uses tumbler assignment*/
		temptumbler = corptr->stream[0];
		corptr->stream[0] =corptr->stream[1];
		corptr->stream[1] = temptumbler;
	}
} 
