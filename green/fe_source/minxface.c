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


/***************
	**** The doc-id returned by possiblyopen() is sometimes ignored!!
	**** IS THIS CORRECT??  ECH 6-29
****************/


#include "fest.h"
#include "requests.h"

extern FILE *fromxu, *toxu;

  static tumbler
openinbackend(taskptr,tp,permission,mode)
  tasktype *taskptr;
  tumbler *tp;
  med permission;
  med mode;
{
  tumbler result;
  long n;
/*
 fprintf(stderr,"openinbackend perm = %d mode = %d\n",permission,mode);
*/
	 if (checkforopen(tp) != -1) {
/*
 fprintf(stderr,"closing\n");
*/
		 removefromopen(tp);
		 inittask (taskptr, stdin, toxu);
		 sendclose(taskptr,tp);
		 inittask (taskptr, fromxu, stdout);
		 if (!(takenumber (taskptr, &n) && n == CLOSE)) {
			 fprintf (taskptr->errp, "Error - close %d\n",n);
			 gerror("openinbackend close\n");
		 }
	 }
/*
 fprintf(stderr,"opening ");
 dumptumbler(tp);
 fprintf(stderr,"\n");
*/
	 inittask (taskptr, stdin, toxu);
	 sendopen(taskptr,tp,permission,mode);
	 inittask (taskptr, fromxu, stdout);
	 if (!(takenumber (taskptr, &n) && n == OPEN)) {
		 fprintf (taskptr->errp, "Error - open %d\n",n);
		 gerror("openinbackend\n");
	 } else {
		 takeopen(taskptr, &result);
		 if (!tumblereq(&result,tp)) {
			 /*made new version! */
/*
 fprintf(stderr,"made new version\n");
*/
			 hacknewversioninto(currentwindow,&result);
		 }
	 }
/*
 fprintf(stderr,"open returning ");
 dumptumbler(&result);
 fprintf(stderr,"\n");
*/
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

/* if open enough return
   else close if necessary
	open with permissions

	if the returned address is different
	 1: fix up vm
	 2: fix up the calling stuff (? how hard is this?)

*/
}

  static  void
possiblyopenfromspec(specp,permission,mode)
  spectype *specp;
  med permission;
  med mode;
{
	for(;specp;specp = specp->nextspec){
		 possiblyopen(&specp->docid,permission,mode);
	}
}

hacknewversioninto(windowp,versionp)
  windowtype * windowp;
  tumbler *versionp;
{

	/*fprintf(stderr,"in hacknewversioninto window");
	dumptumbler(versionp);
*/
	windowp->isendsetokflag = FALSE;
	tumblercopy(versionp,&windowp->windowview.viewspecptr->docid);
}


openenough(old,new)
  int old,new;
{
/*fprintf(stderr,"in openenough new = %d old = %d\n",new,old);*/
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

  void
retrievelinksetfrombackend (taskptr, specptr, whichend, linksetptr)
  tasktype *taskptr;
  spectype *specptr;
  med whichend;
  tumbleritemtype **linksetptr;
{
  linkrestrictiontype restriction;
  long n;
/*
 fprintf(stderr,"entering retrievelinksetfrombackend \n");
*/
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
/*
 fprintf(stderr,"%d returned",n);
*/
                gerror (" retrievelinksfrombackend\n");
        } else {
                takefindlinksfromtothree (taskptr, linksetptr);
        }
/*
 fprintf(stderr,"leaveing retrievelinksetfrombackend \n");
*/
}

  void
retrieveendsetfrombackend (taskptr,linkidptr,whichend,/*restrictedto,*/specsetptr)
  tasktype *taskptr;
  tumbler *linkidptr;
  med whichend;
  spectype **specsetptr /***restrictedto*/;
{
  long n;
/*
 fprintf(stderr ,"entering retrieveendsetfrombackend\n");
*/
        inittask (taskptr, stdin, toxu);
        sendfollowlink (taskptr, linkidptr, whichend /*, restrictedto*/);
        inittask (taskptr, fromxu, stdout);
        if (!(takenumber (taskptr, &n) && n == FOLLOWLINK)) {
                if (/*FALSE &&*//*TRUE||*/whichend != 3) {  
                        fprintf(stderr,"end = %d n = %d in link\n",whichend,n);
			dumptumbler(linkidptr);
                        gerror ("Error - retrieveendsetfrombackend\n");
                }
                *specsetptr = NULL;
        } else {
                takefollowlink (taskptr, specsetptr);
        }
/*
 fprintf(stderr ,"leaveing retrieveendsetfrombackend\n");
*/
} 

  void
