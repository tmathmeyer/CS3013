

#include "mailbox.h"
#include <stdio.h>
#include <string.h>

#define SENDCOUNT 10

int main()
{
	int proc = 0;
	printf("please enter the process to send to \n");
	scanf ("%d",&proc);
	char* msg = "hello recieving processes!"
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