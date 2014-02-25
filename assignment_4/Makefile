
obj-m += postal_service.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) KBUILD_EXTMOD=`pwd` modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test: hashmap
	gcc -o test hashtest.c hashmap.o

hashmap: hashmap.c hashmap.h
	gcc -Wall -c hashmap.c
