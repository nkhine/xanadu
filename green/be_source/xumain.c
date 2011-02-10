/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* Udanax main program */
/* stand alone version */
 
#include <errno.h>
#include "xanadu.h"
#include "requests.h"

#include "players.h"
#define MAX_PLAYERS 5
int user = 0;
PLAYER player[MAX_PLAYERS];

extern INT errno;
FILE *febelog = NULL;
bool isxumain = TRUE;
bool maximumsetupsizehasbeenhit;
INT maximumsetupsize;
bool logstuff;
typetask *taskptrx;
FILE *interfaceinput = NULL;

main ()  /* inside temporary */
{
  typetask task;
  FILE *fd;
  void diskexit();
  INT i=0;

        setbuf(stderr,NULL);
        debug = FALSE;
	processrcfile();
        init (0);
        inittask (&task);
/*
        if (fd = fopen ("xusetup", "r"))
                task.inp = fd;
        errno = 0;
*/
        initmagicktricks ();
	taskptrx = &task;
	getaccount(&task,&task.account);
        for (;;) {
ntaskorcommand++;
/*
fprintf(stderr,"%d ",i++);
*/
                xanadu (&task);
                testforreservedness ("main");
/*
                if (maximumsetupsizehasbeenhit) {
                        fprintf (stderr,"Setup has reached size limit.\n");
                        diskexit ();
                }
*/
        }
}

/* "xanadu" is the backend request dispatcher */
xanadu(taskptr)
  typetask *taskptr;
{
  typerequest request;

	logstuff = FALSE;
        if (getrequest (taskptr, &request))
                (*requestfns[request]) (taskptr);
/*
          else
                taskptr->inp = stdin;
*/
	logstuff = FALSE;
        tfree (taskptr);
}

  bool
setmaximumsetupsize (taskptr)
  typetask *taskptr;
{
  char buff[100];

        fprintf (taskptr->outp, "maximumsetupsize = ? ");
        maximumsetupsize = atoi (fgets (buff, 100, taskptr->inp));
        return (TRUE);
}

  bool
sourceunixcommand (taskptr)
  typetask *taskptr;
{
  char unixcommand[132];
  char file[64];
  typeisa docisa;
  typetext *textsetptr;
  /*char *ealloc();*/
  FILE *infile, *fopen();
  INT count, lines, bugger;
  INT status;
  bool getmuchtext();
  INT *taskalloc();

        count = 0;
        lines = 0;
        bugger = debug;

        sprintf (file, "xum%d", getpid());
        prompt (taskptr, " Enter unix command : ");
        fgets (unixcommand, 132, taskptr->inp);
        if (unixcommand[strlen (unixcommand) -1] == '\n')
                unixcommand[strlen(unixcommand) -1] = '\0';
        strcat (unixcommand, " >");
        strcat (unixcommand, file);
        if (status = system (unixcommand)) {
                fprintf (stderr, "Exit status = %d\n", status);
                perror ("Udanax(system call 0)");
        /*        return (FALSE);       */
        }
        if (debug) {
                prompt (taskptr, "lines until debug : ");
                count = atoi (fgets (unixcommand, 132, taskptr->inp));
                if (count)
                        debug = 0;
        }

        docreatenewdocument (taskptr, &docisa);
        /*testforreservedness ("eatunixcommand createdoc");*/
        putcreatenewdocument (taskptr, &docisa);

        infile = taskptr->inp;
        if (!(taskptr->inp = fopen (file, "r"))) {
                perror ("xanadu");
                fprintf (stderr, "Couldn't open %s\n", file);
                gerror ("Awful badness in sourceunixfile.\n");
        }
        while (
           (textsetptr = (typetext *)taskalloc(taskptr,sizeof(typetext)))
        && (getmuchtext (taskptr, textsetptr) || (textsetptr->length > 0))) {
if (debug)fprintf (stderr,"line # %d\n", lines);
                textsetptr->next = NULL;
                textsetptr->itemid = TEXTID;
                textsetptr->length = strlen (textsetptr->string);
                doappend (taskptr, &docisa, textsetptr);
                /*testforreservedness ("eatunixcommand loop");*/
                ++lines;
                tfree (taskptr);
                if (count && --count == 0)
                        debug = bugger;
        }
        fclose (taskptr->inp);
        unlink (file);
        taskptr->inp = infile;
        return (TRUE);
}

  bool
getmuchtext (taskptr, textptr)   /*
                                        this code stolen from get2.d which could use some improvement
                                        but be careful
                                */
  typetask *taskptr;
  typetext *textptr;
{           
  INT numinstring = 0; 
  INT temp;
        
        numinstring = temp = 0;          
        for(;;){
                if (!fgets (textptr->string+numinstring, GRANTEXTLENGTH - numinstring, taskptr->inp)) {
                        temp = 0;
                        break;
                }
                temp = strlen (textptr->string+numinstring);
                if (temp < 1){ 
                        break;
                }   
                numinstring += temp;
                if(numinstring >=GRANTEXTLENGTH){
                        break;
                }
        } 
        if(numinstring > GRANTEXTLENGTH){
                gerror(" got too much in getmuchtext");
        }
        if(numinstring == 0){                 
                textptr->length = 0;
                return(FALSE);
        }          
        textptr->length = numinstring;
        textptr->itemid = TEXTID;
        return(TRUE);
}

  bool
decrementusers()
{
  return(FALSE);
}


isthisusersdocument(tp)
  tumbler *tp;
{
	return tumbleraccounteq(tp,&taskptrx->account);
}
