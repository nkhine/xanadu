/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/* split.d - routines to split overfull loaves */

#include "crum.h"

/* Should be called whenever a crum may have too many sons
**  if it does, it is split, and its father is checked, etc...
*/
  bool
splitcrumupwards(father)
  typecuc *father;
{
  bool splitsomething;
        
        splitsomething = FALSE;
        if (father->height <= 0)
#ifndef DISTRIBUTION
                gerror("splitcrumupwards on bottom crum\n");
#else
	gerror("");
#endif
        for (; toomanysons(father); father = (typecuc *)findfather((typecorecrum*)father)) {
                if (isfullcrum((typecorecrum*)father)) {
                        levelpush(father);
                        splitcrum((typecuc*)findleftson(father));
#ifndef DISTRIBUTION
                        fprintf(stderr,"splitcrumupwards split something\n");
asserttreeisok(father);
#endif
                        return(TRUE);
                }
                splitcrum (father);
                splitsomething = TRUE;
        }       
#ifndef DISTRIBUTION
asserttreeisok(father);
#endif
        return(splitsomething);
}

/* splits an individual crum */
splitcrum(father)
  typecuc *father;
{
       switch (father->cenftype) {
          case GRAN:
                splitcrumseq(father);
                break;
          case POOM:
                splitcrumpm(father);
                break;
          case SPAN:
                splitcrumsp(father);
                break;
         default:
#ifndef DISTRIBUTION
                gerror("splitcrum: bad enftype\n");
#else
	gerror("");
#endif
        }
        setwispupwards(father, 0);
}

/* splits a crum for sequential enfilades */
splitcrumseq(father)
  typecuc *father;
{
  typecorecrum *new, *ptr, *next;
  typecorecrum *createcrum();
  INT i, halfsons;

        ivemodified((typecorecrum*)father);
        new = createcrum((INT)father->height, (INT)father->cenftype);
        reserve(new);
        adopt(new, RIGHTBRO, (typecorecrum*)father);
        rejuvinate(new);
        ivemodified(new);
        halfsons = father->numberofsons / 2;
        for (i = 0, ptr = findrightmostson(father); i < halfsons && ptr; ++i, ptr = next) {
                next = findleftbro(ptr);
                disown(ptr);
                adopt(ptr, LEFTMOSTSON, new);
                rejuvinate(ptr);
                ivemodified(ptr);/*  zzz */
        }

        setwispupwards(father, 0);
        setwispupwards((typecuc*)new,0);
}

splitcrumsp(father)
  typecuc *father;
{
  typecorecrum *ptr, *correctone;
  typecorecrum *createcrum();

        for (correctone=ptr=findleftson(father); ptr; ptr = findrightbro(ptr)) {
/*                if (tumblercmp(&ptr->cdsp.dsas[SPANRANGE], &correctone->cdsp.dsas[SPANRANGE]) == GREATER)*/
                if (comparecrumsdiagonally(ptr, correctone) == GREATER)
                        correctone = ptr;
        }
        peelcrumoffnd(correctone);
}

splitcrumpminthiscrum(father)
  typecuc *father;
{
        for(; father; father = (typecuc *)findrightbro((typecorecrum*)father)) {
                while (toomanysons(father)) {
                        splitcrumpm(father);
                }
        }
}

splitcrumpm(father)
  typecuc *father;
{
  typecorecrum *ptr, *correctone;
  typecorecrum *createcrum();

        for(correctone=ptr=findleftson(father);ptr;ptr = findrightbro(ptr)) {
                if (tumblercmp (&ptr->cdsp.dsas[SPANRANGE], &correctone->cdsp.dsas[SPANRANGE]) == GREATER)
               /* if (comparecrumsdiagonally(ptr, correctone) == LESS) */
                        correctone = ptr;
        }
        peelcrumoffnd(correctone);
}
           
peelcrumoffnd(ptr)
  typecorecrum *ptr;
{
  typecuc *father;
  typecorecrum *new;
  INT ofatherage,optrage;
/* fprintf(stderr,"peelcrumoffnd\n");       */
        if (isfullcrum(ptr)){
#ifndef DISTRIBUTION
                gerror("peeloffcurmnd called with fullcrum");
#else
	gerror("");
#endif
	}
        father = findfather(ptr);
	ofatherage = father->age;
	optrage = ptr->age;
	father->age = NEW;
	ptr->age = NEW;
        reserve((typecorecrum*)father);
        father->modified = TRUE;/*an uncle will get ivemodified shortly in 10 lines */
        reserve(ptr);
        disown(ptr);
        new = createcrum((INT)father->height,(INT)father->cenftype);
        adopt(new, RIGHTBRO, (typecorecrum*)father);
        movewisp(&father->cdsp, &new->cdsp);
        adopt(ptr, LEFTMOSTSON, new);
        rejuvinate(new);
        rejuvinate(ptr);
        rejuvinate((typecorecrum*)father);
	if (ofatherage == RESERVED){
		father->age = RESERVED;
	}
	if (optrage == RESERVED){
		ptr->age = RESERVED;
	}
        ivemodified(ptr);
        setwispupwards(father,0);
        setwispupwards((typecuc*)new,0);
        setwispupwards((typecuc*)ptr,1);
}


