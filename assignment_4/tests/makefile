# Ted Meyer
# Makefile
# GPLv2


all: test5 test6 test7 cons consread


mailbox.o: mailbox.c mailbox.h
	gcc -Wall -c mailbox.c


cons: mailbox.o cons.c
	gcc -Wall cons.c mailbox.o -o cons

consread: mailbox.o consread.c
	gcc -Wall consread.c mailbox.o -o consread

test5: mailbox.o testmailbox5.c
	gcc -Wall testmailbox5.c mailbox.o -o test5

test6: mailbox.o testmailbox6.c
	gcc -Wall testmailbox6.c mailbox.o -o test6

test7: mailbox.o testmailbox7.c
	gcc -Wall testmailbox7.c mailbox.o -o test7








clean:
	rm *.o test5 test6 test7 cons consread
