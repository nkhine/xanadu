/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* xanadu.h - xanadu top header file */

#include "common.h"   

#define typegranf INT *   /* temp -- INT for alignment */
extern typegranf granf;       
#define typespanf INT *   /* temp -- INT for alignment */
extern typespanf spanf;        
#define typeorgl  INT *   /* temp -- INT for alignment */

#define typerequest INT
#define NREQUESTS 40    /* number of requests */
extern void (*requestfns[NREQUESTS])(); 

typedef tumbler typeisa;

#define typeitemid char
#define TEXTID 0
#define ISPANID 1
#define VSPANID 2
#define VSPECID 3
#define NODEID 4
#define ADDRESSID 5
#define SPORGLID 6
#define LINKID ADDRESSID

/* spanfilade span types */
#define LINKFROMSPAN    1
#define LINKTOSPAN      2
#define LINKTHREESPAN   3
#define DOCISPAN        4

typedef struct structspanpair {
        struct structspanpair *nextspanpair;
        tumbler stream1;
        tumbler stream2;
        tumbler widthofspan;
} typespanpair;

typedef typespanpair * typespanpairset;
/*
*/

typedef struct structitemheader{
        struct structitemheader *next;
        typeitemid      itemid;
} typeitemheader;
 
typedef struct structtypetext{
        struct structtypetext *next;
        typeitemid      itemid;
        INT length;
        char string[GRANTEXTLENGTH];
} typetext;
typedef typetext * typetextset;
 
typedef struct structtypespan{
        struct structtypespan *next;
        typeitemid      itemid;
        tumbler stream;
        tumbler width;
} typespan;
typedef typespan * typespanset;

typedef typespan typevspan;
typedef typevspan * typevspanset;
typedef typespan typeispan;
typedef typeispan * typeispanset;
 
typedef struct structtypevspec{
        struct structtypevspec *next;
        typeitemid      itemid;
        typeisa docisa;
        typevspanset vspanset;
} typevspec;
 
typedef union {
        typevspec xxxvspec;
        typeispan xxxispan;
} typespec;
typedef typespec * typespecset;
 
typedef struct structtypeboolsetnode{
        struct structtypeboolsetnode *next;
        typeitemid      itemid;
        typespanset val;
} typeboolsetnode;
/*typedef typeboolsetnode * typeboolset;*/
 
typedef struct structtypeaddress{
        struct structtypeaddress *next;
        typeitemid      itemid;
        typeisa address;
} typeaddress;

/*#define linkaddress address*/
typedef typeaddress * typeaddressset;
typedef typeaddress typelink;
typedef typelink * typelinkset;

typedef union {
        typetext xxxtext;
        typelink xxxlink;
} typevstuff;
typedef typevstuff * typevstuffset;

typedef struct structsporgl{
	struct structsporgl *next;
	typeitemid	itemid;
        tumbler sporglorigin;
        tumbler sporglwidth;
        typeisa sporgladdress;
} typesporgl;

typedef union {
        typeispan xxxxispan;
        typesporgl xxxxsporgl;
} typesporglitem;
typedef typesporglitem * typesporglset;

typedef union {
        typetext        xxtext;
        typespan        xxspan;
        typevspec       xxvspec;
        typeboolsetnode xxboolsetnode;
        typeaddress     xxaddress;
        typesporgl      xxsporgl;
} typeitem;
/*typedef typeitemheader typeitem;*/
typedef typeitem * typeitemset;

#define NODE      1
#define ACCOUNT   2
#define DOCUMENT  3
#define ATOM      4
                      /* atom types - don't change these */
#define TEXTATOM  1
#define LINKATOM  2

typedef struct {
        INT supertype;
        INT subtype;
        INT atomtype;
        typeisa hintisa;
} typehint;

/*
#include "protonocrum.h"
*/
