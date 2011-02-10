/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* get2.d - Udanax input routines - file #2 - no front end version */
/* as of Jul 12 19:00:00 */

#include "xanadu.h"

  bool
getnum (taskptr, numptr)  /* inside temporary */
  typetask *taskptr;
  INT *numptr;
{
  metachar c;
  INT num;
  bool flag;

	flag = FALSE;   /* should check for minus */
	for (num = 0; (c = getc(taskptr->inp)) != EOF && isdigit(c);) {
		num = num * 10 + c - '0';
		flag = TRUE;
	}
	if (!flag){
		fprintf (taskptr->errp, "no number\n"); 
#ifndef DISTRIBUTION
		if ( c == '?'||c == 'h'){
			system("cat /usr3/xu/requests.j");
		}else if(c == '!'){
			system("csh");
		}
#endif
	}else {
		 ungetc (c, taskptr->inp);
		 *numptr = num;
	}
	return (flag);
}

  bool
eatchar (taskptr, c)
  typetask *taskptr;
  char c;
{
  metachar m;

	if ((m = getc (taskptr->inp)) != c) {
		ungetc (m, taskptr->inp);
		return (FALSE);
	}
	else
		return (TRUE);
}

  bool
needchar (taskptr, c)
  typetask *taskptr;
  char c;
{
	if (!eatchar (taskptr, c)) {
		fprintf (taskptr->errp, "needed a ");
		if (c == '\n')
			fprintf (taskptr->errp, "newline\n");
		else
			fprintf (taskptr->errp, "%c\n");
		return(FALSE);
	}
	return(TRUE);
}

  bool
getnumber (taskptr, numptr)
  typetask *taskptr;
  INT *numptr;
{
	return (getnum (taskptr, numptr) && needchar (taskptr, '\n'));
}

  bool
gettumbler (taskptr, tumblerptr)
  typetask *taskptr;
  tumbler *tumblerptr;
{
  INT i;

	tumblerclear (tumblerptr);
	if (eatchar (taskptr, '-'))
		tumblerptr->sign = 1;
	for (i = 0; i < NPLACES; ++i) {
		if (!getnum (taskptr, &tumblerptr->mantissa[i]))
			return (FALSE);
		if (tumblerptr->mantissa[i] == 0 && i == 0) {
			--tumblerptr->exp;
			--i;
		}
		if (!eatchar (taskptr, '.'))
			break;
	}
	if (eatchar (taskptr, '.')) {
		fprintf (taskptr->errp, "tumbler overflow\n");
		return (FALSE);
	}
	for (i = 0; i < NPLACES && tumblerptr->mantissa[i] == 0; ++i);
	if (i == NPLACES)
		tumblerptr->exp = 0;

	return (needchar (taskptr, '\n'));
}

  bool
getbool (taskptr, boolptr)
  typetask *taskptr;
  bool *boolptr;
{
  int c;

	prompt (taskptr, "(y/n) ");
	c =  (getc (taskptr->inp));
	if (isupper (c))
		c = tolower (c);
	eatchar (taskptr, '\n');
	if (c == 'y') {
		*boolptr = TRUE;
		return (TRUE);
	} else if (c == 'n') {
		*boolptr = FALSE;
		return (TRUE);
	} else {
		error (taskptr, "need 'y' or 'n'");
		return (FALSE);
	}
}

  bool
getisa (taskptr, isaptr)
  typetask *taskptr;
  typeisa *isaptr;
{
	return (gettumbler (taskptr, isaptr));
}

  bool
getvsa (taskptr, vsaptr)
  typetask *taskptr;
  tumbler *vsaptr;
{
	return (gettumbler (taskptr, vsaptr));
}

  bool
getrequest (taskptr, requestptr)
  typetask *taskptr;
  typerequest *requestptr;
{
  bool validrequest();
  int c;

	prompt (taskptr, "\nrequest? ");
	c = getc (taskptr->inp);
	if ((INT)c == EOF) {
		fprintf(stderr,"endfile\n");
		taskptr->inp = stdin;
	}/* else if (c == ':') {
		while ((c = getc (taskptr->inp)) != '\n');
		return (FALSE);
	}*/
	ungetc (c, taskptr->inp);
	return (getnumber (taskptr, requestptr) &&
		validrequest (taskptr, *requestptr) );
}

  bool
validrequest (taskptr, request)
  typetask *taskptr;
  typerequest request;
{
	if (request >= 0 && request < NREQUESTS && requestfns[request] != NULL)
		return (TRUE);
	fprintf (taskptr->errp, "invalid request: %d\n", request);
	return (FALSE);
}

  bool
validaccount(taskptr, accountptr)
  typetask *taskptr;
  typeisa *accountptr;
{
	return(TRUE);
}

  bool
