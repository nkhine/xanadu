/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:        dump.d
***************************************************/

#include "fest.h"

  med findlastplace();
med displine = 58;

  void
dumpsmalltumbler(tumblerptr)
  smalltumbler *tumblerptr;
{
 med i;

        if (tumblerptr->sign)
                gerror ("There shouldn't be any negative smalltumblers.\n");
/*                fprintf(stderr,"-");
*/        for(i = tumblerptr->exp; i; ++i)
                fprintf(stderr,"0.");
        for(i = 0; i < SMALLNPLACES; ++i){
                fprintf(stderr,"%lu", tumblerptr->smallmantissa[i]);
                if (i < SMALLNPLACES-1)
                        fprintf(stderr,".");
        }
        fprintf(stderr," ");
}

  void
dumpcutseq(cuts)
  cutseqtype *cuts;
{                      
  med i;
        for(i=0;i<cuts->numberofcuts;i++){
                dumpsmalltumbler(&cuts->cutsarray[i]);
        }
}

  void
dumptumbler(tumblerptr)
  tumbler *tumblerptr;
{
  long i;
#ifdef MSDOS
fprintf(stderr,"in dumptumbler tumblerptr = %p \n",tumblerptr);
#else
/* fprintf(stderr,"in dumptumbler tumblerptr = %x \n",tumblerptr);*/
#endif
        if (!tumblerptr){
                fprintf(stderr,"null tumblerptr\n");
                abort ();
        }
/*
        for(i = 0;i<sizeof(tumbler);i++){
                fprintf(stderr,"%x ",*(((char *)(tumblerptr))+i));
        }
        fprintf(stderr,"\n");
*/
        if (tumblerptr->sign) {
                fprintf(stderr, "There shouldn't be any negative tumblers.\n");                return;

        }
/*                fprintf(stderr,"-");
*/        for (i = tumblerptr->exp; i;++i)
                fprintf(stderr,"0.");
        for(i = 0;i <=NPLACES-1/* findlastplace (tumblerptr)*/ ; ++i){
                fprintf(stderr,"%lu", tumblerptr->mantissa[i]);
                if (i < NPLACES-1/*findlastplace (tumblerptr)*/)
                        fprintf(stderr,".");
        }
        fprintf(stderr," ");
}

  void
dumpvmthing (thingptr)
  vmthingtype *thingptr;
{
        fprintf(stderr,"thing %x ",thingptr);
        if (!thingptr) {
                fprintf(stderr," --- WRONG\n");
                abort();
        }
        switch (thingptr->vmdocument.typeofthing) {
          case CHARSPANTYPE :
                fprintf(stderr,"CHARSPANTYPE\n");
                dumpcharspan/*set*/ ((charspantype *)thingptr);
                break;
          case DOCUMENTTYPE :
                fprintf(stderr,"DOCUMENTTYPE\n");
                dumpdocument ((documenttype *)thingptr);
                break;
          case SPANTYPE :
                fprintf(stderr,"SPANTYPE\n");
                dumpspanset ((spantype *)thingptr);
                break;
          case SPECTYPE :
                fprintf(stderr,"SPECTYPE\n");
                dumpspecset ((spectype *)thingptr);
                break;
          case DOCRANGETYPE :
                fprintf(stderr,"DOCRANGETYPE\n");
                break;
          case LINKRESTRICTIONTYPE :
                fprintf(stderr,"LINKRESTRICTIONTYPE\n");
                break;
          case LINKGROUPTYPE :
                fprintf(stderr,"LINKGROUPTYPE\n");
                break;
          case LINKCONTENTTYPE :
                fprintf(stderr,"LINKCONTENTTYPE\n");
                break;
          case TUMBLERITEMTYPE:
                fprintf(stderr,"TUMBLERITEMTYPE\n");
                dumptumbleritem((tumbleritemtype *)thingptr);
                break;
          case CORRESPONDENCETYPE:
                fprintf(stderr,"CORRESPONDENCETYPE");
                dumpcorrespondence((correspondencetype *)thingptr);
                break;
          default :
                gerror ("UNKNOWN thingtype\n");
        }
}

  void
dumpcorrespondenceset(correspondenceset)
  correspondencetype *correspondenceset;
{
        if(correspondenceset){
                fprintf(stderr,"correspondence set:\n");
        }else{
                fprintf(stderr,"null correspondence set:\n");
        }       
        for(;correspondenceset;correspondenceset = correspondenceset->nextcorrespondence){
                dumpcorrespondence(correspondenceset);
        }
}

  void
