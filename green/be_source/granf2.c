/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* granf2.d - granfilade interface routines */

#include "xanadu.h"
#include "enf.h"

#ifdef DISTRIBUTION
char granf2err[] = "g2error\n";
#endif
static findisatoinsertnonmolecule(); 
static klugefindisatoinsertnonmolecule();
static findisatoinsertmolecule();


  typeorgl
fetchorglgr(taskptr, fullcrumptr, address)
  typetask *taskptr;
  typegranf fullcrumptr;
  typeisa *address;
{
  typecrumcontext *context, *retrievecrums();
  typecuc *ret;

#ifndef DISTRIBUTION
if (debug) {fprintf(stderr,"fetchorglgr ");dumptumbler(address);fprintf(stderr,"\n");}
#endif

	if (tumblercmp (&((typecuc*)fullcrumptr)->cwid.dsas[WIDTH], address) == LESS)
		return (NULL);

	if ((context = retrievecrums ((typecuc*)fullcrumptr, address,  WIDTH)) == NULL)
		return NULL;

	if (!tumblereq((tumbler*)&context->totaloffset, address)) {
		crumcontextfree(context);   
		return (NULL);  
	}

	if (!context->corecrum->cinfo.granstuff.orglstuff.orglptr
	   && context->corecrum->cinfo.granstuff.orglstuff.diskorglptr.diskblocknumber == DISKPTRNULL) {
#ifndef DISTRIBUTION
		gerror ("No orgl core ptr when diskptr is null.\n");
#else
		gerror(granf2err);
#endif
	}

	if (context->corecrum->cinfo.infotype == GRANORGL) {
		if (!context->corecrum->cinfo.granstuff.orglstuff.orglincore) {
			if (context->corecrum->cinfo.granstuff.orglstuff.diskorglptr.diskblocknumber == DISKPTRNULL){
#ifndef DISTRIBUTION
				gerror ("fetchorglgr null diskorglptr\n");
#else
				gerror(granf2err);
#endif
			}
			inorgl (context->corecrum);
		}
		ret = context->corecrum->cinfo.granstuff.orglstuff.orglptr;
		if (!ret) {
#ifndef DISTRIBUTION
			gerror ("fetchorglgr null orglptr\n");
#else
			gerror(granf2err);
#endif
		}
	} else {
#ifndef DISTRIBUTION
		dump (context->corecrum);
		qerror ("I should have found an orgl in fetchorglgr\n");
#else
		gerror(granf2err);
#endif
	}
	crumcontextfree (context);
	rejuvinate ((typecorecrum*)ret);
	return ((typeorgl)ret);
}

  bool
inserttextgr (taskptr, fullcrumptr, hintptr, textset, ispansetptr)
  typetask *taskptr;
  typegranf fullcrumptr;
  typehint *hintptr;
  typetextset textset;
  typeispanset *ispansetptr;
{
  tumbler lsa, spanorigin;
  typegranbottomcruminfo locinfo;
  typeispan *ispanptr;
  bool findisatoinsertgr();
  INT *taskalloc();


	if (!findisatoinsertgr ((typecuc*)fullcrumptr, hintptr, &lsa))
		return (FALSE);
	movetumbler (&lsa, &spanorigin);
	for (; textset; textset = textset->next) {
		locinfo.infotype = GRANTEXT;
		locinfo.granstuff.textstuff.textlength = textset->length;
		movmem(textset->string,locinfo.granstuff.textstuff.textstring, locinfo.granstuff.textstuff.textlength);
		insertseq ((typecuc*)fullcrumptr, &lsa, &locinfo);
		tumblerincrement (&lsa, 0, textset->length, &lsa);
	}
	ispanptr = (typeispan *) taskalloc (taskptr, sizeof(typeispan));
	ispanptr->itemid = ISPANID;
	ispanptr->next = NULL;
	movetumbler (&spanorigin, &ispanptr->stream);
	tumblersub (&lsa, &spanorigin, &ispanptr->width);
	*ispansetptr = ispanptr;
	return (TRUE);
}

  bool
