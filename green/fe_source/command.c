/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:     command.d
***************************************************/

#include "fest.h"
#include "feminreq.h"
#include "fekeys.h"

#define ESC 0x1b

  bool
iscommand(c)
  metachar c;
{
        switch (c) { 
          case  COMPARECHAR:
                return(TRUE);
          case  WEIRDOPTIONS:
                return(TRUE);
          case  WINSWT:
                return(TRUE);
          case  DELCUTS:
                return(TRUE);
          case  CUT:
                return(TRUE);
          case  DELBLK:
                return(TRUE);
          case  COPY:
                return(TRUE);
          case  RRANGE:
                return(TRUE);
          case  DEL:
                return(TRUE);
          case  FLINK:
                return(TRUE);
          case  CHFR:
                return(TRUE);
          case  CHTO:
                return(TRUE);
          case ESC2:
          case ESC:
                return(TRUE);
          case NEWDOC:
                return(TRUE);
          case MLINK:
                return(TRUE);
          case CREATEFOOTNOTECHAR:
                return(TRUE);
          case CREATEVERSIONCHAR:
                return(TRUE);
          case REDRAW:
		return(TRUE);
          default:
                return(FALSE);
        }
        return(FALSE);
}
  void
setupcommandstruct (requestptr)
  requesttype *requestptr;
{
  metachar c;
  metachar gettypeahead();

        c = gettypeahead();

        switch (c) {
          case  COMPARECHAR: 
                requestptr -> requestnumber = COMPARE;
                return /*(TRUE)*/ ;
          case  WEIRDOPTIONS:
                requestptr -> requestnumber = WEIRDOPTIONSCMD;
                return;
          case  WINSWT:
                requestptr -> requestnumber = CHANGEDOCUMENT;/*change windows*/
                return;
          case  DELCUTS: /* CLENDSETS */
                requestptr -> requestnumber =  CLEARENDSETS;
                return;
          case  DELBLK:
                requestptr -> requestnumber =  /*DELETEBLOCK*/DELETECUTS;
                return;
          case  COPY:
                requestptr -> requestnumber =  /*COPYFUNCTION*/  MAKECUT;
                return;
          case  CUT:
                requestptr -> requestnumber =  MAKECUT;
                return;
          case  RRANGE:
                requestptr -> requestnumber =  XREARRANGE;
                return;
          case  DEL:
                requestptr -> requestnumber =  DELETECHARACTER;
                return;
          case  FLINK:
                requestptr -> requestnumber =  XFOLLOWLINK;
                return;
          case CHFR:
                requestptr -> requestnumber = CHANGEFROMSET;
                return;
          case CHTO:
                requestptr -> requestnumber = CHANGETOSET;
                return;
          case MLINK /*CUT*/:
                requestptr -> requestnumber =  MAKELINK2;
                return;
          case  ESC2:
          case  ESC:
                requestptr -> requestnumber =  RETURNFROMSTACK;
                return;
          case  NEWDOC:
                requestptr -> requestnumber =  CREATEDOCUMENT;
                return;
          case  CREATEFOOTNOTECHAR:
                requestptr ->requestnumber  =  CREATEFOOTNOTE;
                return;
/*        case  SHOWLINKSCHAR:
                requestptr -> requestnumber =  SHOWLINKS;
                return;
                break;
*/          case  CREATEVERSIONCHAR:
                requestptr -> requestnumber =  CREATEVERSION;
                return;
          case  EXITNORMALLYCHAR:
                requestptr -> requestnumber =  EXITNORMALLY;
                return;
          case  BECOMECRUFTYCHAR:
                requestptr -> requestnumber =  BECOMECRUFTY;
                return;
          case  REDRAW:
                requestptr -> requestnumber =  REDRAWCMD;
		return;
          default:
                return;
                break;
        }
}
