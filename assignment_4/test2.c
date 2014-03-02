#include "mailbox.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main()
{
	int msgs;
	printf("syscall create: %ld\n", ManageMailbox(0, &msgs));
	printf("there were %i messages\n", msgs);



	char* voidstar = "hi there!!";
	printf("syscall result: %ld\n", SendMsg(getpid(), (void*)voidstar, 10, false));

	voidstar = "fuck everything and jump off a cliff";
	printf("syscall result: %ld\n", SendMsg(getpid(), (void*)voidstar, 36, false));



	pid_t sender;
	char* message = (char*)malloc(MAX_MSG_SIZE);
	int length;
	printf("syscall create: %ld\n", RcvMsg(&sender, message, &length));
	printf("the message was %i chars long\n", length);
	printf("and the message was: %s\n", message);


	printf("syscall create: %ld\n", RcvMsg(&sender, message, &length));
	printf("the message was %i chars long\n", length);
	printf("and the message was: %s\n", message);
}
