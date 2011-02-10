/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/

#include "crum.h"
#include "ndenf.h"
 static INT widdiffs();
/* use with SPAN and POOM */

insertnd (taskptr, fullcrumptr, origin, width, infoptr, index)
  typetask *taskptr;
  typecuc *fullcrumptr;
  typewid *origin;  /* origin is vsa fo crum*//* note that here they're wids,*/
  typewid *width;       /* and in deletend they're single tumblers */
  type2dbottomcruminfo *infoptr;
  INT index;
{
  INT bothertorecombine;
  INT oldheight;
  INT olddiff,newdiff;
  tumbler oldwid;
#ifdef UndEfInEd
foo("insertnd\n");		
if((ntaskorcommand%100) == 0){  /* do only 1/n of the time */
fprintf(stderr,"entering insertnd\n");
	asserttreeisok(fullcrumptr);
}
#endif
	  
#ifdef UNdeFINed
/**/fixincoresubtreewids(fullcrumptr);/*1999 // a temp kluge zzz till we find where setwisp isnt called//*/ /* this is a brute  force kluge, if this fixes anything it means that the wids aren't being set properly somplace else probably near here */
	fprintf(stderr,"entering insertnd \n");
     switch (fullcrumptr->cenftype) {
       case POOM:
	fprintf(stderr,"in insertnd  dumppoomwisps\n");
	dumppoomwisps(fullcrumptr);
	break;
       case SPAN:
	fprintf(stderr,"in insertnd  showspanf\n");
	showspanf(fullcrumptr);
	break;
     }
#endif

     olddiff = widdiffs(fullcrumptr);
     oldheight = fullcrumptr->height;
     if (iszerotumbler (&width->dsas[index]))
	   gerror ("zero width in insertnd\n");
     tumblercopy(&fullcrumptr->cwid.dsas[V],&oldwid);
     switch (fullcrumptr->cenftype) {

       case POOM:
	  makegappm (taskptr, fullcrumptr, origin, width);      
	checkspecandstringbefore();
	  setwispupwards(fullcrumptr,0);
	  bothertorecombine=doinsertnd(fullcrumptr,origin,width,infoptr,index);
	  setwispupwards(fullcrumptr,1);
	  /*fixincoresubtreewids(fullcrumptr);//1999 // a temp kluge zzz till we find where setwisp isnt called// this is a brute  force kluge, if this fixes anything it means that the wids aren't being set properly somplace else probably near here */

	  break;
       case SPAN:
	  bothertorecombine=doinsertnd(fullcrumptr,origin,width,infoptr,index);
	  setwispupwards(fullcrumptr,1);
	  /*fixincoresubtreewids(fullcrumptr);//1999 // a temp kluge zzz till we find where setwisp isnt called// this is a brute  force kluge, if this fixes anything it means that the wids aren't being set properly somplace else probably near here */
	   break;
       default:
	  gerror ("insertnd: bad enftype\n");
     }
     if(/*TRUE||*/bothertorecombine||(fullcrumptr->height != oldheight)){
#ifndef DISTRIBUTION
	if(!bothertorecombine){
		fprintf(stderr,"insertnd recombineing and notbothertorecombine\n");
	}
#endif
	  recombine (fullcrumptr); 
     }
     newdiff = widdiffs(fullcrumptr);
#ifndef DISTRIBUTION
     if(FALSE&&(fullcrumptr->cenftype==POOM)&&olddiff != newdiff){      
	  fprintf(stderr,"insertnd  possible error dumping widdiffs%d %d\n\r",olddiff,newdiff);
	dump/*wholesubtree*/(fullcrumptr);
	  fprintf(stderr,"insertnd not match after insert asserting \n");       
		asserttreeisok(fullcrumptr);
     }
#endif
#ifndef DISTRIBUTION
 if(FALSE&&fullcrumptr->cenftype==POOM&&tumblereq(&oldwid,&fullcrumptr->cwid.dsas[V])){/* remember links this code doesnt work*/
		fprintf(stderr,"insertnd no change after insert\n");
		dumptumbler(&oldwid);
		dumpwholesubtree(fullcrumptr);
		gerror("insertnd no change after insert");
	}
#endif
	  

#ifdef UNdeFIned
	/**/fixincoresubtreewids(fullcrumptr);/*1999 // a temp kluge zzz till we find where setwisp isnt called//*/ /* this is a brute  force kluge, if this fixes anything it means that the wids aren't being set properly somplace else probably near here */
	fprintf(stderr,"leaving insertnd \n");
     switch (fullcrumptr->cenftype) {
       case POOM:
	fprintf(stderr,"in insertnd  dumppoomwisps\n");
	dumppoomwisps(fullcrumptr);
	break;
       case SPAN:
	fprintf(stderr,"in insertnd  showspanf\n");
	showspanf(fullcrumptr);
	break;
     }
#endif
}
			       

  static INT
