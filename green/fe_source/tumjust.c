/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:     tumjust.d
***************************************************/

#include "fest.h"

/* --------- Routines below set and change tumblers -------- */

/*
tumblerjustify (tumblerptr)
  tumbler *tumblerptr;
{
  med i, j;

        if (iszerotumbler (tumblerptr)) {
                tumblerptr->exp = 0;
                tumblerptr->sign = 0;
                return;
        }
        while (tumblerptr->mantissa[0] == 0) {
                for (i = 0, j = 1; j < NPLACES;)
                        tumblerptr->mantissa[i++] = tumblerptr->mantissa[j++];
                tumblerptr->mantissa[NPLACES - 1] = 0;
                --(tumblerptr->exp);
        }
}
*/

  void
tumblerjustify (tumblerptr)
  tumbler *tumblerptr;
{
  med i, j, shift;
  tumbler temp;


movetumbler(tumblerptr,&temp);
        if (tumblerptr->mantissa[0] != 0) {

                return;
        }
        for (shift = 0; tumblerptr->mantissa[shift] == 0; ++shift) {
                if (shift == NPLACES - 1) {
                        tumblerptr->exp = 0;
                        tumblerptr->sign = 0;

                        return;
                }
        }
        for (i = 0, j = shift; j < NPLACES;)
                tumblerptr->mantissa[i++] = tumblerptr->mantissa[j++];
        while (i < NPLACES)
                tumblerptr->mantissa[i++] = 0;
        tumblerptr->exp -= shift;

}
