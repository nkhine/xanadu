/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/


/* common xanadu header file */
/*#include <assert.h>*/
#include <stdio.h>
#include <ctype.h>


#include "tags.h"
#include "defs.h"
#include "reap.h"
#ifndef max
#define max(a,b)        (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)        (((a) < (b)) ? (a) : (b))
#endif


#define XUBUFSIZ 1024

#ifdef sun
#define BCOPYWORKS 1
#define SUN 1
#else 
#define SUN 0
#define BCOPYWORKS 0
#endif
typedef unsigned  bool;
#define TRUE    1
#define FALSE   0
#define ERROR 	-1
#define stdprint 2
#define INTLEN 7
#define NAMELEN 103      /* size of filename */
#define metachar INT

#define BIT
#define ESC     '\033'  /* Standard ASCII 'escape' character */
#define OK 0            /* General purpose "no error" return value */
#define MAXLINE 400     /* Longest line of input expected from the console */
#define NPLACES 11       /* places in mantissa of tumbler */
#define RECURSIVE
typedef unsigned char * humber;
#define TUMBLERMINUS 129
typedef unsigned INT tdigit;
/* tumbler */
typedef struct structtumbler {
	humber xvartumbler;
	char varandnotfixed;
        char sign  BIT ;  /* 1 if negative, otherwise 0 */
        short exp;
        tdigit mantissa[NPLACES];
} tumbler;

#define ZEROTUMBLER  {0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0} /* 0.0.0.0.0.0 */
                                                                 
extern tumbler ZEROTUMBLERvar; 		/* EXTERNAL BEWARE */

#define tumblerclear(tumblerptr) ((*(tumblerptr)) = ZEROTUMBLERvar)
#define iszerotumbler(tumblerptr) (!((tumblerptr) -> mantissa[0]))
#define movetumbler(A,B) (*((tumbler *)(B)) = *((tumbler *)(A)))
/*#define movetumbler(A,B) tumblercopy((A),(B)) */

/* relationship between tumblers */
/* "<" and ">" type comparisons are allowed, so don't change */

#define GREATER 1
#define EQUAL   0
#define LESS   -1

/* relationship between tumbler interval and address */
/* "<" and ">" type comparisons are allowed, so don't change */

#define TOMYLEFT -2
#define ONMYLEFTBORDER -1
#define THRUME 0
#define ONMYRIGHTBORDER 1
#define TOMYRIGHT 2

#define typetthingheader struct structtthingheader
struct structtthingheader {
        typetthingheader *tlast;
        typetthingheader *tnext;
};

typedef struct structtask {
        FILE *inp, *outp, *errp;
        typetthingheader *tempspacehead; /* doubly linked talloc space so */
        typetthingheader *tempspacetail; /* that single items can be freed */
        tumbler account;
        bool charinbuff;
        char charbuff;
}typetask;


#define MAXCUTS  4

typedef struct structcutseq {
        INT numberofcuts;
        tumbler cutsarray[MAXCUTS];
}typecutseq;

#define GRANTEXTLENGTH  950 

INT debug;       

#define gerror(s) qerror(s)

typedef unsigned tagtype;   /* for tagging allocated things -- see credel.d*/  

long ntaskorcommand;


#define rejuvinateifnotRESERVED(x) (((x)->age==RESERVED)?(int)(x):((x)->age = NEW))

#define rejuvinate(x) ((x)->age==RESERVED)?(reservnumber?--reservnumber:testforrejuvinate(x)):0,(x)->age = NEW
/*not needed #define rejuvinate(x) funcrejuvinate(x)*/




#define getrightbro(x) routinegetrightbro(x) 


#define macrogetrightbro(x)    ((rejuvinateifnotRESERVED (x), (x) = (x)->rightbro,  ((x)? (rejuvinateifnotRESERVED (x)): 0),(x)))

/*not needed#define macrogetrightbro(x) getrightbro()*/



#define macrotumblermax(a,b,c) ((*(tumbler*)(c))=(tumblercmp((a),(b))==GREATER)?( (*(tumbler *)(a))) :((*(tumbler *)(b))) )
#define macrotumblermin(a,b,c) ((*(tumbler*)(c))=(tumblercmp((a),(b))==LESS)?( (*(tumbler *)(a))) :((*(tumbler *)(b))) )

/* not needed
#define macrotumblermax(a,b,c) tumblermax(a,b,c)
#define macrotumblermin(a,b,c) tumblermin(a,b,c)
*/



/*#define findrightbro(A) macrogetrightbro(A)*/
#define findrightbro(A) funcfindrightbro(A)


extern INT reservnumber;
#define lengthoftumbler(x) intof((x))
#define SIZEOFZEROHUMBER 3
extern char *malloc();

#define clear(ptr,count) memset(ptr,0,count)
#define movmem(src,dest,count) memmove(dest,src,count)

#define NOBERTREQUIRED 0
#define READBERT 1
#define WRITEBERT 2

#define BERTMODEONLY   1
#define BERTMODECOPYIF 2
#define BERTMODECOPY   3

/* credel.c */
extern void          etag();
extern INT          *eallocwithtag();
extern INT          *ealloc();
extern void          efree();
extern void          efreewithtag();

extern humber humberput();
extern humber humber3put();

/* tumble.c */
extern bool tumblereq();
extern bool tumbleraccounteq();
extern INT tumblercmp();
extern INT intervalcmp();
extern bool is1story();
extern bool tumblercheckptr();
#define intof(x) ((unsigned INT)((unsigned)((humber)(x))[0]<=127)?((humber)(x))[0]:functionintof(x))
 
#define lengthof(x) ((unsigned INT)((unsigned)((humber)(x))[0]<=127)?(unsigned INT)1:functionlengthof(x))
/*
#define lengthof(x) functionlengthof(x)*/
#define weakfindfather(x) ((typecuc *)(((!(((typecuc*)(x))->isapex))&&(((typecuc*)(x))->isleftmost))?((typecuc*)((typecuc*)(x))->leftbroorfather):(typecuc*)functionweakfindfather((typecuc*)(x))))
/*#define tumbleradd(x,y,z) if(iszerotumbler(y)){movetumbler(x,z);}else{functiontumbleradd(x,y,z);}*/
#define tumbleradd(x,y,z) functiontumbleradd(x,y,z)
