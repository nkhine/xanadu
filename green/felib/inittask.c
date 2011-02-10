/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:    inittask.d
***************************************************/

#include <signal.h>
#include "fest.h"
#include "feminreq.h"

/*
extern bool issfe;
*/

extern FILE *fromxu, *toxu;

  void
inittask (taskptr, infile, outfile)
  tasktype *taskptr;
  FILE *infile, *outfile;
{
        taskptr->inp = infile;
        taskptr->outp = outfile;
        taskptr->errp = stderr;
        taskptr->tempspace = NULL;
        taskptr->charinbuff = FALSE;
}

  void
pushc (taskptr, c)
  tasktype *taskptr;
  char c;
{

        if (taskptr->charinbuff)
                error (taskptr, "charbuff occupied\n");
        else {
                taskptr->charinbuff = TRUE;
                taskptr->charbuff = c;
        }
}
 nat 
fegetc(stream)
  FILE *stream;
{
  nat c;

#ifdef MSDOS
	while(!inp_cnt()) ;
	c = inp_char();  /kluge input from console/
#endif
#ifdef UNIX
        c = getc(stream);
#endif
#ifdef AMIGA
        c = xfegetc();
#endif

	if(c == EOF) {
		gerror("EOF in fegetc");
	}
	c = c & 0x7f;
        return(c);
}

  char
pullc (taskptr)
  tasktype *taskptr;
{
  nat k;
  char c;
  nat e;
        if (taskptr->charinbuff) {
                taskptr->charinbuff = FALSE;
                return (taskptr->charbuff);
        } else {
                for(;;){
                        k = fegetc (taskptr->inp);
                        if(e=ferror(taskptr->inp)){
                                fprintf(stderr,"error = %d from ferror in pullc \n",e);
                                gerror("error in pullc");
                        }
                        if(k != -1){
                                c = (char)(k & 0x0000007f);
                                return (c&0x7F);
                        } else {
                                fprintf(stderr,"EOF in pullc taskptr->inp = %d\n",taskptr->inp);
				gerror("EOF in pullc");
                        }
                }
        }
}     

  void
pullblock(taskptr,buf,n)
  tasktype *taskptr;
  char buf[];
  med n;
{
  char c;
  med i;
  nat k;
        if (taskptr->charinbuff) {
                taskptr->charinbuff = FALSE;
                buf[0] =taskptr->charbuff;
                pullblock(taskptr, &buf[1], n-1);
        } else {
                for(i=0;i<n;){
                        k = fegetc (taskptr->inp);
                        if(ferror(taskptr->inp)){
                               fprintf(stderr,"error from ferror in pullblock \n");
                                gerror("error in pullblock");
                        }
                        if(k != -1) {
                                c = (char)(k & 0x0000007f);
                        } else {
                                fprintf(stderr,"EOF in pullblock taskptr->inp = %d\n",taskptr->inp);
                        }
                        buf[i++] = c;
                }
        }
}
      
#ifndef UNIX
wait()
{
	exit(0);
}
#endif
