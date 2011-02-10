/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:      tumble.d
***************************************************/

/* tumbler arithmetic routines */

#include "fest.h"

  void
tumblercopy (fromptr, toptr)
  tumbler *fromptr, *toptr;
{
        movetumbler (fromptr, toptr);
}

#ifdef COMMENT
  void
tumblermin (aptr, bptr, cptr)
  tumbler *aptr, *bptr, *cptr;
{

        if (tumblercmp (aptr, bptr) == LESS)
                movetumbler (aptr, cptr);
          else
                movetumbler (bptr, cptr);

}

  void
tumblermax (aptr, bptr, cptr)
  tumbler *aptr, *bptr, *cptr;
{
        if (tumblercmp (aptr, bptr) == GREATER)
                movetumbler (aptr, cptr);
          else
                movetumbler (bptr, cptr);
}

  void
tumblertruncate (aptr, bint, cptr)
  tumbler *aptr, *cptr;
  med bint;
{
  tumbler answer;
  med i;

        movetumbler (aptr, &answer);
        for  (i = answer.exp; i < 0 && bint > 0; ++i, --bint);
        if (bint <= 0)
                tumblerclear (&answer);
          else
                for (; bint < NPLACES; ++bint)
                        answer.mantissa[bint] = 0;
        tumblerjustify (&answer);
        movetumbler (&answer, cptr);
}

  void
prefixtumbler (aptr, bint, cptr)
  tumbler *aptr, *cptr;
  med bint;
{
  tumbler temp1, temp2;

        tumblerclear (&temp1);
        temp1.mantissa[0] = bint;
        movetumbler (aptr, &temp2);
        temp2.exp -= 1;
        tumbleradd (&temp1, &temp2, cptr);
}

/*
beheadtumbler (aptr, bptr)
  tumbler *aptr, *bptr;
{
  tumbler temp;
  med i;

        movetumbler (aptr, &temp);
        if (temp.exp < 0)
                ++temp.exp;
          else {
                for (i = 0; i < NPLACES-1; ++i)
                        temp.mantissa[i] = temp.mantissa[i+1];
                temp.mantissa[NPLACES-1] = 0;
        }
        tumblerjustify (&temp);
        movetumbler (&temp, bptr);
}
*/

  void
beheadtumbler (aptr, bptr)
  tumbler *aptr, *bptr;
{
  tumbler temp;

        movetumbler (aptr, &temp);
        ++temp.exp;
        if (aptr->exp == 0)
                temp.mantissa[0] = 0;
        tumblerjustify (&temp);
        movetumbler (&temp, bptr);
}
#endif COMMENT

/*
tumblerclear(tumblerptr)
  tumbler *tumblerptr;
{
  static tumbler tumblerzero = 0;
        *tumblerptr = tumblerzero;
if (!iszerotumbler(tumblerptr))
  gerror("settumblertozero don't work\n");
}
*/
  void
tumblerclear (tumblerptr)
  tumbler *tumblerptr;
{
        setmem ((char *)tumblerptr, sizeof (*tumblerptr), 0);
}

  void
smalltumblerclear (smalltumblerptr)
  smalltumbler *smalltumblerptr;
{
        setmem ((char *)smalltumblerptr, sizeof(*smalltumblerptr), 0);
}

  void
zdumptumbler (tumblerptr)
  tumbler *tumblerptr;
{
        return;
}
