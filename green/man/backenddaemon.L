.TH BACKENDDAEMON L "25 August 1988"
.SH NAME
\fIbackenddaemon\fP \- multiple user
.ie t Xanadu\*(Tm
.el Xanadu(tm)
88.1x backend daemon
.SH TYPICAL USAGE
\fIbackenddaemon\fP is usually spawned when necessary by
.IR ints(L)
or
.IR intx(L),
which will be referred to as ``glue'' programs.
.SH DESCRIPTION
\fIBackenddaemon\fP accepts and responds to requests from up to five frontends through
.IR socket(2)
connections using the \fIXanadu FeBe Protocol 88.1x\fP.
.SS Error Redirection
The first action by the \fIbackenddaemon\fP process is to close \fIstderr\fP and reopen it
as the file \fIbackenderror\fP to avoid disrupting the frontend interaction.
.SS Parameter and Data Structure Files
\fIBackenddaemon\fP attempts to open \fI.backendrc\fP in the
working directory inherited from the process that spawned it, either a glue program
or a shell. This file is used by \fIbackenddaemon\fP, both glue programs, and
.IR xlog(L)
to set various run-time values to other than their defaults with lines of the form
.IP
\fBmetaname =\fP \fIword\fP
.LP
where each program uses a different set of \fBmetaname\fPs, although since they all use the
same routine to read the file, lines not used by a particular program must still be correct.
Lines in \fI.backendrc\fP beginning with ``\fB#\fP'' are ignored.
.PP
\fIBackenddaemon\fP then tries to open \fIenf.enf\fP in the same directory.
\fIEnf.enf\fP contains the data and links from all sessions since it was created.
If this file does not exist or has been obviously corrupted, a new one is created.
.SS Connection to Frontend
After the data structure initialization, \fIbackenddaemon\fP creates a
.IR socket(2)
with AF_INET address format and SOCK_STREAM communications semantics.
This socket is then bound to either the default port 55146 or the port given by
.IP
\fBport =\fP \fIport\-number\fP
.LP
in \fI.backendrc\fP.
The socket address is set to INADDR_ANY so that connections will be accepted from any host in the
domain.
\fIBackenddaemon\fP then waits for frontend connections with
.IR select(2).
It is up to a frontend or a glue program to attach to the socket with
.IR connect(2).
.PP
After a connection is established, \fIbackenddaemon\fP reads a device name from the
socket.  This name should be ``SOCKET`` if frontend interaction is to be through the
socket.
.IR Ints(L)
uses this mechanism.  Otherwise, \fIbackenddaemon\fR will communicate through the named device; see
.IR intx(L)
for an example of this method.
.PP
After the device name is acquired, but before it is used, \fIbackenddaemon\fP expects an
\fBx-account\fP request over the socket to establish the initial account to be used (see
.SM Accounts,
below).
.PP
When the account is set, \fIbackenddaemon\fP switches to the non-socket device if one was
specified, otherwise communication continues to the original socket connection.
.SS Metaprotocol
Next, \fIbackenddaemon\fP conducts a dialog with the frontend to establish that both parties
understand a common protocol.
This ``metaprotocol'' consists of one or more newline characters (i.e., byte value
0x0a) from the frontend
as synchronization; before the newlines, all input is ignored.  After the newlines,
\fIbackenddaemon\fP expects to receive the string:
.br
.IP
.B P0~
.LP
(i.e., byte values 0x50 0x30 0x7e). On seeing this, \fIbackenddaemon\fP responds with:
.IP
.B \\\\nP0~
.LP
(0x0a 0x50 0x30 0x7e) meaning that \fIXanadu FeBe Protocol 88.1x\fP represented by ``0'' is understood.
If \fIbackenddaemon\fP sees any other input after the last newline, it responds with:
.IP
.B \\\\nP?~
.LP
(0x0a 0x50 0x3f 0x7e) meaning that it did not understand the frontend, and then
closes the connection.
.SS Interactive Session
\fIBackenddaemon\fP is now ready to accept \fIXanadu FeBe Protocol 88.1x\fP requests.
The session lasts until the frontend issues a \fBquit\fP request.  After
\fIbackenddaemon\fP
responds with a \fBquit\fP code the frontend may exit.  After the \fBquit\fP command,
\fIBackend\fP closes the connection that the frontend was using.
.SS Accounts
See
.IR backend(L)
for a description of account maintainance.
.SS Edit Logging
\fIBackenddaemon\fP records all edit requests as
.IR backend(L)
does, with the added property that an \fBx-account\fP request is placed in the
log file whenever the requests that may follow are from a different account
than the previous requests.
.SH FILES
.I .backendrc
\- run time switches
.br
\fIbackenderror\fP \- redirected stderr
.br
\fIenf.enf\fP \- the magic data structure file
.br
\fIln?.?.?:?\fP \- the edit log file
.SH SEE ALSO
backend(L), fex(L), ints(L), intx(L), xlog(L), accept(2), bind(2), socket(2)
.br
\fIXanadu FeBe Protocol 88.1x Documentation\fP,
Xanadu Operating Company, August 1988
.SH BUGS
In \fI.backendrc\fP, whitespace is required on both sides of the ``\fB=\fP'' on each line.
.br
When given a device name other than ``SOCKET,'' \fIdaemon\fP does not read lines that are
not terminated with a newline.
.br
See
.IR backend(L)
for more bugs.
.SH NOTES
Since the data structure is easy to corrupt with invalid but unchecked requests, it is
a good idea to occasionally copy \fIenf.enf\fP to a backup file.
.PP
The limit of 5 frontend connections is an arbitrary one, and does not reflect any limitations
in the internal structure of the backend. The
.IR socket(2)
facility was chosen because it was simple enough to not distract from the
development of the more intricate Xanadu hypertext functionality (i.e., it was a quick hack).
