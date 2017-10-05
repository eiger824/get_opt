test: main.c get_opt.h 
	gcc -Wall -Wextra -Wpedantic -DGET_OPT_DEBUG_ -I. main.c -o test

clean:
	rm -f test *~
