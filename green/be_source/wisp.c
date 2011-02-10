/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* inwisp.d - integrated wid and dsp arithmetic routines */

#include "common.h"
#include "enf.h"

/* GRAN dsps across, POOM and SPAN dsp down */
dspadd (a, b, c, enftype)
  typedsp *a;   /* adds a dsp */
  typewisp *b;  /* to either a wid or a dsp */
  typedsp *c;   /* to get another dsp */
  INT enftype;
{
        lockadd (a->dsas, b->dsas, c->dsas, (unsigned)dspsize(enftype));
}


dspsub (a, b, c, enftype)
  typedsp *a;
  typewisp *b;
  typedsp *c;
  INT enftype;
{
        locksubtract (a->dsas, b->dsas, c->dsas, (unsigned)dspsize(enftype));
}

/*widequal(a,b)
  char//typewisp// *a,*b;
{
  INT i;
	for( i=0 ;i<widsize(enftype);i++){
		if(*a++ != *b++)
			return(FALSE);
	}
	return(TRUE);
}*/
/*              These two routines are presently macros
  INT
dspsize (enftype)
  SINT enftype;
{
       switch (enftype) {
          case GRAN:
                return (DSPSIZEGR);
          case POOM:
                return (DSPSIZEPM);
          case SPAN:
                return (DSPSIZESP);
          default:
#ifndef DISTRIBUTION
                gerror ("dspsize: bad enftype\n");
#else
		gerror("");
#endif
        }
        return (-1);
}

  INT
widsize (enftype)
  SINT enftype;
{
        switch (enftype) {
          case GRAN:
                return (WIDSIZEGR);
          case POOM:
                return (WIDSIZEPM);
          case SPAN:
                return (WIDSIZESP);
          default:
#ifndef DISTRIBUTION
                gerror ("widsize: bad enftype\n");
#else
		gerror("");
#endif
        }
        return (-1);
}
*/

/* Should be called whenever the wisp of a crum may need to
**   be recalculated by looking at its son.
*/
setwispupwards (ptr,testflag)
  typecuc *ptr;
  INT testflag;
{
  typecuc *father, *findfather(),*oldptr;
  bool setwisp();
  bool changed;
  INT ntimeschanged;
	ntimeschanged = 0;
/*checkwholesubtree(ptr);*/
/*asserttreeisok(ptr);*/
        if (!ptr)
                return(0);
        oldptr = ptr;
        for (changed = TRUE;changed && ptr; ptr = father) {
                father = findfather ((typecorecrum*)ptr);
		changed = setwisp ((typecorecrum*)ptr);
		if (changed) {
			ntimeschanged +=1;
		}
        } 
	if (ntimeschanged) {
                ivemodified((typecorecrum*)oldptr);
        }
#ifndef DISTRIBUTION
if(testflag){
asserttreeisok(oldptr);
}
#endif
	return /**/(0!=ntimeschanged)/**/;
}

setwispsofsons(ptr)
  typecuc *ptr;
{
	for(ptr = (typecuc *)findleftmostbro((typecorecrum*)ptr); ptr; ptr = (typecuc *)getrightbro((typecorecrum*)ptr)){
		 setwisp ((typecorecrum*)ptr);
	}
}

/* the widditive operation in general */
  bool  /* return whether wisp of ptr has changed */
setwisp (ptr)
  typecorecrum *ptr;
{
  bool setwidseq(),setwispnd();
        if(ptr->height == 0)
                return (FALSE);
        switch (ptr->cenftype) {
          case GRAN:
                return (setwidseq ((typecuc*)ptr));
          case SPAN:
          case POOM:
                return (setwispnd ((typecuc*)ptr));
          default:
#ifndef DISTRIBUTION
                gerror ("setwisp: bad enftype\n");
#else
		gerror("");
#endif
        }
        return (-1); /* Oh, the things one has to do to make computers happy */
}

/* the local widditive operation in sequential enfilades */
/*widopseq (aptr, bptr, cptr)
  typewid *aptr,*bptr,*cptr;
{
        lockadd (aptr->dsas, bptr->dsas, cptr->dsas, widsize(GRAN));
}*/
#define widopseq(a,b,c) lockadd((a)->dsas,(b)->dsas,(c)->dsas,widsize(GRAN))
/* the widditive operation in sequential enfilades */
  bool
setwidseq (father)
  typecuc *father;
{
  typecorecrum *ptr;
  typewid sum;
  bool lockeq();

        if (father->height == 0) {
                return (FALSE);
        }
        clear (&sum, sizeof (sum));
        for (ptr = findleftson (father); ptr; ptr = getrightbro (ptr)){
                  widopseq (&sum, &ptr->cwid, &sum);
        }
        if (lockeq (sum.dsas, father->cwid.dsas, (unsigned)widsize(father->cenftype)))
                return (FALSE);
        movewisp (&sum, &father->cwid);
        ivemodified ((typecorecrum*)father); /* zzzz */
        return (TRUE);
}

/* the widditive operation for nd */                                     
  bool
