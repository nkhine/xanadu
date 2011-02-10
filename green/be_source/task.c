/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* Udanax task stuff */

#include "xanadu.h"

extern INT errno;

inittask (taskptr)
  typetask *taskptr;
{
  FILE /**fd,*/ *fopen();

        taskptr->inp = stdin;
        taskptr->outp = stdout;
        taskptr->errp = stderr;
        taskptr->tempspacehead = NULL;
        taskptr->tempspacetail = NULL;
/*	tumblerclear(&taskptr->account);*/
}


bool getaccount (taskptr, accountptr)
  typetask *taskptr;
  typeisa *accountptr;
{
  bool validaccount();
tumblerclear (accountptr);
return (TRUE);


       /* prompt (taskptr, "account? ");*/
        
/*           gettumbler (taskptr, accountptr)
        && validaccount(taskptr, accountptr);
	fprintf(stderr,"in get account \n");
	return(TRUE); */
}



  INT *
taskalloc (taskptr, nbytes)
  typetask *taskptr;
  INT nbytes;
{
  INT *talloc();

       return (talloc (taskptr, nbytes));
}

/* lint will go crazy */
  INT *
talloc (taskptr, nbytes)
  typetask *taskptr;
  INT nbytes;
{
  typetthingheader *this, *head;
  INT *ealloc();

        head = taskptr->tempspacehead;
        this = (typetthingheader *)eallocwithtag((unsigned)(sizeof(typetthingheader) + nbytes), TASKEDTAG);
        if (((unsigned)this) & 1)
                qerror ("ealloc returned unaligned pointer.\n");
        this->tnext = head;
        this->tlast = NULL;
        if (head)
                head->tlast = this;
        taskptr->tempspacehead = this;
        ++this;
        if (((unsigned)this) & 1)
                qerror ("talloc trying to return unaligned pointer.\n");
        return ((INT *)this);
}

tfree (taskptr)
  typetask *taskptr;
{
  typetthingheader *ptr;
  typetthingheader *p;

        for (ptr = taskptr->tempspacehead; ptr; ptr = p) {
                p = ptr->tnext;
                checkpointer ("tfree: ", ptr);
                /*if (ptr > (char *) 0x1b0000)
                        gerror ("tfree: ptr pointing into stack region.\n");*/
                efree((char*)ptr);
        }
        taskptr->tempspacehead = NULL;
}

tfreeexplicit (taskptr, ptr)
  typetask *taskptr;
  char *ptr;
{
  typetthingheader *header;

checkpointer ("tfreeexplicit: ", ptr);
/*return;*/

        header = (typetthingheader *)ptr;
        --header;
        if (header->tnext)
                header->tnext->tlast = header->tlast;
        if (header->tlast)
                header->tlast->tnext = header->tnext;

        if (header == taskptr->tempspacehead)
                taskptr->tempspacehead = header->tnext;

        efree((char*)header);
}

tfreeitemset (taskptr, itemset)
  typetask *taskptr;
  typeitemset itemset;
{
  typeitem *nextitem;
        
	for (; itemset; itemset = nextitem) {
                checkitem ("tfreeitemset: ", itemset);
                nextitem = (typeitem *)((typeitemheader *)itemset)->next;
                if (((typeitemheader *)itemset)->itemid == VSPECID)
                        tfreeitemset (taskptr, (typeitemset)((typevspec *)itemset)->vspanset);
                tfreeexplicit (taskptr, (char*)itemset);
        }
}
