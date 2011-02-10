/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:   getusrreq.d
***************************************************/

#include "fest.h"
#include "feminreq.h"
#include "fekeys.h"

  void
getuserrequest(requestptr)
  requesttype *requestptr;
{
  bool iscursorcontrol(), isinsertdeletechar(), iscommand();
  metachar peektypeahead();
  metachar gettypeahead();
  metachar waste;

        besureistypeahead(); /* in suntool case should never get here unless */
			     /* there is stuff */

        if (peektypeahead() == EXITNORMALLYCHAR) {
                requestptr->requestnumber = EXITNORMALLY;
                waste = gettypeahead();
                return;
        }
        if (peektypeahead() == BECOMECRUFTYCHAR) {
                requestptr->requestnumber = BECOMECRUFTY;
                waste = gettypeahead();
/*
putf("%CBECOMECRUFTY\n",0,0);
*/
                return;
        }
        if (iscursorcontrol (peektypeahead())) {
                requestptr -> requestnumber = CURSORPOSITION;
                accumulatecursorcontrol (requestptr);
                return;
        }
        if (isinsertdeletechar (peektypeahead())) {
                requestptr->requestnumber = INSERTANDDELETECHAR;
                interactivelyaccumulatestring (requestptr);
                return;
        }
        if (iscommand (peektypeahead())) {
                setupcommandstruct (requestptr);
                return;
        }
	if(peektypeahead() == EOF){
		fprintf(stderr," EOF in getusrreq through peektypeahead");
		putf("C% EOF in getusrreq through peektypeahead",0,0);
	}
        baderror("what is this request\n");
        waste = gettypeahead(); /* throw away offending error */
}
