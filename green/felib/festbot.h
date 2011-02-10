/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:     festbot.h
***************************************************/


#define loop    for(;;)
#define elif    else if

#include "fetypealias.h"

#ifndef TRUE
#define TRUE    1
#endif
#ifndef FALSE 
#define FALSE   0
#endif

#define INTLEN 7
#define NAMELEN 103      /* size of filename */
#define BIGVIEW 2000000

#define PROTOLEN 2	/* max length of FeBe protocol id */

#define CURRENTMAXIMUMNUMBEROFNODES     5

/*
#define xconscell struct structxconscell

struct structxconscell {
        xconscell *xcar,*xcdr;
};
*/

#define BIT
#define NPLACES 12       /* places in mantissa of tumbler */
#define SMALLNPLACES 2

typedef unsigned long tdigit;
typedef struct structtumbler {
        bool sign  BIT ;  /* 1 if negative, otherwise 0 */
        med exp;
        tdigit/*unsigned long*/ mantissa[NPLACES];
} tumbler;

/* #define movetumbler(A,B) movmem(A,B,sizeof(tumbler))  */
#define movetumbler(A,B) *(B) = *(A)


typedef struct structsmalltumbler {
        bool sign  BIT ;  /* 1 if negative, otherwise 0 */
        med exp;
        /*tdigit mantissa[0];*/ /* synonym for smallmantissa*/
        tdigit/*unsigned long*/ smallmantissa[SMALLNPLACES];
} smalltumbler;

/* #define movesmalltumbler(A,B) movmem((A),(B),sizeof(smalltumbler)) */
#define movesmalltumbler(A,B) *(B) = *(A)

#define iszerotumbler(tumblerptr)  (!((tumblerptr) ->mantissa[0]))

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


typedef struct structtask {
        FILE *inp, *outp, *errp;
        char *tempspace;        /* to talloc out of */
        tumbler account;
        bool charinbuff;
        char charbuff;
        } tasktype;

#define MAXCUTS  4

/*keeps track of location of (user-inserted) delimiters in a document*/
typedef struct structcutseq {
        med numberofcuts;
        smalltumbler cutsarray[MAXCUTS];
        } cutseqtype;

/*objects described below are all on a virtual memory ring, each type is also on a ring with other objects of the same type */

typedef struct structcharspan {
        struct structcharspan *nextcharspan, *prevcharspan;
        med typeofthing;
        med age;
        char *nextvm, *prevvm;
        smalltumbler vstartaddress;  /*where span starts in document*/
        long numberofcharactersinspan;
        char *charinspanptr;
        } charspantype;
#define CHARSPANTYPE 1

typedef struct structdocument {
      /* pointers into document ring */
        struct structdocument *nextdocument, *prevdocument;
        med typeofthing;
        med age;
        char *nextvm, *prevvm;
        tumbler documentid;
        charspantype *doccharspanset; /*pointer to contents*/
	med textsize;
	bool istextsizevalid;
        } documenttype;
#define DOCUMENTTYPE 2


/*specifies positions in documents*/
/* and mode of display for contents of those places*/

typedef struct structspan {
        struct structspan *nextspan, *prevspan;
        med typeofthing;
        med age;
        char *nextvm, *prevvm;
        smalltumbler vstartaddress;
        long sizeofspan;
        /* med mask; */
        } spantype;
#define SPANTYPE 3

typedef struct structspec {
        struct structspec *nextspec, *prevspec;
        med typeofthing;
        med age;
        char *nextvm, *prevvm;
        tumbler docid;
        spantype *specspanptr;
        } spectype;
#define SPECTYPE 4

typedef struct structdocrange {
        struct structdocrange *nextdocrange, *prevdocrange;
        med typeofthing;
        med age;
        char *nextvm, *prevvm;
        tumbler docrangeorigin;
        tumbler docrangewidth;
        } docrangetype;
#define DOCRANGETYPE 5

typedef struct structlinkrestriction {
        struct structlinkrestriction *nextlinkrestriction, *prevlinkrestriction;
        med typeofthing;
        med age;
        char *nextvm, *prevvm;
        docrangetype *homeset;
        spectype *fromset;
        spectype *toset;
        spectype *threeset;
        } linkrestrictiontype;
#define LINKRESTRICTIONTYPE 6

typedef struct structtumbleritem {
        struct structtumbleritem *nexttumbleritem, *prevtumbleritem;
        med typeofthing;
        med age;
        char *nextvm, *prevvm;
        tumbler value;
        } tumbleritemtype;
#define TUMBLERITEMTYPE 7

typedef struct structlinkgroup {
        struct structlinkgroup *nextlinkgroup, *prevlinkgroup;
        med typeofthing;
        med age;
        char *nextvm, *prevvm;
        linkrestrictiontype linkrestriction;
        tumbleritemtype *linkset;
        } linkgrouptype;
#define LINKGROUPTYPE 8

typedef struct structlinkcontent {
        struct structlinkcontent *nextlinkcontent, *prevlinkcontent;
        med typeofthing;
        med age;
        char *nextvm, *prevvm;
        tumbler linkid;
        spectype *linkfromset;
        spectype *linktoset;
        spectype *linkthreeset;
        } linkcontenttype;
#define LINKCONTENTTYPE 9

typedef struct structcorrespondence {
        struct structcorrespondence *nextcorrespondence, *prevcorrespondence;
        med typeofthing;
        med age;
        char *nextvm, *prevvm;
        tumbler stream[2]; 
        /*tumbler stream1;
        tumbler stream2;*/
        tumbler widthofspan;
} correspondencetype;
#define CORRESPONDENCETYPE 10

typedef union unionvmthing {
        documenttype            vmdocument;
        charspantype            vmcharspan;
        spantype                vmspan;
        spectype                vmspec;
        docrangetype            vmdocrange;
        linkrestrictiontype     vmlinkrestriction;
        tumbleritemtype         vmtumbleritem;
        linkgrouptype           vmlinkgroup;
        linkcontenttype         vmlinkcontent;
        correspondencetype      vmcorrespondence;
} vmthingtype;



#define FROM	1		/* link endsets */
#define TO	2
#define THREE	3

#define NLINKTYPES	4	/* link types */

#define JUMPLINK	 0
#define QUOTELINK	 1
#define FOOTNOTELINK	 2
#define MARGINALNOTELINK 3

#define DEFAULTLINK   JUMPLINK
#define PLAINDOCUMENT JUMPLINK  /* similiar behaviour */


#define BERTMODEONLY   1
#define BERTMODECOPYIF 2
#define BERTMODECOPY   3

#define NOBERTREQUIRED 0
#define READBERT       1
#define WRITEBERT      2


#ifdef BCOPYWORKS
#define movmem(a,b,c) bcopy((char *)a,(char *)b,(int)c)
#endif
#ifdef AMIGA
/* #define movmem(a,b,c) memcpy((char *)(a),(char*)(b),(unsigned int)(c)) */
#endif
#ifdef MSDOS
#endif

typedef void  functionthatreturnsvoid();
typedef functionthatreturnsvoid (*ptftrv);

#ifdef MAINDECLARATION
spectype *tempfromset;
spectype *temptoset;
spectype *tempthreeset; /* this will get a link isa with width 1 */

cutseqtype cutsequence ;

#else

extern spectype *tempfromset;
extern spectype *temptoset;
extern spectype *tempthreeset;

extern cutseqtype cutsequence ;

#endif MAINDECLARATION

#define exitasgracefullyaspossible() fflush(stdout);fflush(stderr);abort();
