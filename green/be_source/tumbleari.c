/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
#include "common.h"
 unsigned INT calculatetotallength();
 unsigned INT functionintof();
  unsigned INT functionlengthof();
  humber humberput();
  humber fooalloc();
  humber vartumbleralloc();
  humber exponentof();
  humber mantissaof();

/* Various integers relating to humbers */
#define FIRSTCHAR  0x100
#define SECONDCHAR 0x10000
#define THIRDCHAR  0x1000000
#define FOURTHCHAR 0xffffffff

/* --------- Routines below set and change tumblers -------- */

unsigned INT lengthoflength();
#define mlengthoflength(x) (unsigned INT)((*(x))<128?1:lengthoflength(x))
#define mexponentof(x) (humber)((x)+mlengthoflength(x))

INT tumblerptrtofixed(p,tptr)
  humber p;
  tumbler *tptr;                                             
{
  INT i;
  INT temp,temp2;
  humber humberEnd;
  register humber mantissadigitptr;
  humber tempexp;
	tumblerclear(tptr);
	temp = lengthoftumbler (p);
	tempexp = mexponentof(p);
	if(*tempexp <= 127){
		tptr->exp = -*tempexp;
	}else{
		tptr->exp = -functionintof(mexponentof(p));
	}
	humberEnd = p + temp;
	p +=mlengthoflength(p);
	if(*(p)<128){ /* always the case */
		mantissadigitptr = p+1;
	}else{
		mantissadigitptr = p+functionlengthof(p);
	}
        for (i = 0;
	     i < NPLACES && mantissadigitptr < humberEnd; i++) {
	        if(mantissadigitptr[0] <=127){/* see intof*/
	              tptr->mantissa[i] = mantissadigitptr[0];
		    mantissadigitptr +=1   ;
	        }else{
                   tptr->mantissa[i] = functionintof(mantissadigitptr);
		   mantissadigitptr +=  functionlengthof(mantissadigitptr);
		}
        }
	return(temp);
}

INT tumblerfixedtoptr(ptr,p)
  tumbler * ptr;
  humber p; 
{                
  unsigned INT tumblerlength;
  unsigned INT lengthofexponent;
  unsigned INT i;
  unsigned INT digitlength;
  unsigned INT totallength;
  unsigned INT lengthlength;
  unsigned INT numberofsignificantdigits;
  humber op;
  unsigned INT intlengthoflength();
        if(ptr->sign)
                gerror("negative tumbler in tumblerfixedtoptr\n");
	op = p;
   	/*dumptumbler(ptr);*/
        tumblerlength = 0;
        humberput(-ptr->exp,p,&lengthofexponent);  
        p += lengthofexponent;
        tumblerlength +=lengthofexponent;
        numberofsignificantdigits = nstories(ptr);
        for(i=0 ; i<numberofsignificantdigits;i++){
                humberput((INT) ptr->mantissa[i], p, &digitlength);  
                p += digitlength;
                tumblerlength += digitlength;
        }                                    
        totallength = calculatetotallength(tumblerlength);
	lengthlength = intlengthoflength(totallength);
        movmem(op,op+ptr->sign+lengthlength,tumblerlength);
        humberput((INT) totallength, op + ptr->sign, &lengthlength);      
	return(totallength);
}

/*
humberfree(ptr)
  humber ptr;
{
        foofree(ptr);
}
*/
/*  humber
vartumbleralloc(size)
 unsigned INT size;
{
        return(fooalloc(size));
}
*/
  humber
fooalloc(size)
  unsigned INT size;
{
        return((humber)eallocwithtag((unsigned)size, HUMBERTAG));
}

foofree(ptr)
  humber ptr;
{
        efree((char *)ptr);
}
  
 unsigned INT
calculatetotallength(lengthofbody) /* of tumbler ie adds length of exponent in length of body*/
 unsigned INT lengthofbody;
{
        if(lengthofbody <127){
                return(lengthofbody + 1);
        }else if(lengthofbody < FIRSTCHAR - 1){
                return(lengthofbody + 2);
        }else if(lengthofbody < SECONDCHAR - 1/*?*/){
                return(lengthofbody + 3);
        }else if(lengthofbody < THIRDCHAR - 1){
                return(lengthofbody + 4);
        }else if (lengthofbody < FOURTHCHAR/* - 1*/){
                return(lengthofbody + 5);
        }else
                gerror("difficultly large length");return(333333);
}                        
  humber