retrieveendsetswithinspecsetfrombackend (taskptr,specset,fromsetptr,tosetptr,threesetptr)
  tasktype *taskptr;
  spectype *specset;
  spectype **fromsetptr, **tosetptr, **threesetptr;
{ 
  long n= 999;
/*
 fprintf(stderr,"retrieveendsetswithinspecsetfrombackend\n");
*/
        inittask (taskptr, stdin, toxu);
        sendretrieveendsets (taskptr, specset);
        inittask (taskptr, fromxu, stdout);

        if (!(takenumber (taskptr, &n) && n == RETRIEVEENDSETS)) { /* MSM 6/23 */
                fprintf (taskptr->errp, "Error - retrieveendsetswithinspecsetfrombackend  \n");
        } else {
                takeretrieveendsets(taskptr,fromsetptr, tosetptr, threesetptr);
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
/*
 fprintf(stderr,"retrievefrombackend\n");
*/
	possiblyopenfromspec(specptr,READBERT,BERTMODECOPYIF);

        inittask (&task, stdin, toxu);
        /* since we're making an expensive retrieve, let's get a lot */
        temp = nchars = specptr->specspanptr->sizeofspan;
        if (currentwindow->typeofwindow == JUMPLINK)
                specptr->specspanptr->sizeofspan = 1500/*800*/;
/*
 fprintf(stderr,"sending retrievev\n");
 dumpspec(specptr);
*/
        sendretrievev (&task, specptr);
        inittask (&task, fromxu, stdout);
        if (!(
           takenumber (&task, &n)
           && n == RETRIEVEV
           && takeretrievev (&task, &charspanptr, &a)
           && charspanptr)) {
/*
 fprintf(stderr,"  no text n=%d\n",n);
*/
                if(textptr)  *textptr = '\0';
                specptr->specspanptr->sizeofspan = temp;
                return;
        }
        localspec = specspanalloc ();
        localspan = localspec->specspanptr;
        movetumbler (&specptr->docid, &localspec->docid);

        movesmalltumbler (&specptr->specspanptr->vstartaddress, &localspan->vstartaddress);

        for (n = 0; charspanptr; charspanptr = charspanptr->nextcharspan) {
                localspan->sizeofspan = charspanptr->numberofcharactersinspan;
                puttextinvm (localspec, charspanptr);
                localspan->vstartaddress.smallmantissa[1] += localspan->sizeofspan;
                n += min (charspanptr->numberofcharactersinspan, nchars);
                if (textptr && n <= screenwidth) {
                        movmem (charspanptr->charinspanptr, textptr, min (/*screenwidth*/nchars, charspanptr->numberofcharactersinspan));
                        textptr += min (nchars, charspanptr->numberofcharactersinspan);
                }
                if (nchars > charspanptr->numberofcharactersinspan)
                        nchars -= charspanptr->numberofcharactersinspan;
        }
        if(textptr) *textptr = '\0';
        specptr->specspanptr->sizeofspan = temp;
        specfree (localspec);
        tfree (&task.tempspace);
/*
 fprintf(stderr,"nchars=%d\n",nchars);
*/
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

  void
inserttextinbackend (taskptr, docidptr, vsaptr, charspanptr)
  tasktype *taskptr;
  tumbler *docidptr;
  smalltumbler *vsaptr;
  charspantype *charspanptr;
{
  long n;
/*fprintf(stderr,"inserttextinbackend\n");
*/
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
/*
gotoxy(0,screenheight-1);
fprintf(stderr,"%d %d\n",m,n);
*/
                fprintf (taskptr->errp, "Error - deletefrombackend   \n");
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
 possiblyopen(docidptr,READBERT,BERTMODECOPYIF);
}

  void
createnewversioninbackend (taskptr, olddocidptr, newdocidptr)
  tasktype *taskptr;
  tumbler *olddocidptr, *newdocidptr;
{ 
  long n;
/*possiblyopen(olddocidptr);*/
        inittask (taskptr, stdin, toxu);
        sendcreatenewversion (taskptr, olddocidptr);
        inittask (taskptr, fromxu, stdout);
        if (!(takenumber (taskptr, &n) && n == CREATENEWVERSION)) {
                fprintf (taskptr->errp, "Error - createnewversioninbackend  \n");
        } else {
                takecreatenewversion (taskptr, newdocidptr);
        }
 possiblyopen(newdocidptr,READBERT,BERTMODECOPYIF);
}

  void
createlinkinbackend (taskptr, docid, fromset, toset, threeset)
  tasktype *taskptr;
  tumbler *docid;
  spectype *fromset, *toset, *threeset;
{
  tumbler link;
  long n;

	*docid = possiblyopen(docid,WRITEBERT,BERTMODECOPYIF);

        inittask (taskptr, stdin, toxu);
        sendcreatelink (taskptr, docid, fromset, toset, threeset);
        inittask (taskptr, fromxu, stdout);
        if (!(takenumber (taskptr, &n) && n == CREATELINK)) {
                fprintf (stderr, "%d ", n);
                fprintf (taskptr->errp, "Error - createlinkinbackend\n");
        } else {
                takecreatelink (taskptr, &link);
	/*	dumptumbler(&link);*/
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
/*
 fprintf(stderr,"rearrange callse with");
 dumpcutseq(cutseqptr);
*/
        inittask (taskptr, fromxu, stdout);
        if (!(takenumber (taskptr, &n) && n == REARRANGE)) {
                fprintf (stderr, "%d ", n);
                fprintf (taskptr->errp, "Error - rearrangeinbackend  \n");
        }
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
retrievedocvspanset(docidptr,spansetptr)
  tumbler *docidptr;
  spantype **spansetptr;
{
  tasktype task;
  long n;

/*
fprintf(stderr,"\nretrievedocvspanset\n");
dumptumbler(docidptr);
fprintf(stderr,"\n");
*/

        inittask (&task, stdin, toxu);
        sendretrievedocvspanset(&task,docidptr);
        inittask (&task, fromxu, stdout);
        if (!(takenumber(&task,&n) && n == RETRIEVEDOCVSPANSET)) {
                fprintf (task.errp, "Error - retrievedocvspanset\n");
        } else {
        	takeretrievedocvspanset(&task,spansetptr);
/* dumpsmalltumbler(&(*spansetptr)->vstartaddress); */
        }
/* fprintf(stderr,"\n leaving retrievedocvspanset\n"); */
}
