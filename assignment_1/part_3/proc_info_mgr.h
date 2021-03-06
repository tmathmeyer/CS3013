//Ted Meyer
#ifndef PROC_INFO_MGR_H
#define PROC_INFO_MGR_H

#include <sys/resource.h>

typedef struct proc_info {
   int err_code;		  //my own thing
   long long real_time;   //usertime???
   int cpu_time;          //systemtime???
   int count_preempted;   //incoluntary context switches
   int count_cpu_yeild;   //voluntary context switches
   int count_page_fault;  //page faults
   int count_pf_kcache;   //page reclaims???
} proc_info;

// result[0] = the totals for the shell
// result[1] = the totals for the studied process
proc_info* get_proc_info(struct rusage *usage, proc_info *shell);

// get process info corresponding to NOW
proc_info* get_init();

// add the time to a proc_info
proc_info* inject_time(proc_info* inf);

// print info from the proc_info
void print_info(proc_info* p);


#endif