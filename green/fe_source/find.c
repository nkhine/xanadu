/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:        find.d
***************************************************/

#include "fest.h"
#include "feminreq.h"
   
  long
lastl (displacementwithinwindowview)
  long displacementwithinwindowview;
{ 
 long temp, findnlinviewbeforehere(),answer,nextl();
 bool first;
fflush(stdout);
        temp =/* 1;*/findnlinviewbeforehere(displacementwithinwindowview);
        answer = temp;
	first = TRUE;
	for(;(first ||temp)&&temp<displacementwithinwindowview;temp = nextl(temp)){
/*printf("temp = %d ",temp);*/
		answer = temp;
		first = FALSE;
	}

	if(answer == 0){
		answer = 1;
	}
        fflush(stdout);
	return(answer);
}

  bool
attop(k)
  long k;
{
  long lastl();
	if(k <= 1){
		return(TRUE);
	}
	if(k>currentwindow->windowwidth){
		return(FALSE);
	}
	/*fprintf(stderr,"K = %d ",k);*/
        if(/**/TRUE&&/**/lastl(k) >1){
                return(FALSE);
        }else{
                return(TRUE);
        }    
}

  long
findnlinviewbeforehere(displacementwithinwindowview)  
  long displacementwithinwindowview;    
{
  long prevnlfind();
        return(prevnlfind(currentwindow->windowview.viewspecptr,displacementwithinwindowview));
}

  long
prevnlfind (specptr,displace)
  spectype *specptr;     
  long displace;
{
  documenttype *docforspec;
  documenttype *finddocumentinvmlist();
  charspantype *charspanptr;
  spantype *spanptr;
  long spanvsa, charspanvsa,i,difference;
  long findnlintext();

        if (!(docforspec = finddocumentinvmlist (&specptr->docid))) {
                return (NULL);
        }
        if (displace <=0) {
/*fprintf(stderr,"foo2 %d",displace);*/
                return (0);
        }
        spanptr = specptr->specspanptr;
        spanvsa = displace-1;
        for (charspanptr = docforspec->doccharspanset; charspanptr; charspanptr = charspanptr->nextcharspan) {
                charspanvsa = charspanptr->vstartaddress.smallmantissa[1];
                if (charspanvsa <= spanvsa && charspanvsa+charspanptr->numberofcharactersinspan > spanvsa) {                    
                        difference = spanvsa - charspanvsa-1;   
                        i=findnlintext(charspanptr->charinspanptr,difference);  
                        if(i == -1){ 
                           return(prevnlfind(specptr,charspanvsa));
                        }else{
                                return(i+charspanvsa);
                        }
                }else if (spanvsa < charspanvsa){
                        return (NULL); /* missed it */
		}
        }
        return (NULL);
}
  long
findnlintext(text,nchars)
  char text[];
  long nchars;
{
  long i;
        for (i= nchars-1;i>=0;i--){
                if (text[i] == '\n'){
                        return(i);
                }
        }
        return(-1);
}

#ifdef COMMENT
  spantype *
findlastspaninspec (specptr)
  spectype *specptr;
{
  spantype *spanptr;

        if (specptr->specspanptr) {
                for (spanptr = specptr->specspanptr; spanptr->nextspan; spanptr = spanptr->nextspan) ;
                return (spanptr);
        } else {
                return (/*NULL*/ (spantype*)specptr);            /*  Wh???? */
        }
}
#endif COMMENT
