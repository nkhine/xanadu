/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:     sendbot.d
***************************************************/

#include "fest.h"
#include "requests.h"

#define WORDELIM '~'
#define TUMDELIM '.'
#define SPANFLAG 's'
#define VSPECFLAG 'v'
#define TEXTFLAG 't'

#define OLDPROTO

  void
feputc (c, fd)
  char c;
  FILE *fd;
{
#ifdef MSDOS
	outp_char(c);
#endif
#ifdef UNIX
        putc (c, fd);
/*
 fprintf(stderr,">%c",c);
*/
#endif
#ifdef AMIGA
        xfeputc(c);
#endif
}

  void
feputstring (string, fd)
  char *string;
  FILE *fd;
{
        while (*string)
                feputc (*string++, fd);
}

  void
sendrequestbuffer (taskptr)
  tasktype *taskptr;
{

        fflush (taskptr->outp);
}

#ifdef COMMENT
  void
prompt (taskptr, string)
  tasktype *taskptr;
  char *string;
{
        feputstring (string, taskptr->errp);
}
#endif COMMENT

  void
error (taskptr, string)
  tasktype *taskptr;
  char *string;
{
        feputstring (string, taskptr->errp);
}

  void
sendnum (outfile, num)
  FILE *outfile;
  tdigit  num;
{
  char digits[32];
  med i;

        if (!num)
                feputc ('0', outfile);
        else {
                for (i = 0; num; i++) {
                        digits[i] = (char) (num % 10 +  '0');
                        num /= 10;
                }
                while (i--)
                        feputc (digits[i], outfile);
        }
}

  void
sendnumber (outfile, num)
  FILE *outfile;
  tdigit num;
{
        sendnum (outfile, num);
        feputc (WORDELIM, outfile);
}

  void
sendtumbler (outfile, tumblerptr)
  FILE *outfile;
  tumbler *tumblerptr;
{
        dosendtumbler (outfile, tumblerptr, NPLACES);
}

  void
sendsmalltumbler (outfile, smalltumblerptr)
  FILE *outfile;
  smalltumbler *smalltumblerptr;
{
        dosendtumbler (outfile, (tumbler *)smalltumblerptr, SMALLNPLACES);
}

  void
dosendtumbler (outfile, tumblerptr, size)
  FILE *outfile;
  tumbler *tumblerptr;
  med size;
{
  med i;

        sendnum (outfile, (tdigit)-tumblerptr->exp);
        do {
                --size;
        } while (size > 0 && tumblerptr->mantissa[size] == 0);

        for (i = 0; i <= size; ++i) {
                feputc (TUMDELIM, outfile);


                sendnum (outfile,(tdigit) tumblerptr->mantissa[i]);
        }
        feputc (WORDELIM, outfile);

}

  void
sendtextset (outfile, textset)
  FILE *outfile;
  charspantype *textset;
{
  charspantype *temp;
  med i;

        if (textset == NULL) {
                sendnumber (outfile, (tdigit)0);
        }
        for (temp = textset, i = 0; temp; temp = temp->nextcharspan, ++i);
        sendnumber (outfile, (tdigit)i);
        for (; textset; textset = textset->nextcharspan) {
                sendtext (outfile, textset);
        }
}

  void
sendtext (outfile, textptr)
  FILE *outfile;
  charspantype *textptr;
{
  char *ptr;
  med i;

        feputc (TEXTFLAG, outfile);
        sendnumber (outfile,(tdigit) textptr->numberofcharactersinspan);
        ptr = textptr->charinspanptr;
/*           write (fileno (outfile), textptr->charinspanptr, textptr->numberofcharactersinspan);
*/
        for (i = 0; i < textptr->numberofcharactersinspan; ++i) {
                feputc (*(ptr++), outfile);
        }

#ifdef OLDPROTO
   /* why putc and not feputc? zzz */     feputc (WORDELIM, outfile);
#endif
}

  void
