//Ted Meyer

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "proc_info_mgr.h"
#include "doit.h"

char** split_args(char* buffer)
{
	char* clone;
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
		chdir("~");
	}

	if (getcwd(dir, sizeof(dir)) != NULL)
	{
		printf("%s\n", dir);
	}
}

int main()
{
	printf("~~$ ");
	char* buffer = (char*)malloc(128);
	int bufferspace = 0;
	char c = 0;
	proc_info *p = get_init();


	while(c != EOF )
	{
		c = getchar();
		if (c == '\n')
		{
			char** args = split_args(buffer);

			if (strcmp("exit", args[0]) == 0)
			{
				return 0;
			}
			else if (strcmp("cd", args[0]) == 0)
			{
				change_dir(args[1]);
			}
			else
			{
				proc_info **p2 = execute(args, p);
				p = p2[0];
				print_info(p2[1]);
				free(p2);
			}

			memset(buffer, 0, 128);
			bufferspace = 0;
		}
		else
		{
			buffer[bufferspace++] = c;
		}
	}

	return 0;
}
