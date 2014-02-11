all: bathroom.o manager.o
	gcc -Wall -lm -lpthread -o bathroom bathroom.o manager.o

manager: manager.c bathroom.h
	gcc -Wall -lm -lpthread manager.c bathroom.h

bathroom: bathroom.c bathroom.h
	gcc -Wall -gc bathroom.c



clean:
	rm *.o bathroom
