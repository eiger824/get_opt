MACRO=
ifeq ($(DEBUG),1)
	MACRO=-DGET_OPT_DEBUG_
endif

all: test_in_c test_in_cpp

test_in_c: main.c get_opt.h
	gcc -Wall -Wextra -Wpedantic ${MACRO} -I. main.c -o test_in_c

test_in_cpp: main.cpp get_opt.h
	g++ -Wall -Wextra -Wpedantic --std=c++1y ${MACRO} -I. main.cpp -o test_in_cpp


clean:
	rm -f test_in_c test_in_cpp *~
