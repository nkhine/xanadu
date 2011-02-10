/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* core-disk header file */

/* typediskloafhedr */
typedef struct  structdiskloafhedr {
	INT sizeofthisloaf;
        SINT/*bool*/ isapex;    /* TRUE if this is top of orgl */
        SINT height;      /* if == 0, then dbcloaf */
        SINT denftype;    /* GRAN, SPAN, POOM */
        SINT numberofcrums;
        SINT refcount;      /* for subtree sharing, */
                /* disk garbage collecting */
	SINT allignmentdummy;
} typediskloafhedr;

/* typeduc */   /* disk upper crum */
 typedef struct  structduc {
        typewid dwid;
        typedsp ddsp;
        typediskloafptr sonloafptr;
} typeduc;

/* typedbc */        /* disk bottom crum = loaf */
typedef struct structdbc {
     /**/   typediskloafhedr        xdbchedr;/**/
        typewid                 dbcwid;
        typedsp                 dbcdsp;
        typebottomcruminfo      dinfo;
} typedbc ;
#define typedbcloaf             struct  structdbc

/* type2ddbc */        /* disk bottom crum = loaf */
typedef struct struct2ddbc {
    /* */   typediskloafhedr        xdbchedr;/**/
        typewid                 dbcwid;
        typedsp                 dbcdsp;
        type2dbottomcruminfo    d2dinfo;
} type2ddbc;

/* type2ddbcloaf */        /* disk upper loaf */
typedef struct  struct2ddbcloaf {
        typediskloafhedr        x2ddbcloafhedr;
        type2ddbc               d2dbcarray[MAX2DBCINLOAF];
} type2ddbcloaf;

/* typeducloaf */        /* disk upper loaf */
typedef struct  structducloaf {
        typediskloafhedr        xducloafhedr;
        typeduc                 ducarray[MAXUCINLOAF];
} typeducloaf;

/* typediskloaf */
typedef union uniondiskloaf {
        typeducloaf     xducloaf;
        typedbcloaf     xdbcloaf;
        type2ddbcloaf   x2ddbcloaf;
} typediskloaf;   

typedef struct structuberdiskloaf{
	INT versiondisknumber;
	unsigned short numberofunterloafs;
	SINT anoherallignmentdummy;
	typediskloaf fakepartialuberloaf;
} typeuberdiskloaf;

typedef struct structuberdiskloaffake{
	INT versiondisknumber;
	unsigned short numberofunterloafs;
	SINT yetanotherallignmentdummy;
	INT fakepartialuberloaf;
} typeuberdiskloaffake;

/* typediskcrum */
typedef union uniondiskcrum {
        typeduc xduc;
        typedbc xdbc;
        type2ddbc x2ddbc;
        typeducloaf     xducloaf;
        typedbcloaf     xdbcloaf;
        type2ddbcloaf   x2ddbcloaf;
} typediskcrum ;

/* typefreediskloaf inverse crum for disk freeing */
typedef struct structfreediskloaf {
        struct structfreediskloaf *nextloaftofree;
        struct structfreediskloaf *sonloaftofree;
	typediskloafptr freeloafptr;
} typefreediskloaf;


#define NUMDISKBLOCKSINLOAF 1/*((sizeof(typeuberdiskloaf) +XUBUFSIZ -1)/XUBUFSIZ) */

#define NUMBYTESINLOAF ((NUMDISKBLOCKSINLOAF)*XUBUFSIZ)
typedef union unionuberrawdiskloaf{
	typeuberdiskloaffake xuberdiskloaf;
	char	     rawdiskloaf[NUMBYTESINLOAF];
} typeuberrawdiskloaf;

#define MAXTHINGIESINLOAF   9/*((1024-sizeof(struct structdiskloafhedr))/sizeof(struct structduc)) // change these numbers //*/

#define NUMLOAFSOFBITMAP 50
#define BITMAPSIZE (NUMBYTESINLOAF*NUMLOAFSOFBITMAP-(sizeof(unsigned)+sizeof(bool)))

struct {
        unsigned filesize;              /* number of last allocated block */
        char bitmap[BITMAPSIZE];  /* bit set for each free block */
        bool hasenftops;                /* GRAN & SPAN tops are in file */
} diskheader;                           /* header for file */

#define NUMDISKLOAFSINHEADER (sizeof(diskheader)/NUMBYTESINLOAF+1)

#define GRANFDISKLOCATION (NUMDISKLOAFSINHEADER+1)
#define SPANFDISKLOCATION (NUMDISKLOAFSINHEADER+2)
#define PARTIALFREEDISKLOCATION (NUMDISKLOAFSINHEADER+3)

typedef struct{
	INT partialdiskblocknumber;
	unsigned short freespaceinloaf;
	SINT moreallignmentdummy;
} freediskentry;

#define NFREEENTRYS ((NUMBYTESINLOAF - sizeof(freediskentry))/(sizeof(freediskentry)))
typedef struct{
	INT nextdiskblocknumber;
	INT numberofentrysinthisblock;
	freediskentry freeentryarray[NFREEENTRYS];
} freediskarray;

int varcrums;

#define hgetfromloaf(ip,lp) (*(ip)=intof((humber)lp),(lp)=((char*)lp)+lengthof((humber)lp))


#define SIZEOFUBERDISKHEADER 6

extern unsigned INT intlengthoflength();

extern void writeloaf();
extern void readloaf();
