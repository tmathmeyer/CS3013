

#include "mailbox.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
	pid_t sender;
	int len, zz;
	char* msg;
	char* send = "I got your mail!";
	
    printf("proc_id: %i\n", (int)getpid());

	do
	{
		printf("waiting\n");
		msg = (char*)malloc(MAX_MSG_SIZE);
		if (zz = RcvMsg(&sender, (void*)msg, &len, BLOCK))
		{
			printf("error: %i\n", zz);
			return;
		}
		else if (!strncmp(msg, "kill", 4))
		{
			return;
		}
		else
		{
			printf("process id:%i, sent:\n\t%s\n", (int)sender, msg);
			if (zz = SendMsg(sender, send, strlen(send), NO_BLOCK))
			{
				printf("error: %i\n", zz);
			}
		}
		free(msg);
	}
	while(1);
}
