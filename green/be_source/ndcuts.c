/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
#include "crum.h"
#include "ndenf.h"
#include "players.h"
   bool crumiscut(),sonsarecut(),crumiscutbyithknife();

 
makecutsnd(fullcrumptr, knives)
  typecuc *fullcrumptr;
  typeknives *knives;
{
  typewid offset; 

logbertmodifiedforcrum(fullcrumptr, user);
/*(fprintf(stderr,"\nmakecutsnd knives = \n"); displaycutspm(knives);*/
	clear(&offset,sizeof(offset));	 
	makecutsdownnd(fullcrumptr,&offset,knives);
	clear(&offset,sizeof(offset));	 /*clears ARE redundant*/
	for (fullcrumptr = findfullcrum((typecorecrum*)fullcrumptr); sonsarecut(fullcrumptr, &offset, knives); fullcrumptr = findfullcrum((typecorecrum*)fullcrumptr)) {
		clear(&offset,sizeof(offset));	 
		makecutsdownnd(fullcrumptr,&offset,knives);
	}
#ifndef DISTRIBUTION
asserttreeisok(fullcrumptr);
#endif
} 

makecutsdownnd (fullcrumptr,offset, knives)
  typecuc *fullcrumptr;
  typewid *offset;
  typeknives *knives;
{	      
  typecuc *ptr;
  typecorecrum *sonptr,*son;
  INT n;
   
	ptr = fullcrumptr;
	for (;(knives->nblades > 1) && FALSE && ptr->height;) {
		for (n = 0, son = findleftson(ptr); son; son = findrightbro(son)) {
			if (crumiscut((typecuc*)son, offset, knives)) {
				n++;
				sonptr = son;
			}
		}
		if (n==1) {
			prologuend((typecorecrum*)ptr, offset, offset, (typedsp*)NULL);
			ptr = (typecuc *)sonptr;
		} else {
			break;
		} 
	} /*now we're at intersection*/ 
	makecutsbackuptohere(ptr,offset,knives);
	if (toomanysons(ptr)) {
/*fprintf(stderr,"found toomany sons in makecutsdownnd\n");*/
		if (isfullcrum((typecorecrum*)ptr)) {
/*fprintf(stderr,"and was fullcrum\n");*/
/*return;*/
			levelpush(ptr);
			makecutsnd(fullcrumptr,knives);
/*fprintf(stderr,"returningfrom makecutsnd\n");	*/
		} else {			 
			makecutsnd(fullcrumptr,knives);
		}
	}
}
		
makecutsbackuptohere(ptr,offset, knives)
  typecuc *ptr;
  typewid *offset;
  typeknives *knives;
{ 
  typewid grasp;
  INT i;
  typecuc *son, *new, *nextson;
  typecorecrum *createcrum(),*findleftson();
  bool crumiscutbyithknife();

	if (ptr->height == 0) {
	    for (i = 0; i < knives->nblades; i++) {     
		if (whereoncrum((typecorecrum*)ptr, offset, &knives->blades[i], knives->dimension) == THRUME) {
		      new = (typecuc *)createcrum((INT)ptr->height,(INT)ptr->cenftype);
			if (ptr->cenftype == GRAN) {
				((typecbc *)new)->cinfo.infotype = ((typecbc *)ptr)->cinfo.infotype;
			}
		      slicecbcpm((typecorecrum*)ptr, offset, (typecorecrum*)new, &knives->blades[i], knives->dimension);
		      ivemodified((typecorecrum*)ptr);
		      ivemodified((typecorecrum*)new);
	    /**/setwisp((typecorecrum*)ptr);/**/
/*asserttreeisok(ptr);*/
		}
	    }
	    return;
	} else {
	  for (i=0;i<knives->nblades;++i) {
	    cutsons(ptr,offset,knives);  

	    if (!isfullcrum((typecorecrum*)ptr)) {
	      /**/setwispupwards(ptr,0);/**/
	      if (crumiscutbyithknife(ptr,offset,knives, i)) {
		for (son = (typecuc *)findleftson(ptr);son;son=nextson) {
		  nextson = (typecuc *)findrightbro((typecorecrum*)son);
		  prologuend((typecorecrum*)ptr, offset, &grasp, (typedsp*)NULL);

		  makeithcutonson((typecorecrum*)ptr, offset, (typecorecrum*)son, &grasp, knives, i);
		  if (!crumiscutbyithknife(ptr, offset, knives, i)) {
			if (ptr->numberofsons == 0) {
				return;
			}
			break;
		  }
		}
	      }
	      /**/setwispupwards(ptr,0);/**/
	    }
/*asserttreeisok(ptr);*/
	  }
	}
	if (!isfullcrum((typecorecrum*)ptr)) {
		if (toomanysons(ptr)) {
			while(toomanysons(ptr)) {
#ifndef DISTRIBUTION
fprintf(stderr,"setwispupwards 4\n",1);
#endif
				/**/setwispupwards(ptr,0);/**/
#ifndef DISTRIBUTION
				fprintf(stderr,"toomanysons peel\n");
#endif
				peeloffcorrectson((typecorecrum*)ptr,knives);
			}
#ifndef DISTRIBUTION
	fprintf(stderr,"calling makecutsbackuptohere\n");
#endif
			makecutsbackuptohere(ptr,offset,knives);
		}
	} else if (toomanysons(ptr)) {
#ifndef DISTRIBUTION
		fprintf(stderr,"________________toomanysons in fullcrum\n");
#endif
	}
	/**/setwispupwards(ptr,1);/**/
}  

