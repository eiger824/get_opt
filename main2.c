#include <stdio.h>

#include "get_opt.h"

void version()
{
	printf("Version 0.1 of get_opt.h\n");
}

void help()
{
	printf("USAGE: ./example-short-flags [flags]\n");
	printf("-z\tThis flag does not accept any parameters\n");
	printf("-x\tThis flag expects at least one argument\n");
	printf("-c\tThis flag expects at least one argument\n");
	printf("-b\tThis flag expects at least one argument\n");
	printf("-v\tThis flag does not accept any parameters\n");
	printf("-h\tThis flag displays this help message\n");
}

int main(int argc, char* argv[])
{
	get_opt_set_flags("zxcvbh", NULL, "0++0+0");
	
	int e,z=0,x=0,c=0,v=0,b=0,h=0;

	while ((e = get_opt(argc, argv)) != -1)
	{
		switch(e)
		{
			case 'z':
				z=1;
				break;
			case 'x':
				x=1;
				//as usual, args are obtained from getoptarg
				for (unsigned i=0; i<getoptargcnt; ++i)
				{
					printf("Argument %d: \"%s\"\n", i, getoptarg[i]);
				}
				break;
			case 'c':
				c=1;
				//as usual, args are obtained from getoptarg
				for (unsigned i=0; i<getoptargcnt; ++i)
				{
					printf("Argument %d: \"%s\"\n", i, getoptarg[i]);
				}
				break;
			case 'v':
				v=1;
				version();
				return 0;
			case 'b':
				b=1;
				//as usual, args are obtained from getoptarg
				for (unsigned i=0; i<getoptargcnt; ++i)
				{
					printf("Argument %d: \"%s\"\n", i, getoptarg[i]);
				}
				break;
			case 'h':
				h=1;
				help();
				return 0;
		}
	}
	//Free resources allocated by get_opt
	get_opt_free();

	printf("(z,x,c,v,b) = (%d,%d,%d,%d,%d)\n", z,x,c,v,b);

	return 0;
}
