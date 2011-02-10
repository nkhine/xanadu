/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/

/* retrie.d - integrated enfilade retrieve routines */

#include "xanadu.h"
#include "enf.h"

  typecrumcontext *
retrievecrums (fullcrumptr, address, index)
  typecuc *fullcrumptr;
  tumbler *address;
  INT index;
{
  typecrumcontext *findcbcseqcrum();
  typedsp offset;
        clear (&offset, sizeof(typedsp));
        if (fullcrumptr->cenftype == GRAN)
                return (findcbcseqcrum ((typecorecrum*)fullcrumptr, &offset, address));
        else{
#ifndef DISTRIBUTION
                gerror ("retrivecrum - bad enftype\n");
#else
		gerror("");
#endif
		return(NULL); /* for lint*/
	}
	
}

  typecontext *
retrieve (fullcrumptr, address, index)
  typecuc *fullcrumptr;
  tumbler *address;
  INT index;    /* used when enftype == SPAN or POOM */
{
  typecontext *findcbcnd();
  typedsp offset;
  typecontext *findcbcseq();

        clear (&offset, sizeof(typedsp));
        switch (fullcrumptr->cenftype) {
          case GRAN:
                return (findcbcseq ((typecorecrum*)fullcrumptr, &offset, address));
          case SPAN:
          case POOM:
                return (findcbcnd ((typecorecrum*)fullcrumptr, &offset, address, index));
          default:
#ifndef DISTRIBUTION
                gerror ("retrieve - bad enftype\n");
#else
		gerror("");
#endif
                return (NULL);
        }
}

  typecontext *
retrieverestricted (fullcrumptr, span1ptr, index1, span2ptr, index2, docisaptr)
  typecuc *fullcrumptr;
  typespan *span1ptr, *span2ptr;
  INT index1, index2;
  typeisa *docisaptr;
{
  tumbler span1start, span1end, span2start, span2end;
  type2dbottomcruminfo info, *infoptr;
  typecontext *retrieveinarea();
  typecontext *temp;

        if (span1ptr) {
                movetumbler (&span1ptr->stream, &span1start);
                tumbleradd (&span1start, &span1ptr->width, &span1end);
        } else {
                tumblerclear (&span1start);
                tumblerclear (&span1end);
        }
        if (span2ptr) {
                movetumbler (&span2ptr->stream, &span2start);
                tumbleradd (&span2start, &span2ptr->width, &span2end);
        } else {
                tumblerclear (&span2start);
                tumblerclear (&span2end);
        }
        if (docisaptr) {
                movetumbler(docisaptr,&info.homedoc/*shouldberestrictiondoc*/);
                infoptr = &info;
        } else {
                infoptr = NULL;
        }
        temp = retrieveinarea (fullcrumptr, &span1start, &span1end, index1, &span2start, &span2end, index2, (typebottomcruminfo*)infoptr);
        return(temp);
}

  typecontext *
retrieveinarea (fullcrumptr, span1start, span1end, index1, span2start, span2end, index2, infoptr)
  typecuc *fullcrumptr;
  tumbler *span1start, *span1end, *span2start, *span2end;
  typebottomcruminfo *infoptr;
  INT index1, index2;
{
  typedsp offset;
  typecontext *context;
/*foo("entering retrieveinarea\n");*/
        clear (&offset, sizeof(offset));
        context = NULL;
        switch (fullcrumptr->cenftype) {
          case SPAN:
          case POOM:
                findcbcinarea2d ((typecorecrum*)fullcrumptr, &offset, span1start, span1end, index1, span2start, span2end, index2, &context, infoptr);
                break;
          default:
#ifndef DISTRIBUTION
                /*fprintf (stderr, "%s\n", enftypestring (fullcrumptr->cenftype));*/
                gerror ("retrieveinarea - wrong enftype\n");
#else
		gerror("");
#endif
        }
/*foo("leaving retrieveinarea\n");*/

        return (context);
}

  typecontext *
retrieveinspan (fullcrumptr, spanstart, spanend, index)
  typecuc *fullcrumptr;
  tumbler *spanstart, *spanend;
  INT index;
{
  typedsp offset;
  typecontext *context, *c;
  typecontext *findlastcbcseq();

        clear (&offset, sizeof(offset));
        context = NULL;
        switch (fullcrumptr->cenftype) {
          case GRAN:
                findcbcinspanseq ((typecorecrum*)fullcrumptr, &offset, spanstart, spanend, &context);
                if (tumblercmp (spanend, &fullcrumptr->cwid.dsas[WIDTH]) == GREATER) {
                        c = findlastcbcseq ((typecorecrum*)fullcrumptr);
                        oncontextlistseq (&context, c);
                }
                return (context);
          default:
#ifndef DISTRIBUTION
                gerror ("retrieveinspan - wrong enftype\n");
#else
		gerror("");
#endif
                return (NULL);
        }
}

  typecontext *