humberput(i,humberfoo,lengthofhumberptr)
 /*unsigned*/ INT i;                   
  humber humberfoo; 
 unsigned INT *lengthofhumberptr;
{
	if((int)i == -1){
		gerror("humberput of -1\n");
	}
	if((int)i <0){
		fprintf(stderr,"humberput of %d\n",(int)i);
		gerror("humberput of negative number\n");
	}
        if(i<128){
                *lengthofhumberptr = 1;
                *humberfoo = i;
                
        } else /*if(i<256*256*256*256) impossible for a 32 bit int */{
/* NOTE zzz check to see that compiler does the 256*256 at compile time */
/*and fix these damn shifts sometime to be efficient*/
                if(i <FIRSTCHAR){
                        *lengthofhumberptr = 2;
                        *(humberfoo  ) = 128 + 2;
                        *(humberfoo + 1) = i ;
                }else if (i<SECONDCHAR){
                        *lengthofhumberptr = 3;
                        *(humberfoo  )   = 128 + 3;
                        *(humberfoo + 1) = i>>8 ;
                        *(humberfoo + 2) = i & 0xff;
                }else if (i<THIRDCHAR){
                        *lengthofhumberptr = 4;
                        *(humberfoo  )   = 128 + 4;
                        *(humberfoo + 1) = i>>16 ;
                        *(humberfoo + 2) = i>>8 & 0xff;
                        *(humberfoo + 3) = i & 0xff;
                } else {
                        *lengthofhumberptr = 5;
                        *(humberfoo  )   = 128 + 5;
                        *(humberfoo + 1) = i>>24 ;
                        *(humberfoo + 2) = i>>16 & 0xff;
                        *(humberfoo + 3) = i>>8 & 0xff;
                        *(humberfoo + 4) = i & 0xff;
                }
        } 
/*fprintf(stderr,"humberput puts %d\n",i);*/
        return(humberfoo);
}

  humber
humber3put(i,humberfoo,lengthofhumberptr)
 /*unsigned*/ INT i;                   
  humber humberfoo; 
 unsigned INT *lengthofhumberptr;
{
	if((int)i == -1){
		gerror("humber3put of -1\n");
	}
	if((int)i <0){
		fprintf(stderr,"humberput of %d\n",(int)i);
		gerror("humber3put of negative number\n");
	}
	 if (i<SECONDCHAR){
                        *lengthofhumberptr = 3;
                        *(humberfoo  )   = 128 +3;
                        *(humberfoo +1 ) = i>>8 ;
                        *(humberfoo + 2) = i & 0xff;
	}else{
		fprintf(stderr,"in humber3put i = %d\n",i);
		gerror("humber3put called with too larg a value\n");
	}
/*
                }else if (i<THIRDCHAR){
                        *lengthofhumberptr = 4;
                        *(humberfoo  )   = 128 +4;
                        *(humberfoo + 1) = i>>16 ;
                        *(humberfoo + 2) = i>>8 &0xff;
                        *(humberfoo + 3) = i & 0xff;
                }else {
                        *lengthofhumberptr = 5;
                        *(humberfoo  )   = 128 +5;
                        *(humberfoo + 1) = i>>24 ;
                        *(humberfoo + 2) = i>>16 &0xff;
                        *(humberfoo + 3) = i>>8 &0xff;
                        *(humberfoo + 4) = i & 0xff;
                }
        } 
*/
/*fprintf(stderr,"humberput puts %d\n",i);*/
        return(humberfoo);
}

 unsigned INT functionintof(h)
  humber h;
{
  INT k;
  INT i;
        if(((unsigned)h[0])<=127){
                return(h[0]);
        } else {
                k = h[0] - 128;
                switch (k) {
                  case 0: /* reserved for chge or wierd stuff */
                        fprintf(stderr,"case %d in intof first 6 bytes: ",k);
			for (i =0; i < 6; i++) fprintf(stderr,"%x ", h[i]);
                        gerror("weird length 0 in intof \n");
                        break;
                  case 2:
/*fprintf(stderr,"%x ",h[1]);*/
                        return (h[1]);
                  case 3:
/*fprintf(stderr,"%x %x ",h[1],h[2]);*/
                        return ((h[1] <<8) +h[2]);
                  case 4:
/*fprintf(stderr,"%x %x %x ",h[1],h[2],h[3]);*/
                        return ((((h[1] <<8)+h[2]) << 8) +h[3]);
                  case 5:
/*fprintf(stderr,"%x %x %x %x ",h[1],h[2],h[3],h[4]);*/
                        k= (((((h[1] <<8)+h[2]) << 8) +h[3])<<8) +h[4];
			if (k == -1) {
				gerror("-1 in intof \n");
			}
			return(k);
                  default:
                        fprintf(stderr,"case %d in intof close by bytes: ",k);
			for (i =-8;i < 0; i++) fprintf(stderr,"%x ", h[i]);
			fprintf(stderr,": ");
			for (i =0; i < 6; i++) fprintf(stderr,"%x ", h[i]);
			fprintf(stderr,"\n");
			for (i =-8;i < 0; i++) fprintf(stderr,"%d ", h[i]);
			fprintf(stderr,": ");
			for (i =0; i < 6; i++) fprintf(stderr,"%d ", h[i]);
			fprintf(stderr,"\n");
                        gerror("\nin intof default case\n");return(33333);
                }
        }
        return 0 ;/* for lint */
}

  unsigned INT
