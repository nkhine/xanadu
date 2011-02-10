.TH XUMAIN L "25 August 1988"
.SH NAME
\fIxumain\fP \- interactive stand alone
.ie t Xanadu\*(Tm
.el Xanadu(tm)
backend with awful numerical interface
.SH SYNOPSIS
\fBxumain\fP [ < \fIsetup\-script\fP ]
.SH DESCRIPTION
\fIXumain\fP is a quasi-interactive stand alone version of
.IR backend(L),
which does not use the \fIXanadu FeBe Protocol 88.1x\fP syntax, instead prompting for input to most requests.
\fIXumain\fP does, however, have enough similarities to \fIXanadu FeBe Protocol 88.1x\fP for the document
to be a useful reference.
.PP
\fIXumain\fP is the original Xanadu
backend, written before any frontend, and was intentionally
left as a user-hostile program to encourage us to really write a frontend.
The program continues to exist principally as a tool to create Xanadu
documents from text files
using the \fBsource-unix-command\fP command, which should really be a function of some frontend.
.SS Spec-sets
Spec-sets, spans, and v-spans are handled differently by \fIxumain\fP, with prompts for yes/no
responses to determine which type to use; some of the prompts are context dependent.
Tumblers are also different: \fIxumain\fP requires that the leading zeros of a tumbler are
explicitly given, rather than with an exponent.
All the prompts below that end with ``\fB=>\fP'' require a tumbler according to the \fIXanadu FeBe Protocol 88.1x\fP.
When a spec set is expected the user is prompted with
.IP
\fBany spans or vspecs? (y/n)\fP
.LP
where span has its usual meaning and vspec is a document number with a set of v-spans.
The spec set is terminated with a negative response to this prompt.  A positive response is
followed by the prompt
.IP
a span? (y/n)
.LP
This is asking whether you want a span or a collection of v-spans within a document.  A `yes'
answer elicits a prompt for a span:
.IP
enter span
       start=>
.br
        width=>
.LP
If you respond with `no', then you will be prompted for a document number with
.IP
document=>
.LP
and then for a v-span set with
.IP
any spans? (y/n)
.LP
except that this time, the span in question is a v-span. V-spans are requested until a `no'
answer, after which you will be prompted for the next spec.
.PP
As an example consider this request to retrieve the first 10 characters from the document 1.1.0.1.
(The emphasized text is the user's response.):
.IP
request? \fI5\fP
.br
any spans or vspecs? (y/n)\fI y\fP
.br
a span? (y/n) \fIn\fP
.br
document=> \fI1.1.0.1\fP
.br
any spans? (y/n)\fI y\fP
.br
enter span
       start=> \fI1.1\fP
.br
        width=> \fI0.10\fP
.br
any spans? (y/n) \fIn\fP
.br
any spans of vspecs? (y/n) \fIn\fP
.LP
Assuming they are present, \fIxumain\fP will then respond with those 10 characters.
.SS Commands
There are four commands,
\fBx-account\fP, \fBcreate-node-or-account\fP, \fBopen\fP, and \fBclose\fP which do not
prompt for their arguments. All these, however, accept the same arguments as
\fIXanadu FeBe Protocol 88.1x\fP, but separated by newlines rather than the \fIXanadu FeBe Protocol 88.1x\fP word delimiter character.
.PP
The \fBinsert\fP request in \fIxumain\fP has an important difference from \fIXanadu FeBe Protocol 88.1x\fP.
Rather than giving a count of the number of characters in the text item, the input text
is terminated with a blank line.
.PP
There is a request unique to \fIxumain\fP called \fBsource-unix-command\fP, which is request number
\fB21\fP. This asks for a command for execution by
.IR sh(1)
with the standard output directed to a file with the name ``xum'' followed by
\fIxumain\fP's process number. A new document is created and the command output is placed there.
The new document number is printed for future use. Finally, the output file is deleted.
.SH FILES
\fIenf.enf\fP \- the magic data structure file.
.br
\fIxum???\fP \- command output for \fBsource-unix-command\fP
.SH SEE ALSO
backend(L), sh(1)
.br
\fIXanadu FeBe Protocol 88.1x Documentation\fP,
Xanadu Operating Company, August 1988
.SH BUGS
See
.IR backend(L).
.SH NOTE
Since the data structure is easy to corrupt with invalid but unchecked requests, it is
a good idea to occasionally copy \fIenf.enf\fP to a backup file.
.br
\fIXumain\fP will not appear in future releases.
