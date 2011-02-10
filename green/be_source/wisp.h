/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* wid and dsp header file */

/* wid and dsp index for gr */
#define WIDTH  0

/* wid and dsp indexes for sp */
#define ORGLRANGE 0
#define SPANRANGE 1

/* wid and dsp indexes for pm */
#define I  0
#define V  1

#define DSPSIZEGR 1
#define WIDSIZEGR 1
#define DSPSIZESP 2
#define WIDSIZESP 2
#define DSPSIZEPM 2
#define WIDSIZEPM 2

/*
**        Note that dsp's of gr are not stored on crums
**          themselves, but are discovered thru tree walking
**
**                    wid             dsp
**         -----------------------------------------
**        |
**   gr:  |           WIDTH           WIDTH
**        |
**   sp:  |         ORGLRANGE         ORGLRANGE
**        |           SPANRANGE         SPANRANGE
**        |
**   pm:  |             I                     I
**        |               V                     V
**        |
*/



#define MAXLOXIZE 2    

struct structwid {
        tumbler dsas[MAXLOXIZE];
};
typedef struct structwid typewid;
typedef typewid typedsp;
typedef typewid typewisp; 

                        /*  wid & dsp */
#define movewisp(A,B) movmem((A),(B),sizeof(typewisp))
/*#define movewisp(A,B)  ((typewisp)(*(B))) = ((typewisp)(*(A)))*/  
#define widsize(A) ((A)==GRAN?WIDSIZEGR:((A)==SPAN?WIDSIZESP:WIDSIZEPM))
#define dspsize(A) ((A)==GRAN?DSPSIZEGR:((A)==SPAN?DSPSIZESP:DSPSIZEPM))



/*#define moveinfo(A,B) ((typebottomcruminfo)(*(B))) = ((typebottomcruminfo)(*(A)))*/ 
/*#define move2dinfo(A,B) ((type2dbottomcruminfo)(*(B))) = ((type2dbottomcruminfo)(*(A)))*/

#define GRANNULL	0
#define GRANTEXT        1
#define GRANORGL        2
#define GRANCLEARLYILLEGALINFO 42


/* typegrantext */
typedef struct structgrantext {
        char textstring[GRANTEXTLENGTH];
        unsigned textlength;
} typegrantext;


struct structorgl {
        struct structcuc *orglptr;
        typediskloafptr diskorglptr;
} ;


/* typegranorgl */
typedef struct structgranorgl {
        struct structcuc *orglptr;
        typediskloafptr diskorglptr;
        bool orglincore;
} typegranorgl;

/* typegranstuff */
typedef union uniongranstuff {
        typegrantext textstuff;
        typegranorgl orglstuff;
}typegranstuff  ;

/* typegranbottomcruminfo */
typedef struct structgranbottomcruminfo {
        typegranstuff granstuff;
        INT infotype;
} typegranbottomcruminfo;

/* type2dbottomcruminfo */
typedef struct struct2dbottomcruminfo {
        tumbler homedoc;
}type2dbottomcruminfo ;
#define move2dinfo(A,B) movmem((A),(B),sizeof(type2dbottomcruminfo))

/* typebottomcruminfo */
typedef union unionbottomcruminfo {
        typegranbottomcruminfo granbottomcruminfo;
        type2dbottomcruminfo x2dbottomcruminfo;
} typebottomcruminfo;
#define moveinfo(A,B) movmem((A),(B),sizeof(typebottomcruminfo))

extern bool iszerolock();
extern bool setwisp();
extern bool lockis1story();
extern bool lockeq();
