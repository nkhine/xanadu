/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* enf.h */
/* general enfilade header file - does not include common.h */

#define GRAN            1
#define POOM            2
#define SPAN            3

typedef INT typediskloafptrdigit; /*long  disk block number*/

typedef struct disktypeloafstruct{
	typediskloafptrdigit diskblocknumber;
	INT		insidediskblocknumber;
} typediskloafptr;

#define DISKPTRNULL -1  /* invalid disk pointer */

#include "wisp.h"
#define MAXUCINLOAF 6
#define MAXBCINLOAF    1        /* so text will fit *//* as you wish */
#define MAX2DBCINLOAF   4       /* for a start */

/* typecorecrumhedr */
struct  structcorecrumhedr {
        bool isapex BIT; /* TRUE if this is the fullcrum */
        SINT height;       /* 0 => this is bottom crum */
        SINT cenftype;    /* GRAN, SPAN, or POOM */
        bool modified BIT;
        bool isleftmost BIT; /* whether I'm leftmost sibling */
        struct structcorecrumhedr *nextcrum,
                     *prevcrum;
                        /* doubly linked circular list of */
                        /*   all corecrums for grim reaping */
        /*SINT*/unsigned char age;         /* for deciding to grimly reap */
        struct structcorecrumhedr *leftbroorfather;
                /* if it is NULL then this is fullcrum */
                /* father if leftmost, else leftbro */
        struct structcorecrumhedr *rightbro;
                /* if none, this == NULL */
        typewid cwid;
        typedsp cdsp;
} ;
typedef struct structcorecrumhedr typecorecrumhedr;
typedef typecorecrumhedr typecorecrum;

/* typecuc */   /* core upper crum */
struct structcuc {
/*        typecorecrumhedr        xcorecrumhedr;*/
        bool isapex BIT; /* TRUE if this is the fullcrum */
        SINT height;       /* 0 => this is bottom crum */
        SINT cenftype;    /* GRAN, SPAN, or POOM */
        bool modified BIT;
        bool isleftmost BIT; /* whether I'm leftmost sibling */
        typecorecrum *nextcrum,
                     *prevcrum;
                        /* doubly linked circular list of */
                        /*   all corecrums for grim reaping */
        /*SINT*/unsigned char age;         /* for deciding to grimly reap */
        typecorecrum *leftbroorfather;
                /* if it is NULL then this is fullcrum */
                /* father if leftmost, else leftbro */
        typecorecrum *rightbro;
                /* if none, this == NULL */
        typewid cwid;
        typedsp cdsp;

/*bottom of hedr*/
        typediskloafptr sonorigin;
                /* if son didn't come from disk, this should */
                /* be DISKPTRNULL */
        typecorecrum            *leftson;
                /* if no sons in core this == NULL */
        INT                      numberofsons;
} ;
typedef struct structcuc  typecuc;

/* typecbc */   /* core bottom crum */
struct structcbc {
/*        typecorecrumhedr        xcorecrumhedr;*/
        bool isapex BIT; /* TRUE if this is the fullcrum */
        SINT height;       /* 0 => this is bottom crum */
        SINT cenftype;    /* GRAN, SPAN, or POOM */
        bool modified BIT;
        bool isleftmost BIT; /* whether I'm leftmost sibling */
        typecorecrum *nextcrum,
                     *prevcrum;
                        /* doubly linked circular list of */
                        /*   all corecrums for grim reaping */
        /*SINT*/unsigned char age;         /* for deciding to grimly reap */
        typecorecrum *leftbroorfather;
                /* if it is NULL then this is fullcrum */
                /* father if leftmost, else leftbro */
        typecorecrum *rightbro;
                /* if none, this == NULL */
        typewid cwid;
        typedsp cdsp;
/*bottom of hedr*/
        typegranbottomcruminfo      cinfo;
};
typedef struct structcbc typecbc;
/* type2dcbc */   /* 2d core bottom crum */
struct struct2dcbc {
/*        typecorecrumhedr        xcorecrumhedr;*/
        bool isapex BIT; /* TRUE if this is the fullcrum */
        SINT height;       /* 0 => this is bottom crum */
        SINT cenftype;    /* GRAN, SPAN, or POOM */
        bool modified BIT;
        bool isleftmost BIT; /* whether I'm leftmost sibling */
        typecorecrum *nextcrum,
                     *prevcrum;
                        /* doubly linked circular list of */
                        /*   all corecrums for grim reaping */
        /*SINT*/unsigned char age;         /* for deciding to grimly reap */
        typecorecrum *leftbroorfather;
                /* if it is NULL then this is fullcrum */
                /* father if leftmost, else leftbro */
        typecorecrum *rightbro;
                /* if none, this == NULL */
        typewid cwid;
        typedsp cdsp;
/*bottom of hedr*/
        type2dbottomcruminfo      c2dinfo;
};
typedef struct struct2dcbc type2dcbc;
             

/* relatives  used as parameters to adopt*/
#define LEFTMOSTSON     0
#define RIGHTMOSTSON    1
#define SON             LEFTMOSTSON
#define LEFTBRO         2
#define RIGHTBRO        3



typedef struct structcontext {
        struct structcontext *nextcontext;
        INT contexttype;
        typedsp totaloffset;
        typewid contextwid;
        typebottomcruminfo contextinfo;
        struct structcontext *lastcontext;
} typecontext;

typedef struct struct2dcontext {
        /*typecontext*/struct struct2dcontext *nextcontext;
        INT contexttype;
        typedsp totaloffset;
        typewid contextwid;
        type2dbottomcruminfo context2dinfo;
} type2dcontext;

typedef struct structcrumcontext {
        struct structcrumcontext *nextcrumcontext;
        typecbc *corecrum;
        typedsp totaloffset;
} typecrumcontext;


/* genf.c */
extern bool          is2dcrum();
extern typecorecrum *getleftson();
extern typecorecrum *routinegetrightbro();
extern typecorecrum *getrightmostbro();
extern typecorecrum *getleftbro();
extern typecorecrum *getleftmostbro();
extern typecuc      *getfather();
extern typecuc      *findfullcrum();
#define isfullcrum(x) ((bool)((typecorecrum *)(x))->isapex)

extern bool          isemptyenfilade();
extern typecuc      *findfather();
extern typecorecrum *findleftbro();
extern typecorecrum *findleftmostbro();
extern typecorecrum *weakfindleftmostbro();
extern typecorecrum *funcfindrightbro();
extern typecorecrum *weakfindrightbro();
extern typecorecrum *findrightmostbro();
extern typecorecrum *findleftson();
extern typecorecrum *findrightmostson();
extern bool          toomanysons();
extern bool          roomformoresons();

/* credel.c */
/* These next two are macros right now.
	extern void          rejuvinate();
	extern void          rejuvinateifnotRESERVED();
*/
extern void          reserve();
extern void          testforreservedness();
extern void          subtreefree();
extern void          freecrum();
extern void          loaffree();
extern void          orglfree();
extern typecuc      *createenf();
extern typecorecrum *createcrum();
extern void          initcrum();

/*#include "protocrum.h"*/

/* test.c */
extern typecorecrum *sonoriginok();
