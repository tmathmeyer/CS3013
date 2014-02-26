# Ted Meyer
# Makefile
# GPLv2


all: test5


mailbox.o: mailbox.c mailbox.h
	gcc -Wall -c mailbox.c

testmailbox5.o:
	gcc -Wall -c testmailbox5.c




test5: mailbox.o testmailbox5.o
	gcc -Wall testmailbox5.o mailbox.o -o test5








clean:
	rm *.o test5
