/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/***************************************************
  Copyright (c) 1987 Xanadu Operating Company
  XU.87.1 Frontend Source Code:        bert.d
***************************************************/

#include "fest.h"

typedef struct {
	long connection;
	tumbler documentid;
	char created,modified;
	long type;
	long count;
}bertentry;

typedef struct{
	char *next;
  char *stuff;
}conscell;
long hashoftumbler();
long primes[] = {3,7,11,17,37,41,59,71,97,103,113,131,151,137,277,421,433,567,643,743};
#define NUMBEROFBERTTABLE 1327

static conscell *berttable[NUMBEROFBERTTABLE];

  long
checkforopen(tp/*,type*/)
  tumbler *tp;
  /*int type;*/
{
  long hash;
  conscell *p;

	hash = hashoftumbler(tp);
	for(p = berttable[hash];p&&p->stuff;p=(conscell *)p->next){
		if(tumblereq(tp,&((bertentry *)(p->stuff))->documentid)){
			return ((bertentry *)(p->stuff))->type;
		}
	}
	return -1;
}

#ifdef COMMENT
incrementopen(tp)
  tumbler *tp;
{
  long hash;
  conscell *p;
	hash = hashoftumbler(tp);
	for(p = berttable[hash];p&&p->stuff;p=(conscell *)p->next){
		if(tumblereq(tp,&((bertentry *)(p->stuff))->documentid)){
			((bertentry *)(p->stuff))->count +=1;;
		}
	}
}
#endif COMMENT

addtoopen(tp,connection,created,type)
  tumbler *tp;
  long connection;
  long created;
  long type;
{
  long hash;
  conscell *consp;
  bertentry *ptr;

	hash = hashoftumbler(tp);
	ptr = (bertentry *)malloc(sizeof(bertentry));
	tumblercopy(tp,&ptr->documentid);
	ptr->connection = connection;
	ptr->count = 1;
	ptr->created = created;
	ptr->modified = NULL;
	ptr->type = type;
	consp = (conscell *)malloc(sizeof(conscell));
	consp->stuff = (char *)ptr;
	consp->next = (char *)berttable[hash];
	berttable[hash] = consp;
}

removefromopen(tp)
  tumbler *tp;
{
  long hash;
  conscell *p,*oldptr;
  conscell *temp;
  long status;
	hash = hashoftumbler(tp);
	oldptr = 0;
	for(p = berttable[hash];p&&p->stuff;p=(conscell *)p->next){
		if(tumblereq(tp,&((bertentry *)(p->stuff))->documentid)){
			status = ((bertentry*)(p->stuff))->modified&&((bertentry*)(p->stuff))->created ; 
			if(status){
				/*deleteversion(tp);*/
			}
			free(p->stuff);
			if(oldptr == 0){
				temp = berttable[hash];
				berttable[hash]=(conscell *)berttable[hash]->next;
				free((char *)temp);
				
				return;
			}else{
				temp = (conscell *)oldptr->next;
				oldptr->next = p->next;
				free((char *)temp);
				return;
			}
		}
		oldptr = p;
	}

}

/* probably the worlds most inefficient hashing function  I'll fix it if gprof says I should, and not before */
long hashoftumbler(tp)
  tumbler *tp;
{
  long ret;
  long i;
	ret = tp->exp;
	for(i=0;i<NPLACES;i++){
		ret += tp->mantissa[i]*primes[i];
	}
	return ret%NUMBEROFBERTTABLE;
}