sendspecset (outfile, specset)
  FILE *outfile;
  spectype *specset;
{
  spectype *temp;
  med i;
       if (specset == NULL) {
                sendnumber (outfile, (tdigit)0);
                return;
        }
        for (temp = specset, i = 0; temp; temp = temp->nextspec, ++i);
        sendnumber (outfile, (tdigit)i);
        for (; specset; specset = specset->nextspec) {
                sendspec (outfile, specset);
        }
}

  void
sendspec (outfile, specptr)
  FILE *outfile;
  spectype *specptr;
{
        feputc (VSPECFLAG, outfile);
        feputc (WORDELIM, outfile);
        sendtumbler (outfile, &(specptr->docid));
        sendspanset (outfile, specptr->specspanptr);
}

  void
sendspanset (outfile, spanset)
  FILE *outfile;
  spantype *spanset;
{
  spantype *temp;
  med i;
        if (spanset == NULL) {
                return;
        }
       for (temp = spanset, i = 0; temp; temp = temp->nextspan, ++i);
       sendnumber (outfile, (tdigit)i);
       for (; spanset; spanset = spanset->nextspan) {
                sendspan (outfile, spanset);
        }
}

  void
sendspan (outfile, spanptr)
  FILE *outfile;
  spantype *spanptr;
{
  smalltumbler width;
        sendsmalltumbler (outfile, &(spanptr->vstartaddress));
        memclear ((char *)&width, sizeof(smalltumbler));
	if (spanptr->sizeofspan == BIGVIEW) {
		width.exp = 0;
        	width.smallmantissa[0] = 1;
	} else {
       		 width.exp = -1; 
	        width.smallmantissa[0] = spanptr->sizeofspan;
	}
        sendsmalltumbler (outfile, &width);
}



  void
sendlinkrestriction (outfile, linkrestrictionptr)
  FILE *outfile;
  linkrestrictiontype *linkrestrictionptr;
{
	sendspecset (outfile, linkrestrictionptr->fromset);
	sendspecset (outfile, linkrestrictionptr->toset);
	sendspecset (outfile, linkrestrictionptr->threeset);	/* MSM 6/26 */
	senddocrangeset (outfile, linkrestrictionptr->homeset);
}

#ifdef COMMENT
  void
sendnorestriction (outfile, ishomeset)
  FILE *outfile;
  bool ishomeset;
{
  smalltumbler stumbler;

        sendnumber (outfile, (tdigit)1);
        if (!ishomeset) {
                feputc (SPANFLAG, outfile);
                feputc (WORDELIM, outfile);
        }
        memclear ((char *)&stumbler, sizeof(smalltumbler));
        sendsmalltumbler (outfile, &stumbler);
        stumbler.smallmantissa[0] = CURRENTMAXIMUMNUMBEROFNODES;
        sendsmalltumbler (outfile, &stumbler);
}
#endif COMMENT

   void
senddocrangeset (outfile, docrangeset)
  FILE *outfile;
  docrangetype *docrangeset;
{
  docrangetype *temp;
  med i;

        if (docrangeset == NULL) {
                sendnumber (outfile, (tdigit)0);
                return;
        }
        for (temp = docrangeset, i = 0; temp; temp = temp->nextdocrange, ++i);
        sendnumber (outfile, (tdigit)i);
        for (; docrangeset; docrangeset = docrangeset->nextdocrange) {
                senddocrange (outfile, docrangeset);
        }
}

  void
senddocrange (outfile, docrangeptr)
  FILE *outfile;
  docrangetype *docrangeptr;
{
        sendtumbler (outfile, &docrangeptr->docrangeorigin);
        sendtumbler (outfile, &docrangeptr->docrangewidth);
}

  void
sendprotocol(taskptr,proto)
  tasktype *taskptr;
  char *proto;
{
  register med i;
  med n;
	feputc('\n',taskptr->outp);
	feputc('P',taskptr->outp);
	for (n=strlen(proto),i=0; i<n; i++) {
		feputc(proto[i],taskptr->outp);
	}
	feputc(WORDELIM,taskptr->outp);
	sendrequestbuffer(taskptr);
}
