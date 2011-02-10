/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:    parseesc.d
***************************************************/

/* we may want to remove a lot of this "0x7f &" cruft... --MMFB 8/87 */

#include "fest.h"
#include "feminreq.h"
#include "fekeys.h"

#define ESC 0x1b
#define CSI 0x9b  /* ANSI single-char equiv to <ESC>[ */
#define MASK7BIT 0x7f
#define MASK8BIT 0xff

#define controlsequenceintroducerparseescape \
        escapesquarebracketparseescape

  metachar
functionofchartypeahead(iptr,nchar)
  metachar *iptr,nchar;
{
  metachar mc,escapesquarebracketparseescape(),escapeOparseesc(),nonescapeparseescape();
  metachar getchartypeahead();
  metachar controlsequenceintroducerparseescape();

/*fprintf(stderr,"nchars: %d\n",nchar);
 fprintf(stderr,"typeahead char    :%x\n",chartypeahead[*iptr]);
*/
        mc = MASK8BIT & chartypeahead[*iptr];
/*fprintf(stderr,"resulting mc char :%x\n",mc);
*/
        (*iptr)++;
        switch(mc){
		case CSI:
			mc = MASK7BIT & (char)getchartypeahead(iptr,nchar);
/*fprintf(stderr,"char after CSI:%x\n",mc);
*/
			return(controlsequenceintroducerparseescape(mc));
			break;
		case ESC:
                	mc = MASK7BIT & (char)getchartypeahead(iptr,nchar);
                	switch(mc){
                        	case '[':
                                    mc = MASK7BIT & (char)getchartypeahead(iptr,nchar);
                                    return(escapesquarebracketparseescape(mc));
                                    break;
                        	case 'O':
                                    mc = MASK7BIT & (char)getchartypeahead(iptr,nchar);
                                    return(escapeOparseesc(mc)); 
                                    break;
                        	case TABDOWN:
                                    return(CTABDOWN);
                                    break;
                        	default :
                                    return(IGNOREMETACHAR);
                                    break;/*zzzz*/
		}
		default :
			return(nonescapeparseescape(mc));
			break;
	}
        
        gerror("impossible");
        return(0); /* for lint*/
}

  metachar
nonescapeparseescape(c)
  metachar c;
{
#ifdef MSDOS
	if(SPECIALHIGHBYTE == (c & 0xff00)){

		c &= 0xff;
		switch(c){


		case 59: /*F1*/
			return(CUT);
		case 60: /*F2*/
			return(MLINK);
		case 61: /*F3*/
			return(COMPARECHAR);
		case 62: /*F4*/
			return(NEWDOC);
		case 63: /*F5*/
		       return(ESC2);/*returnfromstack*/
		case 64: /*F6*/
			return(NEWDOC);
		case 65: /*F7*/
			return(DELCUTS);
		case 66: /*F8*/
			return(COPY);
		case 67: /*F9*/
			return(RRANGE);
		case 68: /*F10*/
			return(DELBLK);




		case 84:/*shift F1*/
			return(CHTO);
		case 85:/*shft F2*/
			return(EXITNORMALLYCHAR);
		case 86:/*shft F3*/
			
		case 87:/*shft F4*/
			return(BECOMECRUFTYCHAR);
		case 88:/*shft F5*/
			return(CREATEVERSIONCHAR);
		case 89:/*shft F6*/
	      
		case 90:/*shft F7*/
		case 91:/*shft F8*/

		case 92:/*shft F9*/

		case 93:/*shft F10*/



	
		case 94:/*ctrl F1*/
		case 95:/*ctrl F2*/
		case 96:/*ctrl F3*/
		case 97:/*ctrl F4*/
		case 98:/*ctrl F5*/
		case 99:/*ctrl F6*/
		case 100:/*ctrl F7*/
		case 101:/*ctrl F8*/
		case 102:/*ctrl F9*/
		case 103:/*ctrl F10*/



		case 104:/*alt F1*/
		case 105:/*alt F2*/
		case 106:/*alt F3*/
		case 107:/*alt F4*/
		case 108:/*alt F5*/
		case 109:/*alt F6*/
		case 110:/*alt F7*/
		case 111:/*alt F8*/
		case 112:/*alt F9*/
		case 113:/*alt F10*/

		case 120:/*alt 1*/
		case 121:/*alt 2*/
		case 122:/*alt 3*/
		case 123:/*alt 4*/
		case 124:/*alt 5*/
		case 125:/*alt 6*/
		case 126:/*alt 7*/
		case 127:/*alt 8*/
		case 128:/*alt 9*/
		case 129:/*alt 0*/
	
		case 130:/*alt -Hyphen*/
		case 131:/*alt =*/



		case 16:/*alt Q*/
		case 17:/*alt W*/
		case 18:/*alt E*/
		case 19:/*alt R*/
		case 20:/*alt T*/
		case 21:/*alt Y*/
		case 22:/*alt U*/
		case 23:/*alt I*/
		case 24:/*alt O*/
		case 25:/*alt P*/


		case 30:/*alt A*/
		case 31:/*alt S*/
		case 32:/*alt D*/
		case 33:/*alt F*/
		case 34:/*alt g*/
		case 35:/*alt H*/
		case 36:/*alt J*/
		case 37:/*alt K*/
		case 38:/*alt L*/



		case 44:/*alt Z*/
		case 45:/*alt X*/
		case 46:/*alt C*/
		case 47:/*alt V*/
		case 48:/*alt B*/
		case 49:/*alt N*/
		case 50:/*alt M*/
			return(IGNOREMETACHAR);
		case 15:/*shift tab*/
			return(RTAB);
			
		case 71:/*home*/
			return(FLINK);
		case 72:/*up arrow*/
			return(CUP);
		case 73:/*pgup*/
			return(PREVPAGE);
		case 75:/*left arrow*/
			return(CLEFT);
		case 77:/*right arrow*/
			return(CRIGHT);
		case 79:/*end*/
			return(IGNOREMETACHAR);
		case 80:/*down arrow*/
			return(CDOWN);
		case 81:/*pgdn*/
			return(NEXTPAGE);       
		case 82:/*insert*/
			return(IGNOREMETACHAR);
		case 83:/*delete*/
			return(DEL);		
		case 114:/*echo  (ctrl-prt-sc*/


		case 115:/*ctrl left arrow*/

		case 116:/*ctrl right arrow*/
		case 117: /*ctrl-end*/
		case 118: /*ctrl-pgdn*/
		case 119: /*ctrl-home*/


		case 132: /*ctrl-pgup*/









			return(IGNOREMETACHAR);




		case 3:/*would be null character i.e. what null is map to DEL */
			return(DEL);
		      default:
		        gerror("impossible ibm function key\n");
		}
	}

#endif
	if (c == (metachar) ('^' - '@'))
		return (CREATEFOOTNOTECHAR);
	if (c == (metachar) ('_' - '@')){
		return (DEL);
	}
	if (c == (metachar) (']' - '@'))
		return (NEWDOC);
	if ( c == CR)
		return(NL);
	return(c);

}

 metachar
