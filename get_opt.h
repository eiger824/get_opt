#ifndef GET_OPT_H_
#define GET_OPT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Pointer to the list of subsequent arguments used with flags */
char** getoptarg = NULL;

/* Number of entries getoptarg will have in order
   to free them all in every call to get_opt */
unsigned no_entries = 0;

/* Pointer to the stringified arguments */
char* input_flags = NULL;
char* input_flags_init = NULL;

char* tmp_args = NULL;

/* Pointer to the short flags to read from */
char* global_short_flags = NULL;

/* Pointer to the long flags to read from */
char* global_long_flags = NULL;

/* Pointer to the occurrences of the flags */
char* occurrences = NULL;

/* Indicates if flags have been set */
int flags_set = -1;

/* Indicates which format was chosen:
 0->short
 1->long
 2->both
*/
typedef enum {
   NONE = -1,
   SHORT,
   LONG,
   BOTH
} FORMAT;

FORMAT selected_format = NONE;

/* Holds the value of the currently being processed flag */
char current_flag = '\0';

unsigned current_index = 0;

int get_opt_valid_flag(char c);
int get_opt_valid_long_flag(char* flag);
void get_opt_free();
int get_opt_parse_argv(char *arg);
void get_opt_stringify(int argc, char* argv[]);
void get_opt_print_flags();
void get_opt_2_list(char* buffer);
int get_opt(int argc, char* argv[]);
char* get_opt_long(int argc, char* argv[]);

void get_opt_stringify(int argc, char* argv[])
{
   unsigned j;
   int i;
   input_flags = (char*) malloc(sizeof(char) * 500);
   for (i=1; i<argc; ++i)
   {
      for (j=0; j<strlen(argv[i]); ++j)
      {
         input_flags[current_index] = argv[i][j];
         ++current_index;
      }
      if (i < argc - 1)
      {
         input_flags[current_index] = ' ';
         ++current_index;
      }
   }
   input_flags[current_index] = '\0';
   //input_flags_init points to the same memory address as input_flags
   input_flags_init = input_flags;
#ifdef GET_OPT_DEBUG_
   printf("Stringified args: \"%s\" (strlen:%lu)\n",
          input_flags, strlen(input_flags));
#endif
}

char* get_opt_long(int argc, char* argv[])
{
   printf("\n");
   if (flags_set != -1)
   {
      if (argc > 1)
      {
         if (!input_flags)
         {
            get_opt_stringify(argc, argv);
            current_index = 0;
         }
//free getoptarg every time this function is called
         if (getoptarg != NULL)
         {
            for (unsigned i=0; i<no_entries; ++i)
            {
               free(getoptarg[i]);
            }
            free(getoptarg);
            no_entries = 0;
            getoptarg = NULL;
         }
         //loop through the input args
         unsigned i;
         for (i=current_index; i<strlen(input_flags); ++i)
         {
            char *c = strstr(input_flags, "--");
            if (c != NULL)
            {
               int index = c - input_flags;
               //add up current_index (two incoming hyphens)
               input_flags+=2;
               //read until end of input_flags or space or '=' symbol
               unsigned j;
               char* flag = (char*) malloc(sizeof(char) * 50);
               for (j=0; strlen(input_flags); ++j)
               {
                  if (input_flags[j] == '\0' ||
                      input_flags[j] == ' ' ||
                      input_flags[j] == '=')
                     break;
                  flag[j] = input_flags[j];
               }
               flag[j] = '\0';
               int n;
               if ((n = get_opt_valid_long_flag(flag)) != -1)
               {
                  if (n == 0) //no args
                  {
                     input_flags+=(strlen(flag)+1);
                     current_index = 0;
                     return flag;
                  }
                  else if (n == 1) //at least one
                  {
                     //tmp_args = (char*) malloc( (sizeof *tmp_args) * 500);
                     //TODO - parse args
                     return flag;
                  }
                  else //zero or more
                  {
                     //tmp_args = (char*) malloc( (sizeof *tmp_args) * 500);
                     //TODO - parse args
                     return flag;
                  }
               }
               else
               {
                  fprintf(stderr, "Invalid flag -- \"%s\"\n", flag);
                  get_opt_free();
                  free(flag);
                  exit(2);
               }
            }
            else
            {
               //no valid flags, return
               //last check
               if (strchr(input_flags, '-') != NULL)
               {
                  fprintf(stderr, "Invalid flag format (long format selected)\n");
                  get_opt_free();
                  exit(2);
               }
               return NULL;
            }
         }
      }
      else
      {
         return NULL;
      }
   }
   else
   {
      fprintf(stderr, "ERROR: flags were not set\n");
      fprintf(stderr, " (did you forget to call get_opt_set_flags() ? )\n");
      fprintf(stderr, " (did you forget to set the flags' occurrences ? )\n");
      fprintf(stderr, "Exiting now\n");
      get_opt_free();
      exit(2);
   }
   return NULL;
}

