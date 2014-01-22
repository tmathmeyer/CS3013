//Ted Meyer
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> 
#include <sys/time.h>
#include "proc_info_mgr.h"


//typedef struct proc_info {
//   long real_time;         //usertime???
//   long cpu_time;          //systemtime???
//   int count_preempted;   //incoluntary context switches
//   int count_cpu_yeild;   //voluntary context switches
//   int count_page_fault;  //page faults
//   int count_pf_kcache;   //page reclaims???
//} proc_info;



void print_info(proc_info* p)
{
	printf("\n");
	printf("╔═══════════════════════╗\n");
	printf("║clock time (µs):  %i\t║\n", p->real_time);
	printf("║cpu time (µs):    %i\t║\n", p->cpu_time);
	printf("║context switches: \t║\n");
	printf("║    voluntary:    %i\t║\n", p->count_preempted);
	printf("║    involntary:   %i\t║\n", p->count_cpu_yeild);
	printf("║page faults:      %i\t║\n", p->count_page_fault);
	printf("║page reclaims:    %i\t║\n", p->count_pf_kcache);
	printf("╚═══════════════════════╝\n");
	
}

proc_info** get_proc_info(struct rusage *usage, proc_info *shell)
{
	//init struct data
	proc_info* exec = (proc_info*) (malloc(sizeof(struct proc_info)));
	struct timeval t;
	gettimeofday(&t, 0);
	long elapsed = t.tv_sec*1000000 + t.tv_usec;

	//write the stastics
	exec -> count_preempted = 0
			- shell -> count_preempted + 
			usage -> ru_nivcsw;
	exec -> count_cpu_yeild = 0
			- shell -> count_cpu_yeild +
			usage -> ru_nvcsw;
	exec -> count_page_fault = 0
			- shell -> count_page_fault +
			usage -> ru_majflt;
	exec -> count_pf_kcache = 0
			- shell -> count_pf_kcache +
			usage -> ru_minflt;
	exec -> real_time = 
			elapsed - 
			shell -> real_time;

	//overwrite the status numbers
	shell -> count_preempted = 
			 usage -> ru_nivcsw;
	shell -> count_cpu_yeild = 
			 usage -> ru_nvcsw;
	shell -> count_page_fault = 
			 usage -> ru_majflt;
	shell -> count_pf_kcache = 
			 usage -> ru_minflt;
	shell -> real_time = elapsed;

	proc_info** result = (proc_info**)(malloc(2* sizeof(proc_info*)));
	*result = shell;
	*(result+1) = exec;

	return result;
}

proc_info* get_init()
{
	proc_info* init = (proc_info*) (malloc(sizeof(struct proc_info)));
	struct timeval t;
	gettimeofday(&t, 0);
	init->real_time = t.tv_sec*1000000 + t.tv_usec;
	return init;
}

