/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/

#define ALLOCSIZE /* 0x30000*/  20000000 /* 3000000*/ /* 0x20000*/ /*0x62080*/ /* 0x44080 */ /* or0x24080*/     /* 0x10000 almost works 12/24/86 */
#define INCREMENTALALLOCSIZE 2500000
typedef INT ALIGN;
union header {
        struct{
                union header *ptr;
                unsigned size;
        }s;
        ALIGN x;
};

typedef union header HEADER;
extern INT allocsize;
extern INT incrementalallocsize;
 