cutsons(ptr,offset,knives)
  typecuc *ptr;
  typewid *offset;
  typeknives *knives;
{
  typewid grasp;
  typecorecrum *son,*nextson,*findleftson();

	while (sonsarecut(ptr,offset,knives)) {
		/**/setwispupwards(ptr,1);/**/
		for (son = findleftson(ptr); son; son = nextson) {
		   nextson = findrightbro(son);    
		   prologuend((typecorecrum*)ptr, offset, &grasp, (typedsp*)NULL);
		   for (; crumiscut((typecuc*)son, &grasp, knives);
			  prologuend((typecorecrum*)ptr, offset, &grasp, (typedsp*)NULL)) {
			makecutsbackuptohere((typecuc*)son, &grasp, knives);
		   }
		}
	}
}    

makeithcutonson(ptr,offset,son,grasp,knives,i)
  typecorecrum *ptr,*son;
  typewid *offset,*grasp;
  typeknives *knives;
  INT i;
{
  INT temp;
	if (!crumiscutbyithknife((typecuc*)ptr, offset, knives, i)) {
#ifndef DISTRIBUTION
fprintf(stderr,"foo return in makeithcutonson\n");
#endif
		return;
	}
	if ((temp =whereoncrum(son,grasp,&knives->blades[ i ],knives->dimension))<THRUME) {
		peelsoncorrectly(ptr,offset,son,grasp,knives,i);
		/**/setwispupwards((typecuc*)ptr,1);/**/
		if (!crumiscutbyithknife((typecuc*)ptr,offset,knives, i )) {
			/**/setwispupwards((typecuc*)ptr,1);/**/
			/*prologuend(ptr,offset,grasp,NULL);*/
			if (((typecuc *)ptr)->numberofsons == 0) {
#ifndef DISTRIBUTION
				displaycutspm(knives); dumpwid(offset,ptr->cenftype);
				fprintf(stderr,"sons went away\n");
				/*dump(ptr);*/ /*dumpwholetree(ptr);*/
				check(ptr);
#endif
				return;
			}
			return;
		}       
		(void) findleftson((typecuc*)ptr);/* make sure its in core*/
		if (((typecuc *)ptr)->numberofsons == 0) {
#ifndef DISTRIBUTION
			displaycutspm(knives); dumpwid(offset,ptr->cenftype);
			fprintf(stderr,"____sons went away\n");
			dump(ptr); 
			check(ptr);
			gerror ("sons went away");
#else
			gerror("");
#endif
		}
	} else if (temp ==THRUME) { 
#ifndef DISTRIBUTION
		gerror("makecutsbackuptohere crum not cut");
#else
		gerror("");
#endif
	}
}

	  
peelsoncorrectly(ptr,offset,son,grasp,knives,i)
  typecorecrum *ptr,*son;
  typewid *offset,*grasp;
  typeknives *knives;
  INT i;
{  /* put son in leftest uncle with room that  is less than cut*/
  typecuc *uncle;
  bool crumleftofithcut();
  typecorecrum *createcrum();

	uncle = (typecuc *)findleftmostbro(ptr);
	for (; uncle; uncle = (typecuc *)findrightbro((typecorecrum*)uncle)) {
		if (uncle == (typecuc *)ptr) {
			continue;
		}
		if (roomformoresons(uncle)) {
			if (crumleftofithcut((typecorecrum*)uncle,offset,knives,i)) {
				newpeelcrumoffnd((typecorecrum*)son,(typecorecrum*)uncle);
			  return;
			}
		}
	}	
	uncle = (typecuc *)createcrum((INT)ptr->height, (INT)ptr->cenftype);
	adopt((typecorecrum*)uncle,RIGHTBRO,findrightmostbro(ptr));
	movewisp (&ptr->cdsp, &uncle->cdsp); 
	newpeelcrumoffnd((typecorecrum*)son,(typecorecrum*)uncle);
}

  bool
