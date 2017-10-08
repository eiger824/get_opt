MACRO=-DGET_OPT_DEBUG_
GDB=-g
ifeq ($(DEBUG),0)
	MACRO=
	GDB=
endif

all: test_in_c test_in_cpp

test_in_c: main.c get_opt.h
	gcc -Wall -Wextra -Wpedantic ${MACRO} ${GDB} -I. main.c -o test_in_c

test_in_cpp: main.cpp get_opt.h
	g++ -Wall -Wextra -Wpedantic --std=c++1y ${MACRO} ${GDB} -I. main.cpp -o test_in_cpp


clean:
	rm -f test_in_c test_in_cpp *~
