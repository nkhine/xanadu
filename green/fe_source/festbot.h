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

#include "defs.h"
#undef bool
#include <curses.h>

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

/*#define xconscell struct structxconscell

struct structxconscell {
        xconscell *xcar,*xcdr;
};
*/

#define BIT
/*******   Some console (video) terminal characteristics:   *******/
#define SCREENWIDTH  200     /* number of columns */
#define MAXSCREENHEIGHT 200
/*#define SUNSCREENHEIGHT 34*/

#define NPLACES 12       /* places in mantissa of tumbler */
#define SMALLNPLACES 2

typedef unsigned long tdigit;
typedef struct structtumbler {
        bool sign  BIT ;  /* 1 if negative, otherwise 0 */
        med exp;
        tdigit/*unsigned long*/ mantissa[NPLACES];
} tumbler;

/*#define movetumbler(A,B) movmem(A,B,sizeof(tumbler))  */

#define movetumbler(A,B) *(B) = *(A)
#define movesmalltumbler(A,B) *(B) = *(A)



typedef struct structsmalltumbler {
        bool sign  BIT ;  /* 1 if negative, otherwise 0 */
        med exp;
        /*tdigit mantissa[0];*/ /* synonym for smallmantissa*/
        tdigit/*unsigned long*/ smallmantissa[SMALLNPLACES];
} smalltumbler;
/*#define movesmalltumbler(A,B) movmem((A),(B),sizeof(smalltumbler))
  */
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
        med mask;
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

typedef spectype maskspectype;

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

/* linearization of all we wish displayed */
typedef struct structview{
        long characterdisplacementwithinview;
        spectype *viewspecptr;
        maskspectype *viewmaskspecptr;
        maskspectype *viewfromset;
        maskspectype *viewtoset;
        maskspectype *viewthreeset;
        maskspectype *viewcutset;
        } viewtype;

/* windows map 1-d view into 2-d screen */
typedef struct structwindowdef{
        struct structwindowdef *nextwindow,*prevwindow;
        struct structwindowdef *superordinatewindow, *subordinatewindow;
        med typeofwindow; /* whether document, footnote, marginal, etc. */
        med windowwidth;
        med windowheight;
        bool isendsetokflag; /*tells whether we know how to display endsets*/
        bool windowline_glorpfed[MAXSCREENHEIGHT+1];
        bool ispartofparalleltextface;
        bool dependentscreenp;
        bool newlycreateddocumentp;
        bool cursorisonscreen;
        med   lm,tm;    /* window boundaries */
        med   rcursl,rcursc;  /* cursor position relative to tm,lm */
                                /*? cursor vsa here?*/

        long windowleftmargins[MAXSCREENHEIGHT+1];  /* displacement into view*/

        /*tells whether line needs to be redisplayed*/
        viewtype windowview;
        struct structwindowdef *correspondingwindow;
        correspondencetype *correspondencelist;
	WINDOW *w;					 /* curses */
	WINDOW *border;

        } windowtype;

typedef struct structscreen{
        windowtype basewindow;
        }screentype;

#define DEFAULTPAD  ' ' /*0x7f*/
#define DEFAULTWRAP FALSE
#define DEFAULTCEOL FALSE

#define TABSIZE 8




#define NTYPEAHEAD 200 /* can change this */





#define CURRENTMAXIMUMNUMBEROFNODES     5

/*carries the interpretation of most user requests to the processes that implement them*/
typedef struct requeststruct{
        med requestnumber;
        tumbler requestvposition;
        long requestnumberoflines;
        long requestcursordeltax,requestcursordeltay;
        spectype *initialcursorpositionspecptr;
        long requestnumberofcharacters;
        char requeststring[NTYPEAHEAD];
        }requesttype;


typedef struct maskstruct{
        long/*long or int?*/ numberwiththisattribute;
        med displayattribute;
        }masktype;


/* endsets of link to be made */

#define JUMPLINK 0
#define QUOTELINK 1
#define FOOTNOTELINK 2
#define MARGINALNOTELINK 3

#define PLAINDOCUMENT JUMPLINK  /* similiar behaviour */



#define NOCHANGE        0
#define NORMALDISPLAY   0
#define QUARTERDISPLAY  1
#define SPLITVERTICAL   2
#define SPLITHORIZONTAL 3
#define FULLDISPLAY     4
#define RANDOMDISPLAY   5
#define EXPERIMENTALDISPLAY 6
#define ILLEGALDISPLAY   7                     

