.TH INTF L "25 August 1988"
.SS NAME
\fIintf\fP \- connect a frontend to the single-user
.ie t Xanadu\*(Tm
.el Xanadu(tm)
88.1x backend
.SS SYNOPSIS
\fBintf\fP \fIbackend frontend\fP
.SS DESCRIPTION
\fIIntf\fP uses the
.IR pipe(2)
facility to connect a frontend program to the standard I/O of a program
such as
.IR backend(L).
.PP
Two pipes are created by \fIintf\fP, one from the frontend to the backend,
the other in the opposite direction.
One end of each pipe is attached to \fIstdin\fP and \fIstdout\fP.
\fIIntf\fP then forks and executes the backend program with these pipes
as its standard I/O. 
The original branch of the fork then executes the frontend program with
the file descriptor numbers for the other ends of the pipes passed as command
line arguments in the form
.IP
\fBfrontend\fP \fIpipe-from-backend pipe-to-backend\fP
.LP
so that the frontend can communicate with the backend through them.
\fIIntf\fP does not exit if successul, turning into the frontend instead.
.SS SEE ALSO
backend(L), frontend(L), execl(2), fork(2), pipe(2)
