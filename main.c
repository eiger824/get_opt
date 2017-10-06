#include <stdio.h>
#include "get_opt.h"

int main(int argc, char* argv[])
{
	int c;
        int a=0,b=0,c1=0,d=0,e=0;
        char* marg;
        
        get_opt_set_flags("abcde", NULL, "00*0+"); //OK
        //get_opt_set_flags(NULL, NULL, "00*0+"); //wrong
        //get_opt_set_flags("abcde", NULL, NULL); //wrong
        //get_opt_set_flags(NULL, "all;block;clear;delete;enable", "00*0+"); //OK
        //get_opt_set_flags("abcde", "all;block;clear;delete;enable", "00*0+"); //OK
	while ((c = get_opt(argc, argv)) != -1)
	{
           switch(c)
           {
           case 'a':
              a=1;
              break;
           case 'b':
              b=1;
              break;
           case 'c':
              c1=1;
              marg = getoptarg;
              printf("c parsed --> arg is \"%s\"\n", marg);
              break;
           case 'd':
              d=1;
              break;
           case 'e':
              marg = getoptarg;
              printf("e parsed --> arg is \"%s\"\n", marg);
              e=1;
              break;
           }
	}

        printf("(a,b,c,d,e):(%d,%d,%d,%d,%d)\n",a,b,c1,d,e);
        get_opt_free();
	return 0;
}