#define FOOTNOTEDISPLAY 1001
#define MARGINALNOTEDISPLAY 1002


 
#define FROM 1
#define TO   2

#define NORMALINPUTSTATE 0
#define ESCAPEINPUTSTATE 1
#define ESCAPESQUAREBRACKETINPUTSTATE 2
#define ESCAPE2INPUTSTATE 3
#define INSERTDELETECHARINPUTSTATE 4
#define ESCAPEOINPUTSTATE 5

#define OUTPUTLEN 1000 /* fiz this somethime zzz */


#define FIRSTMENUCHOICE0      4000
#define DISPLAYOPTIONSCHOICE0 4100
#define DOCUMENTMENUCHOICE0   4200
#define LINKMENUCHOICE0       4300
#define LINKTYPEMENUCHOICE0   4400
#define JUMPMENUCHOICE0       4500
#define STACKRETURNCHOICE0    4600
#define WEIRDOPTIONCHOICE0    5000
#define DOCUMENTSELECTBASE 5100
#define LINKSELECTBASE 5200

#define SWITCHWINDOW 5300

typedef void  functionthatreturnsvoid();
typedef functionthatreturnsvoid (*ptftrv);
/*nat (*documentmenuarray[20])();*/
/*nat (*linkmenuarray[20])();*/

 
#define FIRSTMENUSTATE 0
#define WEIRDMENUSTATE 1
#define CONSTRUCTEDMENUSTATE 2
/*#define LINKTYPEMENUSTATE    3*/
#ifdef GRAPHICS
#ifdef SUNGRAPHICS
struct menu *menu_ptr[10];
#endif
#endif

#define iszerotumbler(tumblerptr)  (!((tumblerptr) ->mantissa[0]))

#define BERTMODEONLY   1
#define BERTMODECOPYIF 2
#define BERTMODECOPY   3

#define NOBERTREQUIRED 0
#define READBERT 1
#define WRITEBERT 2

#ifdef BCOPYWORKS
#define movmem(a,b,c) bcopy((char *)a,(char *)b,(int)c)
#endif
#ifdef AMIGA
/*#define movmem(a,b,c) memcpy((char *)(a),(char*)(b),(unsigned int)(c))*/
#endif
#ifdef MSDOS
#endif
extern med screenwidth;
extern med screenheight;
extern screentype theoneandonlyscreen;
extern windowtype *currentwindow;
#ifdef MSDOS
extern metachar     chartypeahead[NTYPEAHEAD];
#else
extern char     chartypeahead[NTYPEAHEAD];
#endif
extern metachar fetypeahead [NTYPEAHEAD];
extern metachar *firsttypeahead,*nexttypeahead;
extern spectype *tempfromset;
extern spectype *temptoset;
extern spectype *tempthreeset;     /* this will get a link isa with width 1 */
extern med testmargins ;
extern cutseqtype cutsequence ;
extern med displaystyle;
extern tumbler staticlinkhomeposition ;/*= {0,-2,  2,0,0,0,0,0,0,0};*/ /* 0.0.2.0 */
extern char longblankline[SCREENWIDTH+1];
extern med inputstate;
extern med disableinsert;
extern nat (*switchwindow[10])();
extern ptftrv weirdoptionsarray[15];
extern nat (*firstmenuarray[10])();
extern nat (*displayoptionsarray[10])();
extern nat displaystylearray[10];

#ifdef MAINDECLARATION

med screenwidth;
med screenheight;
screentype theoneandonlyscreen;
windowtype *currentwindow;
#ifdef MSDOS
metachar     chartypeahead[NTYPEAHEAD];
#else
char     chartypeahead[NTYPEAHEAD];
#endif
metachar fetypeahead [NTYPEAHEAD];
metachar *firsttypeahead,*nexttypeahead;
spectype *tempfromset;
spectype *temptoset;
spectype *tempthreeset;     /* this will get a link isa with width 1 */
med testmargins ;
cutseqtype cutsequence ;
med displaystyle;
tumbler staticlinkhomeposition ;/*= {0,-2,  2,0,0,0,0,0,0,0};*/ /* 0.0.2.0 */
char longblankline[SCREENWIDTH+1];
med inputstate;
med disableinsert;
nat (*switchwindow[10])();
ptftrv weirdoptionsarray[15];
nat (*firstmenuarray[10])();
nat (*displayoptionsarray[10])();
nat displaystylearray[10];
#endif
