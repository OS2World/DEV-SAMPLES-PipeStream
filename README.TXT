Named pipe stream classes  V1.0

The named pipe stream classes were written using Watcom 10a, under OS/2.
This was my first attempt at writing new classes derived from streambuf,
and istream/ostream, so there may be problems.

Files:
	in.cpp        opens a named pipe and sends the data to stdout
	out.cpp       creates a named pipe and sends stdin to the pipe

	pipestrm.h    named pipe header
	pipestrm.cpp  named pipe source


Classes:
	Pipebuf
	iNPstream
	oNPstream
	(sorry no ioNPstream)

The Pipebuf class uses three member functions to read/write to a
named pipe: underflow, overflow and sync.

Currently, the class can only be used for input or output but not
both.  The most likely way to have an i/o Pipe would be to split
in half and use one half for the get area, and the other half for
the put area.


oNPstream::oNPstream()  creates an output named pipe stream object,
	does not create a pipe.
	
oNPstream::oNPstream(const char*)  creates an output named pipe stream
	object and creates and connects to the pipe.  The string
	doesn't need the \pipe\ prepended, the class automatically
	does that for you.
	
oNPstream::open(const char*)  creates and connects to a named pipe.
	if a pipe is already, it returns 0.

oNPstream::close()  closes a named pipe.  Returns 0 if there is no
	pipe open.
	
The iNPstream function members are essentially the same except that
	iNPstream::open(const char*) opens the pipe and doesn't create
	a pipe.

These source files may be used for any private or commercial product with
no required payment or royalty.  I only ask that if you modify, improve
or fix bugs, that copies of the changes be sent to me.  When distributing
this package, please do not modify the files, and leave the copyright
intact.  Feel free to contact me for suggestions, comments, etc.

Eddy Kim
72327.1100@cis.com
