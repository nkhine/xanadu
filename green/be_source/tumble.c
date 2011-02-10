/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* tumble.d -  tumbler arithmetic routines */

#include "common.h"	/* EXTERNAL VARIABLE BEWARE!!*/

tumbler ZEROTUMBLERvar;
static INT abscmp();

/* ---------------- Routines to test tumblers -------------- */

  bool
tumblereq (a,b)
  tumbler *a,*b;
{
  register char *aptr = (char *) a; /*char * for cheating in compare loop*/
  register char *bptr = (char *) b;
  register INT i;
       /* return (tumblercmp (aptr, bptr) == EQUAL);// old safe & slow// */ 
/* could speed up by doing comparison directly or stealing from abscmp*/
	i = (int)aptr+sizeof(tumbler);
	for (;((int)aptr)<i;){
		if(*aptr++ != *bptr++)
			return(FALSE);
	}
	return (TRUE);
}

  bool
tumbleraccounteq (aptr, bptr)
  tumbler  *aptr,*bptr;
{
  INT i, j;

	if (aptr->exp != bptr->exp || aptr->sign != bptr->sign) {
		return(FALSE);
	}
	for (j = 0, i = 0; i < NPLACES; i++) {
		if (aptr->mantissa[i] != bptr->mantissa[i]) {
			return(FALSE);
		}
		if (aptr->mantissa[i] == 0 && ++j == 2) {
			return(TRUE);
		}
	}
	return (TRUE);
}

  INT
tumblercmp (aptr, bptr)
  register tumbler *aptr, *bptr;
{
	if (iszerotumbler(aptr)){
		if (iszerotumbler(bptr))
			return (EQUAL);
		else
			return (bptr->sign ? GREATER : LESS);
	}
	if (iszerotumbler(bptr))
		return (aptr->sign ? LESS : GREATER);
	if (aptr->sign == bptr->sign)
		return (aptr->sign ? abscmp(bptr,aptr) : abscmp(aptr,bptr));
	return (aptr->sign ? LESS : GREATER);
}
#ifndef ExPeriMental
static INT
abscmp(aptr,bptr)
  tumbler *aptr, *bptr;
{
  register INT *a, *b;
  register INT i, cmp;

	if (aptr->exp != bptr->exp) {
		if (aptr->exp < bptr->exp) {
			return(LESS);
		} else {
			return(GREATER);
		}
	} else {
		a = (INT *) aptr->mantissa;
		b = (INT *) bptr->mantissa;
		for (i = NPLACES; i--;) {
		        if(!(cmp = *a++ - *b++)){
			} else if (cmp < 0) {
				return (LESS);	/* a < b */
			} else {  /* if (cmp > 0) */
				return (GREATER);
			}
		}
	}
	return (EQUAL);
}
#else
static INT
abscmp(aptr,bptr)
  tumbler *aptr, *bptr;
{
  register INT *a, *b;
  register INT i, cmp;

	if (aptr->exp != bptr->exp) {
		if (aptr->exp < bptr->exp) {
			return(LESS);
		} else {
			return(GREATER);
		}
	} else {
		a = (INT *) aptr->mantissa;
		b = (INT *) bptr->mantissa;
		for (i = NPLACES; i--;) {
			cmp = *a - *b;
			if (cmp == 0) { /* this is an efficiency hack */
				a++; b++;
				continue;
			} else if (cmp < 0) {
				return (LESS);	/* a < b */
			} else {  /* if (cmp > 0) */
				return (GREATER);
			}
		}
	}
	return (EQUAL);
}

#endif

  INT
intervalcmp (left, right, address)
  tumbler *left, *right, *address;
{
  register INT cmp;

	cmp = tumblercmp (address, left);
	if (cmp == LESS)
		return (TOMYLEFT);
	  else if (cmp == EQUAL)
		return (ONMYLEFTBORDER);
	cmp = tumblercmp (address, right);
	if (cmp == LESS)
		return (THRUME);
	  else if (cmp == EQUAL)
		return (ONMYRIGHTBORDER);
	  else
		return (TOMYRIGHT);
}

/*  bool
iszerotumbler(tumblerptr)
  tumbler *tumblerptr;
{
	return(!(tumblerptr -> mantissa[0]));
} */

  bool
