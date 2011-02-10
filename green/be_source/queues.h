/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/*

	Queue definitions

*/


/*  General purpose queue  */

struct queue {
	struct queue *qnext,	   /* Next item in queue */
		     *qprev;	   /* Previous item in queue */
};

/*  Queue macros  */

#define qempty(x) (qnext((struct queue *)(x),(struct queue *)(x))==NULL)

/*  Queue functions  */

void qinit(), qinsert(), qpush();
int qlength(), qvalid();
struct queue *qremove(), *qnext(), *qdchain();
