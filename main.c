#include <stdio.h>
#include "get_opt.h"

int main(int argc, char* argv[])
{
	int c;
        int a=0,b=0,c1=0,d=0,e=0;
        char** arg_list = NULL;
        char** arg_list_2 = NULL;
        
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
              arg_list = getoptarg;
              if (arg_list != NULL)
              {
                 unsigned i;
              
                 for (i=0; arg_list[i]; ++i)
                 {
                    printf("Parsed arg nr.%d: ", i);
                    printf("\"%s\"\n", arg_list[i]);
                 }
              }
              break;
           case 'd':
              d=1;
              break;
           case 'e':
              e=1;
              arg_list_2 = getoptarg;
              if (arg_list_2 != NULL)
              {
                 unsigned i;
              
                 for (i=0; arg_list_2[i]; ++i)
                 {
                    printf("Parsed arg nr.%d: ", i);
                    printf("\"%s\"\n", arg_list_2[i]);
                 }
              }
              break;
           }
	}

        printf("(a,b,c,d,e):(%d,%d,%d,%d,%d)\n",a,b,c1,d,e);
        get_opt_free();
        
        //free arg lists
        free(arg_list);
        free(arg_list_2);
	return 0;
}