getchartypeahead(iptr,nchar)
 metachar *iptr,nchar;
{
  char temp = 0;
#ifdef AMIGA
  extern struct FileHandle *Open();
  extern struct FileHandle *Awindow_fh;
#endif
        if (*iptr < nchar){
                temp = chartypeahead[*iptr];
                *iptr += 1;
                return(temp);
        }else{
                fflush(stdin);
#ifdef UNIX
                if(!read(0,&temp,1)){
			curlupanddye();
		}
#endif
#ifdef AMIGA

putf("\33[0 p"); /*turn off cursor */ /* UGLY KLUDGE WARNING -- MMFB */

		if(!Read(Awindow_fh,&temp,1)){
			curlupanddye();
		}  /* "blocking" read can't tell slow from fast input.
			Is this a bug or a feature?  --MMFB 8/87       */

putf("\33[ p"); /* turn on cursor */ /* UGLY KLUDGE WARNING -- MMFB */

/* purpose of above ugly kludge pair is to turn cursor on IFF we're waiting
   for input in the Read()... will go away when we use Text() -- MMFB */
/* reversed what's supposed to work??!? */
#endif

                /*fflush(stdin);
                temp = getchar();*/
                return(temp);   /* get the suckers one at a time*/
        }
}     
  metachar
escapeOparseesc(c)
  metachar c;
{

      switch(c){
	case 'A': /*PF1*/
		return(CUT);
	case 'B': /*PF2*/
		return(MLINK);
	case 'C': /*PF3*/
		return(COMPARECHAR);
	case 'D': /*PF4*/
		return(NEWDOC);
	case 'E': /*PF5*/
	       return(ESC2);/*returnfromstack*/
	case 'F': /*PF6*/
		return(NEWDOC);
	case 'G': /*PF7*/
		return(DELCUTS);
	case 'H': /*PF8*/
		return(COPY);
	case 'I': /*PF9*/
		return(RRANGE);
	case 'J': /*PF10*/
		return(DELBLK);
	case 'K': /*PF11*/
		return(FLINK);
	case 'L': /*PF12*/
		return(CHFR);
	case 'M':/*shift PF1*/
		return(CHTO);
	case 'N':/*shft PF2*/
		return(EXITNORMALLYCHAR);
	case 'O':/*shft PF3*/
		
	case 'P':/*shft PF4*/
		return(BECOMECRUFTYCHAR);
	case 'Q':/*shft PF5*/
		return(CREATEVERSIONCHAR);
	case 'R':/*shft PF6*/
      
	case 'S':/*shft PF7*/
	case 'T':/*shft PF8*/

	case 'U':/*shft PF9*/

	case 'V':/*shft PF10*/

	case 'W':/*shft PF11*/

	case 'X':/*shft PF12*/
		return(COMPARECHAR);
      } 
	return(IGNOREMETACHAR);
}

  metachar
escapesquarebracketparseescape (c)   
  metachar c;
{
        switch (c) {
          case ESCCUP:
                return(CUP);
                break;
          case ESCCDOWN:
                return(CDOWN);
                break;
          case ESCRIGHT:
                return(CRIGHT);
                break;
          case ESCLEFT:
                return(CLEFT);
                break;
          case ESCHOME:
                return(FLINK/*HOMEUP*/);
                break;
          case TABUP:
                return(CTABUP);
                break;

          default:
                fprintf (stderr, "eh? %c   0x%x\n",c,c);  /* zzz */
        return(IGNOREMETACHAR);
                break;
        }
        gerror("impossible");
        return(0); /* for lint*/
}
