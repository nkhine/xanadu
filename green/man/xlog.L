.TH XLOG L "25 August 1988"
.SH NAME
\fIxlog\fP \- entry point for experimental multiple user
.ie t Xanadu\*(Tm
.el Xanadu(tm)
hypertext system
.SH SYNOPSIS
\fBxlog\fP [ \fIuser-name\fP ]
.SH DESCRIPTION
\fIXlog\fP attempts to find a Xanadu
account number based on a user name, if any, and then invokes either
.IR ints(L)
or
.IR intx(L)
with that account number.  If no user name is given, account \fB1.1.0.1\fP is used.
.PP
\fIXlog\fP first reads \fI.backendrc\fP in the current directory. If the line
.IP
\fBbackenddir =\fP \fIpath\fP
.LP
is present, \fIxlog\fP sets \fIpath\fP as its working directory, otherwise the directory is
unchanged.  \fIXlog\fP then prompts for a password (which is not checked), and looks for
the account number for \fIuser-name\fP in either \fIaccountfile\fP in the directory set above,
or in the file named by
.IP
\fBaccountfile =\fP \fIfile-name\fP
.LP
in \fI.backendrc\fP. This file contains lines with the format
.IP
user-name:password:account-number
.LP
for example,
.IP
albert:space-time:1.42.0.23
.LP
.PP
\fIXlog\fP then asks if you want to run the standard frontend (through
.IR ints(L))
or one running on a different machine (connected via
.IR intx(L)).
The appropriate program is then executed, and \fIxlog\fP vanishes.
.SH FILES
\fIaccountfile\fP \- file to map from user names to account numbers
.br
.I .backendrc
\- run time switches
.SH SEE ALSO
daemon(L), fex(L), ints(L), intx(L), execl(2)
.SH BUGS
If the directory is changed by \fI.backendrc\fP, \fIints\fP, \fIintx\fP, and \fIdaemon\fP will
start with that directory, and attempt to read \fI.backendrc\fP from there.  For now, just
keep a copy wherever it may be used.
