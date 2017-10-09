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
	int c;
        int a=0,b=0,c1=0,d=0,e=0,h=0;
        char** arg_list = NULL;
        char** arg_list_2 = NULL;
        
        //get_opt_set_flags("abcde", NULL, "00*0+"); //OK
        //get_opt_set_flags(NULL, NULL, "00*0+"); //wrong
        //get_opt_set_flags("abcde", NULL, NULL); //wrong
        get_opt_set_flags(NULL, "all;block;create;delete;enable;help", "00*0+0"); //OK
        //get_opt_set_flags("abcde", "all;block;clear;delete;enable", "00*0+"); //OK
	/* while ((c = get_opt(argc, argv)) != -1) */
	/* { */
        /*    switch(c) */
        /*    { */
        /*    case 'a': */
        /*       a=1; */
        /*       break; */
        /*    case 'b': */
        /*       b=1; */
        /*       break; */
        /*    case 'c': */
        /*       c1=1; */
        /*       arg_list = getoptarg; */
        /*       if (arg_list != NULL) */
        /*       { */
        /*          unsigned i; */
              
        /*          for (i=0; arg_list[i]; ++i) */
        /*          { */
        /*             printf("Parsed arg nr.%d: ", i); */
        /*             printf("\"%s\"\n", arg_list[i]); */
        /*          } */
        /*       } */
        /*       break; */
        /*    case 'd': */
        /*       d=1; */
        /*       break; */
        /*    case 'e': */
        /*       e=1; */
        /*       arg_list_2 = getoptarg; */
        /*       if (arg_list_2 != NULL) */
        /*       { */
        /*          unsigned i; */
              
        /*          for (i=0; arg_list_2[i]; ++i) */
        /*          { */
        /*             printf("Parsed arg nr.%d: ", i); */
        /*             printf("\"%s\"\n", arg_list_2[i]); */
        /*          } */
        /*       } */
        /*       break; */
        /*    } */
	/* } */





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
