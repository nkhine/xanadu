/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/

#include "xanadu.h"
#include "enf.h"
#include "coredisk.h"

INT maxthingies;
INT numbytesinloaf;
INT numdiskblocksinloaf;
INT numdiskloafsinheader;     

initmagicktricks ()
{
  bool initenffile();
  void initkluge(), initgrimreaper();

	/* for debugging with adb or dbx*/
	varcrums = TRUE;
	numbytesinloaf = NUMBYTESINLOAF;
	numdiskblocksinloaf = NUMDISKBLOCKSINLOAF;
	maxthingies = MAXTHINGIESINLOAF;
	numdiskloafsinheader = NUMDISKLOAFSINHEADER;
/*fprintf(stderr,"numbytesinloaf = %d numdiskblocksinloaf = %d maxthingies = %d numdiskloafsinheader = %d\n",numbytesinloaf,numdiskblocksinloaf,maxthingies,numdiskloafsinheader);

//fprintf(stderr,"sizeof typediskloaf = %d sizeof typeuberdiskloaf = %d sizeof typeuberrawdiskloaf = %d \n", sizeof(typediskloaf), sizeof(typeuberdiskloaf),sizeof(typeuberrawdiskloaf));//

fprintf(stderr,"sizeof(typeducloaf) = %d sizeof(typedbcloaf) = %d sizeof(type2ddbcloaf) = %d\n"
,sizeof(typeducloaf) , sizeof(typedbcloaf) , sizeof(type2ddbcloaf) );
*/
	initgrimreaper();
	/**inithash();*/
	initqueues();
	clear(&ZEROTUMBLERvar,sizeof(tumbler));
	if (initenffile()) {
		initkluge ((typecuc**)&granf, (typecuc**)&spanf);
	} else {
		granf = (typegranf) createenf (GRAN);
		spanf = (typespanf) createenf (SPAN);
	}
}