/**Function:     get_opt - A new CLI argument parser!
   Description:  Given the input arguments parsed from the command line when
                 starting the main routine, this function shall go through the
                 parsed arguments and set the corresponding runtime values.
                
                 This modification of the well-known getopt function, defined
                 in <getopt.h>, supports both long and short argguments, i.e.,
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
      
   if (flags_set != -1)
   {
      if (argc > 1)
      {
         if (!input_flags)
         {
            get_opt_stringify(argc, argv);
            current_index = 0;
         }
         
         //free getoptarg every time this function is called
         if (getoptarg != NULL)
         {
            for (unsigned i=0; i<no_entries; ++i)
            {
               free(getoptarg[i]);
            }
            free(getoptarg);
            no_entries = 0;
            getoptarg = NULL;
         }
         
         //Last check if flags didn't start with '-'
         if (input_flags[0] != '-')
         {
            fprintf(stderr, "ERROR: flags must start with '-'\n");
            get_opt_free();
            exit(2);
         }
         //And start the processing
         for (l=current_index; l<strlen(input_flags); ++l)
         {
            char c = input_flags[l];
            if (c == '-')
            {
               //new flag is coming
               current_flag = 0;
               ++current_index;

               // fprintf(stderr, "Invalid flag --  %c\n", c);
               // get_opt_free();
               // exit(2);
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
                     tmp_args = (char*)malloc(sizeof(char) * 500);
                     //see where to start
                     if (input_flags[current_index+1] == ' ') offset=1;
                     for (i=current_index + 1 + offset;
                          i < (strlen(input_flags));
                          ++i)
                     {
                        if (input_flags[i] == '-')
                           break;
                        tmp_args[i-current_index-offset-1] = input_flags[i];
                     }
                     tmp_args[i-l] = '\0';
                     //Now since the flag expects at least one arg, check if one
                     //was provided
                     if (!strlen(tmp_args))
                     {
                        fprintf(stderr,
                                "ERROR -- flag '%c' requires at least an argument\n",
                                c);
                        get_opt_free();
                        exit(2);
                     }
                     //transform to list
                     get_opt_2_list(tmp_args);
                     //update index
                     current_index = l + offset + strlen(tmp_args) + 1;
#ifdef GET_OPT_DEBUG_
                     printf("Current index is now: %d\n", current_index);
#endif
                     return c;
                  case 2:
#ifdef GET_OPT_DEBUG_
                     printf("Valid flag '%c' found at index %d\n",
                            c,
                            current_index);
#endif
                     tmp_args = (char*)malloc(sizeof(char) * 500);
                     //see where to start
                     if (input_flags[current_index+1] == ' ') offset=1;
                     for (i=current_index + 1 + offset;
                          i < (strlen(input_flags));
                          ++i)
                     {
                        if (input_flags[i] == '-')
                           break;
                        tmp_args[i-current_index-offset-1] = input_flags[i];
                     }
                     tmp_args[i-l] = '\0';
                     get_opt_2_list(tmp_args);
                     //update index
                     current_index = l + offset + strlen(tmp_args) + 1;
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
      fprintf(stderr, "ERROR: flags were not set\n");
      fprintf(stderr, " (did you forget to call get_opt_set_flags() ? )\n");
      fprintf(stderr, " (did you forget to set the flags' occurrences ? )\n");
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
   free(global_short_flags);
   free(global_long_flags);
   free(occurrences);
   //assign back input_flags to where it pointed at the beginning
   //no need to free input_flags_init
   input_flags = input_flags_init;
   free(input_flags);
   free(tmp_args);
   free(getoptarg);
}



/**Function: get_opt_set_flags
   Description:        This function sets the flags that the get_opt function needs
                       to check for the validity of the input arguments. Both short
                       and long flags are specified so a 1:1 mapping exists between
                       the two formats.
   @param short_flags: A string containing the available flags to use to check the
                       validity of the input arguments.
                       The format of @short_flags is in the short, "traditional"
                       form e.g.
                                               "ab:cd:e"
                       , where options denoted by a subsequent ':' character suggest
                       that they must accept a parameter.
                       Flags can accept
                          (a) No arguments at all
                          (b) A subsequent argument MAY be provided (i.e., 0 or more
                              times), denoted by a start '*'
                          (c) A subsequent argument must be provided at least once
                              (i.e.) the correspondent flag expects to read at least
                              one argument.
   @param long_flags:  A string containing the available flags to use to check the
                       validity of the input arguments. The format of @long_flags is
                       in the long form e.g.
                                    "all;block;coming-from*;delegate;enable+"
                       , where the options are separated with semicolons ';', and
                       similarly to @short_flags, they can accept
                          (a) No arguments at all
                          (b) A subsequent argument MAY be provided (i.e., 0 or more
                              times), denoted by a start '*'
                          (c) A subsequent argument must be provided at least once
                              (i.e.) the correspondent flag expects to read at least
                              one argument.
   @param occurrence:  A string containing the occurrence of every input flag. As
                       discussed, every flag will take (a) 0 args, (b) * args or
                       (c) + args, in which case @occurrence may take the form
                       of an array of 0s, *s and +s, e.g.
                                  "0*0+0**"
   Returns:            Nothing
 */

