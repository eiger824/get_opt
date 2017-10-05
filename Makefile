test: main.c 
	gcc -Wall -Wextra -Wpedantic -I. main.c -o test

clean:
	rm -f test *~