tumblercheckptr (ptr,crumptr)
  tumbler *ptr;
  INT /*typecrum*/ *crumptr;
{
  bool wrong;
  INT i;
	wrong = FALSE;
	if (ptr->exp > 0){
#ifndef DISTRIBUTION
		fprintf(stderr,"bad exp ");
#endif
		wrong = TRUE;
	}
	if (ptr->sign && ptr->mantissa[0] == 0){
#ifndef DISTRIBUTION
		fprintf(stderr," negative zero ");
#endif
		wrong = TRUE;
	}
	if (ptr->exp && ptr->mantissa[0] == 0){
#ifndef DISTRIBUTION
		fprintf(stderr,"fucked up non-normalized");
#endif
		wrong = TRUE;
	}
	if (ptr->mantissa[0] == 0){
		for (i = 1; i < NPLACES; ++i){
			if (ptr->mantissa[i] != 0){
#ifndef DISTRIBUTION
				fprintf(stderr,"nonzerozerotumbler");
#endif
				wrong = TRUE;
			}
		}
	}
	for (i = 0; i < NPLACES; ++i){
		if ((INT)(ptr->mantissa[i]) < 0){
#ifndef DISTRIBUTION
			fprintf(stderr,"negative digit");
#endif
			wrong = TRUE;
		}
	}
	if (wrong) {
#ifndef DISTRIBUTION
			dumptumbler (ptr);
			if(crumptr){
				dump(crumptr);
			}
			fprintf(stderr,"\n\n invalid tumbler \n\n");
			if(crumptr){
				dumpwholetree(crumptr);
			}
			gerror("  invalid tumbler\n");
#else
	gerror("");
#endif
			return (FALSE);
	}
	return (TRUE);
}

  bool
tumblercheck(ptr)
  tumbler *ptr;
{
	return(tumblercheckptr(ptr, (INT*) NULL));
}

/* says whether there is no more than a single non-zero
**  digit in mantissa
*/
  bool
is1story (tumblerptr)
  tumbler *tumblerptr;
{
  INT i;

/*	if (!tumblercheck (tumblerptr))
		qerror ();
*/	for (i = 1; i < NPLACES; i++)
		if (tumblerptr->mantissa[i] != 0)
			return (FALSE);
	return (TRUE);
}

  INT
nstories(tumblerptr)
  tumbler *tumblerptr;
{
  INT i;

/*	if (!tumblercheck (tumblerptr))
		qerror ();
*/	for (i = NPLACES; i > 0 && tumblerptr->mantissa[--i] == 0;);
	return (i + 1);
}

  INT
tumblerlength (tumblerptr)
  tumbler *tumblerptr;
{
	return (nstories (tumblerptr) - tumblerptr->exp);
}

/*  INT
nzeroesintumbler (tumblerptr)
  tumbler *tumblerptr;
{
  INT n, i, count;

	n = nstories (tumblerptr);
	for (count = i = 0; i < n; ++i)
		if (tumblerptr->mantissa[i] == 0)
			++count;
	count -= tumblerptr->exp;
	return (count);
}*/

  INT
lastdigitintumbler (tumblerptr)
  tumbler *tumblerptr;
{
  INT n, digit;

	n = nstories (tumblerptr);
	digit = tumblerptr->mantissa[n -1];
	return (digit);
}

/* --------- Routines below set and change tumblers -------- */

tumblerjustify (tumblerptr)
 register tumbler *tumblerptr;
{
  register INT i, j;
  INT shift;
  tdigit *mantissaptr;
  
	mantissaptr = tumblerptr->mantissa;
	if (mantissaptr[0] != 0) {
		return;
	}
	for (shift = 0; mantissaptr[shift] == 0; ++shift) {
		if (shift == NPLACES - 1) {
			tumblerptr->exp = 0;
			tumblerptr->sign = 0;
			return;
		}
	}
	for (i = 0, j = shift; j < NPLACES;)
		mantissaptr[i++] = mantissaptr[j++];
	while (i < NPLACES)
		mantissaptr[i++] = 0;
	tumblerptr->exp -= shift;
/*	if (!tumblercheck (tumblerptr))
		qerror ();
*/}

partialtumblerjustify (tumblerptr)
 register tumbler *tumblerptr;
{
  register INT i, j;
  INT shift;
  tdigit *mantissaptr;
  
	mantissaptr = tumblerptr->mantissa;
	/* test commented out because is done before this routine is called for efficiency */
      /*  if (mantissaptr[0] != 0) {
		return;
	}*/
	for (shift = 0; mantissaptr[shift] == 0; ++shift) {
		if (shift == NPLACES - 1) {
			tumblerptr->exp = 0;
			tumblerptr->sign = 0;
			return;
		}
	}
	for (i = 0, j = shift; j < NPLACES;)
		mantissaptr[i++] = mantissaptr[j++];
	while (i < NPLACES)
		mantissaptr[i++] = 0;
	tumblerptr->exp -= shift;
/*	if (!tumblercheck (tumblerptr))
		qerror ();
*/}

tumblercopy (fromptr, toptr)
  tumbler *fromptr, *toptr;
{
	/*movmem (fromptr, toptr, sizeof(tumbler));  */
	movetumbler(fromptr,toptr); 
}

