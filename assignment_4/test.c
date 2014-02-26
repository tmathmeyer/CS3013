#include "mailbox.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SEND 349
#define RECV 350
#define MANG 351


int main()
{
	int msgs;
	printf("syscall create: %ld\n", syscall(MANG, 0, &msgs));
	printf("there were %i messages\n", msgs);



	char* voidstar = "hi there!!";
	printf("syscall result: %ld\n", syscall(SEND, getpid(), (void*)voidstar, 10, false));


	pid_t sender;
	char* message = (char*)malloc(MAX_MSG_SIZE);
	int length;
	printf("syscall create: %ld\n", syscall(RECV, &sender, message, &length));
	printf("the message was %i chars long\n", length);
	printf("and the message was: %s\n", message);




	voidstar = "fuck everything and jump off a cliff";
	printf("syscall result: %ld\n", syscall(SEND, getpid(), (void*)voidstar, 36, false));



	printf("syscall create: %ld\n", syscall(RECV, &sender, message, &length));
	printf("the message was %i chars long\n", length);
	printf("and the message was: %s\n", message);
}
