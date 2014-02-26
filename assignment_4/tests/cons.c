

#include "mailbox.h"
#include <stdio.h>
#include <string.h>

int main()
{
	char msg[MAX_MSG_SIZE];
	int proc = 1;

	while(proc)
	{
		scanf ("%d",&proc);
		scanf ("%s",msg);

		if (proc && SendMsg(proc, msg, strlen(msg), NO_BLOCK))
		{
			printf("error!");
		}
	}
	
}