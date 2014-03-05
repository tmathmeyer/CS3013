

#include "mailbox.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>


#define SENDCOUNT 10
#define PAUSETIME 1000
void *readmsgs();

int main()
{
	pthread_t newThread;
	pthread_create(&newThread, NULL, readmsgs, NULL);
	
	int proc = 0;
	printf("please enter the process to send to \n");
	scanf ("%d",&proc);
	char* msg = "hello recieving processes!";
	int i = 0;
	int err = 0;


	for(;i<SENDCOUNT;i++)
	{
		if ((err = SendMsg(proc, msg, strlen(msg), NO_BLOCK)))
		{
			printf("error: %i\n", err);
			i = SENDCOUNT;
		}
		usleep(PAUSETIME);
	}
	
	return 0;
}


void *readmsgs()
{
	printf("trying to read replies!\n");
	pid_t sender;
	char* msg;
	int len, err, i=0;

    for(;i<1;i++)
	{
		msg = (char*)malloc(MAX_MSG_SIZE);
		if ((err = RcvMsg(&sender, (void*)msg, &len, BLOCK)))
		{
			printf("error: %i\n", err);
			i = SENDCOUNT;
		}
		else
		{
			printf("mail send back from: %i\n", (int)sender);
			printf("contents:\n\t%s\n", msg);
		}
		free(msg);
	}
	return 0;
}