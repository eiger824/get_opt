#include <stdio.h>
#include "get_opt.h"

int main(int argc, char* argv[])
{
	int c;
        //while ((c = get_opt(argc, argv, "all;block;coming-from*;delegate;enable+")) != -1);
        get_opt_set_flags("abc:de:");
	while ((c = get_opt(argc, argv)) != -1)
	{
		//todo
		printf("Doing stuff\n");
	}
        free_get_opt();
	return 0;
}
