KERNEL_HEADERS = /lib/modules/$(shell uname -r)/build
#KERNEL_HEADERS = /usr/src/linux
CC = gcc

TARGET := app
obj-m := my_drive.o

all:
	@$(MAKE) -C $(KERNEL_HEADERS) M=$(PWD) modules

app: 
	$(CC) -o $@ $@.c

clean:
	@$(MAKE) -C $(KERNEL_HEADERS) M=$(PWD) clean
	rm -f *.o $(TARGET)
