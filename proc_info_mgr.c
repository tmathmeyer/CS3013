//Ted Meyer
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> 
#include "proc_info_mgr.h"

//typedef struct proc_info {
//   int real_time;         //usertime???
//   int cpu_time;          //systemtime???
//   int count_preempted;   //incoluntary context switches
//   int count_cpu_yeild;   //voluntary context switches
//   int count_page_fault;  //page faults
//   int count_pf_kcache;   //page reclaims???
//} proc_info;


// included from proc_info_mgr.h
proc_info** get_proc_info(struct rusage *usage, proc_info *shell)
{
	proc_info* exec = (proc_info*) (malloc(sizeof(struct proc_info)));
	
	exec -> count_preempted = 
			shell -> count_preempted - 
			usage -> ru_nivcsw;
	exec -> count_cpu_yeild = 
			shell -> count_cpu_yeild -
			usage -> ru_nvcsw;
}


void process(struct rusage *p, char *when)
{
	printf("%s\n", when);
	printf(" /* user time used */                   %8d  %8d\n",  p->ru_utime.tv_sec,p->ru_utime.tv_usec   );
	printf(" /* system time used */                 %8d  %8d\n",  p->ru_stime.tv_sec,p->ru_stime.tv_usec   );
	printf(" /* integral shared memory size */      %8d\n",  p->ru_ixrss           );
	printf(" /* integral unshared data  */          %8d\n",  p->ru_idrss           );
	printf(" /* integral unshared stack  */         %8d\n",  p->ru_isrss           );
	printf(" /* page reclaims */                    %8d\n",  p->ru_minflt          );
	printf(" /* page faults */                      %8d\n",  p->ru_majflt          );
	printf(" /* swaps */                            %8d\n",  p->ru_nswap           );
	printf(" /* block input operations */           %8d\n",  p->ru_inblock         );
	printf(" /* block output operations */          %8d\n",  p->ru_oublock         );
	printf(" /* messages sent */                    %8d\n",  p->ru_msgsnd          );
	printf(" /* messages received */                %8d\n",  p->ru_msgrcv          );
	printf(" /* signals received */                 %8d\n",  p->ru_nsignals        );
	printf(" /* voluntary context switches */       %8d\n",  p->ru_nvcsw           );
	printf(" /* involuntary  */                     %8d\n",  p->ru_nivcsw          );
}