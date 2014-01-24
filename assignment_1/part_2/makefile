
# Ted Meyer
# 21/01/2014

all: doit.o proc_info_mgr.o proc_list.o console.o async_io.o
	gcc proc_info_mgr.o doit.o proc_list.o console.o async_io.o -o console -lpthread
	rm -f *.o

async_io.o: async_io.c async_io.h
	gcc -c -lpthread async_io.c

console.o: console.c
	gcc -c -g -O0 console.c

proc_info_mgr.o: proc_info_mgr.c proc_info_mgr.h
	gcc -c proc_info_mgr.c

proc_list.o: proc_list.c proc_list.h
	gcc -c proc_list.c

doit.o: doit.c doit.h
	gcc -c doit.c

clean:
	rm -f console *.o