findlastcbcseq (fullcrumptr)
  typecorecrum *fullcrumptr;
{
  typecorecrum *ptr, *findleftson();
  typecontext *c, *makecontextfromcbc();
  tumbler offset;
        tumblerclear (&offset);
        for (ptr = fullcrumptr; ptr; ptr = findleftson((typecuc*)ptr)) {
                for (; getrightbro (ptr); ptr = ptr->rightbro)
                        tumbleradd (&offset, &ptr->cwid.dsas[WIDTH], &offset);
                if (ptr->height == 0) {
                        c = makecontextfromcbc ((typecbc*)ptr, (typewid*)&offset);
                        return(c);
                }
        }
#ifndef DISTRIBUTION
	gerror("in findlastcbcseq couldn't find anything \n");
#else
	gerror("");
#endif
	return(NULL);  /* for lint*/
}


/* all findcbc* routines are recursive with depth of
**   recursion == height of enfilade being searched
*/


/* Model-T like retrieves */
  typecrumcontext *
findcbcseqcrum (ptr, offsetptr, address)
  typecorecrum *ptr;
  typedsp *offsetptr;
  tumbler *address;
{ 

  typecrumcontext *createcrumcontext();

        if (!ptr)
#ifndef DISTRIBUTION
                gerror ("findcbcseqcrum called with NULL ptr.");
#else
		gerror("NULL");
#endif
        for (; getrightbro (ptr); ptr = ptr->rightbro) {
                if (whereoncrum (ptr, offsetptr, address, WIDTH) <= THRUME)
                        break;
                dspadd (offsetptr, &ptr->cwid, offsetptr, (INT)ptr->cenftype);
        }
        if (ptr->height != 0) {
                ptr = findleftson ((typecuc*)ptr);
                return (findcbcseqcrum (ptr, offsetptr, address));
        } else {
                return (createcrumcontext (ptr, offsetptr));
        }
}

  typecontext *
findcbcseq (ptr, offsetptr, address)
  typecorecrum *ptr;
  typedsp *offsetptr;
  tumbler *address;
{ 

  typecontext *makecontextfromcbc();
        for (; getrightbro (ptr); ptr = ptr->rightbro) {
                if (whereoncrum (ptr, offsetptr, address, WIDTH) <= THRUME)
                        break;
                dspadd (offsetptr, &ptr->cwid, offsetptr, (INT)ptr->cenftype);
        }
        if (ptr->height != 0) {
                ptr = findleftson ((typecuc*)ptr);
                return (findcbcseq (ptr, offsetptr, address));
        } else {
                return (makecontextfromcbc ((typecbc*)ptr, (typewid*)offsetptr));
        }
}

  typecontext *
findcbcnd (father, offsetptr, address, index)
  typecorecrum *father;
  typewid *offsetptr;
  tumbler *address;
  INT index;
{ 

  typecorecrum *ptr, *findleftson();
  typewid grasp;
  typecontext *retr, *makecontextfromcbc();
  INT cmp;
        if ((cmp = whereoncrum (father, offsetptr, address, index)) < ONMYLEFTBORDER || cmp > THRUME )
                return (NULL);

        if (father->height != 0) {
                prologuend (father, offsetptr, &grasp, (typedsp*)NULL);
                for (ptr = findleftson ((typecuc*)father); ptr; ptr = getrightbro (ptr))
                        if (retr = findcbcnd (ptr, &grasp, address, index))
                                break;
        } else { /* FOUND IT! */
                retr = makecontextfromcbc ((typecbc*)father, offsetptr);
        }
        return (retr);
}

