all:
	gcc -c collatz.c -Wall
	ar rcs libcollatz.a collatz.o
	gcc client.c libcollatz.a -o collatz_stat -Wall

	gcc -fPIC -c collatz.c -Wall
	gcc -shared collatz.o -o libcollatz.so -Wall
	gcc client.c -L. -lcollatz -Wl,-rpath=. -o collatz_shared -Wall

	gcc client_dynamic.c -o collatz_dynamic -Wall

.PHONY: clean
clean:
	rm -f collatz_stat collatz_shared collatz_dynamic
	rm -f collatz.o libcollatz.a libcollatz.so
