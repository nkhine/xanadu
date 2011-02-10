/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:      vmedit.d
***************************************************/

#include "fest.h"
#include "feminreq.h"

  void
fixvm (specptr,string,nchars)
  spectype *specptr;
  char *string;
  long nchars;

{
  charspantype *charspantofix, *findcharspaninvm();
/*
fprintf(stderr,"fixvm\n");
*/
        if (charspantofix = findcharspaninvm(specptr)) {
                if (nchars > 0) {
                        inserttextincharspanindocument (
				charspantofix, specptr->specspanptr, string, nchars);
                } else {
                        deletetextfromdocumentinvm (charspantofix, specptr->specspanptr, nchars);
                }
        }
        if (!charspantofix) {
                if(currentwindow->newlycreateddocumentp){
                        charspantofix = newcharspaninvm(specptr);
                        inserttextincharspanindocument (
				charspantofix, specptr->specspanptr, string, nchars);
                        return;
                } else {
                        gerror("no charspan in fixvm");
                }
        }
       fixvsasforrestofcharspansindocument(charspantofix->nextcharspan,nchars);
}

  void
inserttextincharspanindocument (charspanptr, spanptr, string, nchars)
  charspantype *charspanptr;
  spantype *spanptr;
  char *string;
  long nchars;

{
  char *newtext;
  nat  *ealloc();
  long offset;
        if(!charspanptr)
                gerror("NULL charspanptr in inserttextincharspanindocument");
        offset = spanptr->vstartaddress.smallmantissa[1] - charspanptr->vstartaddress.smallmantissa[1]/*-1*/; 
/*
 fprintf(stderr,"inserttextincharspanindocument offset = %d span mant = %d charspan mant = %d\n" , offset , spanptr->vstartaddress.smallmantissa[1] , charspanptr->vstartaddress.smallmantissa[1]); 
*/
        newtext = (char *)ealloc (charspanptr->numberofcharactersinspan + nchars +1);

        movmem (charspanptr->charinspanptr, newtext, (unsigned)offset);

        movmem (string, newtext+offset, (unsigned)nchars);

        movmem (charspanptr->charinspanptr+offset,
                newtext + offset + nchars ,
                (unsigned)(charspanptr->numberofcharactersinspan-offset+1));

        charspanptr->numberofcharactersinspan += nchars;
        *(newtext+charspanptr->numberofcharactersinspan) = '\0';
        if(charspanptr->charinspanptr){
                ffree (charspanptr->charinspanptr);
        }
        charspanptr->charinspanptr = newtext;
}

  void
deletetextfromdocumentinvm (charspanptr, spanptr, nchars)
  charspantype *charspanptr;
  spantype *spanptr;
  long nchars;
{
  long offset, charstodelete;
  charspantype *next;

        charstodelete = - nchars ;
        offset = spanptr->vstartaddress.smallmantissa[1] - charspanptr->vstartaddress.smallmantissa[1];
        if (offset+charstodelete <= charspanptr->numberofcharactersinspan) {
                if(offset+charstodelete<charspanptr->numberofcharactersinspan){
                        movmem (charspanptr->charinspanptr+offset+charstodelete,
                                charspanptr->charinspanptr+offset,
                                (unsigned)(charspanptr->numberofcharactersinspan-offset-charstodelete));
                }
                charspanptr->numberofcharactersinspan -= charstodelete;
        } else {
                charstodelete -= charspanptr->numberofcharactersinspan-offset;
                charspanptr->numberofcharactersinspan = offset;
/*zzz initialize next */ next = (charspantype *)1; /* kluge*/
                while (charstodelete && next) {
                       next = charspanptr->nextcharspan;
                       if(charstodelete>=charspanptr->numberofcharactersinspan){
                           charstodelete-=charspanptr->numberofcharactersinspan;
                           removevmtypethingfromlist ((vmthingtype*)charspanptr);
                           removefromvmlist ((vmthingtype *)charspanptr);
                           ffree ((char *)charspanptr);
                       } else {
                               movmem (charspanptr->charinspanptr+charstodelete,
                                       charspanptr->charinspanptr,
                                       (unsigned)(charspanptr->numberofcharactersinspan-charstodelete));
                               charstodelete = 0;
                        }
                        charspanptr = next;
                }
        }
}

  void
fixvsasforrestofcharspansindocument (charspanptr, numberofnewcharacters)
  charspantype *charspanptr;
  long numberofnewcharacters;
{
        for (; charspanptr; charspanptr = charspanptr->nextcharspan) {
             charspanptr->vstartaddress.smallmantissa[1]+=numberofnewcharacters;
        }
}

  void