getspecset (taskptr, specsetptr)
  typetask *taskptr;
  typespecset *specsetptr;
{
  bool any, type;
  bool getvspec(),getbool(),getspan(),gettext();
  typespec *specset;
  INT *taskalloc();

	for (;;) {
		prompt (taskptr, "any spans or vspecs? ");
		if (!getbool (taskptr, &any))
			return (FALSE);
		if (!any) {
			*specsetptr = NULL;
			return (TRUE);
		}
		prompt (taskptr,"a span? ");
		if(!getbool(taskptr,&type))
			return(FALSE);
		if(type) {
		      specset=(typespec *)taskalloc(taskptr,sizeof(typespan));
			if (!getspan(taskptr,specset,ISPANID))
				return(FALSE);
		}else{
		     specset=(typespec *)taskalloc(taskptr,sizeof(typevspec));
			if (!getvspec(taskptr,specset))
				return(FALSE);
		}
		*specsetptr = specset;
		specsetptr = (typespecset *)&(((typevspec *)specset)->next);
	}
}

  bool
getvspec (taskptr, vspecptr)
  typetask *taskptr;
  typevspec *vspecptr;
{
  bool getspanset();
	vspecptr->itemid = VSPECID;
	vspecptr->next = NULL;
	prompt(taskptr, "document=> ");
	if(!(getisa(taskptr,&vspecptr->docisa)
			&& getspanset(taskptr, &vspecptr->vspanset,VSPANID)))
			return(FALSE);
	return(TRUE);
}

  bool
getspanset(taskptr, spansetptr,id)
  typetask *taskptr;
  typespanset *spansetptr;
  char id;
{
  bool any;
  typespan *spanset;
  INT *taskalloc();
  bool getspan();

	for (;;) {
		prompt (taskptr, "any spans? ");
		if (!getbool (taskptr, &any))
			return (FALSE);
		if (!any) {
			*spansetptr = NULL;
			return (TRUE);
		}
		spanset = (typespan *) taskalloc(taskptr, sizeof(typespan));
		if (!getspan (taskptr, spanset, id))
			return(FALSE);
		*spansetptr = spanset;
		spansetptr = &spanset->next;
	}
}


  bool
getspan (taskptr, spanptr, id)
  typetask *taskptr;
  typespan *spanptr;
  char id;
{
	prompt(taskptr, "enter span\n       start=> ");
	if (!getisa(taskptr, &spanptr->stream))
		return(FALSE);
	spanptr->itemid = id;
	prompt(taskptr, "	width=> ");
	if(!(getisa(taskptr, &spanptr->width)))
			return(FALSE);
	return(TRUE);
}


  bool
gettextset (taskptr, textsetptr)
  typetask *taskptr;
  typetextset *textsetptr;
{
  typetext *textset;
  INT *taskalloc();
  bool gettext();

	for (;;) {
		textset = (typetext *) taskalloc(taskptr, sizeof(typetext));
		if (!gettext(taskptr, textset)) {
			*textsetptr = NULL;
			break;
		}
		*textsetptr = textset;
		textsetptr = &textset->next;
	}
	return(TRUE);
}

  bool
gettext (taskptr, textptr)
  typetask *taskptr;
  typetext *textptr;
{
	if (!fgets (textptr->string, GRANTEXTLENGTH, taskptr->inp)) {
		textptr->length = 0;
		return (FALSE);
	}
	textptr->length = strlen (textptr->string);
	if (textptr->length <= 1)
		return(FALSE);
			       /* remove newlines */
/*
	if (textptr->string[textptr->length - 1] == '\n')
		textptr->string[--textptr->length] = '\0';
*/
	textptr->itemid = TEXTID;
	return(TRUE);
}


  bool
getcutseq(taskptr, cutseqptr)
  typetask *taskptr;
  typecutseq *cutseqptr;
{
  INT i;
  bool anycuts;
  tumbler cutaddress;

	i = 0;
	for (;;) {
		prompt(taskptr, "any cuts? ");
		if (!getbool(taskptr, &anycuts))
			return(FALSE);
		if (!anycuts)
			break;
		prompt(taskptr, "cut address=> ");
		if(!getvsa(taskptr, &cutseqptr->cutsarray[i]))
			return(FALSE);
		if(++i > MAXCUTS)
			break;
		cutseqptr->numberofcuts = i;
	}
	return(TRUE);
}


  bool
getboolset(taskptr, boolsetptr)
  typetask *taskptr;
  typeboolsetnode **boolsetptr;
{
  bool disjunct;
  typeboolsetnode * boolset;
  INT *taskalloc();

	for (;;) {
		 prompt (taskptr, "any disjunctions? ");
		 if(!getbool(taskptr, &disjunct))
			return(FALSE);
		 if(!disjunct) {
			*boolsetptr = NULL;
			return(TRUE);
		}
		boolset = (typeboolsetnode *) taskalloc(taskptr, sizeof(typeboolsetnode));
		prompt (taskptr, "enter disjunction=> ");
		if(!getspanset(taskptr, &boolset->val, ISPANID))
			return(FALSE);
/*zzz  &  zzz*/
		boolset->itemid = NODEID;
		*boolsetptr = boolset;
		boolsetptr = &boolset->next;
	}
}