crumleftofithcut(ptr,offset,knives,i)
  typecorecrum *ptr;
  typewid *offset;
  typeknives *knives;
  INT i;
{
	if (whereoncrum(ptr,offset,&knives->blades[i],knives->dimension)>THRUME) {
		return(FALSE);
	} else {
		return(TRUE);
	}
}

peeloffcorrectson(ptr,knives)
  typecorecrum *ptr;
  typeknives *knives;
{
  typecorecrum *bro, *uncle, *createcrum(), *findleftson();

	if (toomanysons((typecuc*)ptr)) {
		for (bro = findrightbro(ptr); bro; ptr = findrightbro(ptr)) {
			if (roomformoresons/*!toomanysons*/((typecuc*)bro)) {
				newpeelcrumoffnd(findleftson((typecuc*)ptr),bro);
				return(TRUE);
			}
		}
		uncle=createcrum((INT)ptr->height,(INT)ptr->cenftype);
		adopt(uncle,RIGHTBRO,ptr);
		movewisp (&ptr->cdsp, &uncle->cdsp); 
		newpeelcrumoffnd(findleftson((typecuc*)ptr),uncle);
	}
	return(TRUE);
}

	   

newpeelcrumoffnd (ptr,newuncle)
  typecorecrum *ptr,*newuncle;
{
  typedsp temp;
  typecuc *father;
  typedsp grasp;
  typewid origin;
  typedsp offset;

	if (!roomformoresons((typecuc*)newuncle)) {
#ifndef DISTRIBUTION
		gerror("noroomfor more  sons for newuncle\n");
#else
		gerror("");
#endif
	}
	if (isfullcrum(ptr)) {
#ifndef DISTRIBUTION
		gerror("peeloffcurmnd called with fullcrum");
#else
		gerror("");
#endif
	}
	ivemodified(ptr); /* to set father->modified*/
	father = findfather(ptr);
	clear(&offset,sizeof(offset));
	dspadd(&father->cdsp, &ptr->cdsp, &origin, (INT)father->cenftype);     
/* 1 ptr->cdsp */
/*
fprintf(stderr,"in newpeelcrumoffnd 1 \n"); 
dumpwid(&father->cdsp);
dumpwid(&ptr->cdsp);
dumpwid(&origin);
*/
	(void) tumblercheckptr((tumbler *) &origin, (INT *) ptr);   

	makeroomonleftnd((typecuc*)newuncle, &offset, &origin, &grasp);
	disown (ptr);
	adopt (ptr, LEFTMOSTSON, newuncle);
	/*dspsub(&father->cdsp,&newuncle->cdsp,&temp,ptr->cenftype); */
/* 2 &temp <- father->cdsp - newuncle->cdsp check this father temp stuff4/6/85 */
	dspadd(&father->cdsp, &ptr->cdsp, &temp, (INT)ptr->cenftype);
/*
dumpwid(&newuncle->cdsp);
fprintf(stderr,"in newpeelcrumoffnd 2 \n");
dumpwid(&temp);
*/
	(void) tumblercheckptr((tumbler *) &temp, (INT *) ptr);
							   
	/*dspadd(&ptr->cdsp,&temp,&ptr->cdsp, ptr->cenftype);    */
	dspsub(&temp, &newuncle->cdsp, &ptr->cdsp, (INT)ptr->cenftype);
/* 3 ptr->cdsp  <- temp + ptr->cdsp */
/*
fprintf(stderr,"in newpeelcrumoffnd 3 \n");
dumpwid(&ptr->cdsp);
dump(ptr);
*/
	(void) tumblercheckptr((tumbler *) &ptr->cdsp, (INT *) ptr);

	ivemodified (ptr);
	ivemodified(newuncle);
	/*setwispsofsons(newuncle);// to fix makeroomonleftnd//*/
	setwispupwards((typecuc*)ptr,0);
	setwispupwards((typecuc*)newuncle,0);
	setwispupwards(father,1);
	if (toomanysons((typecuc*)newuncle)) {
#ifndef DISTRIBUTION
		gerror("toomany sons for newuncle\n");
#else
		gerror("");
#endif
	}
/*fprintf(stderr,"calling fixincoresubtreewids\n");*/
/*fixincoresubtreewids(findfullcrum(ptr));*/
}



  bool
