/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:         uxf.d
***************************************************/

#include "fest.h"

/*#include "feminreq.h"*/
/*#include <sgtty.h>*/
/*#include <sys/types.h>*/
/*#include <sys/stat.h>*/

extern med termisambassador;

#undef UNIX_BSD4_2
#ifdef UNIX_BSD4_2
  void
raw()
{
  struct sgttyb arg;
      ioctl(0,TIOCGETP,&arg);
      arg.sg_flags = (arg.sg_flags |/*CBREAK */ RAW) & ~ECHO;
      ioctl(0,TIOCSETP,&arg);
if(termisambassador)
clearautoxoff();
}

  void
cooked()
{
struct sgttyb arg;
        ioctl(0,TIOCGETP,&arg);
        arg.sg_flags = (arg.sg_flags & /*~CBREAK */~RAW) | ECHO;
        ioctl(0,TIOCSETP,&arg);
/*if(termisambassador)
setautoxoff();
*/
}
#else

/*
void
raw()
{
}
void
cooked()
{
}
*/

#endif
#undef max
#ifndef max
  nat /* the following stolen from usefull to allow it to be use.s*/
max (a,b)
  nat a,b;
{
        return (a > b ? a : b);
}
#endif

/*
#undef abs
#ifndef abs
  InT
abs(x)
  InT x;
{
	return(x>0?x:-x);
}
#endif

  void
causelinttoyellaboutminnotmadeforlongs()
{
return;
}
  InT
min (a,b)
  InT a,b;
{
        return (a < b ? a : b);
}
*/

  nat
diff (a,b)
  nat a,b;
{
        return (max(a,b) - min(a,b));
}

  void
gerror(message)
        char *message;
{
        fprintf(stderr,"Fe Gerror: %s",message);
	noraw();
	endwin();
	fflush(stdout);
        /*system ("cp core old.core");*/
#ifdef AMIGA
        AmiCleanup();
#endif

        abort();
}
