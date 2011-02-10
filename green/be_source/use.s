	.data
	.comm 	_debug,4
	.comm 	_granf,4
	.comm 	_spanf,4
	.comm 	_GCreque,160
	.comm 	_BCgrimr,4
	.text
	.globl  _clear
_clear:
	movl 	#.L25,a0
	jbsr 	mcount
	.data
.L25:
	.long 0
	.text
	link 	a6,#-.F1
	movl 	a5,sp@-
	movl 	d7,sp@-
	movl 	a6@(8),a5
	movl 	a6@(12),d7
	tstl 	d7
	jeq 	.L26
	subql 	#1,d7
.L20001:
	clrb 	a5@+
	dbra 	d7,.L20001
.L26:
	movl 	sp@+,d7
	movl 	sp@+,a5
	unlk 	a6
	rts
.F1 = 8
.S1 = 0x2080
| end
	.text
	.globl  _setmem
_setmem:
	movl 	#.L30,a0
	jbsr 	mcount
	.data
.L30:
	.long 0
	.text
	link 	a6,#-.F2
	moveml  #.S2,a6@(-.F2)
	movl 	a6@(8),a5
	movl 	a6@(12),d7
	movb 	a6@(19),d6
	tstl 	d7
	jeq 	.L32
	subql 	#1,d7
.L20003:
	movb 	d6,a5@+
	dbra 	d7,.L20003
.L32:
	moveml  a6@(-.F2),#0x20c0
	unlk 	a6
	rts
.F2 = 12
.S2 = 0x20c0
| end
	.text
	.globl  _movmem
|movmem (source, dest, count)
|  register char *source,*dest;
|  register unsigned count;
|{
| 	 if (source > dest){
| 		 while (count--)
| 			 *dest++ = *source++;
| 	 }
| 	 else {
| 		 source += count;
| 		 dest += count;
| 		 while (count--)
| 			 *--dest = *--source;
| 	 }
|}
_movmem:
	movl 	#.L35,a0
	jbsr 	mcount
	.data
.L35:
	.long 0
	.text
	link 	a6,#-.F3
	moveml  #.S3,a6@(-.F3)
	movl 	a6@(8),a5
	movl 	a6@(12),a4
	movl 	a6@(16),d7
	tstl 	d7
	jeq 	.L34
| 	 cmpl 	 a5,a4
| 	 jcc 	 .L36
	subql 	#1,d7
.L37:
	movb 	a5@+,a4@+
	dbra 	d7,.L37
| 	 jra 	 .L34
|.L36:
| 	 subql   #1,d7
| 	 movl 	 d7,d0
| 	 addl 	 d0,a5
| 	 movl 	 d7,d0
| 	 addl 	 d0,a4
|.L40:
| 	 movb 	 a5@-,a4@-
| 	 dbra 	 d7,.L40
.L34:
	moveml  a6@(-.F3),#0x3080
	unlk 	a6
	rts
.F3 = 12
.S3 = 0x3080
| end
	.data
