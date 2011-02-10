/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:   sunscroll.d
***************************************************/

#include "fest.h"

  void
scrlsubwindow (lm,tm,windowwidth,windowheight, n)
    nat lm, tm, windowheight, windowwidth, n;
{
	if(n>0){
		winblockmove(lm,tm+n,windowwidth,windowheight-n,lm,tm);
	}else if(n<0){
		winblockmove(lm,tm,windowwidth,windowheight+n,lm,tm-n);
	}
}