void get_opt_set_flags(const char* short_flags,
                       const char* long_flags,
                       const char* occurrence)
{
   global_short_flags = (char*) malloc(sizeof(char) * 500);
   if (short_flags != NULL)
   {
      memcpy(global_short_flags, short_flags, strlen(short_flags)+1);
      global_short_flags[strlen(short_flags)+1] = '\0';
      selected_format = SHORT;
   }

   global_long_flags = (char*) malloc(sizeof(char) * 500);
   if (long_flags != NULL)
   {
      memcpy(global_long_flags, long_flags, strlen(long_flags)+1);
      global_long_flags[strlen(long_flags)+1] = '\0';
      if (selected_format == SHORT)
         selected_format = BOTH;
      else
         selected_format = LONG;
   }

   occurrences = (char*) malloc(sizeof(char) * 500);
   if (occurrence != NULL)
   {
      memcpy(occurrences, occurrence, strlen(occurrence)+1);
      occurrences[strlen(occurrence)+1] = '\0';
   }
   
   if ((short_flags == NULL && long_flags == NULL) ||
      occurrence == NULL)
   {
      flags_set = -1;
      selected_format = NONE;
   }
   else
   {
      flags_set = 1;
      //last check, lengths of short_flags == occurrences
      if (selected_format == SHORT)
      {
         assert(strlen(global_short_flags) == strlen(occurrences));
      }
   }

#ifdef GET_OPT_DEBUG_
   get_opt_print_flags();
#endif
}


//Returns: nr args it accepts, -1 otherwise
int get_opt_valid_flag(char c)
{
   unsigned i;
   if (selected_format == SHORT)
   {
      for (i=0; i<strlen(global_short_flags); ++i)
      {
         if (global_short_flags[i] == c)
         {
            current_flag = c;
           
            if (occurrences[i] == '+')
            {
               return 1;
            }
            else if (occurrences[i] == '*')
            {
               return 2;
            }
            else
            {
               return 0;
            }
            
         }
      }
   }
   return -1;
}

int get_opt_valid_long_flag(char* flag)
{
   char *p = strstr(global_long_flags, flag);
   if (p != NULL)
   {
      //last check for "partial" matches
      //e.g. if a valid flag is for example "all",
      //but flag="a" was passed onto this function.
      //strstr will still give us a match
      char *q = strchr(p, ';');
      if (q != NULL)
      {
         for (unsigned i=0; i <q-p; ++i)
         {
            if (p[i] != flag[i]) return -1;
         }
      }
      else //last flag
      {
         if (strcmp(q, flag)) return -1;
      }
      unsigned flag_nr;
      for (flag_nr=0; p[flag_nr]; (p[flag_nr] == ';') ? ++flag_nr : *p++);
      flag_nr = 5 - flag_nr - 1;
#ifdef GET_OPT_DEBUG_
      printf("Flag \"%s\" is at flag position: %d/4\n", flag, flag_nr);
#endif
      //assert that flag was found in a valid range
      assert(0 <= flag_nr && flag_nr < strlen(occurrences));
      //then return how many parameters it accepts
      if (occurrences[flag_nr] == '0') return 0;
      else if (occurrences[flag_nr] == '+') return 1;
      else return 2;
   }
   else
   {
      return -1;
   }
}