fixendsetvm (specptr, nchars)
  spectype *specptr;
  long nchars;

{
  spectype *localspecptr;
/*
 fprintf(stderr,"fixendsetvm\n");
*/
        localspecptr = specspanalloc();
        movetumbler (&specptr->docid, &localspecptr->docid);
        movesmalltumbler (&specptr->specspanptr->vstartaddress, &localspecptr->specspanptr->vstartaddress);
        localspecptr->specspanptr->sizeofspan = nchars;
        adjustendsetlist (localspecptr, currentwindow->windowview.viewfromset);
        adjustendsetlist (localspecptr, currentwindow->windowview.viewtoset);
        specfree (localspecptr);
}

  void
adjustendsetlist (specptr, specset)
  spectype *specptr, *specset;
{
  spectype *tmpspec;
  spantype *tmpspan;
/*
 fprintf(stderr,"adjustendsetlist\n");
*/
        for (tmpspec = specset; tmpspec; tmpspec = tmpspec->nextspec) {
                if (tumblereq (&tmpspec->docid, &specptr->docid)) {
                        break;
                }
        }
        for (; tmpspec; tmpspec = tmpspec->nextspec) {
/*
 fprintf(stderr,">--------------------\n");
 dumpspecset(tmpspec);
 fprintf(stderr,"<--------------------\n");
*/
            for(tmpspan=tmpspec->specspanptr;tmpspan;tmpspan=tmpspan->nextspan){
                if (specptr->specspanptr->vstartaddress.smallmantissa[1] <= tmpspan->vstartaddress.smallmantissa[1] + tmpspan->sizeofspan) {
                        if (specptr->specspanptr->vstartaddress.smallmantissa[1] /*>=*/ > tmpspan->vstartaddress.smallmantissa[1] && specptr->specspanptr->sizeofspan > 0) {
                               splitspan(tmpspan,( long )(specptr->specspanptr->vstartaddress.smallmantissa[1]),specptr->specspanptr->sizeofspan);
                                tmpspan = tmpspan->nextspan;
                        } else {
                                tmpspan->vstartaddress.smallmantissa[1] += specptr->specspanptr->sizeofspan;
                        }
                }
            }
/*
 fprintf(stderr,"-------------------->\n");
 dumpspecset(tmpspec);
 fprintf(stderr,"--------------------<\n");
*/
        }
}

  void
splitspan (spantobesplit, gapstart, gapwidth)
  spantype *spantobesplit;
  long gapstart, gapwidth;
{
  spantype *newspan;
  long oldsize;

/*
 fprintf(stderr,"splitspan %d %d\n",gapstart,gapwidth);
*/
        copyspanset (spantobesplit, &newspan);
        newspan->nextspan = spantobesplit->nextspan;
        spantobesplit->nextspan = newspan;
        oldsize = spantobesplit->sizeofspan;
        spantobesplit->sizeofspan = gapstart - spantobesplit->vstartaddress.smallmantissa[1];
        newspan->vstartaddress.smallmantissa[1] = gapstart + max(0,gapwidth);
        newspan->sizeofspan =oldsize-spantobesplit->sizeofspan+min(0,gapwidth);
}

  void
fixvmforrearrange(docid) 
  tumbler *docid;
{
  spectype *specptr , *insertspecptr;
  long temp;
  char *textbuffer;
        
        if(TRUE/* case for moving first set of cut stuff*/){
                specptr = specspanalloc();
                movesmalltumbler(docid,&specptr->docid);
                specptr->specspanptr->vstartaddress.smallmantissa[1] = cutsequence.cutsarray[0].smallmantissa[1];
                 temp = specptr->specspanptr-> sizeofspan = cutsequence.cutsarray[1].smallmantissa[1]-cutsequence.cutsarray[0].smallmantissa[1] /*- 1*/;

                insertspecptr = specspanalloc();
                movesmalltumbler(docid,&insertspecptr->docid);
                insertspecptr->specspanptr->vstartaddress.smallmantissa[1] = cutsequence.cutsarray[2].smallmantissa[1]/* zzz why +1*/;
                insertspecptr->specspanptr-> sizeofspan = 1; 

                textbuffer = (char *)ealloc(temp+11);
                grabspec(specptr,textbuffer);
                fixvm(insertspecptr,textbuffer,temp);/*inserts text at 3d cut*/
                fixendsetvm(insertspecptr,temp);
                specfree(insertspecptr);
                fixvm(specptr,(char *)NULL,-temp);      
                fixendsetvm(specptr,-temp);
                specfree(specptr);
                ffree(textbuffer);
        }else{


        }

        /* fix masks */


}     