setwispnd (father)
  typecuc *father;
{
  typecorecrum *ptr;
  typedsp newdsp, mindsp;
  typewid newwid,tempwid;
  bool lockeq(),iszerolock();
  bool lockiszerop;
  bool somethingchangedp = FALSE; /* mindsp != 0 or some tempwid !=0 */

	if(father->height ==  0){
		return(FALSE);;
	}

        /* remember original so can tell if changed */
        if ((ptr = findleftson (father)) == NULL) {
#ifndef DISTRIBUTION
gerror("in setwispnd null findleftson\n");
                clear (&father->cdsp, sizeof(father->cdsp));
                clear (&father->cwid, sizeof(father->cwid));
                ivemodified((typecorecrum*)father);
                return (TRUE);
#else
		gerror("");
#endif
        }
        /* find new upper-left corner */
        movewisp (&ptr->cdsp, &mindsp);
        for (ptr = getrightbro(ptr); ptr; ptr = getrightbro (ptr))   {
                lockmin ((tumbler*)&mindsp, (tumbler*)&ptr->cdsp, (tumbler*)&mindsp, (unsigned)dspsize(ptr->cenftype));
        }                       
        lockiszerop = iszerolock((tumbler*)&mindsp, (unsigned)dspsize(father->cenftype)); 
        if(!lockiszerop){
		somethingchangedp = TRUE;
                dspadd (&father->cdsp, &mindsp, &newdsp, (INT)father->cenftype);
        }else{
                movewisp(&father->cdsp,&newdsp);               
        }
        /* find new lower-right corner at the same time that */
        /*  we are readjusting the son's dsps to compensate */
        /*  for change in the father */
        clear (&newwid, sizeof(newwid));
        for (ptr = findleftson (father); ptr; ptr = getrightbro (ptr)) {
          if (!lockiszerop) {
              ptr->modified = TRUE;   /* father gets ivemodified soon */
              dspsub(&ptr->cdsp, &mindsp, &ptr->cdsp, (INT)ptr->cenftype);
          }
          lockadd((tumbler*)&ptr->cdsp, (tumbler*)&ptr->cwid, (tumbler*)&tempwid,(unsigned)widsize(ptr->cenftype));
          lockmax((tumbler*)&newwid, (tumbler*)&tempwid, (tumbler*)&newwid, (unsigned)widsize(ptr->cenftype));
        }
	if (!somethingchangedp && !lockeq((tumbler*)&newwid, (tumbler*)&father->cwid, (unsigned)widsize(father->cenftype))) {
		somethingchangedp = TRUE;
	}
        /* report whether anything changed */
        if(!somethingchangedp){
                didntchangewisps();
                return (FALSE); 
        }
        movewisp (&newdsp, &father->cdsp);
        movewisp (&newwid, &father->cwid);
	ivemodified((typecorecrum*)father);
        return (TRUE);
}

didntchangewisps()
{
}

/* reset father's wid but leave dsp alone */
setwidnd (father)
  typecuc *father;
{
  typecorecrum *ptr;
  typewid newwid;
        for (ptr = findleftson (father); ptr; ptr = getrightbro (ptr)) {
                clear (&newwid, sizeof(newwid));
                lockmax ((tumbler*)&newwid, (tumbler*)&ptr->cwid, (tumbler*)&newwid, (unsigned)widsize(ptr->cenftype));
        }
        if (!lockeq((tumbler*)&father->cwid, (tumbler*)&newwid, (unsigned)widsize(father->cenftype))) {
                movewisp (&newwid, &father->cwid);
                ivemodified ((typecorecrum*)father); /* zzz */
        }     
}


/* --------- lock routines deal with full lock ------------- */
/* a lock is an array of loxize tumblers */

  bool
iszerolock (lock, loxize)
  tumbler *lock;
  unsigned loxize;
{
        while (loxize--)
               if (!iszerotumbler (lock++))
                        return(FALSE);
        return(TRUE);
}

  bool
lockeq (lock1, lock2, loxize)
  tumbler *lock1, *lock2;
  register unsigned loxize;
{
        while (loxize--)
               if (!tumblereq (lock1++, lock2++))
                        return(FALSE);
        return(TRUE);
}

lockadd (lock1, lock2, lock3, loxize)
  register tumbler *lock1, *lock2, *lock3; /* arrays of tumblers */
  register unsigned loxize;
{
        while (loxize--)
                tumbleradd (lock1++, lock2++, lock3++);
}

locksubtract (lock1, lock2, lock3, loxize)
  tumbler *lock1, *lock2, *lock3;
  unsigned loxize;
{
        while (loxize--)
                tumblersub (lock1++, lock2++, lock3++);
}

lockmin (lock1, lock2, lock3, loxize)
  tumbler *lock1, *lock2, *lock3;
  unsigned loxize;
{
        while (loxize--){
                macrotumblermin (lock1, lock2, lock3);
                lock1++; lock2++; lock3++;
	}
}

lockmax (lock1, lock2, lock3, loxize)
  tumbler *lock1, *lock2, *lock3;
  unsigned loxize;
{
        while (loxize--){
                macrotumblermax (lock1, lock2, lock3);
                lock1++; lock2++; lock3++;
	}
}

/* Returns whether ALL the lock is 1 story */
  bool
lockis1story (lock, loxize)
  tumbler *lock;
  unsigned loxize;
{
        while (loxize--)
                if (!is1story (lock++))
                        return (FALSE);
        return (TRUE);
}
/*setwispallthewayupwards (ptr)
  typecuc *ptr;
{
  typecuc *father, *findfather(),*oldptr;
  bool setwisp();
  bool changed;
        if (!ptr)
                return;
        oldptr = ptr;
        for (; ptr; ptr = father) {
                father = findfather (ptr);
			for(ptr = findleftmostbro(ptr);ptr;ptr= getrightbro(ptr)){
				 setwisp (ptr);
			}
        } 
        if(TRUE&&changed){
                ivemodified(oldptr);
        }
}
*/