createorglgr (taskptr, fullcrumptr, hintptr, isaptr)
  typetask *taskptr;
  typegranf fullcrumptr;
  typehint *hintptr;
  typeisa *isaptr;
{
  typegranbottomcruminfo locinfo;
  bool findisatoinsertgr();
  typecuc *createenf();

	  if (!findisatoinsertgr ((typecuc*)fullcrumptr, hintptr, isaptr))
		  return (FALSE);
	  locinfo.infotype = GRANORGL;
	  locinfo.granstuff.orglstuff.orglptr = createenf (POOM);
	reserve ((typecorecrum*)locinfo.granstuff.orglstuff.orglptr);
	  locinfo.granstuff.orglstuff.orglincore = TRUE;
	  locinfo.granstuff.orglstuff.diskorglptr.diskblocknumber = DISKPTRNULL;
	  locinfo.granstuff.orglstuff.diskorglptr.insidediskblocknumber = 0;
	  insertseq ((typecuc*)fullcrumptr, isaptr, &locinfo);
	rejuvinate ((typecorecrum*)locinfo.granstuff.orglstuff.orglptr);
	  return (TRUE);
}

  bool
findisatoinsertgr (fullcrumptr, hintptr, isaptr)
  typecuc *fullcrumptr;
  typehint *hintptr;
  typeisa *isaptr;
{
  bool isaexistsgr();

	/*  isaexistsgr (fullcrumptr, &hintptr->hintisa);*/
	  if (!isaexistsgr (fullcrumptr, &hintptr->hintisa)) {
		  if(hintptr->subtype != ATOM){

		  klugefindisatoinsertnonmolecule(fullcrumptr, hintptr, isaptr);
		tumblerjustify(isaptr);
		  return(TRUE);
		  }
#ifndef DISTRIBUTION
		  fprintf (stderr,"nothing at hintisa\n");
#endif
		  return (FALSE);
	  }
	
	  if (hintptr->subtype == ATOM)
		  findisatoinsertmolecule (fullcrumptr, hintptr, isaptr);
	  else
		  findisatoinsertnonmolecule (fullcrumptr, hintptr, isaptr);
	tumblerjustify(isaptr);
	  return (TRUE);
}

static findisatoinsertmolecule (fullcrumptr, hintptr, isaptr)
  typecuc *fullcrumptr;
  typehint *hintptr;
  typeisa *isaptr;
{
  typeisa upperbound, lowerbound;

	tumblerincrement (&hintptr->hintisa, 2, hintptr->atomtype + 1, &upperbound);
	clear (&lowerbound, sizeof(lowerbound));
	findpreviousisagr ((typecorecrum*)fullcrumptr, &upperbound, &lowerbound);
	if (tumblerlength (&hintptr->hintisa) == tumblerlength (&lowerbound)) {
		tumblerincrement (&lowerbound, 2, hintptr->atomtype, isaptr);
		tumblerincrement (isaptr, 1, 1, isaptr);
	} else if (hintptr->atomtype == TEXTATOM) {
			tumblerincrement (&lowerbound, 0, 1, isaptr);
	} else if (hintptr->atomtype == LINKATOM) {
		tumblerincrement (&hintptr->hintisa, 2, 2, isaptr);
		if (tumblercmp (&lowerbound, isaptr) == LESS)
			tumblerincrement (isaptr, 1, 1, isaptr);
		else
			tumblerincrement (&lowerbound , 0, 1, isaptr);
	}
#ifndef DISTRIBUTION
	else
		gerror ("findisatoinsertmoleculegr\n");
#endif
}

static klugefindisatoinsertnonmolecule (fullcrumptr, hintptr, isaptr)
  typecuc *fullcrumptr;
  typehint *hintptr;
  typeisa *isaptr;
{
/*  typeisa upperbound, lowerbound;
  INT depth, hintlength;

	depth = hintptr->supertype == hintptr->subtype ? 1 : 2;
	hintlength = tumblerlength (&hintptr->hintisa);
	tumblerincrement (&hintptr->hintisa, depth - 1, 1, &upperbound);
	clear (&lowerbound, sizeof(lowerbound));
	findpreviousisagr (fullcrumptr, &upperbound, &lowerbound);
	tumblertruncate (&lowerbound, hintlength + depth, isaptr);
        tumblerincrement(isaptr,tumblerlength(isaptr)==hintlength?depth:0,1,isaptr);
*/
#ifdef UnDeFIned
	tumblercopy(/*&*/hintptr/*->hintisa*/,isaptr); /* ECH 8-30-88 was hintptr, not &hintptr->hintisa */
#endif
	tumblercopy(&hintptr->hintisa,isaptr);

}

