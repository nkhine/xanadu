/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* get2fe.d - Udanax bottom-level input routines - front end version */
/* as of Jul 12 19:55:45 */

#include "xanadu.h"
#include "players.h"

#define WORDELIM '~'
#define TUMDELIM '.'
#define SPANFLAG 's'
#define VSPECFLAG 'v'
#define TEXTFLAG 't'

FILE *logfile;
FILE *nulllog;
FILE *reallog;

extern bool logstuff;
FILE *interfaceinput;
FILE *febelog;

pushc (taskptr, c)
  typetask *taskptr;
  char c;
{
	if (taskptr->charinbuff)
		error (taskptr, "charbuff occupied\n");
	else {
		taskptr->charinbuff = TRUE;
		taskptr->charbuff = c;
	}
}

  char
pullc (taskptr)
  typetask *taskptr;
{
  INT temp;

	if (taskptr->charinbuff) {
		taskptr->charinbuff = FALSE;
		return (taskptr->charbuff);
	} else {
		temp = getc (taskptr->inp) ;
		if (temp == EOF) {
			fprintf(stderr, "EOF on user %d in pullc\n", user);
			frontenddied();	/* does longjmp in backenddaemon, exit in backend */
			/*fprintf (stderr, "Premature end-of-file in backend\n");
			diskexit (); // try to avoid screwing enf.enf //
			gerror("pullc");*/
		}
		temp &= 0x7f;
		if (logstuff && interfaceinput && interfaceinput != nulllog) {
			if (temp == WORDELIM)
				putc ('\n', interfaceinput);
			else
				putc (temp, interfaceinput);
		}
		if (febelog && febelog != nulllog) {
			putc(temp, febelog);
		}
		return (temp);
	}
}

  bool
gettumbler (taskptr, tumblerptr)
  typetask *taskptr;
  tumbler *tumblerptr;
{
  char c;
  bool gettdigit();
  bool getnum();
  INT i, num, value;
/*fprintf(taskptr->errp,"X gettumbler\n"); */
	tumblerclear (tumblerptr);
	getnum (taskptr, &num);
	tumblerptr->exp = -num;
	for (i = 0; gettdigit(taskptr, &value); i++) {
		if (i > NPLACES) {
		       error (taskptr, "gettumbler overflow\n");
		       return (FALSE);
		}
		tumblerptr->mantissa[i] = value;
	}
/*fprintf (logfile, " "); puttumbler (logfile, tumblerptr); */
	return ((c = pullc (taskptr)) == WORDELIM || c == '\n');
}

  bool
gettdigit (taskptr, valueptr)
  typetask *taskptr;
  INT *valueptr;
{
  char c;
  bool getnum();
	if ((c = pullc (taskptr)) != TUMDELIM) {
		pushc (taskptr, c);
		return (FALSE);
	}
	return (getnum (taskptr, valueptr));
}

  bool
getnum (taskptr, numptr)
  typetask *taskptr;
  INT *numptr;
{
  char c;
  INT num;
  bool flag;
/* fprintf(taskptr->errp,"X getnum\n"); */
	num = 0;
	flag = FALSE;
	while ((c = pullc (taskptr)) /* && putc (c,taskptr->errp) */ && isdigit(c)) {
		num = num * 10 + c - '0';
		flag = TRUE;
	}
/* fprintf(taskptr->errp,"\n"); */
	pushc (taskptr, c);
	*numptr = num;
	return (flag);
}

  bool
getnumber (taskptr, numptr)
  typetask *taskptr;
  INT *numptr;
{
  char c;
  INT num;
  bool flag;
	num = 0;
	flag = FALSE;
	while ((c = pullc (taskptr)) && isdigit(c)) {
		num = num * 10 + c - '0';
		flag = TRUE;
	}
	*numptr = num;
/*fprintf (logfile, " %d", *numptr); */
	return (flag && (c == WORDELIM || c == '\n'));
}

  bool
eatchar (taskptr, c)
  typetask *taskptr;
  char c;
{
  metachar m;

	if ((m = pullc (taskptr)) != c) {
		pushc (taskptr, m);
		return (FALSE);
	} else {
		return (TRUE);
	}
}

  bool
getspecset (taskptr, specsetptr)
  typetask *taskptr;
  typespecset *specsetptr;
{
  INT num;
  char c, c1;
  typespecset specset;
  bool getvspec();
  bool getspan();

	*specsetptr = NULL;
	if (!getnumber (taskptr, &num)) {
		return (FALSE);
	}
	if (num == 0)
		return (TRUE);
	while (num--) {
		c = pullc(taskptr);
		if ((c != SPANFLAG) && (c != VSPECFLAG))
			return(FALSE);
/*mightn't work*/if ((c1 = pullc(taskptr)) != WORDELIM && c1 != '\n')
			return(FALSE);

		if (c == SPANFLAG) {
		      specset=(typespecset)taskalloc(taskptr,sizeof(typespan));
			if (!getspan(taskptr,specset,ISPANID))
				return(FALSE);
		} else {
		     specset=(typespecset)taskalloc(taskptr,sizeof(typevspec));
			if (!getvspec(taskptr,specset))
				return(FALSE);
		}
		*specsetptr = specset;
		specsetptr = (typespecset*)&((typeitemheader *)specset)->next;
	}
	return (TRUE);
}

  bool