findcbcinarea2d(crumptr,offsetptr,span1start,span1end,index1,span2start,span2end, index2, headptr, infoptr)
  typecorecrum *crumptr;
  typedsp *offsetptr;
  tumbler *span1start, *span1end, *span2start, *span2end;
  typecontext **headptr;
  typebottomcruminfo *infoptr;
  INT index1, index2;
{ 

  typedsp localoffset;
  typecontext *context, *makecontextfromcbc();
  bool crumintersectsarea2d ();
  bool crumqualifies2d();

#ifndef DISTRIBUTION
/*foo(" entering findcbcinarea2d\n");*/
/*footumbler("span1start = ",span1start);*/
/*footumbler("span1end = ",span1end);*/
/*footumbler("span2start = ",span2start);*/
/*footumbler("span2end = ",span2end);*/
/*foodsp("offset = ",offsetptr,crumptr->cenftype);*/
if(infoptr){
fprintf(stderr,"not NULL infoptr versions mumble specialcase 11/27/84 shouldent happen till we try something fancier\n");
gerror("findcbcinarea2d");
}
#else
	if (infoptr)
		gerror("!NULL");
#endif
        for (; crumptr; crumptr = getrightbro (crumptr)) {
                if (!crumqualifies2d (crumptr, offsetptr, span1start, span1end, index1, span2start, span2end, index2, (type2dbottomcruminfo*)infoptr)){
                        continue;
                }
/*foocrum("crumqualifies in findcbcinarea2d\n",crumptr);*/
                if (crumptr->height != 0) {
                        dspadd (offsetptr, &crumptr->cdsp, &localoffset, (INT)crumptr->cenftype);
                        findcbcinarea2d (findleftson ((typecuc*)crumptr), &localoffset, span1start, span1end, index1, span2start, span2end, index2, headptr, infoptr);
                } else {
/*foo("make context in findcbcinarea2d\n");*/
                        context = makecontextfromcbc ((typecbc*)crumptr, (typewid*)offsetptr);
                        incontextlistnd (headptr, context, index1);
                }
        }
	/*contextfree(context);*/
/*foo("leaving findcbcinarea2d\n");*/
}

    bool                  /* 6-28-84 old code*/
crumqualifies2d (crumptr, offset, span1start, span1end, index1, span2start, span2end, index2, infoptr)  /* NOTE in retrieves this is ---40% -- of cpu*/
  typecorecrum *crumptr;
  typedsp *offset;
  tumbler *span1start, *span1end, *span2start, *span2end;
  type2dbottomcruminfo *infoptr;
  INT index1, index2;
{
  INT startcmp, endcmp;
/*foocrum("entering crumqualifies2d\n",crumptr);*/
        if ((crumptr->height == 0) && infoptr && !tumblereq (&infoptr->homedoc, &(((type2dcbc *)crumptr)->c2dinfo.homedoc))){
#ifndef DISTRIBUTION
		fprintf(stderr,"mumble homedoc");
#endif
/*foo("returningfalse case a");*/
                return (FALSE);
	}

        endcmp = iszerotumbler (span1end) ? TOMYRIGHT : whereoncrum (crumptr, offset, span1end, index1);
	if ( endcmp <=/*=*/ ONMYLEFTBORDER){
		return(FALSE);
	}
        startcmp = whereoncrum (crumptr, offset, span1start, index1);
         if( (startcmp > THRUME /*&& endcmp > THRUME*/)){
/*foo("returningfalse case d");*/
                return (FALSE);
	 }

        endcmp = iszerotumbler (span2end) ? TOMYRIGHT : whereoncrum (crumptr, offset, span2end, index2);
        if ( endcmp < ONMYLEFTBORDER)/* <= was < 12/20/84 */{
/*foo("returningfalse case e");*/
		return(FALSE);
	}
        startcmp =  whereoncrum (crumptr, offset, span2start, index2);
         if( (startcmp > THRUME /*&& endcmp > THRUME*/)){
/*foo("returningfalse case f");*/
                return (FALSE);
	 }

/*foo("crumqualifies2d returning TRUE\n");*/
        return (TRUE);
}