widdiffs(crumptr)
  typecuc *crumptr;
{
  INT i,v;
	if(crumptr->cenftype != POOM)
		return(0);
	i = lastdigitintumbler(&crumptr->cwid.dsas[I]);	
	v = lastdigitintumbler(&crumptr->cwid.dsas[V]);	
	return(i-v);
}

makegappm (taskptr, fullcrumptr, origin, width)
  typetask *taskptr;
  typecuc *fullcrumptr;
  typewid *origin , *width;
{
  typeknives knives;
  typewid offset, grasp, reach;
  typecuc *father;
  typecorecrum *ptr, *findleftson();
  typewid foffset, fgrasp;
  INT i/*,enfheight*/;
		  
#ifndef DISTRIBUTION
foo("makegappm\n");
checkwholesubtree(fullcrumptr);
#endif
/*	enfheight = fullcrumptr->height;*/
	clear (&offset, sizeof(offset)); /* fullcrum alway has zero offset */
	prologuend ((typecorecrum*)fullcrumptr, &offset, &grasp, &reach);
	if (iszerotumbler (&fullcrumptr->cwid.dsas[V])
	|| tumblercmp (&origin->dsas[V], &grasp.dsas[V]) == LESS
	|| tumblercmp (&origin->dsas[V], &reach.dsas[V]) != LESS)
		return;    /* this if for extensions to bc without calling cut*/
	movetumbler (&origin->dsas[V], &knives.blades[0]);
	findaddressofsecondcutforinsert(&origin->dsas[V],&knives.blades[1]);
	knives.nblades = /*1*/2;
	knives.dimension = V;
	makecutsnd (fullcrumptr, &knives);
	newfindintersectionnd (fullcrumptr, &knives, &father, &foffset);
	prologuend ((typecorecrum*)father, &foffset, &fgrasp, (typedsp*)NULL);
	for (ptr = findleftson (father); ptr; ptr = findrightbro (ptr)) {
	       i=insertcutsectionnd(ptr,&fgrasp,&knives);
		switch (i) {
		  case 0:
		  case 2:
			break;
		  case -1:      /* THRUME*/
			dump(ptr);
			gerror ("makegappm can't classify crum\n");
			break;
		  case 1:/*9-17-87 fix */
			tumbleradd(&ptr->cdsp.dsas[V],&width->dsas[V],&ptr->cdsp.dsas[V]);
			/*tumbleradd(&width->dsas[V],&ptr->cdsp.dsas[V],&ptr->cdsp.dsas[V]);*/
			ivemodified (ptr);
			break;
		  default:
			gerror ("unexpected cutsection\n");
		}
	}
      setwidnd(father);
	setwispupwards (findfather ((typecorecrum*)father),1);
}

findaddressofsecondcutforinsert (position, secondcut)
  tumbler *position;
  tumbler *secondcut;
{    /*needs this to give it a place to find intersectionof for text is 2.1*/
  tumbler zero, intpart;

       tumblerclear (&zero);
       tumblerincrement (position, -1, 1, secondcut);
       beheadtumbler (position, &intpart);
       tumblerincrement(secondcut,0,-tumblerintdiff(&intpart,&zero),secondcut);
       tumblerincrement (secondcut, 1, 1, secondcut);
}

  INT
doinsertnd (father, origin, width, infoptr, index)
  typecuc *father;
  typewid *origin, *width;
  type2dbottomcruminfo *infoptr;
  INT index;
{
  typedsp  offset;
		   
	if (iszerotumbler (&width->dsas[index]))
		/*q*/gerror ("zero width in doinsertnd\n");
	if (isemptyenfilade (father)) {
		firstinsertionnd (father, origin, width, infoptr);
		return(FALSE);
	}
	clear (&offset, sizeof(typedsp));
	return(insertmorend (father, &offset, origin, width, infoptr, index));
}

firstinsertionnd (father, origin, width, infoptr)
  typecuc *father;
  typewid *origin, *width;
  type2dbottomcruminfo *infoptr;
{
  typecorecrum *ptr;

	ptr = findleftson (father);
	movewisp (origin, &ptr->cdsp);
	movewisp (width, &ptr->cwid);
	move2dinfo (infoptr, &((type2dcbc *)ptr)->c2dinfo);
	ivemodified (ptr);
	setwisp ((typecorecrum*)father);
	return;
}
  INT 
