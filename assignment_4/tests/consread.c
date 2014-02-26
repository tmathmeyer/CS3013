

#include "mailbox.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	pid_t send;
	int len;
	char* msg;

	while(1)
	{
		msg = (char*)malloc(MAX_MSG_SIZE);
		if (RcvMsg(&send, (void*)msg, &len, BLOCK))
		{
			printf("error!\n");
		}
		else
		{
			printf("process id:%i, sent:\n\t%s\n", (int)send, msg)
		}
		free(msg);
	}
	
}