/*tumblermin (aptr, bptr, cptr)
 register  tumbler *aptr, *bptr, *cptr;
{
	if (tumblercmp (aptr, bptr) == LESS)
		movetumbler (aptr, cptr);
	  else
		movetumbler (bptr, cptr);
}*/

tumblermax (aptr, bptr, cptr)
 register tumbler *aptr, *bptr, *cptr;
{
	if (tumblercmp (aptr, bptr) == GREATER)
		movetumbler (aptr, cptr);
	  else
		movetumbler (bptr, cptr);
}

functiontumbleradd (aptr, bptr, cptr)  /* tumbler add is ~50% of cpu so has been */
			       /*tightened somewhat */
 register tumbler *aptr, *bptr, *cptr;
{
	if (iszerotumbler(bptr)){
		movetumbler (aptr, cptr);
		return;
	  }else if (iszerotumbler(aptr)){
		movetumbler (bptr, cptr);
		return;
	  }else if (aptr->sign == bptr->sign) {
		absadd (aptr, bptr, cptr);
		cptr->sign = aptr->sign;
		/*absadd returns justified result so no need to justify*/
		/* I'm not so sure of the subtracts, they aren't used much*/
		/*
		if(cptr->mantissa[0] == 0){
			partialtumblerjustify (cptr);
		}
		*/
	} else if (abscmp (aptr, bptr) == GREATER) {
		strongsub (aptr, bptr, cptr);
		cptr->sign = aptr->sign;
		if(cptr->mantissa[0] == 0){
			partialtumblerjustify (cptr);
		}
	} else {
		weaksub (bptr, aptr, cptr);
		cptr->sign = bptr->sign;
		if(cptr->mantissa[0] == 0){
			partialtumblerjustify (cptr);
		}
	}
/*	tumblercheck (cptr);*/
/*
	if (cptr->sign) {
		fprintf(stderr,"TUMBLERADD NEGATIVE OUTPUT\n");
		dumptumbler(cptr);
		fprintf(stderr,"\n");
	}
*/}

tumblersub (aptr, bptr, cptr)
 register tumbler *aptr, *bptr, *cptr;
{
  tumbler temp;
/*
	if(aptr->sign || bptr->sign) {
		fprintf(stderr,"TUMBLERSUB NEG IN \n");
		dumptumbler(aptr);
		fprintf(stderr,"   ");
		dumptumbler(bptr);
		fprintf(stderr,"\n");
	}
*/
	if (iszerotumbler (bptr))
		movetumbler (aptr, cptr);
	else if (tumblereq (aptr, bptr))
		tumblerclear (cptr);
	else if (iszerotumbler (aptr)) {
		movetumbler (bptr, cptr);
		cptr->sign = !cptr->sign;
	} else {
		movetumbler (bptr, &temp);
		temp.sign = !temp.sign;
		tumbleradd (aptr, &temp, cptr);
	}
	tumblerjustify (cptr);
/*	tumblercheck (cptr);*/
/*
	if (cptr->sign) {
		fprintf(stderr,"TUMBLERSUB NEGATIVE OUTPUT\n");
		dumptumbler(cptr);
		fprintf(stderr,"\n");
	}
*/

} 
#ifndef ExPeRiMENATL
#endif

absadd (aptr, bptr, cptr)
  tumbler *aptr, *bptr;
  tumbler *cptr;
{
  register INT i, j;
  INT place;
  INT temp;
  register tdigit *ansmant;
  register tdigit *bmant, *amant;
  tumbler answer;		      

	i = j = 0;
	amant = aptr->mantissa;
	bmant = bptr->mantissa;
	answer.xvartumbler = 0;
	answer.varandnotfixed = 0;
	answer.sign = 0;
	ansmant = answer.mantissa;
	if (aptr->exp == bptr->exp) {
		answer.exp = aptr->exp;
		ansmant[0] = amant[0] + bmant[0];
		i = j = 1;
	} else if (aptr->exp > bptr->exp) {
		answer.exp = aptr->exp;
		temp = aptr->exp-bptr->exp;
		while ( i < temp ) {
			ansmant[j++] = amant[i++];
		}
		ansmant[j++] = amant[i++] + bmant[0];
		i = 1;
	} else {
		answer.exp = bptr->exp;
		temp = bptr->exp - aptr->exp;
		while (i <= temp) {
			ansmant[j++] = bmant[i++];
		}
	}

	while ( j <= NPLACES -1 ) {    
		ansmant[j++] = bmant[i++];
	}	 
	movetumbler (&answer, cptr);
	return;
}