insertmorend (father, offset, origin, width, infoptr, index)
  typecuc *father;
  typedsp *offset;
  typewid *origin, *width;
  type2dbottomcruminfo *infoptr;
  INT index;
{

  typedsp grasp;
  typecorecrum *ptr;
  typecorecrum *findsontoinsertundernd();
  INT temp;
		    
	if (iszerotumbler (&width->dsas[index]))
		gerror ("zero width in insertmorend\n");
	makeroomonleftnd (father, offset, origin, &grasp);
	if (father->height == 1) {
	       return(insertcbcnd (father, &grasp, origin, width, infoptr));
	}
	ptr = findsontoinsertundernd (father, &grasp, origin, width, index);
	temp = insertmorend ((typecuc*)ptr, &grasp, origin, width, infoptr, index);
	/*setwispupwards(ptr,1);*/ /* was done in insertcbcnd*/
	setwispupwards(father,1);
	ivemodified(ptr);/*zzz possibly redundant with setwispupwards*/
	return(temp);
}


  INT
insertcbcnd (father, grasp, origin, width, infoptr)
  typecuc *father;
  typedsp *grasp;
  typewid *origin, *width;
  type2dbottomcruminfo *infoptr;
{
  typecorecrum *ptr, *new;
  typecorecrum *createcrum();
  bool isanextensionnd(), iszerolock();
  bool splitsomething, splitcrumupwards();

	for (ptr = findleftson (father); ptr; ptr = findrightbro (ptr)) {
		if (isanextensionnd ((typecbc*)ptr, grasp, origin, infoptr)) {
			dspadd (&ptr->cwid, width, &ptr->cwid, (INT)father->cenftype);
			ivemodified (ptr);
			setwispupwards (father,1);
			if(!isfullcrum((typecorecrum*)father)){
				return(setwispupwards(findfather((typecorecrum*)father),1)); /* was ...),1); ECH ****/
			}
			return(FALSE);
		}
	}
	new = createcrum (0, (INT)father->cenftype);
	reserve (new);
	adopt (new, SON, (typecorecrum*)father);
	dspsub (origin, grasp, &new->cdsp, (INT)father->cenftype);
	if (iszerolock ((tumbler*)width, 2))
		gerror ("zero width in insertnd\n");
	movewisp (width, &new->cwid);
	move2dinfo (infoptr, &((type2dcbc *)new)->c2dinfo);
	ivemodified (new);
	/*setwispallthewayupwards (new//father//);*/
	setwispupwards ((typecuc*)new/*father*/,0);
	setwispupwards (father,1);
	splitsomething = splitcrumupwards (father);
	rejuvinate (new);
	return(splitsomething);
}
 
  typecorecrum *
findsontoinsertundernd (father, grasp, origin, width, index)
  typecuc *father;
  typedsp *grasp;
  typewid *origin, *width;
  INT index;
{
  typecorecrum *ptr, *nearestonleft;
  tumbler spanend, sonstart;

	if (iszerotumbler (&width->dsas[index]))
		gerror ("width is zero in findsontoinsertundernd\n");
	tumbleradd (&origin->dsas[index], &width->dsas[index], &spanend);
	ptr = nearestonleft = findleftson (father);
	for (; ptr; ptr = findrightbro(ptr)) {
	       tumbleradd(&grasp->dsas[index],&ptr->cdsp.dsas[index],&sonstart);
	       if (
		  tumblercmp (&sonstart, &origin->dsas[index]) != GREATER
	       && tumblercmp (&ptr->cdsp.dsas[index], &nearestonleft->cdsp.dsas[index]) != LESS) {
		       nearestonleft = ptr;
	       }
	       if (
		  whereoncrum(ptr,grasp,&origin->dsas[index],index)>=ONMYLEFTBORDER
	       && whereoncrum (ptr, grasp, &spanend, index) <= ONMYRIGHTBORDER)
		       return (ptr);
	}
	return (nearestonleft);
}

  bool
isanextensionnd (ptr, offsetptr, originptr, infoptr)
  typecbc *ptr;
  typedsp *offsetptr, *originptr;
  type2dbottomcruminfo *infoptr;
{
  typedsp grasp, reach;
  bool lockeq();
	if (!tumblereq (&infoptr->homedoc, &((type2dcbc *)ptr)->c2dinfo.homedoc))
		return (FALSE);
	prologuend ((typecorecrum*)ptr, offsetptr, &grasp, &reach);
	return (lockeq (reach.dsas, originptr->dsas, (unsigned)dspsize(ptr->cenftype)));
}