crumiscut(ptr,offset,knives)
  typecuc *ptr;
  typewid *offset;
  typeknives *knives;
{
  INT i;

	for (i = 0; i<knives->nblades; ++i) {   
		if (whereoncrum ((typecorecrum*)ptr, offset, &knives->blades[i], knives->dimension) == THRUME) {
			 return(TRUE);
		}
	}	 
	return(FALSE);
}

  bool
crumiscutbyithknife(ptr, offset, knives, i)
  typecuc *ptr;
  typewid *offset;
  typeknives *knives;
  INT i;
{
	if (whereoncrum((typecorecrum*)ptr, offset, &knives->blades[i], knives->dimension) == THRUME) {
		return(TRUE);
	}
	return(FALSE);
}
  
  bool
sonsarecut (ptr,offset, knives)
  typecuc *ptr;
  typewid *offset;
  typeknives *knives;
{	      
  typecuc *son;
  bool crumiscut();
  typewid grasp;
	
	prologuend((typecorecrum*)ptr, offset, &grasp, (typedsp*)NULL);
	for (son = (typecuc *)findleftson(ptr); son; son = (typecuc*)findrightbro((typecorecrum*)son)) {
		if (crumiscut(son, &grasp, knives))
			return(TRUE);
	}
	return(FALSE);
}

slicecbcpm (ptr, offset, new, cut, index)
  typecorecrum  *ptr;
  typecorecrum  *new;
  typewid *offset;
  tumbler *cut;
  INT index;
{
  typedsp grasp/*, reach*/;
  tumbler localcut;
  typewid newwid;
  INT enftype, i;
/*fprintf(stderr,"entering slicecbcpm \n");*/
	enftype = ptr->cenftype;

	prologuend (ptr, offset, &grasp, /*&reach*/ (typedsp*)NULL);
	if (whereoncrum (ptr, offset, cut, index) != THRUME)
#ifndef DISTRIBUTION
		gerror ("Why are you trying to slice me?\n");
#else
		gerror("");
#endif
	if (!lockis1story (ptr->cwid.dsas, (unsigned)widsize(enftype)))
#ifndef DISTRIBUTION
		gerror ("Not one story in POOM wid\n");
#else
		gerror("");
#endif

	tumblersub (cut, &grasp.dsas[index], &localcut);

	if (localcut.exp != ptr->cwid.dsas[index].exp ) {
#ifndef DISTRIBUTION
		dump(ptr);
		dump(new);
		dumptumbler(cut);
		dumptumbler(&localcut);
		dumpwholetree(ptr);
		gerror ("Oh well, I thought I understood this1\n");
#else
		gerror("");
#endif
	}
	if (!is1story (&localcut)) {
#ifndef DISTRIBUTION
fprintf(stderr,"\nin slicecbcpm  ptr \n");
		dump(ptr);
fprintf(stderr,"\nin slicecbcpm new  \n");
		dump(new);
fprintf(stderr,"\nin slicecbcpm  offset \n");
		dumpwid(offset,ptr->cenftype);
fprintf(stderr,"\nin slicecbcpm  grasp \n");
		dumpwid(&grasp,ptr->cenftype);
fprintf(stderr,"\nin slicecbcpm  cut \n");
		dumptumbler(cut);
fprintf(stderr,"\nin slicecbcpm  localcut \n");
		dumptumbler(&localcut);
fprintf(stderr,"\nin slicecbcpm wholedamn tree  \n");
		dumpwholetree(ptr);
		gerror ("Oh well, I thought I understood this2\n");
#else
		gerror("");
#endif
	}
	if (tumblerlength (cut) != tumblerlength (&ptr->cwid.dsas[index]))
#ifndef DISTRIBUTION
		gerror ("level mismatch\n");
#else
		gerror("");
#endif

	movewisp (&ptr->cwid, &newwid);
	for (i = 0; i < widsize(enftype); i++) {/* I really don't understand this loop */
		newwid.dsas[i].mantissa[0] = localcut.mantissa[0];
		tumblerjustify (&newwid.dsas[i]);
	}
	/*locksubtract (&ptr->cwid, &newwid, &new->cwid, widsize (enftype));*/
	locksubtract ((tumbler*)&ptr->cwid, (tumbler*)&newwid, (tumbler*)&new->cwid, (unsigned)widsize(enftype));
	movewisp (&newwid, &ptr->cwid);
	dspadd (&ptr->cdsp, &ptr->cwid, &new->cdsp, enftype);
	move2dinfo (&((type2dcbc *)ptr)->c2dinfo, &((type2dcbc *)new)->c2dinfo);
	adopt(new,RIGHTBRO,ptr); 
/*fprintf(stderr,"leaving slicecpcpm \n");*/
}

