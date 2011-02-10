/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:      fetake.d
***************************************************/

/* frontend take -- communications with the backend */

#include "fest.h"

#define WORDELIM '~'
#define TUMDELIM '.'
#define SPANFLAG 's'
#define VSPECFLAG 'v'
#define TEXTFLAG 't'

char pullc();

  bool
eatchar (taskptr, c)
  tasktype *taskptr;
  char c;
{
  char m;

        if ((m = pullc (taskptr)) != c) {
                pushc (taskptr, m);
                return (FALSE);
        } else
                return (TRUE);
}

  bool
takenum (taskptr, numptr)
  tasktype *taskptr;
  long *numptr;
{
  char c;
  long num;
  bool flag;

        num = 0;
        flag = FALSE;
        while ((c = pullc (taskptr)) && isdigit(c)) {
                num = num * 10 + c - '0';
                flag = TRUE;
        }
        pushc (taskptr, c);
        *numptr = num;
        return (flag);
}

  bool
takenumber (taskptr, numptr)
  tasktype *taskptr;
  long *numptr;
{
  char c;
  long num;
  bool flag;

/*
 fprintf(stderr,"takenumber ");
*/
        num = 0;
        flag = FALSE;
        while ((c = pullc (taskptr))&& isdigit(c)) {
                num = num * 10 + c - '0';
                flag = TRUE;
        }

        *numptr = num;
/*
 fprintf(stderr,"num=%d ret=%d\n",num,flag && (c == WORDELIM) );
*/
        return (flag && (c == WORDELIM));
}

  bool
taketumbler (taskptr, tumblerptr)
  tasktype *taskptr;
  tumbler *tumblerptr;
{
  bool dotaketumbler();
        tumblerclear (tumblerptr);
        return (dotaketumbler (taskptr, tumblerptr, NPLACES));
}

  bool
takesmalltumbler (taskptr, smalltumblerptr)
  tasktype *taskptr;
  smalltumbler *smalltumblerptr;
{
  bool dotaketumbler();
        smalltumblerclear (smalltumblerptr);
        return(dotaketumbler(taskptr,(tumbler *)smalltumblerptr,SMALLNPLACES));
}

  bool
dotaketumbler (taskptr, tumblerptr, size)
  tasktype *taskptr;
  tumbler *tumblerptr;
  med size;
{
  med i;
  long num, value;
  bool taketdigit(), takenum();


        takenum (taskptr, &num);
        tumblerptr->exp =(med) -num;
        for (i = 0; taketdigit (taskptr, &value); i++) {
                if (i > size) {
                       error (taskptr, "taketumbler overflow\n");
                       return (FALSE);
                }
                tumblerptr->mantissa[i] = value;
        }
        return (( pullc (taskptr)) == WORDELIM);
}

  bool
taketdigit (taskptr, valueptr)
  tasktype *taskptr;
  long *valueptr;
{
  char c;

        if ((c = pullc (taskptr)) != TUMDELIM) {
                pushc (taskptr, c);
                return (FALSE);
        }
        return (takenum (taskptr, valueptr));
}

  bool
takespecset (taskptr, specsetptr)
  tasktype *taskptr;
  spectype **specsetptr;
{
  long num;
  spectype *specset;
  char c;
  tumbler x;
  bool taketumbler(), takespec();

        *specsetptr = NULL;
        if (!takenumber (taskptr, &num))
                return (FALSE);
        while (num--) {
                c = pullc (taskptr);
                                 /*  incomprehensible kludge - remove when
                                     we have code to deal with garbage from
                                     the back end                           */
                if (c == SPANFLAG) {
                        if (
                           pullc (taskptr) == WORDELIM
                        && taketumbler (taskptr, &x)
                        && taketumbler (taskptr, &x)){
                                continue;
                          }else{	
                                return (FALSE);
			}
                }
                if (
                   c != VSPECFLAG
                || pullc (taskptr) != WORDELIM){
                        return (FALSE);
		}
                specset = (spectype *) taskalloc (taskptr, sizeof(spectype));
                specset->typeofthing = SPECTYPE;
                if (!takespec (taskptr, specset)){
                        return(FALSE);
		}
                *specsetptr = specset;
                specsetptr = &specset->nextspec;
        }
        return (TRUE);
}

  bool
takespec (taskptr, specptr)
  tasktype *taskptr;
  spectype *specptr;
{
  bool takespanset();
  char c;
        specptr->nextspec = NULL;
        return (
           taketumbler (taskptr, &specptr->docid)
        && takespanset (taskptr, &specptr->specspanptr));
}

  bool
