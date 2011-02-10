/* Copyright © 1979-1999 Udanax.com. All rights reserved.

* This code is licensed under the terms of The Udanax Open-Source License, 
* which contains precisely the terms of the X11 License.  The full text of 
* The Udanax Open-Source License can be found in the distribution in the file 
* license.html.  If this file is absent, a copy can be found at 
* http://udanax.xanadu.com/license.html and http://www.udanax.com/license.html
*/
/*
** A structure is maintained with information on each player.
*/

typedef struct _player {
        char    *name;          /* Player's name        */
        INT     userid;         /* player's user id number      */
        INT     wantsout;       /* Quit after this hand?        */
        INT     socket;         /* To communicate with player on*/
        FILE    *inp;
        FILE    *outp;
	tumbler account; 	/* xanadu host and account tumbler */
        } PLAYER;

extern PLAYER  player[];

extern INT     n_players;      /* current number of players (including computer) */

extern  void    *new_players(),*leave(),*crash();

extern INT fdtoplayer[32];
extern INT inputfds ;
extern INT nfds ;
extern INT user;
