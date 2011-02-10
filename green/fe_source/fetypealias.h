/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code: fetypealias.h
***************************************************/

#ifndef FETYPEALIASESDEFINED
#define SHORT short
#ifndef MSDOS
typedef SHORT/*yes really int*/ med; /* for things that can be as small as 64k */
#else
typedef int/*yes really int*/ med; /* for things that can be as small as 64k */

#endif
typedef int/*yes really int*/ nat; /*for things that really have to be native int*/
typedef long big; /*for random things that want to be longs*/
typedef unsigned long ubig; /*for random things that want to be unsigned longs*/
#ifndef bool
#define bool unsigned /* #$%! defined in sunwindow/include somewhere so must be unsigned to avoid conflict */
#endif
#define metachar SHORT
#define FETYPEALIASESDEFINED
#endif