takespanset (taskptr, spansetptr)
  tasktype *taskptr;
  spantype **spansetptr;
{
  spantype *spanset;
  long num;
  bool takespan();
/* fprintf(stderr,"takespanset "); */
        *spansetptr = NULL;
        if (!takenumber (taskptr, &num))
                return (FALSE);
/* fprintf(stderr,"%d\n",num); */
        while (num--) {
                spanset = (spantype *) taskalloc (taskptr, sizeof(spantype));
                spanset->typeofthing = SPANTYPE;
                if (!takespan (taskptr, spanset))
                        return(FALSE);
/* dumpsmalltumbler(&spanset->vstartaddress); */
                *spansetptr = spanset;
                spansetptr = &spanset->nextspan;
        }
/* fprintf(stderr," returning truly\n"); */
        return (TRUE);
}

  bool
takeaddressset (taskptr, addresssetptr, nptr)
  tasktype *taskptr;
  tumbleritemtype **addresssetptr;
  long *nptr;
{
  tumbleritemtype *addressset;
  long num;
  bool takenumber(), takeaddress();

        *addresssetptr = NULL;
        if (!takenumber (taskptr, &num))
                return (FALSE);
        if (nptr)
                *nptr = num;
/*
 fprintf(stderr,"takeaddressset %ld\n",num);
*/
        while (num--) {
                addressset = (tumbleritemtype *) taskalloc (taskptr, sizeof(tumbleritemtype));
                if (!takeaddress (taskptr, addressset))
                        return(FALSE);
                *addresssetptr = addressset;
                addresssetptr = &addressset->nexttumbleritem;
        }
        return (TRUE);
}
  

  bool
takeaddress (taskptr, addressptr)
  tasktype *taskptr;
  tumbleritemtype *addressptr;
{
  bool taketumbler();

        if (!taketumbler (taskptr, &addressptr->value))
                return(FALSE); 
        addressptr->nexttumbleritem = NULL;
        return (TRUE);
}

  bool
takespan (taskptr, spanptr)
  tasktype *taskptr;
  spantype *spanptr;
{
  smalltumbler temp;
  bool taketumbler();

        spanptr->nextspan = NULL;
        if (!(
           takesmalltumbler (taskptr, &spanptr->vstartaddress)
        && takesmalltumbler (taskptr, &temp)
        && temp.exp == -1)){
                 return (FALSE);
        }
        spanptr->sizeofspan = temp.smallmantissa[0];
        return (TRUE);
}

  bool
taketext (taskptr, textptr)
  tasktype *taskptr;
  charspantype *textptr;
{
  bool takenumber();
  char *charptr;
  long i;
/*printf ("taketext\n");
*/
        if (!takenumber (taskptr, &textptr->numberofcharactersinspan))
                return (FALSE);

/*printf("%d chars\n", textptr->numberofcharactersinspan);
*/
        textptr->nextcharspan = NULL;
        charptr = (char *)taskalloc (taskptr,(med) textptr->numberofcharactersinspan);
        textptr->charinspanptr = charptr;
 /*       for (i = 0; i < textptr->numberofcharactersinspan; ++i) {
                *(charptr++) = pullc (taskptr);
        }
        *charptr = '\0';
*/
	pullblock(taskptr,charptr,(nat)textptr->numberofcharactersinspan);
/*
 fprintf(stderr, "%s\n", textptr->charinspanptr);
*/
        return (TRUE);
}

  bool
takevstuff (taskptr, textsetptr, addresssetptr)
  tasktype *taskptr;
  charspantype **textsetptr;
  tumbleritemtype **addresssetptr;
{
  charspantype *textset;
  tumbleritemtype *addressset;
  long num;
  char c;
  bool takenumber(), taketext(), takeaddress();

        *textsetptr = NULL;
        *addresssetptr = NULL;
        if (!takenumber (taskptr, &num)) {
/*
 fprintf (stderr,"takevstuff true\n");
*/
                return (FALSE);
	}
        while (num--) {
                if ((c = pullc (taskptr)) == TEXTFLAG) {
                        textset = (charspantype *) taskalloc (taskptr, sizeof(charspantype));
                        if (!taketext (taskptr, textset))
                                return (FALSE);
                        *textsetptr = textset;
                        textsetptr = &textset->nextcharspan;
                } else {
                        pushc (taskptr, c);
                        addressset = (tumbleritemtype *) taskalloc (taskptr,sizeof(tumbleritemtype));
                        if (!takeaddress (taskptr, addressset))
                                return (FALSE);
                        *addresssetptr = addressset;
                        addresssetptr = &addressset->nexttumbleritem;
                }
        }
/*
 fprintf (stderr,"takevstuff true\n");
*/
        return (TRUE);
}

  bool
takecorrespondenceset (taskptr, correspondencesetptr)
  tasktype *taskptr;
  correspondencetype **correspondencesetptr;
{
  long num;
  correspondencetype *correspondence;

        *correspondencesetptr = NULL;
        if (!takenumber (taskptr, &num))
                return (FALSE);
        while (num--) {
                correspondence = (correspondencetype *) falloc ( sizeof (correspondencetype));
                if (!taketumbler (taskptr, &correspondence->stream[0]))
                        return (FALSE);
                if (!taketumbler (taskptr, &correspondence->stream[1]))
                        return (FALSE);
                if (!taketumbler (taskptr, &correspondence->widthofspan))
                        return (FALSE);
                *correspondencesetptr = correspondence;
                correspondencesetptr = &correspondence->nextcorrespondence;
        }
        *correspondencesetptr = NULL;
        return (TRUE);
}

