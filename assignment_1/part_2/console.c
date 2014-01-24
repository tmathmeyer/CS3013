//Ted Meyer

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "proc_info_mgr.h"
#include "proc_list.h"

char** split_args(char* buffer)
{
	char* clone = (char*)(malloc(128));
	int   count = 0;
	strncpy(clone, buffer, 128);

	char* toks = strtok (clone," ");
	while (toks != NULL)
	{
		count++;
		toks = strtok (NULL, " ");
	}
	free(toks);

	char** result = (char**)(malloc(sizeof(char*) * count));
	char** writer = result;

	toks = strtok (buffer," ");
	while (toks != NULL)
	{
		char* arg = (char*)(malloc(sizeof(char) * (strlen(toks) + 1)));
		strcpy(arg, toks);
		strcat(arg, "\0");
		*(writer++) = arg;
		toks = strtok (NULL, " ");
	}
	free(toks);

	return result;
}

int change_dir(char* dirname)
{
	char dir[1024];
	if (dirname)
	{
		chdir(dirname);
	}
	else
	{
		chdir("/");
	}

	if (getcwd(dir, sizeof(dir)) != NULL)
	{
		printf("%s\n", dir);
	}
}

int main(int argc, char*argv[])
{
	current_process = 0;
	printf("~~$ \n");
	char* buffer = (char*)malloc(128);
	int bufferspace = 0;
	char** args = (char**)(malloc(sizeof(char*)));
	*args = "ls";

	proc_list* running = exec(args, 0);
	running = exec(args, running);
	running = exec(args, running);
	running = exec(args, running);
	running = exec(args, running);
	running = exec(args, running);
	running = exec(args, running);
	running = exec(args, running);

	print_all(running);

	while(running)
	{
		running = prune(running);
	}
	

	return 0;
}
