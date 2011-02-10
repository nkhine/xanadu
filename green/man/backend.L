.TH BACKEND L "25 August 1988"
.SH NAME
\fIbackend\fP \- single user
.ie t Xanadu\*(Tm
.el Xanadu(tm)
88.1x backend
.SH SYNOPSIS
\fBbackend\fP
.SH TYPICAL USAGE
intf \fIbackend\fP fex
.SH DESCRIPTION
\fIBackend\fP accepts requests from a frontend, such as
.IR fex(L),
through \fIstdin\fP and responds through
\fIstdout\fP according to the \fIXanadu FeBe Protocol 88.1x\fP.
.SS Metaprotocol
Initially \fIbackend\fP conducts a dialogue with the frontend to establish that both parties
understand a common protocol.
This ``metaprotocol'' consists of one or more newline characters (i.e., byte
value 0x0a) from the frontend
as synchronization; before the newlines, all input is ignored.  After the newlines, \fIbackend\fP
expects to receive the string:
.IP
\fBP0~\fP
.LP
(i.e., byte values 0x50 0x30 0x7e). On seeing this, \fIbackend\fP responds with:
.IP
.B \\\\nP0~
.LP
(0x0a 0x50 0x30 0x7e) meaning that \fIXanadu FeBe Protocol 88.1x\fP represented by ``0'' is understood.
If \fIbackend\fP sees any other input after the last newline, it responds with:
.IP
.B \\\\nP?~
.LP
(0x0a 0x50 0x3f 0x7e) meaning that it did not understand the frontend, and then exits.
.SS Error Redirection
After successful completion of the metaprotocol dialogue, \fIbackend\fP closes \fIstderr\fP
and reopens it with the file \fIbackenderror\fP to avoid disrupting the frontend
interaction.
.SS Data Structure File
\fIBackend\fP then attempts to open the file \fIenf.enf\fP in the current working directory.
This file contains the data and links from all sessions since
it was created.  If this file does not exist or has been obviously corrupted, a new one is created.
.SS Interactive Session
\fIBackend\fP is now ready to accept \fIXanadu FeBe Protocol 88.1x\fP requests.
The session lasts until the frontend issues a \fBquit\fP request.  After \fIbackend\fP
responds with a \fBquit\fP code the frontend may exit.   After sending the
\fBquit\fP code, \fIbackend\fP
updates \fIenf.enf\fP and closes it before itself exiting.
.SS Accounts
When \fIbackend\fP starts accepting requests, the working user account is set to
the default account 1.1.0.1, under which new documents and versions will be created
until the working user account is changed with the \fBx-account\fP request.
.PP
Note that if the session is using a new enfilade file, neither 1.1.0.1 nor any other
account will exist, and must be created before any documents may be created.
To create the account 1.1.0.1, first create the node 1.1 with:
.IP
\fBcreate-node-or-account 0.1.1\fP
.LP
and then create account 1.1.0.1 with:
.IP
\fBcreate-node-or-account 0.1.1.0.1\fP
.LP
.SS Edit Logging
\fIBackend\fP records all edit requests in a file called:
.IP
\fBln\fIm.d.h:m\fP
.LP
where \fIm\fP, \fId\fP, \fIh\fP, and \fIm\fP are the month, day, hour, and
minute, respectively that the backend began execution.  Each line in this
file is a single backend request.  This format is possible since the \fIXanadu FeBe Protocol 88.1x\fP
terminator character `~' and a newline are accepted as identical by \fIbackend\fP,
and multiple delimiters before a new request are ignored (treated as null requests).
The file is produced so that edit operations made during any session may be
reproduced later should \fIbackend\fP fail, or for whatever other reason may
arise.
If you want to redo the operations recorded in an individual log file,
edit that file so that it has at least one blank line followed by ``\fBP0~\fP''
at the top, then invoke \fIbackend\fP as:
.IP
\fBbackend <\fIlogfile\fP
.LP
where \fIlogfile\fP is the name of the appropriate edit log.  The edit
described above is necessary since the metaprotocol does not appear in the
log.
.PP
For reconstructing entire enfilades from edit logs, it is useful to concatenate
the relevant logs into one file.  To do this, first append the files
together with
.IR cat(1),
or some other utility.  Then edit the resulting file to remove all \fBquit\fP
requests (\fB16~\fP) except at the last line of the file.  This new file may
then be used to execute the contained requests in the manner described above.
.SH FILES
\fIbackenderror\fP \- redirected stderr
.br
\fIenf.enf\fP \- the magic data structure file
.br
\fIln?.?.?:?\fP \- the edit log file
.SH SEE ALSO
fex(L), intf(L)
.br
\fIXanadu FeBe Protocol 88.1x Documentation\fP,
Xanadu Operating Company, August 1988
.SH BUGS
Virtually no sanity checking is performed on the input.  There are many possible requests,
particularly edit operations on non-existent addresses and creating documents in non-existent
accounts that are likely to corrupt the data structure.
.br
Numeric inputs to the backend (i.e., tumbler digits, sizes and counts) are
currently limited to values representable by 32 bit unsigned integers.
.br
Tumblers input to the backend are allowed a maximum depth of 11 tumbler
digits from the the last leading zero digit.
.br
Text items for \fBinsert\fP may be no more than 950 bytes long; larger insertions must be broken
into multiple text items.  Longer items are likely to cause the program to fail (see previous bug).
.br
Node numbers are meaningless other than as a prefix for account numbers.
.br
The data file \fIenf.enf\fP has a size limit around 400K disk blocks because of a kluged,
fixed size allocation table.
.br
etc...
.SH NOTE
Since the data structure is easy to corrupt with invalid, but unchecked, requests it is
a good idea to occasionally copy \fIenf.enf\fP to a backup file.
