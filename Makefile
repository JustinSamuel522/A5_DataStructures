a5: A5.c
	gcc -o a5 A5.c

run: A5
	./a5 points.txt

clean: # remove all machine-generated files
	rm -f a5 *.o memcheck.txt *~