static findisatoinsertnonmolecule (fullcrumptr, hintptr, isaptr)
  typecuc *fullcrumptr;
  typehint *hintptr;
  typeisa *isaptr;
{
  typeisa upperbound, lowerbound;
  INT depth, hintlength;

	depth = hintptr->supertype == hintptr->subtype ? 1 : 2;

	hintlength = tumblerlength (&hintptr->hintisa);

	tumblerincrement (&hintptr->hintisa, depth - 1, 1, &upperbound);

	clear (&lowerbound, sizeof(lowerbound));

	findpreviousisagr ((typecorecrum*)fullcrumptr, &upperbound, &lowerbound);

	tumblertruncate (&lowerbound, hintlength + depth, isaptr);

   tumblerincrement(isaptr,tumblerlength(isaptr)==hintlength?depth:0,1,isaptr);
}

  bool
isaexistsgr (crumptr, isaptr)
  typecuc *crumptr;
  typeisa *isaptr;
{
  typecontext *context, *retrieve();
  bool ret;

	  context = retrieve (crumptr, isaptr,  WIDTH);
	  ret = tumblereq ((tumbler*)&context->totaloffset, isaptr);
	  contextfree (context);
	  return (ret);
}

findpreviousisagr (crumptr, upperbound, offset)
  typecorecrum *crumptr;
  typeisa *upperbound, *offset;
{ RECURSIVE    /* findpreviousisagr*/
  INT tmp;
  typecorecrum *ptr, *findleftson();

  /*zzz?      if (!offset)
		tumblerclear (offset);
*/
	if (crumptr->height == 0) {
		findlastisaincbcgr ((typecbc*)crumptr, offset);
		return;
	}
	for (ptr = findleftson((typecuc*)crumptr); ptr; ptr = findrightbro(ptr)) {
		if (
		 (tmp= whereoncrum (ptr, (typewid*)offset, upperbound, WIDTH)) == THRUME
		|| tmp == /*ONMYLEFTBORDER*/ONMYRIGHTBORDER
		|| !ptr->rightbro) {
			findpreviousisagr (ptr, upperbound, offset);
			return;
		} else {
			tumbleradd(offset, &ptr->cwid.dsas[WIDTH], offset);
		}
	}
}

findlastisaincbcgr (ptr, offset)
  typecbc *ptr;
  typeisa *offset;   /* offset is last isa if non-text or one char */
{
	if (ptr->cinfo.infotype == GRANTEXT)
		tumblerincrement (offset, 0, (INT) ptr->cinfo.granstuff.textstuff.textlength - 1, offset);
}

  typevstuffset *
ispan2vstuffset (taskptr, fullcrumptr, ispanptr, vstuffsetptr)
  typetask *taskptr;
  typegranf fullcrumptr;
  typeispan *ispanptr;
  typevstuffset *vstuffsetptr;
{
  typevstuffset vstuffset;
  typeisa lowerbound, upperbound;
  typecontext *context, *temp;
  typecontext  *retrieveinspan();
  bool context2vstuff();

	*vstuffsetptr = NULL;
	movetumbler (&ispanptr->stream, &lowerbound);
	tumbleradd(&lowerbound, &ispanptr->width, &upperbound);
	context = retrieveinspan ((typecuc*)fullcrumptr, &lowerbound, &upperbound, WIDTH);
#ifndef DISTRIBUTION
foocontextlist ("retrieveinspan returning\n", context);
#endif

	for (temp = context; temp; temp = temp->nextcontext) {
#ifndef DISTRIBUTION
foocontext ("passing context temp =",temp);
#endif
		if (context2vstuff (taskptr, temp, ispanptr, &vstuffset)) {
#ifndef DISTRIBUTION
foohex("vstuffsetptr = ",vstuffsetptr);
foohex("vstuffset = ", vstuffset);
foohex("&vstuffset->next = ", &((typeitemheader *)vstuffset)->next);
#endif
			*vstuffsetptr = vstuffset;
			vstuffsetptr = (typevstuffset *)&((typeitemheader *)vstuffset)->next;
		}
	}
	contextfree (context);
	return (vstuffsetptr);
}

