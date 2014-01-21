// Ted Meyer

#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "proc_info_mgr.h"

int main(int argc, char** argv)
{
	//init structs
	struct rusage usage;
	struct rusage *p = &usage;
	getrusage(RUSAGE_SELF, p);
	process(p, "lalalalalala");


	//execute code
	int fork_id = fork();

	if (fork_id == 0)
	{ // this is the child process
		char** forked_args = argv+1;
		execvp(forked_args[0], forked_args);
	} 
	else 
	{ // this is the parent process
		int exit_code = wait(fork_id);
		while (exit_code > 0);
		printf(
			   "process with id: %i exited with error code %i\n",
			   fork_id,
			   exit_code
		);

		getrusage(RUSAGE_SELF, p);
		process(p, "lalalalalala");
	}
}