// $Author: EddyKim $
// $Header: H:/owin/debug/stream/RCS/pipestrm.h 1.2 1995/02/11 13:28:49 EddyKim Exp EddyKim $
//
// $Revision: 1.2 $

#ifndef PIPESTRM_H
#define PIPESTRM_H

#define INCL_DOSPROCESS
#define INCL_DOSNMPIPES
#include <os2.h>

#include <iostream.h>
#include <iostream.h>

class Pipebuf: public streambuf
{
public:
    Pipebuf();
    ~Pipebuf();
    int create(const char*);
    int open(const char*);
    int close();

    int underflow();
    int overflow(int = EOF);
    int sync();
protected:
    HPIPE hPipe;
    ULONG Openmode,Pipemode,Outbufsize,Inbufsize,Timeout;
};

const MAXPIPENAME=260;

class oNPstream:public virtual ostream
{
public:
    oNPstream();
    oNPstream(const char* pipename);
    ~oNPstream();
    open(const char* pipename);
    close();
protected:
    Pipebuf _pipebuf;
};

class iNPstream: public virtual istream
{
public:
    iNPstream();
    iNPstream(const char* pipename);
    ~iNPstream();
    open(const char* pipename);
    close();
protected:
    Pipebuf _pipebuf;
};

#endif
