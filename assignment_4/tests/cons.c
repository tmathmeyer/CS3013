

#include "mailbox.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


#define SENDCOUNT 10
#define PAUSETIME 1000

int main()
{
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
	



	printf("trying to read replies!\n");
	char* rmsg;
	pid_t sender;
	int len;
	for(i=0;i<SENDCOUNT;i++)
	{
		rmsg = (char*)malloc(MAX_MSG_SIZE);
		if ((err = RcvMsg(&sender, (void*)rmsg, &len, BLOCK)))
		{
			printf("error: %i\n", err);
			i = SENDCOUNT;
		}
		else
		{
			printf("mail sent back from: %i\n", (int)sender);
			printf("contents:\n\t%s\n", rmsg);
		}
		free(rmsg);
	}







	return 0;
}