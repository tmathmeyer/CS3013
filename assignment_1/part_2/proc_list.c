//Ted Meyer

#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>

#include "proc_list.h"
#include "proc_info_mgr.h"


//typedef struct proc_list {
//	proc_list* next;        //0 if none
//	proc_info* start_info;  //initial stats on the process
//	pid_t pid;              //process id
//} proc_list;

//adds a new process to the stack
proc_list* exec(char** args, proc_list* current)
{
	proc_list* result = (proc_list*)(malloc(sizeof(proc_list)));
	int pid = fork();
	if (pid == 0)
	{
		execvp(args[0], args);
		return 0;
	}
	else
	{
		printf("[%i] %i\n", ++current_process, pid);

		result -> start_info = get_init();
		result -> next       = current;
		result -> pid        = pid;
		result -> my_id      = current_process;
		

		return result;
	}
}


//removes all finished processessessessess from the stack and prints their information
proc_list* prune(proc_list* current)
{
	proc_list* new_top = 0;
	struct rusage *r = (struct rusage*)(malloc(sizeof(struct rusage)));
	while(current != 0)
	{
		if (wait4(current->pid, NULL, WNOHANG, r))
		{ // this process has finished!!!
			printf("[%i] %i\n", current->my_id, current->pid);
			proc_info* info = get_proc_info(r, current->start_info);
			print_info(info);
			free(info);
			free(r);
			r = (struct rusage*)(malloc(sizeof(struct rusage)));
			current = current->next;
		}
		else
		{ // no process has finished, put me back into tree
			proc_list* next = current->next;
			current->next = new_top;
			new_top = current;
			current = next;
		}
	}

	return new_top;
}

//prints all processes
void print_all(proc_list* current)
{
	printf("╔═processes═════╗\n");
	while(current != 0)
	{
		printf("║[%i] %i\t║\n", current->my_id, current->pid);
		current = current->next;
	}
	printf("╚═══════════════╝\n");
}