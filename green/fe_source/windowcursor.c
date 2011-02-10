/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code: windowcursor.d
***************************************************/

#include <suntool/tool_hs.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <suntool/msgsw.h>
#include <suntool/optionsw.h>

int mywindowfd;
extern char     *sys_errlist[];
extern int       errno;

static u_int    icon_array[128];
mpr_static(icon_pr, 64, 64, 1, icon_array);

static u_int    new_cursor_array[16] = {
		0xFFFF, 0xFFFF, 0xFFFF, 0x8001, 0x8001, 0xFFFF, 0x8001, 0x8001,
                0xFFFF, 0x8001, 0x8001, 0xFFFF, 0x8001, 0x8001, 0xFFFF, 0xFFFF,

                };

mpr_static(new_cursor_pr, 16, 16, 1, new_cursor_array);
static struct cursor    new_cursor  =  {
                                0, 0,
                                PIX_SRC |/*^*/ PIX_DST,
                                &new_cursor_pr
};

static u_int    main_cursor_array[8]  =  {
                        0xC000E000, 0xF000F800,
                        0xFC00F000, 0x90001800,
                        0x18000C00, 0x0C000600,
                        0x06000300, 0x03000100
                };
mpr_static(main_cursor_pr, 16, 16, 1, main_cursor_array);
static struct cursor    main_cursor  =  {
                                0, 0,
                                PIX_SRC | PIX_DST,
                                &main_cursor_pr
};

  void
normal_cursor(normal)
  int normal;
{
        if (normal)  {
                win_setcursor(mywindowfd, &main_cursor);
        } else {
                win_setcursor(mywindowfd, &new_cursor);
        }
}
