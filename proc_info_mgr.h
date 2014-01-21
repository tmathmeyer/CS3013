//Ted Meyer
#include <sys/resource.h>

typedef struct proc_info {
   int real_time;         //usertime???
   int cpu_time;          //systemtime???
   int count_preempted;   //incoluntary context switches
   int count_cpu_yeild;   //voluntary context switches
   int count_page_fault;  //page faults
   int count_pf_kcache;   //page reclaims???
} proc_info;

// result[0] = the totals for the shell
// result[1] = the totals for the studied process
proc_info** get_proc_info(struct rusage *usage, proc_info *shell);
void process(struct rusage *p, char *when);