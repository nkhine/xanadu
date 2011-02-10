/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* ndinters.d - nd enfilade cut routines - file #2 */

#include "crum.h"
#include "ndenf.h"

/* given fullcrumptr & knives
** sets *ptrptr & *offset to be cuc which contains all
**   space between the cuts
*/
/*findintersectionnd (fullcrumptr, knives, ptrptr, offset)
  typecuc *fullcrumptr;
  typeknives *knives;
  typecuc **ptrptr;
  typewid *offset;
{
  typecorecrum *ptr, *next;
  typecorecrum *findthecutsonnd();

	clear (offset, sizeof(*offset));
	ptr = fullcrumptr;
	for (;;) {  // descend down the generations //
		if ((next = findthecutsonnd (ptr, offset, knives)) == NULL)
			break;
		lockadd (offset, &ptr->cdsp, offset, dspsize (ptr->cenftype));
		ptr = next;
	}
	*ptrptr = ptr;
	// offset is already set appropriately //
}*/
newfindintersectionnd (fullcrumptr, knives, ptrptr, offset)
  typecuc *fullcrumptr;
  typeknives *knives;
  typecuc **ptrptr;
  typewid *offset;
{
	*ptrptr = fullcrumptr;
	clear (offset,sizeof(*offset));
}
/*  typecorecrum *
findthecutsonnd (father, offset, knives)
  typecorecrum *father;
  typewid *offset;
  typeknives *knives;
{
  typedsp grasp;
  typecorecrum *ptr, *cutcrum;
  bool allcutswiththiscrumnd();
  bool cutinthiscrumnd();

	// note that intersection can't be bottom crum //
	if (father->height == 1 || (ptr = findleftson (father)) == NULL)
		return (NULL);
	prologuend (father, offset, &grasp, (typedsp *)NULL);
	cutcrum = NULL;
	for (; ptr; ptr = findrightbro (ptr)) {
		if (cutinthiscrumnd (ptr, &grasp, knives)) {
			if (cutcrum) // cuts thru > 1 //
				return (NULL);
			  else  // first that cuts thru //
				cutcrum = ptr;
		}
	}
	if (!cutcrum) // cuts thru none of them //
		return (NULL);
	if (!allcutswiththiscrumnd (cutcrum, &grasp, knives))
		return (NULL);  // missed some somehow //
	return (cutcrum);
}
*/
/* is TRUE if satisfied on ANY cuts
** is ONLY satisfied by cuts within
*/
  bool
cutinthiscrumnd (ptr, offset, knives)
  typecorecrum *ptr;
  typewid *offset;
  typeknives *knives;
{
  INT i;

	for (i = 0; i < knives->nblades; ++i) {
		if (whereoncrum(ptr, offset, &knives->blades[i], knives->dimension) == THRUME)
			return(TRUE);
	}
	return(FALSE);
}

/* is TRUE only if satisfied on ALL cuts.
** is satisfied by cut at edge as well as within.
*/

/* //new version//  bool
allcutswiththiscrumnd (ptr, offset, knives)
  typecorecrum *ptr;
  typewid *offset;
  typeknives *knives;
{
  INT i,cmp;

    for (i = 0; i < knives->nblades; ++i) {
	cmp = whereoncrum (ptr, offset, &knives->blades[i], knives->dimension);
	     if (cmp == TOMYLEFT || cmp == TOMYRIGHT)
		    return (FALSE);
    }
    return (TRUE);
}*/	       

  bool     /* old version*/
allcutswiththiscrumnd (ptr, offset, knives)
  typecorecrum *ptr;
  typewid *offset;
  typeknives *knives;
{
  tumbler cut;
  INT i,cmp;


	for (i = 0; i < knives->nblades; ++i) {
		tumblercopy (&knives->blades[i], &cut);
		cmp = whereoncrum (ptr, offset, &cut, knives->dimension);
		if (cmp == TOMYLEFT || cmp == TOMYRIGHT)
			return (FALSE);
	}
	return (TRUE);
}
