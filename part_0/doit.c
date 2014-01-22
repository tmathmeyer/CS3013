// Ted Meyer

#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "proc_info_mgr.h"

proc_info** execute(char** exec, proc_info* status);

int main(int argc, char** argv)
{
	proc_info *p = get_init();
	proc_info **p2 = execute(argv+1, p);
	p = p2[0];

	print_info(p2[1]);
}

proc_info** execute(char** exec, proc_info* status)
{
	struct rusage *r = (struct rusage*)(malloc(sizeof(struct rusage)));

	int pid = fork();
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