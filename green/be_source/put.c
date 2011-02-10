/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* put.d - Udanax output routines - no front end version */
/* as of Jul 12 19:00:00 */

#include "xanadu.h"

#define MINEXP  -10

prompt (taskptr, string)
  typetask *taskptr;
  char *string;
{
        fprintf(taskptr->outp, "%s", string);
}

error (taskptr, string)
  typetask *taskptr;
  char *string;
{
        fprintf(taskptr->errp, "%s", string);
}

puttumbler(outfile, tumblerptr)
  FILE *outfile;
  tumbler *tumblerptr;
{
  INT i, place;

        if (!tumblercheck (tumblerptr) || tumblerptr->exp < MINEXP) {
                dumptumbler (tumblerptr);
                return;
        }
        if (tumblerptr->sign)
                fprintf(outfile, "-");
        for(i = tumblerptr->exp; i < 0; ++i)
                fprintf(outfile, "0.");
        place = NPLACES;
        do {--place;
        } while(place > 0 && tumblerptr->mantissa[place] == 0);
        for(i = 0; i <= place; ++i) {
                putnum(outfile, tumblerptr->mantissa[i]);
                if (i < place)
                        putc('.', outfile);
        }
}

putnum(outfile, num)
  FILE *outfile;
  INT num;
{
        fprintf(outfile, "%d", num);
}


putisa(taskptr, isaptr)
  typetask *taskptr;
  typeisa *isaptr;
{
        puttumbler(taskptr->outp, isaptr);
}


putitemset (taskptr, itemset)
  typetask *taskptr;
  typeitemset itemset;
{
        if (itemset == NULL){
                fprintf (taskptr->outp, "  \nitemset empty\n");
		return;
	}
        for (; itemset; itemset = (typeitemset)((typeitemheader *)itemset)->next) {
                putitem (taskptr, itemset);
                if (!(((typeitemheader *)itemset)->next && ((typeitemheader *)itemset)->itemid == TEXTID && ((typeitemheader *)itemset)->next->itemid == TEXTID))
                        putc ('\n', taskptr->outp);
        }
}

putitem (taskptr, itemptr)
  typetask *taskptr;
  typeitem *itemptr;
{
        switch (((typeitemheader *)itemptr)->itemid) {
          case ISPANID:
                fprintf(taskptr->outp, "  ispan\n");
                putspan(taskptr, itemptr);
                break;
          case VSPANID:
                fprintf(taskptr->outp, "  vspan\n");
                putspan(taskptr, itemptr);
                break;
          case VSPECID:
                fprintf(taskptr->outp, "document: ");
                putisa(taskptr, &((typevspec *)itemptr)->docisa);
                fprintf(taskptr->outp, "\nspans");
                putitemset(taskptr, ((typevspec *)itemptr)->vspanset);
                break;
          case TEXTID:
                puttext(taskptr, itemptr);
                break;
          case LINKID:
                putisa (taskptr, &((typelink *)itemptr)->/*link*/address);
                break;
#ifndef DISTRIBUTION
          case SPORGLID:
                fprintf(taskptr->outp, "sporgl address: ");
                putisa (taskptr, &((typesporgl *)itemptr)->sporgladdress);
                fprintf(taskptr->outp, "\n   sporgl origin: ");
                putisa(taskptr, &((typesporgl *)itemptr)->sporglorigin);
                fprintf(taskptr->outp, "\n   sporgl width: ");
                putisa(taskptr, &((typesporgl *)itemptr)->sporglwidth);
                fprintf (taskptr->outp, "\n");
                break;
#endif
          default:
                error(taskptr, "illegal item id for putitem ");
                fprintf (taskptr->outp,"%x  %d\nd",itemptr,((typeitemheader *)itemptr)->itemid);
                return;
        }
}


putspan (taskptr, spanptr)
        typetask *taskptr;
        typespan *spanptr;
{
        fprintf(taskptr->outp, "   span address: ");
        puttumbler(taskptr->outp, &spanptr->stream);
        fprintf(taskptr->outp, "\n   span width: ");
        puttumbler(taskptr->outp, &spanptr->width);
}

puttext (taskptr, textptr)
        typetask *taskptr;
        typetext *textptr;
{
        write (fileno(taskptr->outp), textptr->string, textptr->length);
}

putspanpairset (taskptr, spanpairset)
  typetask *taskptr;
  typespanpairset spanpairset;
{
	if (!spanpairset)
		fprintf(taskptr->outp, "NULL relationship\n");
	else
            for (; spanpairset; spanpairset = spanpairset->nextspanpair)
                putspanpair (taskptr, spanpairset);
}

