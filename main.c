#include <stdio.h>
#include "get_opt.h"

void help()
{
	printf("Usage:\n");
	printf("--all (noargs)\n");
	printf("--block (noargs)\n");
	printf("--create (zero or more args)\n");
	printf("--delete (noargs)\n");
	printf("--enable (at least one arg)\n");
	printf("--help (noargs)\n");
}

int main(int argc, char* argv[])
{
        int a=0,b=0,c1=0,d=0,e=0,h=0;
        char** arg_list = NULL;
        char** arg_list_2 = NULL;
        
        get_opt_set_flags(NULL, "all;block;create;delete;enable;help", "00*0+0"); //OK

	char *p;
        while ((p = get_opt_long(argc, argv)) != NULL)
        {
           if (!strcmp(p, "all"))
           {
              //printf("Do FOO\n");
              a=1;
           }
           else if (!strcmp(p, "block"))
           {
              b=1;
           }
           else if (!strcmp(p, "create"))
           {
              c1=1;
           }
           else if (!strcmp(p, "delete"))
           {
              d=1;
           }
           else if (!strcmp(p, "enable"))
           {
              e=1;
              for (unsigned i=0; i<getoptargcnt; ++i)
              {
                 printf("Parsed: \"%s\"\n", getoptarg[i]);
              }
           }
           else if (!strcmp(p, "help"))
           {
              h=1;
              help();
           }
        }

        printf("(a,b,c,d,e,h):(%d,%d,%d,%d,%d,%d)\n",a,b,c1,d,e,h);
        get_opt_free();
        printf("Get opt's resources have been free'd\n");
        
        //free arg lists
        free(arg_list);
        free(arg_list_2);
	return 0;
}
