

#include "mailbox.h"
#include <stdio.h>
#include <string.h>

#define SENDCOUNT 10

int main()
{
	pthread_t newThread;
	pthread_create(&newThread, NULL, readmsgs);
	
	int proc = 0;
	printf("please enter the process to send to \n");
	scanf ("%d",&proc);
	char* msg = "hello recieving processes!";
	int i = 0;
	int err = 0;


	for(;i<SENDCOUNT;i++)
	{
		if (err = SendMsg(proc, msg, strlen(msg), NO_BLOCK))
		{
			printf("error: %i\n", err);
			i = SENDCOUNT;
		}
	}
	
}


void *readmsgs()
{
	pid_t sender;
	char* msg;
	int len, err, i=0;

    for(;i<SENDCOUNT;i++)
	{
		if (err = RcvMsg(&sender, (void*)msg, &len, BLOCK))
		{
			printf("error: %i\n", err);
			i = SENDCOUNT;
		}
		else
		{
			printf("mail send back from: %i\n", (int)sender);
			printf("contents:\n\t%s\n", msg);
		}
	}
}