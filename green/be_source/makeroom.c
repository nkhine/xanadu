/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/

#include "crum.h"
#include "ndenf.h"

makeroomonleftnd (father, offset, origin, grasp)
  typecuc *father;
  typedsp *offset, *grasp;
  typewid *origin;
{
  typedsp newdsp;
  tumbler base;
  INT i;
  INT savedebug;
#ifndef DISTRIBUTION         
savedebug = debug;
/*if(!is1story(offset)){ debug = TRUE; } */
foo("entering makeroomonleftnd\n");
#endif
        prologuend ((typecorecrum*)father, offset, grasp, (typedsp*)NULL);
        for (i = 0; i < widsize(father->cenftype); ++i) {
                if (tumblercmp (&origin->dsas[i], &grasp->dsas[i]) == LESS) {
#ifndef DISTRIBUTION
footumbler(" origin = ",&origin->dsas[i]);
footumbler(" grasp = ",&grasp->dsas[i]);
#endif
                    tumblersub (&grasp->dsas[i], &origin->dsas[i], &base);
#ifndef DISTRIBUTION
footumbler(" base = ",&base);
footumbler(" origin = ",&origin->dsas[i]);
footumbler(" offset = ",&offset->dsas[i]);
#endif
                    tumblersub(&origin->dsas[i],&offset->dsas[i],&newdsp.dsas[i]);
#ifndef DISTRIBUTION
footumbler(" newdsp = ",&newdsp.dsas[i]);
#endif
                    expandcrumleftward ((typecorecrum*)father, &newdsp.dsas[i], &base, i);
                    prologuend ((typecorecrum*)father, offset, grasp, (typedsp*)NULL);
                }
        }
#ifndef DISTRIBUTION
foo("leaving makeroomonleftnd\n");
debug = savedebug;
#endif
}


expandcrumleftward (crumptr, newdsp, base, index)
  typecorecrum *crumptr;
  tumbler *newdsp, *base;
  INT index;
{
  typecorecrum *ptr,  *findleftson();
#ifndef DISTRIBUTION
foo("entering expandcrum leftward \n");
#endif
        movetumbler (newdsp, &crumptr->cdsp.dsas[index]);
        tumbleradd(base,&crumptr->cwid.dsas[index],&crumptr->cwid.dsas[index]);
#ifndef DISTRIBUTION
footumbler("in expandcrumleftward cwid = ",&crumptr->cwid.dsas[index]);
#endif
        ptr = findleftson((typecuc*)crumptr);
        for (; ptr; ptr = (typecorecrum *)findrightbro(ptr)) {
                tumbleradd(base,&ptr->cdsp.dsas[index],&ptr->cdsp.dsas[index]);
		ivemodified(ptr);
#ifndef DISTRIBUTION
footumbler("in expandcrumleftward cdsp = ",&crumptr->cdsp.dsas[index]);
#endif
        }
#ifndef DISTRIBUTION
foo("leaveing expandcrum leftward \n");
#endif
}
