#include "pipestrm.h"

main()
{
    iNPstream pin("test1");
    char c;
    char buf[256];

    while (!pin.eof())
    {
	pin.getline(buf,256);
	cout <<buf<<endl;
    }
}
