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
	prinfo* cpi = (prinfo*)(malloc(sizeof(prinfo)));
	printf("syscall result: %ld\n", syscall(__NR_cs3013_syscall1, cpi));\

	sleep(5);

	printf("╔═══════════════════════════════╗\n");
	printf("║proc_state: %ld\t\t\t║\n", cpi -> state);
	printf("║proc_id:    %i\t\t║\n", cpi -> pid);
	printf("║parent_id:  %i\t\t║\n", cpi -> parent_pid);
	printf("║y_child_id: %i\t\t║\n", cpi -> youngest_child);
	printf("║y_sibl_in:  %i\t\t\t║\n", cpi -> younger_sibling);
	printf("║o_sibl_in:  %i\t\t\t║\n", cpi -> older_sibling);
	printf("║user_id:    %i\t\t║\n", cpi -> uid);
	printf("║start_time: %lld\t║\n", cpi -> start_time);
	printf("║user_time:  %lld\t\t\t║\n", cpi -> user_time);
	printf("║sys_time:   %lld\t\t\t║\n", cpi -> sys_time);
	printf("║cu_time:    %lld\t\t\t║\n", cpi -> cutime);
	printf("║cs_time:    %lld\t\t\t║\n", cpi -> cstime);
	printf("╚═══════════════════════╝\n");


}
