//Ted Meyer

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
		*(writer++) = toks;
		toks = strtok (NULL, " ");
	}
	free(toks);

	return result;
}


int main()
{
	printf("~~$ ");
	char* buffer = (char*)malloc(128);
	int bufferspace = 0;
	char c = 0;
	proc_info *p = get_init();


	while(c != EOF)
	{
		c = getchar();
		if (c == '\n')
		{
			char** qq = split_args(buffer);

			
			proc_info **p2 = execute(qq, p);
			p = p2[0];

			print_info(p2[1]);

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
}