#include "pipestrm.h"

main()
{
    oNPstream onps("test1");
    char buf[60];

    while (1)
    {
	cin.getline(buf,60);
	if (buf[0]=='.')
	    break;
	onps<<buf<<endl;
    }
}
