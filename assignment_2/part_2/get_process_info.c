#include "prinfo.h"

#include <stdlib.h>
#include <stdio.h>

#define __NR_cs3013_syscall1 349

//good goy why is this not typedef'd to begin with?? we're not in the 70's anymore
//typedef struct prinfo {
//    long state;            // current state of process
//    pid_t pid;             // process ID of this process
//    pid_t parent_pid;      // process ID of parent
//    pid_t youngest_child;  // process ID of youngest child
//    pid_t younger_sibling; // process ID of next younger sibling
//    pid_t older_sibling;   // process ID of next older sibling
//    uid_t uid;             // user ID of process owner
//    long long start_time;  // process start time in nanosecond since boot time
//    long long user_time;   // CPU time spent in user mode (microseconds)
//    long long sys_time;    // CPU time spend in system mode (microseconds)
//    long long cutime;      // total user time of children (microseconds)
//    long long cstime;      // total system time of children (microseconds)
//} prinfo;                  // struct prinfo



int main(int argc, char** argv)
{
	prinfo* cpi;
	printf("syscall result: %i\n", syscall(__NR_cs3013_syscall1, cpi));

	printf("╔═══════════════════════╗\n");
	printf("║proc_state: %ld\n", cpi -> state);
	printf("║proc_id: %i\n", cpi -> pid);
	printf("║%i\n", cpi -> parent_pid);
	printf("║%i\n", cpi -> youngest_child);
	printf("║%i\n", cpi -> younger_sibling);
	printf("║%i\n", cpi -> older_sibling);
	printf("║%i\n", cpi -> uid);
	printf("║%lld\n", cpi -> start_time);
	printf("║%lld\n", cpi -> user_time);
	printf("║%lld\n", cpi -> sys_time);
	printf("║%lld\n", cpi -> cutime);
	printf("║%lld\n", cpi -> cstime);
	printf("╚═══════════════════════╝\n");


}