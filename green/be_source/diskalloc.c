/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/

#include "crum.h"
#include "coredisk.h"

INT enffiledes;
extern bool isxumain;
bool maximumsetupsizehasbeenhit = FALSE;
INT maximumsetupsize = 0;

char mask[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };

  INT
isalloced(n)
  INT n;
{
	return (diskheader.bitmap[n/8]|mask[n%8]);
}
  typediskloafptr
diskalloc ()
/*  Find an unallocated loaf on the the disk,
*       allocate it (take it off the free list), and
*       return something thru which I can deal with it.
*/
{
  INT i, j;
  typediskloafptr ret;

	for (i = 0; i < BITMAPSIZE && !diskheader.bitmap[i];
		++i);
	if (i == BITMAPSIZE)
		gerror ("Out of diskalloc bitmap space.\n");

	for (j = 0; j < 8; ++j) {
		if (diskheader.bitmap[i] & mask[j]) {
			diskheader.bitmap[i] = diskheader.bitmap[i] & ~mask[j];
			break;
		}
	}
	ret.diskblocknumber = i*8+j;
	ret.insidediskblocknumber = 0/*1*/;
	if (maximumsetupsize && ret.diskblocknumber >= maximumsetupsize){
		maximumsetupsizehasbeenhit = TRUE;
	}
/*fprintf(stderr,"diskalloc returns %d:%d\n",ret.diskblocknumber, ret.insidediskblocknumber);*/
	return (ret);
}

/* Return the named piece of disk to free space,
*       i.e. de-allocate it
*/
diskfree (loafptr)
  typediskloafptrdigit loafptr;
{
  bool goodblock();

	if (loafptr == DISKPTRNULL)
		return;
	if (!goodblock (loafptr)) {
		gerror ("Unallocated block in diskfree.\n");
	}
	diskheader.bitmap[loafptr/8] = diskheader.bitmap[loafptr/8] | mask[loafptr%8];
}

diskset (loafptr)
  typediskloafptrdigit loafptr;
{
  bool goodblock();

	if (loafptr == DISKPTRNULL)
		return;
	if (!goodblock (loafptr)) {
		gerror ("Unallocated block in diskfree.\n");
	}
	diskheader.bitmap[loafptr/8] = diskheader.bitmap[loafptr/8] &~ mask[loafptr%8];
}

  bool /* FALSE is not good enfilade file */
readallocinfo (fd)
  INT fd;
{
  bool ret;

	if (lseek (fd, 0L, 0) < 0) {
		perror("lseek in readallocinfo");
		gerror("lseek failed\n");
	}
	if (read (fd, (char *)&diskheader, sizeof(diskheader)) <= 0) {
		perror("read");
		gerror("read\n");
	}
	ret = TRUE;
	if (!(ret = diskheader.hasenftops)) {
		fprintf (stderr, "Old enffile invalid... re-initializing\n");
		initheader ();
		ret = FALSE;
	}else{
		enffiledes = fd;
		readpartialdiskalloctablefromdisk();
	}
	return (ret);
}

initheader ()
{
	setmem (diskheader.bitmap, sizeof (diskheader.bitmap), 0xff);   /* free all */
	clear (diskheader.bitmap, (NUMDISKLOAFSINHEADER+3)/8+1); 
				/* unfree bitmap and granf and spanf*/
				/* zzzz this rounds up a byte thus may reserve
				   unused blocks  fix by clearing bits ! */

	diskheader.hasenftops = FALSE;
	initincorealloctables();
}

diskallocexit (fd)
  INT fd;
{
	diskheader.hasenftops = TRUE;
	savepartialdiskalloctabletodisk();
	writeallocinfo (fd);
}

writeallocinfo (fd)
  INT fd;
{
	if (lseek (fd, 0L, 0) < 0) {
		perror("lseek in writeallocinfo");
		gerror("lseek failed");
	}
	if (write (fd, (char*)&diskheader, sizeof(diskheader)) == -1) {
		perror("write in writeallocinfo");
		gerror("write failed");
	}
}
/*
  bool
isalloced (loafptr)
  typediskloafptr loafptr;
{
	return (!(diskheader.bitmap[loafptr/8] & mask[loafptr%8]));
}
*/
  bool
goodblock (diskptr)
  typediskloafptrdigit diskptr;
{
	if (diskptr == 0) return(FALSE);
	return ( ! (diskheader.bitmap[diskptr/8] & mask[diskptr%8]));
}


