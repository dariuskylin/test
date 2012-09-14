all:
	gcc -Wall test.c -g -o test
	valgrind --tool=memcheck --leak-check=full ./test
