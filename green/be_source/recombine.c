/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License,
* which contains precisely the terms of the X11 License.  The full text of
* The Udanax Open-Source License can be found in the distribution in the file
* license.html.  If this file is absent, a copy can be found at
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
#include "crum.h"
#include "reap.h"

long noishouldbother = 0;
long notakenephewnd = 0;
long noeatbrosnd = 0;

recombine (father)
  typecuc *father;
{
        switch (father->cenftype) {
          case GRAN:
                recombineseq (father);
                break;
          case SPAN:
                recombinend (father);
                break;
          case POOM:
                recombinend (father);
        }
}

/* GRANfilade recombine */

recombineseq (father) /** zzz reg 1999 this recombines too much */
  typecuc *father;
{ 
  typecuc *ptr;
        if (father->height < 3 || !father->modified){
                return;
	}
	if(!roomformoresons(father)){
		return;
	}
        for (ptr = (typecuc *)getleftson (father); ptr; ptr = (typecuc *)macrogetrightbro ((typecorecrum *)ptr)){
                recombineseq (ptr);
	}
        for(ptr=(typecuc *)getleftson(father);ptr&&ptr->rightbro;ptr=(typecuc *)macrogetrightbro((typecorecrum *)ptr)){
                if (ptr->age == RESERVED)
                        continue;
                if (ptr->leftson && roomformoresons (ptr)) {
                /*if (ptr->leftson && toofewsons (ptr)) {**/
                        if (((typecuc *)ptr->rightbro)->leftson) {
                                if (ptr->numberofsons + ((typecuc *)ptr->rightbro)->numberofsons <= MAXUCINLOAF) {
                                        eatbrossubtreeseq (ptr);
                                        break;
                                } else {
                                        takeovernephewsseq (ptr);
                                        break;
                                }
                        }
                }
        }
        if (father->isapex)
                levelpull (father);
}

takeovernephewsseq (me)
  typecorecrum *me;
{
  typecuc *ptr;
  typecorecrum *next;

        for(ptr=(typecuc *)getleftson(routinegetrightbro(me));ptr&&roomformoresons(me);ptr=(typecuc *)next){
                next = (typecorecrum *)routinegetrightbro (ptr);
                if (ptr->age == RESERVED)
                        continue;
                disown (ptr);
                adopt (ptr, RIGHTMOSTSON, me);
                ivemodified (ptr);
        }
        setwispupwards (getrightbro (me),0);
        setwispupwards (me,1);
}

eatbrossubtreeseq (me)
  typecuc *me;
{
  typecuc *bro;

        bro = (typecuc *)getrightbro (me);
        getleftson (bro)->leftbroorfather = getrightmostbro (getleftson (me));
        getrightmostbro (getleftson (me))->rightbro = bro->leftson;
        bro->leftson->isleftmost = FALSE;

        me->numberofsons += bro->numberofsons;
        disown (bro);
        freecrum (bro);
        setwispupwards (me,1);
}

/* 2d recombine */

recombinend (father)
  typecuc *father;
{ 

  typecorecrum *ptr;
  typecorecrum *sons[MAXUCINLOAF];
  INT i, j, n;
  bool ishouldbother();

        if (father->height < 2  || !father->modified)
                return;
        for (ptr = getleftson (father); ptr;ptr=(typecorecrum *)getrightbro (ptr)){
                recombinend (ptr);
        }

        getorderedsons (father, sons);
        n = father->numberofsons;
        for (i = 0; i < n-1; i++) {
                for (j = i+1; sons[i] && j < n; j++) {
                        if(i != j && sons[j] && ishouldbother(sons[i],sons[j])){
                                takeovernephewsnd (&sons[i], &sons[j]);
                                /*break;*/
                                /*break;//zzz6/16/84 reg//*/
                        }
                }
        }
        if (father->isapex)
                levelpull (father);
}
  bool randomness(probability)
    float probability;
    {
  static float i = 0;
  return(TRUE);
  /*
	i += probability;
	if(i>=1.){
		while (i>1){
			i -= 1.;
		}
		return(TRUE);
	}else{
		return FALSE;
	}
	*/
}


	bool
ishouldbother (dest, src)
  typecuc *dest, *src;
{
        ++noishouldbother;
	if(src->numberofsons == 0){
		if(src->sonorigin.diskblocknumber == DISKPTRNULL){
			check(src);
		}else{
			return(FALSE);
		}
	}
        if (dest->age == RESERVED || src->age == RESERVED)
                return (FALSE);
        return (dest->numberofsons + src->numberofsons <= (dest->height>1 ? MAXUCINLOAF : MAX2DBCINLOAF)&&randomness(.3));
}

takeovernephewsnd (meptr, broptr)
  typecuc **meptr, **broptr;
{
  typecorecrum *sons[MAXUCINLOAF], *ptr;
  typecuc *me, *bro;
  INT i, n;
  bool ret;
  bool roomformoresons();

        me = *meptr;
        bro = *broptr;
        if (!me->leftson || !bro->leftson){
                return (FALSE);
        }
        ret = FALSE;
        if (me->numberofsons + bro->numberofsons <= MAXUCINLOAF) {
                eatbrossubtreend (me, bro);
                *broptr = NULL;
                return (TRUE);
        } else {
                getorderedsons (bro, sons);
		findleftson(bro);/* to make sure its in core zzzz*/
                n = bro->numberofsons;
                for (i = 0; i < n && roomformoresons (me); i++) {
                        ptr = sons[i];
                        takenephewnd (me, ptr);
			/*fixincoresubtreewids(me);*/
                        ret = TRUE;
                }
        }
        if (bro->numberofsons)
                setwispupwards (bro,0);
        else {
                disown (bro);
                freecrum (bro);
                *broptr = NULL;
        }
        setwispupwards (me,1);
        return (ret);
}

