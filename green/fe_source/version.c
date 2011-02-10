/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:     version.d
***************************************************/

#include "fest.h"

  void
createversion()
{                                                                   

  tasktype task;
  tumbler olddocid, newdocid, docvsa;
  smalltumbler temp;
                                           
        movetumbler (&currentwindow->windowview.viewspecptr->docid, &olddocid);
    /*    putindexdocumentonstack();*/
        createnewversioninbackend (&task, &olddocid, &newdocid);        
/*
dumptumbler(&newdocid);
fprintf(stderr,"in createversion \n");
*/
        smalltumblerclear (&temp);
        temp.smallmantissa[0] = temp.smallmantissa[1] = 1;
        makedocvsa (&newdocid, &temp, &docvsa);
        makejumpwindow (&docvsa);
        fillwindow (currentwindow, &docvsa);
                                                              
}
