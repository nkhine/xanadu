/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:       nextl.d
***************************************************/

#include "fest.h"
#include "feminreq.h"

  long
nextl (displacementwithinwindowview)
  long displacementwithinwindowview;
{       
  long temp;
  long findnlinviewafterhere();

        temp = findnlinviewafterhere(displacementwithinwindowview)+1-displacementwithinwindowview;

if(debug>2)fprintf(stderr,"nextl dispInWindow=%d temp=%d\n",displacementwithinwindowview,temp);

        if (temp >= currentwindow->windowwidth) {	/* next physical line produced by wrap */
		return(displacementwithinwindowview+currentwindow->windowwidth);
        } else {					/* next \n character*/
                return(displacementwithinwindowview + temp);
        }
}

  long
findnlinviewafterhere(displacementwithinwindowview)
  long displacementwithinwindowview;
{ 
  long temp;
  long nextnlfind();
        temp = nextnlfind(currentwindow->windowview.viewspecptr,displacementwithinwindowview);
        return(temp);
}

  long
nextnlfind (specptr,displace)
  spectype *specptr;     
  long displace;
{
  documenttype *docforspec;
  documenttype *finddocumentinvmlist();
  charspantype *charspanptr;
  spantype *spanptr;
  long spanvsa, targetvsa,difference,numchars;
  register long i;

if(debug>2)fprintf(stderr,"nextnlfind specptr=%x displace=%d : ",specptr,displace);

        if(displace <1){
                displace = 0;
        }
        if (!(docforspec = finddocumentinvmlist (&specptr->docid))) {
                grabspecwithouttext (specptr);
		if (!(docforspec = finddocumentinvmlist (&specptr->docid))) {
if(debug)fprintf(stderr,"returning 0\n");
			return (0);
		}
        }
        spanptr = specptr->specspanptr;
        spanvsa = displace;    

        for (charspanptr = docforspec->doccharspanset; charspanptr; charspanptr = charspanptr->nextcharspan) {
                numchars = charspanptr->numberofcharactersinspan;
                targetvsa = charspanptr->vstartaddress.smallmantissa[1];
                if (targetvsa <= spanvsa && targetvsa+numchars >/*>=*/ spanvsa) {
                        difference = spanvsa - targetvsa;  
                        for (i = difference/*+1*/;i</*=*/numchars;i++){
                                if(charspanptr->charinspanptr[i] == '\n'){
if(debug) fprintf(stderr,"targetvsa=%d i=%d returning %d\n",targetvsa,i,targetvsa+i);
                                        return(targetvsa+i);
                                }
                        }
                        return(nextnlfind(specptr,(long)(targetvsa+numchars))); 
                }
        }
if(debug) fprintf(stderr,"targetvsa=%d nchars=%d returning %d\n",targetvsa,numchars,targetvsa+numchars);

        return (targetvsa + numchars);
}
