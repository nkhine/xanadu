/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:    minxface.d
***************************************************/

#include "fest.h"
#include "requests.h"

extern FILE *fromxu, *toxu;

#define RETRIEVELEN	1500
#define VMSTRLEN	80


  static tumbler
openinbackend(taskptr,tp,permission,mode)
  tasktype *taskptr;
  tumbler *tp;
  med permission;
  med mode;
{
  tumbler result;
  long n;
	 if (checkforopen(tp) != -1) {
		 removefromopen(tp);
		 inittask (taskptr, stdin, toxu);
		 sendclose(taskptr,tp);
		 inittask (taskptr, fromxu, stdout);
		 if (!(takenumber (taskptr, &n) && n == CLOSE)) {
			 fprintf (taskptr->errp, "Error - close %d\n",n);
			 gerror("openinbackend close\n");
		 }
	 }
	 inittask (taskptr, stdin, toxu);
	 sendopen(taskptr,tp,permission,mode);
	 inittask (taskptr, fromxu, stdout);
	 if (!(takenumber (taskptr, &n) && n == OPEN)) {
		 fprintf (taskptr->errp, "Error - open %d\n",n);
		 gerror("openinbackend\n");
	 } else {
		 takeopen(taskptr, &result);
	 }
	 return(result);
 }

   static tumbler
possiblyopen(tp,permission,mode)
   tumbler *tp;
   med permission;
   med mode;
 {
   tasktype task;
   tumbler result;
	if (openenough(checkforopen(tp), permission)) {
		return(*tp);
	} else {
		result = openinbackend(&task,tp,permission,mode);
		addtoopen(&result,0L,0L,(long)permission);
	}
	return(result);
}

  static  void
possiblyopenfromspec(specp,permission,mode)
  spectype *specp;
  med permission;
  med mode;
{
	for(;specp;specp = specp->nextspec) {
		specp->docid = possiblyopen(&specp->docid,permission,mode);
	}
}

  bool
openenough(old,new)
  int old,new;
{
	switch(new){
	  case  READBERT:
	    switch(old){
	      case  READBERT:
		return(TRUE);
	      case  WRITEBERT:
		return(TRUE);
	      default:
		return FALSE;
	    }
	  case  WRITEBERT:
	    switch(old){
	      case  READBERT:
		return FALSE;
	      case  WRITEBERT:
		return(TRUE);
	      default:
		return FALSE;
	    }
	  default:
	    switch(old){
	      case  READBERT:
		return FALSE;
	      case  WRITEBERT:
		return FALSE;
	      default:
		return FALSE;
	    }
	}
}


  bool
negotiateprotocol(feproto)
  char *feproto;
{
  tasktype task;
  char beproto[PROTOLEN];

	inittask(&task,stdin, toxu);
        sendprotocol(&task,feproto);
	inittask(&task,fromxu,stdout);     
        if (!takeprotocol(&task,beproto) || strcmp(feproto,beproto) !=  0) {
		return(FALSE);
	}
	return(TRUE);
}

  void
retrievedocvspanset(docidptr,spansetptr)
  tumbler *docidptr;
  spantype **spansetptr;
{
  tasktype task;
  long n;

	*docidptr = possiblyopen(docidptr,READBERT,BERTMODECOPYIF);

        inittask (&task, stdin, toxu);
        sendretrievedocvspanset(&task,docidptr);
        inittask (&task, fromxu, stdout);
        if (!(takenumber(&task,&n) && n == RETRIEVEDOCVSPANSET)) {
                fprintf (task.errp, "Error - retrievedocvspanset\n");
        } else {
        	takeretrievedocvspanset(&task,spansetptr);
        }
}

  void
retrievefrombackend (specptr, textptr)
  spectype *specptr;
  char *textptr;
{
  tasktype task;
  charspantype *charspanptr;
  tumbleritemtype *a;
  spectype *localspec;
  spantype *localspan;
  long n;
  long temp, nchars;
  spectype *specspanalloc(); 

	possiblyopenfromspec(specptr,READBERT,BERTMODECOPYIF);

        inittask (&task, stdin, toxu);
        temp = nchars = specptr->specspanptr->sizeofspan;
        specptr->specspanptr->sizeofspan = RETRIEVELEN;
        sendretrievev (&task, specptr);
        inittask (&task, fromxu, stdout);
        if (!(
           takenumber (&task, &n)
           && n == RETRIEVEV
           && takeretrievev (&task, &charspanptr, &a)
           && charspanptr)) {
                if(textptr)  *textptr = '\0';
                specptr->specspanptr->sizeofspan = temp;
                return;
        }
        localspec = specspanalloc ();
        localspan = localspec->specspanptr;
        movetumbler (&specptr->docid, &localspec->docid);
        movesmalltumbler (&specptr->specspanptr->vstartaddress,
			    &localspan->vstartaddress);

        for (n = 0; charspanptr; charspanptr = charspanptr->nextcharspan) {
                localspan->sizeofspan = charspanptr->numberofcharactersinspan;
                puttextinvm (localspec, charspanptr);
                localspan->vstartaddress.smallmantissa[1] += localspan->sizeofspan;
                n += min (charspanptr->numberofcharactersinspan, nchars);
                if (textptr) {
                        movmem (charspanptr->charinspanptr, textptr, min (nchars, charspanptr->numberofcharactersinspan));
                        textptr += min (nchars,charspanptr->numberofcharactersinspan);
                }
                if (nchars > charspanptr->numberofcharactersinspan)
                        nchars -= charspanptr->numberofcharactersinspan;
        }
        if(textptr) *textptr = '\0';
        specptr->specspanptr->sizeofspan = temp;
        specfree (localspec);
        tfree (&task.tempspace);
} 