#ifdef  OlDVeRsIon
absadd (aptr, bptr, cptr)
  tumbler *aptr, *bptr;
  tumbler *cptr;
{
  register INT i, j;
  INT place;
  INT temp;
  register tdigit *ansmant;
  register tdigit *bmant, *amant;
  tumbler answer;		      

	i = j = 0;
	amant = aptr->mantissa;
	bmant = bptr->mantissa;
	tumblerclear (&answer);
	ansmant = answer.mantissa;
	if (aptr->exp == bptr->exp) {
		answer.exp = aptr->exp;
		ansmant[0] = amant[0] + bmant[0];
		i = j = 1;
	} else if (aptr->exp > bptr->exp) {
		answer.exp = aptr->exp;
		temp = aptr->exp-bptr->exp;
		while ( i < temp ) {
			ansmant[j++] = amant[i++];
		}
		ansmant[j++] = amant[i++] + bmant[0];
		i = 1;
	} else {
		answer.exp = bptr->exp;
		temp = bptr->exp - aptr->exp;
		while (i <= temp) {
			ansmant[j++] = bmant[i++];
		}
	}

	while ( j <= NPLACES -1 ) {    
		ansmant[j++] = bmant[i++];
	}	 
	movetumbler (&answer, cptr);
	return;
}
 
#endif




strongsub (aptr, bptr, cptr)
  register tumbler *aptr, *bptr;
  tumbler *cptr;
{
  tumbler answer;
  register INT i, j;

	tumblerclear(&answer);
	if (tumblereq (aptr, bptr)) {
		movetumbler (&answer, cptr);
		return;
	}
	if (bptr->exp < aptr->exp) {
		movetumbler(aptr,cptr);
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

weaksub (aptr, bptr, cptr)
 register tumbler *aptr, *bptr;
  tumbler *cptr;
{
   tumbler answer;
  register INT i;
  INT expdiff;

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

 INT
tumblerintdiff (aptr, bptr)
  tumbler *aptr, *bptr;
{
  tumbler c;

	tumblersub (aptr, bptr, &c);
	return (c.mantissa[0]);
}

tumblerincrement (aptr, rightshift, bint, cptr)
  register tumbler *aptr, *cptr;
  INT bint, rightshift;
{
  register INT idx;

	if (iszerotumbler (aptr)) {
		tumblerclear (cptr);
		cptr->exp = -rightshift;
		cptr->mantissa[0] = bint;
		return;
	}
	if (aptr != cptr)
		movetumbler(aptr,cptr);
	for (idx = NPLACES ; aptr->mantissa[--idx] == 0 && idx > 0;);
	if (idx + rightshift >= NPLACES) {
#ifndef DISTRIBUTION
		dumptumbler (aptr);
		fprintf(stderr," idx = %d  rightshift = %d\n", idx, rightshift);
		gerror ("tumblerincrement overflow\n");
#else
		gerror("");
#endif
	}
	cptr->mantissa[idx + rightshift] += bint;
	tumblerjustify (cptr);
}

tumblertruncate (aptr, bint, cptr)
  tumbler *aptr, *cptr;
  INT bint;
{
  tumbler answer;
  INT i;

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

prefixtumbler (aptr, bint, cptr)
  tumbler *aptr, *cptr;
  INT bint;
{
  tumbler temp1, temp2;

	tumblerclear (&temp1);
	temp1.mantissa[0] = bint;
	movetumbler (aptr, &temp2);
	if (!iszerotumbler (&temp2)) /* yuckh! */
		temp2.exp -= 1;
	tumbleradd (&temp1, &temp2, cptr);
}

/*
beheadtumbler (aptr, bptr)
  tumbler *aptr, *bptr;
{
  tumbler temp;
  INT i;

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

docidandvstream2tumbler (docid, vstream, tumbleptr)
  tumbler *docid;
  tumbler *vstream;
  tumbler *tumbleptr;
{
  INT i, j;

	movetumbler (docid, tumbleptr);
	for (i = NPLACES-1; i >= 0; i--) {
		if (tumbleptr->mantissa[i]) {
			++i;
			break;
		}
	}
	for (j = 0; i < NPLACES && j < NPLACES;) {
		tumbleptr->mantissa[++i] = vstream->mantissa[j++];
	}
}

/*
tumblerclear(tumblerptr)
  tumbler *tumblerptr;
{
  static tumbler tumblerzero = 0;
	*tumblerptr = tumblerzero;// uses struct assignment in some compilers//
if (!iszerotumbler(tumblerptr))
  gerror("settumblertozero don't work\n");
}
*/
/*
tumblerclear (tumblerptr)
  tumbler *tumblerptr;
{
//	setmem (tumblerptr, sizeof (tumbler), 0);//
	clear (tumblerptr, sizeof (tumbler));
} 
*/




