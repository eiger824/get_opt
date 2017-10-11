MACRO=-DGET_OPT_DEBUG_
GDB=-g
ifeq ($(DEBUG),0)
	MACRO=
	GDB=
endif

all: test_c_long \
	test_c_short \
	test_cpp_short

test_c_long: main.c get_opt.h
	gcc -Wall -Wextra -Wpedantic ${MACRO} ${GDB} -I. main.c -o $@

test_c_short: main2.c get_opt.h
	gcc -Wall -Wextra -Wpedantic ${MACRO} ${GDB} -I. main2.c -o $@

test_cpp_short: main.cpp get_opt.h
	g++ -Wall -Wextra -Wpedantic --std=c++1y ${MACRO} ${GDB} -I. main.cpp -o $@

clean:
	rm -f test_c_* test_cpp_* *~
