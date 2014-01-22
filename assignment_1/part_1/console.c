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


int main()
{
	int continuation = 1;
	printf("~~$ ");
	char* buffer = (char*)malloc(128);
	int bufferspace = 0;
	char c = 0;
	proc_info *p = get_init();


	while(c != EOF && continuation)
	{
		c = getchar();
		if (c == '\n')
		{
			char** args = split_args(buffer);
			printf("%c%c%c%c\n", args[0][0], args[0][1], args[0][2], args[0][3]);
			printf("%c%c%c%c\n", args[0][0], args[0][1], args[0][2], args[0][3]);

			int val = (args[0][0]=='e')+(args[0][1]=='x')+(args[0][2]=='i')+(args[0][3]=='t');
			printf("so the if statement SHOULD be fine");

			if (val == 4)
			{
				printf("this should be printed, no?");
				continuation = 0;
			}
			else if (strcmp("cd", args[0]) == 0)
			{
				char dir[1024];
				if (args[1])
				{
					chdir(args[1]);
				}
				else
				{
					chdir("/home/ted");
				}

				if (getcwd(dir, sizeof(dir)) != NULL)
				{
					printf("%s\n", dir);
				}
			}
			else
			{
				proc_info **p2 = execute(args, p);
				p = p2[0];

				print_info(p2[1]);
			}

			memset(buffer, 0, 128);
			bufferspace = 0;
		}
		else if (c == 127)
		{
			printf("\b \b\b \b");
		}
		else
		{
			buffer[bufferspace++] = c;
		}
	}

	return 0;
}