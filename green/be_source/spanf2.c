/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/

/* spanf2.d - lower-level spanfilade calls */

#include "xanadu.h"
#include "enf.h"

  bool
isinlinklist (linkset, linkisaptr)
  typelinkset linkset;
  typeisa *linkisaptr;
{
        for (; linkset; linkset = linkset->next) {
                if (tumblereq (&linkset->address, linkisaptr))
                        return (TRUE);
        }
        return (FALSE);
}

  bool
onlinklist (taskptr, linksetptr, linkisaptr)
  typetask *taskptr;
  typelinkset *linksetptr;
  typeisa *linkisaptr;
{
  typelink *linkset, *temp, *nextlink;
  typelink *makelinkitem();

        linkset = makelinkitem (taskptr, linkisaptr);

        if (*linksetptr == NULL) {
                *linksetptr = linkset;
                return;
        }

        for (temp = *linksetptr; nextlink = temp->next; temp = nextlink) {
                if (tumblereq (&temp->address, linkisaptr))
                        return;
        }

        temp->next = linkset;
}

intersectlinksets (taskptr, linkset1, linkset2, linkset3, linkset4ptr)
  typetask *taskptr;
  typelinkset linkset1;
  typelinkset linkset2;
  typelinkset linkset3;
  typelinkset *linkset4ptr;
{
  typelinkset linkset4;
  typelinkset temp1;
  typelinkset temp2;
  typelinkset temp3;
  typelink *makelinkitem();
bool olddebug = debug;
#ifndef DISTRIBUTION
if (debug) {
fprintf(stderr,"\nINTERSECTLINKSETS\n");
fooitemset ("", linkset1);
fooitemset ("", linkset2);
fprintf(stderr,"inter linkset1 = %x, linkset2 = %x, linkset3 = %x\n",linkset1, linkset2,linkset3);
}
#endif

	/* If only one linkset is non-null, then just use it */
        if (linkset1 && !linkset2 && !linkset3)
                *linkset4ptr = linkset1;
        else if (!linkset1 && linkset2 && !linkset3)
                *linkset4ptr = linkset2;
        else if (!linkset1 && !linkset2 && linkset3)
                *linkset4ptr = linkset3;
        else
                *linkset4ptr = NULL;

        if (*linkset4ptr) {
#ifndef DISTRIBUTION
if(debug){ fprintf(stderr,"*linkset4ptr = %x\n",*linkset4ptr); }
#endif
debug = olddebug;
                return;
        }

	/* At least two linksets aren't null */
	/* If only two are, put them in temp1 and linkset2 */
        if (linkset1)
                temp1 = linkset1;
        if (linkset1 && !linkset2 && linkset3) {
                linkset2 = linkset3;
                linkset3 = NULL;
        } else if (!linkset1 && linkset2 && linkset3) {
                temp1 = linkset3;
                linkset3 = NULL;
        }
#ifndef DISTRIBUTION
if (debug) { fprintf(stderr, "temp1 = %x, linkset2 = %x, linkset3 = %x\n",temp1,linkset2,linkset3);}
#endif
	if (!linkset3) {
        	for (; temp1; temp1 = temp1->next) {
                	for (temp2 = linkset2; temp2; temp2 = temp2->next) {
                        	if (tumblereq (&temp1->address, &temp2->address)) {
                                	linkset4 = makelinkitem (taskptr, &temp1->address);
	                                *linkset4ptr = linkset4;
        	                        linkset4ptr = &linkset4->next;
				}
	                }
        	}
	} else {
	        for (; temp1; temp1 = temp1->next) {
        	        for (temp2 = linkset2; temp2; temp2 = temp2->next) {
                	        for (temp3 = linkset3; temp3; temp3 = temp3->next) {
                        	        if (tumblereq (&temp1->address, &temp2->address)
					    && tumblereq (&temp2->address, &temp3->address)) {
                                	        linkset4 = makelinkitem (taskptr, &temp1->address);
                                        	*linkset4ptr = linkset4;
                                        	linkset4ptr = &linkset4->next;
					}
	                        }
        	        }
	        }
	}
debug = olddebug;
}

  typelink *
makelinkitem (taskptr, linkisa)
  typetask *taskptr;
  typeisa *linkisa;
{
  INT *taskalloc();
  typelink *link;
#ifndef DISTRIBUTION
footumbler("makelinkitem", linkisa);
#endif
        link = (typelink *) taskalloc (taskptr, sizeof (typelink));
        movetumbler (linkisa, &link->address);
        link->itemid = LINKID;
        link->next = NULL;
        return (link);
}
