.TH INTS L "25 August 1988"
.SH NAME
\fIints\fP \- connect a frontend to a
.ie t Xanadu\*(Tm
.el Xanadu(tm)
backend daemon
.SH SYNOPSIS
\fBints\fP \fIfrontend account\fP
.SH DESCRIPTION
\fIInts\fP uses the
.IR socket(2)
facility to connect a frontend program to
.IR daemon(L).
.PP
\fIInts\fP first reads the file \fI.backendrc\fP in the current directory.  If the line
.IP
\fBbackenddir =\fP \fIpath\fP
.LP
is present, \fIints\fP sets \fIpath\fP as its working directory, otherwise the directory is
unchanged.
.PP
Next, \fIints\fP acquires a socket with AF_INET address format and SOCK_STREAM
semantics\-\-the same form as
.IR daemon(L).
\fIInts\fP then binds the socket to a specific port and host. The default port is \fB55146\fP
and the default host is \fBlocalhost\fP, although these can be changed in \fI.backendrc\fP with
.IP
\fBport =\fP \fIport-number\fP
.LP
and
.IP
\fBhost =\fP \fIhost-name\fP
.LP
.IR Connect(2)
is then called with this binding. If the connection is not made, \fIints\fP spawns a daemon on
the local host machine with
.IR fork(2)
and
.IR execl(2).
The default daemon is \fIbackenddaemon\fP in the directory set above, but this may be changed
with
.IP
\fBbackend =\fP \fIdaemon-name\fP
.LP
in \fI.backendrc\fP.
\fIInts\fP then makes a second attempt to connect after a 15 second wait. A second failure results in
termination.
.PP
Once the connection is acquired, \fIints\fP sends the string ``SOCKET'' to \fIdaemon\fP
to indicate that the socket connection will be used by the frontend.  \fIInts\fP then sends
an \fBx-account\fP request to \fIdaemon\fP with the account number from the command line.
.PP
Finally, \fIints\fP executes the frontend program.  The default frontend is \fIfex\fP in the
directory set above, but may be changed with
.IP
\fBfrontend =\fP \fIfrontend-name\fP
.LP
in \fI.backendrc\fP.
The file descriptor numbers for the socket are passed as command line arguments
in the form
.IP
\fBfrontend\fP \fIfrom-backend to-backend\fP
.LP
so that the frontend can communicate with the backend through them.
\fIInts\fP does not exit if it is successful.
.SH FILES
.I .backendrc
\- run time switches
.br
\fIbackenddaemon\fP \- Xanadu backend daemon program
.br
\fIfex\fP \- frontend program
.SH SEE ALSO
daemon(L), fex(L), bind(2), connect(2), socket(2)
.br
\fIXanadu FeBe Protocol 88.1x Documentation\fP,
Xanadu Operating Company, August 1988
.SH BUGS
If you are trying to connect to a backend daemon on another machine, it will not
help for \fIints\fP to spawn one on the local machine, as the second connection attempt
will still fail.