/*  // old code 3/5/85  //
slicecbcpm (ptr, offset, new, cut, index)
  typecorecrum  *ptr;
  typecorecrum  *new;
  typewid *offset;
  tumbler *cut;
  INT index;
{
  typedsp grasp//, reach//;
  tumbler localcut;
  typewid newwid;
  INT enftype, i;
#ifndef DISTRIBUTION
fprintf(stderr,"entering slicecpcpm \n");
#endif
	enftype = ptr->cenftype;

	prologuend (ptr, offset, &grasp, //&reach// NULL);
	if (whereoncrum (ptr, offset, cut, index) != THRUME)
#ifndef DISTRIBUTION
		gerror ("Why are you trying to slice me?\n");
#else
		gerror("");
#endif
	if (!lockis1story (ptr->cwid.dsas, widsize(enftype)))
#ifndef DISTRIBUTION
		gerror ("Not one story in POOM wid\n");
#else
		gerror("");
#endif

	tumblersub (cut, &grasp.dsas[index], &localcut);

	if (localcut.exp != ptr->cwid.dsas[index].exp ) {
#ifndef DISTRIBUTION
		dump(ptr);
		dump(new);
		dumptumbler(cut);
		dumptumbler(&localcut);
		dumpwholetree(ptr);
		gerror ("Oh well, I thought I understood this1\n");
#else
		gerror("");
#endif
	}
	if (!is1story (&localcut)) {
#ifndef DISTRIBUTION
		dump(ptr);
		dump(new);
		dumptumbler(cut);
		dumptumbler(&localcut);
		dumpwholetree(ptr);
		gerror ("Oh well, I thought I understood this2\n");
#else
		gerror("");
#endif
	}
	if (tumblerlength (cut) != tumblerlength (&ptr->cwid.dsas[index]))
#ifndef DISTRIBUTION
		gerror ("level mismatch\n");
#else
		gerror("");
#endif

	movewisp (&ptr->cwid, &newwid);
	for (i = 0; i < widsize(enftype); i++) {// I really don't understand this loop //
		newwid.dsas[i].mantissa[0] = localcut.mantissa[0];
		tumblerjustify (&newwid.dsas[i]);
	}
	//locksubtract (&ptr->cwid, &newwid, &new->cwid, widsize (enftype));//
	locksubtract (&ptr->cwid, &newwid, &new->cwid, widsize (enftype));
	movewisp (&newwid, &ptr->cwid);
	dspadd (&ptr->cdsp, &ptr->cwid, &new->cdsp, enftype);
	move2dinfo (&ptr->c2dinfo, &new->c2dinfo);
	adopt(new,RIGHTBRO,ptr); 
}
  */



