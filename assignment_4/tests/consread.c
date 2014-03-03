

#include "mailbox.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	pid_t send;
	int len, zz;
	char* msg;

	do
	{
		printf("waiting\n");
		msg = (char*)malloc(MAX_MSG_SIZE);
		if (zz = RcvMsg(&send, (void*)msg, &len, BLOCK))
		{
			printf("error wtf! %i\n", zz);
			sleep(2);
		}
		else
		{
			printf("process id:%i, sent:\n\t%s\n", (int)send, msg);
		}
		free(msg);
	}
	while(1);
}
