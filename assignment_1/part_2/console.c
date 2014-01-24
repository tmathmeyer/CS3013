//Ted Meyer

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "doit.c"      // for forground tasks
#include "proc_info_mgr.h"
#include "proc_list.h" // for background tasks
#include "async_io.h"  // for asynchronous io

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
	int bufferspace = 0;
	int exited      = 0;
	pthread_t thread;
	int pipes[2];

	init(pipes, &thread);
	int background = 0;

	char* buffer = (char*)malloc(128);
	proc_list* running = 0;

	memset(buffer, 0, 128);

	//char** args = (char**)(malloc(sizeof(char*)));
	//*args = "ls";
	
	//running = exec(args, running);
	//running = exec(args, running);
	//running = exec(args, running);
	//running = exec(args, running);
	//running = exec(args, running);
	//running = exec(args, running);
	//running = exec(args, running);

	//print_all(running);

	while(running || exited==0)
	{
		char c = getchar_as();
		if (c == '\n')
		{
			if (bufferspace == 0)
			{
				printf("~~$");
			}
			else
			{
				char** args = split_args(buffer);
				if (strcmp("exit", args[0]) == 0)
				{
					exited = 0;
					printf("waiting for processess to terminate\n");
				}
				else if (strcmp("cd", args[0]) == 0)
				{
					change_dir(args[1]);
				}
				else
				{
					if (background)
					{
						running = exec(args, running); //background
					}
					else
					{
						proc_info* start = get_init();
						proc_info* result = exec_forg(args, start); //forground
						print_info(result);
						free(start);
						free(result);
					}
				}
			}
		}
		else if (c != -2)
		{
			buffer[bufferspace++] = c;
			background = (c == '&');
		}






		running = prune(running);
	}
	

	return 0;
}
