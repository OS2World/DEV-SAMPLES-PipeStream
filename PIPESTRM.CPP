// $Author: EddyKim $
// $Header: H:/owin/debug/stream/RCS/pipestrm.cpp 1.2 1995/02/11 13:28:49 EddyKim Exp EddyKim $
//
// $Revision: 1.2 $

#include <string.h>
#include <assert.h>
#include "pipestrm.h"

Pipebuf::Pipebuf(): streambuf(), hPipe(0)
{
    Openmode=NP_ACCESS_OUTBOUND;
    Pipemode=NP_WAIT | NP_TYPE_MESSAGE | 0x01;
    Timeout=10000;
}

Pipebuf::~Pipebuf()
{
}

int Pipebuf::underflow()
{
//  1. if no reserve area is present and the
//  streambuf object is buffered, allocate the 
//  reserve area using the allocate member 
//  function and set up the reserve area 
//  pointers using the setb protected member function, 
    if (base()==NULL)
    {
	int retval=allocate();
	assert(retval!=EOF);
	setg(base(),base(),ebuf());
    }

//  2. flush any other uses of the reserve area, 
    setp(NULL,NULL);

// 3. read some characters from the streambuf 
// object's source into the get area, 
    ULONG bytesRead;
    APIRET rc;
    if (hPipe!=NULLHANDLE)
    {
	rc=DosRead(hPipe,eback(),blen(),&bytesRead);
	assert(rc==0);
    }
    else
	return EOF;

// 4. set up the get area pointers to reflect
// the characters that were read, 
    setg(base(),base(),base()+bytesRead);

// 5. return the first character of the get area, 
// or EOF if no characters could be read. 
    if (bytesRead>0)
	return sgetc();
    else
	return EOF;
}

int Pipebuf::overflow(int i)
{
//   1. if no reserve area is present and the streambuf 
//   object is not unbuffered, allocate a reserve area 
//   using the allocate member function and set up the 
//   reserve area pointers using the setb protected 
//   member function,
    if (base()==NULL)
    {
	int retval=allocate();
	assert(retval!=EOF);
	setp(base(),ebuf());
	return sputc(i);
    }

//    2. flush any other uses of the reserve area, 
    setg(NULL,NULL,NULL);

//    3. write any characters in the put area to 
//    the streambuf object's destination, 
    ULONG bytesWritten;

    APIRET rc;
    if(hPipe!=NULLHANDLE)
    {
	rc=DosWrite(hPipe,pbase(), (int) (pptr()-pbase()),
		    &bytesWritten);
	assert(rc==0);
    }

//    4. set up the put area pointers to reflect 
//    the characters that were written, 
    setp(base(),ebuf());

//    5. return __NOT_EOF on success, otherwise return EOF. 
    if (rc==0)
	return __NOT_EOF;
    else
	return EOF;
}

int Pipebuf::sync()
{
//   1. flush the put area, 
//   2. discard the contents of the get area and reposition 
//   the stream device so that the discarded characters may
//   be read again. 
    if (hPipe!=NULLHANDLE)
    {
	APIRET rc=0;
	ULONG bytes;
	if (in_avail()>0)
	{
	    rc=DosRead(hPipe,eback(),blen(),&bytes);
	    if (rc!=0)
	    {
		return EOF;
	    }
	    setg(base(),base(),base()+bytes);
	}
	if (out_waiting()>0)
	{
	    rc=DosWrite(hPipe,pbase(),(int)(pptr()-pbase()),
			&bytes);
	    if (rc!=0)
	    {
		return EOF;
	    }
	    setp(base(),ebuf());
	}
    }

    return __NOT_EOF;
}

int Pipebuf::create(const char* pipename)
{
    const Outbufsize=Inbufsize=blen()-1;
    // create pipe and connect and set handle if not already set
    if (hPipe==NULLHANDLE)
    {
	APIRET rc=DosCreateNPipe(pipename,&hPipe,Openmode,
				 Pipemode,Outbufsize,Inbufsize,
				 Timeout);
	if (rc!=0)
	    return 0;
	rc=DosConnectNPipe(hPipe);
	if (rc!=0)
	    return 0;
	return 1;
    }
    return 0;
}

int Pipebuf::open(const char* pipename)
{
    // open pipe and set handle if handle is not already set
    // maybe should allow setting of different flags via member
    // function.

    ULONG ActionTaken;
    const ULONG Filesize=0;
    const ULONG FileAttribute=FILE_READONLY;
    const ULONG Openflag=OPEN_ACTION_OPEN_IF_EXISTS;
    const ULONG Openmode=OPEN_SHARE_DENYNONE | OPEN_ACCESS_READONLY;

    if (hPipe==NULLHANDLE)
    {
	APIRET rc=DosOpen(pipename,&hPipe,&ActionTaken,
			  Filesize, FileAttribute,
			  Openflag,Openmode, NULL);
	if (rc!=0)
	    return 1;
    }
    return 0;
}

int Pipebuf::close()
{
    // close pipe and reset handle
    if (hPipe!=NULLHANDLE)
    {
	APIRET rc=DosClose(hPipe);
	if (rc==0)
	{
	    hPipe=NULLHANDLE;
	    return 1;
	}
    }
    return 0;
}

oNPstream::oNPstream(): ostream(&_pipebuf)
{
}

oNPstream::oNPstream(const char* pipename): ostream(&_pipebuf)
{
    open(pipename);
}

oNPstream::~oNPstream()
{
    close();
}

oNPstream::open(const char* pipename)
{
    char str[MAXPIPENAME];
    strcpy(str,"\\PIPE\\");
    strcat(str,pipename);
    return _pipebuf.create(str);
}

oNPstream::close()
{
    _pipebuf.sync();
    return _pipebuf.close();
}

iNPstream::iNPstream(): istream(&_pipebuf)
{
}

iNPstream::iNPstream(const char* pipename): istream(&_pipebuf)
{
    open(pipename);
}

iNPstream::~iNPstream()
{
    close();
}

iNPstream::open(const char* pipename)
{
    char str[MAXPIPENAME];
    strcpy(str,"\\PIPE\\");
    strcat(str,pipename);
    return _pipebuf.open(str);
}

iNPstream::close()
{
    _pipebuf.sync();
    return _pipebuf.close();
}
