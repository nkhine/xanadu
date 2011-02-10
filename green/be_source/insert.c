/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License,
* which contains precisely the terms of the X11 License.  The full text of
* The Udanax Open-Source License can be found in the distribution in the file
* license.html.  If this file is absent, a copy can be found at
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* insert.d - enfilade crum insertion routines */
 
#include "crum.h"
#include "ndenf.h"
 

/* use with GRAN */

insertseq (fullcrumptr, address, info)
  typecuc *fullcrumptr;
  tumbler *address;
  typegranbottomcruminfo *info;
{
  typecrumcontext *context, *retrievecrums();
  typecorecrum  *new, *createcrum();
  typecbc *ptr;
  typedsp offset, reach;
  tumbler nextaddress;
  bool fillupcbcseq();
  bool splitsomething, splitcrumupwards();

	movetumbler (address, &nextaddress);
	context = retrievecrums (fullcrumptr, address, WIDTH);
	ptr = context->corecrum;
	clear (&offset, sizeof(offset));
	movewisp (&context->totaloffset, &offset);
	crumcontextfree (context);

	if (			     /* crum can be extended */
	   info->infotype == GRANTEXT
	&& ptr->cinfo.infotype == GRANTEXT
	&& ptr->cinfo.granstuff.textstuff.textlength < GRANTEXTLENGTH) {
		if (!fillupcbcseq (ptr, &nextaddress, info)) {
			ivemodified ((typecorecrum*)ptr);
			return;
		}
	}
	reserve ((typecorecrum*)ptr);
	new = createcrum (0,(INT)ptr->cenftype);
	reserve (new);
	adopt (new, RIGHTBRO, (typecorecrum*)ptr);
	ivemodified (new);
	splitsomething = splitcrumupwards (findfather (new));
	if (info->infotype == GRANORGL){
		info->granstuff.orglstuff.orglptr->leftbroorfather = new;
	}
	moveinfo (info, &((typecbc *)new)->cinfo);/*in GRAN */
	if (iszerotumbler (&ptr->cwid.dsas[WIDTH])) {  /* last crum in granf */
		tumblerclear (&new->cwid.dsas[WIDTH]);
		tumblersub (&nextaddress, &offset.dsas[WIDTH], &ptr->cwid.dsas[WIDTH]);
	} else {
		dspadd (&offset, &ptr->cwid, &reach, GRAN);
		tumblersub (&reach.dsas[WIDTH], &nextaddress, &new->cwid.dsas[WIDTH]);
		tumblersub (&nextaddress, &offset.dsas[WIDTH], &ptr->cwid.dsas[WIDTH]);
	}
	ivemodified ((typecorecrum*)ptr);
	setwispupwards (findfather ((typecorecrum*)ptr),0);
	setwispupwards (findfather ((typecorecrum*)new),1);
	splitsomething |= splitcrumupwards (findfather ((typecorecrum*)ptr));
	rejuvinate((typecorecrum*)ptr);
	rejuvinate(new);
	if(splitsomething){
		recombine (fullcrumptr);
	}
}
 static foocounter(){}
 static barcounter(){}
  
  bool
fillupcbcseq (ptr, crumboundary, info) 
  typecbc *ptr;
  tumbler *crumboundary;
  typegranbottomcruminfo *info;
{
  char temp[GRANTEXTLENGTH];
  INT  crumlength, remainingroom, textlength;

	/** first     copy the text from the info to the crum that ptr points to */
	/* do it nicer than this **/
	crumlength =  ptr->cinfo.granstuff.textstuff.textlength;
	remainingroom = GRANTEXTLENGTH - crumlength;
	textlength = info->granstuff.textstuff.textlength;

	if(remainingroom > textlength){	
		movmem(&info->granstuff.textstuff.textstring,(char*)(&(ptr->cinfo.granstuff.textstuff))+crumlength,textlength);
		ptr->cinfo.granstuff.textstuff.textlength =  crumlength + textlength  ;
		tumblerincrement (crumboundary, 0, textlength, crumboundary);
		return(FALSE);
	}else{
		movmem(&info->granstuff.textstuff.textstring,(char*)(&(ptr->cinfo.granstuff.textstuff))+crumlength,remainingroom);
		ptr->cinfo.granstuff.textstuff.textlength= GRANTEXTLENGTH  ;
		tumblerincrement (crumboundary, 0, remainingroom, crumboundary);
	}
	/* then take  whats left in info and renormalize it */
	info->granstuff.textstuff.textlength -= remainingroom;

	movmem (&info->granstuff.textstuff.textstring[remainingroom], temp, info->granstuff.textstuff.textlength);
	clear (info->granstuff.textstuff.textstring, GRANTEXTLENGTH);
	movmem (temp, info->granstuff.textstuff.textstring, info->granstuff.textstuff.textlength);
	return (TRUE);
}
