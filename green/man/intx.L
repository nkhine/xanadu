.TH INTX L "25 August 1988"
.SH NAME
.I intx
\- connect a
.ie t Xanadu\*(Tm
.el Xanadu(tm)
backend daemon to a terminal
.SH SYNOPSIS
\fBintx\fP \fIbackend account\fP
.SH DESCRIPTION
\fIIntx\fP is almost identical to
.IR ints(L).
Instead of connecting to a frontend via a socket, however, \fIintx\fP connects the backend
to the terminal that is attached to \fIstdin\fP. The purpose of this is to allow a
frontend program running on a different machine connected to a network host via a serial
line to connect to a Xanadu backend. 
.SH FILES
.I .backendrc
\- run time switches
.br
\fIbackenddaemon\fP \- Xanadu backend daemon program
.SH SEE ALSO
daemon(L), xlog(L), connect(2), socket(2), ttyname(3)
.SH BUGS
Presently requests to the backend are line bufferred with \fIintx\fP.
