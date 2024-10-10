A5: A5.c
	gcc -o a5 A5.c

a5: A5
	./a5

clean: # remove all machine-generated files
	rm -f a5 *.o memcheck.txt *~