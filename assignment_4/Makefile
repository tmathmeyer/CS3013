
obj-m += postal_service.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) KBUILD_EXTMOD=`pwd` modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
