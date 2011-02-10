
        .text
        .globl  call, callc, calle, cerror
call:
        movl    a0,sp@-
        movl    d0,sp@-
        jsr     _cntcall        |       cntcall (call, address)
        movl    sp@+,d0
        movl    sp@+,a0
        trap    #0
        bcs     cerror
        rts

callc:
        movl    a0,sp@-
        movl    d0,sp@-
        jsr     _cntcall        |  cntcall (call, address)
        movl    sp@+,d0
        movl    sp@+,a0
        trap    #0
        bcs     cerror
        clrl    d0
        rts

calle:
        movl    a0,sp@-
        movl    d0,sp@-
        jsr     _cntcall        |  cntcall (call, address)
        movl    sp@+,d0
        movl    sp@+,a0
        trap    #0

cerror:
        movl    d0,_errno
        moveq   #-1,d0
        rts
|end
