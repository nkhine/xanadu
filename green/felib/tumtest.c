/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:     tumtest.d
***************************************************/

#include "fest.h"


/* ---------------- Routines to test tumblers -------------- */
#ifdef UNDEFINED
/*  bool
iszerotumbler(tumblerptr)
  register tumbler *tumblerptr;
{

/*  register InT i;
  static FILE *fd = 0;
  register char *tptr,*tptrp1;
         if(!fd){
                fd = fopen("zizreotum","a");
        }
*/

 /*           for (i = 0; i < NPLACES;)
                if (tumblerptr->mantissa[i++]){

                        return (FALSE);
        return (TRUE);
 */
return(!(tumblerptr -> mantissa[0]));
/*

        tptr = (char *) tumblerptr -> mantissa;
        tptrp1 = tptr +1;
        for(i = 0;i<2*NPLACES;i+=4){
                if(*(InT *)(tptrp1 + i) || *(InT *)(tptr + i)){
                        if(tumblerptr -> mantissa[0] == 0){
                                fprintf(fd,"found one"); /*commented out in iszerotumbler*/
                        }
                        return(FALSE);
                }
        }
        return(TRUE);
*/
}*/
#endif UNDEFINED

  bool
tumblereq (aptr,bptr)
  tumbler *aptr, *bptr;
{
        return (tumblercmp (aptr, bptr) == EQUAL);
}

  bool
smalltumblereq (aptr,bptr)
  tumbler *aptr, *bptr;
{
        return (smalltumblercmp (aptr, bptr) == EQUAL);
}

  med
tumblercmp (aptr, bptr)
  register tumbler *aptr, *bptr;
{

        if (iszerotumbler(aptr) && iszerotumbler(bptr))
                return (EQUAL);
        if (iszerotumbler(aptr))
                return (bptr->sign ? GREATER : LESS);
        if (iszerotumbler(bptr))
                return (aptr->sign ? LESS : GREATER);
        if (aptr->sign == bptr->sign)
                return (aptr->sign ? abscmp(bptr,aptr) : abscmp(aptr,bptr));
        return (aptr->sign ? LESS : GREATER);
}

  med
smalltumblercmp (aptr, bptr)
  register smalltumbler *aptr, *bptr;
{
  bool iszerosmalltumbler();

        if (iszerosmalltumbler(aptr) && iszerosmalltumbler(bptr))
                return (EQUAL);
        if (iszerosmalltumbler(aptr))
                return (bptr->sign ? GREATER : LESS);
        if (iszerosmalltumbler(bptr))
                return (aptr->sign ? LESS : GREATER);
        if (aptr->sign == bptr->sign)
                return (aptr->sign ? abscmpsmall(bptr,aptr) : abscmpsmall(aptr,bptr));
        return (aptr->sign ? LESS : GREATER);

}

  med
abscmp (aptr,bptr)
  tumbler *aptr, *bptr;
{
  register tdigit *a, *b;
  register med i;

        if (aptr->exp < bptr->exp)
                return(LESS);
        if (aptr->exp > bptr->exp)
                return(GREATER);
        a = (tdigit *) aptr->mantissa;
        b = (tdigit *) bptr->mantissa;
        for (i = NPLACES; i--;) {
                if (*a < *b)
                        return (LESS);
                if (*a++ > *b++)
                        return (GREATER);
        }
        return (EQUAL);
}


  med
abscmpsmall (aptr,bptr)
  smalltumbler *aptr, *bptr;
{
  register tdigit *a, *b;
  register med i;

        if (aptr->exp < bptr->exp)
                return(LESS);
        if (aptr->exp > bptr->exp)
                return(GREATER);
        a = (tdigit *) aptr->smallmantissa;
        b = (tdigit *) bptr->smallmantissa;
        for (i = SMALLNPLACES; i--;) {
                if (*a < *b)
                        return (LESS);
                if (*a++ > *b++)
                        return (GREATER);
        }
        return (EQUAL);
}

/*
  med
intervalcmp (left, right, address)
  tumbler *left, *right, *address;
{
  med cmp;

        cmp = tumblercmp (address, left);
        if (cmp == LESS)
                return (TOMYLEFT);
          elif (cmp == EQUAL)
                return (ONMYLEFTBORDER);
        cmp = tumblercmp (address, right);
        if (cmp == LESS)
                return (THRUME);
          elif (cmp == EQUAL)
                return (ONMYRIGHTBORDER);
          else
                return (TOMYRIGHT);
}
*/

  bool
iszerosmalltumbler (tumblerptr)
  smalltumbler *tumblerptr;
{
        return (!tumblerptr->smallmantissa[0]);
}

/*
  bool
tumblercheck (ptr)
  tumbler *ptr;
{
  bool wrong;
  med i;

        wrong = FALSE;
        if (ptr->exp > 0)
                wrong = TRUE;
        if (ptr->sign && ptr->mantissa[0] == 0)
                wrong = TRUE;
        if (ptr->exp && ptr->mantissa[0] == 0)
                wrong = TRUE;
        if (ptr->mantissa[0] == 0)
                for (i = 1; i < NPLACES; ++i)
                        if (ptr->mantissa[i] != 0)
                                wrong = TRUE;
        if (wrong) {
                        zdumptumbler (ptr);
                        fprintf(stderr,"  invalid tumbler\n");
gerror("in tumbler check");
                        return (FALSE);
        }
        return (TRUE);
}
*/

/* says whether there is no more than a single non-zero
**  digit in mantissa
*/
/*
  bool
is1story (ptr)
  register tumbler *ptr;
{
  register med i;

        for (i = 1; i < NPLACES; i++)
                if (ptr->mantissa[i] != 0)
                        return (FALSE);
        return (TRUE);
}
*/

  med
nstories(tumblerptr)
  tumbler *tumblerptr;
{
  med i;

        for (i = NPLACES; i > 0 && tumblerptr->mantissa[--i] == 0;);
        return (i + 1);
}

/*
 med 
tumblerlength (tumblerptr)
  tumbler *tumblerptr;
{
        return (nstories (tumblerptr) - tumblerptr->exp);
}
*/

/*
  med
nzeroesintumbler (tumblerptr)
  tumbler *tumblerptr;
{
  med n, i, count;

        n = nstories (tumblerptr);
        for (count = i = 0; i < n; ++i)
                if (tumblerptr->mantissa[i] == 0)
                        ++count;
        count -= tumblerptr->exp;
        return (count);
}
*/

  long
lastdigitintumbler (tumblerptr)
  tumbler *tumblerptr;
{
  med n;
  long digit;

        n = nstories (tumblerptr);
        digit = tumblerptr->mantissa[n -1];
        return (digit);
}
