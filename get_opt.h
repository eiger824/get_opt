#ifndef GET_OPT_H_
#define GET_OPT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Pointer to the next argument */
char* next = NULL;

/* Pointer to the flags to read from */
char global_flags[500];

/* Indicates the input flags' format */
int format = -1;

/* Counter with the current argument number */
int argnr = 1;

/**Function:     get_opt - A new CLI argument parser!
   Description:  Given the input arguments parsed from the command line when
                 starting the main routine, this function shall go through the
                 parsed arguments and set the corresponding runtime values.
                
                 This modification of the well-known getopt function, defined
                 in <getopt.h>, supports both long and short arguments, i.e.,
                 one can input flags in both -h format or --help format.
                 
                 This function shall return the currently parsed option and
                 shall also set the value of the char* pointer pointing to the
                 corresponding subsequent arguments.
   @param argc:  The number if input arguments, received from the main function.
   @param argv:  A pointer to the argument array, also received from the main
                 function.
   Returns:      On success, it returns the first character of the input flag
                 When the last argument is processed, this function returns -1
                 Upon invalid flag provided or wrong number of subsequent
                 arguments provided to a flag, the function calls the exit()
                 routine and shall inform that an erroneous flag was input
*/
int get_opt(int argc, char* argv[])
{
   //Check if flags have been set
   if (format != -1)
   {
      if (argnr < argc)
      {
         next = argv[argnr];
         printf("Current arg. number: %d, value is %s: \n", argnr, next);
         ++argnr;
         //TODO return real value!!
         return 0;
      }
      else
      {
         printf("End of arguments. Returning -1\n");
         return -1;
      }
   }
   else
   {
      fprintf(stderr, "ERROR: flags were not set");
      fprintf(stderr, " (did you forget to call get_opt_set_flags() ? )\n");
      fprintf(stderr, "Exiting now\n");
      exit(2);
   }
}


/**Function:    get_opt_free
   Description: This function frees the resources allocated by get_opt
   Returns:     Nothing
 */
void get_opt_free()
{
   free(next);
}

/**Function: get_opt_set_flags
   Description: This function sets the flags that the get_opt function needs
                to check for the validity of the input arguments.
   @param flags: A string containing the available flags to use to check the
                 validity of the input arguments.
                 The format of @flags can be either in the short, "traditional"
                 form e.g.
                                  "ab:cd:e"
                 , where options denoted by a subsequent ':' character suggest
                 that they must accept a parameter, or in the long form e.g.
                           "all;block;coming-from*;delegate;enable+"
                 , where the options are separated with semicolons ';', and
                 flags can accept
                     (a) No arguments at all
                     (b) A subsequent argument MAY be provided (i.e., 0 or more
                         times), denoted by a start '*'
                     (c) A subsequent argument must be provided at least once
                         (i.e.) the correspondent flag expects to read at least
                         one argument.
   Returns: Nothing
 */

void get_opt_set_flags(const char* flags)
{
   if (strstr(flags, ";") != NULL)
   {
      printf("Long options\n");
      format = 1;
   }
   else
   {
      printf("Short options\n");
      format = 0;
   }
   memcpy(global_flags, flags, strlen(flags)+1);
   global_flags[strlen(flags)+1] = '\0';

   printf("Global flags is: %s (format is %d)\n", global_flags, format);
}

#endif /*GET_OPT_H_*/