void retrievelinksetfrombackend (taskptr, specptr, whichend, linksetptr)
  tasktype *taskptr;
  spectype *specptr;
  med whichend;
  tumbleritemtype **linksetptr;
{
  linkrestrictiontype restriction;
  long n;

        restriction.fromset = NULL;
        restriction.toset = NULL;
        restriction.threeset = NULL;
        restriction.homeset = NULL;
        switch (whichend) {
          case 1:
                restriction.fromset = specptr;
                break;
          case 2:
                restriction.toset = specptr;
                break;
          case 3:
                restriction.threeset = specptr;
        }
        inittask (taskptr, stdin, toxu);
        sendfindlinksfromtothree (taskptr, &restriction);
        inittask (taskptr, fromxu, stdout);
        if ((!takenumber (taskptr, &n) && n == FINDLINKSFROMTOTHREE)) {
                gerror (" retrievelinksfrombackend\n");
        } else {
                takefindlinksfromtothree (taskptr, linksetptr);
        }
}

  void
retrieveendsetfrombackend (taskptr,linkidptr,whichend,specsetptr)
  tasktype *taskptr;
  tumbler *linkidptr;
  med whichend;
  spectype **specsetptr;
{
  long n;
        inittask (taskptr, stdin, toxu);
        sendfollowlink (taskptr, linkidptr, whichend);
        inittask (taskptr, fromxu, stdout);
        if (!(takenumber (taskptr, &n) && n == FOLLOWLINK)) {
                if (whichend != 3) {  
                        fprintf(stderr,"end = %d n = %d in link\n",whichend,n);
			dumptumbler(linkidptr);
                        gerror ("Error - retrieveendsetfrombackend\n");
                }
                *specsetptr = NULL;
        } else {
                takefollowlink (taskptr, specsetptr);
        }
} 

  void
retrieveendsetswithinspecsetfrombackend (taskptr,specset,fromsetptr,tosetptr,threesetptr)
  tasktype *taskptr;
  spectype *specset;
  spectype **fromsetptr, **tosetptr, **threesetptr;
{ 
  long n;
        inittask (taskptr, stdin, toxu);
        sendretrieveendsets (taskptr, specset);
        inittask (taskptr, fromxu, stdout);

        if (!(takenumber (taskptr, &n) && n == RETRIEVEENDSETS)) {
                fprintf (taskptr->errp, "Error - retrieveendsetswithinspecsetfrombackend  \n");
        } else {
                takeretrieveendsets(taskptr,fromsetptr, tosetptr, threesetptr);
        }
}


  void
inserttextinbackend (taskptr, docidptr, vsaptr, charspanptr)
  tasktype *taskptr;
  tumbler *docidptr;
  smalltumbler *vsaptr;
  charspantype *charspanptr;
{
  long n;
	*docidptr = possiblyopen(docidptr,WRITEBERT,BERTMODECOPYIF);

        inittask (taskptr, stdin, toxu);
        sendinsert (taskptr, docidptr, vsaptr, charspanptr);
        inittask (taskptr, fromxu, stdout);
        if (!(takenumber (taskptr, &n) && (n == INSERT))){
              fprintf(taskptr->errp,"Error - inserttextinbackend  = %d  \n",n);
	}
}

  void
deletefrombackend (taskptr, docidptr, spanptr)
  tasktype *taskptr;
  tumbler *docidptr;
  spantype *spanptr;
{
  bool m;
  long n;

	*docidptr = possiblyopen(docidptr,WRITEBERT,BERTMODECOPYIF);

        inittask (taskptr, stdin, toxu);
        senddeletevspan (taskptr, docidptr, spanptr);
        inittask (taskptr, fromxu, stdout);

        if ( (m = !(takenumber (taskptr, &n)) && n == DELETEVSPAN))  {
                fprintf (taskptr->errp, "Error - deletefrombackend   \n");
        }

}

  void
rearrangeinbackend (taskptr, docidptr, cutseqptr)
  tasktype *taskptr;
  tumbler *docidptr;
  cutseqtype *cutseqptr;
{
  long n;

	*docidptr = possiblyopen(docidptr,WRITEBERT,BERTMODECOPYIF);

        inittask (taskptr, stdin, toxu);
        sendrearrange (taskptr, docidptr, cutseqptr);
        inittask (taskptr, fromxu, stdout);
        if (!(takenumber (taskptr, &n) && n == REARRANGE)) {
                fprintf (stderr, "%d ", n);
                fprintf (taskptr->errp, "Error - rearrangeinbackend  \n");
        }
}

  void
