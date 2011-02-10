/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:     usefull.d
***************************************************/

#include "fest.h"

  void
memclear (addr, count)
   char *addr;
   unsigned count;
{
/*
setmem(addr,count,0);
*/
        while((count--) >0)        /*I put parens around this ??? hugh*/
                *addr++ = 0;

}
/* decomment when we go to UNIX */
  void
setmem (addr, count, byte)
   char *addr;
   unsigned count;
   char byte;
{
/*
        while (count-- > 0)
                *addr++ = byte;
*/
  register char *temp;
        temp = addr + count;
        while (addr < temp){
                *addr++ = byte;
        }
}
#ifndef movmem
  void
movmem (source, dest, count)
  char *source,*dest;
  unsigned count;
{
                if ((nat)count < 0)
                        gerror ("count < 0 (or way too big) in movmem\n");
                if (source > dest)
                        while (count--)
                                *dest++ = *source++;
                else {
                        source += count -1;
                        dest += count -1;
                        while (count--)
                                *dest-- = *source--;
                }
}
#endif
 
