/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:        init.d
***************************************************/

#include <signal.h>

#include "fest.h"
#include "feminreq.h"
 
  med findlastplace();

/*tumbler staticdocvposition = {0,-2,  1,0,1,1,0,0,0,0};*/ /* 0.0.1.0.1.1 */
tumbler staticdocvposition = {0,0,  1,1,0,1,0,1,0,1,1,0,0}; /* 1,1,0,1,0.1.0.1.1 */

/*tumbler foostaticlinkhomeposition = {0,-2,  2,0,0,0,0,0,0,0}; *//* 0.0.2.0 */
tumbler foostaticlinkhomeposition = {0,0,  1,1,0,1,0,2,0,0,0,0,0,0}; /* 0.0.2.0 */

  void
init (docvpositionptr)
  tumbler *docvpositionptr;
{
  long l;
  documenttype *initdoc, *documentalloc();
  spectype *specalloc();
  tasktype task;



        /*system("mkdir feprof");*/
        chdir("feprof");
        /*staticlinkhomeposition = foostaticlinkhomeposition;*/

        movetumbler(&staticdocvposition,docvpositionptr);


        memclear ((char *)&cutsequence, sizeof(cutseqtype));
        /*cleartempendsets();*/

}
 
  void
copyvpositionintospec (docpositionptr, specptr)
                          /* breaks a v address into span format */
  tumbler *docpositionptr;
  spectype *specptr;

{
  med i;
  spantype *spanalloc();

        i = findbottomfield (docpositionptr);
        movetumbler (docpositionptr, &specptr->docid);
        specptr->docid.mantissa[i] = 0;
        specptr->docid.mantissa[i+1] = 0;
        appendvmtypethingtolist ( (vmthingtype*)specptr, (vmthingtype*) spanalloc ());
        specptr->specspanptr->vstartaddress.exp = 0;
        specptr->specspanptr->vstartaddress.sign = 0;
        specptr->specspanptr->vstartaddress.smallmantissa[0] = docpositionptr->mantissa[i];
        specptr->specspanptr->vstartaddress.smallmantissa[1] = docpositionptr->mantissa[i+1];
}


  void
copyspecintovposition (docpositionptr, specptr)
                          /* span format breaks a v address into */
  tumbler *docpositionptr;
  spectype *specptr;

{
  med i;

        movetumbler ( &specptr->docid,docpositionptr);
        i = findlastplace (docpositionptr);
        docpositionptr->mantissa[i+2] = 1/*specptr->specspanptr->vstartaddress.smallmantissa[0]*/;
        docpositionptr->mantissa[i+3] = 1/*specptr->specspanptr->vstartaddress.smallmantissa[1]*/;
/*dumptumbler (docpositionptr); */
}


  med
findbottomfield (tumblerptr)
  tumbler *tumblerptr;
{
  med i;
/* this looks questionable*/
        i = findlastplace (tumblerptr);
        for (; tumblerptr->mantissa[i] != 0 && i >= 0; i--) {
        }
        i++;
        return (i);
}

  void
quit()
{
        closeupshop();
        exit(0);
}
      
  void
closeupshop()
{
#ifdef AMIGA
	AmiCleanup();
#endif
					/* CURSES STUFF */
	/*noraw();*/
	endwin();

        tellbackendtoquit();
        fprintf(stderr,"Waiting 2 for Backend to terminate\n\r");
}

  void
curlupanddye(){
             closeupshop();
             fprintf(stderr,"Waiting 1 for Backend to terminate\n");
             wait (0);
}



