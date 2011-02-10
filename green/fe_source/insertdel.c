/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:   insertdel.d
***************************************************/

/* insert/delete chars -- contains routines that handle input of text*/

#include "fest.h"
#include "feminreq.h"
#include "fekeys.h"

  bool
isinsertdeletechar(c)
  metachar c;
{
        return ((c >= ' '  &&  c <= 0x7f) || c == CR || c == NL || c == DEL);
}
  bool
glyph()
{
if(iswindow){
	fprintf(stderr,"\ncalling besureistypeahead in interactivelyaccumulatestring");
}
return(TRUE);

}

  void
interactivelyaccumulatestring (requestptr)
  requesttype *requestptr;
{
  char localstring[NTYPEAHEAD];
  long  localnumberofcharacters,numberofblanks,totalnumberofcharacters;
  bool isinsertdeletechar();
  metachar peektypeahead();
  spectype *specptr, *specalloc();
  tasktype task;
        zerooutcursorcontrol(requestptr);
        localnumberofcharacters= 0;
        requestptr->initialcursorpositionspecptr = specalloc();
        numberofblanks = findspecfromcursorposition (requestptr->initialcursorpositionspecptr);
        if (numberofblanks > 0) {
        /*     numberofblanks = 0*/   ;/*zzz*/
        }                  
        totalnumberofcharacters = numberofblanks;
        for(;glyph()&&besureistypeahead()&& isinsertdeletechar(peektypeahead()) && totalnumberofcharacters <NTYPEAHEAD;){
	    accumulatestring (requestptr,localstring,&localnumberofcharacters,numberofblanks);
/*
fprintf(stderr,"LOCAL # OF CHARS = %d\n",localnumberofcharacters);
*/
	    totalnumberofcharacters += localnumberofcharacters;
	    if (currentwindow->newlycreateddocumentp) {
#ifdef ZZZ
			if (localnumberofcharacters <= 0) {
				/*don't delete in empty document;*/	
				return;
			} else {
#endif
			    specptr = specalloc ();
			    findspecfromcursorposition (specptr);
#ifdef ZZZ
			}
#endif
	    } else { if (localnumberofcharacters < 0) {
			specptr = specalloc();
			findspecfromcursorpositionoffsetandlength(specptr,localnumberofcharacters/*-1*/,-localnumberofcharacters);
			if (requestptr->requestnumberofcharacters<0){
				displayinsertanddeletechar(specptr,localstring,&localnumberofcharacters,numberofblanks/*(long)0*/);
                                specfree(specptr);
				for(;besureistypeahead()&&peektypeahead()== DEL;){
					numberofblanks = 0;
					localnumberofcharacters = 0;
					accumulatestring (requestptr,localstring,&localnumberofcharacters,numberofblanks);
					totalnumberofcharacters += localnumberofcharacters;
					specptr = specalloc();
					findspecfromcursorpositionoffsetandlength(specptr,localnumberofcharacters/*-1*/,-localnumberofcharacters);
					displayinsertanddeletechar(specptr,localstring,&localnumberofcharacters,(long)0);
				        specfree(specptr);

				}
				break;
				/*
				return; */
			}
		    } elif (localnumberofcharacters > 0) {
			    specptr = specalloc ();
			    findspecfromcursorposition (specptr);
		    }    
		}                                    
            displayinsertanddeletechar(specptr,localstring,&localnumberofcharacters,numberofblanks);
            numberofblanks = 0;

            specfree(specptr);
            localnumberofcharacters = 0;
        } 
/**/
        if(currentwindow->ispartofparalleltextface){
                compareviewspecs(&task,currentwindow->windowview.viewspecptr,currentwindow->correspondingwindow->windowview.viewspecptr );
        }
/**/
}

  void
accumulatestring (requestptr,localstring,localnumberofcharactersptr,numberofblanks)
  requesttype *requestptr;
  char localstring[];
  long *localnumberofcharactersptr,numberofblanks;
{
  metachar c,gettypeahead();

        for (;istypeahead() && isinsertdeletechar (peektypeahead())
             && requestptr->requestnumberofcharacters < NTYPEAHEAD;) {
                c = gettypeahead();
                if (numberofblanks > 0 && c != CR && c!= NL){
                        for(;numberofblanks>0;numberofblanks--){
                           requestptr->requeststring
                           [requestptr->requestnumberofcharacters++] = ' ';
                           localstring[(*localnumberofcharactersptr)++] = ' ';
                        }
                }
                if (c == DEL) {
                        requestptr->requestnumberofcharacters--;
                        (*localnumberofcharactersptr)--;
                        continue;
                }
		if(c == CR){
			c = NL;
		}
                if (*localnumberofcharactersptr >= 0) {
                        requestptr->requeststring
                         [requestptr->requestnumberofcharacters++] = c;
                        localstring[(*localnumberofcharactersptr)++] = c;
                }
                else{
                        return;
                }
        }
/*localstring[(*localnumberofcharactersptr)] = '\0';*/
/* above WILL FAIL when c==DEL -- localnumberofcharactersptr set to -1*/
}
