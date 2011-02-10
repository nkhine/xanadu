/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/

#include "xanadu.h"
#include "alloc.h"

lookatalloc2(abaseallocated)/*baseallocated is statics in alloc.d*/
  HEADER *abaseallocated;
{
#ifndef DISTRIBUTION
  register HEADER *p;
  unsigned allocated=0,unallocated=0,maxunallocatedblock=0;/*in header units*/
	p= abaseallocated->s.ptr;
	for (; p < abaseallocated->s.ptr+abaseallocated->s.size; ){
		if(!p->s.ptr){/* allocated*/
			analyzeanddebug(p);
			allocated += p->s.size;
		}else{  /*unallocated*/ 
			unallocated += p->s.size;
			maxunallocatedblock=max(p->s.size,maxunallocatedblock);
		}
		/* step past current one */
		p = p + p->s.size;    
		
	}		   
	fprintf(stderr," allocated = %d unallocated = %d maxunallocatedblock = %d\n",allocated*sizeof(HEADER),unallocated*sizeof(HEADER),maxunallocatedblock*sizeof(HEADER));
#endif
}

analyzeanddebug(ptr) /* ptr to thing with alloc header and tag header */
  char * ptr;
{
#ifndef DISTRIBUTION
  tagtype tag;
  char *tagptr;
  HEADER *allocptr;
	allocptr = (HEADER *)ptr;
	tagptr = ptr + sizeof(HEADER);
	tag = *tagptr;
	ptr = tagptr + sizeof(tagtype);
/*	fprintf(stderr,"\n size is %d ", ((HEADER *)allocptr)->s.size * sizeof(HEADER));*/
		      if(tag == TASKEDTAG){/* undifferientated task alloced stuff*/

	}/*else   if(tag & TASKEDTAG){ //tagged task alloced stuff//
		switch(tag &~TASKEDTAG){
		}
	}*/ else{  
		switch(tag){
		  case  INTTAG:				
			fprintf(stderr,"int  in allocspace \n");
			break;
		  case  ITEMTAG:
			fprintf(stderr,"item  in allocspace \n");
			dumpitem(ptr);
			break;
	
		  case  CONTEXTTAG:
			fprintf(stderr,"context %x in allocspace \n",ptr);
			dumpcontext(ptr);
			break;
		  case  CONTEXT2DTAG:
			fprintf(stderr,"context2d %x in allocspace \n", ptr);

			break;
		  case  CRUMCONTEXTTAG:
			fprintf(stderr,"crumcontext  in allocspace \n");

			break;
		  case  CUCTAG:
		  break;/*
			fprintf(stderr,"cuc  in allocspace ");
			dump(ptr);
			break;
			*/
		  case  CBCTAG:
		  break;/*
			fprintf(stderr,"cbc  in allocspace ");
			dump(ptr);
			break;
			*/
		  case  SPANTAG:
			fprintf(stderr,"span  in allocspace \n");
			dumpspan(ptr);
			break;
		  case  TUMBLERTAG:
			fprintf(stderr,"tumbler  in allocspace \n");
			dumptumbler(ptr);
			break;
			
		  case  ISPANTAG:
			fprintf(stderr,"aspan  in allocspace \n");

			break;
		  case  VSPANTAG:
			fprintf(stderr,"vspan  in allocspace \n");

			break;
		  case  SPORGLTAG:
			fprintf(stderr,"sporgl  in allocspace \n");

			break;
		  case  LINKTAG:
			fprintf(stderr,"link  in allocspace \n");

			break;
			
		  case  VSPECTAG:
			fprintf(stderr,"vspec  in allocspace \n");

			break;
		  case  FREEDISKLOAFTAG:
			fprintf(stderr,"freediskloaf  in allocspace \n");

			break;
		  default:
			fprintf(stderr,"randomunidentified thing  in allocspace \n");
			
			break;      
		}
	}
#endif
} 

