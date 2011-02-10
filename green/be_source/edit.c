/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* edit.d -  crum rearrange and deletion routines */

#include "common.h"
#include "enf.h"
#include "ndenf.h"

/* use with GRAN */
deleteseq (fullcrumptr, address, index)
  typecuc *fullcrumptr;
  tumbler *address;
  INT index;
{
  typecrumcontext *context;
  typecbc *ptr;
  typecrumcontext *retrievecrums();     

	context = retrievecrums (fullcrumptr, address, index);
	ptr = (typecbc *) context->corecrum;
	freecrum ((typecorecrum*)&context->corecrum);
	crumcontextfree(context);
	setwispupwards(findfather((typecorecrum*)ptr), 1);
	recombine(findfather((typecorecrum*)ptr));
}

/* use with SPAN and POOM */
deletend (fullcrumptr, origin, width, index)
  typecuc *fullcrumptr;
  tumbler *origin, *width;
  INT index;
{
  typeknives knives;
  typewid offset, grasp, reach;
  typecuc *father, *ptr, *next;
  typewid foffset, fgrasp;

	clear (&offset, sizeof(offset)); /* fullcrum alway has zero offset */
	prologuend ((typecorecrum*)fullcrumptr, &offset, &grasp, &reach);
	movetumbler (origin, &knives.blades[0]);
	tumbleradd (origin, width, &knives.blades[1]);
	knives.nblades = 2;
	knives.dimension = index;
	makecutsnd (fullcrumptr, &knives);
	newfindintersectionnd (fullcrumptr, &knives, &father, &foffset);
	prologuend ((typecorecrum*)father, &foffset, &fgrasp, (typedsp*)NULL);
	for (ptr = (typecuc *) findleftson (father); ptr; ptr = next) {
		next = (typecuc *) findrightbro((typecorecrum*)ptr);
		switch (deletecutsectionnd ((typecorecrum*)ptr, &fgrasp, &knives)) {
		  case -1:
#ifndef DISTRIBUTION
			gerror ("deletend can't classify crum\n");
#else
			gerror("-1");
#endif
		  case 0:
			break;
		  case 1:
			disown ((typecorecrum*)ptr);
			subtreefree ((typecorecrum*)ptr);
			break;
		  case 2:
			tumblersub (&ptr->cdsp.dsas[index], width, &ptr->cdsp.dsas[index]);
			/* This will get set to modified in setwisp */
			break;
		  default:
#ifndef DISTRIBUTION
			gerror ("unexpected cutsection\n");
#else
			gerror("");
#endif
		}
	}
	setwispupwards (father,1);
	recombine (father);
}

rearrangend (fullcrumptr, cutseqptr, index)
  typecuc *fullcrumptr;
  typecutseq *cutseqptr;
  INT index;
{
  typecuc *father, *ptr;
  typewid foffset, fgrasp;
  typeknives knives;
  tumbler diff[4];
  INT i;
  bool splitcrumupwards();
#ifdef UNdeFined
	fprintf(stderr,"entering rearrangend\n");
          /**/fixincoresubtreewids(fullcrumptr);/*1999 // a temp kluge zzz till we find where setwisp isnt called//*/ /* this is a brute  force kluge, if this fixes anything it means that the wids aren't being set properly somplace else probably near here */
        fprintf(stderr,"in rearrangend \n");
     switch (fullcrumptr->cenftype) {
       case POOM:
        fprintf(stderr,"in rearrangend  dumppoomwisps\n");
        dumppoomwisps(fullcrumptr);
	break;
       case SPAN:
        fprintf(stderr,"in rearrangend  showspanf\n");
        showspanf(fullcrumptr);
	break;
     }
#endif
/*displaycutspm(cutseqptr); */
	knives.dimension = index;
	knives.nblades = cutseqptr->numberofcuts;
	for (i = 0; i < knives.nblades; i++) {
		movetumbler (&cutseqptr->cutsarray[i], &knives.blades[i]);
	}
	sortknives (&knives);
	makeoffsetsfor3or4cuts (&knives, diff);
/*for(i=1;i<=knives.nblades;i++){fprintf(stderr,"\noffset for cut %d = ",i);dumptumbler(&diff[i]);}*/
	makecutsnd (fullcrumptr, &knives);
	newfindintersectionnd (fullcrumptr, &knives, &father, &foffset);
	prologuend ((typecorecrum*)father, &foffset, &fgrasp, (typedsp*)NULL);
	for (ptr = (typecuc*)findleftson(father); ptr; ptr = (typecuc *)findrightbro((typecorecrum*)ptr)) {
		i = rearrangecutsectionnd((typecorecrum*)ptr, &fgrasp, &knives);
		switch (i) {
		  case -1:
#ifndef DISTRIBUTION
			gerror ("rearrangend can't classify crum\n");
#else
			gerror("-1");
#endif
		  case 0:  case 4: /* these never move */
			break;
		  case 1:  case 2:  case 3: /* 3 only moves in 4 cuts */
			tumbleradd (&ptr->cdsp.dsas[index], &diff[i], &ptr->cdsp.dsas[index]);
/*fprintf(stderr,"\nptr = %x i = %d ",ptr,i); dump(ptr); */
			ivemodified((typecorecrum*)ptr);	       
			break;
		  default:
#ifndef DISTRIBUTION
			gerror ("unexpected cutsection\n");
#else
			gerror("");
#endif
		}
	}
	setwispupwards (father,1); /* should do nothing, */
			/* but, just on general principles.. */
	recombine (fullcrumptr);

	(void)splitcrumupwards(fullcrumptr);/* can we move this before recombine ?*/
#ifdef UnDEfINed
	fprintf(stderr,"leaving rearrangend\n");

          /**/fixincoresubtreewids(fullcrumptr);/*1999 // a temp kluge zzz till we find where setwisp isnt called//*/ /* this is a brute  force kluge, if this fixes anything it means that the wids aren't being set properly somplace else probably near here */
        fprintf(stderr,"in rearrangend \n");
     switch (fullcrumptr->cenftype) {
       case POOM:
        fprintf(stderr,"in rearrangend  dumppoomwisps\n");
        dumppoomwisps(fullcrumptr);
	break;
       case SPAN:
        fprintf(stderr,"in rearrangend  showspanf\n");
        showspanf(fullcrumptr);
	break;
     }
#endif


}

