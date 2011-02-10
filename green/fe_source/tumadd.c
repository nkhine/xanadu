/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:      tumadd.d
***************************************************/

#include "fest.h"

  void
tumbleradd (aptr, bptr, cptr)  /* Karnaugh map used */
  tumbler *aptr, *bptr, *cptr;
{
/*
if(aptr->sign || bptr->sign) {
 fprintf(stderr,"TUMBLERADD NEG IN\n");
zdumptumbler(aptr);
 fprintf(stderr,"   ");
zdumptumbler(bptr);
 fprintf(stderr,"\n");
}
*/
        if (iszerotumbler(bptr))
                movetumbler (aptr, cptr);
          elif (iszerotumbler(aptr))
                movetumbler (bptr, cptr);

          elif (aptr->sign == bptr->sign) {
                absadd (aptr, bptr, cptr);
                cptr->sign = aptr->sign;
        } elif (abscmp (aptr, bptr) == GREATER) {
                strongsub (aptr, bptr, cptr);
                cptr->sign = aptr->sign;
        } else {
                weaksub (bptr, aptr, cptr);
                cptr->sign = bptr->sign;
        }
        tumblerjustify (cptr);
/*           tumblercheck (cptr);
        if (cptr->sign) {
                fprintf(stderr,"TUMBLERADD NEGATIVE OUTPUT\n");
                zdumptumbler(cptr);
                fprintf(stderr,"\n");
        }
*/}

  void
tumblersub (aptr, bptr, cptr)
  tumbler *aptr, *bptr, *cptr;
{
  tumbler temp;
/*
        if(aptr->sign || bptr->sign) {
                fprintf(stderr,"TUMBLERSUB NEG IN \n");
                zdumptumbler(aptr);
                fprintf(stderr,"   ");
                zdumptumbler(bptr);
                fprintf(stderr,"\n");
        }
*/
        if (iszerotumbler (bptr))
                movetumbler (aptr, cptr);
          elif (tumblereq (aptr, bptr))
                tumblerclear (cptr);
          elif (iszerotumbler (aptr)) {
                movetumbler (bptr, cptr);
                cptr->sign = !cptr->sign;
        } else {
                movetumbler (bptr, &temp);
                temp.sign = !temp.sign;
                tumbleradd (aptr, &temp, cptr);
        }
        tumblerjustify (cptr);
/*           tumblercheck (cptr);
        if (cptr->sign) {
                fprintf(stderr,"TUMBLERSUB NEGATIVE OUTPUT\n");
                zdumptumbler(cptr);
                fprintf(stderr,"\n");
        }
*/

}
  void
absadd (aptr, bptr, cptr)
  register tumbler *aptr, *bptr, *cptr;
{
  tumbler answer;
  register med i, j, place;

        i = j = 0;
        tumblerclear (&answer);
        if (aptr->exp == bptr->exp) {
                answer.exp = aptr->exp;
                answer.mantissa[0] = aptr->mantissa[0] + bptr->mantissa[0];
                i = j = 1;
        } elif (aptr->exp > bptr->exp) {
                answer.exp = aptr->exp;
                while ( i < aptr->exp - bptr->exp ) {
                        answer.mantissa[j++] = aptr->mantissa[i++];
                        if ( j >= NPLACES || i >= NPLACES ) {
                              fprintf(stderr,"tumbler add overflow  ");
                              zdumptumbler(&answer);
                              putf("%c",'\n');
                              return;
                        }
                }
                answer.mantissa[j++] = aptr->mantissa[i++] + bptr->mantissa[0];
                i = 1;
        } else {
                answer.exp = bptr->exp;
                while ( i <= bptr->exp - aptr->exp ) {
                        answer.mantissa[j++] = bptr->mantissa[i++];
                        if ( j >= NPLACES || i >= NPLACES ) {
                              fprintf(stderr,"tumbler add overflow  ");
                              zdumptumbler(&answer);
                              putf("%c",'\n');
                        }
                }
        }
        for (place = NPLACES-1; place >= 0 && bptr->mantissa[place] == 0; --place);
        while ( i <= place ) {
                if ( j >= NPLACES ) {
                        fprintf(stderr,"tumbler add overflow  ");
                        zdumptumbler(&answer);
                        putf("%c",'\n');
                }
                answer.mantissa[j++] = bptr->mantissa[i++];
        }
        movetumbler (&answer, cptr);
        return;
}

  void
strongsub (aptr, bptr, cptr)
  tumbler *aptr, *bptr, *cptr;
{
  tumbler answer;
  med i, j;

        tumblerclear(&answer);
        if (tumblereq (aptr, bptr)) {
                movetumbler (&answer, cptr);
                return;
        }
        if (bptr->exp < aptr->exp) {
                movetumbler (aptr, cptr);
                return;
        }
        answer.exp = aptr->exp;
        for (i = 0; aptr->mantissa[i] == bptr->mantissa[i]; ++i) {
                --answer.exp;
                if (i >= NPLACES) {
                        movetumbler (&answer, cptr);
                        return;
                }
        }
        answer.mantissa[0] = aptr->mantissa[i] - bptr->mantissa[i];
        if (++i >= NPLACES) {
                movetumbler (&answer, cptr);
                return;
        }
        for (j = 1; j < NPLACES && i < NPLACES;)
                answer.mantissa[j++] = aptr->mantissa[i++];
        movetumbler (&answer, cptr);
        return;
}
 
  void
weaksub (aptr, bptr, cptr)
  tumbler *aptr, *bptr, *cptr;
{
  tumbler answer;
  med i, expdiff;

        tumblerclear(&answer);
        if (tumblereq (aptr, bptr)) {
                movetumbler (&answer, cptr);
                return;
        }
        answer.exp = aptr->exp;
        expdiff = aptr->exp - bptr->exp;
        for (i = 0; i < expdiff; ++i) {
                answer.mantissa[i] = aptr->mantissa[i];
                if (i >= NPLACES) {
                        movetumbler (&answer, cptr);
                        return;
                }
        }
        answer.mantissa[i] = aptr->mantissa[i] - bptr->mantissa[0];
        movetumbler (&answer, cptr);
}

#ifdef COMMENT
 long
tumblerintdiff (aptr, bptr)
  tumbler *aptr, *bptr;
{
  tumbler c;

        tumblersub (aptr, bptr, &c);
        return (c.mantissa[0]);
}

  void
tumblerincrement (aptr, rightshift, bint, cptr)
  tumbler *aptr, *cptr;
  long bint;
  med  rightshift;
{
  med idx;

        if (iszerotumbler (aptr)) {
                tumblerclear (cptr);
                cptr->exp = -rightshift;
                cptr->mantissa[0] = bint;
                return;
        }
        if(aptr != cptr)  movetumbler(aptr,cptr);
        for (idx = NPLACES ; aptr->mantissa[--idx] == 0 && idx > 0;);
        if (idx + rightshift >= NPLACES) {
                zdumptumbler (aptr);
                fprintf(stderr," idx = %d  rightshift = %d\n", idx, rightshift);
                gerror ("tumblerincrement overflow\n");
        }
        cptr->mantissa[idx + rightshift] += bint;
        tumblerjustify (cptr);
}
#endif COMMENT