putspanpair (taskptr, spanpair)
  typetask *taskptr;
  typespanpair *spanpair;
{
        fprintf (taskptr->outp, "start1:  ");
        puttumbler (taskptr->outp, &spanpair->stream1);
        fprintf (taskptr->outp, "\nstart2:  ");
        puttumbler (taskptr->outp, &spanpair->stream2);
        fprintf (taskptr->outp, "\nwidth:  ");
        puttumbler (taskptr->outp, &spanpair->widthofspan);
        fprintf (taskptr->outp, "\n");
}

putcreatelink (taskptr, istreamptr)
  typetask *taskptr;
  typeisa *istreamptr;
{
        fprintf(taskptr->outp, "\nlink made: ");
        putisa(taskptr, istreamptr);
        fprintf(taskptr->outp, "\n");
}

putfollowlink (taskptr, specset)
  typetask *taskptr;
  typespecset specset;
{
        fprintf(taskptr->outp, "link endset is:\n");
        putitemset (taskptr, specset);
}

putretrievedocvspanset (taskptr, spansetptr)
        typetask *taskptr;
        typespanset *spansetptr;
{
        fprintf(taskptr->outp, "docvspans are:\n");
        putitemset(taskptr, *spansetptr);
}

putretrievedocvspan (taskptr, vspanptr)
        typetask *taskptr;
        typespan *vspanptr;
{
        fprintf(taskptr->outp, "docvspan is:\n");
        putspan(taskptr, vspanptr);
}

putretrievev (taskptr, vstuffsetptr)
  typetask *taskptr;
  typevstuffset *vstuffsetptr;
{
        fprintf (taskptr->outp, "\nvstuff is:\n");
        putitemset (taskptr, *vstuffsetptr);
}

putfindlinksfromtothree (taskptr, linkset)
  typetask *taskptr;
  typelinkset linkset;
{
        fprintf (taskptr->outp, "\nlinks\n");
        putitemset (taskptr, linkset);
}

putfindnumoflinksfromtothree (taskptr, num)
  typetask *taskptr;
  INT num;
{
        fprintf(taskptr->outp, "\nnumber of links: %d\n", num);
}

putfindnextnlinksfromtothree (taskptr, n, nextlinkset)
  typetask *taskptr;
  INT n;
  typelinkset nextlinkset;
{
        fprintf(taskptr->outp, "next number of links: %d\n", n);
        putitemset(taskptr, nextlinkset);
}

putshowrelationof2versions (taskptr, relation)
  typetask *taskptr;
  typespanpairset relation;
{
        fprintf(taskptr->outp, "relation between versions:\n");
        putspanpairset (taskptr, relation);
}

putcreatenewdocument (taskptr, newdocisaptr)
  typetask *taskptr;
  typeisa *newdocisaptr;
{
        fprintf(taskptr->outp, "new document: ");
        putisa(taskptr, newdocisaptr);
        fprintf(taskptr->outp,"\n\n");
}

putcreatenewversion (taskptr, newdocisaptr)
  typetask *taskptr;
  typeisa *newdocisaptr;
{
        fprintf(taskptr->outp, "new version: ");
        putisa(taskptr, newdocisaptr);
        fprintf (taskptr->outp, "\n");
}

putfinddocscontaining (taskptr, addressset)
  typetask *taskptr;
  typeitemset addressset;
{
        fprintf(taskptr->outp, "\ndocuments\n");
        putitemset (taskptr, addressset);
}

putretrieveendsets (taskptr, fromset, toset, threeset)
  typetask *taskptr;
  typespecset fromset;
  typespecset toset;
  typespecset threeset;
{
        fprintf (taskptr->outp, "\nfromset\n");
        putitemset (taskptr, fromset);
        fprintf (taskptr->outp, "\ntoset\n");
        putitemset (taskptr, toset);
        fprintf (taskptr->outp, "\nthreeset\n");
        putitemset (taskptr, toset);
}

putinsert (taskptr)
  typetask *taskptr;
{
}

putcopy (taskptr)
  typetask *taskptr;
{
}

putdeletevspan (taskptr)
  typetask *taskptr;
{
}

putrearrange (taskptr)
  typetask *taskptr;
{
}

putrequestfailed (taskptr)
  typetask *taskptr;
{
        fprintf (taskptr->outp,"?\n");
}
kluge()
{
}
putxaccount(taskptr)
  typetask *taskptr;
{
return(TRUE);
}
putcreatenode_or_account(taskptr,tp)
  typetask *taskptr;
  tumbler *tp;
{
  puttumbler(taskptr->outp,tp);
return(TRUE);

}

putopen(taskptr,tp)
  typetask *taskptr;
  tumbler *tp;

{
  puttumbler(taskptr->outp,tp);
return(TRUE);
	
}
putclose(taskptr)
  typetask *taskptr;
{
return(TRUE);
}

putquitxanadu(taskptr)
  typetask *taskptr;
{
  fprintf(taskptr->outp, "Good Bye.\n");
  return(TRUE);
}