/* Prepares offsets for both 3 and 4 cut rearranges */

makeoffsetsfor3or4cuts (knives, diff)
  typeknives *knives;
  tumbler diff[];
{
  tumbler a, b;

	/* diff[0] is simply ignored */
	if (knives->nblades == 4) {
		tumblersub (&knives->blades[2], &knives->blades[0], &(diff[1]));
		tumblersub (&knives->blades[1], &knives->blades[0], &a);
		tumblersub (&knives->blades[3], &knives->blades[2], &b);
		tumblersub (&b, &a, &(diff[2]));
/*		tumblersub (&knives->blades[0], &knives->blades[2], &(diff[3]));*//* should be <0 */
		movetumbler (&diff[1], &diff[3]);
		diff[3].sign = !diff[1].sign;
	} else if (knives->nblades == 3) {
		tumblersub (&knives->blades[2], &knives->blades[1], &diff[1]);
		tumblersub (&knives->blades[1], &knives->blades[0], &diff[2]); /* should be negative */
		diff[2].sign = !diff[2].sign;
		tumblerclear (&(diff[3]));
	} else
		gerror ("Wrong number of cuts.");
}

/* Returns which between-cut slice the crum is in. Does this
**   by finding the first cut to the right of the crum
** Each editting routine requires a slightly different version of
**   this function.
*/
  INT
rearrangecutsectionnd (ptr, offset, knives)
  typecorecrum *ptr;
  typewid *offset;
  typeknives *knives;
{
  INT i, cmp;

	for (i = knives->nblades -1; i >= 0 ; --i) {
		cmp = whereoncrum (ptr, offset, &knives->blades[i], knives->dimension);
		if (cmp == THRUME) {
			return (-1);
		} else if (cmp <= ONMYLEFTBORDER) {
			return (i+1);
		}
	}
	return (0);
}  


  INT
insertcutsectionnd (ptr, offset, knives)
  typecorecrum *ptr;
  typewid *offset;
  typeknives *knives;
{
  INT i, cmp;		       /*hacked from delete*/

	/*for (i = knives->nblades-1; i >= 0; --i){*/
	if (knives->nblades == 2) {
	      i = 1;
	      cmp = whereoncrum(ptr, offset, &knives->blades[i], knives->dimension);
	      if (/*FALSE&&*/cmp == THRUME) {
			dumpwid(&ptr->cwid.dsas[i],ptr->cenftype);
		       return (-1);
	      } else if (cmp <= ONMYLEFTBORDER)/* compare last to first */{
		       return (2);
	      }
	}
	      i = 0;
	      cmp = whereoncrum(ptr, offset, &knives->blades[i], knives->dimension);
	      if (cmp == THRUME) {
		       dumpwid(&ptr->cwid.dsas[i],ptr->cenftype);
		       return (-1);
	      } else if (cmp <= ONMYLEFTBORDER)/* compare last to first */{
		       return (1);
	      }
	
       /* }*/
	return (0);
}   
   
  INT
deletecutsectionnd (ptr, offset, knives)
  typecorecrum *ptr;
  typewid *offset;
  typeknives *knives;
{
  INT i, cmp;

	for (i = knives->nblades-1; i >= 0; --i)/* unique to delete */{
	       cmp = whereoncrum(ptr, offset, &knives->blades[i], knives->dimension);
	       if (cmp == THRUME) {
		       return (-1);
	       } else if (cmp <= ONMYLEFTBORDER)/* compare last to first */{
		       return (i+1);
	       }
	}
	return (0);
}   

sortknives (knifeptr)
  typeknives *knifeptr;
{
  tumbler temp;
  INT i;

     for (i = 0; i < knifeptr->nblades-1; ++i) {
	   if (tumblercmp(&knifeptr->blades[i], &knifeptr->blades[i+1]) == GREATER) {
		       movetumbler(&knifeptr->blades[i+1], &temp);
		       movetumbler(&knifeptr->blades[i], &knifeptr->blades[i+1]);
		       movetumbler(&temp, &knifeptr->blades[i]);
		       --i;
	   }
     }
}