#ifdef GET_OPT_DEBUG_
void get_opt_print_flags()
{
   if (flags_set != -1)
   {
      printf("Flags are (selected format is %d):\n-----------\n", selected_format);
      unsigned j, start = 0;
      int where;
      unsigned short_index = 0;
      if (selected_format == BOTH || selected_format == LONG)
      {
         char *c = strchr(global_long_flags, ';');
         while (c != NULL)
         {
            if (selected_format == BOTH)
               printf("%c/", global_short_flags[short_index]);
      
            where = c - global_long_flags + 1;
            for (j=start; j<where-1; ++j)
            {
               printf("%c", global_long_flags[j]);
            }
            if (occurrences[short_index] == '*')
            {
               printf("  (#args >=0)\n");
            }
            else if (occurrences[short_index] == '+')
            {
               printf(" (#args >=1)\n");
            }
            else
            {
               printf(" (no args.)\n");
            }

            ++short_index;
            start=where;
            //start looking again after found position
            c = strchr(c + 1, ';');
      
            if (c == NULL) //last round
            {
               if (selected_format == BOTH)
                  printf("%c/", global_short_flags[short_index]);

               for (j=start; j<strlen(global_long_flags); ++j)
               {  
                  printf("%c", global_long_flags[j]);
               }
               if (occurrences[short_index] == '*')
               {
                  printf("  (#args >=0)\n");
               }
               else if (occurrences[short_index] == '+')
               {
                  printf(" (#args >=1)\n");
               }
               else
               {
                  printf(" (no args.)\n");
               }
            }
         }
         free(c);
      }
      else if (selected_format == SHORT)
      {
         for (j=0; j<strlen(global_short_flags); ++j)
         {
            printf("%c", global_short_flags[j]);
            if (occurrences[j] == '*')
               {
                  printf(" (#args >=0)\n");
               }
               else if (occurrences[j] == '+')
               {
                  printf(" (#args <=1)\n");
               }
               else
               {
                  printf(" (no args.)\n");
               }
         }
      }
      else
      {
         fprintf(stderr, "(WARNING: format was not set)\n");
      }
      printf("-----------\n");
   }
}
#endif

void get_opt_2_list(char* buffer)
{
   //Trim every possible trailing whitespaces
   unsigned i, init_len = strlen(buffer);
   if (init_len > 0)
   {
      for (i=init_len-1; ; --i)
      {
         if (buffer[i] == ' ') buffer[i] = 0;
         else break;
      }
      init_len = i+1;
#ifdef GET_OPT_DEBUG_
      printf("Buffer to transform: \"%s\"\n", buffer);
#endif
      //Count the number of items the list will contain
      for (no_entries=0; buffer[no_entries];
           buffer[no_entries] == ' ' ?
              no_entries++ :
              *buffer++);
      no_entries++;
#ifdef GET_OPT_DEBUG_
      printf("No. items: %d\n", no_entries);
#endif
      buffer -= (init_len-no_entries+1);
   
      //initiate the string list with the number of parsed arguments
      getoptarg = (char**) malloc ((no_entries + 1) * sizeof(char*));
   
      for (i=0; i<no_entries; ++i)
      {
         //initialize each argument field with a big size
         getoptarg[i] = (char*) malloc(sizeof(char) * 500);
         char *c = strchr(buffer, ' ');
         int at;
         if (c != NULL)
         {
            at = c - buffer;
            memcpy(getoptarg[i], buffer, at+1);
            getoptarg[i][at] = '\0';
#ifdef GET_OPT_DEBUG_
            printf("New element appended: \"%s\"\n", getoptarg[i]);
#endif
            buffer+=(at+1);
         }
         else
         {
            //last element, just copy @buffer in the list
            memcpy(getoptarg[i], buffer, strlen(buffer)+1);
            getoptarg[i][strlen(buffer)] = '\0';
#ifdef GET_OPT_DEBUG_
            printf("New element appended: \"%s\"\n", getoptarg[i]);
#endif  
         }
      }
      //and zero-terminate the list
      getoptarg[i] = 0;
   }
   else
   {
      getoptarg = (char**) malloc(1 * sizeof (char*));
      getoptarg[0] = 0;
   }
}

#endif /*GET_OPT_H_*/
