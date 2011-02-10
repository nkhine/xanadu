.TH FEX L "25 August 1988"
.SH NAME
\fIfex\fP \- experimental
.ie t Xanadu\*(Tm
.el Xanadu(tm)
frontend
.SH SYNOPSIS
.B fex 
.I pipe-from-backend
.I pipe-to-backend
.SH INVOCATION
.I fex
is normally invoked by a "glue" program such as 
.I intf
that starts both the frontend and the backend and sets up 
the pipes to allow them to communicate.
.SH DESCRIPTION
.I fex
is a screen-oriented program resembling a visual text editor which provides
a user interface to the Xanadu backend
.IR backend(L).
There is no explicit insert mode: alphanumeric characters typed at any time are
inserted into the text under the cursor position. The DEL key is used to 
delete characters before the cursor position.
.PP
When \fIfex\fP
is invoked it automatically locates and places on the screen a document 
(1.1.0.1.0.1)
that serves as
an entry point to the system. This document contains a summary of frontend commands and
a link connecting it to another document that serves as a general index. Additional commands  
are invoked through a pop-up menu.
.SH TERMINOLOGY
.SS
.TP 15
.I endset
A region of text (possibly discontinuous and possibly spanning more than one
document) connected to another such region by a link.
.TP 15
.I link
A connection between two endsets, modified by a third endset. These are respectively
called the \fIfromset, toset\fP, and \fIthreeset\fP of the link. 
.IR Fromset s
are displayed as back-lit;
.IR toset s
as underlined (termcap
permitting) or as back-lit.
.IR Threeset s
are used to describe the intended meaning of the link, such as if it is a jump
link or a footnote link.
The distinction between from-set and to-sets reflects
the link author's intentions rather than any functional difference.
The from-sets and to-sets of a link are really symmetrical.
.TP 15
.I cut
A temporary mark made in the text to specify boundaries of an endset
for a link to be created, or a section to be rearranged or deleted.
.TP 15
.I display style
When a link is followed, the text at the destination is displayed in a window that can be a
quarter-screen (default), a horizontal half-screen, a vertical half-screen, or an entire screen.
The display style can be changed through the menu (see below).
If a window was created by following a link, its display style is initially
determined by the threeset of that link.
.SH COMMANDS
.SS
.I cursor control
.TP 20
^W or up arrow
Move cursor up
.PD 0
.TP 20
^Z or down arrow
Move cursor down
.TP 20
^A or left arrow
Move cursor left
.TP 20
^S or right arrow
Move cursor right
.SS
.TP 20
^Q
Top of document
.TP 20
^F
Down one page
.SS
.PD 1
.I menu 
.TP 10
^N
Show menu: current selection is back-lit. Up and down cursor controls change
current selection and SPACE BAR executes it. Available menu selections are
listed under
.SM MENU OPTIONS
below.
.TP 10
.I link following
.TP 10
^Y
Follow the link indicated by the cursor. If the cursor position
is occupied by multiple, overlapping endsets, the first lines of each
destination will be displayed in a menu.
When the link is selected the destination text will appear in a new window whose size and
position is determined by the 
.I display style.
.TP 10
^U
Return from link: the first lines of the windows to which a return is possible
are displayed in a menu.
.PD 0
.SS
.TP 10
.I text editing
.SS
.TP 10
^G
Make cut. (For deletions see 
.I MENU OPTIONS
)
.TP 10
^P
Delete block
.TP 10
^C
Rearrange
.SS
.TP 10
.I link creation
.SS
.TP 10
^V
Change fromset: text delimited by cuts is added to a temporary fromset; then the
cuts are cleared.
.TP 10
^B
Change toset: text delimited by cuts is added to a temporary toset;then the
cuts are cleared.
.TP 10
^L
Make link: a menu appears asking if the link is to be a jump, quote, footnote,
or marginal note.  An appropriately typed link is then created between
the temporary fromset and the temporary toset; then the temporary endsets
are cleared.
.SS
.TP 10
.I miscellaneous
.SS
.TP 10
^]
Redraw screen
.TP 10
^T
Exit program
.SS
.SH MENU OPTIONS
.SS
.PD 1
.TP 22
set display style
Select a 
.I display 
.IR style,
as defined above
.TP 22
show key definitions
(unimplemented)
.TP 22
compare
Show corresponding portions of two documents
.TP 22
follow link
= ^Y
.TP 22
create link
= ^L
.TP 22
make cut
= ^G
.TP 22
delete cuts
Clear all cuts
.TP 22
delete block
= ^P
.TP 22
rearrange
= ^C
.TP 22
create document
Create a new, empty document.
NOTE: a link to the new document must be made before leaving the document
or the frontend will be unable to retrieve the document even though it still exists.
.TP 22
change fromset
= ^V
.TP 22
change toset
= ^B
.TP 22
clearendsets
Remove all temporary endsets created by the previous two commands.
.TP 22
create version
Duplicate the current document as a new version.
NOTE: a link to the new document must be made before leaving the document
or the frontend will be unable to retrieve the document even though it still exists.
.SH BUGS
This is an experimental frontend. The following bugs are known to exist.
There are undoubtedly others.
.TP 4
1.
Hitting DELETE with the cursor in the first position of a window causes a fatal error.
.TP 4
2.
Text cannot be inserted below the last line of a document. Attempts to do so cause
a fatal error. To append text to a document, place the cursor at the end of the last line
and hit RETURN. Blank lines may be appended with repeated RETURNs.
.TP 4
3.
Deletion of text in a newly-created document (i.e., one created in the current invocation of
.IR fex )
may result in a mispositioning of the cursor.
The actual document 
.I should
be edited correctly. However, it is recommended that editing of newly-created documents be
kept to a minimum.