intlengthoflength(i)
  unsigned INT i;
{
        if (i<127) {
		return(1);
	} else if (i<FIRSTCHAR) {
		return(2);
	}else if (i<SECONDCHAR) {
		return (3);
	} else if (i <THIRDCHAR) {
		return(4);
	} else if (i <FOURTHCHAR) {
		return(5);
	}
	gerror("impossible  in intlengthof\n");
	return(0);
} 

unsigned INT
lengthoflength(ptr)
  humber ptr;
{/* YUCK */int i;
	if (*ptr < 128)
		return 1;
        switch (*ptr){
          case 128:     /* break to ultraINT and specialcases*/
                gerror("ultraINTs not defined yet");
                return(12134);
          case 129:
                return(1+lengthoflength(ptr+1));
          case 130:
                return(3);
          case 131:
                return(4);
          case 132:
                return(5);
          case 133:
                return(6); 
          default:
                fprintf(stderr,"case %d in lengthoflength close by bytes:\n\t",ptr[-1]);
		for (i =-6;i < 0; i++) fprintf(stderr,"%x ", ptr[i-1]);
		fprintf(stderr,": ");
		for (i =0; i < 6; i++) fprintf(stderr,"%x ", ptr[i-1]);
		fprintf(stderr,"\n");
		gerror("Bad lengthoflength\n");
                return(1);
        }
} 

  humber
exponentof(ptr)
  humber /*vartumbler */ ptr;
{
        return(ptr + mlengthoflength(ptr));
}
 
 unsigned INT
lengthofexp(ptr)
  tumbler * ptr;
{
        return(lengthof(mexponentof(ptr->xvartumbler))); /* ZZZ ECH 8-26-88 */
}
 
 unsigned INT
functionlengthof(ptr)   /* length of humber or vartumbler */
  humber ptr; 
{
  int i;
/*typedef union{char charlongchar[sizeof(INT)];INT charlonglong;} typecharlong;

  typecharlong temp;               
        temp.charlonglong = 0;
*/
	if (*ptr < 128)
		return 1;
        switch(*ptr++){
          case 128:     /* break to ultralong and specialcases*/
                gerror("ultralongs not defined yet");
                return(12134);

          case 129: /* negative */
                return( 1+lengthof(ptr+1));/* is this right if we use 1 in the length of length*/
                
          case 133:
		return(5);
          case 132:
		return(4);
          case 131:
		return(3);
          case 130:
		return(2);
        
          default:
                fprintf(stderr,"case %d in lengthof close by bytes:\n\t",ptr[-1]);
		for (i =-6;i < 0; i++) fprintf(stderr,"%x ", ptr[i-1]);
		fprintf(stderr,": ");
		for (i =0; i < 6; i++) fprintf(stderr,"%x ", ptr[i-1]);
		fprintf(stderr,"\n");
		gerror("Bad length in functionlengthof\n");
                return(1);
        }
}
  humber
mantissaof(ptr)  /* returns a ptr to the first humber in the mantissa of the vartumbler of the tumbler*/
  humber ptr;
{
humber temp;
/* this is correct but expensive
        temp =ptr+lengthoflength(ptr)+lengthofexp(ptr);
*/
	ptr = ptr+mlengthoflength(ptr);/*thus ptr points at expenent*/
	if(*ptr <128){
	   temp = ptr +1;
	 }else{
             temp =ptr+lengthof(ptr); /* and add the length of the exponent */
	}
        return(temp);
}       
  