/* -------------- top level take routines ----------------- */


  bool
takecreatelink (taskptr, linkid)
  tasktype *taskptr;
  tumbler *linkid;
{
        return (taketumbler (taskptr, linkid));
}

  bool
takefollowlink (taskptr, specptrptr)
  tasktype *taskptr;
  spectype **specptrptr;
{
  bool temp;
        temp = takespecset (taskptr, specptrptr);
	if(*specptrptr){
		if(!(*specptrptr)->specspanptr){
                    fprintf(stderr,"former gerror takefollowlink no spanset");
		}
	}
	return(temp);
}

  bool
takecreatenewversion (taskptr, docid)
  tasktype *taskptr;
  tumbler *docid;
{
        return (taketumbler (taskptr, docid));
}
  bool
takecreatenewdocument (taskptr, docid)
  tasktype *taskptr;
  tumbler *docid;
{
        return (taketumbler (taskptr, docid));
}

  bool
takeretrievedocvspanset (taskptr, spanptrptr)
  tasktype *taskptr;
  spantype **spanptrptr;
{
        return (takespanset (taskptr, spanptrptr));
}

  bool
takeretrievedocvspan (taskptr, spanptr)
  tasktype *taskptr;
  spantype *spanptr;
{
        return (takespan (taskptr, spanptr));
}


  bool
takeretrievev (taskptr, textptrptr, addressptrptr)
  tasktype *taskptr;
  charspantype **textptrptr;
  tumbleritemtype **addressptrptr;
{

        return (takevstuff (taskptr, textptrptr, addressptrptr));
}

  bool
takefindlinksfromtothree (taskptr, linkptrptr)
  tasktype *taskptr;
  tumbleritemtype **linkptrptr;
{
/*
 fprintf(stderr,"takefindlinksfromtothree\n");
*/
        return (takeaddressset (taskptr, linkptrptr, (long *)NULL));
}

#ifdef COMMENT
  bool
takefindnumoflinksfromto (taskptr, nptr)
  tasktype *taskptr;
  long *nptr;
{
        return (takenumber (taskptr, nptr));
}

  bool
takefindnextnlinksfromto (taskptr, linkptrptr, nptr)
  tasktype *taskptr;
  tumbleritemtype *linkptrptr;
  long *nptr;
{

        return (takeaddressset (taskptr, (tumbleritemtype **)linkptrptr, nptr));
}
#endif COMMENT

  bool
takeretrieveendsets (taskptr, fromsetptr, tosetptr, threesetptr)
  tasktype *taskptr;
  spectype **fromsetptr, **tosetptr, **threesetptr;
{
  spectype *threeset;
  bool result;

        if (!(
           takespecset (taskptr, fromsetptr)
        && takespecset (taskptr, tosetptr))){
		gerror("takerieveendsets error in takspecset \n");
                return (FALSE);
	}
	result = takespecset (taskptr, &threeset);	/* MSM 6/26 */
        if (threesetptr){
		*threesetptr = threeset;
	} else if (threeset != NULL) {
/*
 dumpvmthing(threeset);
*/
/*		specfree (threeset);*/ /* Should free  ECH 6/27 */
	}
  	return result;
}


  bool
takeshowrelationof2versions (taskptr, correspondencesetptr)
  tasktype *taskptr;
  correspondencetype **correspondencesetptr;
{
        return (takecorrespondenceset (taskptr, correspondencesetptr));
}


  bool
takefinddocscontaining (taskptr, addressptrptr,numptr)
  tasktype *taskptr;
  tumbleritemtype **addressptrptr;
  long * numptr;
{
        return (takeaddressset (taskptr, addressptrptr, numptr));
}

  bool
takeopen(taskptr,resultp)
  tasktype *taskptr;
  tumbler *resultp;
{
	taketumbler(taskptr,resultp);
}

  bool
takediskexit(taskptr)
  tasktype *taskptr;
{
  long n;
	takenumber(taskptr,&n);
}

#define PROTOLEN 8

  bool
takeprotocol(taskptr,proto)
  tasktype *taskptr;
  char proto[];
{
  register med i;
  char c;
        if (!( eatchar(taskptr,'\n') && eatchar(taskptr,'P'))) {
		return(FALSE);
	}
	for (i=0; i<PROTOLEN; i++) {
		proto[i] = '\0';
                if ((c = pullc(taskptr)) == WORDELIM)
			break;
		proto[i] = c;
	}
	return(i==PROTOLEN ? FALSE : TRUE);
}
