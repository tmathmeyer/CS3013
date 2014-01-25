# Ted Meyer
# 21/01/2014
# make all the parts of the assignment. can also use makefiles in the subdirs


all: doit shell shell2

doit: part_1/doit.c part_1/proc_info_mgr.c part_1/proc_info_mgr.h
	cd part_1 && \
	gcc -o doit doit.c proc_info_mgr.c && \
	mv doit ../ && \
	rm -f *.o

shell: 2_proc_info_mgr.o 2_doit.o 2_console.o
	cd part_2 && \
	gcc proc_info_mgr.o doit.o console.o -o console && \
	mv console ../shell && \
	rm -f *.o

shell2: 3_proc_info_mgr.o 3_doit.o 3_console.o 3_proc_list.o
	cd part_3 && \
	gcc proc_info_mgr.o doit.o proc_list.o console.o -o console -lpthread && \
	mv console ../shell2 && \
	rm -f *.o



2_console.o: part_2/console.c
	cd part_2 && \
	gcc -c console.c

2_proc_info_mgr.o: part_2/proc_info_mgr.c part_2/proc_info_mgr.h
	cd part_2 && \
	gcc -c proc_info_mgr.c

2_doit.o: part_2/doit.c part_2/doit.h
	cd part_2 && \
	gcc -c doit.c 


3_console.o: part_3/console.c
	cd part_3 && \
	gcc -c console.c

3_proc_list.o: part_3/proc_list.c part_3/proc_list.h
	cd part_3 && \
	gcc -c proc_list.c

3_proc_info_mgr.o: part_3/proc_info_mgr.c part_3/proc_info_mgr.h
	cd part_3 && \
	gcc -c proc_info_mgr.c

3_doit.o: part_3/doit.c part_3/doit.h
	cd part_3 && \
	gcc -c doit.c 


clean:
	rm -f doit shell shell2