getvspec (taskptr, vspecptr)
  typetask *taskptr;
  typevspec *vspecptr;
{
  bool getspanset();
/*fprintf (logfile, " vspec"); fprintf (taskptr->errp, "X getvspec\n"); */
	vspecptr->itemid = VSPECID;
	vspecptr->next = NULL;
	return (
	   gettumbler (taskptr, &vspecptr->docisa)
	&& getspanset (taskptr, &vspecptr->vspanset, VSPANID));
}

  bool
getspanset (taskptr, spansetptr, id)
  typetask *taskptr;
  typespanset *spansetptr;
  char id;
{
  typespanset spanset;
  INT num;
  bool getspan();
/*fprintf (logfile, " spanset"); fprintf (taskptr->errp, "X getspanset\n"); */
	*spansetptr = NULL;
	if (!getnumber (taskptr, &num))
		return (FALSE);
/* fprintf (taskptr->errp, "X nspans %d\n", num); fprintf (logfile, " {");*/
	while (num--) {
		spanset=(typespanset)taskalloc(taskptr,sizeof(typespan));
		if (!getspan (taskptr, spanset, id))
			return(FALSE);
		*spansetptr = spanset;
		spansetptr = &spanset->next;
	}
/*fprintf (logfile, " }"); */
	return (TRUE);
}


  bool
getspan (taskptr, spanptr, id)
  typetask *taskptr;
  typespan *spanptr;
  char id;
{
/*fprintf (logfile, " span"); fprintf (taskptr->errp, "X getspan\n"); */
	spanptr->itemid = id;
	spanptr->next = NULL;
	return (
	   gettumbler (taskptr, &spanptr->stream)
	&& gettumbler (taskptr, &spanptr->width));
}


  bool
getcutseq (taskptr, cutseqptr)
  typetask *taskptr;
  typecutseq *cutseqptr;
{
  INT ncuts, i;
  tumbler cutaddress;
/*fprintf (logfile, " cutseq"); */
	if (!(
	   getnumber (taskptr, &ncuts)
	&& (ncuts == 3 || ncuts == 4)))
		return (FALSE);
/*fprintf (logfile, " {"); */
	cutseqptr->numberofcuts = ncuts;
	for (i = 0; i < ncuts; ++i) {
		if(!gettumbler (taskptr, &cutseqptr->cutsarray[i]))
			return(FALSE);
	}
/*fprintf (logfile, " }"); */
	return(TRUE);
}


  bool
gettextset (taskptr, textsetptr)
  typetask *taskptr;
  typetextset *textsetptr;
{
  typetextset textset;
  INT num;
  bool gettext();
/*fprintf (logfile, " textset"); fprintf (taskptr->errp, "X gettextset\n"); */
	*textsetptr = NULL;
	if (!getnumber (taskptr, &num))
		return (FALSE);
/* fprintf (taskptr->errp, "X number of texts is %d\n", num); fprintf (logfile, " {"); */
	while (num--) {
		textset = (typetextset) taskalloc (taskptr, sizeof(typetext));
		if (!gettext (taskptr, textset))
			return (FALSE);
		*textsetptr = textset;
		textsetptr = &textset->next;
	}
/*fprintf (logfile, " }");*/
	return (TRUE);
}

  bool
gettext (taskptr, textptr)
  typetask *taskptr;
  typetext *textptr;
{
  INT i;
/* fprintf (taskptr->errp, "X gettext\n"); */
	if (!(
	   eatchar (taskptr, TEXTFLAG)
	&& getnumber (taskptr, &textptr->length))) {
fprintf(taskptr->errp, "\33[55;1fBackend receiving nothing in insert request.\n");
		return (FALSE);
	}
	textptr->itemid = TEXTID;
	textptr->next = NULL;

/*fprintf (logfile, " \"");*/
	for (i = 0; i < textptr->length; ++i) {
		textptr->string[i] = pullc (taskptr);
/*fprintf (logfile, "%c", textptr->string[i]);*/
	}
/*
if (i != -1 && i != 0) {
write (fileno(taskptr->errp), textptr->string, textptr->length);
fprintf(taskptr->errp, "FINIS\n");
}
else
fprintf (taskptr->errp, "read failed\n");
*/
/*	if (! (eatchar (taskptr, WORDELIM) || eatchar (taskptr, '\n')))
		return (FALSE);   REDUNDANT */
/*fprintf (logfile, "\""); */
	return (i != -1 && i != 0);
}

  bool
getrequest (taskptr, requestptr)
  typetask *taskptr;
  typerequest *requestptr;
{
  bool validrequest();
char c;
INT num;
bool flag;

/*fprintf (logfile, "\nrequest "); */
	taskptr->charinbuff = FALSE;
/*
	return (
	   getnumber (taskptr, requestptr)
	&& validrequest (taskptr, *requestptr));
*/
	num = 0;
	flag = FALSE;
	while (c = pullc (taskptr)){
		fprintf (logfile, "%c",c);
		if (!isdigit(c))
			break;
		num = num * 10 + c - '0';
		flag = TRUE;
	}
	*requestptr = num;
	return (flag && (c == WORDELIM || c == '\n') && validrequest (taskptr, *requestptr));
}

  bool
validrequest (taskptr, request)
  typetask *taskptr;
  typerequest request;
{
	if (
	   request >= 0
	&& request < NREQUESTS
	&& requestfns[request] != NULL)
		return(TRUE);
	  else {
		fprintf (taskptr->errp, "invalid request: %d\n", request);
		return (FALSE);
	}
}


  bool
validaccount (taskptr, accountptr)
  typetask *taskptr;
  typeisa *accountptr;
{
	return (TRUE);
}