eatbrossubtreend (me, bro)
  typecuc *me, *bro;
{
  typedsp offset, grasp;
  typecuc *oldfather;
  typecorecrum *son;

        ++noeatbrosnd;
reserve(bro);
        clear (&offset, sizeof (offset));
/*fprintf(stderr,"in eatbrossubtreend calling makeroom onleftnd \n");*/
        makeroomonleftnd (me, &offset, &bro->cdsp, &grasp);
        fixdspsofbroschildren (me, bro);
        getleftson (bro)->leftbroorfather = getrightmostbro (getleftson (me));
        getrightmostbro (getleftson (me))->rightbro = getleftson (bro);
        bro->leftson->isleftmost = FALSE;

        me->numberofsons += bro->numberofsons;
        for(son = findleftson(me);son;son=findrightbro(son)){
                setwisp(son);
        }
oldfather =(typecuc *) findfather(bro);
rejuvinate(bro);
        disown (bro);
        freecrum (bro);
        setwispupwards (me,0);
setwispupwards (oldfather,1);
        ivemodified (me);
        /*fixincoresubtreewids(me);*/
}

takenephewnd (me, nephew)
  typecuc *me, *nephew;
{
  typecuc *bro;
  typedsp nephewsgrasp, grasp, offset;

        ++notakenephewnd;
        bro = (typecuc *)getfather (nephew);
        disown (nephew);
        dspadd (&bro->cdsp,&nephew->cdsp, &nephew->cdsp, bro->cenftype);
        adopt (nephew, RIGHTMOSTSON, me);
        prologuend (nephew, &bro->cdsp, &nephewsgrasp, (typedsp *)NULL);
/*fprintf(stderr,"in takenephewnd calling makeroomnd \n");*/
        makeroomonleftnd (me, &offset, &nephew->cdsp, &grasp);
        dspsub (&nephew->cdsp, &me->cdsp, &nephew->cdsp, me->cenftype);
        if (!bro->numberofsons) {
                disown (bro);
                freecrum (bro);
        } else  {
                setwispupwards (bro,0);
        }
        ivemodified (nephew);
        setwispupwards (me,1);
}

fixdspsofbroschildren (me, bro)
  typecuc *me, *bro;
{
  typecorecrum *nephew;

        for (nephew = getleftson (bro); nephew; nephew =(typecorecrum *)getrightbro (nephew)){
                dspadd (&bro->cdsp,&nephew->cdsp, &nephew->cdsp, me->cenftype);
                dspsub (&nephew->cdsp, &me->cdsp, &nephew->cdsp, me->cenftype);
                ivemodified(nephew);
        }
}

getorderedsons (father, sons)
  typecuc *father;
  typecorecrum *sons[];
{
  typecorecrum *ptr;
  INT i, comparecrumsdiagonally();

        sons[0] = NULL;
        for (ptr = getleftson (father), i = 0; ptr; ptr =(typecorecrum *)getrightbro (ptr))
                sons[i++] = ptr;
        sons[i] = NULL;
        shellsort (sons, i);
}

shellsort (v, n)
  typecorecrum *v[];
  INT n;
{
  typecorecrum *temp;
  INT gap, i, j;

  tumbler tarray[100],*tarrayp[100],*temptp;
	if(n>100){
#ifndef DISTRIBUTION
		gerror("fatal error in shellsort in be under recombine - n > 100");
#else
		gerror("sort crash");
#endif

	}
	for(i=0;i<n;i++){/* build up a list of sumps of disp[0] and dsp[1]*/
			   /* for compare crums diagonally hack */
		tumbleradd(&v[i]->cdsp.dsas[0],&v[i]->cdsp.dsas[1],&tarray[i]);
		tarrayp[i] = &tarray[i];
	}
        for (gap = n/2; gap > 0; gap /= 2)
            for (i = gap; i < n; i++)
                for (j = i-gap; j >= 0 && tumblercmp (tarrayp[j], tarrayp[j+gap]) == GREATER; j -= gap) {
                        temp = v[j];
			temptp = tarrayp[j];
                        v[j] = v[j+gap];
			tarrayp[j] = tarrayp[j+gap];
                        v[j+gap] = temp;
			tarrayp[j+gap] = temptp;
                }
}

  INT
comparecrumsdiagonally (a, b)
  typecorecrum *a, *b;
{
  tumbler amagnitude, bmagnitude;

        tumbleradd (&a->cdsp.dsas[0], &a->cdsp.dsas[1], &amagnitude);
        tumbleradd (&b->cdsp.dsas[0], &b->cdsp.dsas[1], &bmagnitude);
        return (tumblercmp (&amagnitude, &bmagnitude));
}
fixincoresubtreewids(ptr)
  typecuc *ptr;
{
  typecorecrum *son;
        if(ptr->height == 0)
                return;
        for(son = (typecorecrum *)getleftson(ptr);son;son = (typecorecrum *)getrightbro(son)){
                fixincoresubtreewids(son);
        }
        if(setwisp(ptr)){
#ifndef DISTRIBUTION
		fprintf(stderr,"fixing %x \n",ptr);
#endif
	}
}
