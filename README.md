(IN PROGRESS)

_get_opt_ - A new version of the classical getopt.h header!

This simple function parses the input arguments received from the command line whenever a program is started.

For the impatient: just clone the project and run `make`. It will output a simple test file that uses the function.

It's use is very simple:

1. A call to the following function:

                           get_opt_set_flags(const char* short_flags,
                                              const char* long_flags,
                                              const char* occurrences);
                      
This function sets the flags to consider when parsing the input arguments. One does not need provide both flags in short and long modes, e.g. by providing the _short_flags_ (e.g. `"hac"`) and passing `NULL` as _long_flags_ , only the traditional short flags will be supported, as in

                                   ./some-program -h -a -c some_arg
                                   
If the long format is preferred over the short one, one could pass `NULL`as _short_flags_ and provide only the long flags (e.g. `"help;all;create"`), separated by a semicolon (;), so the program would look for long options as in

                                   ./some-program --help --all --create some_arg

Finally, if **both** long and short arguments are to be supported, **both** _short_flags_ and _long_flags_ shall be provided to the function. This is for the cases where the user may input flags in both formats:

                                   ./some-program -h -a --create some_arg
                                   
The _occurrences_ parameter is **mandatory** since it specifies the number of optional arguments each flag accepts. Two requirements are set in this parameter:

1.1. It can only contain the following characters: **0 + \***

    0 -> the corresponding flag __does not__ accept any parameters.
    
    + -> the corresponding flag accepts __at least__ one subsequent parameter
    
    * -> the corresponding flag __optionally__ accepts parameters, i.e. zero or more parameters
    
1.2. Its length __must match__ the length of _short_flags_, i.e.:

                                    strlen(ocurrences) == strlen(short_flags)

otherwise a format error is triggered and the program exits.


2. Call the _get_opt_ main function as follows:

                              int c;
                              while ((c = get_opt(argc, argv)) != -1)
                              {
                                switch (c)
                                {
                                  //make a case for every flag provided
                                }
                              }
                              ...
                              
The function will first "stringify" all the input parameters coming from the main() function of the program, i.e., it will append all of them in a string and add spaces between them. Then it shall loop through its character in search of valid flags and check if for a read flag, the subsequent characters yield an complementary argument, regardless of whether they were provided in the next parsed word. When no flags are left or no characters are left in the "stringified" parsed arguments, the function shall return -1 and thus end the while loop.

Following the previous example, in the case of a flag accepting (optionally or not) some parameter(s), they shall be retrieved from the global variable _getoptarg_, defined in the header file "_get_opt.h_", e.g.:

                              int c;
                              char *parsed;
                              while ((c = get_opt(argc, argv)) != -1)
                              {
                                switch (c)
                                {
                                  ...
                                  case 'c':
                                    parsed = getoptarg;
                                  break;
                                  ...
                                }
                              }
                              ...
                              
3. Finally, call a call to _get_opt_free_ shall free all the allocated resources by the parsing.


Enjoy! :)


----------------------------------------------
Author: Santiago Pagola
Email: santipagola@gmail.com

Please write to me if you have some improvement tips, you will be properly mentioned!
