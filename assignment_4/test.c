#include "mailbox.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SEND 349
#define RECV 350
#define MANG 351


int main()
{
	char* voidstar = "hi there!!";
	printf("syscall result: %ld\n", syscall(SEND, getpid(), (void*)voidstar, 10, false));
}