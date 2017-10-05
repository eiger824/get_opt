#include <stdio.h>
#include "get_opt.h"

extern char getoptarg[500];

int main(int argc, char* argv[])
{
	int c;
        char* marg;
        //get_opt_set_flags("all;block;coming-from*;delegate;enable+");
        get_opt_set_flags("abc*de+");
	while ((c = get_opt(argc, argv)) != -1)
	{
           switch(c)
           {
           case 'a':
              break;
           case 'b':
              break;
           case 'c':
              marg = getoptarg;
              printf("c parsed --> arg is %s\n", marg);
              break;
           case 'd':
              break;
           case 'e':
              break;
           }
	}
        get_opt_free();
	return 0;
}
