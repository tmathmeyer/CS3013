// Ted Meyer

#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>

#include "proc_info_mgr.h"
#include "doit.h"

proc_info** execute(char** exec, proc_info* status)
{
	struct rusage *r = (struct rusage*)(malloc(sizeof(struct rusage)));

	int pid = fork();
	inject_time(status);
	if (pid == 0)
	{ // this is the child process
		execvp(exec[0], exec);
	}
	else
	{ // this is the parent process
		wait(pid);
		getrusage(RUSAGE_SELF, r);
		return get_proc_info(r, status);
	}
}