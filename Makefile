MACRO=-DGET_OPT_DEBUG_
ifeq ($(DEBUG),0) 
	MACRO=
endif

test: main.c get_opt.h 
	gcc -Wall -Wextra -Wpedantic ${MACRO} -I. main.c -o test

clean:
	rm -f test *~