copyinbackend (taskptr, docidptr, vsaptr, specptr)
  tasktype *taskptr;
  tumbler *docidptr;
  smalltumbler *vsaptr;
  spectype *specptr;
{
  long n;

	*docidptr = possiblyopen(docidptr,WRITEBERT,BERTMODECOPYIF);

        inittask (taskptr, stdin, toxu);
	sendcopy (taskptr, docidptr, vsaptr, specptr);
        inittask (taskptr, fromxu, stdout);
        if (!(takenumber (taskptr, &n) && n == COPY)) {
                fprintf (stderr, "%d ", n);
                fprintf (taskptr->errp, "Error - copyinbackend  \n");
        }
}

  void
createnewdocumentinbackend (taskptr, docidptr)
  tasktype *taskptr;
  tumbler *docidptr;
{ 
  long n;
  smalltumbler v;
  charspantype c;
        inittask (taskptr, stdin, toxu);
        sendcreatenewdocument (taskptr);
        inittask (taskptr, fromxu, stdout);
        if (!(takenumber (taskptr, &n) && n == CREATENEWDOCUMENT)) {
                fprintf (taskptr->errp, "Error - createnewdocumentinbackend   \n");
        } else {
                takecreatenewdocument (taskptr, docidptr);
        }
	*docidptr = possiblyopen(docidptr,READBERT,BERTMODECOPYIF);
}

  void
createlinkinbackend (taskptr, docid, fromset, toset, threeset, linkptr)
  tasktype *taskptr;
  tumbler *docid;
  spectype *fromset, *toset, *threeset;
  tumbler *linkptr;
{
  long n;
	*docid = possiblyopen(docid,WRITEBERT,BERTMODECOPYIF);

        inittask (taskptr, stdin, toxu);
        sendcreatelink (taskptr, docid, fromset, toset, threeset);
        inittask (taskptr, fromxu, stdout);
        if (!(takenumber (taskptr, &n) && n == CREATELINK)) {
                fprintf (stderr, "%d ", n);
                fprintf (taskptr->errp, "Error - createlinkinbackend\n");
        } else {
                takecreatelink (taskptr, linkptr);
	}
}

  void
createnewversioninbackend (taskptr, olddocidptr, newdocidptr)
  tasktype *taskptr;
  tumbler *olddocidptr, *newdocidptr;
{ 
  long n;
        inittask (taskptr, stdin, toxu);
        sendcreatenewversion (taskptr, olddocidptr);
        inittask (taskptr, fromxu, stdout);
        if (!(takenumber (taskptr, &n) && n == CREATENEWVERSION)) {
                fprintf (taskptr->errp, "Error - createnewversioninbackend  \n");
        } else {
                takecreatenewversion (taskptr, newdocidptr);
        }
	*newdocidptr = possiblyopen(newdocidptr,READBERT,BERTMODECOPYIF);
}

  void
finddocscontaininginbackend (taskptr, specset, docset)
  tasktype *taskptr;
  spectype *specset;
  tumbleritemtype **docset;
{
  long n;

	possiblyopenfromspec(specset,READBERT,BERTMODECOPYIF);
        inittask (taskptr, stdin, toxu);
        sendfinddocscontaining (taskptr, specset);
        inittask (taskptr, fromxu, stdout);
        if (!(takenumber (taskptr, &n) && n == FINDDOCSCONTAINING)) {
                fprintf (stderr, "%d", n);
                fprintf (taskptr->errp, "Error - finddocscontaining\n");
        } else {
                takefinddocscontaining (taskptr, docset,&n);
                if (n == 0) {
                        gerror ("no docs found in finddocscontaining");
                }
        }
}

  void
showrelationof2versionsinbackend (taskptr, version1, version2, correspondencesetptr)
  tasktype *taskptr;
  spectype *version1, *version2;
  correspondencetype **correspondencesetptr;
{
  long n;
	possiblyopenfromspec(version1,READBERT,BERTMODECOPYIF);
	possiblyopenfromspec(version2,READBERT,BERTMODECOPYIF);
        inittask (taskptr, stdin, toxu);
        sendshowrelationof2versions (taskptr, version1, version2);
        inittask (taskptr, fromxu, stdout);
        if (!(takenumber (taskptr, &n) && n == SHOWRELATIONOF2VERSIONS)) {
                fprintf (taskptr->errp, "Error - showrelationof2versions\n");
        } else
                takeshowrelationof2versions (taskptr, correspondencesetptr);
}

  void
tellbackendtoquit()
{
  tasktype task;

        inittask (&task, stdin, toxu);
        senddiskexit (&task);
        inittask (&task, fromxu, stdout);
        takediskexit (&task);
}
