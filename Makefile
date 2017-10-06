MACRO=-DGET_OPT_DEBUG_
ifeq ($(DEBUG),0) 
	MACRO=
endif

all: test test2

test: main.c get_opt.h 
	gcc -Wall -Wextra -Wpedantic ${MACRO} -I. main.c -o test

test2: main.cpp get_opt.h
	g++ -Wall -Wextra -Wpedantic --std=c++1y ${MACRO} -I. main.cpp -o test2
	

clean:
	rm -f test test2 *~
