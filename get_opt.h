#ifndef GET_OPT_H_
#define GET_OPT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Pointer to the next argument */
char* next = NULL;
char* stringify = NULL;

/* Array of subsequent arguments used with flags */
char* getoptarg = NULL;

/* Pointer to the flags to read from */
char global_flags[500];

/* Indicates the input flags' format */
int format = -1;

/* Counter with the current argument number */
int argnr = 1;

/* Holds the value of the currently being processed flag */
char current_flag = '\0';

unsigned current_index = 0;

int get_opt_valid_flag(char c);
void get_opt_free();
int get_opt_parse_argv(char *arg);
void get_opt_stringify(int argc, char* argv[]);


void get_opt_stringify(int argc, char* argv[])
{
   unsigned j;
   int i;
   stringify = (char*) malloc(sizeof(char) * 500);
   for (i=1; i<argc; ++i)
   {
      for (j=0; j<strlen(argv[i]); ++j)
      {
         stringify[current_index] = argv[i][j];
         ++current_index;
      }
      if (i < argc - 1)
      {
         stringify[current_index] = ' ';
         ++current_index;
      }
   }
   stringify[current_index] = '\0';
#ifdef GET_OPT_DEBUG_
   printf("Stringified args: \"%s\" (strlen:%lu)\n",
          stringify, strlen(stringify));
#endif
}

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
   unsigned i,l;
   int n;
      
   if (format != -1)
   {
      if (argc > 1)
      {
         if (!stringify)
         {
            get_opt_stringify(argc, argv);
            current_index = 0;
         }

         //Last check if flags didn't start with '-'
         if (stringify[0] != '-')
         {
            fprintf(stderr, "ERROR: flags must start with '-'\n");
            get_opt_free();
            exit(2);
         }
         //And start the processing
         for (l=current_index; l<strlen(stringify); ++l)
         {
            char c = stringify[l];
            if (c == '-')
            {
               //new flag is coming
               current_flag = 0;
               ++current_index;
            }
            else if (c == ' ') //space
            {
               //TODO: just wait for now
               ++current_index;
            }
            else
            {
               
                  //c is a valid flag
               if ((n = get_opt_valid_flag(c)) != -1)
               {
                  current_flag = c;
                  int offset = 0;
                  switch(n)
                  {
                  case 0:
                     //option in 'c' does not accept args, just return
#ifdef GET_OPT_DEBUG_
                     printf("Valid flag '%c' found at index %d\n",
                            c,
                            current_index);
#endif
                     current_index = l+1;
                     return c;
                  case 1:
                     //option in 'c' accepts at least one argument
                     //idea, parse all upcoming characters until the next '-'
                     //is read or until the end or characters
#ifdef GET_OPT_DEBUG_
                     printf("Valid flag '%c' found at index %d\n",
                            c,
                            current_index);
#endif
                     getoptarg = (char*)malloc(sizeof(char) * 500);
                     //see where to start
                     if (stringify[current_index+1] == ' ') offset=1;
                     for (i=current_index + 1 + offset;
                          i < (strlen(stringify));
                          ++i)
                     {
                        if (stringify[i] == '-')
                           break;
                        getoptarg[i-current_index-offset-1] = stringify[i];
                     }
                     getoptarg[i-l] = '\0';
                     //Now since the flag expects at least one arg, check if one
                     //was provided
                     if (!strlen(getoptarg))
                     {
                        fprintf(stderr,
                                "ERROR -- flag '%c' requires at least an argument\n",
                                c);
                        get_opt_free();
                        exit(2);
                     }
                     //update index
                     current_index = l + strlen(getoptarg) + 1;
                     return c;
                  case 2:
#ifdef GET_OPT_DEBUG_
                     printf("Valid flag '%c' found at index %d\n",
                            c,
                            current_index);
#endif
                     getoptarg = (char*)malloc(sizeof(char) * 500);
                     //see where to start
                     if (stringify[current_index+1] == ' ') offset=1;
                     for (i=current_index + 1 + offset;
                          i < (strlen(stringify));
                          ++i)
                     {
                        if (stringify[i] == '-')
                           break;
                        getoptarg[i-current_index-offset-1] = stringify[i];
                     }
                     getoptarg[i-l] = '\0';
                     //update index
                     current_index = l + strlen(getoptarg) + 1;
                     return c;
                  }
               }
               else //invalid flag, just end the program
               {
                  fprintf(stderr, "Invalid flag --  %c\n", c);
                  get_opt_free();
                  exit(2);
               }
               
            }
         }
         
         //When having looped through all characters in the string, return
         return -1;
      }
      else
      {
         return -1;
      }
   }
   else
   {
      fprintf(stderr, "ERROR: flags were not set");
      fprintf(stderr, " (did you forget to call get_opt_set_flags() ? )\n");
      fprintf(stderr, "Exiting now\n");
      get_opt_free();
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
   free(stringify);
   free(getoptarg);
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
