/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
#include "crum.h"
#include "coredisk.h"
/* crude random replacement hash cashe */
#define HASHSIZE 301
#define HASHMULT 754343 /* random prime */
typedef struct hashfoo{
    INT blocknumber;
    typeuberrawdiskloaf urdloaf;
} hashtable;
hashtable xhashtable[HASHSIZE];
inithash()
{
  INT i;
	for(i =0;i<HASHSIZE;i++){
		xhashtable[i].blocknumber = -1;
	}
}

void actuallyreadrawloaffromhash (loafptr, blocknumber)
  typeuberrawdiskloaf *loafptr;
  INT blocknumber;
{
  INT temp;
  void addtohash(), actuallyreadrawloaf();
/*fprintf(stderr,"actuallyreadrawloaffromhash %d \n",blocknumber);*/
    if(temp = blockisinhash(blocknumber)){
	*loafptr = xhashtable[temp].urdloaf;
	return;
    }
    actuallyreadrawloaf(loafptr,blocknumber);
    addtohash(loafptr,blocknumber);

}

void writethruhash(loafptr,blocknumber)
  typeuberrawdiskloaf *loafptr;
  INT blocknumber;
{
  INT temp;
  void addtohash();

    if(temp = blockisinhash(blocknumber)){
	xhashtable[temp].urdloaf = *loafptr;
    }else{
	addtohash(loafptr,blocknumber);
    }
}
hashcasheclash()
{
}
INT blockisinhash(blocknumber)
  INT blocknumber;
{
  INT temp;
      temp = hash(blocknumber);
      if(xhashtable[temp].blocknumber == blocknumber){
	  return temp;
      }else{
	  return 0;
      }
}
void addtohash(loafptr,blocknumber)
  typeuberrawdiskloaf *loafptr;
  INT blocknumber;
{
  INT temp;
      temp = hash(blocknumber);
	if(xhashtable[temp].blocknumber != -1){
		hashcasheclash();
	}
      xhashtable[temp].blocknumber = blocknumber;
      xhashtable[temp].urdloaf = *loafptr;
}

INT hash(blocknumber)
  INT blocknumber;
{
    return(abs(blocknumber*HASHMULT)%HASHSIZE);
}
