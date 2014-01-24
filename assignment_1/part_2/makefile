
# Ted Meyer
# 21/01/2014

main: proc_info_mgr.o doit.o console.o
	gcc proc_info_mgr.o doit.o console.o -o console
	rm -f *.o

test: async_io.c
	gcc -lpthread async_io.c

console.o: console.c
	gcc -c console.c

proc_info_mgr.o: proc_info_mgr.c proc_info_mgr.h
	gcc -c proc_info_mgr.c

doit.o: doit.c doit.h
	gcc -c doit.c

clean:
	rm -f console
