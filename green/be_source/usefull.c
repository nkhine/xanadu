/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
#include "xanadu.h"
#include "enf.h"


/* 
clear (addr, count)
  register char *addr;
  register unsigned count;
{
	bzero(addr,count);
//
        while(count--)
                *addr++ = 0;
//
}
*/
setmem (addr, count, byte)
  register char *addr;
  register unsigned count;
  register char byte;
{
        while (count--){
                *addr++ = byte;
        }
}

mymovmem (source, dest, count)
  register char *source,*dest;
  register unsigned count;
{
        if (source > dest){
                while (count--)
                        *dest++ = *source++;
        }
        else {
                source += count;
                dest += count;
                while (count--)
                        *--dest = *--source;
        }
}

/*movmem(source,dest,count)
 char *source,*dest;
 int count;
{
	bcopy(source,dest,count);
}
*/