dumpcorrespondence(correspondenceptr)
  correspondencetype * correspondenceptr;
{                  
        fprintf(stderr,"stream[0] = ");

        dumptumbler(&correspondenceptr->stream[0] );
        fprintf(stderr,"\nstream [1] = ");
        dumptumbler(&correspondenceptr->stream[1]);
        fprintf(stderr,"\nwidthofspan = ");
        dumptumbler(&correspondenceptr->widthofspan);
        fprintf(stderr,"\n");
}
 
  void 
dumptumbleritemset(tumbleritemptr)
  tumbleritemtype * tumbleritemptr;
{                                               
        if(tumbleritemptr){
                fprintf(stderr,"tumbleritemset : \n");
        }else{
                fprintf(stderr,"null tumbleritemset");
        }
        for (;tumbleritemptr;tumbleritemptr = tumbleritemptr->nexttumbleritem)
                dumptumbleritem(tumbleritemptr);
}

  void
dumptumbleritem(tumbleritemptr)
  tumbleritemtype * tumbleritemptr;
{
        fprintf(stderr," tumbleritem:\n");
        dumptumbler(&tumbleritemptr->value);
}

  void 
dumpspecset (specptr)
  spectype *specptr;
{
        if (specptr)
                fprintf(stderr," specset :\n");
        else {
                fprintf(stderr," empty specset\n");
                return;
        }
        for (;specptr; specptr = specptr->nextspec)
                dumpspec (specptr);
}

  void
dumpspec (specptr)
  spectype *specptr;
{
        fprintf(stderr,"  spec %x<%x>%x    docid = ",specptr->prevspec,specptr,specptr->nextspec);
        dumptumbler (&specptr->docid);
        fprintf(stderr,"\n");
        dumpspanset (specptr->specspanptr);
}

  void
dumpspanset (spanptr)
  spantype *spanptr;
{
        if (spanptr)
                fprintf(stderr,"   spanset :\n");
        else {
                fprintf(stderr,"   empty spanset\n");
                return;
        }
        for (;spanptr; spanptr = spanptr->nextspan)
                dumpspan (spanptr);
}

  void
dumpspan (spanptr)
  spantype *spanptr;
{
        fprintf(stderr,"    span %x<%x>%x\n     vsa = ",spanptr->prevspan, spanptr, spanptr->nextspan);
        dumpsmalltumbler (&spanptr->vstartaddress);
        fprintf(stderr,"  span length = %d\n",spanptr->sizeofspan);
}

  void
dumpdocument (documentptr)
  documenttype *documentptr;
{
        fprintf(stderr," document %x<%x>%x\n  document id = ",documentptr->prevdocument,documentptr,documentptr->nextdocument);
        dumptumbler (&documentptr->documentid);
        fprintf(stderr,"\n");
        dumpcharspanset (documentptr->doccharspanset);
}

  void
dumpcharspanset (charspanptr)
  charspantype *charspanptr;
{
        if (charspanptr)
                fprintf(stderr,"   charspanset :\n");
        else {
                fprintf(stderr,"   empty charspanset\n");
                return;
        }
        for (; charspanptr; charspanptr = charspanptr->nextcharspan)
                dumpcharspan (charspanptr);
}

  void
dumpcharspan (charspanptr)
  charspantype *charspanptr;
{
  long i;
  char *c;

        fprintf(stderr,"    charspan %x<%x>%x\n     vsa = ",charspanptr->prevcharspan,charspanptr,charspanptr->nextcharspan);
        dumpsmalltumbler (&charspanptr->vstartaddress);
        fprintf(stderr,"  span length = %d\n",charspanptr->numberofcharactersinspan);
        fprintf(stderr,"\n     text = \"");
        for (i = 0, c= charspanptr->charinspanptr; i < charspanptr->numberofcharactersinspan; i++)
                fprintf(stderr,"%c",*c++);
        fprintf(stderr,"\"\n\n");
}

  void
dumpvm()
{
  documenttype *ptr;
  extern vmthingtype *vmbaseptr;
        for (ptr = (documenttype *)vmbaseptr; ptr; ptr = ptr->nextdocument)
                dumpdocument (ptr);
}

dumpvmlist()
{
  vmthingtype *ptr;
  extern vmthingtype *vmbaseptr;
  bool first = TRUE;
        for (ptr=(vmthingtype *)vmbaseptr; ptr; ptr=(vmthingtype *)ptr->vmcharspan.nextvm) {
		if (!first && (ptr == (vmthingtype *)vmbaseptr)) {
			break;
		}
                dumpvmthing (ptr);
		first = FALSE;
	}
}
