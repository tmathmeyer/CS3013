//Ted Meyer

#ifndef PROC_LIST_H
#define PROC_LIST_H

#include "proc_info_mgr.h"

typedef struct proc_list {
	struct proc_list* next;   //0 if none
	proc_info* start_info;    //initial stats on the process
	pid_t pid;                //process id
	int my_id;                //internal process id numbering
} proc_list;

//adds a new process to the stack
proc_list* exec(char** args, proc_list* current);

//removes all finished processessessessess from the stack and prints their information
proc_list* prune(proc_list* current);

//prints all processes
void print_all(proc_list* current);

#endif







#ifndef PROC_COUNT
#define PROC_COUNT

//stores current process number
int current_process;

#endif
