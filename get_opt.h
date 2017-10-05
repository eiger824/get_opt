#ifndef GET_OPT_H_
#define GET_OPT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Pointer to the next argument */
char* next = NULL;

/* Array of subsequent arguments used with flags */
char getoptarg[500];

/* Pointer to the flags to read from */
char global_flags[500];

/* Indicates the input flags' format */
int format = -1;

/* Counter with the current argument number */
int argnr = 1;

/* Holds the value of the currently being processed flag */
char current_flag = '\0';

/* Flag that determines if the i-th argument has been totally read */
int arg_processed = 0;

unsigned current_index = 1;

int get_opt_valid_flag(char c);
void get_opt_free();
int get_opt_parse_argv(char *arg);

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
      if (argc > 1 && argv[1][0] != '-')
      {
         fprintf(stderr, "FORMAT ERROR: First flag must start with '%s'\n",
                 (format)?"--":"-");
         get_opt_free();
         exit(1);
      }
      if (argnr < argc)
      {
         next = (char*) malloc(sizeof(char) * (strlen(argv[argnr]) + 1));
         memcpy(next, argv[argnr], strlen(argv[argnr]));
         next[strlen(argv[argnr])] = '\0';

#ifdef GET_OPT_DEBUG_
         printf("Current arg. number: %d, value is %s (current flag: %c)\n", argnr, next, current_flag);
#endif
         
         int c = get_opt_parse_argv(next);
         
         if (arg_processed)
            ++argnr;
         //TODO return real value!!
         return c;
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
      format = 1;
   }
   else
   {
      format = 0;
   }
   memcpy(global_flags, flags, strlen(flags)+1);
   global_flags[strlen(flags)+1] = '\0';

#ifdef GET_OPT_DEBUG_
   printf("Global flags is: '%s' (%s format)\n",
          global_flags,
          (format)?"long":"short");
#endif
}

int get_opt_parse_argv(char *arg)
{
   if (strlen(arg) < 2)
   {
      //just a single character - invalid
      return -1;
   }
   else //argv_i is valid
   {
      if (arg[0] == '-')
      {
         unsigned i;
         for (i=current_index; i<strlen(arg); ++i)
         {
            int c;
            switch ((c= get_opt_valid_flag(arg[i])))
            {
            case 0:
#ifdef GET_OPT_DEBUG_
               printf("Valid, non-argument-returning flag found: %c\n",
                      arg[i]);
#endif
               if (i == strlen(arg)-1)
               {
                  current_index = 1;
                  arg_processed = 1;
               }
               else
               {
                  ++current_index;
               }
               return arg[i];
            case 1:
#ifdef GET_OPT_DEBUG_
               printf("Valid, at-least-one-argument-returning flag found: %c\n",
                      arg[i]);
#endif 
               if (i == strlen(arg)-1)
               {
                  current_index = 1;
               }
               else
               {
                  ++current_index;
                  unsigned j;
                  for (j=current_index; j<strlen(arg); ++j)
                  {
                     getoptarg[j] = arg[j];
                  }
                  getoptarg[j-current_index+1] = '\0';
               }
               arg_processed = 1;
               return arg[i];
            case 2:
#ifdef GET_OPT_DEBUG_
               printf("Valid, optional-argument-returning flag found: %c\n",
                      arg[i]);
#endif 
               if (i == strlen(arg)-1)
               {
                  current_index = 1;
               }
               else
               {
                  ++current_index;
                  printf("Current index is %d\n", current_index);
                  unsigned j;
                  for (j=current_index; j<strlen(arg); ++j)
                  {
                     getoptarg[j-current_index] = arg[j];
                     printf("j: %d, getoptarg[%d]: %c\n", j, j, getoptarg[j]);
                  }
                  getoptarg[j-current_index+1] = '\0';
                  printf("Going to return: %s\n", getoptarg);
               }
               arg_processed = 1;
               return arg[i];  
            default:
#ifdef GET_OPT_DEBUG_
               fprintf(stderr,
                       "Invalid flag --  %c\n",
                       arg[i]);
#endif
               exit(2);
            }
         }
      }
      else
      {
         //check whether the current flag accepts parameters
      }
   }
}


//Returns: nr args it accepts, -1 otherwise
int get_opt_valid_flag(char c)
{
   unsigned i;
   for (i=0; i<strlen(global_flags); ++i)
   {
      if (global_flags[i] == c)
      {
         if (i == strlen(global_flags) - 1)
         {
            current_flag = c;
            return 0;
         }
         else
         {
            if (global_flags[i+1] == '+')
            {
               current_flag = c;
               return 1;
            }
            else if (global_flags[i+1] == '*')
            {
               current_flag = c;
               return 2;
            }
            else
            {
               current_flag = c;
               return 0;
            }
         }
      }
   }
   return -1;
}

#endif /*GET_OPT_H_*/