findcbcinspanseq (crumptr, offsetptr, spanstart, spanend, headptr)
  typecorecrum *crumptr;
  typewid *offsetptr;
  tumbler *spanstart, *spanend;
  typecontext **headptr;
{ 

  typewid localoffset;
  typecontext *context, *makecontextfromcbc();
  bool crumintersectsspanseq();

        movewisp (offsetptr, &localoffset);
        for (; crumptr;  crumptr = getrightbro (crumptr)) {
                if (!crumintersectsspanseq (crumptr, (tumbler*)&localoffset, spanstart, spanend)) {
                        dspadd (&localoffset, &crumptr->cwid, &localoffset, (INT)crumptr->cenftype);

                        continue;
                }
                if (crumptr->height == 0) {
                        context = makecontextfromcbc ((typecbc*)crumptr, offsetptr);
                        oncontextlistseq (headptr, context);
                } else {
                        findcbcinspanseq(findleftson((typecuc*)crumptr), &localoffset, spanstart, spanend, headptr);
                }
                dspadd (&localoffset, &crumptr->cwid, &localoffset, (INT)crumptr->cenftype);
        }
}

 
/* sets grasp & reach from ptr & offset */
/*  reach may be NULL so that we won't set it */
prologuend (ptr, offset, grasp, reach)
  typecorecrum *ptr;
  typedsp *offset, *grasp, *reach;
{
        dspadd (offset, &ptr->cdsp, grasp, (INT)ptr->cenftype);
        if (reach)
                dspadd (grasp, &ptr->cwid, reach, (INT)ptr->cenftype);
}

#define intervalcmppart1(left,address) cmp = tumblercmp ((address), (left)); if (cmp == LESS) return (TOMYLEFT); else if (cmp == EQUAL) return (ONMYLEFTBORDER);

#define intervalcmppart2(right,address) cmp = tumblercmp ((address), (right)); if (cmp == LESS) return (THRUME); else if (cmp == EQUAL) return (ONMYRIGHTBORDER); else return (TOMYRIGHT);

  INT
whereoncrum (ptr, offset, address, index)/*speed up by subsuming intervalcmp*/
  typecorecrum *ptr;
  typewid *offset;
  tumbler *address;
  register INT index;   /* used when enftype == SPAN or POOM */
{
  tumbler left, right;
  INT cmp;

        switch (ptr->cenftype) {
          case GRAN:
             tumbleradd (&offset->dsas[WIDTH], &ptr->cwid.dsas[WIDTH], &right);
             return (intervalcmp (&offset->dsas[WIDTH], &right, address));
          case SPAN:
          case POOM:
                tumbleradd(&offset->dsas[index],&ptr->cdsp.dsas[index], &left);
	         cmp = tumblercmp(address,&left);
	         if(cmp == LESS){
		     return(TOMYLEFT);
		 }else if(cmp == EQUAL){
		     return(ONMYLEFTBORDER);
		 }
/*		intervalcmppart1(&left,address);*/
                tumbleradd (&left, &ptr->cwid.dsas[index], &right);
	        cmp = tumblercmp(address,&right);
	        if(cmp == LESS){
		    return(THRUME);
		}else if (cmp == EQUAL){
		    return (ONMYRIGHTBORDER);
		}else{
		    return(TOMYRIGHT);
		}
/*		intervalcmppart2(&right,address);*/
/*#ifndef DISTRIBUTION
		gerror("whereoncrum:can't get here\n");
#else
	gerror("");
#endif
*/
/*
                tumbleradd(&offset->dsas[index],&ptr->cdsp.dsas[index], &left);
                tumbleradd (&left, &ptr->cwid.dsas[index], &right);
        	return (intervalcmp (&left, &right, address));
*/
          default:
#ifndef DISTRIBUTION
                gerror ("whereoncrum: bad enftype\n");
#else
		gerror("");
#endif
        }
#ifndef DISTRIBUTION
  gerror("whereoncrum: can't get here either\n");
#else
  gerror("");
#endif
  return(0); /* for lint */
}
/*

intervalcmp (left, right, address)
  tumbler *left, *right, *address;
{
  register INT cmp;

        cmp = tumblercmp (address, left);
        if (cmp == LESS)
                return (TOMYLEFT);
          else if (cmp == EQUAL)
                return (ONMYLEFTBORDER);
        cmp = tumblercmp (address, right);
        if (cmp == LESS)
                return (THRUME);
          else if (cmp == EQUAL)
                return (ONMYRIGHTBORDER);
          else
                return (TOMYRIGHT);
}

*/


  bool
crumintersectsspanseq (crumptr, offsetptr, spanstart, spanend)
  typecorecrum *crumptr;
  tumbler *offsetptr, *spanstart, *spanend;
{
        if (iszerotumbler (&crumptr->cwid.dsas[WIDTH])) {
                return(FALSE);
        }
        return ((whereoncrum (crumptr, (typewid*)offsetptr, spanstart, WIDTH) < ONMYRIGHTBORDER)
                 && (whereoncrum (crumptr, (typewid*)offsetptr, spanend, WIDTH) >/*=*/ ONMYLEFTBORDER));
}
