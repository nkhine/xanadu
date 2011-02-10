/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:    feconfig.h
***************************************************/

/*#define UNIX_V7 */
/*#define UNIX_S3 */
/*#define UNIX_S5 */
/*#define MSDOS */
/*#define AMIGA */
/*#define ATARIST*/
#ifdef unix
#ifdef sun
#define UNIX_BSD4_2 
/*#define SUNGRAPHICS*/
#else
#define UNIX_S5 
#endif
#endif

#ifdef UNIX_BSD4_2
#define UNIX
#define VARIABLEDIR
#define BCOPYWORKS 1
#define void int /* compiler bug in 4.2*/
#else
#undef  VARIABLEDIR
#define BCOPYWORKS 0
#endif

#ifdef unix
#ifndef UNIX
#define UNIX
#endif
#endif

#ifndef MAXNAMLEN
#define MAXNAMLEN 14
#endif
#ifdef UNIX
#define TERMCAP
#endif
/*#define FPROTO */  
#ifdef FPROTO
#define LINT_ARGS
#endif
