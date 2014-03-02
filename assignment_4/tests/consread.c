

#include "mailbox.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	pid_t send;
	int len, zz;
	char* msg;

    printf("syscallmng: %i\n", ManageMailbox(false, &len));
    printf("proc_id: %i\n", (int)getpid());

    sleep(2);

	while(1)
	{
		printf("waiting\n");
		msg = (char*)malloc(MAX_MSG_SIZE);
		if (zz = RcvMsg(&send, (void*)msg, &len, BLOCK))
		{
			printf("error wtf! %i\n", zz);
		}
		else
		{
			printf("process id:%i, sent:\n\t%s\n", (int)send, msg);
		}
		free(msg);
	}
	
}
