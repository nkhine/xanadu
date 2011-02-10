/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* granf1.d - Udanax granfilade calls */
int backenddaemon = 0; 
#include "xanadu.h"
#include "players.h"

extern bool isxumain;

  bool
findorgl (taskptr, granfptr, isaptr, orglptr, type)/*BERT*/
  typetask *taskptr;
  typegranf granfptr;
  typeisa *isaptr;
  typeorgl *orglptr;
  int type;
{
  typeorgl fetchorglgr();
  int temp;

	if (/*backenddaemon &&*/(temp = checkforopen(isaptr, type, user)) <= 0) {
#ifndef DISTRIBUTION
		if (!isxumain) {
			fprintf(stderr,"orgl for ");
			dumptumbler(isaptr);
			fprintf(stderr," not open in findorgl temp = %d\n",temp);
			return FALSE;
			/*gerror("Temporary crash in findorgl\n");*/
			/* ECH ?? or should I simply return FALSE? */
		}
#else
		if (!isxumain) {
			*orglptr = NULL;
			return FALSE;
		}
#endif
	}
	*orglptr = fetchorglgr(taskptr, granfptr, isaptr);
	return (*orglptr ? TRUE : FALSE);
}

  bool
inserttextingranf (taskptr, granfptr, hintptr, textset, ispansetptr)
  typetask *taskptr;
  typegranf granfptr;
  typehint *hintptr;
  typetextset textset;
  typeispanset *ispansetptr;
{  bool inserttextgr();
	 return (inserttextgr (taskptr, granfptr, hintptr, textset, ispansetptr));
}

  bool
createorglingranf (taskptr, granfptr, hintptr, isaptr)
  typetask *taskptr;
  typegranf granfptr;
  typehint *hintptr;
  typeisa *isaptr;
{
  bool createorglgr();

	 return (createorglgr(taskptr, /*GRR not defd (typecuc*)*/granfptr, hintptr, isaptr));
}

  bool
ispanset2vstuffset (taskptr, granfptr, ispanset, vstuffsetptr)
  typetask *taskptr;
  typegranf granfptr;
  typeispanset ispanset;
  typevstuffset *vstuffsetptr;
{
typevstuffset *save;
  typevstuffset *ispan2vstuffset();
save = vstuffsetptr;
#ifndef DISTRIBUTION
if (debug) { fprintf(stderr,"\n\nISPANSET2VSTUFFSET\n"); fooitemset("",ispanset); }
#endif
	*vstuffsetptr = NULL;
	for (; ispanset; ispanset = ispanset->next) {
		vstuffsetptr = ispan2vstuffset (taskptr, granfptr, ispanset, vstuffsetptr);
	}
#ifndef DISTRIBUTION
if (debug) fooitemset("",*save);
#endif
	return (TRUE);
}
